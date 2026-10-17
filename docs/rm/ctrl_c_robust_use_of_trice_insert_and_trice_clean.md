# Safe Build Scripts and Cancellation

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Background: GitHub issue #658](#background-github-issue-658)
* [2. Managed repository tests](#managed-repository-tests)
* [3. Recommended build-script ownership rule](#recommended-build-script-ownership-rule)
* [4. Recommended Bash pattern](#recommended-bash-pattern)
  * [4.1. Deprecated Insert/Clean builds](#deprecated-insertclean-builds)
* [5. Preserve the build exit code](#preserve-the-build-exit-code)
* [6. Be careful with current working directory changes](#be-careful-with-current-working-directory-changes)
* [7. Prefer Makefile clean targets when available](#prefer-makefile-clean-targets-when-available)
* [8. Example scripts](#example-scripts)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

Use [Bind](bind.md#build-integration) for normal builds: bind, compile, flash. There is no post-build Clean step. The [deprecated Insert/Clean workflow](insert_clean_cache.md) rewrites calls and needs additional cleanup.

## 1. <a id="background-github-issue-658"></a>Background: GitHub issue #658

An interruption has two different scopes:

| Scope | Protection |
| --- | --- |
| One changed file | The [atomic writer](../../internal/id/helper.go) writes a temporary sibling, flushes and closes it, then renames it over the original. An unsuccessful replacement leaves the old file intact. |
| A whole preparation/build run | A wrapper must own preparation, child processes and restoration. Atomic individual writes do not make a multi-file run one indivisible operation. |

Insert, Clean and dictionary writes use this atomic writer. Bind additionally rolls back already published files when a handled publication error occurs. This does not guarantee a complete multi-file rollback after process termination or power loss. The deprecated `-cache` is a timestamp-preserving build optimization, not a transaction journal.

## 2. <a id="managed-repository-tests"></a>Managed repository tests

The [test-state helper](../../scripts/_140_trice_test_state.sh) captures initial source bytes, TIL/LI and generated-header state before preparation. On success, failure or cancellation it restores that state, including local edits present at the start.

The [runner](../../scripts/_110_test_runner.sh) stops starting new work after Ctrl-C. Managed workers stop or drain their children before restoring sources, so a surviving compiler cannot race with cleanup. During restoration, further INT/TERM signals are ignored; pressing Ctrl-C repeatedly must not interrupt that restoration.

Wait for the final result and any restoration message. A forced kill, power loss or failed restore needs manual inspection of the retained recovery directory. Running Bind or `format_repo.sh` creates a usable Bind state, but does **not** recover the exact pre-test bytes.

Do not edit shared test sources or run formatting/preparation commands concurrently with these tests.

## 3. <a id="recommended-build-script-ownership-rule"></a>Recommended build-script ownership rule

Exactly one level owns each shared source tree:

| Owner | Responsibilities |
| --- | --- |
| Outer wrapper | Prepare once, start builds, wait for all children, restore once. Children do not prepare or restore shared inputs. |
| Individual build | Prepare and restore its own isolated inputs. An outer orchestrator only starts and waits for builds. |

For a normal application build, Bind is a compilation dependency. For tests that temporarily change workflows, use a snapshot owner like the repository helper. Do not mix global Insert/Clean with child scripts that also modify the same sources.

## 4. <a id="recommended-bash-pattern"></a>Recommended Bash pattern

This is a cleanup pattern, not a complete replacement for the managed helper. `restore_initial_state` stands for the wrapper's restoration function; call this after its children have stopped.

```bash
finish() {
    local status="$1"
    local restore_status=0

    # Prevent another Ctrl-C from interrupting restoration.
    trap '' INT TERM
    trap - EXIT
    restore_initial_state || restore_status=$?

    # Preserve a failed build; otherwise report restoration failure.
    if [ "$status" -eq 0 ] && [ "$restore_status" -ne 0 ]; then
        status="$restore_status"
    fi
    exit "$status"
}

trap 'finish $?' EXIT
trap 'finish 130' INT
trap 'finish 143' TERM
```

Arm restoration **before** the first mutation, not only after preparation succeeds. A failed preparation may have changed some files already. Report restoration failures and retain recovery data.

### 4.1. <a id="deprecated-insertclean-builds"></a>Deprecated Insert/Clean builds

For a deliberately clean-only legacy source tree, an application wrapper may use Clean after Insert/build. This is weaker than snapshot restoration: it cannot preserve arbitrary initial explicit IDs or local edits made during the run. When exact restoration matters, capture and restore the initial state instead.

## 5. <a id="preserve-the-build-exit-code"></a>Preserve the build exit code

Capture build failures when more commands must run afterwards:

```bash
make_status=0
make || make_status=$?
# The EXIT handler performs restoration and preserves make_status.
exit "$make_status"
```

A cleanup failure must not turn an already failed build into success. Conversely, a successful build with failed restoration must report failure.

## 6. <a id="be-careful-with-current-working-directory-changes"></a>Be careful with current working directory changes

Stabilize the working directory near the script entry. If a helper needs a different directory, use a subshell so the caller's directory remains unchanged:

```bash
(
    cd "$project_root" || exit 1
    run_project_cleanup
)
```

Otherwise a later `make clean` can operate in the wrong directory.

## 7. <a id="prefer-makefile-clean-targets-when-available"></a>Prefer Makefile clean targets when available

Use `make clean` for build outputs when the project provides it. It knows the configured output paths. Build-output cleanup and restoration of modified sources are separate tasks.

## 8. <a id="example-scripts"></a>Example scripts

| Script | Responsibility |
| --- | --- |
| [test runner](../../scripts/_110_test_runner.sh) | Run steps, stop after cancellation and verify tracked state. |
| [test-state helper](../../scripts/_140_trice_test_state.sh) | Snapshot ownership, signal handling and exact restoration. |
| [PC worker](../../scripts/_160_pc_target_test_worker.sh) | Per-configuration CGO execution and environment. |
| [GCC example worker](../../scripts/_200_gcc_example_build_worker.sh) | Build a selected example configuration. |
| [GCC workflow orchestrator](../../scripts/_210_gcc_example_builds_all_workflows.sh) | Coordinate the selected ID workflows and child builds. |

