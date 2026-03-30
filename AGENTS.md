# Repository Agent Rules

## Editing Rules

- Use `apply_patch` for manual text edits.
- Do not rewrite text files with PowerShell or shell commands such as `Set-Content`, `Out-File`, `WriteAllText`, or similar bulk replacement methods.
- Preserve file encoding exactly. Treat UTF-8 and Unicode content as fragile.
- If a file contains non-ASCII characters, emojis, or other encoding-sensitive content, do not perform mechanical rewrites without checking the diff first.
- For changes affecting more than 5 files, show a sample diff before continuing.

## Commits

- If asked to "commit first", create only the requested safety commit and stop for confirmation before further edits.
- Keep unrelated changes out of the same commit.

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
