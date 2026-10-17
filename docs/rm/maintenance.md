# Repository and Documentation Maintenance

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Project structure and ownership](#project-structure-and-ownership)
* [2. GitHub workflows](#github-workflows)
  * [2.1. Coverage](#coverage)
  * [2.2. Link checks](#link-checks)
* [3. Trice Reference Manual Maintenance](#trice-reference-manual-maintenance)
* [4. Cleaning the Sources](#cleaning-the-sources)
* [5. GitHub statistics](#github-statistics)
  * [5.1. Initial scope](#initial-scope)
  * [5.2. Repository integration](#repository-integration)
  * [5.3. One-time GitHub setup](#one-time-github-setup)
  * [5.4. Reading the standard report](#reading-the-standard-report)
  * [5.5. Recovery and local checks](#recovery-and-local-checks)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## 1. <a id="project-structure-and-ownership"></a>Project structure and ownership

The [Guide](../TriceGuide.md) describes the repository layout. This appendix covers maintenance, not application setup.

| Location | Maintainer use |
| --- | --- |
| [.github/workflows](../../.github/workflows) | Automation; each YAML file defines its triggers, permissions and steps. |
| [.github/labeler.yml](../../.github/labeler.yml) | Rules consumed by the label workflow, not an executable workflow. |
| [.github/ISSUE_TEMPLATE](../../.github/ISSUE_TEMPLATE) | Issue forms. |
| [.clang-format](../../.clang-format), [.clang-format-ignore](../../.clang-format-ignore) | C formatting and exclusions. |
| [.editorconfig](../../.editorconfig), [.gitattributes](../../.gitattributes) | Editor settings and Git line-ending rules. |
| [.markdownlint.yaml](../../.markdownlint.yaml), [.markdownlintignore](../../.markdownlintignore), [lychee.toml](../../lychee.toml) | Documentation checks and exclusions. |
| [gitAddWorktreeFromGitLogLineData.sh](../../scripts/gitAddWorktreeFromGitLogLineData.sh), [gitAddWorktreesBetween.sh](../../scripts/gitAddWorktreesBetween.sh) | Create separate checkouts for historical comparisons. Read their usage before running. |
| [gitLogWithBranches.sh](../../scripts/gitLogWithBranches.sh) | Inspect history with branch context. |

## 2. <a id="github-workflows"></a>GitHub workflows

The workflow files are the authority for current triggers. Local checks use the same repository scripts where possible; [Testing Trice](target_tests.md) explains test scope and state restoration.

| Workflow | Purpose |
| --- | --- |
| [trice_lib_ci.yml](../../.github/workflows/trice_lib_ci.yml) | **Full** library matrix for PRs into `main`; quick nightly/manual checks. The existing check name remains “Heavy CI (quick)” even for a full PR run. |
| [trice_lib_ci_full.yml](../../.github/workflows/trice_lib_ci_full.yml) | Full nightly/manual library matrix. |
| [trice_lib_reusable.yml](../../.github/workflows/trice_lib_reusable.yml) | Shared library build/test implementation. |
| [go.yml](../../.github/workflows/go.yml) | Manually triggered Go build, tests and coverage upload. |
| [coverage.yml](../../.github/workflows/coverage.yml) | Go coverage on PRs, pushes to `main`, monthly and manual runs; upload to Coveralls. |
| [codeql.yml](../../.github/workflows/codeql.yml) | Static analysis; separate from compiling and executing tests. |
| [clang-format.yml](../../.github/workflows/clang-format.yml) | Check C formatting without rewriting sources. |
| [shfmt.yml](../../.github/workflows/shfmt.yml), [shellcheck.yml](../../.github/workflows/shellcheck.yml) | Shell formatting and shell diagnostics. |
| [superlinter.yml](../../.github/workflows/superlinter.yml) | YAML and Markdown linting. |
| [link-check.yml](../../.github/workflows/link-check.yml) | Links, including source-file links handled by the local wrapper. |
| [install-checks.yml](../../.github/workflows/install-checks.yml) | Build and exercise checkout installations and release-shaped packages. |
| [goreleaser.yml](../../.github/workflows/goreleaser.yml), [release-audit.yml](../../.github/workflows/release-audit.yml) | Publish/audit release artifacts; see [Building and Releasing](tool_build_release.md). |
| [pages.yml](../../.github/workflows/pages.yml) | Build and deploy the documentation website, with prepared statistics integration below. |
| [label.yml](../../.github/workflows/label.yml), [stale.yml](../../.github/workflows/stale.yml) | Repository labeling and inactive-issue handling. |
| [statistics.yml](../../.github/workflows/statistics.yml) | Repository activity reports; not a Trice logging feature. |

A push to `wip` alone does not trigger the library matrix. Once a PR into `main` exists, a new commit updates that PR and starts the full PR checks again. The nightly activity guard applies only to scheduled runs, not PRs or explicit manual runs. Branch-protection settings determine whether a failed check blocks merging.

### 2.1. <a id="coverage"></a>Coverage

Use the same package selection as the coverage workflow:

```sh
mkdir -p temp/log
go test ./... -covermode=atomic -coverprofile=./temp/log/coverage.out -coverpkg=./...
go tool cover -func=./temp/log/coverage.out
go tool cover -html=./temp/log/coverage.out
```

Go coverage measures Go code, not the C target library. The CGO and firmware matrices provide separate evidence for C configurations. Compare coverage with the target branch; a rounded statement percentage is not identical to Coveralls covered-line coverage.

### 2.2. <a id="link-checks"></a>Link checks

Run `./scripts/_530_test_links.sh`. It uses [lychee.toml](../../lychee.toml), checks source references as well as documentation and writes details to `temp/log/_530_test_links.log`. It maps the CLI-help source URL to the local checkout. Missing Lychee is reported as a skip. `GITHUB_TOKEN` or `GH_TOKEN` can reduce GitHub API throttling.

## 3. <a id="trice-reference-manual-maintenance"></a>Trice Reference Manual Maintenance

Edit the chapter sources in `docs/rm/` and list every chapter once in [README.md](README.md). The User Manual contains the first steps; put complete reference details here and link them from other documents. Do not maintain a second hand-edited monolith.

Keep each chapter's `mdtoc` container. Write plain headings without manual numbers or anchors; the formatter generates them. For example:

```md
<!-- mdtoc -->
<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->
```

Keep headings and contents labels free of emojis. Use the [shared symbol meanings](HEADER.md#symbols-and-table-results) sparingly in UM and RM prose: one symbol plus its English text label per note. In comparison tables, keep the words beside ✅ Yes and ❌ No, retain any restrictions with Partial, and distinguish — (not applicable), ? (unknown/not checked) and 0 (a measured zero). Do not decorate commands, code or quoted tool output. Preserve the commented symbol collection in [FOOTER.md](FOOTER.md) as reference material.

| Task | Command |
| --- | --- |
| Regenerate manual ToCs, numbering and anchors | `./scripts/_310_refresh_trice_user_manual.sh format` |
| Verify them without editing | `./scripts/_310_refresh_trice_user_manual.sh check` |
| Check Markdown | `./scripts/_450_test_markdownlint.sh` |
| Check links | `./scripts/_530_test_links.sh` |
| Build both PDFs | `./scripts/_320_generate_trice_user_manual_pdf.sh` |

The PDF build assembles the RM chapter list, rewrites internal links and numbers the complete document. Generated assembly files are build artifacts. Check the resulting PDFs for chapter starts, code blocks, tables and working links.

VS Code Markdown previews, table formatting and paste extensions can help editing; they are optional. Use the repository commands for reproducible ToCs and PDFs.

## 4. <a id="cleaning-the-sources"></a>Cleaning the Sources

* `./scripts/format_repo.sh check` checks manual, shell, Go and C formatting. It preserves sources, ID metadata, example build outputs and `.DS_Store` files; it does not install formatters.
* `./scripts/format_repo.sh format` also refreshes Bind headers, cleans example build outputs and removes `.DS_Store` files. Run it separately from `testAll.sh`.
* `docs/_scratchPad/` contains notes and historical sources. It is excluded from repository-wide Go discovery, formatting, linting and link checks; no `go.mod` is needed there.
* [`_300_clean_dsstore.sh`](../../scripts/_300_clean_dsstore.sh) removes macOS folder metadata when explicitly requested. Tests report it without deleting it.
* [`_240_legacy_clean_ids.sh`](../../scripts/_240_legacy_clean_ids.sh) belongs to the [deprecated Insert/Clean workflow](insert_clean_cache.md), not normal Bind preparation.

## 5. <a id="github-statistics"></a>GitHub statistics

### 5.1. <a id="initial-scope"></a>Initial scope

Use the unchanged [github-repo-stats action](https://github.com/jgehrcke/github-repo-stats)
to archive GitHub traffic and generate its standard HTML/PDF report. It covers
daily views and clones, stars, forks, referrers and popular paths. No custom
traffic collector, chart library or Trice dependency is added. A separate
[download report generator](../../scripts/github_downloads.py) uses Python's
standard library to render release-download charts.

Release markers and a separately measured daily star/fork inventory remain
deferred. The original statistics task describes
possible later work; it does not expand this initial scope.

The action is pinned to **v1.4.2**, commit
`306db38ad131cab2aa5f2cd3062bf6f8aa78c1aa`, under the
[Apache-2.0 license](https://github.com/jgehrcke/github-repo-stats/blob/306db38ad131cab2aa5f2cd3062bf6f8aa78c1aa/LICENSE).
The published release is old (December 2022). The pin avoids automatic upstream
changes; a successful authenticated run is still needed to confirm operation
with GitHub's current API and the upstream container.

### 5.2. <a id="repository-integration"></a>Repository integration

[statistics.yml](../../.github/workflows/statistics.yml) runs daily at 23:43 UTC
and supports manual runs. It runs only on this repository's default branch,
including manual starts. A run selected on `wip` is skipped. GitHub schedules
become active only after the workflow reaches the default branch.

The action writes data and reports to the `github-repo-stats` branch in this
same repository. It can create that branch on its first run. Its report is at
`rokath/trice/latest-report/report.html` in the data branch. Development and
release branches receive no daily statistics commits. Concurrent collection
runs are serialized.

After upstream succeeds, a second job reads all release API pages and stores
one download snapshot per UTC day under `rokath/trice/downloads/snapshots/`
in the same data branch. A repeat run replaces that day's snapshot. It renders
SVG charts and a separate HTML report under `rokath/trice/downloads/report/`.
The job uses the normal read-only `GITHUB_TOKEN` for release counts and the
existing `GHRS_GITHUB_API_TOKEN` for data-branch writes; no new secret is needed.

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

The public dashboard is at `/trice/stats/`; the unchanged upstream report
is embedded below a short explanation of its limits. The separate download
report is linked at `/trice/stats/downloads/` after its first collection.
Before then, Pages shows a notice; incomplete download reports fail with the
missing artifact's path. No README, Trice source,
build or release configuration is changed. GitHub confirms `main` as the
default branch and GitHub Actions as the existing Pages build type. The Pages
statistics trigger is deliberately limited to `main`.

### 5.3. <a id="one-time-github-setup"></a>One-time GitHub setup

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

### 5.4. <a id="reading-the-standard-report"></a>Reading the standard report

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

**Release downloads:** the separate report shows the current sum per published
release (including prereleases) and its measured total over time. Counters include
all release assets, such as binaries, manuals and checksums, and repeated or
automated downloads. Git operations and automatically generated source archives
are excluded. Historical daily counts cannot be reconstructed; the curve starts
with the first download snapshot. Missing days remain gaps. Deleting or replacing
assets can lower the total because it sums counters of currently existing assets.
The report also links both standalone SVG charts for download.

### 5.5. <a id="recovery-and-local-checks"></a>Recovery and local checks

After a failed or missed run, inspect the Actions error, restore token access
if necessary, then run the workflow manually on the default branch. Retain
the data branch. Repeated snapshots are handled by upstream aggregation;
individual runs can still create additional Git commits and snapshot files.
Traffic older than GitHub's retention window cannot be recovered after a long
interruption. Do not invent values for those gaps.

Run `actionlint .github/workflows/statistics.yml .github/workflows/pages.yml` and
`markdownlint docs/rm/maintenance.md` locally. These checks do not run
Trice builds, change release files, contact the traffic API or push data.

For an offline report, save `gh api --paginate --slurp 'repos/rokath/trice/releases?per_page=100'`
to `temp/releases.json`, then run
`python3 scripts/github_downloads.py temp/releases.json temp/downloads`.
Open `temp/downloads/report/index.html`. Reuse `temp/downloads` to retain local
daily snapshots. `go test ./scripts -run 'TestDownload'` checks the generator
and Pages integration with fixtures, without API access or Git writes.

