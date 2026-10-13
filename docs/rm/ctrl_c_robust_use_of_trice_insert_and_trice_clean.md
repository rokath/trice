# Ctrl-C robust use of `trice insert` and `trice clean`

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Background: GitHub issue #658](#background-github-issue-658)
* [2. What Bash scripts can and cannot protect against](#what-bash-scripts-can-and-cannot-protect-against)
* [3. Recommended build-script ownership rule](#recommended-build-script-ownership-rule)
* [4. Recommended Bash pattern](#recommended-bash-pattern)
* [5. Preserve the build exit code](#preserve-the-build-exit-code)
* [6. Be careful with current working directory changes](#be-careful-with-current-working-directory-changes)
* [7. Prefer Makefile clean targets when available](#prefer-makefile-clean-targets-when-available)
* [8. Example scripts](#example-scripts)
* [9. Summary](#summary)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

`trice insert` and `trice clean` can modify many source files. This is useful for build workflows where IDs are inserted before compilation and removed afterwards, but it also means that interruption handling matters.

This chapter summarizes practical recommendations for robust build scripts and explains the background of GitHub issue #658.

## 1. <a id="background-github-issue-658"></a>Background: GitHub issue #658

GitHub issue #658 discusses the risk that `trice insert` or `trice clean` may be interrupted while source files are being modified.

There are two different kinds of interruption effects:

```text
1. Repository-level mixed state
   Some files are already processed, while others are not.

2. Single-file write-back risk
   A file could be left partially written if it is overwritten directly and
   the process is interrupted at the wrong time.
```

The first case is inconvenient but usually recoverable.

The second case is more serious, because a source file could become empty or incomplete.

The robust tool-side solution is that Trice should write changed files atomically:

```text
1. Write the new content to a temporary file next to the target file.
2. Flush and close the temporary file.
3. Atomically rename it over the target file.
```

The temporary file should be created in the same directory as the target file, for example:

```text
target: src/foo.c
temp:   src/.foo.c.trice-tmp-<pid>-<random>
```

This avoids cross-filesystem rename problems and ensures that the replacement is local to the target file.

The `-cache` mechanism can still be useful for recovery metadata, hashes, transaction manifests, and diagnostics, but it should not be required for the atomic replacement of source files.

## 2. <a id="what-bash-scripts-can-and-cannot-protect-against"></a>What Bash scripts can and cannot protect against

A shell script can improve the workflow by making sure that `trice clean` is called after a successful `trice insert`, even when a build fails or the user presses Ctrl-C.

However, a shell script cannot fully protect against interruption exactly inside the Trice process while Trice is writing one file. That must be solved inside Trice itself with atomic write-back.

Therefore, shell-script robustness and Trice-internal atomic writes solve different parts of the problem:

```text
Bash script:
- Can run cleanup after build failure.
- Can run cleanup after Ctrl-C during make.
- Can avoid leaving the repository intentionally inserted.

Trice implementation:
- Must prevent partially written files.
- Must handle interruption during file write-back safely.
- Can provide transaction/recovery diagnostics.
```

## 3. <a id="recommended-build-script-ownership-rule"></a>Recommended build-script ownership rule

Only one script level should own the full sequence:

```text
trice clean -> trice insert -> build -> trice clean
```

If an outer script calls many inner build scripts, and each inner build script already performs its own `trice insert` and `trice clean`, then the outer script should not also perform one global insert/clean around the whole sequence.

Otherwise both levels modify the same source tree, which can cause confusing behavior such as cleanup happening while another build step still expects inserted IDs.

Recommended ownership models:

```text
Model A: Outer script owns Trice state
------------------------------------
outer script:
  trice clean
  trice insert
  run all builds
  trice clean

inner build scripts:
  do not call trice insert/clean


Model B: Inner scripts own Trice state
-------------------------------------
outer script:
  run child build scripts
  optionally run final safety clean

inner build script:
  trice clean
  trice insert
  build
  trice clean
```

Do not mix both models unintentionally.

## 4. <a id="recommended-bash-pattern"></a>Recommended Bash pattern

The following pattern is a simplified example. It keeps cleanup in one place and makes the normal success path use the same cleanup logic as error and interruption paths.

```bash
#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd -- "${SCRIPT_DIR}/../.." && pwd)"

ids_inserted=0

run_trice_clean_if_needed() {
  if [ "${ids_inserted}" -eq 1 ]; then
    echo "cleanup: running trice clean"

    local clean_status=0
    (
      cd "${ROOT}" || exit 1
      bash "${ROOT}/scripts/_240_legacy_clean_ids.sh"
    ) || clean_status=$?

    if [ "${clean_status}" -ne 0 ]; then
      echo "warning: cleanup: trice clean failed with exit code ${clean_status}" >&2
      return "${clean_status}"
    fi

    ids_inserted=0
  fi

  return 0
}

cleanup_and_exit() {
  local status="${1:-$?}"
  local clean_status=0

  trap - INT TERM EXIT

  run_trice_clean_if_needed || clean_status=$?
  if [ "${status}" -eq 0 ] && [ "${clean_status}" -ne 0 ]; then
    status="${clean_status}"
  fi

  exit "${status}"
}

trap 'cleanup_and_exit $?' EXIT
trap 'cleanup_and_exit 130' INT
trap 'cleanup_and_exit 143' TERM

(
  cd "${ROOT}" || exit 1
  bash "${ROOT}/scripts/_240_legacy_clean_ids.sh"
  bash "${ROOT}/scripts/_230_legacy_insert_ids.sh"
)

ids_inserted=1

cd "${SCRIPT_DIR}"
make

clean_status=0
run_trice_clean_if_needed || clean_status=$?

trap - INT TERM EXIT
exit "${clean_status}"
```

Important points in this pattern:

```text
- ids_inserted becomes 1 only after insert completed successfully.
- cleanup runs clean only if insert completed successfully.
- cleanup disables traps first to avoid recursive cleanup.
- helper scripts run from a controlled directory.
- directory changes for helper calls are done inside subshells where possible.
- the normal success path and abnormal paths use the same cleanup helper.
```

## 5. <a id="preserve-the-build-exit-code"></a>Preserve the build exit code

If the build command may fail and the script still needs to run cleanup afterwards, do not let `set -e` abort before the exit code is captured.

For example:

```bash
set +e
make ${MAKE_JOBS} TRICE_FLAGS="${flags}" gcc
make_status=$?
set -e

clean_status=0
run_trice_clean_if_needed || clean_status=$?
if [ "${make_status}" -eq 0 ] && [ "${clean_status}" -ne 0 ]; then
  make_status="${clean_status}"
fi

trap - INT TERM EXIT
exit "${make_status}"
```

This keeps the original build result unless cleanup itself fails after an otherwise successful build.

## 6. <a id="be-careful-with-current-working-directory-changes"></a>Be careful with current working directory changes

A subtle problem can occur when a cleanup helper changes the current working directory and does not change it back.

For example:

```bash
run_trice_clean_if_needed() {
  cd "${ROOT}" || exit 1
  bash "${ROOT}/scripts/_240_legacy_clean_ids.sh"
}
```

After this function returns, the caller is still in `${ROOT}`.

If the script later runs:

```bash
make clean
```

it may accidentally run in the repository root instead of the example directory.

Prefer one of these forms:

```bash
(
  cd "${ROOT}" || exit 1
  bash "${ROOT}/scripts/_240_legacy_clean_ids.sh"
)
```

or explicitly return to the build directory:

```bash
cd "${SCRIPT_DIR}"
make clean
```

## 7. <a id="prefer-makefile-clean-targets-when-available"></a>Prefer Makefile `clean` targets when available

If an example Makefile provides a clean target, prefer:

```bash
make clean
```

over hard-coded shell cleanup such as:

```bash
rm -rf out out.gcc
```

The Makefile knows the actual build output directories, for example:

```make
.PHONY: clean

clean:
	@rm -rf "$(GCC_BUILD)" "$(CLANG_BUILD)"
```

A direct `rm -rf out out.gcc` can be used as a fallback, but it is less precise.

## 8. <a id="example-scripts"></a>Example scripts

The following scripts are useful examples for Ctrl-C robust wrapping of `trice insert` and `trice clean`.

They illustrate slightly different situations:

```text
scripts/_160_pc_target_test_worker.sh
  PC/CGO test wrapper.
  Shows how an outer test script can run cleanup after insert and restore
  temporary environment changes such as C_INCLUDE_PATH.

scripts/_200_gcc_example_build_worker.sh
  First Step-12 variant.
  Shows a global outer-script cleanup owner.

scripts/_210_gcc_example_builds_all_workflows.sh
  Improved Step-12 orchestrator variant.
  Shows the case where child example build scripts own insert/clean, while the
  outer script only runs a final safety clean.

build_trice_safe_cleanup.sh
  Generic example build script.
  Shows pre-clean, insert, build, and final cleanup in one script.

build_with_clang_trice_safe_cleanup.sh
  Clang build script.
  Shows the same cleanup pattern for a clang build target.

build_pattern_preserving_trice_safe_cleanup.sh
  Pattern-preserving GCC build script with TRICE_OFF handling.
  Shows conditional insert behavior.

build_gcc_preserve_make_exit_trice_safe_cleanup.sh
  GCC build script that preserves the make exit code.
  Shows how to temporarily disable set -e around make and still run cleanup.

G0B1_inst_build_fixed_cwd_cleanup.sh
  Corrected G0B1_inst build script.
  Shows how to avoid current-working-directory bugs by running helper commands
  in subshells and returning to the example directory before make clean.
```

These scripts are examples for build-wrapper robustness. They do not replace the need for atomic file write-back inside Trice itself.

## 9. <a id="summary"></a>Summary

Recommended practical rules:

```text
1. Let exactly one script level own each insert/build/clean sequence.
2. Set a state flag only after trice insert completed successfully.
3. Run trice clean on every exit path after successful insert.
4. Disable traps at the beginning of cleanup to avoid recursion.
5. Preserve the original build exit code where needed.
6. Use subshells for cleanup helper calls that change directories.
7. Prefer Makefile clean targets over hard-coded rm -rf.
8. Treat Bash cleanup as a workflow aid, not as a substitute for atomic writes.
```

For the core safety issue, the Trice implementation should still ensure:

```text
A source file is never left partially written after Ctrl-C, SIGTERM, crash, or write error.
```

<p align="right">(<a href="#ctrl-c-robust-use-of-trice-insert-and-trice-clean">back to top</a>)</p>



