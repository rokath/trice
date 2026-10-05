#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Export one stopped Codex session; shared helpers also serve the start script.

Only the Python standard library is used. No Git write, network request, credential
copy, process termination, or modification of Codex's SQLite databases is performed.
"""

import argparse
import collections
import contextlib
import datetime as dt
import hashlib
import io
import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import socket
import sqlite3
import subprocess
import sys
import uuid
import zipfile

sys.dont_write_bytecode = True
SCHEMA = 1
MAX_BYTES = 512 * 1024 * 1024
STATE_DIR = "trice-handover"
ARCHIVE_GLOB = "codex-handover-*.zip"


class HandoverError(Exception):
    """An actionable refusal; callers must not bypass a failed precondition."""


def say(message):
    """Flush each step so slow reads/compression never look like a hung script."""
    print(message, flush=True)


def run(args, cwd=None):
    """Run an argument vector without a shell, preserving paths with spaces."""
    args = list(args)
    args[0] = shutil.which(args[0]) or args[0]
    try:
        result = subprocess.run(args, cwd=cwd, capture_output=True, text=True,
                                encoding="utf-8", errors="replace", timeout=60)
    except (OSError, subprocess.TimeoutExpired) as exc:
        raise HandoverError(f"Befehl nicht ausführbar: {args[0]}: {exc}") from exc
    if result.returncode:
        raise HandoverError(f"Befehl fehlgeschlagen: {args[0]}\n{result.stderr.strip()}")
    return result.stdout.strip()


def digest(data):
    """Identify content independently of file dates and computer clocks."""
    return hashlib.sha256(data).hexdigest()


def encoded(value):
    """Use deterministic UTF-8 for our own metadata, never for rewriting rollouts."""
    return (json.dumps(value, ensure_ascii=False, sort_keys=True) + "\n").encode("utf-8")


def read_optional(path):
    """Distinguish an absent file from an existing empty file during rollback."""
    if path.is_symlink():
        raise HandoverError(f"Symlink als Zieldatei nicht unterstützt: {path}")
    if not path.exists():
        return None
    if path.stat().st_size > MAX_BYTES:
        raise HandoverError(f"Datei größer als 512 MiB: {path}")
    before = path.stat()
    data = path.read_bytes()
    after = path.stat()
    if (before.st_size, before.st_mtime_ns, before.st_ino) != (after.st_size, after.st_mtime_ns, after.st_ino):
        raise HandoverError(f"Datei während des Lesens verändert: {path}")
    return data


def rows(data, label):
    """Validate complete JSONL records, retaining original bytes for shared files."""
    if data is None:
        return []
    if data and not data.endswith(b"\n"):
        raise HandoverError(f"Unvollständige letzte JSONL-Zeile in {label}; nichts kopiert.")
    result = []
    for number, line in enumerate(io.BytesIO(data), 1):
        try:
            item = json.loads(line)
            if not isinstance(item, dict):
                raise ValueError("object required")
        except (ValueError, UnicodeError) as exc:
            raise HandoverError(f"Ungültige JSONL-Zeile {number} in {label}.") from exc
        result.append((item, line))
    return result


def canonical_origin(value):
    """Compare SSH/HTTPS spellings without storing credentials from remote URLs."""
    from urllib.parse import urlsplit
    if "://" in value:
        url = urlsplit(value)
        if not url.hostname:
            raise HandoverError("Origin muss eine eindeutige Server-URL haben.")
        host, path = url.hostname, url.path
    else:
        match = re.fullmatch(r"(?:[^@/]+@)?([^/:]+):(.+)", value)
        if not match:
            raise HandoverError("Origin muss eine SSH- oder HTTPS-Server-URL haben.")
        host, path = match.groups()
    return host.lower() + "/" + path.strip("/").removesuffix(".git")


def repository(root):
    """Require a clean, named project; ignored archives remain outside Git."""
    top = Path(run(["git", "rev-parse", "--show-toplevel"], root)).resolve()
    if top != root.resolve():
        raise HandoverError(f"Erwartetes Repository: {root}; erkannt: {top}")
    status = run(["git", "status", "--porcelain=v1", "--untracked-files=all"], root)
    if status:
        raise HandoverError("Git ist nicht clean. Änderungen zuerst prüfen/committen:\n" + status)
    origin = canonical_origin(run(["git", "remote", "get-url", "origin"], root))
    return {"identity": digest(origin.encode()),
            "commit": run(["git", "rev-parse", "HEAD"], root),
            "branch": run(["git", "branch", "--show-current"], root)}


def codex_home():
    """Use native Python's home on Windows, including when launched by Git Bash."""
    home = Path(os.environ.get("CODEX_HOME", str(Path.home() / ".codex"))).expanduser()
    if not home.is_dir():
        raise HandoverError(f"Codex-Verzeichnis fehlt: {home}. Codex zuerst lokal einrichten.")
    return home.resolve()


def codex_version():
    """Reject unknown versions instead of guessing compatibility of local formats."""
    output = run(["codex", "--version"])
    match = re.search(r"\b\d+\.\d+\.\d+(?:[-+][\w.-]+)?", output)
    if not match:
        raise HandoverError("Codex-Version nicht erkennbar: " + output)
    return match.group()


def require_login(home):
    """Check local CLI authentication without printing potentially sensitive output.

    This is only a local status probe, not a model request or token refresh. Use
    the same resolved profile as the subsequent resume, including CODEX_HOME.
    """
    executable = shutil.which("codex")
    if not executable:
        raise HandoverError("codex fehlt im PATH. Codex zuerst lokal installieren.")
    try:
        result = subprocess.run([executable, "login", "status"],
                                env=dict(os.environ, CODEX_HOME=str(home)),
                                capture_output=True, timeout=30)
    except (OSError, subprocess.TimeoutExpired) as exc:
        raise HandoverError("Codex-Anmeldestatus nicht prüfbar. Lokal 'codex login status' aufrufen.") from exc
    if result.returncode:
        raise HandoverError("Codex-Anmeldung nicht bestätigt. Lokal 'codex login' ausführen, "
                            "danach das Startskript erneut aufrufen. Es wurde noch nichts importiert.")
    say("[Anmeldung] Lokale Codex-Anmeldung vorhanden; keine Zugangsdaten übertragen.")


def is_codex_process(name, command=""):
    """Recognize native binaries, app helpers, and the npm launcher, not our scripts."""
    name = name.replace("\\", "/").lower()
    base = name.rsplit("/", 1)[-1]
    if base in {"codex", "codex.exe", "chatgpt", "chatgpt.exe"}:
        return True
    if base.startswith("codex-") or "/codex.app/" in name or "/chatgpt.app/" in name:
        return True
    if base in {"node", "node.exe", "bun", "bun.exe"}:
        command = command.replace("\\", "/").lower()
        return "@openai/codex" in command or "/codex.js" in command
    return False


def active_codex():
    """Conservatively block all local Codex processes, including idle daemons.

    An executable name cannot prove that a daemon will stay idle. No command line
    is printed, because unrelated process arguments may contain credentials.
    """
    found = []
    if os.name == "nt":
        script = ("[Console]::OutputEncoding = [System.Text.UTF8Encoding]::new(); "
                  "Get-CimInstance Win32_Process | "
                  "Select-Object ProcessId,Name,CommandLine | ConvertTo-Json -Compress")
        raw = json.loads(run(["powershell.exe", "-NoProfile", "-NonInteractive", "-Command", script]) or "[]")
        for entry in raw if isinstance(raw, list) else [raw]:
            if is_codex_process(entry["Name"], entry.get("CommandLine") or ""):
                found.append((int(entry["ProcessId"]), entry["Name"]))
    else:
        output = run(["ps", "-axo", "pid=,comm="])
        for line in output.splitlines():
            pid_text, name = line.strip().split(None, 1)
            command = ""
            if Path(name).name in {"node", "bun"}:
                command = run(["ps", "-p", pid_text, "-o", "args="])
            if is_codex_process(name, command):
                found.append((int(pid_text), name))
    return found


def require_quiet():
    """Fail closed if process enumeration itself is unavailable."""
    found = active_codex()
    if found:
        details = "\n".join(f"  PID {pid}: {name}" for pid, name in found)
        raise HandoverError("Codex läuft noch lokal:\n" + details +
                            "\nCLI, App, IDE-Codex und Hintergrunddienst vollständig beenden."
                            "\nNur wenn alle Arbeiten beendet sind: codex app-server daemon stop"
                            "\nEs wird kein Prozess automatisch beendet.")


def state_path(home):
    """Keep handover ownership separate from Codex's own files and databases."""
    return home / STATE_DIR / "state.json"


def load_state(home):
    """A corrupt ownership record must never silently reset an exported session."""
    data = read_optional(state_path(home))
    if data is None:
        return {"schema": SCHEMA, "projects": {}, "sessions": {}}
    try:
        state = json.loads(data)
        if state["schema"] != SCHEMA or not isinstance(state["sessions"], dict) or not isinstance(state["projects"], dict):
            raise ValueError("schema")
        for sid, entry in state["sessions"].items():
            uuid.UUID(sid)
            if entry["status"] not in {"local", "exported"} or not isinstance(entry["size"], int) or entry["size"] < 0:
                raise ValueError("checkpoint")
            for key in ("repo", "sha256"):
                if not re.fullmatch(r"[0-9a-f]{64}", entry[key]):
                    raise ValueError(key)
            for key in ("outgoing", "imported"):
                if not isinstance(entry.get(key, []), list):
                    raise ValueError(key)
                for package_id in entry.get(key, []):
                    uuid.UUID(package_id)
        for sid in state["projects"].values():
            if sid not in state["sessions"]:
                raise ValueError("unknown project session")
        return state
    except (ValueError, KeyError, TypeError, AttributeError) as exc:
        raise HandoverError("Übergabestatus beschädigt; Sicherung prüfen, nicht zurücksetzen.") from exc


@contextlib.contextmanager
def handover_lock(home):
    """Use an OS lock, automatically released on crashes; Codex does not honor it."""
    folder = home / STATE_DIR
    folder.mkdir(parents=True, exist_ok=True)
    lock = folder / "lock"
    fd = os.open(lock, os.O_CREAT | os.O_RDWR, 0o600)
    with os.fdopen(fd, "r+b") as stream:
        if not os.fstat(fd).st_size:
            stream.write(b"\0")
            stream.flush()
        stream.seek(0)
        try:
            if os.name == "nt":
                import msvcrt
                msvcrt.locking(fd, msvcrt.LK_NBLCK, 1)
            else:
                import fcntl
                fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except OSError as exc:
            raise HandoverError(f"Übergabeskript bereits aktiv: {lock}. Den anderen Lauf zuerst beenden lassen.") from exc
        try:
            stream.seek(1)
            stream.write(encoded({"pid": os.getpid(), "computer": socket.gethostname()}))
            stream.truncate()
            stream.flush()
            yield
        finally:
            stream.seek(0)
            if os.name == "nt":
                msvcrt.locking(fd, msvcrt.LK_UNLCK, 1)
            else:
                fcntl.flock(fd, fcntl.LOCK_UN)


def replace_file(path, data):
    """Replace one file atomically; preserve restrictive existing permissions."""
    import tempfile
    path.parent.mkdir(parents=True, exist_ok=True)
    mode = path.stat().st_mode & 0o777 if path.exists() else 0o600
    fd, temporary = tempfile.mkstemp(prefix=".handover-", dir=path.parent)
    try:
        with os.fdopen(fd, "wb") as stream:
            stream.write(data)
            stream.flush()
            os.fsync(stream.fileno())
        os.chmod(temporary, mode)
        os.replace(temporary, path)
    finally:
        Path(temporary).unlink(missing_ok=True)


def safe_target(root, relative):
    """Disallow ZIP/journal traversal and symlink escapes before any replacement."""
    part = PurePosixPath(relative)
    if not relative or part.is_absolute() or ".." in part.parts or "\\" in relative or ":" in relative:
        raise HandoverError("Unzulässiger relativer Pfad: " + relative)
    path = root.joinpath(*part.parts)
    if not path.resolve().is_relative_to(root.resolve()):
        raise HandoverError("Pfad verlässt das erlaubte Verzeichnis: " + relative)
    return path


def recover_transaction(home, root):
    """Undo an interrupted multi-file replacement only where our bytes still match."""
    journal = home / STATE_DIR / "pending.json"
    raw = read_optional(journal)
    if raw is None:
        return
    say("[Wiederherstellung] Unterbrochene Übergabe erkannt; vorherigen Stand prüfen.")
    record = json.loads(raw)
    if record["root"] != str(root):
        raise HandoverError("Offene Übergabe gehört zu einem anderen Repository: " + record["root"])
    entries = []
    for entry in record["files"]:
        target = safe_target(home if entry["area"] == "home" else root, entry["path"])
        current = read_optional(target)
        before = read_optional(safe_target(home, entry["backup"])) if entry["existed"] else None
        if (digest(before) if before is not None else None) != entry["before"]:
            raise HandoverError("Sicherung beschädigt: " + entry["backup"])
        current_hash = digest(current) if current is not None else None
        if current_hash not in {entry["before"], entry["after"]}:
            raise HandoverError(f"Datei wurde nach dem Abbruch anderweitig geändert: {target}; Sicherung bleibt erhalten.")
        entries.append((target, current_hash, entry, before))
    require_quiet()
    for target, current_hash, entry, before in reversed(entries):
        # Recheck each file immediately before rollback; an external writer may
        # have appeared after the initial validation of all journal entries.
        require_quiet()
        current = read_optional(target)
        if (digest(current) if current is not None else None) != current_hash:
            raise HandoverError(f"Datei während der Wiederherstellung geändert: {target}; Sicherung bleibt erhalten.")
        if current_hash == entry["after"]:
            if before is None:
                target.unlink()
            else:
                replace_file(target, before)
    journal.unlink()
    say("[Wiederherstellung] Vorheriger Stand wiederhergestellt; Sicherungen bleiben erhalten.")


def transaction(home, root, changes, guard):
    """Back up all files first, journal the operation, then compare-and-replace.

    A later run recovers a hard interruption. Unexpected concurrent writes are
    never overwritten during rollback; the journal then remains for diagnosis.
    """
    guard()
    token = str(uuid.uuid4())
    backup_root = home / STATE_DIR / "backups" / token
    backup_root.mkdir(parents=True)
    entries = []
    for number, (target, before, after) in enumerate(changes):
        if read_optional(target) != before:
            raise HandoverError(f"Datei inzwischen verändert: {target}; neu beginnen.")
        area = "home" if target.is_relative_to(home) else "repo"
        base = home if area == "home" else root
        relative = target.relative_to(base).as_posix()
        safe_target(base, relative)
        backup = backup_root / str(number)
        if before is not None:
            replace_file(backup, before)
        entries.append({"area": area, "path": relative, "backup": backup.relative_to(home).as_posix(),
                        "existed": before is not None, "before": digest(before) if before is not None else None,
                        "after": digest(after)})
    record = {"root": str(root), "files": entries}
    replace_file(backup_root / "manifest.json", encoded(record))
    journal = home / STATE_DIR / "pending.json"
    replace_file(journal, encoded(record))
    try:
        for target, before, after in changes:
            guard()
            if read_optional(target) != before:
                raise HandoverError(f"Datei inzwischen verändert: {target}")
            replace_file(target, after)
        guard()
        for target, _, after in changes:
            if read_optional(target) != after:
                raise HandoverError(f"Datei nach dem Ersetzen anderweitig verändert: {target}")
        journal.unlink()
    except BaseException:
        recover_transaction(home, root)
        raise
    say(f"[Sicherung] {backup_root}")


def validate_rollout(data, expected=None):
    """Validate canonical legacy/paginated JSONL and collect actual user inputs."""
    if not data or not data.endswith(b"\n"):
        raise HandoverError("Sessiondatei leer oder unvollständig; Codex erst sauber beenden.")
    first = None
    messages = []
    completed_users = set()
    for number, line in enumerate(io.BytesIO(data), 1):
        try:
            record = json.loads(line)
            payload = record.get("payload", {})
            if number == 1:
                if record["type"] != "session_meta":
                    raise ValueError("metadata")
                first = payload
                uuid.UUID(first["id"])
                if expected and first["id"] != expected:
                    raise ValueError("session mismatch")
                if first.get("history_mode", "legacy") not in {"legacy", "paginated"}:
                    raise ValueError("unsupported history mode")
            if first.get("history_mode") == "paginated" and record.get("ordinal") != number - 1:
                raise HandoverError(f"Paginated-Verlauf hat eine Lücke oder falsche Reihenfolge in Zeile {number}.")
            if record.get("type") == "event_msg" and payload.get("type") == "item_completed":
                item = payload.get("item", {})
                if item.get("type") == "UserMessage":
                    key = (payload["turn_id"], item["id"])
                    content = item.get("content", [])
                    for part in content:
                        if part.get("type") in {"localImage", "local_image"}:
                            raise HandoverError("Session hat lokale Bildanhänge; vollständige Übertragung nicht belegt.")
                        if part.get("type") in {"image", "input_image"} and not str(part.get("image_url", part.get("url", ""))).startswith("data:"):
                            raise HandoverError("Nicht eingebetteter Bildanhang; vollständige Übertragung nicht belegt.")
                    text = "\n".join(part["text"] for part in content if part.get("type") == "text")
                    if key not in completed_users and text:
                        timestamp = dt.datetime.fromisoformat(record["timestamp"].replace("Z", "+00:00"))
                        messages.append({"session_id": first["id"], "ts": int(timestamp.timestamp()), "text": text})
                        completed_users.add(key)
            if record.get("type") == "event_msg" and payload.get("type") == "user_message":
                if payload.get("local_images"):
                    raise HandoverError("Session hat lokale Bildanhänge. Vollständigkeit ist mit diesem Format nicht belegt; Export abgebrochen.")
                text = payload.get("message")
                if isinstance(text, str) and text:
                    timestamp = dt.datetime.fromisoformat(record["timestamp"].replace("Z", "+00:00"))
                    messages.append({"session_id": first["id"], "ts": int(timestamp.timestamp()), "text": text})
            if record.get("type") == "response_item" and payload.get("role") == "user":
                for item in payload.get("content", []):
                    if item.get("type") == "input_image" and not str(item.get("image_url", "")).startswith("data:"):
                        raise HandoverError("Nicht eingebetteter Bildanhang; vollständige Übertragung nicht belegt.")
        except (ValueError, KeyError, TypeError, AttributeError) as exc:
            raise HandoverError(f"Nicht unterstützte Sessiondaten in Zeile {number}.") from exc
    return first, messages


def merge_history(existing, incoming):
    """Match occurrences, not just texts: repeated intentional prompts stay repeated.

    A rollout event and the corresponding CLI history entry can differ slightly
    in timestamp. Match each existing occurrence at most once, preferring exact
    timestamps; retain all unrelated entries and their original JSON bytes.
    """
    old = rows(existing, "history.jsonl")
    candidates = collections.defaultdict(list)
    for index, (item, _) in enumerate(old):
        if not isinstance(item.get("ts"), (int, float)) or not isinstance(item.get("text"), str):
            raise HandoverError("Unbekanntes History-Format; vorhandene History bleibt unverändert.")
        candidates[(item.get("session_id"), item["text"])].append(index)
    added = []
    for item in incoming:
        key = (item["session_id"], item["text"])
        matches = [i for i in candidates[key] if abs(old[i][0]["ts"] - item["ts"]) <= 5]
        if matches:
            nearest = min(matches, key=lambda i: abs(old[i][0]["ts"] - item["ts"]))
            candidates[key].remove(nearest)
        else:
            added.append((item, encoded(item)))
    combined = sorted(old + added, key=lambda pair: pair[0]["ts"])
    return b"".join(line for _, line in combined)


def require_portable_history(home, sid, data=None):
    """Keep databases local; reject a cache newer than the canonical rollout.

    Paginated histories retain ordered raw records. Their SQLite history is a
    projection rebuilt by Codex when loading the raw file. Unknown modes or a
    projection beyond the available raw bytes must still fail closed.
    """
    if data is None:
        path = locate_session(home, sid)
        data = read_optional(path) if path else None
    meta = json.loads(io.BytesIO(data).readline()).get("payload", {}) if data else {}
    raw_mode = meta.get("history_mode", "legacy")
    database_home = Path(os.environ.get("CODEX_SQLITE_HOME", str(home)))
    config = home / "config.toml"
    if config.exists():
        import tomllib
        settings = tomllib.loads(config.read_text(encoding="utf-8"))
        if settings.get("sqlite_home"):
            database_home = Path(settings["sqlite_home"]).expanduser().resolve()
        if settings.get("history", {}).get("persistence") == "none":
            raise HandoverError("Codex-History ist lokal deaktiviert. history.persistence zuerst bewusst prüfen.")
    for database in database_home.glob("state_*.sqlite"):
        try:
            with contextlib.closing(sqlite3.connect(database.resolve().as_uri() + "?mode=ro", uri=True)) as conn:
                columns = {row[1] for row in conn.execute("PRAGMA table_info(threads)")}
                if "history_mode" in columns:
                    entry = conn.execute("SELECT history_mode FROM threads WHERE id=?", (sid,)).fetchone()
                    if entry and entry[0] not in {"legacy", "paginated"}:
                        raise HandoverError(f"Nicht unterstützte Datenbank-History {entry[0]!r}.")
                    if entry and entry[0] == "paginated" and (not data or raw_mode != "paginated"):
                        raise HandoverError("Paginated-History ohne passenden vollständigen JSONL-Verlauf; nichts übertragen.")
        except sqlite3.Error as exc:
            raise HandoverError(f"Codex-Index nicht zuverlässig lesbar: {database.name}: {exc}") from exc
    if raw_mode == "paginated":
        for database in database_home.glob("thread_history_*.sqlite"):
            try:
                with contextlib.closing(sqlite3.connect(database.resolve().as_uri() + "?mode=ro", uri=True)) as conn:
                    entry = conn.execute("SELECT next_rollout_byte_offset, next_rollout_ordinal "
                                         "FROM thread_history_projection_state WHERE thread_id=?", (sid,)).fetchone()
                    if entry:
                        offset, ordinal = entry
                        if (not isinstance(offset, int) or not isinstance(ordinal, int) or
                                offset < 0 or offset > len(data) or ordinal < 0 or
                                (offset and data[offset - 1:offset] != b"\n") or
                                data.count(b"\n", 0, offset) != ordinal):
                            raise HandoverError("History-Index passt nicht zum vollständigen JSONL-Verlauf; nichts übertragen.")
            except sqlite3.Error as exc:
                raise HandoverError(f"Codex-History-Index nicht zuverlässig lesbar: {database.name}: {exc}") from exc


def session_files(home, sid=None):
    """Read only active session files, never archived or backup folders."""
    pattern = f"rollout-*-{sid}.jsonl" if sid else "rollout-*.jsonl"
    return sorted((home / "sessions").rglob(pattern))


def locate_session(home, sid):
    """Multiple same-ID files are ambiguous, even if one happens to be newer."""
    files = session_files(home, sid)
    if len(files) > 1:
        raise HandoverError(f"Mehrere Dateien für Session {sid}; keine automatische Auswahl.")
    return files[0] if files else None


def choose(options, prompt):
    """Never silently select the newest of several plausible files/sessions."""
    if not options:
        raise HandoverError("Keine passende Session/ZIP gefunden.")
    if len(options) == 1:
        say("[Auswahl] " + options[0][0])
        return options[0][1]
    if not sys.stdin.isatty():
        raise HandoverError("Mehrere Möglichkeiten; --session beziehungsweise --archive ausdrücklich angeben.")
    for index, (label, _) in enumerate(options, 1):
        say(f"  {index}: {label}")
    answer = input(prompt + " [Nummer, Enter = Abbruch]: ").strip()
    if not answer.isdigit() or not 1 <= int(answer) <= len(options):
        raise HandoverError("Auswahl abgebrochen; nichts geändert.")
    return options[int(answer) - 1][1]


def select_session(home, root, repo, state, requested):
    """Prefer an explicitly managed session; otherwise show project candidates."""
    sid = requested or state["projects"].get(repo["identity"])
    if sid:
        uuid.UUID(sid)
        path = locate_session(home, sid)
        if path is None:
            raise HandoverError("Sessiondatei fehlt: " + sid)
        return path
    titles = {item["id"]: item.get("thread_name", "")
              for item, _ in rows(read_optional(home / "session_index.jsonl"), "Sessionindex") if "id" in item}
    candidates = []
    for path in sorted(session_files(home), key=lambda item: item.stat().st_mtime, reverse=True):
        with path.open("rb") as stream:
            first = json.loads(stream.readline())
        meta = first.get("payload", {})
        origin = meta.get("git", {}).get("repository_url", "")
        belongs = meta.get("cwd") == str(root)
        if origin:
            try:
                belongs = belongs or digest(canonical_origin(origin).encode()) == repo["identity"]
            except HandoverError:
                pass
        if belongs and meta.get("source") in {"cli", "vscode"}:
            changed = dt.datetime.fromtimestamp(path.stat().st_mtime).isoformat(timespec="seconds")
            candidates.append((f"{titles.get(meta['id'], 'Ohne Titel')} | zuletzt {changed} | {meta['id']}", path))
    return choose(candidates, "Welche Session soll weitergeführt werden?")


def require_project(meta, root, repo, previous):
    """An explicit UUID is selection, not permission to attach a foreign project."""
    if previous:
        if previous.get("repo") == repo["identity"]:
            return
    else:
        origin = meta.get("git", {}).get("repository_url", "")
        if origin and digest(canonical_origin(origin).encode()) == repo["identity"]:
            return
        if meta.get("cwd") == str(root):
            return
    raise HandoverError("Session gehört nicht nachweislich zu diesem Repository.")


def verify_checkpoint(data, entry):
    """Use an earlier content prefix as the high-water mark, never a file date."""
    if entry and (len(data) < entry["size"] or digest(data[:entry["size"]]) != entry["sha256"]):
        raise HandoverError("Session ist älter oder unabhängig verändert. Keine automatische Zusammenführung; beide Fassungen behalten.")


def write_archive(manifest, payload):
    """Keep a small allowlist of portable data; auth/config/databases stay local."""
    manifest = dict(manifest, files={name: {"sha256": digest(data), "size": len(data)} for name, data in payload.items()})
    target = io.BytesIO()
    with zipfile.ZipFile(target, "w", compression=zipfile.ZIP_DEFLATED) as archive:
        archive.writestr("manifest.json", encoded(manifest))
        for name, data in payload.items():
            archive.writestr(name, data)
    return target.getvalue()


def read_archive(data):
    """Validate exact member names, sizes, checksums and schema before extraction."""
    try:
        with zipfile.ZipFile(io.BytesIO(data)) as archive:
            names = archive.namelist()
            if set(names) != {"manifest.json", "rollout.jsonl", "history.jsonl", "session_index.jsonl"} or len(names) != 4:
                raise HandoverError("ZIP enthält unerwartete oder doppelte Einträge.")
            if sum(item.file_size for item in archive.infolist()) > MAX_BYTES:
                raise HandoverError("Entpackte ZIP ist größer als 512 MiB.")
            manifest = json.loads(archive.read("manifest.json"))
            if manifest["schema"] != SCHEMA:
                raise HandoverError("Nicht unterstützte Übergabeversion.")
            uuid.UUID(manifest["package_id"])
            uuid.UUID(manifest["session_id"])
            for key in ("identity", "commit"):
                if not isinstance(manifest["repository"][key], str):
                    raise ValueError("repository")
            for key in ("codex_version", "rollout_name", "source_computer", "exported_at"):
                if not isinstance(manifest[key], str):
                    raise ValueError(key)
            payload = {name: archive.read(name) for name in names if name != "manifest.json"}
            if set(manifest["files"]) != set(payload):
                raise HandoverError("Unvollständige ZIP-Dateiliste.")
            for name, content in payload.items():
                expected = manifest["files"][name]
                if expected != {"sha256": digest(content), "size": len(content)}:
                    raise HandoverError("Prüfsumme oder Größe falsch: " + name)
            validate_rollout(payload["rollout.jsonl"], manifest["session_id"])
            for item, _ in rows(payload["history.jsonl"], "ZIP-History"):
                if item.get("session_id") != manifest["session_id"] or not isinstance(item.get("ts"), (int, float)) or not isinstance(item.get("text"), str):
                    raise HandoverError("ZIP-History gehört nicht vollständig zur ausgewählten Session.")
            for item, _ in rows(payload["session_index.jsonl"], "ZIP-Index"):
                if item.get("id") != manifest["session_id"]:
                    raise HandoverError("ZIP-Index gehört zu einer anderen Session.")
            return manifest, payload
    except (zipfile.BadZipFile, KeyError, ValueError, TypeError, RuntimeError) as exc:
        raise HandoverError("ZIP beschädigt oder Format unbekannt; nichts importiert.") from exc


def export_session(root, home, requested=None):
    """Snapshot one stopped session, then seal the source before publishing its ZIP."""
    say("[1/7] Sauberen Git-Stand prüfen.")
    repo = repository(root)
    say(f"[Git] Branch {repo['branch'] or '(detached HEAD)'}; Commit {repo['commit']}")
    say("[2/7] Laufende Codex-Prozesse prüfen.")
    require_quiet()
    version = codex_version()
    say(f"[Codex] Version {version}; Profil {home}")
    with handover_lock(home):
        recover_transaction(home, root)
        state = load_state(home)
        say("[3/7] Session auswählen und gespeicherten Stand prüfen.")
        path = select_session(home, root, repo, state, requested)
        data = read_optional(path)
        meta, prompts = validate_rollout(data)
        sid = meta["id"]
        require_portable_history(home, sid, data)
        previous = state["sessions"].get(sid)
        require_project(meta, root, repo, previous)
        if previous and previous["status"] == "exported":
            raise HandoverError("Session bereits abgegeben. Vorhandene ZIP erneut übertragen: " + previous["archive"])
        verify_checkpoint(data, previous)
        say(f"[Session] {sid}; Codex {version}; Commit {repo['commit'][:12]}")
        say("[4/7] Eingabe-History vervollständigen; nur diese Session verpacken.")
        history_path, index_path = home / "history.jsonl", home / "session_index.jsonl"
        original_history, original_index = read_optional(history_path), read_optional(index_path)
        own_history = b"".join(line for item, line in rows(original_history, "History") if item.get("session_id") == sid)
        history = merge_history(own_history, prompts)
        own_index = b"".join(line for item, line in rows(original_index, "Sessionindex") if item.get("id") == sid)
        if not own_index:
            own_index = encoded({"id": sid, "thread_name": prompts[0]["text"][:80] if prompts else "Trice",
                                 "updated_at": meta["timestamp"]})
        package_id = str(uuid.uuid4())
        manifest = {"schema": SCHEMA, "package_id": package_id, "session_id": sid,
                    "repository": repo, "codex_version": version,
                    "exported_at": dt.datetime.now(dt.timezone.utc).isoformat(),
                    "source_computer": socket.gethostname(), "rollout_name": path.name}
        say("[5/7] ZIP komprimieren und sämtliche Prüfsummen prüfen.")
        package = write_archive(manifest, {"rollout.jsonl": data, "history.jsonl": history,
                                           "session_index.jsonl": own_index})
        read_archive(package)
        archive = root / "docs" / "scratchPad" / f"codex-handover-{sid}-{package_id}.zip"
        if archive.exists():
            raise HandoverError("Archivname bereits belegt; keine Datei überschrieben.")
        run(["git", "check-ignore", "--quiet", str(archive.relative_to(root))], root)
        original_state = read_optional(state_path(home))
        entry = dict(previous or {}, repo=repo["identity"], status="exported", size=len(data), sha256=digest(data),
                     archive=str(archive), package=package_id,
                     outgoing=(previous or {}).get("outgoing", []) + [package_id])
        state["sessions"][sid] = entry
        state["projects"][repo["identity"]] = sid

        def guard():
            """Recheck source and shared files; exporting never edits these inputs."""
            require_quiet()
            if repository(root) != repo:
                raise HandoverError("Git-Stand während des Exports verändert.")
            for source, snapshot in [(path, data), (history_path, original_history), (index_path, original_index)]:
                if read_optional(source) != snapshot:
                    raise HandoverError(f"Codex-Datei während des Exports verändert: {source}")

        say("[6/7] Ausgangsrechner als abgegeben markieren und ZIP veröffentlichen.")
        transaction(home, root, [(state_path(home), original_state, encoded(state)), (archive, None, package)], guard)
        say(f"[7/7] FERTIG: {archive}\n[Größe] {len(package) / 1024 / 1024:.1f} MiB")
        say("[Weiter] Diese ZIP auf dem Ziel in docs/scratchPad ablegen, denselben Git-Commit holen "
            "und ./scripts/codex_handover_start.sh starten.")
        say("[Hinweis] ZIP enthält Gespräch/Code, aber keine separat kopierten Zugangsdaten. Mail-Größenlimit beachten.")
        return archive


def main():
    """Keep the normal UI small; UUID selection is optional and never guessed."""
    parser = argparse.ArgumentParser(prog="codex_handover_export.sh",
                                     description="Gestoppte Codex-Session für den Rechnerwechsel exportieren.")
    parser.add_argument("--session", help="Session-UUID; sonst bekannte Session oder Auswahl")
    args = parser.parse_args()
    try:
        if sys.version_info < (3, 11):
            raise HandoverError("Python 3.11 oder neuer erforderlich.")
        root = Path(__file__).resolve().parents[1]
        export_session(root, codex_home(), args.session)
        return 0
    except (HandoverError, OSError, ValueError, KeyboardInterrupt) as exc:
        say(f"ABBRUCH: {exc}")
        return 1


if __name__ == "__main__":
    sys.exit(main())
