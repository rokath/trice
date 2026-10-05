# GitHub Actions Workflows

Start with the workflow matching the change you want to check. Each YAML file
defines its own triggers, permissions, tools and commands; some workflows are
manual or scheduled rather than running on every pull request.

| Area | Workflows | Purpose |
| --- | --- | --- |
| Target library and integration | [Quick](trice_lib_ci.yml), [Full](trice_lib_ci_full.yml), [Reusable runner](trice_lib_reusable.yml) | Compile and test target configurations and host/target integration. Quick and Full call the shared runner. |
| Go tests and coverage | [Go](go.yml), [Coverage](coverage.yml) | Manual Go build/tests and automated coverage reporting. |
| C/C++ formatting | [Clang-Format](clang-format.yml) | Check project sources while respecting vendor exclusions. |
| Shell checks | [ShellCheck](shellcheck.yml), [shfmt](shfmt.yml) | Check shell correctness and formatting. |
| Documentation and configuration | [Links](link-check.yml), [Super-Linter](superlinter.yml) | Check links with Lychee, and Markdown/YAML with Super-Linter. |
| Security analysis | [CodeQL](codeql.yml) | Analyze Go and C/C++ code. |
| Distribution | [GoReleaser](goreleaser.yml), [Installation checks](install-checks.yml), [Release audit](release-audit.yml) | Build distributions and check packaged tools and release assets. Inspect each workflow before starting a manual run. |
| Website | [Pages](pages.yml) | Build and deploy the documentation website. |
| Repository maintenance | [Labeler](label.yml), [Stale](stale.yml) | Apply pull-request labels and handle inactive issues/pull requests. |

For local checks, run `./scripts/testAll.sh` from the repository root. Use
`./scripts/testAll.sh full` for final validation across the full target matrix;
it requires the corresponding compiler tools and takes longer. The runner
prints each result and points to detailed logs. See
[CONTRIBUTING.md](../../CONTRIBUTING.md) for contribution guidance and
[the test scripts](../../scripts/) for focused checks.

Workflow behavior is defined in the YAML files. This directory contains no
organization workflow templates or template metadata.
