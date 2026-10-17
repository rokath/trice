# Trice Generate

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Bind Artifact Report](#bind-artifact-report)
* [2. Colors](#colors)
* [3. C-Code](#c-code)
* [4. C#-Code](#c-code-1)
* [5. Generating a Trice ABC Function Pointer List](#generating-a-trice-abc-function-pointer-list)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

For a compact, readable copy of the ID dictionaries, run `trice generate -onelineJSON -til til.json -li li.json`. This writes `til.oneline.json` and `li.oneline.json` in `-genDir` (default `./generated`) as complete JSON objects with one ID entry per line. In the LI copy, each entry shows `Line` before `File`. The original files remain authoritative and unchanged; rerun the command after updating them. Use `-li off` to export only the TIL copy. Missing or invalid requested input files cause an error without replacing either copy. This option cannot be combined with `-logC` or `-abc`.

## 1. <a id="bind-artifact-report"></a>Bind Artifact Report

Use `trice generate -bindReport` to inspect existing generated files and their source references. This is an explicit diagnostic command, not an automatic step of every Bind run:

```sh
trice generate -bindReport -genDir generated -src Core
trice generate -bindReport -genDir generated -src Core -src ../../_test/testdata -exclude Core/vendor
```

Paths are relative to the calling directory. `-genDir` defaults to `./generated`; `-src` defaults to the current directory and accepts multiple files or directories, with the usual C/C++ source/header selection and `-exclude` rules. Hidden paths and the selected generation directory are excluded from the source scan. The inventory lists immediate entries in `-genDir`; it does not descend into its subdirectories or follow non-regular entries. Missing directories or source roots and read failures are errors: no partial inventory is published. An empty generation directory is valid.

The report goes to standard output and needs neither `til.json` nor `li.json`. It never assigns IDs, runs Bind, creates directories, rewrites files or deletes artifacts. It cannot be combined with `-logC`, `-onelineJSON`, `-abc` or `-colors`. For an optional saved copy, redirect its output yourself:

```sh
trice generate -bindReport -src Core > bind-report.txt
```

For example, if `Core/main.c` includes `trice_sidecar_main_c_1111111111111111.h`, the relevant report entry is:

```text
trice_sidecar_main_c_1111111111111111.h [sidecar]
  File Key: K1111111111111111; owner sidecar: trice_sidecar_main_c_1111111111111111.h
  Referenced in scan: Core/main.c
```

An existing `trice_sidecar_triceCheck_c_2222222222222222.h` whose source is outside `-src Core` instead says `Not referenced in selected scan; owner source not established for this artifact.` This does **not** mean that the owner was deleted or that the header is unused. Including `../../_test/testdata` in the second command can reveal its reference from `triceCheck.c`. A source that moved without changing its include remains associated with the same sidecar.

Rebase entries name their owner sidecar and separately show whether the helper itself is referenced. A helper with no reference may be an old helper belonging to a still-referenced owner. Its contents must exactly match the generated phase helper to establish that metadata; missing or invalid owner declarations and edited helper contents are marked `UNVERIFIED`. Sidecar checks validate the File Key and BIND route declarations against the filename, not every descriptor or the freshness of the file. A handwritten file can imitate these declarations; this report is not a certificate of generated ownership.

`AMBIGUOUS` identifies a File Key claimed by multiple scanned sources. `[missing]` identifies a referenced artifact absent from `-genDir`. Repeated literal includes in one source are deduplicated; both `#include "trice_...h"` and `#include <trice_...h>` are recognized. Comments do not count as references. Macro-based include expressions are not resolved. Physical includes inside `#if` branches do count, because preprocessing conditions, compiler include search paths and actual build dependencies are not evaluated. Inventory findings are printed for inspection; `UNVERIFIED`, `AMBIGUOUS` and `[missing]` do not themselves cause a command error.

Other files remain `[unclassified]`, explicitly with no ownership established. This includes user-owned ABC selection headers, field registries and unrelated files. Nothing in this report authorizes pruning. Bind only removes its own excess rebase helpers for a currently known owner; globally removing artifacts outside a selected scan is a separate operation that is not provided here. `bind -v` retains its existing summary of the current Bind operation; use `generate -bindReport` for an independent inventory.

The [inventory behavior tests](../../internal/id/generateBindReport_test.go) cover scope selection, real Bind output, ambiguous ownership, missing and altered artifacts, failure paths and zero attempted writes. The [CLI tests](../../internal/args/handler_additional_test.go) also check mode exclusivity and inspection on a read-only filesystem without dictionaries or logfile setup.

## 2. <a id="colors"></a>Colors

Support for finding a color style:

![generateColors.PNG](../ref/generateColors.png)

See [Check Alternatives](tags_color_and_weights.md#check-color-alternatives) chapter.

## 3. <a id="c-code"></a>C-Code

To generate a compact C metadata table for current target-side Trice sites, first run `trice insert` or `trice bind` and then run `trice generate -src <source> -logC[=<output.c>]`. Multiple `-src` options are accepted. Explicit Insert IDs and numeric Bind sidecar descriptors are validated against the selected TIL; no ID is guessed from a matching format string. Historical TIL entries that are absent from the selected sources are omitted without changing the TIL itself. Bind sidecars are read from `./generated` by default; specify `-genDir` for a different directory. Bare `-logC` writes `./generated/til.c`; an explicit output path takes precedence. `-logC` and `-abc` are alternative generation modes and cannot be combined.

Commented Trice calls with explicit Insert IDs remain selectable. An ID-free call that exists only in a C comment has no Bind preprocessor site and therefore no exact sidecar ID; `-logC` reports it instead of guessing or silently omitting it. Use `trice insert` for such retained commented calls, give the commented example an explicit authoritative ID, or exclude that source from this generated table.

```C
// SPDX-License-Identifier: MIT

#include "triceRx.h"

const triceLog_t triceLog[] = {
	/* Trice type ( extended ) */ /*   id, bitWidth, paramCount, format-string */
	/* trice      ( trice_0   ) */ { 1000u, 32u, 0u, "ready\n" },
	/* trice      ( trice32_2 ) */ { 1001u, 32u, 2u, "value=%d hex=%x\n" },
};

const unsigned triceLogElements = sizeof(triceLog) / sizeof(triceLog[0]);
```

## 4. <a id="c-code-1"></a>C#-Code

The current `trice generate` command does not provide a C# source generator. C# applications can read the generated `til.json` as input to their own decoder or use the Trice host tool to produce text, JSON, or KV output.

## 5. <a id="generating-a-trice-abc-function-pointer-list"></a>Generating a Trice ABC Function Pointer List

Use `-abc=<target>` to generate the target-specific ABC receive selection and table files:

```text
trice generate -i til.json -abc=deviceX
```

Run from the project directory. This creates `generated/deviceX.h` if it does not exist, otherwise uses it as the user-edited selection input. It always regenerates `generated/deviceX.c` from `til.json` and the active declarations in `generated/deviceX.h`. `-genDir` changes the generated directory; an explicit target path such as `-abc=custom/deviceX` keeps that path. For the workflow and examples see [Trice ABC - Asynchronous Broadcast Commands](abc_asynchronous_broadcast_commands.md).



<p align="right">(<a href="#trice-generate">back to top</a>)</p>

