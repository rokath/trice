# GitHub statistics

## Initial scope

Use the unchanged [github-repo-stats action](https://github.com/jgehrcke/github-repo-stats)
to archive GitHub traffic and generate its standard HTML/PDF report. It covers
daily views and clones, stars, forks, referrers and popular paths. No custom
collector, chart library or Trice dependency is added.

Release downloads, additional charts, release markers and a separately measured
daily star/fork inventory are deferred. The original statistics task describes
possible later work; it does not expand this initial scope.

The action is pinned to **v1.4.2**, commit
`306db38ad131cab2aa5f2cd3062bf6f8aa78c1aa`, under the
[Apache-2.0 license](https://github.com/jgehrcke/github-repo-stats/blob/306db38ad131cab2aa5f2cd3062bf6f8aa78c1aa/LICENSE).
The published release is old (December 2022). The pin avoids automatic upstream
changes; a successful authenticated run is still needed to confirm operation
with GitHub's current API and the upstream container.

## Repository integration

[statistics.yml](../../.github/workflows/statistics.yml) runs daily at 23:43 UTC
and supports manual runs. It runs only on this repository's default branch,
including manual starts. A run selected on `wip` is skipped. GitHub schedules
become active only after the workflow reaches the default branch.

The action writes data and reports to the `github-repo-stats` branch in this
same repository. It can create that branch on its first run. Its report is at
`rokath/trice/latest-report/report.html` in the data branch. Development and
release branches receive no daily statistics commits. Concurrent collection
runs are serialized.

The existing [Pages workflow](../../.github/workflows/pages.yml) builds the
documentation from the repository root with Jekyll and deploys one `_site`
artifact. The root `_config.yml` uses the Cayman theme. **Pages integration is
prepared locally and is not live yet.** The workflow adds the report under
`_site/stats/` after Jekyll finishes, before the existing artifact upload.
Successful statistics runs on `main` request the same Pages build/deployment.
There is no second deployment, and existing documentation files are retained.
Before the first collection, an absent data branch is skipped so documentation
can still be published. A missing or empty report in an existing data branch
fails with its expected path instead of publishing broken statistics.

The public dashboard will be at `/trice/stats/`; the unchanged upstream report
is embedded below a short explanation of its limits. No README, Trice source,
build or release configuration is changed. GitHub confirms `main` as the
default branch and GitHub Actions as the existing Pages build type. The Pages
statistics trigger is deliberately limited to `main`.

## One-time GitHub setup

Review both workflows before activating collection and publication.

1. Create a **fine-grained personal access token** from an account with the
   required repository access. Select resource owner `rokath` and **only**
   `rokath/trice`. Give it **Administration: read** for traffic and
   **Contents: read and write** for the data branch; Metadata read is automatic.
   Do not use a classic token with broad `repo` scope. Set an expiry and renew
   the secret before it expires.
2. In the repository, open **Settings > Secrets and variables > Actions > New
   repository secret**. Name it **`GHRS_GITHUB_API_TOKEN`** and enter the token
   there. Never put the token in chat, files, reports or command arguments.
3. Have the reviewed changes merged into the actual default branch when the
   release preparation allows it. No merge, push or deployment is performed by
   preparing these files locally.
4. Verify **Settings > Pages > Build and deployment > Source** is **GitHub
   Actions**. Keep the existing Pages deployment.
5. Open **Actions > Repository statistics > Run workflow**, select the default
   branch and run once. Check that the workflow succeeds and the data branch
   contains the report. This first run imports the traffic still available to
   GitHub, normally the last 14 days.
6. Wait for **Deploy GitHub Pages** to finish after that successful run. Verify
   both the documentation and `/trice/stats/` in a browser. The dashboard links
   to the documentation and to the full upstream report. No browser token or
   login is needed to read it. If the automatic Pages run does not start,
   start **Deploy GitHub Pages** manually on `main` and inspect its logs.

The upstream action accepts one token for reading and writing. Consequently
the fine-grained token needs both permissions above; the normal `GITHUB_TOKEN`
has no permissions in this workflow. A branch rule requiring pull requests on
the data branch would block the action's direct commits. Keep existing branch
protection unchanged; resolve any restriction explicitly before activation.

GitHub's [stargazer access restrictions](https://docs.github.com/en/rest/activity/starring)
also apply. Authentication and account access must be verified in the first
run. No authenticated run or container execution has been performed locally.

## Reading the standard report

**GitHub activity statistics are not a count of unique Trice users.
Automated access and repeated downloads are included.**

| Metric or display | Limit |
| --- | --- |
| Daily unique visitors and cloners | GitHub distinguishes sources within that day, not necessarily people. |
| Cumulative unique visitors/cloners | These are accumulated daily values, not unique people across the whole period. |
| Missing traffic values | The upstream analysis may replace missing individual values with zero. Do not treat this report as proof of uninterrupted collection. |
| Historical stars and forks | Reconstruction from currently existing entries is not a complete historical net inventory; removed entries cannot be recovered that way. |
| Report timestamp | The report gives its generation time in UTC. Check Actions for collection failures. |

The dashboard places the warning and these limits beside the unchanged
upstream report. Its charts and wording are not locally patched. No user
identities are intentionally added to the public statistics.

## Recovery and local checks

After a failed or missed run, inspect the Actions error, restore token access
if necessary, then run the workflow manually on the default branch. Retain
the data branch. Repeated snapshots are handled by upstream aggregation;
individual runs can still create additional Git commits and snapshot files.
Traffic older than GitHub's retention window cannot be recovered after a long
interruption. Do not invent values for those gaps.

Run `actionlint .github/workflows/statistics.yml .github/workflows/pages.yml` and
`markdownlint docs/rm/github_statistics.md` locally. These checks do not run
Trice builds, change release files, contact the traffic API or push data.
