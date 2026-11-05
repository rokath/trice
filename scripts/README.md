# Repository scripts

Public shell commands intended for manual use have no leading underscore. Internal shell, Python and Node.js helpers start with `_`, including the numbered test steps. Helpers may also be run directly for focused maintenance or diagnostics.

Run the public commands with Bash, or Git Bash on Windows, from the repository root:

Entry point                                                                  | Purpose
---------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------
[`./scripts/testAll.sh`](testAll.sh)                                         | Run the test suite; see [test commands and options](../docs/rm/target_tests.md#how-to-run-the-tests).
[`./scripts/buildTriceTool.sh`](buildTriceTool.sh)                           | Build and install `trice` and `tlog` from the checkout; see [tool builds](../docs/rm/tool_build_release.md).
[`./scripts/format_repo.sh`](format_repo.sh)                                 | Check or apply repository formatting; see [source maintenance](../docs/rm/maintenance.md#cleaning-the-sources).
[`./scripts/release_prep.sh`](release_prep.sh)                               | Run release preparation checks; `format` mode also builds and checks snapshot artifacts.
[`gitLogWithBranches.sh`](gitLogWithBranches.sh)                             | Inspect Git history with branch context.
[`gitAddWorktreeFromGitLogLineData.sh`](gitAddWorktreeFromGitLogLineData.sh) | Create a worktree for a selected historical commit.
[`gitAddWorktreesBetween.sh`](gitAddWorktreesBetween.sh)                     | Create worktrees for a range of historical commits.

The Reference Manual owns the [script naming rules](../docs/rm/maintenance.md#project-structure-and-ownership) and [test internals](../docs/rm/target_test_internals.md). Read each Git tool's usage comments before running it.
