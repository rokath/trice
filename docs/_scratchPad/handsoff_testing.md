# Testing handover

Updated: 2026-10-30. Release context: v2.0.0-beta-2.

## Goal and Git state

- Continue native testing with reproducible pinned-tool preparation and a compact startup inventory.
- Branch: `wip`; HEAD at refresh: `808fe4e9c1fa44036856f6f4b6217c757cca7e46`.
- The tool-preparation redesign and macOS Clang fixes are committed through `9ce21052`; `808fe4e9` only refreshed this handover.
- Four implementation/documentation files are now uncommitted: `scripts/portability_test.go`, `scripts/_110_test_runner.sh`, `scripts/_116_test_tool_plan.sh`, `docs/rm/target_tests.md`. This handover is a fifth change. No staging, commit or push was executed by the agent.
- No active testAll lock, agent check or pending restoration remains. This is the sole active testing handover. No local cache, log, ZIP or personal profile is required for transfer.

## Committed behavior to preserve

- Reuse matching installations and verified worktree caches. Missing versions use pinned release packages where available; source builds are fallback only where required.
- The inventory appears once. Rechecking after system updates does not repeat it. Installation start/completion/failure and a 30-second heartbeat stay visible; detailed command output remains in logs.
- Node reuse preserves selected CLI precedence. shfmt accepts the pinned version with or without a `v` prefix. CMake/GCC/LLVM/ARM packages retain complete resources.
- macOS build setup appends Xcode tools rather than replacing the selected Clang. Homebrew-only fixture cases are selected before creating subtests. NUL bytes remain in raw logs without hiding readable failure diagnostics.

## New uncommitted fixes

- Windows Cppcheck fixtures inherited a real 2.22.0 installation through ProgramFiles, bypassing their older doubles and expected installer/refusal paths. Git Bash repopulated PROGRAMFILES even when Go supplied an empty value.
- `runFixture` now removes inherited aliases, normalizes explicit ProgramFiles overrides and reapplies the controlled value after Bash starts using `TRICE_FIXTURE_PROGRAMFILES`. Production Cppcheck discovery/pins are unchanged. Regression cases cover both spellings, a matching host installation and explicitly enabled fixture paths with spaces.
- Setup refusal is distinct from real preparation failure via `TRICE_TEST_SETUP_CANCELLED`. Choosing `a`, empty input or EOF prints `Cancelled.` once. The runner does not replay an already mirrored inventory or call refusal a preparation failure.
- Captured/noninteractive cancellation still replays its log once, while genuine setup failures keep their full diagnostics. Cancellation starts no tests, remains INCOMPLETE/exit 2 and records a specific reason in results.json.
- New runner tests use the real planner and runner with doubles, including mirrored/captured output, explicit/empty/EOF cancellation, successful continuation and genuine failure. Descriptor 9 models terminal output; descriptor 3 belongs to tracked-state hashing. No real PTY is used.
- The test manual explains package-level reuse: WinLibs needed for GCC/Clang also brings CMake/Cppcheck. System and Cache show independent presence; Location selects the used copy. A cached CMake 4.4.4 alongside system CMake 4.4.4 is valid, not a separate CMake download. Its space is counted with WinLibs.

## Binding user decisions

- The user performs final validation. Do not automatically run testAll, coverage or compiler/integration suites. Focused implementation checks are allowed.
- Use `temp/tools`; retain exact pins, including mdtoc v1.0.3. No unrequested dependencies, pin changes or cache cleanup/migration.
- Startup: two short introductory lines; `Tool / Required / System / Cache / Location / Size` table with a continuous horizontal separator. No Go path paragraph, `external`/`Use`, legends, repeated `available` list or zero-size/peak columns.
- Approximate Size is extra installed space in MB/GB; existing tools show `--`. Staging/reserve checks stay internal. Normal disk output shows free space/sufficiency; shortages name the additional space needed.
- `c`: reuse matching tools and install missing pinned versions locally, without system changes.
- `u`: offer confirmed upgrades of older managed system tools to available exact pins; never exceed pins or downgrade newer versions. Recheck afterwards and prepare remaining pins locally. Declining returns to the menu; real upgrade failures stop preparation.
- `a` or empty input aborts. The c/u/a menu remains unchanged when prerequisites are missing; blocked continuation explains the reason.
- After preparation, `Format repo? [y/N]` offers the existing formatter. Empty input, EOF or n skips it; `--no-format` and CI suppress it. Format failures start no tests. Authorized formatting becomes the test baseline; later mutations still fail the tracked-state guard.
- Preserve Bash 3.2 compatibility, compiler warnings and strict evidence. Do not manufacture PASS with retries, new skip exceptions or relaxed checks.
- Avoid interactive PTY/ConPTY previews on Windows: an earlier preview caused terminal-input problems. Use noninteractive fixtures and saved output.

## Validation

- Historical **PASS, native macOS arm64**, reported by the user: clean `9ce21052`, `temp/log/testAll.hsoJtQ`, all 25 steps, 28,916 seconds, finished 2026-10-30 09:26:22 CET. Steps 560/570 used Homebrew Clang 23.1.4 without warnings. This predates the new uncommitted fixes.
- Earlier focused macOS checks passed under modern Bash and Bash 3.2. These are not native acceptance of the new Windows-fixture/cancellation changes.
- Windows `testAll.NXYY7D` was interrupted after step 540 failed on Cppcheck fixtures; step 520 passed. The fixture defect is corrected locally, but no new complete Windows PASS has been supplied.
- Windows `testAll.7pkKW1` was cancelled at startup. Its duplicate table exposed the corrected refusal path; it performed no tests.
- **PASS, focused Windows checks**, run through rtk, without real installations or compiler matrices:

```sh
rtk go test ./scripts -run '^(TestCppcheckFixturesIgnoreHostPackages|TestCppcheckPinnedVersion|TestCppcheckLocalCache|TestCppcheckWindowsSetup)$' -count=1 -timeout=2m
rtk go test ./scripts -run '^(TestCppcheckFixturesIgnoreHostPackages|TestPinnedSystemToolUpdates|TestSystemUpgradeVersionBounds|TestToolPlanInventorySeparatesSystemAndCache|TestToolPlanListsOnlyMissingTools)$' -count=1 -timeout=3m
rtk go test ./scripts -run '^(TestRunnerSetupCancellation|TestRunnerLogsAndCancellation|TestRunnerDiagnosticsWithNUL|TestRunnerOffersRepositoryFormatting|TestToolPlanConsentAndSpace|TestMissingPrerequisiteKeepsMenu)$' -count=1 -timeout=4m
```

- Outcomes respectively: 20, 55 and 59 passing cases including parent tests. The first two validate the fixture fix; the third is the latest cancellation/runner check. Platform branches inside Windows fixtures are simulations, not native macOS/Linux runs.
- PASS: ShellCheck at warning severity and pinned shfmt for both changed shell files; gofmt for the changed Go file; diff whitespace checks. No full suite, real apt/Homebrew/winget upgrade or interactive terminal preview was run for these fixes.
- Initial attempts failed because clearing the environment alone did not survive Git-Bash startup, and because the first mirror fixture reused descriptor 3. Both causes were corrected; the passing runs above supersede those attempts.

## Open limits and next steps

- The user should run `./scripts/testAll.sh` or `./scripts/testAll.sh --no-format` next. Inspect reported failures before further edits. Current uncommitted changes have no complete native acceptance on Windows, Linux, Intel macOS or Apple Silicon.
- Preserve the historical macOS PASS, but do not attribute it to these subsequent edits. Remote CI was not rechecked.
- Deferred mixed-cache edge: an old native bin directory containing valid jq plus unverified actionlint/shellcheck can be rejected by `compatibleToolCaches` despite individual jq planning reporting reuse. Investigate only if it recurs or is requested; do not silently repair/migrate caches.
- Manually commit the four changed files and this handover, then transfer those commits, normally with a push. New commits are an expected difference from the HEAD recorded above.
- On `handson testing`, compare branch, HEAD, worktree and relevant files, restore context and wait for the next instruction.

## Relevant files

- Current fixes: `scripts/portability_test.go`, `scripts/_110_test_runner.sh`, `scripts/_116_test_tool_plan.sh`, [target_tests.md](../rm/target_tests.md).
- Shared preparation: `scripts/_105_setup_test_tools.sh`, `scripts/_115_prepare_native_tests.sh`, `scripts/_117_system_test_tools.sh`.
- Compiler/cache behavior: `scripts/_150_setup_build_environment.sh`, `scripts/testcheck/tools.go`, `scripts/test-tools.env`.
