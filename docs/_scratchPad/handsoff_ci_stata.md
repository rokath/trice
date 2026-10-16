# Handoff ci_stata

## Goal and binding scope

Finish the first successful statistics run and verify the public dashboard at
<https://rokath.github.io/trice/stats/> alongside the existing documentation.

The user chose the smallest integration: unchanged `jgehrcke/github-repo-stats`
v1.4.2, pinned to `306db38ad131cab2aa5f2cd3062bf6f8aa78c1aa` (Apache-2.0),
with its standard report. Release downloads, custom charts/collectors, a local
fork and separate daily star/fork inventories are deferred. These decisions
override the broader [original task](Trice_GitHub_Statistics_Codex_Task.md).

The user is preparing a release in parallel. Preserve unrelated work; do not
change Trice source, build, packaging or release configuration. The user
explicitly authorized the isolated statistics PR, merge and initial workflow
and Pages runs. Do not infer approval for unrelated changes or later commits.

## Checkout and Git state

- Checked on 2026-10-16: branch `wip`, HEAD
  `1ad74cc7b04f28c951e6936425b3ef7370625e35`.
- Statistics and Pages integration are already committed and merged through
  [PR #747](https://github.com/rokath/trice/pull/747), merge `d3b17b42`.
  The PR changed only the three statistics files listed below.
- Local `origin/main` is `89fca8920b1bf4e5a4aeaac9d41d029647de912d`
  (PR #748). This is a tracking ref, not a fresh remote verification.
- No tracked local changes were present at this check. Existing untracked
  `handsoff_ci_stat1.md` in this directory belongs to other work; preserve it.
  This handoff is newly created and uncommitted.
- A clean local worktree remains at `temp/statistics/activation-worktree`,
  branch `setup-github-statistics`, HEAD
  `4214a1d8aee7f4f31047da0d940e793257ed6187`. It is not needed for operation.

## Implemented files

- [statistics.yml](../../.github/workflows/statistics.yml): daily 23:43 UTC
  and manual collection, restricted to the default branch. The action uses
  `GHRS_GITHUB_API_TOKEN` and writes to `github-repo-stats` in the same repo.
  The ordinary `GITHUB_TOKEN` has no permissions. Runs are serialized.
- [pages.yml](../../.github/workflows/pages.yml): successful statistics runs
  on `main` trigger the existing Pages workflow. After Jekyll, it copies
  `rokath/trice/latest-report` from the data branch to `_site/stats/report/`
  and adds a small landing page with statistical caveats. There is one shared
  artifact/deployment; existing documentation is retained. An absent data
  branch is skipped; an existing but missing/empty report fails visibly.
- [github_statistics.md](../rm/github_statistics.md): setup, permissions and
  limitations. Its phrase "prepared locally and is not live yet" predates
  the merge; use actual run/deployment evidence to establish current status.

## Live evidence and unresolved issue

GitHub previously confirmed default branch `main`, workflow-based Pages and
the existence of secret `GHRS_GITHUB_API_TOKEN`. No secret value was read.

The user supplied a failed live run from 2026-10-16 around 01:39 UTC. The
upstream container successfully read referrers, paths, clones, views and forks,
then failed at `repo.get_stargazers_with_dates()` with:

```text
403 Resource not accessible by personal access token
GET /repos/rokath/trice/stargazers
```

The action exited before publishing its data/report. The token initially had
only Administration and Metadata read access. The user has now confirmed
adding "Read and Write access to code", corresponding to Contents read/write.
**A successful rerun after that correction has not been confirmed.** No code
change or broader token scope is currently justified. User permissions are
not required; these are repository permissions for only `rokath/trice`.

The last attempted assistant command
`gh workflow run statistics.yml --repo rokath/trice --ref main` was not
executed: automatic approval review failed because the workspace was out of
credits. This was a review-system failure, not an unsafe-action finding or
missing user authorization. Resolve that prerequisite before retrying; do not
bypass approval. Browser automation was unavailable, and web fetches could
not verify the dashboard. Neither successful publication nor a real HTTP 404
has been established by those fetch failures.

## Checks already completed

- `actionlint -shellcheck='' .github/workflows/statistics.yml .github/workflows/pages.yml`: passed. ShellCheck was disabled because its installed executable was blocked locally.
- `markdownlint docs/rm/github_statistics.md`: passed.
- `git diff --check`: passed.
- `python3 temp/statistics/check_integration.py`: eight PASS results, covering configuration/Bash syntax, absent data branch, complete report/assets, missing and empty reports, remote/fetch errors and missing-secret diagnostics. Fixtures preserved documentation and source bytes.
- All required PR #747 checks passed before merge, including both CodeQL languages, Super-Linter, ShellCheck, shfmt and clang-format. Optional heavy CI was not fully monitored.

The fixture helper and its inputs are ignored local files under
`temp/statistics/`; they will not transfer through Git. They are optional for
continuation, not runtime dependencies. No local upstream-container execution
was performed. The live run above failed; do not describe collection as passed.

## Next concrete steps

1. Check [statistics runs](https://github.com/rokath/trice/actions/workflows/statistics.yml) before starting another run. The user was instructed to select the failed run and choose **Re-run failed jobs** after changing token permissions; that rerun may now exist.
2. If needed, rerun on `main`. Confirm the Stargazer request succeeds, the whole action finishes and the data branch contains `rokath/trice/latest-report/report.html`. If a new token was generated instead of editing the existing one, the user must replace the repository secret themselves; never request a token in chat.
3. Wait for **Deploy GitHub Pages** to finish after successful collection. If it does not start automatically, dispatch the existing Pages workflow on `main`.
4. Verify the dashboard, embedded/full report, chart resources and existing documentation publicly. Report any new failure from its exact final error; do not expand scope into a custom implementation.

For transfer, manually add and commit this handoff, then push the commit to
the remote before switching computers. Transfer `handsoff_ci_stat1.md` too if needed.
