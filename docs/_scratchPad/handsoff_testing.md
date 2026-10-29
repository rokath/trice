# Testing handover

Updated: 2026-10-29. Current release: v2.0.0-beta-2.

## Goal and repository state

- Make pinned tool preparation practical on Linux, macOS and Windows: reuse complete installations, plan missing tools and space before setup, and keep the startup display short.
- Branch: `wip`; HEAD at refresh: `6ea7c794abf086cb74ea8c8c37cf690ad875d502`. This is the starting revision, not a commit containing the new work.
- Implementation and this handover are uncommitted. No commit or push was executed by the agent. New helper/test files are still untracked and must be included.
- This is the sole active testing handover. The former second file was consolidated here and removed at the user's request. Earlier full-suite/CI PASS results predate this redesign.
- The user's last testAll owner and lock were gone before the final edits. No agent-run check or source restoration is pending.

## Implemented work

- Complete CMake/GCC/LLVM/ARM installations and compatible verified CLI caches are reused in place, including caches from unrelated manifest revisions. No toolchain copying or cache migration is required.
- Missing tools use verified release binaries first; source builds remain only where no declared suitable binary exists. Go bootstrap uses an offline runtime or an approved official archive.
- Local npm setup reuses working global tools and cached dependency groups. Only missing groups use subsets of the existing lockfile; CI keeps the full locked graph. Renderer/Puppeteer resolution and the browser cache are checked consistently.
- Plans cover direct Markdownlint and analyzer entry points. Unlisted tools or disappearing reusable installations cannot silently expand approval. Windows activates the compiler roots found during planning.
- Windows reuses Cppcheck 2.22.0 already in WinLibs. mdtoc validation accepts both `mdtoc 1.0.3` and `mdtoc v1.0.3`, with following metadata lines; other versions remain rejected.
- Preparation is outside test duration. The tracked-worktree guard covers preparation/testing. Failed setup starts no tests.

## Binding decisions

- The user performs final validation. Do not automatically launch testAll, coverage or compiler/integration suites. Focused implementation checks and behavioral tests are allowed.
- Use the existing `temp/tools` directory. The original `test/tools` wording was informal.
- Startup has two short introductory lines, then `Tool / Required / System / Cache / Location / Size`, with a continuous horizontal separator below the header.
- No path paragraph after Go, `external`/`Use`, legends for `--`, repeated `available` list, or zero-size/peak columns. Coexisting system versions go in the System cell.
- Size is approximate additional installed space, e.g. `~150 MB` or `~4.4 GB`; existing tools show `--`. Disk checks still include staging and the configurable reserve per filesystem. Normal output shows free space/sufficiency; only shortages need another message. Estimates are not guarantees.
- Missing prerequisites appear in the table. Always offer `[c] Continue [a] Abort [s] System setup`, default `a`. Blocked `c` names the prerequisite and repeats the same menu without installing or testing. Invalid answers also repeat it.
- `s` means real apt/Homebrew/winget packages, with sudo/admin when needed, only for supported mappings with the exact available pin. Show commands/versions/space and ask again before changes. No silent newer version or checkout fallback. Successful system setup stops for a fresh startup check.
- Preserve strict evidence checks, exact pins and Bash 3.2 compatibility. No unrelated product features, broad cleanup, archive edits or blind ZIP application.
- Avoid interactive PTY/ConPTY previews on this Windows setup. The user reported control characters entering the text input and had to kill the shell after the earlier interactive preview. The cause is not established; use noninteractive diagnostics and inspect saved output instead.

## Relevant files and transfer

- Planning: `scripts/_116_test_tool_plan.sh`, `scripts/_117_system_test_tools.sh`.
- Selection: `scripts/_105_setup_test_tools.sh`, `scripts/_115_prepare_native_tests.sh`, `scripts/testcheck/tools.go`, `scripts/test-tools.env`.
- Integration: `_100`, `_110`, `_260`, `_280`, `_310`, `_320`, `_450`, `_490` scripts; `scripts/_test_npm_tools.cjs`, `scripts/_prepare_reference_manual.cjs`.
- Tests: `scripts/tool_plan_test.go`, `scripts/portability_test.go`, `scripts/testcheck/tools_test.go`, `scripts/testcheck/main_test.go`. The `inspect` CLI was added in `scripts/testcheck/main.go`.
- Documentation: [target_tests.md](../rm/target_tests.md).
- Unrelated user edits in `scratchPad.md` are excluded. The untracked `trice-test-changes.zip` is historical input, not a runtime/continuation prerequisite. No local log or cache must be transferred.

## Validation and limits

- PASS on Windows: `rtk go test ./scripts -run '^(TestMissingPrerequisiteKeepsMenu|TestToolPlanInventorySeparatesSystemAndCache|TestToolPlanConsentAndSpace|TestToolPlanListsOnlyMissingTools|TestToolPlanChecksEachFilesystem|TestRunnerLogsAndCancellation|TestExactSystemPackageSetup)$' -count=1 -timeout=5m` — 66 cases including parent tests. This preceded the last small change hiding the no-install message when a prerequisite is missing.
- PASS after that change: `rtk go test ./scripts -run '^(TestMissingPrerequisiteKeepsMenu|TestToolPlanSizeLabels)$' -count=1 -timeout=2m` — 2 tests. The updated missing-prerequisite case covers the final change.
- PASS: `shellcheck --severity=warning scripts/_116_test_tool_plan.sh scripts/_110_test_runner.sh`; pinned shfmt for those files and diff whitespace checks passed before this handover refresh. Earlier focused mdtoc, npm, resource/cache and setup fixtures passed, but are not a complete final-suite result.
- Windows preview only: source `scripts/_105_setup_test_tools.sh`, set `LOG_DIR=temp/log`, then pipe `a` to `trice_test_tool_plan native`. Expected exit 2, all required tools reusable, no installation, about 66.8 GB free. Terminal mirroring with redirected logs was also verified. Optional evidence: `temp/log/tool-inventory-preview.log`.
- User run `testAll.tp0g7w` was interrupted after step 440 failed on mdtoc's `v` prefix. That identity mismatch is fixed with focused coverage; do not claim the full manual check or current full suite passed.
- User run `testAll.jI7KG4` was stopped before the final layout edits. No complete final testAll result exists for this worktree.
- Linux/macOS branches were simulated in fixtures, not natively accepted. No real apt/Homebrew/winget installation was performed. Historical CI/Windows PASS evidence belongs to older code.
- After the reported terminal problem, a read-only integrity review found no ESC/ANSI controls, NUL bytes, invalid UTF-8, replacement/bidi control characters, changed BOMs or changed line-ending styles in the intended changed text files. `bash -n` passed for all 12 changed shell scripts; `node --check` passed for both changed helpers; `gofmt -l` parsed all six changed Go files without reporting changes. No full suite was run for this review.

## Next steps

- Manually commit the implementation and this handover, then transfer those commits to the other computer, normally with a push. Proposed commands exclude unrelated notes and the input ZIP. The removed second handover was untracked, so it needs no staging/deletion command.
- The user runs `./scripts/testAll.sh`. Inspect failures when results are provided; native Linux/macOS acceptance also remains for the user/CI. Do not launch final validation automatically.
- On `handson testing`, verify branch, HEAD, worktree and relevant files, restore context and wait. Commits containing this work are an expected difference from the starting HEAD recorded above.
