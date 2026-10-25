# Contributing to this project

Contributions are welcome, especially:

- ports (small demo projects) for other hardware and compilers
- benchmarks
- hints and ideas
- an email about you and your project

## How to contribute

1. Fork the repository and create a feature branch.
2. Keep changes focused and small when possible.
3. Add or update tests when behavior changes.
4. Open a pull request with a clear description of what and why.

## Tests

Use [Running the Tests](docs/rm/target_tests.md) for the complete suite, focused checks and native CI. [Test Internals](docs/rm/target_test_internals.md#add-or-change-a-test) explains adding cases and maintaining the CGO harness.

Use `github.com/stretchr/testify/assert` for new assertion-style Go tests to keep the test style consistent across the repository.

## Continuing a Codex task on another computer

Use `handsoff <tag>` to record a concise task handover, then commit and transfer
the repository changes. On the destination, use `handson <tag>` to load and
verify that handover before issuing the next task. See the
[tagged handover rules](AGENTS.md#tagged-handovers) for the exact behavior.
The former profile-transfer scripts are archived; they are not required for
this Git-based workflow.

## License of contributions

By submitting a pull request to this repository, you agree that your contribution is provided under the project's MIT License (`LICENSE.md`).

## First pull request?

If this is your first pull request, this free guide is useful:
[How to Contribute to an Open Source Project on GitHub](https://egghead.io/courses/how-to-contribute-to-an-open-source-project-on-github)
