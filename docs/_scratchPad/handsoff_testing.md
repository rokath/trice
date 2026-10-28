# Native testing handover

Updated: 2026-10-28. Continue PR #763 (`wip` into `main`, `rokath/trice`). The first three fixes are complete. One Library CI preparation failure remains unresolved. The user requested its correction and an explanation of two skipped checks, then prioritized an immediate computer switch. Stop here; no further corrective patch was started.

## Repository and transfer state

- Branch: `wip`; HEAD: `92a830832cdfb883d2547f4742830eb99b8ebfd1`.
- GitHub PR metadata independently confirmed this head. The three fixes below are committed and transferred.
- The worktree was clean before this refresh. Only this handover is now an intended uncommitted change; add, commit and push it manually.
- The local full suite finished, verified tracked state and removed its lock. No test was cancelled, and no source restoration remains pending.
- No local-only file is required on the next computer. Logs and downloaded CI artifacts are optional evidence and can be retrieved again.

## Completed fixes

| Commit | Result |
| --- | --- |
| `593eed24` | Separate Go JSON stdout from stderr, retaining diagnostic files and readable logs. Preserve strict JSON, required-test and exit-status checks. Regression tests include genuine cold/warm module downloads from a local proxy. |
| `4859086b` | Detect tools that stop working after copying, such as relative launchers, and use the declared verified package. Preserve startup/version errors instead of `<nil>`. Tests cover refusal, cold/warm caches and failed preparation. |
| `92a83083` | Regenerate TOCs and anchors in 16 RM chapters with **mdtoc v1.0.3**, restoring existing links and inline formatting. |

The older M3 PASS at `f34386a9` preceded the damaged documentation at `bda238ae`; warm Go caches also hid the mixed stderr/JSON defect. This did not establish a CPU-specific problem. The global mdtoc was an old development build; the project-cache binary was verified through Go build metadata as `github.com/rokath/mdtoc v1.0.3`, without replacements.

## Validation already completed

| Check | Exact outcome |
| --- | --- |
| User full suite, current committed head | **PASS, 25/25**, 3227 seconds. `temp/log/testAll.gmlEKL/results.json` records `92a83083`, clean worktree, selection `full`. Finished 2026-10-28 09:09:57 CET; state verification passed, lock removed. |
| Earlier `./scripts/testAll.sh quick` | **PASS, 19/19**, 1772 seconds. `temp/log/testAll.tMTbDn/results.json`; fixes were then uncommitted on `bda238ae`. Includes complete Go tests, coverage and compiler/example checks. |
| Complete `scripts` package under Bash 3.2 | **PASS**, 624.857 seconds, with prepared pinned Node/npm tools and `TRICE_TEST_NPM_DIR`. |
| `go test ./scripts/testcheck -count=1 -timeout=10m` | **PASS**, 13.774 seconds through the approved path supporting local TCP test servers. |
| Manual format and markdownlint | **PASS**, using verified mdtoc v1.0.3 and pinned npm tools. |
| `./scripts/_530_test_links.sh` | **PASS**, 1950 links, zero errors. |
| Reference-PDF preparation, stitchmd assembly and `node scripts/_prepare_reference_manual.cjs --verify` | **PASS**, 37 sources, 850 internal links, 1199 unique anchors. The local check did not render PDFs; the new CI release-PDF check passed. |

The quick coverage step reported **90.0% Go statements**, a different measure from Coveralls line coverage. Earlier standalone attempts encountered stale npx offline metadata or sandbox denial of a local TCP listener; complete package checks subsequently passed with the prepared environment/approved execution path. Do not relax tests to hide these prerequisite failures.

## CI at the current head

PR head `92a83083`; synthetic merge `2a4810db4b1ec60845d6d805ae57195445eef28c`. Recheck live status when continuing.

- **PASS:** Native Linux, Windows and macOS ARM, run `40649696464`. Jobs: Linux `122686630822`, Windows `122686631194`, macOS `122686630688`.
- **PASS:** Script portability on all three OS (`40649696541`), install/snapshot checks including release PDFs (`40649696602`), Link Check (`40649696543`), Super Linter (`40649696397`), Go Coverage (`40649696531`), CodeQL, ShellCheck, shfmt and clang-format.
- **PASS:** [Coveralls build 82654874](https://coveralls.io/builds/82654874). The old red Coveralls result is not a current failure.
- **FAIL:** Library CI `40649696566`, job `122686631122`, described below.

The two skipped checks are intentional, not substitutes for passing tests:

- `Native macOS Intel`: `.github/workflows/native-tests.yml` uses `if: github.event_name != 'pull_request'`. Intel runs on schedule/manual dispatch; PRs run the three-OS matrix above.
- `Guard - skip if no commits in last 24h`: `trice_lib_ci.yml` runs this guard only for `schedule`. PR/manual runs bypass the activity gate. The heavy job explicitly runs despite the skipped dependency; its actual failure is visible.

## Remaining Library failure

- Failed step: `Verify productive CE/SL and feature examples`, executing `./scripts/_515_test_logging_features.sh full`.
- Public log: `FAIL: CE/SL integration failed in ./internal/args`, exit **2**, after about 7m37s. There is no `+ go test` line.
- Source flow indicates failure in `prepare_native_test_tools` before the Go invocation: `run_go_tests` redirects preparation output into `temp/log/_515_test_logging_features.log`. **The underlying failing prerequisite is not known.** Do not describe this as recurrence of the corrected JSON parser bug.
- Artifact upload was skipped after failure; the run has no artifacts. The build orchestrator never ran.
- The successful native Linux artifact `12799795356` contains `testAll.5GyTJR/tools.log`, including cold compiler/Cppcheck preparation. It selected Python 3.15.2 and runner image `20261023.334.3`; Library used image `20261012.334.1` and system Python 3.12.3. These differences are evidence, not a proven cause.
- Do not blindly add venv as the assumed fix: the Library runner image's upstream Python installer already installs `python3-venv`.

## Next steps and constraints

1. Make the Library preparation failure visible and retain its setup log even when the job fails; inspect the actual prerequisite error before choosing a functional correction. Relevant files: `.github/workflows/trice_lib_reusable.yml`, `scripts/_515_test_logging_features.sh`, `scripts/_100_test_common.sh`, `scripts/_115_prepare_native_tests.sh`, `scripts/_105_setup_test_tools.sh`.
2. Use the GitHub connector for current jobs/logs/artifacts. No new workflow, helper or test changes were made for this remaining failure.
3. Add focused regression coverage in the existing script tests as needed, then validate the complete affected packages and relevant workflow checks. Existing guard tests are in `scripts/release_preparation_test.go`.
4. Keep mdtoc **v1.0.3**, native three-OS checks, Bash 3.2 compatibility and strict evidence checks. Do not manufacture PASS through retries, sleeps, skips or weakened assertions.
5. Separate observation, unchanged: explicit `mdtoc generate` in `_310` can replace stored chapter options with defaults. Incidental edits to `target_tests.md` and `target_test_internals.md` were reverted before validation; preserve their settings.
6. Do not edit a worktree owned by a running suite. Plain `commit` means copyable commands; no actual commit or push without explicit execution authorization.
7. On `handson testing`, verify branch, HEAD, worktree and relevant files, then wait for the next instruction.
