# SPDX-License-Identifier: MIT
"""Behavioral tests for portable handovers; all Git/Codex data lives in fixtures.

Run: ./scripts/test_codex_handover.sh -v
Real Codex processes and personal profiles are never stopped, imported or edited.
"""

import contextlib
import datetime as dt
import io
import json
import os
from pathlib import Path
import shutil
import sqlite3
import subprocess
import sys
import tempfile
import threading
import queue
import unittest
from unittest import mock
import uuid
import zipfile

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parent))
import codex_handover_export as export
import codex_handover_start as start

SESSION = "01234567-89ab-4def-8123-456789abcdef"
OTHER = "fedcba98-7654-4321-8123-456789abcdef"


def event(text, second):
    """Give each deliberate prompt occurrence its own persistent event."""
    stamp = (dt.datetime(2026, 10, 2, tzinfo=dt.timezone.utc) + dt.timedelta(seconds=second)).isoformat()
    return export.encoded({"timestamp": stamp, "type": "event_msg", "payload": {
        "type": "user_message", "message": text, "images": [], "local_images": []}})


class HandoverBehavior(unittest.TestCase):
    """Exercise real archives, Git checks, merges, journals, and ownership changes."""

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="trice handover tests ")
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name) / "repo with spaces"
        self.root.mkdir()
        (self.root / "docs/scratchPad").mkdir(parents=True)
        (self.root / ".gitignore").write_text("docs/scratchPad/codex-handover-*.zip\n", encoding="utf-8")
        self.git("init", "-q")
        self.git("remote", "add", "origin", "https://github.com/example/trice.git")
        self.git("add", ".gitignore")
        self.git("commit", "-qm", "fixture")
        self.homes = [Path(self.temp.name) / name for name in ("Mac", "Windows", "Debian")]
        for home in self.homes:
            home.mkdir()
        self.source, self.windows, self.debian = self.homes
        folder = self.source / "sessions/2026/10/02"
        folder.mkdir(parents=True)
        self.path = folder / f"rollout-2026-10-02T00-00-00-{SESSION}.jsonl"
        meta = {"timestamp": "2026-10-02T00:00:00Z", "type": "session_meta", "payload": {
            "id": SESSION, "timestamp": "2026-10-02T00:00:00Z", "cwd": "C:\\old user\\trice",
            "source": "cli", "cli_version": "0.159.3", "originator": "codex_cli_rs",
            "model_provider": "openai", "base_instructions": {"text": ""},
            "git": {"repository_url": "git@github.com:example/trice.git"}}}
        self.initial = export.encoded(meta) + event("Bitte R06 starten – Grüße", 1) + event("weiter", 2)
        self.path.write_bytes(self.initial)
        # These files must never enter the ZIP or be copied onto another computer.
        (self.source / "auth.json").write_text('{"token":"DO-NOT-COPY"}', encoding="utf-8")
        (self.source / "config.toml").write_text('model = "local-choice"\n', encoding="utf-8")
        self.output = io.StringIO()
        capture = contextlib.redirect_stdout(self.output)
        capture.__enter__()
        self.addCleanup(capture.__exit__, None, None, None)
        self.quiet = mock.patch.object(export, "require_quiet")
        self.quiet.start()
        self.addCleanup(self.quiet.stop)
        self.version = mock.patch.object(export, "codex_version", return_value="0.159.3")
        self.version.start()
        self.addCleanup(self.version.stop)
        self.login = mock.patch.object(export, "require_login")
        self.login.start()
        self.addCleanup(self.login.stop)
        self.launches = []

    def git(self, *args):
        """Use a temporary repository and disable user hooks/signing/configuration."""
        env = dict(os.environ, GIT_CONFIG_GLOBAL=os.devnull, GIT_CONFIG_NOSYSTEM="1")
        return subprocess.run(["git", "-c", "user.name=Fixture", "-c", "user.email=fixture@example.invalid",
                               "-c", "commit.gpgSign=false", *args], cwd=self.root, env=env,
                              check=True, capture_output=True, text=True).stdout.strip()

    def export(self, home=None):
        """Use the public operation without probing personal Codex processes or credentials."""
        return export.export_session(self.root, home or self.source, SESSION)

    def start(self, home, archive=None, local=False):
        """Record the intended session launch without running an actual model."""
        return start.start(self.root, home, archive=archive, local=local,
                           launcher=lambda root, profile, sid: self.launches.append((root, profile, sid)) or 0)

    def state(self, home):
        """Read the persisted ownership result instead of inspecting local variables."""
        return export.load_state(home)["sessions"][SESSION]

    def append(self, home, text, second):
        """Simulate Codex appending a completed prompt on the currently active machine."""
        path = export.locate_session(home, SESSION)
        with path.open("ab") as stream:
            stream.write(event(text, second))

    def test_mac_windows_debian_and_back_preserves_full_history_and_blocks_old_source(self):
        original = self.path.read_bytes()
        zip_a = self.export()
        self.assertEqual(original, self.path.read_bytes(), "export must not rewrite historical Windows paths")
        with self.assertRaisesRegex(export.HandoverError, "abgegeben"):
            self.start(self.source, local=True)
        with self.assertRaisesRegex(export.HandoverError, "eigene Export"):
            self.start(self.source, zip_a)
        self.start(self.windows, zip_a)
        self.append(self.windows, "Windows erledigt", 10)
        zip_b = self.export(self.windows)
        self.start(self.debian, zip_b)
        self.append(self.debian, "Debian erledigt", 20)
        zip_c = self.export(self.debian)
        self.start(self.source, zip_c)
        data = self.path.read_bytes()
        self.assertTrue(data.startswith(original))
        self.assertIn("Windows erledigt", data.decode())
        self.assertIn("Debian erledigt", data.decode())
        self.assertEqual("local", self.state(self.source)["status"])
        self.assertEqual(4, len(export.rows((self.source / "history.jsonl").read_bytes(), "history")))
        self.assertEqual(3, len(self.launches))
        self.assertEqual("", self.git("status", "--porcelain"), "ignored archives keep the repository clean")

    def test_export_reconstructs_missing_prompt_history_without_copying_credentials(self):
        archive = self.export()
        manifest, payload = export.read_archive(archive.read_bytes())
        self.assertEqual(SESSION, manifest["session_id"])
        self.assertEqual({"rollout.jsonl", "history.jsonl", "session_index.jsonl"}, set(payload))
        self.assertNotIn(b"DO-NOT-COPY", b"".join(payload.values()))
        prompts = [item["text"] for item, _ in export.rows(payload["history.jsonl"], "history")]
        self.assertEqual(["Bitte R06 starten – Grüße", "weiter"], prompts)
        self.assertIn("[7/7] FERTIG", self.output.getvalue())
        self.assertFalse((self.source / "history.jsonl").exists(), "source history is not edited as a side effect")

    def test_identical_reimport_does_not_duplicate_history_or_other_session_index(self):
        other = export.encoded({"session_id": OTHER, "ts": 1, "text": "another project"})
        (self.windows / "history.jsonl").write_bytes(other)
        other_index = export.encoded({"id": OTHER, "thread_name": "keep me", "updated_at": "2026-01-01"})
        (self.windows / "session_index.jsonl").write_bytes(other_index)
        archive = self.export()
        self.start(self.windows, archive)
        before = {name: (self.windows / name).read_bytes() for name in ("history.jsonl", "session_index.jsonl")}
        self.start(self.windows, archive)
        self.assertEqual(before, {name: (self.windows / name).read_bytes() for name in before})
        self.assertIn(other, before["history.jsonl"])
        self.assertTrue(before["session_index.jsonl"].startswith(other_index))

    def test_deliberately_repeated_identical_prompts_are_not_collapsed(self):
        self.append(self.source, "weiter", 3)
        self.append(self.source, "weiter", 4)
        archive = self.export()
        self.start(self.windows, archive)
        self.start(self.windows, archive)
        history = [item["text"] for item, _ in export.rows((self.windows / "history.jsonl").read_bytes(), "history")]
        self.assertEqual(3, history.count("weiter"))

    def test_old_zip_and_diverged_same_id_never_overwrite_local_progress(self):
        archive = self.export()
        self.start(self.windows, archive)
        self.append(self.windows, "local newer", 20)
        local_path = export.locate_session(self.windows, SESSION)
        before = local_path.read_bytes()
        with self.assertRaisesRegex(export.HandoverError, "älter"):
            self.start(self.windows, archive)
        self.assertEqual(before, local_path.read_bytes())
        manifest, payload = export.read_archive(archive.read_bytes())
        payload["rollout.jsonl"] += event("different branch", 30)
        manifest["package_id"] = str(uuid.uuid4())
        fork = archive.with_name("codex-handover-diverged.zip")
        fork.write_bytes(export.write_archive(manifest, payload))
        with self.assertRaisesRegex(export.HandoverError, "unabhängig"):
            self.start(self.windows, fork)
        self.assertEqual(before, local_path.read_bytes())

    def test_pre_export_import_cannot_unlock_the_source_after_handover(self):
        first = self.export()
        self.start(self.windows, first)
        self.export(self.windows)
        with self.assertRaisesRegex(export.HandoverError, "vor der Abgabe"):
            self.start(self.windows, first)
        self.assertEqual("exported", self.state(self.windows)["status"])

    def test_checkpoint_refuses_truncated_file_even_if_its_date_is_newer(self):
        archive = self.export()
        self.start(self.windows, archive)
        local_path = export.locate_session(self.windows, SESSION)
        local_path.write_bytes(self.initial.splitlines(keepends=True)[0] + event("replacement", 900))
        with self.assertRaisesRegex(export.HandoverError, "älter oder unabhängig"):
            self.export(self.windows)

    def test_dirty_source_and_dirty_destination_refuse_before_any_handover_write(self):
        (self.root / "new source.c").write_text("uncommitted", encoding="utf-8")
        with self.assertRaisesRegex(export.HandoverError, "Git ist nicht clean"):
            self.export()
        self.assertFalse((self.source / export.STATE_DIR).exists())
        (self.root / "new source.c").unlink()
        archive = self.export()
        (self.root / ".gitignore").write_text("changed", encoding="utf-8")
        with self.assertRaisesRegex(export.HandoverError, "Git ist nicht clean"):
            self.start(self.windows, archive)
        self.assertFalse((self.windows / export.STATE_DIR).exists())

    def test_wrong_commit_and_wrong_repository_leave_destination_untouched(self):
        archive = self.export()
        self.git("commit", "--allow-empty", "-qm", "different commit")
        with self.assertRaisesRegex(export.HandoverError, "Git-Commit passt nicht"):
            self.start(self.windows, archive)
        self.assertFalse((self.windows / "sessions").exists())
        self.git("remote", "set-url", "origin", "https://github.com/example/other.git")
        with self.assertRaisesRegex(export.HandoverError, "anderen Git-Repository"):
            self.start(self.windows, archive)

    def test_missing_or_changed_codex_version_cannot_partially_import(self):
        archive = self.export()
        with mock.patch.object(export, "codex_version", return_value="0.160.0"):
            with self.assertRaisesRegex(export.HandoverError, "Versionen unterscheiden"):
                self.start(self.windows, archive)
        self.assertFalse((self.windows / "history.jsonl").exists())

    def test_missing_codex_on_export_does_not_seal_the_session_or_create_a_zip(self):
        with mock.patch.object(export, "codex_version", side_effect=export.HandoverError("codex fehlt")):
            with self.assertRaisesRegex(export.HandoverError, "codex fehlt"):
                self.export()
        self.assertFalse((self.source / export.STATE_DIR).exists())
        self.assertFalse(list((self.root / "docs/scratchPad").glob("*.zip")))

    def test_missing_login_on_destination_leaves_profile_untouched_and_can_be_retried(self):
        archive = self.export()
        with mock.patch.object(export, "require_login", side_effect=export.HandoverError("codex login")):
            with self.assertRaisesRegex(export.HandoverError, "codex login"):
                self.start(self.windows, archive)
        self.assertEqual([], list(self.windows.iterdir()), "login failure must precede even the import lock")
        self.assertFalse(self.launches)
        self.start(self.windows, archive)
        self.assertEqual(self.initial, export.locate_session(self.windows, SESSION).read_bytes())
        self.assertEqual(1, len(self.launches))

    def test_sqlite_backed_history_is_refused_instead_of_exporting_an_old_rollout(self):
        with sqlite3.connect(self.source / "state_5.sqlite") as connection:
            connection.execute("CREATE TABLE threads (id TEXT, history_mode TEXT)")
            connection.execute("INSERT INTO threads VALUES (?,?)", (SESSION, "paginated"))
        with self.assertRaisesRegex(export.HandoverError, "Datenbank-History"):
            self.export()
        self.assertFalse(list((self.root / "docs/scratchPad").glob("*.zip")))

    def test_local_image_and_truncated_last_record_refuse_without_creating_archive(self):
        broken = json.loads(event("picture", 8))
        broken["payload"]["local_images"] = ["C:\\missing\\photo.png"]
        self.path.write_bytes(self.initial + export.encoded(broken))
        with self.assertRaisesRegex(export.HandoverError, "Bildanhänge"):
            self.export()
        self.path.write_bytes(self.initial.rstrip(b"\n"))
        with self.assertRaisesRegex(export.HandoverError, "unvollständig"):
            self.export()
        self.assertFalse(list((self.root / "docs/scratchPad").glob("*.zip")))

    def test_corrupt_checksum_traversal_and_duplicate_zip_entries_are_rejected(self):
        original = self.export()
        for variant in ("checksum", "traversal", "duplicate"):
            with self.subTest(variant=variant):
                altered = io.BytesIO()
                with zipfile.ZipFile(original) as source, zipfile.ZipFile(altered, "w") as target:
                    for name in source.namelist():
                        data = source.read(name)
                        if variant == "checksum" and name == "history.jsonl":
                            data += b"{}\n"
                        target.writestr(name, data)
                    if variant == "traversal":
                        target.writestr("../../auth.json", "wrong")
                    if variant == "duplicate":
                        import warnings
                        with warnings.catch_warnings():
                            warnings.simplefilter("ignore", UserWarning)
                            target.writestr("history.jsonl", b"")
                with self.assertRaises(export.HandoverError):
                    export.read_archive(altered.getvalue())

    def test_active_process_and_unknown_process_state_block_even_a_clean_export(self):
        for reason in ("Codex läuft noch", "Prozessliste nicht lesbar"):
            with self.subTest(reason=reason), mock.patch.object(export, "require_quiet", side_effect=export.HandoverError(reason)):
                with self.assertRaisesRegex(export.HandoverError, reason):
                    self.export()
        self.assertFalse((self.source / export.STATE_DIR).exists())

    def test_second_handover_script_cannot_enter_existing_lock(self):
        with export.handover_lock(self.source):
            with self.assertRaisesRegex(export.HandoverError, "bereits aktiv"):
                self.export()

    def test_os_releases_handover_lock_after_process_crash_without_manual_unlock(self):
        helper = ("import sys, os; from pathlib import Path; "
                  "sys.path.insert(0, sys.argv[1]); import codex_handover_export as h; "
                  "lock=h.handover_lock(Path(sys.argv[2])); lock.__enter__(); os._exit(23)")
        result = subprocess.run([sys.executable, "-B", "-c", helper,
                                 str(Path(export.__file__).parent), str(self.windows)], timeout=15)
        self.assertEqual(23, result.returncode)
        with export.handover_lock(self.windows):
            pass

    def test_failed_second_replacement_restores_existing_and_removes_new_file(self):
        existing = self.windows / "history.jsonl"
        existing.write_bytes(b"original\n")
        new_file = self.windows / "sessions/new.jsonl"
        real_replace = export.replace_file
        failed = False

        def fail_once(path, data):
            """Fail only the chosen installation write; backup and rollback remain usable."""
            nonlocal failed
            if path == new_file and not failed:
                failed = True
                raise OSError("simulated full disk")
            return real_replace(path, data)

        with mock.patch.object(export, "replace_file", side_effect=fail_once):
            with self.assertRaisesRegex(OSError, "full disk"):
                export.transaction(self.windows, self.root,
                                   [(existing, b"original\n", b"replaced\n"), (new_file, None, b"new\n")], lambda: None)
        self.assertEqual(b"original\n", existing.read_bytes())
        self.assertFalse(new_file.exists())
        self.assertFalse((self.windows / export.STATE_DIR / "pending.json").exists())
        self.assertTrue(list((self.windows / export.STATE_DIR / "backups").glob("*/manifest.json")))

    def test_interrupted_import_keeps_journal_until_codex_is_closed_and_can_then_recover(self):
        target = self.windows / "history.jsonl"
        target.write_bytes(b"before\n")
        calls = 0

        def guard():
            """Simulate Codex appearing after our replacement, so rollback must wait."""
            nonlocal calls
            calls += 1
            if calls == 3:
                raise export.HandoverError("writer appeared")

        with mock.patch.object(export, "require_quiet", side_effect=export.HandoverError("writer active")):
            with self.assertRaisesRegex(export.HandoverError, "writer active"):
                export.transaction(self.windows, self.root, [(target, b"before\n", b"after\n")], guard)
        self.assertTrue((self.windows / export.STATE_DIR / "pending.json").exists())
        export.recover_transaction(self.windows, self.root)
        self.assertEqual(b"before\n", target.read_bytes())

    def test_rollback_does_not_overwrite_a_concurrent_external_edit(self):
        target = self.windows / "history.jsonl"
        target.write_bytes(b"before\n")
        calls = 0

        def guard():
            """An external edit after install must survive even a rollback attempt."""
            nonlocal calls
            calls += 1
            if calls == 3:
                target.write_bytes(b"external writer\n")

        with self.assertRaisesRegex(export.HandoverError, "anderweitig geändert"):
            export.transaction(self.windows, self.root, [(target, b"before\n", b"after\n")], guard)
        self.assertEqual(b"external writer\n", target.read_bytes())
        self.assertTrue((self.windows / export.STATE_DIR / "pending.json").exists())

    def test_default_start_uses_new_archive_then_continues_without_reimport(self):
        self.export()
        self.start(self.windows)
        self.append(self.windows, "local work", 10)
        self.start(self.windows)
        self.assertEqual(2, len(self.launches))
        self.assertIn("local work", export.locate_session(self.windows, SESSION).read_text())

    def test_launch_passes_session_and_new_directory_without_replaying_a_prompt(self):
        with mock.patch.object(start, "shutil_codex", return_value="codex"), mock.patch.object(start.subprocess, "run") as invoke:
            invoke.return_value.returncode = 0
            self.assertEqual(0, start.launch_session(self.root, self.windows, SESSION))
        self.assertEqual(["codex", "--no-daemon", "resume", SESSION, "--cd", str(self.root)], invoke.call_args.args[0])
        self.assertEqual(str(self.windows), invoke.call_args.kwargs["env"]["CODEX_HOME"],
                         "resume must use the profile that was checked and imported")

    def test_wrong_explicit_session_is_rejected_without_replacing_existing_history(self):
        archive = self.export()
        with self.assertRaisesRegex(export.HandoverError, "anderen als der gewünschten"):
            start.start(self.root, self.windows, archive, requested=OTHER, launcher=lambda *args: self.fail("must not launch"))
        self.assertFalse((self.windows / "history.jsonl").exists())

    def test_unseen_wrong_session_zip_cannot_silently_start_an_older_local_session(self):
        archive = self.export()
        self.start(self.windows, archive)
        manifest, payload = export.read_archive(archive.read_bytes())
        manifest["session_id"] = OTHER
        manifest["package_id"] = str(uuid.uuid4())
        foreign = archive.with_name("codex-handover-wrong-session.zip")
        foreign.write_bytes(export.write_archive(manifest, payload))
        self.launches.clear()
        with self.assertRaisesRegex(export.HandoverError, "Neue ZIP gehört zu einer anderen Session"):
            self.start(self.windows)
        self.assertFalse(self.launches, "wrong attachment must require attention before resuming")
        self.assertEqual(self.initial, export.locate_session(self.windows, SESSION).read_bytes())

    def test_source_modified_during_compression_leaves_no_zip_and_no_exported_state(self):
        compress = export.write_archive

        def modify_source_after_compression(manifest, payload):
            """Simulate a writer missed by the process snapshot; byte checks still catch it."""
            package = compress(manifest, payload)
            self.append(self.source, "arrived during compression", 20)
            return package

        with mock.patch.object(export, "write_archive", side_effect=modify_source_after_compression):
            with self.assertRaisesRegex(export.HandoverError, "während des Exports verändert"):
                self.export()
        self.assertFalse(export.load_state(self.source)["sessions"])
        self.assertFalse(list((self.root / "docs/scratchPad").glob("*.zip")))
        self.assertIn("arrived during compression", self.path.read_text())

    def test_failed_zip_publication_rolls_back_the_source_handover_marker(self):
        replace = export.replace_file

        def fail_archive(path, data):
            """Publishing fails after sealing the source; rollback must make it usable again."""
            if path.suffix == ".zip":
                raise OSError("archive disk full")
            return replace(path, data)

        with mock.patch.object(export, "replace_file", side_effect=fail_archive):
            with self.assertRaisesRegex(OSError, "archive disk full"):
                self.export()
        self.assertFalse(export.load_state(self.source)["sessions"])
        self.assertFalse(list((self.root / "docs/scratchPad").glob("*.zip")))
        self.assertEqual(self.initial, self.path.read_bytes())

    def test_corrupt_ownership_record_is_not_treated_as_a_fresh_computer(self):
        self.export()
        export.state_path(self.source).write_bytes(export.encoded({"schema": 1, "sessions": {}, "projects": {"repo": SESSION}}))
        with self.assertRaisesRegex(export.HandoverError, "Übergabestatus beschädigt"):
            self.start(self.source, local=True)
        self.assertFalse(self.launches)

    def test_known_session_from_other_project_is_not_adopted_by_uuid(self):
        self.git("remote", "set-url", "origin", "https://github.com/example/different.git")
        with self.assertRaisesRegex(export.HandoverError, "nicht nachweislich"):
            self.export()

    def test_duplicate_session_files_are_not_selected_by_newest_file_date(self):
        other_folder = self.source / "sessions/2026/10/03"
        other_folder.mkdir()
        (other_folder / self.path.name).write_bytes(self.initial)
        with self.assertRaisesRegex(export.HandoverError, "Mehrere Dateien"):
            self.export()

    def test_partial_import_error_restores_other_history_and_keeps_launch_disabled(self):
        archive = self.export()
        original = export.encoded({"session_id": OTHER, "text": "keep this", "ts": 1})
        (self.windows / "history.jsonl").write_bytes(original)
        real_replace = export.replace_file
        failed = False

        def fail_index_once(path, data):
            """Inject a disk error after rollout/history were already installed."""
            nonlocal failed
            if path == self.windows / "session_index.jsonl" and not failed:
                failed = True
                raise OSError("index write failed")
            return real_replace(path, data)

        with mock.patch.object(export, "replace_file", side_effect=fail_index_once):
            with self.assertRaisesRegex(OSError, "index write failed"):
                self.start(self.windows, archive)
        self.assertEqual(original, (self.windows / "history.jsonl").read_bytes())
        self.assertIsNone(export.locate_session(self.windows, SESSION))
        self.assertFalse(self.launches)

    @unittest.skipUnless(os.environ.get("TRICE_CODEX_HANDOVER_INTEGRATION") == "1", "opt-in: requires installed Codex; no model call")
    def test_installed_codex_reads_imported_rollout_without_copying_any_database(self):
        """Use Codex's real read-only thread API in an isolated, credential-free home."""
        archive = self.export()
        self.start(self.windows, archive)
        self.assertFalse(list(self.windows.glob("*.sqlite")), "import does not install or edit databases")
        env = dict(os.environ, CODEX_HOME=str(self.windows))
        env.pop("CODEX_SQLITE_HOME", None)
        process = subprocess.Popen([start.shutil_codex(), "app-server", "--listen", "stdio://"], cwd=self.root,
                                   env=env, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                   stderr=subprocess.DEVNULL, text=True, encoding="utf-8")
        replies = queue.Queue()

        def receive():
            """Collect protocol replies without depending on POSIX-only pipe polling."""
            for line in process.stdout:
                replies.put(json.loads(line))

        reader = threading.Thread(target=receive, daemon=True)
        reader.start()

        def request(method, params, request_id):
            """Ignore notifications but bound waiting for the requested reply."""
            process.stdin.write(json.dumps({"id": request_id, "method": method, "params": params}) + "\n")
            process.stdin.flush()
            deadline = dt.datetime.now(dt.timezone.utc) + dt.timedelta(seconds=30)
            while True:
                remaining = (deadline - dt.datetime.now(dt.timezone.utc)).total_seconds()
                item = replies.get(timeout=max(0.01, remaining))
                if item.get("id") == request_id:
                    return item
                if remaining <= 0:
                    self.fail("Codex API did not answer before timeout")

        try:
            reply = request("initialize", {"clientInfo": {"name": "trice_handover_test", "version": "1.0"}}, 1)
            self.assertNotIn("error", reply)
            process.stdin.write('{"method":"initialized"}\n')
            process.stdin.flush()
            reply = request("thread/read", {"threadId": SESSION, "includeTurns": True}, 2)
            self.assertNotIn("error", reply)
            self.assertEqual(SESSION, reply["result"]["thread"]["id"])
            self.assertIn("Bitte R06 starten", json.dumps(reply, ensure_ascii=False))
        finally:
            process.stdin.close()
            try:
                process.wait(timeout=10)
            except subprocess.TimeoutExpired:
                process.terminate()
                process.wait(timeout=10)
            reader.join(timeout=5)
            process.stdout.close()


class ShellEntryPoints(unittest.TestCase):
    """Run real POSIX launchers with controlled interpreter candidates on PATH.

    The Windows cases simulate Git Bash discovery; they do not claim an actual
    Windows execution. No fixture ever invokes an installed Codex or user profile.
    """

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="handover shell Grüße ")
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.scripts = self.root / "scripts with spaces"
        self.scripts.mkdir()
        self.bin = self.root / "bin"
        self.bin.mkdir()
        self.shell = shutil.which("sh")
        if not self.shell:
            self.skipTest("POSIX sh required (Windows: run from Git Bash)")
        for source in Path(__file__).resolve().parent.glob("*codex_handover*.sh"):
            shutil.copyfile(source, self.scripts / source.name)
        self.env = dict(os.environ, PATH=str(self.bin) + os.pathsep + os.environ.get("PATH", ""))
        self.platform("Linux")
        for name in ("python3", "python", "py"):
            self.interpreter(name, usable=False)

    def platform(self, name):
        """Select discovery behavior without changing the host OS or Python profile."""
        path = self.bin / "uname"
        path.write_text(f"#!/bin/sh\nprintf '%s\\n' '{name}'\n", encoding="utf-8")
        path.chmod(0o755)

    def interpreter(self, name, usable=True, exit_code=0):
        """Reject an unsuitable candidate or record the exact script argument vector."""
        path = self.bin / name
        path.write_text(
            "#!/bin/sh\nlauncher_flag=none\n"
            'if [ "$1" = -3 ]; then launcher_flag=-3; shift; fi\n'
            f'if [ "$1" = -c ]; then exit {0 if usable else 1}; fi\n'
            f"printf 'INTERPRETER:%s\\n' '{name}'\n"
            'printf "LAUNCHER:%s\\n" "$launcher_flag"\n'
            'printf "ARG:%s\\n" "$@"\n'
            'printf "CWD:%s\\n" "$PWD"\n'
            f"exit {exit_code}\n", encoding="utf-8")
        path.chmod(0o755)

    def invoke(self, name, *args):
        """Invoke from outside the checkout to catch accidental cwd-dependent paths."""
        return subprocess.run([self.shell, str(self.scripts / name), *args], env=self.env,
                              cwd=self.root, capture_output=True, text=True, encoding="utf-8", timeout=15)

    def native_path(self, value):
        """Compare locations across macOS symlinks and Git Bash's /c/... spelling."""
        if sys.platform == "win32":
            value = subprocess.run([self.shell, "-c", 'cygpath -w "$1"', "handover-test", value],
                                   check=True, capture_output=True, text=True, timeout=15).stdout.strip()
        return Path(value).resolve()

    def test_export_and_start_find_their_modules_and_preserve_spaces_and_literal_arguments(self):
        self.interpreter("python3")
        for operation, args in (
            ("export", ["--session", SESSION]),
            ("start", ["--archive", "docs/scratchPad/ZIP Grüße ; $(not-a-command).zip"]),
        ):
            with self.subTest(operation=operation):
                result = self.invoke(f"codex_handover_{operation}.sh", *args)
                self.assertEqual(0, result.returncode, result.stderr)
                forwarded = [line[4:] for line in result.stdout.splitlines() if line.startswith("ARG:")]
                self.assertEqual(["-B", *args], [forwarded[0], *forwarded[2:]])
                self.assertEqual((self.scripts / f"codex_handover_{operation}.py").resolve(), self.native_path(forwarded[1]))
                working = next(line[4:] for line in result.stdout.splitlines() if line.startswith("CWD:"))
                self.assertEqual(self.root.resolve(), self.native_path(working), "relative --archive paths belong to the caller")

    def test_old_python3_falls_back_to_python_on_both_macos_and_linux(self):
        self.interpreter("python", usable=True)
        for platform in ("Darwin", "Linux"):
            with self.subTest(platform=platform):
                self.platform(platform)
                result = self.invoke("codex_handover_export.sh", "--help")
                self.assertEqual(0, result.returncode, result.stderr)
                self.assertIn("INTERPRETER:python\n", result.stdout)

    def test_git_bash_prefers_py_launcher_and_falls_back_when_it_is_unusable(self):
        self.platform("MINGW64_NT-10.0")
        self.interpreter("py")
        self.interpreter("python")
        result = self.invoke("codex_handover_start.sh", "--local")
        self.assertEqual(0, result.returncode, result.stderr)
        self.assertIn("INTERPRETER:py\nLAUNCHER:-3", result.stdout)
        self.interpreter("py", usable=False)
        result = self.invoke("codex_handover_start.sh", "--local")
        self.assertEqual(0, result.returncode, result.stderr)
        self.assertIn("INTERPRETER:python\nLAUNCHER:none", result.stdout)

    @unittest.skipIf(sys.platform == "win32", "non-Windows interpreter required for rejection fixture")
    def test_git_bash_rejects_an_actual_non_windows_python_before_any_handover(self):
        self.platform("MSYS_NT-10.0")
        self.env["HANDOVER_TEST_PYTHON"] = sys.executable
        candidate = self.bin / "python"
        candidate.write_text('#!/bin/sh\nexec "$HANDOVER_TEST_PYTHON" "$@"\n', encoding="utf-8")
        result = self.invoke("codex_handover_start.sh", "--help")
        self.assertEqual(1, result.returncode)
        self.assertIn("natives Windows-Python", result.stderr)
        self.assertNotIn("[Python] Verwende", result.stdout)

    def test_no_usable_python_explains_the_fix_without_invoking_a_handover(self):
        result = self.invoke("codex_handover_export.sh")
        self.assertEqual(1, result.returncode)
        self.assertIn("Python ab 3.11", result.stderr)
        self.assertIn("dasselbe .sh-Skript", result.stderr)
        self.assertNotIn("INTERPRETER:", result.stdout)

    def test_test_wrapper_forwards_unittest_options_and_preserves_failure_exit_status(self):
        self.interpreter("python3", exit_code=23)
        result = self.invoke("test_codex_handover.sh", "-v", "-k", "missing_login")
        self.assertEqual(23, result.returncode, "a failed underlying operation must stay failed")
        forwarded = [line[4:] for line in result.stdout.splitlines() if line.startswith("ARG:")]
        self.assertEqual(["-B", "-v", "-k", "missing_login"], [forwarded[0], *forwarded[2:]])
        self.assertEqual((self.scripts / "test_codex_handover.py").resolve(), self.native_path(forwarded[1]))


class LoginChecks(unittest.TestCase):
    """Authentication probes must neither expose credentials nor start a session."""

    def test_login_status_uses_the_selected_profile_without_echoing_account_details(self):
        output = io.StringIO()
        with mock.patch.object(export.shutil, "which", return_value="codex"), \
                mock.patch.object(export.subprocess, "run") as invoke, contextlib.redirect_stdout(output):
            invoke.return_value = subprocess.CompletedProcess([], 0, b"private account", b"API token")
            export.require_login(Path("fixture profile"))
        self.assertEqual(["codex", "login", "status"], invoke.call_args.args[0])
        self.assertEqual("fixture profile", invoke.call_args.kwargs["env"]["CODEX_HOME"])
        self.assertIn("Anmeldung vorhanden", output.getvalue())
        self.assertNotIn("private account", output.getvalue())
        self.assertNotIn("API token", output.getvalue())

    def test_failed_or_unavailable_login_probe_provides_a_local_recovery_command(self):
        for result in (subprocess.CompletedProcess([], 1, b"secret", b"secret"),
                       subprocess.TimeoutExpired("codex", 30), OSError("cannot execute")):
            with self.subTest(result=type(result).__name__), \
                    mock.patch.object(export.shutil, "which", return_value="codex"), \
                    mock.patch.object(export.subprocess, "run") as invoke:
                if isinstance(result, Exception):
                    invoke.side_effect = result
                else:
                    invoke.return_value = result
                with self.assertRaisesRegex(export.HandoverError, "codex login") as failure:
                    export.require_login(Path("fixture profile"))
                self.assertNotIn("secret", str(failure.exception))


class PortableProcessChecks(unittest.TestCase):
    """Check names from native Windows, macOS apps, Linux, and npm launchers."""

    def test_process_classifier_detects_daemons_and_ide_backends_without_matching_our_python(self):
        cases = [
            ("C:\\Users\\me\\codex.exe", "", True),
            ("/usr/bin/codex", "", True),
            ("/Applications/ChatGPT.app/Contents/MacOS/ChatGPT", "", True),
            ("/opt/tools/codex-code-mode-host", "", True),
            ("node.exe", 'node "C:\\npm\\@openai\\codex\\bin\\codex.js"', True),
            ("python3", "python3 codex_handover_export.py", False),
            ("node", "node unrelated.js", False),
            ("/Applications/Visual Studio Code.app/Contents/MacOS/Electron", "", False),
        ]
        for name, command, expected in cases:
            with self.subTest(name=name):
                self.assertEqual(expected, export.is_codex_process(name, command))

    def test_windows_cim_and_posix_ps_results_identify_the_same_backend(self):
        with mock.patch.object(export.os, "name", "nt"), mock.patch.object(export, "run", return_value='[{"ProcessId":7,"Name":"codex.exe","CommandLine":""}]'):
            self.assertEqual([(7, "codex.exe")], export.active_codex())
        with mock.patch.object(export.os, "name", "posix"), mock.patch.object(export, "run", return_value="  7 /usr/bin/codex\n 8 /usr/bin/python3"):
            self.assertEqual([(7, "/usr/bin/codex")], export.active_codex())

    def test_process_probe_failure_is_not_interpreted_as_no_sessions(self):
        with mock.patch.object(export, "run", side_effect=export.HandoverError("ps denied")):
            with self.assertRaisesRegex(export.HandoverError, "ps denied"):
                export.require_quiet()

    def test_ssh_and_https_origin_names_agree_without_retaining_url_credentials(self):
        self.assertEqual(export.canonical_origin("git@example.org:team/trice.git"),
                         export.canonical_origin("https://user:secret@example.org/team/trice.git"))


if __name__ == "__main__":
    unittest.main()
