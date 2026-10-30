# Testing handover

Updated: 2026-10-30. Release context: v2.0.0-beta-2.

## Goal and Git state

- Continue native testing on the next computer with practical, reproducible pinned-tool preparation.
- Branch: `wip`; HEAD: `9ce21052c1c28c53797228b42a54fdd4f340b72c`.
- Implementation is committed, including `94f04992` (Clang selection), `257c2fd5` (test preparation/reporting) and `9ce21052` (documentation).
- Worktree was clean before this refresh. Only this handover is now uncommitted. No add, commit or push was executed by the agent.
- No active testAll lock, running agent check or pending source restoration remains. This is the sole active testing handover.

## Completed work

- Preparation reuses matching installations and verified worktree caches. Missing versions use pinned release packages where available. Node/npm, complete compiler packages and setup budgets share the startup plan.
- Setup reports installation start, completion/failure and a heartbeat every 30 seconds. Skipped system-package details stay in the saved plan's `.system.details` file. Rechecking after updates does not repeat the inventory table.
- shfmt accepts `3.14.1` and `v3.14.1`. Node reuse preserves already selected CLI tools; new local tools take precedence over older system versions. Go publication and Windows Cppcheck fixtures no longer leak host runtime/architecture choices.
- Homebrew-only scenarios are selected before creating subtests, avoiding inappropriate apt/winget skips. NUL bytes remain in raw logs but no longer hide diagnostic lines behind grep's binary-file notice. Strict JSON auditing was retained.
- macOS build setup appends Xcode tools instead of replacing the selected Clang. The G0B1 Insert/Bind checks now use Clang 23.1.4 without warnings.

## Binding behavior and constraints

- The user performs final validation. Do not automatically launch testAll, coverage or compiler/integration suites. Focused implementation checks are allowed.
- Use `temp/tools`, not `test/tools`. Keep exact pins; mdtoc must remain v1.0.3. Do not add dependencies or change pins without a request.
- Show the compact `Tool / Required / System / Cache / Location / Size` inventory once. Keep additional paths/details in logs. Sizes are approximate extra installed space; disk checks also include staging and reserve.
- `c`: reuse matching tools and install missing pinned tool versions into `temp/tools`; never change system packages.
- `u`: offer confirmed upgrades of older managed system tools to available exact pins, never above the pin and never downgrade newer versions. Then recheck and prepare remaining missing pins locally. Declining upgrades returns to the menu; actual upgrade failures stop preparation.
- `a` or empty startup input aborts. Missing host prerequisites block continuation with a concrete explanation.
- After preparation, local runs ask `Format repo? [y/N]`. Yes runs the existing formatter, including manual refresh, Bind IDs and example cleanup. Empty input, EOF or `n` skips formatting. `--no-format` suppresses the question and formatting; CI also skips the question. Format failures start no tests. Authorized formatting becomes the test baseline; subsequent test mutations still fail the state guard.
- Preserve strict evidence checks, compiler warnings and Bash 3.2 compatibility. Do not hide failures with retries, new skip exceptions or broad warning suppression.
- Avoid interactive PTY/ConPTY previews on Windows: an earlier preview caused terminal input problems. Use noninteractive fixtures and saved output.
- On `handson testing`, verify this handover against the checkout, restore context and wait for instructions.

## Validation

- **PASS, native macOS arm64:** the user's full suite at clean commit `9ce21052`, run `temp/log/testAll.hsoJtQ`, finished 2026-10-30 09:26:22 CET. All 25 steps passed, including Go tests, coverage and compiler matrices; tracked-state verification passed. Reported duration: 28,916 seconds. Entry point: `./scripts/testAll.sh`; saved selection is `full`, but optional CLI flags are not recorded in the report.
- Steps 560/570 in that run explicitly report Homebrew Clang 23.1.4 and contain zero compiler warnings. Their earlier Apple Clang 21 / unknown-warning-option problem is resolved.
- **PASS, focused macOS checks:** 90 named tests under modern Bash and 71 under Bash 3.2, with zero failures/skips; both JSON streams passed the strict audit. This preceded the final Clang-path patch; the full-suite PASS above covers the final code.
- **PASS after the Clang-path patch:** the following portable core commands were executed through rtk with a writable local Go-cache override. Platform cases inside fixtures are simulations, not native Windows/Linux acceptance:

```sh
go test ./scripts -run '^(TestClangWorkerPreservesSelectedCompiler|TestClangWorkerDetachesBuildInput|TestExampleCleanupPreservesFailureStatus|TestL432.*)$' -count=1 -timeout=2m
go test ./scripts -run '^(TestClangWorkerPreservesSelectedCompiler|TestClangWorkerDetachesBuildInput|TestExampleCleanupPreservesFailureStatus)$' -count=1 -timeout=2m
```

- The second command used system Bash 3.2 for fixtures. ShellCheck, pinned shfmt, gofmt and diff whitespace checks passed. A read-only environment probe also retained the real Clang 23.1.4 after sourcing build setup.
- Earlier runs `testAll.Q5hqx0` (INCOMPLETE) and `testAll.doWzey` (ABORTED) predate the final successful run; do not treat them as current failures.

## Open limits and next steps

- Native Windows, Linux and Intel macOS acceptance of this revision remains unverified in this session. Latest PR #763 / remote CI status was not rechecked. No real apt/Homebrew/winget upgrade was executed by the agent.
- Known older-cache edge case remains uncorrected: a valid jq can share an old native `bin` directory with unverified actionlint/shellcheck entries, causing `compatibleToolCaches` to reject the directory although individual planning reports jq reusable. The user deleted `temp` before later runs. Final PASS verifies rebuilt/reused current caches, not that mixed-cache scenario; investigate only if it recurs or is requested.
- Commit this handover manually and transfer the commit, normally by pushing. The implementation has no remaining uncommitted files. No local cache, log, input ZIP or personal profile is required for transfer.
- On the next computer, load `handson testing` and verify checkout/tools. The user can then run `./scripts/testAll.sh` or `./scripts/testAll.sh --no-format`; inspect any reported failure before making further changes.

## Relevant files

- Planning/progress: `scripts/_105_setup_test_tools.sh`, `scripts/_116_test_tool_plan.sh`, `scripts/_117_system_test_tools.sh`.
- Runner/selection: `scripts/_110_test_runner.sh`, `scripts/_115_prepare_native_tests.sh`, `scripts/_150_setup_build_environment.sh`, `scripts/_260_format_shell_scripts.sh`.
- Pins/cache audit: `scripts/test-tools.env`, `scripts/testcheck/tools.go`, `scripts/testcheck/main.go`.
- Regression fixtures: `scripts/portability_test.go`, `scripts/tool_plan_test.go`, `scripts/release_preparation_test.go`.
- Usage: [Running the Tests](../rm/target_tests.md), [Maintenance](../rm/maintenance.md).
