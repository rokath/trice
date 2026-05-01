# Repository Agent Rules

## Source Code Protection

* Files inside `./src` are considered user-owned target source code.
* Do not modify files in `./src` unless the user explicitly requests changes to specific files or explicitly allows modifications in that directory.
* If a task would normally require changes in `./src`, stop and ask for confirmation before proceeding.

---

## Scope Control

* Only modify files that are directly relevant to the requested task.
* Do not refactor, rename, or reorganize unrelated code.
* Avoid "drive-by" improvements outside the requested scope.
* If additional improvements seem beneficial, propose them separately instead of applying them automatically.
* Changes to `demo*.json` at the repository root are usually generated or local-test artifacts. Unless the user explicitly asks for them, ignore such worktree changes silently and do not treat them as part of the task.

---

## Editing Rules

* Use `apply_patch` for manual text edits.
* Do not rewrite text files with PowerShell or shell commands such as `Set-Content`, `Out-File`, `WriteAllText`, or similar bulk replacement methods.
* Preserve file encoding exactly. Treat UTF-8 and Unicode content as fragile.
* If a file contains non-ASCII characters, emojis, or other encoding-sensitive content, do not perform mechanical rewrites without checking the diff first.
* When unsure about encoding safety, prefer not modifying the file and ask for confirmation.
* Prefer minimal, reviewable diffs over large transformations.
* Keep formatting changes separate from functional changes.
* Do not reformat entire files unless explicitly requested.

---

## File Creation

* Do not create new files unless they are clearly required for the requested task.
* Prefer modifying existing files over introducing new ones.
* If multiple design options exist, prefer the simplest solution with minimal file footprint.

---

## Embedded Constraints

* Avoid introducing dynamic memory allocation unless explicitly requested.
* Be careful with timing-sensitive code and side effects.
* Do not change existing logging behavior (e.g., Trice macros) unless explicitly requested.
* Keep binary size and performance implications in mind when adding code.

---

## Build and Dependency Safety

* Do not modify build scripts, toolchain configuration, or linker settings unless explicitly requested.
* Do not introduce new dependencies without explicit user approval.

---

## Script Portability

- All shell scripts must run on Linux, macOS, and Windows (e.g., via standard POSIX-compatible environments).
- Avoid platform-specific commands, flags, or behavior unless explicitly required.
- Prefer portable POSIX shell features over shell-specific extensions.
- Ensure scripts work in typical developer environments on all supported platforms without modification.

---

## Paths and Documentation

* Do not use absolute filesystem paths in repository files, scripts, or build/test commands.
* Keep project paths relative so the repository can be checked out anywhere.
* In scripts, prefer stabilizing the working directory once near the top, then use consistent relative project paths for the rest of the script.
* In documentation, links to files and folders inside this repository must be relative and must not use absolute GitHub URLs.
* Absolute web URLs are allowed in documentation only for truly external resources outside this repository.

---

## Documentation Usage

- Use `README.md` and documentation files (especially `./docs/TriceUserManual.md`) to understand system design, architecture, constraints, and intended usage.
- Consider `./src/TriceDefaultConfig.h` as an additional documentation source.

- Documentation is for understanding only, not for initiating code changes.
- Do not modify code based solely on documentation unless explicitly instructed.

- Do not infer or apply refactoring, restructuring, or behavioral changes from documentation.
- Do not align existing code with documentation automatically.

- If documentation and code appear inconsistent, treat the existing code as the source of truth.
- In such cases, do not modify code. Instead, ask for clarification before proceeding.

- Documentation may describe ideal or future behavior. Never align code with documentation unless explicitly requested.
- If a requested change appears to be based on documentation but is not explicitly specified, ask for confirmation before making modifications.

---

## Change Size Control

* For changes affecting more than 5 files, stop and show a representative diff summary before continuing.

---

## Communication and Language

* Match the user's language (German or English) unless instructed otherwise.
* Communication with Codex may be in German or English.
* All file comments must always be written in English.

---

## Issue Writing

* If asked to formulate issue text, write the issue text in English unless the user explicitly requests a different language.
* Format issue text in Markdown using clear GitHub-friendly sections such as:

  * Title
  * Description
  * Steps to reproduce
  * Expected behavior
  * Actual behavior
  * Impact
  * Suggested resolution

---

## Licensing

* New `.c`, `.h`, and `.go` files must start with `// SPDX-License-Identifier: MIT` as the first line unless the file is clearly external or vendored.
* When editing internal `.c`, `.h`, or `.go` files that are missing the SPDX header, prefer adding `// SPDX-License-Identifier: MIT` as the first line unless there is clear evidence the file is external, vendored, or governed by a different license.

---

## Commits

* If asked to "commit first", create only the requested safety commit and stop for confirmation before further edits.
* Keep unrelated changes out of the same commit.
* If asked to commit and the work contains multiple distinct changes, split them into sensible separate commits instead of one combined commit.
* A sensible commit may span multiple files, but only when those files belong to the same cohesive change.
* If the user asks for `commit` without explicitly requesting a single combined commit, prefer multiple topic-based commits over one broad commit.
* Group commits by change intent (e.g., tests/coverage, script behavior, documentation, generated data) and keep those groups separate unless the documentation change is required to explain the exact same code change.
* Before committing, inspect the worktree for mixed concerns and exclude unrelated or suspicious files until the grouping is clear.
* When a documentation file is modified independently of the code change, treat it as its own commit unless the user explicitly asks to bundle it.
* If the user asks for a `push`, stop first and ask whether `./scripts/format_repo.sh` should be run before pushing.
* If `./scripts/format_repo.sh` is run for that purpose, ask whether the resulting formatting changes should be committed before the `push`.
* Treat this formatting-and-commit step as an optional append-on to the requested push workflow, not as an automatic action.

---

## Tests

* Prefer `github.com/stretchr/testify/assert` for new assertion-style Go tests.

---

## Safety

* If a user message looks accidentally truncated or cut off, ask a brief clarifying question before acting.
* Do not infer or autocomplete missing parts of a truncated message.

---

<!--
Rationale:
- This repository contains encoding-sensitive files. Shell-based rewrites have caused corruption in the past.
- Minimal, explicit patches are preferred over bulk rewrites.
- Source code in ./src is protected and must not be modified without explicit permission.
- The rules are optimized for safe autonomous operation with Codex CLI and --full-auto.
-->
