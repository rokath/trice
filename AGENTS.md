# Repository Agent Rules

## Editing Rules

- Use `apply_patch` for manual text edits.
- Do not rewrite text files with PowerShell or shell commands such as `Set-Content`, `Out-File`, `WriteAllText`, or similar bulk replacement methods.
- Preserve file encoding exactly. Treat UTF-8 and Unicode content as fragile.
- If a file contains non-ASCII characters, emojis, or other encoding-sensitive content, do not perform mechanical rewrites without checking the diff first.
- Do not use absolute filesystem paths in repository files, scripts, or build/test commands. Keep project paths relative so the repository can be checked out anywhere.
- In scripts, prefer stabilizing the working directory once near the top, then use consistent relative project paths for the rest of the script.
- In documentation, links to files and folders inside this repository must be relative and must not use absolute GitHub URLs.
- Absolute web URLs are allowed in documentation only for truly external resources outside this repository.
- For changes affecting more than 5 files, show a sample diff before continuing.
- Communication with Codex may be in German or English.
- All file comments must always be written in English.
- New `.c`, `.h`, and `.go` files must start with `// SPDX-License-Identifier: MIT` as the first line unless the file is clearly external or vendored.
- When editing internal `.c`, `.h`, or `.go` files that are missing the SPDX header, prefer adding `// SPDX-License-Identifier: MIT` as the first line unless there is clear evidence the file is external, vendored, or governed by a different license.
- If a user message looks accidentally truncated or cut off by an unintended Enter key press, ask a brief clarifying question before acting on it.

## Commits

- If asked to "commit first", create only the requested safety commit and stop for confirmation before further edits.
- Keep unrelated changes out of the same commit.
- If asked to commit and the work contains multiple distinct changes, split them into sensible separate commits instead of one combined commit.
- A sensible commit may span multiple files, but only when those files belong to the same cohesive change.
- If the user asks for `commit` without explicitly requesting a single combined commit, prefer multiple topic-based commits over one broad commit.
- Group commits by change intent, for example tests/coverage, script behavior, documentation, or generated data, and keep those groups separate unless the documentation change is required to explain the exact same code change.
- Before committing, quickly inspect the worktree for mixed concerns and exclude unrelated or suspicious files until the grouping is clear.
- When a documentation file is modified independently of the code change, treat it as its own commit unless the user explicitly asks to bundle it.

## Tests

- Prefer `github.com/stretchr/testify/assert` for new assertion-style Go tests.

<!--
Rationale:
- This repo contains encoding-sensitive files. Shell- or PowerShell-based text
  rewrites have previously caused mojibake and other silent corruption.
- Small, explicit patches are preferred over bulk rewrites.
- "Commit first" means "create the safety commit and stop", not "continue with
  unrelated follow-up changes automatically".
-->
