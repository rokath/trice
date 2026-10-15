# Trice clang-format migration — handoff for CodexCLI

Date: 2026-10-05  
Repo: `trice`  
Branch during work: `wip`  
Environment: Windows 11, Git Bash / MINGW64

## Completion

Completed on 2026-10-05 on macOS arm64, starting from commit `2e8635b4`
(`intermediate clang-format improvement state`) on branch `wip`.
The original handoff below is retained as context; its continuation steps are done.

- The formatter remains pinned to **23.1.2**. Formatting and checking now use
  one native `--files` batch and the existing simplified ignore policy, without Go.
- `setup` installs or validates the tool without editing sources. CI uses
  `setup` followed by `check`, sharing the script's version pin. Check mode
  remains offline and reports `SKIP` if the exact formatter is unavailable.
- Cache lookup prefers native wheel binaries over Python launchers. Windows
  cache names remain stable across OS build changes. Explicit tool selections
  are still validated and respected.
- Repeated real 23.1.2 runs reproduced oscillation in `src/triceDoubleBuffer.c`
  and both STM interrupt files named below. Narrow off/on markers now protect
  only the affected directive/comment boundaries. `src/tcobsv1Decode.c` needed
  one ordinary formatting normalization.
- `cmd/clang-filter` remains available as a standalone helper; its README now
  explains that the formatter workflow no longer uses it.

Validation results:

- **PASS:** `bash -n scripts/_280_format_c_code.sh scripts/_430_test_clang_format.sh`.
- **PASS:** `go test ./scripts -run '^TestClangFormat' -count=1`, covering exact
  versions, bootstrap failures, offline reuse, native wheel layouts, stable
  Windows cache keys, setup without source edits, and single-batch error handling.
- **PASS:** Two complete formatting passes compared byte-for-byte across all
  1125 tracked C/C++ files, followed by `./scripts/_280_format_c_code.sh check`.
  No second-pass changes; approximately 1.75 seconds per format pass and
  1.73 seconds for the check on this machine.
- **PASS:** A real-formatter fixture verifies bare/inst/log Core inclusion,
  spaces and UTF-8 paths, all seven vendor/scratch-pad ignore examples, and
  check mode preserving file contents.
- **COMPLETED, one unrelated failure:** `./scripts/testAll.sh` took 390 seconds;
  19 of 20 steps passed, including ShellCheck, formatting, actionlint, all Go
  tests, coverage, Clang/GCC builds, release snapshot, and PC target tests.
  `_420_test_dsstore.sh` failed because the checkout already contains `.DS_Store`.
  That local file was left untouched. The final tracked-byte restoration check
  passed. See `temp/log/testAll_summary.log` for the complete result.
- **NOT RUN:** `./scripts/testAll.sh full` (not required for this formatting migration).

Implementation is complete; no commit or push was requested or performed.
Unrelated changes from the intermediate commit were preserved.

## Goal

Simplify and speed up the clang-format workflow while keeping formatting reproducible across Windows/macOS/Linux:

- pin one exact clang-format version,
- auto-cache it under `temp/tools`,
- avoid dependence on whatever system clang-format is installed,
- avoid accidental future formatting changes,
- remove unnecessary complexity,
- make checks fast enough for normal local/CI use.

Current intended version: **clang-format 23.1.2**.

---

## Initial problem

The script reported:

```text
SKIP: clang-format check requires 19.1.7 for reproducible output; detected clang-format version 21.1.7.
```

A cached 19.1.7 already existed under a Windows-build-specific path:

```text
temp/tools/clang-format-19.1.7/MINGW64_NT-10.0-26200-x86_64/
```

Current Git Bash reported:

```text
uname -s -> MINGW64_NT-10.0-26300
uname -m -> x86_64
```

So a Windows update changed the cache key. The venv launcher also contained an absolute path and broke if the venv directory was renamed. The real binary remained usable.

---

## Script changes already made

`scripts/_280_format_c_code.sh` was changed so Windows uses a stable platform name:

```bash
local platform
platform="$(uname -s)"
case "$platform" in
  MINGW*|MSYS*|CYGWIN*) platform="windows" ;;
esac
tool_dir="./temp/tools/clang-format-$CLANG_FORMAT_REQUIRED_VERSION/$platform-$(uname -m)"
```

Candidate lookup was extended to prefer real clang-format binaries such as:

```text
$tool_dir/clang_format/data/bin/clang-format.exe
$tool_dir/Lib/site-packages/clang_format/data/bin/clang-format.exe
```

before the Python launcher:

```text
$tool_dir/Scripts/clang-format.exe
```

The script currently should still contain:

```bash
CLANG_FORMAT_REQUIRED_VERSION="23.1.2"
```

Verify this first.

---

## Performance findings

Tracked C/C++ files:

```text
~1125
```

Old workflow launched clang-format once per selected file.

Measured on Windows:

- old serial check: ~44 s after using the real binary
- 100 real-binary startups: ~15.7 s
- 100 launcher startups: ~24 s
- `xargs -P 8` experiment: ~8.5 s
- single-process `clang-format --files=...` over ~1125 files: ~4–5 s

Conclusion: **process startup under Git Bash is the main cost**. Desired architecture is one clang-format process using `--files`, not Bash parallelization.

---

## Old Go filter

The old script uses:

```bash
CLANG_FILTER_CMD="${CLANG_FILTER_CMD:-go run ./cmd/clang-filter}"
```

Flow:

```text
git ls-files -> Go clang-filter -> per-file clang-format via stdin
```

The Go helper mainly existed to interpret `.clang-format-ignore` with gitignore-like `!` re-inclusion rules.

The user explicitly wants this simpler. Current goal:

> Remove the Go filter from the clang-format workflow if possible.

---

## `.clang-format-ignore` policy change

Old policy had complicated re-inclusions, e.g.:

```text
examples/*_inst/Core/Inc/*.h
!examples/*_inst/Core/Inc/trice*.h

examples/*_inst/Core/Src/*.c
!examples/*_inst/Core/Src/main.c
```

and whole `_bare` trees were excluded.

The user changed the policy:

> All `examples/**/Core/**` code may be formatted, including `_bare`, `_inst`, `_log`, etc.

Reason: `_bare` and `_inst` are used for learning/comparison. It is more useful if formatting makes them visually comparable. It is acceptable that generated STM code no longer remains byte-identical to generator output.

The intended simplified `.clang-format-ignore` is:

```text
# Files and directories excluded from clang-format.
# Project-owned example code, including all examples/**/Core/** trees, is formatted.

# Third-party / vendor files.
src/SEGGER_RTT.c
src/SEGGER_RTT.h
_test/*/SEGGER_RTT_Conf.h
**/nanoprintf.h

# Third-party code inside example projects.
examples/**/Drivers/**
examples/**/Middlewares/**

# Experimental / obsolete scratch-pad code.
**/scratchPad/**
```

No `!` rules should remain.

---

## Why removing `!` rules matters

clang-format 22.1.8 behaved badly with the old `.clang-format-ignore` re-inclusions. A deliberately malformed probe file was incorrectly reported as ignored.

This made an apparently ultra-fast batch check (~0.18 s) invalid because clang-format skipped files.

Simplifying `.clang-format-ignore` removes that issue and allows native `--files` usage.

---

## Version investigation

### 19.1.7

Worked, but old workflow was slow and cache naming was fragile.

### 22.1.8

Also showed non-idempotent behavior in some preprocessor/comment regions after broader testing.

### 23.1.2

User chose this as final target.

Installed/tested binary on Windows:

```text
temp/tools/clang-format-23.1.2/windows-x86_64/clang_format/data/bin/clang-format.exe
```

Verified:

```text
clang-format version 23.1.2
```

However, 23.1.2 also showed a real non-idempotent/oscillating formatting issue in at least one source construct.

---

## `.clang-format` change

The repo had:

```yaml
ReflowComments: true
```

With newer clang-format this caused ugly comment/Doxygen reflow in STM example files.

Testing showed that:

```yaml
ReflowComments: Never
```

avoids those unwanted changes.

The local `.clang-format` was changed accordingly. Verify:

```bash
grep -n 'ReflowComments' .clang-format
```

Expected:

```text
ReflowComments: Never
```

---

## Known non-idempotent area

`src/triceDoubleBuffer.c`, around the end of `TRICE_MULTI_PACK_MODE`.

Repeated clang-format runs toggled indentation between variants such as:

```c
       //
       // TRICE_MULTI_PACK_MODE
       //////////////////////////////////////////////////////////////////////////////
```

and:

```c
          //
          // TRICE_MULTI_PACK_MODE
          //////////////////////////////////////////////////////////////////////////////
```

The user suggested and accepted using clang-format exclusion markers for such cases.

Recommended narrow protection:

```c
// clang-format off
#endif // TRICE_DEFERRED_TRANSFER_MODE == TRICE_MULTI_PACK_MODE
//
// TRICE_MULTI_PACK_MODE
////////////////////////////////////////////////////////////////////////////////
// clang-format on
}
```

This exact edit was discussed but not confirmed completed. Verify before applying.

Other files that showed instability under 22.1.8 included:

```text
examples/F030_inst/Core/Src/stm32f0xx_it.c
examples/L432_inst/Core/Src/stm32l4xx_it.c
```

Re-test them with **23.1.2** before adding any workaround.

---

## Formatting migration side effects

Once all `examples/**/Core/**` trees were allowed to format, many files changed. This is expected and acceptable.

A later `git status` showed modifications under:

```text
examples/F030_bare/Core/...
examples/F030_inst/Core/...
examples/G0B1_bare/Core/...
examples/G0B1_inst/Core/...
examples/G0B1_log/Core/...
examples/L432_bare/Core/...
examples/L432_inst/Core/...
```

Also modified at various times:

```text
.clang-format
.clang-format-ignore
scripts/_280_format_c_code.sh
scripts/_515_test_logging_features.sh
src/tcobsv1Decode.c
src/trice.c
src/triceDoubleBuffer.c
src/triceUart.c
```

and untracked:

```text
experiments/
```

Important: `scripts/_515_test_logging_features.sh` and `experiments/` may be unrelated to the clang-format work. Preserve them unless clearly relevant.

---

## Git backup status

Before leaving the Windows PC, the user was advised to run:

```bash
git add -A
git commit -m "WIP: clang-format migration"
git push
```

or if needed:

```bash
git push -u origin wip
```

The conversation does **not** confirm that this was actually executed.

First CodexCLI step must therefore be:

```bash
git status -sb
git branch --show-current
git log -3 --oneline
```

Do not reset/restore anything until the WIP state is understood.

---

# Recommended continuation

## 1. Verify current state

```bash
git status -sb
git branch --show-current
git log -3 --oneline
```

Then:

```bash
grep -n 'CLANG_FORMAT_REQUIRED_VERSION' scripts/_280_format_c_code.sh
grep -n 'ReflowComments' .clang-format
cat .clang-format-ignore
```

Expected intent:

```text
CLANG_FORMAT_REQUIRED_VERSION="23.1.2"
ReflowComments: Never
```

and simplified `.clang-format-ignore` with no `!` rules.

---

## 2. Remove Go filter from formatting workflow

Desired architecture:

```text
git ls-files C/C++ patterns
        |
        v
temporary file list
        |
        v
clang-format 23.1.2 --files=<list>
```

No `go run ./cmd/clang-filter` for this workflow.

Do not delete the helper program unless it is unused elsewhere.

---

## 3. Replace per-file loops with one batch call

Create a list of all tracked C/C++ files:

```bash
git ls-files   '*.c' '*.h'   '*.cpp' '*.hpp'   '*.cc' '*.hh'   '*.cxx' '*.hxx'   > "$files_list"
```

Format mode:

```bash
"$CLANG_FORMAT_BIN" -i --files="$files_list"
```

Check mode:

```bash
"$CLANG_FORMAT_BIN" --dry-run --Werror --files="$files_list"
```

clang-format itself should apply the simplified `.clang-format-ignore`.

---

## 4. Keep automatic local installation

Desired lookup/bootstrap order:

1. explicit `CLANG_FORMAT_BIN`, but validate exact 23.1.2
2. exact system 23.1.2
3. local cache under:
   ```text
   temp/tools/clang-format-23.1.2/<stable-platform>-<arch>/
   ```
4. in format/setup path, bootstrap with Python/pip if missing

Do not modify system Python or system clang-format.

Windows cache key should be:

```text
windows-x86_64
```

not a Windows-build-specific MINGW string.

---

## 5. Handle only demonstrably unstable regions

For any file that changes again on a second identical 23.1.2 formatting pass:

```bash
cp file temp/after1
"$CF23" -i file
diff -u temp/after1 file
```

If it changes again, add narrow:

```c
// clang-format off
...
// clang-format on
```

around only that construct.

Primary known candidate:

```text
src/triceDoubleBuffer.c
```

Avoid whole-file exclusions unless necessary.

---

## 6. Verify global idempotence

After final normalization:

```bash
"$CF23" -i --files=temp/clang-format-files.txt
git diff --exit-code
```

or otherwise compare first and second passes.

Then:

```bash
"$CF23" --dry-run --Werror --files=temp/clang-format-files.txt
```

must return 0.

Idempotence is a hard requirement.

---

## 7. Expected performance

Single-process batch formatting/checking of ~1125 C/C++ files was around:

```text
4–5 seconds
```

on the Windows PC.

This is the target performance class.

---

## 8. Final script/tests

At minimum:

```bash
bash -n scripts/_280_format_c_code.sh
./scripts/_280_format_c_code.sh check
```

Then run the normal Trice quick test suite.

Be careful with unrelated changes in `_515_test_logging_features.sh`.

---

## 9. Final Git review

Before commit:

```bash
git status --short
git diff --stat
git diff -- .clang-format .clang-format-ignore scripts/_280_format_c_code.sh
```

Prefer separating, if practical:

1. formatting infrastructure / pinned version
2. ignore-policy simplification
3. one-time repository formatting normalization

---

## Desired final design

Use **clang-format 23.1.2 pinned and auto-cached per platform**, let clang-format itself apply a simple `.clang-format-ignore`, feed all tracked C/C++ files through **one `--files` batch**, remove the Trice-specific Go filter from this formatting workflow, and protect only proven non-idempotent fragments with narrow `// clang-format off/on` markers.
