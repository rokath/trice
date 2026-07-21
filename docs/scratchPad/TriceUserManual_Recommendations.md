# Trice User Manual – Light Restructuring and Quality Recommendations

This document contains suggestions and critical notes only.

## Recommended checks before committing

```bash
./scripts/_refresh_trice_user_manual.sh
# or regenerate the TOC with your normal mdtoc workflow first, then check

markdownlint .
lychee --config lychee.toml .
./scripts/testAll.sh quick
```

## Light restructuring recommendations

### 1. Separate user documentation from maintainer documentation

The manual currently serves several roles at once:

- user manual,
- quickstart guide,
- implementation reference,
- historical narrative,
- development environment setup,
- repository maintenance guide,
- release guide.

For future readability, consider keeping `TriceUserManual.md` focused on user-facing usage and moving maintainer-only material into a separate document, for example:

```text
docs/TriceUserManual.md
docs/TriceDeveloperManual.md
docs/TriceMaintainerManual.md
docs/TriceReleaseChecklist.md
```

This does not need to happen before the next release.

###  Keep quickstarts early and short

The new Quickstarts chapter is valuable and should stay near the beginning. For first-time users, the most important order is:

1. existing non-blocking byte writer / deferred auxiliary 8-bit,
2. SEGGER RTT direct mode,
3. UART or USB-VCOM deferred output,
4. example projects.

The longer explanations can remain in later chapters, but the quickstarts should stay short and copy-paste-friendly.

### Consider marking legacy/experimental sections clearly

Some sections are valuable but can confuse first-time readers because they describe older approaches or future ideas. Consider adding small labels rather than moving them immediately:

```markdown
> Legacy note: ...
```

or:

```markdown
> Experimental / draft: ...
```

Good candidates:

- old encoding discussions,
- legacy memory-size examples,
- RPC/triceF-related sections,
- structured logging draft,
- future development sections.

### Preserve anchors when renaming headings

Keeping old anchors avoids breaking existing README links, GitHub issue references, and external bookmarks.

### 6. Consider a small “Terminology” section

Trice has several project-specific terms that recur often:

- Trice,
- Trice ID,
- `til.json`,
- `li.json`,
- direct mode,
- deferred mode,
- Trice buffer,
- output buffer,
- TCOBS,
- cycle counter,
- Trice cache.

A short terminology section near the beginning would reduce repeated explanations later.

### Review the “Start with Trice” chapter after the quickstarts settle

Chapter 5 and Chapter 6 overlap. This is currently acceptable, but later you could make Chapter 5 mostly installation/tool workflow and Chapter 6 the practical first-use guide.

A possible future split:

- Chapter 5: install, build, clone, fork, port.
- Chapter 6: quickstarts only.
- Later chapters: reference details.

### Move long CI/repository-maintenance details out later

The `.github` and release-maintenance sections are useful for maintainers, but they make the user manual very long. Consider eventually moving them into:

```text
docs/TriceMaintainerManual.md
```

The user manual could then link to that document.

### Recheck code snippets separately

I fixed only very obvious snippet errors. A future pass should compile or at least manually verify all C and shell snippets. Good candidates for review:

- legacy conversion examples,
- quickstart snippets,
- release commands,
- Git commands,
- Windows setup commands,
- old example-project command blocks.

## Critical notes

### Manual length

The manual is extremely comprehensive, but the same strength makes it hard for first-time readers. The improved README now solves part of this problem. For the manual, the next step is not more content, but clearer separation of user paths.

### Personal tone

The personal history and Trice Girl story are valuable. Keep them, but they may not need to precede practical usage forever. A future “Background and history” section could keep the personality without delaying first-use instructions.

### Old terms: `depreciated` and `adaption`

I corrected visible occurrences where safe, but did not globally rewrite anchors. If you later regenerate anchors or break external links intentionally, do it as a coordinated cleanup.

### Similar content appears multiple times

The manual repeats some topics in several places:

- source patching / `trice insert` / `trice clean`,
- Trice cache,
- direct vs deferred mode,
- RTT,
- field-log decoding,
- memory and speed.

This is acceptable in a reference manual, but if future maintenance becomes hard, these should become one authoritative section plus links from elsewhere.
