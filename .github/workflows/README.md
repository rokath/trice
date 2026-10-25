# GitHub Actions Workflows

Start with the workflow matching the change you want to check. Each YAML file
defines its own triggers, permissions, tools and commands; some workflows are
manual or scheduled rather than running on every pull request.

Area | Workflows | Purpose
--- | --- | ---
Target library and integration | [PR / Nightly Quick](trice_lib_ci.yml), [Nightly / Manual Full](trice_lib_ci_full.yml), [Reusable runner](trice_lib_reusable.yml) | PRs to `main` run the full configuration matrix; the Quick workflow uses a smaller selection for nightly/manual runs. Both call the shared runner.
Native platform acceptance | [Native tests](native-tests.yml), [Native runner](native-tests-reusable.yml) | Real Windows/Linux/macOS runs of the shared test plan; explicit quick on PRs, full plus race checks nightly/manually, additional macOS Intel full run.
Go tests and coverage | [Go](go.yml), [Coverage](coverage.yml) | Manual Go build/tests and automated coverage reporting.
C/C++ formatting | [Clang-Format](clang-format.yml) | Check project sources while respecting vendor exclusions.
Shell checks | [ShellCheck](shellcheck.yml), [shfmt](shfmt.yml) | Check shell correctness and formatting.
Documentation and configuration | [Links](link-check.yml), [Super-Linter](superlinter.yml) | Check links with Lychee, and Markdown/YAML with Super-Linter.
Security analysis | [CodeQL](codeql.yml) | Analyze Go and C/C++ code.
Distribution | [GoReleaser](goreleaser.yml), [Installation checks](install-checks.yml), [Release audit](release-audit.yml) | Build distributions and check packaged tools and release assets. Inspect each workflow before starting a manual run.
Website | [Pages](pages.yml) | Build and deploy the documentation website.
Repository maintenance | [Labeler](label.yml), [Stale](stale.yml) | Apply pull-request labels and handle inactive issues/pull requests.
Future tool compatibility | [Weekly / Manual](tool-compatibility.yml) | Try current Go and GoReleaser separately from native acceptance with declared versions.
Statistics data isolation | [Statistics branch guard](statistics-branch-guard.yml) | Reject PRs involving `github-repo-stats`. Make `Statistics branch isolation` a required check on `main`; `wip` retains direct pushes. The workflow alone does not enforce the merge restriction.

Library checks run when a PR to `main` opens, reopens or receives another push.
A push to `wip` without an open PR does not start these library workflows.
Only scheduled runs skip the builds when `main` has no commits in the last
24 hours; manual starts always execute their selected checks. The existing
workflow/job names stay unchanged for branch-protection compatibility, so the
PR check still contains "Quick" in its name even though it runs the full matrix.

For local checks, `./scripts/testAll.sh` defaults to **full**. Use
`./scripts/testAll.sh quick` for a shorter native matrix. Preparation selects
the declared project tools before tests start. Each run retains its own logs
and JSON report under `temp/log/testAll.XXXXXX`; missing mandatory evidence is
INCOMPLETE, never PASS. External links and release artifact generation remain
separate workflow checks. The new native platform jobs need to be added to
required repository checks explicitly; their YAML does not change protection
settings. See
[CONTRIBUTING.md](../../CONTRIBUTING.md) for contribution guidance and
[the test scripts](../../scripts/) for focused checks.

Workflow behavior is defined in the YAML files. This directory contains no
organization workflow templates or template metadata.
