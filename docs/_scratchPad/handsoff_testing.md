# Native testing handover

Updated: 2026-10-28. Native testing work is complete. GitHub CI and the user's full Windows run pass. PR #763 is merged. No further implementation is pending for the previously failing Library setup.

## Repository and transfer state

- Branch: `wip`; HEAD: `b7002d6c0d785968c7b31cfec052fd96e4a6845e`.
- Worktree was clean before this refresh. Only this handover is now an intended uncommitted change.
- GitHub confirms PR #763 merged at `e0881a6bb2e5ba5ae3b0594677b249e33559e1b8`; merge commit: `69ad733c479f3da90ba264e963fcf6b4ed9a3383`.
- Local commits after that tested code revision change only discussion notes and the recorded Windows test log. They are already committed locally; their remote transfer was not independently verified.
- No testAll lock remains. The agent-started tests have finished successfully; no cancellation or source restoration is pending.
- No local-only file is required to continue. Logs and tool caches under `temp/` are optional evidence; the user's Windows console log is committed in [scratchPad.md](scratchPad.md).

## Completed work

- Earlier committed fixes separate Go JSON stdout from stderr, replace copied tools that cannot execute with verified pinned packages, and regenerate manual anchors/TOCs using **mdtoc v1.0.3**.
- `run_go_tests` now retains setup stdout/stderr in a separate `.setup` log and replays it into the readable step log, including on failure. Preparation stays in the caller shell so selected tool paths reach the tests.
- Library CI now uploads `temp/log/` through an independent `always()` step, even when preparation fails before compiler artifacts exist.
- Regression tests cover setup success/failure, quiet/visible output, retained shell environment, and refusing to start Go tests after failed setup.
- `AGENTS.md` now records the binding user preference: the user runs final validation. Do not automatically start final suites, coverage, or compiler/integration checks; run them only on explicit request. Focused implementation/debugging checks and writing behavioral tests remain allowed.

Relevant files: `scripts/_100_test_common.sh`, `.github/workflows/trice_lib_reusable.yml`, `scripts/testcheck/main_test.go`, `scripts/release_preparation_test.go`, `AGENTS.md`.

## Confirmed validation

| Check | Outcome and evidence |
| --- | --- |
| User `./scripts/testAll.sh` on Windows amd64 | **PASS, 25/25**, selection `full`, 9592 seconds. `temp/log/testAll.cQDHeg/results.json` records clean revision `e0881a6b`. Summary confirms tracked-state verification and completion. Console log is committed in `scratchPad.md`. |
| GitHub CI at `e0881a6b` | Connector independently confirms all 11 returned PR workflows completed successfully, including Native Trice tests, Library CI, Script portability, install-checks, coverage, links, CodeQL and linters. |
| Native Trice tests | **PASS**, run `40670589530`. |
| Library CI | **PASS**, run `40670589535`; the earlier preparation failure is no longer an active failing check. |
| Script portability | **PASS**, run `40670589498`. |
| Agent `go test ./scripts/testcheck -count=1 -timeout=10m` | **PASS** under Windows after the diagnostic patch. |
| Agent `go test ./scripts -count=1 -timeout=15m` | **PASS** under Windows; started before the user changed the final-testing preference. |
| Agent `./scripts/_515_test_logging_features.sh full` | **PASS** under Windows, including CE/SL integration, Bind entry points, PC output and G0B1 builds with GCC/Clang. Started before the preference change. |
| Pinned actionlint, ShellCheck warning/error level, shfmt, shell syntax and diff whitespace | **PASS** for the changed workflow/helper before the preference change. |

The current HEAD adds documentation only after `e0881a6b`; do not describe those later documentation commits as the revision tested by the full suite. The result file's embedded preparation context still names older commit `b1aeabae`; its top-level commit field records `e0881a6b`. This is an observed metadata discrepancy, not a requested repair.

## Findings and limits

- The initial Windows ARM setup failed while renaming the extracted package with "Access is denied." The same archive installed successfully through unchanged code, and warm selection plus the later full suite passed. The original cause was not established; no retry, sleep or speculative installer workaround was added.
- The earlier Library failure occurred during preparation before Go tests. Improved diagnostics were implemented; subsequent CI passes. The exact original failing prerequisite remains unknown. Do not attribute it to the already corrected JSON parser issue or invent a Python/venv cause.
- Two skipped CI jobs are intentional: macOS Intel runs on schedule/manual dispatch rather than PRs; the 24-hour activity guard applies only to scheduled Library runs. The Library job runs after a skipped guard for PR/manual events.
- Preserve mdtoc **v1.0.3**, strict evidence checks, native platform coverage and Bash 3.2 compatibility. Do not weaken assertions or skip failures to manufacture PASS.
- Deferred observation only: explicit `mdtoc generate` in `_310` can replace stored chapter options with defaults. Preserve existing chapter settings; no change was authorized for this observation.
- G0B1 firmware was compiled; real MCU execution remains a separate board test.

## Next steps

- Add and commit this handover manually, then transfer the commit to the other computer, normally by pushing. Ensure the two existing local documentation commits are transferred too.
- On `handson testing`, compare the recorded branch, HEAD, worktree and relevant files with the actual checkout, restore this context, and wait for the next user instruction.
- No new testing fix or final validation run is currently required. Do not turn unrelated discussion notes into an implementation task.
