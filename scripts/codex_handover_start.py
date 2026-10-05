#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Import a checked handover or continue locally, then launch the selected session."""

import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import zipfile

sys.dont_write_bytecode = True
import codex_handover_export as handover


def import_session(root, home, repo, state, archive, requested=None):
    """Import only a verified continuation; snapshots and ownership move together."""
    handover.say("[3/6] ZIP vollständig lesen, Prüfsummen und Projektstand prüfen.")
    package = handover.read_optional(archive)
    if package is None:
        raise handover.HandoverError("ZIP fehlt: " + str(archive))
    manifest, payload = handover.read_archive(package)
    sid, package_id = manifest["session_id"], manifest["package_id"]
    if requested and requested != sid:
        raise handover.HandoverError("ZIP gehört zu einer anderen als der gewünschten Session.")
    if manifest["repository"]["identity"] != repo["identity"]:
        raise handover.HandoverError("ZIP gehört zu einem anderen Git-Repository.")
    if manifest["repository"]["commit"] != repo["commit"]:
        raise handover.HandoverError("Git-Commit passt nicht zur ZIP. Erwartet: " + manifest["repository"]["commit"] +
                                    "; vorhanden: " + repo["commit"] +
                                    "; Quellbranch: " + (manifest["repository"].get("branch") or "(detached HEAD)") +
                                    ". Passenden Stand zuerst über Git holen.")
    version = handover.codex_version()
    if version != manifest["codex_version"]:
        raise handover.HandoverError(f"Codex-Versionen unterscheiden sich: ZIP {manifest['codex_version']}, lokal {version}. "
                                    "Vor dem Import dieselbe Version installieren.")
    handover.require_legacy(home, sid)
    previous = state["sessions"].get(sid)
    if previous and previous.get("repo") != repo["identity"]:
        raise handover.HandoverError("Session ist lokal einem anderen Projekt zugeordnet.")
    if previous and package_id in previous.get("outgoing", []):
        raise handover.HandoverError("Das ist der eigene Export. Er darf die Abgabe-Sperre nicht aufheben. Rückgabe-ZIP vom anderen Rechner verwenden.")
    if previous and previous["status"] == "exported" and package_id in previous.get("imported", []):
        raise handover.HandoverError("Diese ZIP wurde bereits vor der Abgabe importiert. Neueste Rückgabe-ZIP verwenden.")
    incoming = payload["rollout.jsonl"]
    handover.verify_checkpoint(incoming, previous)
    target = handover.locate_session(home, sid)
    local = handover.read_optional(target) if target else None
    if local is not None:
        handover.validate_rollout(local, sid)
        if incoming == local:
            handover.say("[Vergleich] Sessioninhalt bereits identisch; keine Nachrichten doppelt einfügen.")
        elif local.startswith(incoming):
            raise handover.HandoverError("ZIP ist älter als der lokale Sessionstand. Kein Zurücksetzen.")
        elif not incoming.startswith(local):
            raise handover.HandoverError("Session wurde unabhängig weitergeführt. Beide Fassungen bleiben erhalten; kein automatisches Zusammenkleben.")
    else:
        meta, _ = handover.validate_rollout(incoming, sid)
        filename = manifest["rollout_name"]
        if not re.fullmatch(r"rollout-[\w.-]+-" + re.escape(sid) + r"\.jsonl", filename):
            raise handover.HandoverError("Unzulässiger Session-Dateiname in ZIP.")
        date = meta["timestamp"][:10]
        if not re.fullmatch(r"\d{4}-\d{2}-\d{2}", date):
            raise handover.HandoverError("Unbekanntes Session-Datumsformat.")
        target = handover.safe_target(home, "sessions/" + date.replace("-", "/") + "/" + filename)
    handover.say("[4/6] History und Sessiontitel zusammenführen; andere Sessions erhalten.")
    history_path, index_path = home / "history.jsonl", home / "session_index.jsonl"
    old_history, old_index = handover.read_optional(history_path), handover.read_optional(index_path)
    incoming_history = [item for item, _ in handover.rows(payload["history.jsonl"], "ZIP-History")]
    history = handover.merge_history(old_history, incoming_history)
    index = b"".join(line for item, line in handover.rows(old_index, "Sessionindex") if item.get("id") != sid)
    index += payload["session_index.jsonl"]
    old_state = handover.read_optional(handover.state_path(home))
    imports = (previous or {}).get("imported", [])
    state["sessions"][sid] = dict(previous or {}, repo=repo["identity"], status="local", size=len(incoming),
                                   sha256=handover.digest(incoming), package=package_id,
                                   imported=imports if package_id in imports else imports + [package_id])
    state["projects"][repo["identity"]] = sid
    changes = [(target, local, incoming), (history_path, old_history, history),
               (index_path, old_index, index), (handover.state_path(home), old_state, handover.encoded(state))]
    changes = [entry for entry in changes if entry[1] != entry[2]]

    def guard():
        """Catch newly started Codex processes and changes to Git or the archive."""
        handover.require_quiet()
        if handover.repository(root) != repo or handover.read_optional(archive) != package:
            raise handover.HandoverError("Projekt oder ZIP während des Imports verändert; neu beginnen.")

    handover.say("[5/6] Vorherigen Stand sichern und geprüfte Dateien übernehmen.")
    if changes:
        handover.transaction(home, root, changes, guard)
    else:
        guard()
        handover.say("[Import] Genau dieser Stand ist bereits vollständig vorhanden.")
    return sid


def select_archive(root, state, repo, requested):
    """Offer unseen packages instead of repeatedly importing yesterday's archive."""
    sid = requested or state["projects"].get(repo["identity"])
    options = []
    foreign = []
    for path in sorted((root / "docs" / "scratchPad").glob(handover.ARCHIVE_GLOB)):
        try:
            with zipfile.ZipFile(path) as archive:
                info = archive.getinfo("manifest.json")
                if info.file_size > 1024 * 1024:
                    raise ValueError("manifest too large")
                manifest = json.loads(archive.read("manifest.json"))
            package_sid = manifest["session_id"]
            entry = state["sessions"].get(package_sid, {})
            if manifest["package_id"] in entry.get("imported", []) + entry.get("outgoing", []):
                continue
            if sid and package_sid != sid:
                foreign.append(path.name)
                continue
            options.append((f"{path.name} | {manifest['source_computer']} | {manifest['exported_at']}", path))
        except (OSError, ValueError, KeyError, zipfile.BadZipFile) as exc:
            raise handover.HandoverError(f"ZIP nicht erkennbar: {path.name}; Datei prüfen oder aus scratchPad herausnehmen.") from exc
    if foreign:
        raise handover.HandoverError("Neue ZIP gehört zu einer anderen Session: " + ", ".join(foreign) +
                                    ". Datei prüfen; gewünschte ZIP/Session ausdrücklich wählen oder mit --local lokal fortsetzen.")
    return handover.choose(options, "Welche ZIP soll übernommen werden?") if options else None


def local_session(root, home, repo, state, requested):
    """A previously exported source cannot resume through this entry point."""
    sid = requested or state["projects"].get(repo["identity"])
    previous = state["sessions"].get(sid)
    if previous and previous["status"] == "exported":
        raise handover.HandoverError("Session wurde auf diesem Rechner abgegeben. Neueste Rückgabe-ZIP ablegen und Startskript erneut ausführen.")
    path = handover.select_session(home, root, repo, state, requested)
    data = handover.read_optional(path)
    meta, _ = handover.validate_rollout(data)
    sid = meta["id"]
    previous = state["sessions"].get(sid)
    if previous and previous["status"] == "exported":
        raise handover.HandoverError("Diese Session ist abgegeben; erst Rückgabe-ZIP importieren.")
    handover.require_project(meta, root, repo, previous)
    handover.require_legacy(home, sid)
    handover.verify_checkpoint(data, previous)
    old_state = handover.read_optional(handover.state_path(home))
    state["projects"][repo["identity"]] = sid
    state["sessions"][sid] = dict(previous or {}, repo=repo["identity"], status="local",
                                   size=len(data), sha256=handover.digest(data))

    def guard():
        """Local adoption also refuses concurrent edits and a newly dirty worktree."""
        handover.require_quiet()
        if handover.repository(root) != repo or handover.read_optional(path) != data:
            raise handover.HandoverError("Projekt oder Session während der Startprüfung verändert.")

    handover.transaction(home, root, [(handover.state_path(home), old_state, handover.encoded(state))], guard)
    return sid


def launch_session(root, home, sid):
    """Keep the handover lock until Codex exits; never inject an old prompt again."""
    handover.require_quiet()
    command = [shutil_codex(), "--no-daemon", "resume", sid, "--cd", str(root)]
    handover.say(f"[6/6] Codex starten: Session {sid}\n[Verzeichnis] {root}\n[Codex-Home] {home}")
    handover.say("[Hinweis] Erst Codex beenden, dann bei Bedarf committen/pushen und Exportskript starten.")
    result = subprocess.run(command, cwd=root, env=dict(os.environ, CODEX_HOME=str(home)))
    handover.say(f"[Beendet] Codex-Exitcode: {result.returncode}; Session bleibt lokal aktiv.")
    return result.returncode


def shutil_codex():
    """Resolve the native Windows npm shim as well as Unix executables."""
    import shutil
    executable = shutil.which("codex")
    if not executable:
        raise handover.HandoverError("codex fehlt im PATH. Codex zuerst lokal installieren/anmelden.")
    return executable


def start(root, home, archive=None, local=False, requested=None, launcher=launch_session):
    """One user action checks/imports and starts, without bypass switches."""
    handover.say("[1/6] Sauberen Git-Stand prüfen.")
    repo = handover.repository(root)
    handover.say(f"[Git] Branch {repo['branch'] or '(detached HEAD)'}; Commit {repo['commit']}")
    handover.say("[2/6] Laufende Codex-Prozesse prüfen.")
    handover.require_quiet()
    version = handover.codex_version()
    handover.say(f"[Codex] Version {version}; Profil {home}")
    handover.require_login(home)
    with handover.handover_lock(home):
        handover.recover_transaction(home, root)
        state = handover.load_state(home)
        requested = requested or state["projects"].get(repo["identity"])
        selected = archive
        if not selected and not local:
            selected = select_archive(root, state, repo, requested)
        if selected:
            sid = import_session(root, home, repo, state, selected, requested)
        else:
            handover.say("[3–5/6] Vorhandenen lokalen Stand und Abgabe-Sperre prüfen.")
            sid = local_session(root, home, repo, state, requested)
        handover.repository(root)
        return launcher(root, home, sid)


def main():
    """An explicit archive or local start is optional; ambiguity requires selection."""
    parser = argparse.ArgumentParser(prog="codex_handover_start.sh",
                                     description="Codex-Übergabe prüfen, gegebenenfalls importieren und Session starten.")
    group = parser.add_mutually_exclusive_group()
    group.add_argument("--archive", type=Path, help="Bestimmte Übergabe-ZIP")
    group.add_argument("--local", action="store_true", help="Bekannte lokale Session fortsetzen; Abgabe-Sperre bleibt wirksam")
    parser.add_argument("--session", help="Gewünschte Session-UUID")
    args = parser.parse_args()
    try:
        if sys.version_info < (3, 11):
            raise handover.HandoverError("Python 3.11 oder neuer erforderlich.")
        root = Path(__file__).resolve().parents[1]
        return start(root, handover.codex_home(), args.archive.resolve() if args.archive else None, args.local, args.session)
    except (handover.HandoverError, OSError, ValueError, KeyboardInterrupt) as exc:
        handover.say(f"ABBRUCH: {exc}")
        return 1


if __name__ == "__main__":
    sys.exit(main())
