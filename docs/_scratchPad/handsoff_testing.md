# Native testing handover

Updated: 2026-10-27. Goal: reliable native tests on Windows, Linux and macOS, with pinned tools, isolated worktree caches and clear preparation output.

## Verified repository state

* Main checkout: `feature/reliable-native-tests`, HEAD `6a9b841f872b1dd25325de985d8913b4c966bb75`.
* Sibling worktree `../trice_wt_wip`: branch `wip`, HEAD `f256ac6ee38348d0830abdfa4e132f1aa7a3585b`, clean. The user has completed the merge of `feature/reliable-native-tests`; no merge conflicts remain.
* Push status was not verified. Do not assume these commits or local changes have reached another computer.

## Completed and committed

* Native tool versions and complete CMake/compiler packages are pinned. Windows CMake uses MinGW Makefiles. Linux/macOS use upstream Clang 23; Intel macOS builds verified LLVM sources because this release has no upstream Intel macOS binary.
* LLVM source builds show phases and a status every 30 seconds, retain compiler diagnostics, and reuse the completed worktree cache.
* CE integration cases use separate executable paths to avoid relinking over recently executed Windows programs.
* PDF setup fixtures model the pinned Node version and avoid accidental real downloads.
* Compiler warning suppression is narrow; vendor code remains unchanged.
* `AGENTS.md` requires affected-caller analysis, complete affected-package validation after the final edit, isolated test outputs and honest platform verification claims.

## Uncommitted work in the feature checkout

The following five files implement the requested tool overview and clearer installation questions:

* `scripts/_105_setup_test_tools.sh`: prints declared tool versions for the host platform; installation questions accept tool/cache context.
* `scripts/_110_test_runner.sh`: displays the overview before preparation on every run and records it in the summary.
* `scripts/_115_prepare_native_tests.sh`: records the overview in the preparation log and identifies Go, bundle, standalone-tool and npm setup requests.
* `scripts/portability_test.go`: covers platform-dependent overview output, repeated listings without installation, prompt context and runner summary output.
* `docs/rm/target_tests.md`: documents the overview and cache invalidation.

These changes are not yet in `wip`. This handover is an additional uncommitted file.

## Findings and binding decisions

* Worktree-specific caches are explicitly accepted. Keep them separate.
* Standalone-tool cache paths hash the complete `scripts/test-tools.env`. The merge changed the wip cache from `0e33c91adc8abfd5` to `2a22af315f3a0dce`, so even unchanged tool versions required preparation again. Python also changed to 3.15.2. npm has a separate lockfile-based cache. The cache algorithm was not changed by the overview work.
* Warm, valid caches should require no installation answer. Preserve consent, refusal, EOF and CI behavior.
* Git write operations, including `git add`, must be supplied as complete copyable commands. Execute them only if the user explicitly requests execution. A request to resolve conflicts does not authorize staging. Do not repeat the previous erroneous staging/approval attempt.
* Reads, relevant edits and tests are authorized. Avoid routine confirmation questions. Platform restrictions still apply.
* Do not edit a worktree while its test suite owns the tracked-state snapshot. Preserve unrelated running tests. Use `apply_patch` for manual edits.

## Validation and remaining uncertainty

* After the final overview changes, the complete `scripts` package passed in 833.362 seconds. Exact command, run from the feature checkout using cmd.exe:

  ```bat
  set GOCACHE=%CD%\.gocache& go test ./scripts -count=1 -timeout=15m > temp/log/codex_scripts.log 2>&1
  ```

* Changed shell files passed shfmt (`-d -i 2 -ci`) and ShellCheck with the repository's `--severity=warning` threshold. The changed documentation passed pinned markdownlint. `git diff --check` passed. No content changes followed those checks.
* Before the merge, the user reported complete Windows `testAll.sh` PASS on both branches. Earlier full `scripts/testcheck` and `internal/args` package checks passed; the affected CE workflows also passed ten repetitions per worktree.
* The later wip run `temp/log/testAll.2rnKQF` ended `INCOMPLETE` after 322 seconds during preparation. Its inspected log did not establish the termination cause. This is not a full PASS of the merged wip state. The logs remain local evidence, not required transfer files.
* Native macOS/Linux runs of the latest changes, especially an actual Intel macOS LLVM bootstrap, remain unverified here. Simulated platform tests do not replace those runs.

## Next steps

1. On continuation, check branch, HEAD, worktree changes and any active test before acting; the user may have committed or tested during the handover.
2. Provide copyable commands for committing the five overview files as one cohesive change and this handover separately, if requested. Do not execute Git writes implicitly.
3. Transfer the commits before switching computers. To use the new overview on wip, integrate its feature commit after it exists and after any active wip test finishes.
4. Obtain a complete Windows run of the resulting wip state and native Linux/macOS acceptance runs. If preparation remains incomplete, inspect the new run's `tools.log` and summary before changing code.

No local-only helper or cache is required to continue; tracked scripts and the pinned manifests can prepare dependencies again.
