# Native testing handover

Updated: 2026-10-27. Goal: reliable native Windows, Linux and macOS tests with pinned tools, separate worktree caches and clear failure reports.

## Verified repository state

* Current checkout: `wip`, HEAD `12549daf` (merge of `feature/reliable-native-tests`). Clean before this documentation update.
* Sibling `../trice`: `feature/reliable-native-tests`, HEAD `13d11b5a`. The overview, report-helper guard and Bash 3.2 fixture fix are committed and included in `wip`.
* No active testAll or Go test process was found before editing. The earlier macOS suite has finished and its checkout lock is gone.
* Local `wip` matches its remote-tracking ref; the live remote was not queried. Do not treat this as verified transfer to another computer.
* This update changes only this handover and `docs/rm/target_tests.md` / `docs/rm/target_test_internals.md`. They remain uncommitted until the user commits them.

## Completed work

* Native compiler/tool packages are pinned. Windows uses the complete MinGW runtime and CMake's MinGW Makefiles generator. macOS/Linux use upstream LLVM; Intel macOS builds verified sources with phase/progress output and worktree-local reuse.
* Every suite run lists declared tool versions. Preparation questions identify the tool/cache; warm valid tools require no installation answer. Standalone-tool cache keys include the whole tool manifest; npm keys use its lockfile.
* CE compiler cases use separate executable paths to avoid Windows linker locks. PDF fixtures model the pinned Node version without real downloads.
* Failed, partial or empty report-helper builds cannot publish a usable helper path. The runner records preparation failure without trying to execute broken output.
* Commit `8cb5d2c8` corrects `TestReportHelperPublication`: runner success now executes and verifies a real plan step. Its former empty plan triggered `TEST_ALL_PLAN_SCRIPTS[@]: unbound variable` under macOS Bash 3.2 with `set -u`. This was a fixture bug, not a reason to skip production checks.
* Commit `13d11b5a` adds Bash 3.2 and separate native OS validation requirements to `AGENTS.md`.
* The RM chapters cover usage/CI/release testing and CGO/test internals. This documentation update adds shell-version diagnosis, repeated Go failures, report-helper failure behavior and comparable timing measurements. Keep run-specific acceptance results here rather than in the RM.

## Binding decisions

* Keep worktree caches separate. Do not replace native OS tests with Linux containers, retries, sleeps or relaxed assertions.
* Preserve installation consent, refusal, EOF and CI behavior. Reads, relevant edits and tests are authorized; avoid routine confirmation questions. Platform restrictions still apply.
* Supply Git writes as copyable commands unless the user explicitly requests execution. Plain `commit` requests commands only; do not stage or commit automatically.
* Do not edit a worktree while its suite owns the tracked-state snapshot. Preserve unrelated running tests and allow restoration to finish. Use `apply_patch` for manual edits; leave archived material unchanged.

## Validation and remaining uncertainty

| Evidence | Outcome and limit |
| --- | --- |
| macOS ARM full run before the fixture fix, `temp/log/testAll.q5aICb` | FAIL, 2966 seconds. Steps 540 and 550 repeat the same report-publication fixture failure. All compiler/CGO matrices passed, including step 25. Restoration completed. |
| macOS ARM reproduction on the pre-fix feature snapshot | `TestReportHelperPublication` failed with the same Bash 3.2 error. After the fix, five repetitions passed (12.550 seconds). |
| Complete affected `scripts` package after the fix | PASS, 378.574 seconds, native macOS with Bash 3.2 and Go 1.27.3 in an isolated snapshot. Invocation: `GOTOOLCHAIN=go1.27.3 go test ./scripts -count=1 -timeout=20m`, with a separately selected local Go cache. Log in sibling checkout: `../trice/temp/portability-check.zv85qe/scripts-package-check.log`. |
| Final feature-checkout focused test | PASS, 3.173 seconds: `GOTOOLCHAIN=go1.27.3 go test ./scripts -run '^TestReportHelperPublication$' -count=1`, using the same cache. The two changed files were byte-identical to the fully tested snapshot. gofmt, diff whitespace and AGENTS markdownlint checks passed. |
| Windows | Earlier overview revision: complete `scripts` package PASS, 833.362 seconds; user also reported earlier full-suite passes. No native run of the final Bash-fixture revision verified here. |
| Linux / Intel macOS | No native acceptance run of the final revision verified here; Intel LLVM bootstrap remains unverified. |

No full suite has been rerun after the fixture fix. Simulated platform tests do not replace the missing native runs.

Documentation checks: `./scripts/_310_refresh_trice_user_manual.sh check` passed; the three edited files passed markdownlint and `git diff --check`. Offline Lychee with `--include-fragments` checked the two RM chapters: 158 successful links, one archive link excluded by repository configuration, zero errors. No runtime test was repeated for these documentation-only edits.

The reported step 24 is `_640_test_pc_targets_bind.sh`: 391 seconds in the failed macOS run, versus an earlier 405-second run and a warm 164-second run. Go/compiler versions, shared inputs and worktree caches differed. Build/cache overhead is a plausible explanation, not a proven performance diagnosis. No performance code was changed; a comparable warm run is still needed.

## Next steps

1. Recheck HEAD, worktree changes and active tests before continuing; the user may have committed or tested in the meantime.
2. Commit/transfer the documentation updates when requested. No further merge of the Bash fix into this `wip` checkout is needed.
3. Run native acceptance on the same resulting revision for Windows, Linux and macOS; include Intel macOS if claiming that architecture. Inspect actual results and retained logs, not just workflow names. The native workflow runs quick on PRs and full on manual/nightly runs; a branch push alone only triggers the separate script portability checks and other push workflows.
4. For the timing question, compare identical warm commands, tools, job limits and worktree with no concurrent builds. Keep caches intact and distinguish compilation from test execution.

Relevant sources: `scripts/portability_test.go`, `scripts/_110_test_runner.sh`, `scripts/_115_prepare_native_tests.sh`, `scripts/test-tools.env`, `.github/workflows/native-tests.yml`, and the two RM test chapters. No local-only helper or cache is required to continue.
