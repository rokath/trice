# GitHub Actions Workflows

[Testing Trice — GitHub Actions](../../docs/rm/target_tests.md#github-actions) explains native Windows/Linux/macOS jobs, library matrices, coverage and focused checks. [GoReleaser and release-package testing](../../docs/rm/target_tests.md#goreleaser-and-release-package-testing) covers snapshots, installation checks and release audits.

[Repository maintenance](../../docs/rm/maintenance.md#github-workflows) covers Pages, labels, stale issues and statistics. For statistics isolation, make `Statistics branch isolation` a required check on `main`; `wip` retains direct pushes. The workflow alone does not enforce the merge restriction.

Each YAML file defines its own events, permissions and commands. This directory contains no organization workflow templates or template metadata. The former overview is retained in [the documentation archive](../../docs/_scratchPad/obsolete/testing/github_workflows_README.md).
