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

Use `github.com/stretchr/testify/assert` for new assertion-style Go tests to keep the test style consistent across the repository.

Run the regular repository checks with:

```sh
./scripts/testAll.sh
```

Use `./scripts/testAll.sh full` before a final release-level change that
affects target C code or compiler configurations. The regular suite already
includes Go tests and coverage. Its logs and coverage profile are written below
`temp/log/`; no browser-based test viewer or additional Go test framework is
required.

## Continuing a Codex task on another computer

Use the shell entry points `./scripts/codex_handover_export.sh` and
`./scripts/codex_handover_start.sh` on macOS, Linux, or Windows with Git Bash.
The [computer-switch guide (German)](docs/Codex_Rechnerwechsel_DE.md) starts
with the short workflow and explains automatic checks and recovery afterwards.
Run their isolated behavioral tests with `./scripts/test_codex_handover.sh -v`.
These developer tools are independent of the Trice runtime and regular TestAll suite.

## License of contributions

By submitting a pull request to this repository, you agree that your contribution is provided under the project's MIT License (`LICENSE.md`).

## First pull request?

If this is your first pull request, this free guide is useful:
[How to Contribute to an Open Source Project on GitHub](https://egghead.io/courses/how-to-contribute-to-an-open-source-project-on-github)
