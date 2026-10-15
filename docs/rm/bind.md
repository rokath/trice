# Trice Bind

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

- [1. Overview](#overview)
- [2. Requirements](#requirements)
- [3. Quick Start](#quick-start)
  - [3.1. New ID-Free Project](#new-id-free-project)
  - [3.2. Migration from trice insert](#migration-from-trice-insert)
- [4. Persistent and Generated Files](#persistent-and-generated-files)
- [5. Hierarchical Metadata Reuse](#hierarchical-metadata-reuse)
- [6. File Key and Sidecar Name](#file-key-and-sidecar-name)
- [7. Sidecar Contents](#sidecar-contents)
- [8. Why the Sidecar Include Is File-Local](#why-the-sidecar-include-is-file-local)
- [9. Headers and static inline](#headers-and-static-inline)
- [10. Automatic Include Position](#automatic-include-position)
- [11. File Classification and Mixed Projects](#file-classification-and-mixed-projects)
  - [11.1. Insert-Owned](#insert-owned)
  - [11.2. Bind-Owned](#bind-owned)
  - [11.3. Mixed](#mixed)
  - [11.4. Insert-Owned File After a Bind Header](#insert-owned-file-after-a-bind-header)
- [12. Supported Trice Calls](#supported-trice-calls)
- [13. ID and Stamp Forms](#id-and-stamp-forms)
  - [13.1. ID-Free](#id-free)
  - [13.2. Zero Placeholders](#zero-placeholders)
- [14. Command Line](#command-line)
- [15. Build Integration](#build-integration)
- [16. TRICE_CLEAN](#trice_clean)
  - [16.1. trice clean After trice bind](#trice-clean-after-trice-bind)
- [17. Re-Migration to trice insert](#re-migration-to-trice-insert)
- [18. Automatic Local Counter Rebase](#automatic-local-counter-rebase)
  - [18.1. When the Normal Line Path Is Sufficient](#when-the-normal-line-path-is-sufficient)
  - [18.2. When trice bind Rebases Locally](#when-trice-bind-rebases-locally)
  - [18.3. Concrete Wrapper Example](#concrete-wrapper-example)
  - [18.4. Preferred Form: Normal or static inline Function](#preferred-form-normal-or-static-inline-function)
  - [18.5. Headers and Translation Units](#headers-and-translation-units)
  - [18.6. Compact Source Boundaries and Generated Helper Headers](#compact-source-boundaries-and-generated-helper-headers)
  - [18.7. Missing &#95;&#95;COUNTER&#95;&#95;](#missing-9595counter9595)
  - [18.8. Unchanged Interfaces](#unchanged-interfaces)
- [19. Supported Boundaries and Remaining Limitations](#supported-boundaries-and-remaining-limitations)
  - [19.1. bind-limits](#bind-limits)
- [20. Diagnostics and Troubleshooting](#diagnostics-and-troubleshooting)
  - [20.1. Sidecar Not Found](#sidecar-not-found)
  - [20.2. File-Key Conflict](#file-key-conflict)
  - [20.3. File Is mixed](#file-is-mixed)
  - [20.4. Bind Include Is in the Wrong Place](#bind-include-is-in-the-wrong-place)
  - [20.5. Unexpected Message After a Source Change](#unexpected-message-after-a-source-change)
  - [20.6. Advanced Construct Requires &#95;&#95;COUNTER&#95;&#95;](#advanced-construct-requires-9595counter9595)
  - [20.7. Counter Count or Rebase Descriptor Does Not Match](#counter-count-or-rebase-descriptor-does-not-match)
- [21. Result](#result)
- [22. Appendix: Preprocessor Fundamentals](#appendix-preprocessor-fundamentals)
  - [22.1. Local Insert/Bind Dispatch](#local-insertbind-dispatch)
  - [22.2. Site Descriptor](#site-descriptor)
- [23. Appendix: Stable ID Assignment and Binding Background](#appendix-stable-id-assignment-and-binding-background)
  - [23.1. Stable ID Assignment](#stable-id-assignment)
  - [23.2. Transfer into the Target Code](#transfer-into-the-target-code)
  - [23.3. Why the Source Scan Remains Authoritative](#why-the-source-scan-remains-authoritative)
  - [23.4. Requirements Met by the Sidecar Approach](#requirements-met-by-the-sidecar-approach)
- [24. Appendix: TRICE_CLEAN States at a Glance](#appendix-trice_clean-states-at-a-glance)
- [25. Appendix: Why Bind Uses Local Counter Rebasing](#appendix-why-bind-uses-local-counter-rebasing)
- [26. Appendix: Bind and Insert Test Evidence](#appendix-bind-and-insert-test-evidence)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

This chapter is the reference for the current `trice bind` workflow, supported source constructs and limitations. Ordinary sites use file-and-line binding; supported ambiguous sites use automatic local counter rebasing. The restrictions for Context Enrichment are narrower and are explained under [Bind Limits](#bind-limits).

This chapter describes the supported workflow, generated files, compiler requirements and troubleshooting. Use [Bind Limits](#bind-limits) to choose between Bind and the explicit-ID `insert/clean` workflow for your source constructs.

## 1. <a id="overview"></a>Overview

`trice bind` assigns and manages stable Trice IDs without writing numeric IDs into bind-managed user Trice calls.

For example, the user code remains:

```c
trice("msg:module initialized\n");
```

`trice bind` scans the project sources, uses the existing ID management with `til.json` and `li.json`, and generates one temporary sidecar header for each bind-managed source or header file. The compiler still compiles the original sources directly.

The normal workflow is:

```text
trice bind
Build
```

A subsequent `trice clean` is not required in a stable bind project.

`trice bind` must be run after every change to a scanned source or header file and before the build. An outdated sidecar may still compile in some cases but contain an ID that no longer matches.

## 2. <a id="requirements"></a>Requirements

A bind project requires:

- the normal Trice library under `./src`,
- `til.json`,
- `li.json`,
- the PC tool with the `trice bind` subcommand,
- a build step before C/C++ compilation,
- the sidecar directory on the include path.

The default directory is:

```text
./generated
```

`trice bind` creates the directory when needed. It is normally not version-controlled.

## 3. <a id="quick-start"></a>Quick Start

### 3.1. <a id="new-id-free-project"></a>New ID-Free Project

1. Write Trice calls without numeric IDs:

   ```c
   trice("msg:start\n");
   ```

2. Run `trice bind`:

   ```sh
   trice bind [shared insert options]
   ```

3. Add the sidecar directory to the compiler include path:

   ```text
   -I./generated
   ```

4. Build the project.

`trice bind` adds a file-local include to bind-managed files, for example:

```c
#include "trice_sidecar_module_c_073A915E9C4021B8.h" // trice-bind: keep as last include before this file's Trice calls
```

### 3.2. <a id="migration-from-trice-insert"></a>Migration from `trice insert`

For a project previously managed entirely with `insert`:

```text
trice clean
trice bind
Build
```

`trice clean` removes inserted IDs. `trice bind` then generates file keys, sidecar includes, and sidecars.

## 4. <a id="persistent-and-generated-files"></a>Persistent and Generated Files

The following files and lines are persistent and normally version-controlled:

- user sources,
- `til.json`,
- `li.json`,
- the sidecar include lines added by `trice bind`,
- for counter-dependent constructs, one clearly marked begin and one end include line in each affected source or header file.

The following are generated and normally not version-controlled:

- owner sidecars under `./generated`,
- rebase helper headers in the same directory with the suffixes `_begin.h` and `_end.h`,
- other normal build artifacts.

The owner include line stores the stable file key. The sidecar and rebase helper headers can be regenerated by `trice bind` at any time. Rebase include pairs are validated and logically removed during every bind run, then regenerated from the current source analysis. Formatter-owned horizontal whitespace is retained when the regenerated boundary has the same identity. The lines must not be moved, renamed, or partially edited manually.

If the last managed Trice call is removed from a previously bound file, its owner include and file key remain. Bind generates an owner sidecar without site descriptors, preserving the file identity for later calls. This does not allocate an ID for an absent log site.

## 5. <a id="hierarchical-metadata-reuse"></a>Hierarchical Metadata Reuse

Each `trice bind` invocation has one writable primary TIL, LI, and generated-file directory selected with `-genDir`. Files selected by `-src` may be individual files or directories. Existing valid File Keys remain unchanged; a bind-owned file without a File Key receives one when needed.

For each source, `bind` performs a bounded search from its directory up to its `-src` anchor, optionally one level higher, and around the configured TIL and LI paths. Hidden directories such as `.git` and `.trice` are ignored. Immediate `*.json` files are recognized as TIL or LI data by their contents, so custom names such as `demoIDs.json` work without another option.

Discovered JSON and historical sidecars in `build/triceIDs` are read-only evidence. Sidecars are parsed to recover earlier assignments but are never copied because their line descriptors may be stale. Current sidecars are always regenerated from the current source into the selected generated-file directory.

The primary TIL always wins a numeric-ID conflict. A conflicting subproject ID quietly yields to another matching or newly allocated primary ID; `-verbose` explains such decisions. A conflict-free historical ID is retained and only its actively used mapping is added to the primary TIL. Secondary TILs, LIs, and build artifacts are never modified.

For repeated identical Trice calls (the same normalized type and exact format string) in one file, a valid sidecar assignment takes precedence over conflicting LI positions when selecting ID candidates. A sidecar in the current build directory has priority over discovered sidecars. File modification times do not decide ownership. TIL format compatibility and existing file ownership still have to match. Candidate selection is followed by the [identical-Trice ordering rule](id_management.md#different-ids-for-same-trices), which determines the final per-site assignment.

Without a usable sidecar assignment, LI candidates for repeated calls are consumed in stored line order as the current calls are visited in source order. Metadata search priority is retained; equal stored lines are ordered by numeric ID. Bind does not independently choose the closest old line for each repeated call. A single current call still uses line proximity to select among matching LI candidates. These rules choose the usable pool, not its final permutation: if IDs 15982 and 15849 are selected for two identical calls, the earlier site receives 15849 and the later site 15982.

`li.json` stores one position per ID, not a sequence of past versions. Bind writes the newly assigned positions back to the primary LI. The final ordering uses the already parsed source sites; it requires no additional source scan. Adding, removing, or moving identical calls may deliberately change their earlier IDs. The original per-call association cannot be recovered from TIL alone and is not preserved across such changes.

All discovery and conflict resolution completes before regular output is written. A fatal ambiguity therefore leaves sources, JSON files, and generated outputs unchanged. The complete normative implementation strategy and fallback order are documented in [`internal/id/bindIDs_doc.go`](../../internal/id/bindIDs_doc.go).

## 6. <a id="file-key-and-sidecar-name"></a>File Key and Sidecar Name

Every bind-managed file receives a randomly generated 64-bit key once:

```text
K073A915E9C4021B8
```

Example:

```text
module.c
→ trice_sidecar_module_c_073A915E9C4021B8.h
```

The `trice_sidecar_` prefix identifies generated compiler headers. The readable source base name comes before the key, grouping equal base names when filenames are sorted. The key also distinguishes files with identical names in different directories.

The filename contains the key's 16 uppercase hexadecimal digits. C macro identifiers add the fixed `K` prefix, as shown above. The key is generated once with `crypto/rand` and retained when the file contents, path or name change. The readable sidecar name normalizes characters outside `[A-Za-z0-9_]` to `_`; the key supplies its identity. The key itself is not a transmitted Trice ID and adds no target runtime data.

If a source is copied together with its sidecar include line, both files initially have the same key. `trice bind` detects this as a conflict; one of the files must receive a new key.

Including the same header in multiple translation units is expected and supported.

## 7. <a id="sidecar-contents"></a>Sidecar Contents

A sidecar may look like this:

```c
/// \file trice_sidecar_module_c_073A915E9C4021B8.h
/// \brief Generated by trice bind. Do not edit.

#undef TRICE_BIND_FILE_KEY
#define TRICE_BIND_FILE_KEY K073A915E9C4021B8
#define TRICE_BIND_ROUTE_K073A915E9C4021B8 BIND

// -defaultStampSize 16
#define TRICE_BIND_SITE_K073A915E9C4021B8_L9 TRICE_BIND_AUTO, Id(12345u) // TRICE("Hello");
#define TRICE_BIND_SITE_K073A915E9C4021B8_L10 TRICE_BIND_REPLACE, id(12346u) // TRICE(id(0), "world");
#define TRICE_BIND_SITE_K073A915E9C4021B8_L11 TRICE_BIND_AUTO, iD(12347u) // trice("!\n");
```

Meaning:

- `TRICE_BIND_AUTO`: The TID expression is inserted as the missing first argument.
- `TRICE_BIND_REPLACE`: An existing zero placeholder is replaced with the stable ID.
- `iD`, `id`, `Id`, and `ID` retain the existing Trice stamp semantics.
- The comments support diagnostics.
- Each site definition occupies one physical line.

During preprocessing, the ID becomes a normal compile-time constant. The target needs neither a string lookup nor an additional runtime mapping table.

Owner sidecars intentionally have no conventional include guard: including an owner sidecar again must reactivate that physical file's bind context. Do not add a guard or maintain generated descriptors manually.

## 8. <a id="why-the-sidecar-include-is-file-local"></a>Why the Sidecar Include Is File-Local

The include does more than provide ID definitions. It activates the bind context of the physical file for the following Trice calls.

Within one translation unit, sidecars for several headers and the `.c` file can become active in sequence. Therefore, each file must activate its own file key before its own Trice calls.

Typical `.c` file:

```c
#include "trice.h"
#include "module.h"
#include "driver.h"
#include "trice_sidecar_module_c_073A915E9C4021B8.h" // trice-bind: keep as last include before this file's Trice calls

void moduleInit(void)
{
    trice("msg:module initialized\n");
}
```

A central include of all sidecars in `triceConfig.h` cannot replace this local selection.

## 9. <a id="headers-and-static-inline"></a>Headers and `static inline`

Headers containing direct Trice calls receive their own sidecar:

```c
#ifndef MODULE_H
#define MODULE_H

#include "trice.h"
#include "dependency.h"
#include "trice_sidecar_module_h_1111111111111111.h" // trice-bind: keep as last include before this file's Trice calls

static inline void moduleCheck(int value)
{
    trice("msg:value=%d\n", value);
}

#endif
```

All translation units use the same stable ID for this textual header site.

For reusable logging helpers with multiple Trice sites, a `static inline` function is normally preferable to a preprocessor macro. Its Trice sites can use the normal file-and-line path and therefore need no rebase includes at each call site. [Preferred Form: Normal or `static inline` Function](#preferred-form-normal-or-static-inline-function) shows the recommended implementation and the semantic differences.

After the header include, the `.c` file activates its own file key again through its own sidecar.

The same owner sidecar may be included several times within a file if a later header switches the active file key.

## 10. <a id="automatic-include-position"></a>Automatic Include Position

If the sidecar include is missing, `trice bind` uses a conservative heuristic:

1. It finds the last include before the first bindable Trice site.
2. It inserts the sidecar immediately after that include.
3. If there is no preceding include, it inserts the sidecar directly before the first bindable Trice site.
4. If a bindable Trice site appears before a later include, no unsafe automatic change is made; the user receives a diagnostic.

An existing valid include is not moved unnecessarily.

The `// trice-bind: ...` comment is a developer aid. Technical detection uses the include directive, sidecar name, and file key; removing only the comment is allowed.

## 11. <a id="file-classification-and-mixed-projects"></a>File Classification and Mixed Projects

`trice bind` classifies every physical file.

### 11.1. <a id="insert-owned"></a>Insert-Owned

All managed Trice calls have explicit IDs greater than zero:

```c
trice(iD(123), "msg:legacy\n");
```

`trice bind` validates the file but does not modify it or generate a sidecar.

### 11.2. <a id="bind-owned"></a>Bind-Owned

The file contains only ID-free calls and/or zero placeholders:

```c
trice("msg:bound\n");
TRICE(ID(0), "msg:bound with stamp\n");
```

`trice bind` manages the file key, include, and sidecar.

### 11.3. <a id="mixed"></a>Mixed

A file contains both forms:

```c
trice(iD(123), "msg:legacy\n");
trice("msg:new\n");
```

This state is not allowed. The file must be managed entirely by either `insert` or `bind`. Regions excluded with `TRICE_INSERT_OFF` and `TRICE_INSERT_ON` are not managed and do not contribute to this classification.

### 11.4. <a id="insert-owned-file-after-a-bind-header"></a>Insert-Owned File After a Bind Header

A bind-owned header can be included by an insert-owned file. Before its own explicitly instrumented Trice calls, the file must remove the bind context:

```c
#include "bound_header.h"

#undef TRICE_BIND_FILE_KEY

trice(iD(123), "msg:insert-owned source\n");
```

This hybrid case is possible but is not the preferred normal workflow.

## 12. <a id="supported-trice-calls"></a>Supported Trice Calls

`trice bind` uses the same parser, ID assignment and user-level macro detection as `trice insert`.

In particular, the following are supported:

- the public lowercase, mixed-case, and uppercase families,
- 8-, 16-, 32-, and existing 64-bit forms,
- arity-encoded forms,
- string, count, buffer, float, RPC/ABC, and assertion families,
- special macros such as `triceAssertOrReturnValue`, where they are part of the public interface,
- the `-alias` and `-salias` names supported by `insert`,
- direct calls in C, C++, and header files,
- normal, `inline`, and `static inline` functions,
- multiple ID-free calls on the same physical line,
- the ordinary statement wrappers described under [Automatic Local Counter Rebase](#automatic-local-counter-rebase).

`TRICE_INSERT_OFF` and `TRICE_INSERT_ON` behave as they do with `trice insert`.

## 13. <a id="id-and-stamp-forms"></a>ID and Stamp Forms

### 13.1. <a id="id-free"></a>ID-Free

```c
trice("msg:hello\n");
TRICE8_3("msg:%d %d %d\n", a, b, c);
```

For macro names containing at least one lowercase letter, the sidecar uses `iD(...)`.

For all-uppercase user-level macros, `-defaultStampSize` determines the form:

- `0` → `id(...)`,
- `16` → `Id(...)`,
- `32` → `ID(...)`.

### 13.2. <a id="zero-placeholders"></a>Zero Placeholders

```c
TRICE8_3(id(0), "msg:%d %d %d\n", a, b, c);
TRICE8_3(Id(0), "msg:%d %d %d\n", a, b, c);
TRICE8_3(ID(0), "msg:%d %d %d\n", a, b, c);
```

The wrapper form is retained; semantically, the sidecar replaces only the zero with the stable ID. An explicit zero placeholder therefore takes precedence over `-defaultStampSize`. Zero placeholders are supported on ordinary line-addressable sites, not in counter-selected regions.

## 14. <a id="command-line"></a>Command Line

Basic form:

```sh
trice bind [options]
```

Short form:

```sh
trice b [options]
```

In general, `bind` accepts the `insert` options relevant to source search, parsing, ID assignment, alias handling, `til.json`, and `li.json`.

`bind`, `insert`, and `generate` accept `-genDir`:

```text
-genDir string
    Directory for generated Trice files, relative to the invocation directory.
    Default: ./generated
```

`bind` writes sidecar headers and `trice-fields.txt` there; `insert` writes `trice-fields.txt`. `generate -logC` reads bind sidecars there and writes `til.c` there when no output path is supplied. `generate -onelineJSON` writes `<name>.oneline.json` views there; `generate -abc target` writes `target.h` and `target.c` there. Explicit `-logC=path/file.c` and `-abc path/target` paths retain their own location. Add `./generated` to the compiler include path when building bound sources. `-buildDir` and `-bindDir` are no longer accepted.

Bind automatically excludes its selected generated-file directory from the source scan. This prevents generated descriptors from being treated as new user log sites, including when `-src` names a parent directory.

For example, run Bind from your project root, then optionally generate a local-formatting table or an ABC dispatcher:

```sh
touch til.json li.json
trice bind -src application -genDir generated -til til.json -li li.json
trice generate -src application -genDir generated -til til.json -logC
trice generate -genDir generated -til til.json -abc device_abc
```

The first line initializes missing dictionaries for a new project without emptying existing ones. Bind requires a TIL file so that a missing dictionary is not silently replaced with new IDs.

The resulting layout separates application files, persistent dictionaries, generated files and compiler output:

```text
project/
  application/                 application sources and triceConfig.h
  til.json                     persistent ID-to-format dictionary
  li.json                      source-location information
  generated/                   -genDir, relative to the command's working directory
    trice_sidecar_main_c_....h   Bind sidecar (representative filename)
    trice-fields.txt            field registry
    til.c                      optional local-formatting table from -logC
    device_abc.h               ABC selection header; user-owned after creation
    device_abc.c               generated ABC dispatch table
  build/                       compiler output, chosen by your build system
```

The two `generate` calls serve different features; an ordinary host-decoded application does not need either one. Add `-Igenerated` when compiling from the project root, or `-I../generated` when compiling from `build`. A relative `-genDir` is resolved from the command's working directory, not from the Trice library's directory or automatically from `build`.

Keep `til.json` under version control and retain the local `li.json` between builds for ID ownership and reuse, even when logging with `-li off`. The repository demos generate their ignored LI files locally; applications may choose to version LI when source ownership must survive checkouts. Preserve dictionaries and matching LI artifacts belonging to firmware whose logs you still need to investigate. Include an edited ABC selection header in your version control even if it is stored under `generated`. Do not delete that directory wholesale: it may contain user-owned files, and old sidecars may still provide historical ID evidence. The read-only [Bind artifact report](generate.md#bind-artifact-report) helps inspect ownership and references without treating an unreferenced file as safe to delete.

With:

```sh
trice bind -dry-run
```

planned changes are calculated and displayed, but no user, JSON, or sidecar files are written.

## 15. <a id="build-integration"></a>Build Integration

`trice bind` is a required generator step before compilation:

```text
Source change
→ trice bind
→ C/C++ build
```

The build system should:

- run `trice bind` before dependent compilations,
- add `./generated` to the include path,
- track sidecars as normal header dependencies,
- not replace the generator with a mere compiler failure.

The generator replaces a sidecar file only if its contents change. This keeps incremental builds limited to the translation units that are actually affected.

To inspect an existing generation directory without running Bind again, use the explicit [`generate -bindReport` inventory](generate.md#bind-artifact-report). It maps sidecars and rebase helpers to physical includes in a selected source scan, reports uncertain ownership, and changes no files. A partial scan does not establish which artifacts a compiler uses or which files are safe to delete.

## 16. <a id="trice_clean"></a>`TRICE_CLEAN`

`TRICE_CLEAN` is an optional editor aid for the legacy Insert/Clean workflow. Normally leave it undefined; the library defaults to `0`. Bind sidecars let editors resolve ID-free calls without this setting.

If your editor reports false macro-argument warnings after `trice clean`, you can explicitly define `TRICE_CLEAN` in your project's `triceConfig.h` only. Use `0` for inserted sources or `1` for already cleaned sources. `trice clean` changes the existing definition to `1`; `trice insert` and `trice bind` reset it to `0`. The tool never adds this definition automatically.

**The compiler sees this value too:** `TRICE_CLEAN=1` suppresses ordinary logging code. Run Insert or Bind before building firmware that must log. For deliberately disabling logging, use `TRICE_OFF` instead. `trice bind` does not introduce a new global `TRICE_MODE`.

With `-v`, Bind prints a note when it finds a `TRICE_CLEAN` definition in a scanned `triceConfig.h`. The note explains that Bind normally does not need this optional Insert/Clean editor aid. If Bind plans to reset `1` to `0`, it reports that change explicitly. This is informational and does not fail the command. With `-dry-run`, the planned reset is reported but not written; without `-v`, these notes are omitted.

If `TRICE_CLEAN` exists in `triceConfig.h`:

- `trice bind` keeps its value at `0`,
- `trice bind` does not add the definition,
- `trice bind` does not remove it automatically.

### 16.1. <a id="trice-clean-after-trice-bind"></a>`trice clean` After `trice bind`

Bind-owned Trice calls already contain no IDs greater than zero. Therefore, `trice clean` does not remove IDs from them and deletes neither sidecar includes nor sidecars.

With an existing definition:

```c
#define TRICE_CLEAN 0
```

`trice clean` changes it as before to:

```c
#define TRICE_CLEAN 1
```

The library then uses the clean/off path. Existing sidecars remain as artifacts but do not produce normal logging code.

Without `TRICE_CLEAN`, running `trice clean` after a bind run has practically no effect:

- user Trice calls remain ID-free,
- sidecar includes remain,
- sidecars remain,
- the bind context remains available for the next build.

Running `trice bind` again resets an existing definition to `0` and updates the sidecars.

`TRICE_CLEAN=1` disables logging; it does not make an included header optional. If an owner sidecar or rebase helper is physically missing, the compiler may still report a missing include even in a disabled build. Regenerate the files with `trice bind` before compiling; do not delete isolated include lines to silence the error.

## 17. <a id="re-migration-to-trice-insert"></a>Re-Migration to `trice insert`

A public re-migration subcommand is still not part of the normal user workflow. However, the repository helper script for returning to the clean state uses the same validated bind re-migration as the tests. It removes complete rebase include pairs, their helper headers, sidecar includes, and owner sidecars together, and corrects the affected lines in `li.json`.

Anyone performing the return manually must remove all related artifacts and must not leave behind an individual begin or end include line. The previous workflow then follows:

```text
trice insert
Build
```

## 18. <a id="automatic-local-counter-rebase"></a>Automatic Local Counter Rebase

Automatic local counter rebasing does not add another command to the normal workflow:

```text
trice bind
Build
```

The user neither sees nor maintains counter values or local ordinals. `__COUNTER__` is used only as a local compile-time selector; its value is never a Trice ID.

### 18.1. <a id="when-the-normal-line-path-is-sufficient"></a>When the Normal Line Path Is Sufficient

Unambiguous sites continue to use only the file key and `__LINE__`. These include:

- one direct Trice call per physical line,
- Trice calls in normal, `inline`, and `static inline` functions,
- a statement wrapper with exactly one inner Trice site when its calls are unambiguous by line.

Such source and header files receive no counter guard, rebase boundaries, or rebase helper headers. A compiler without `__COUNTER__` can build them as before.

### 18.2. <a id="when-trice-bind-rebases-locally"></a>When `trice bind` Rebases Locally

A local rebase is generated automatically only where file and line cannot distinguish an expansion unambiguously:

```c
trice("msg:first\n"); trice("msg:second\n"); trice("msg:third\n");
```

This also applies to wrappers with several inner Trice sites, multiple wrapper calls on the same physical line, and a wrapper call written across several lines. Before the smallest safely enclosing region, the generated code captures a local counter base value. Immediately afterwards, it restores the normal bind path. Earlier counter consumption in the same translation unit is therefore irrelevant.

The smallest region is normally exactly one physical source line. Two independent adjacent lines are deliberately not combined into a common rebase. A larger region could save include lines, but it could also contain unrelated macro expansions, conditional compilation, or an additional indirect use of `__COUNTER__`. A local change would then unnecessarily affect several log sites.

One necessary exception is a single, syntactically connected wrapper call whose argument list spans several physical lines:

```c
LOG_ERROR(
    determineErrorCode(
        fileHandle,
        operation
    )
);
```

A preprocessor directive such as `#include` must occupy its own physical line and cannot be inserted into an open argument list. Therefore, the minimal rebase in this case covers the complete call from the macro name through the terminating semicolon. This does not combine independent statements; it is the smallest syntactically possible enclosure of a single call.

### 18.3. <a id="concrete-wrapper-example"></a>Concrete Wrapper Example

An ordinary statement macro can be defined and used as follows:

```c
#define LOG_ERROR(value)                                      \
    do {                                                      \
        switch (value) {                                      \
        case 0:                                               \
            break;                                            \
        case 7:                                               \
            trice("cannot open file\n");                     \
            break;                                            \
        default:                                              \
            trice("error=%d", 8);                             \
            break;                                            \
        }                                                     \
    } while (0)

void report(int status)
{
    LOG_ERROR(status);
    LOG_ERROR(7); LOG_ERROR(8);
}
```

The two Trice sites in `LOG_ERROR` receive a total of two stable IDs. The first ID permanently belongs to `cannot open file`, and the second to `error=%d`. Every wrapper call uses these two definition IDs in definition order; it does not create additional IDs. The fact that only one `switch` branch executes at runtime does not change the preprocessor order.

For both IDs, `li.json` points to the respective inner definition site in the macro. The call sites contain only generated selection descriptors.

### 18.4. <a id="preferred-form-normal-or-static-inline-function"></a>Preferred Form: Normal or `static inline` Function

If a logging helper does not need genuine preprocessor functionality, it should preferably be written as a normal or `static inline` function. The recommended form of the previous example is:

```c
static inline void logError(int value)
{
    switch (value) {
    case 0:
        break;
    case 7:
        trice("cannot open file\n");
        break;
    default:
        trice("error=%d", 8);
        break;
    }
}

void report(int status)
{
    logError(status);
    logError(7);
    logError(8);
}
```

The two Trice calls now occupy two unambiguous physical lines inside the function. They use the normal file-key-plus-`__LINE__` path. The three `logError` call sites require neither local counter scopes nor additional begin/end includes. This minimizes the source insertions made by `trice bind` and the number of generated rebase helper headers. A target compiler without `__COUNTER__` can also compile this construct.

`static inline` does not imply that a runtime function call must be generated. Depending on optimization, size, and target architecture, common C and C++ compilers can insert the function directly at the call site. Whether they actually inline it remains a compiler decision; the stable Trice ID does not depend on that decision.

For a definition in a header file, the sidecar belongs to the header. All translation units use the same two textual Trice sites and therefore the same stable IDs. For a definition in a `.c` file, the sites belong to that `.c` file accordingly.

When converting a macro to a function, observe the normal C/C++ differences:

- Function arguments are evaluated exactly once and are type-checked.
- A macro may intentionally process tokens, type names, or compile-time configuration; a function cannot always replace that behavior.
- `return`, `break`, or local declarations intended to affect the caller's context must not be moved blindly from a macro into a function.
- If the call site itself is required as log metadata, a function moves the Trice site by definition into its own function body.

For ordinary helpers such as `LOG_ERROR(value)`, which merely select among several fixed Trice messages based on a value, `static inline` is the most robust and simplest form. A logging macro with several inner Trices should be used only when its preprocessor semantics are actually required.

### 18.5. <a id="headers-and-translation-units"></a>Headers and Translation Units

If `LOG_ERROR` is defined in `logging.h` and called from several `.c` files, its definition IDs remain identical in all translation units. The rebase for a call resides in the respective calling file.

If the wrapper call or a line with several direct Trices is itself located in a header, the rebase is also located in that header. Every translation unit that processes this exact header region then requires `__COUNTER__`. Unrelated files and ordinary headers do not become counter-dependent. The local base value makes the number of counter values consumed before the header irrelevant.

### 18.6. <a id="compact-source-boundaries-and-generated-helper-headers"></a>Compact Source Boundaries and Generated Helper Headers

An affected single-line statement is enclosed by exactly two clearly marked include lines:

```c
#include "trice_sidecar_module_c_073A915E9C4021B8_R0_begin.h" // trice-bind: generated rebase begin K073A915E9C4021B8_R0
trice("first"); trice("second");
#include "trice_sidecar_module_c_073A915E9C4021B8_R0_end.h" // trice-bind: generated rebase end K073A915E9C4021B8_R0
```

One affected user line therefore becomes three source lines. Scope definitions, phase macros, and cleanup directives that older generator versions exposed directly in the source are now located entirely in the two generated helper headers under `./generated`. The begin file captures the local counter base and activates the appropriate selection descriptor. The end file checks the consumed counter count and restores the normal bind path.

Two independent lines remain two independent regions:

```c
#include "trice_sidecar_module_c_073A915E9C4021B8_R0_begin.h" // trice-bind: generated rebase begin K073A915E9C4021B8_R0
trice("first"); trice("second");
#include "trice_sidecar_module_c_073A915E9C4021B8_R0_end.h" // trice-bind: generated rebase end K073A915E9C4021B8_R0
#include "trice_sidecar_module_c_073A915E9C4021B8_R1_begin.h" // trice-bind: generated rebase begin K073A915E9C4021B8_R1
trice("third"); trice("fourth");
#include "trice_sidecar_module_c_073A915E9C4021B8_R1_end.h" // trice-bind: generated rebase end K073A915E9C4021B8_R1
```

`trice bind` does not combine these lines even when they are adjacent. Saving two include lines does not justify increasing the region affected by the counter. In particular, an unrelated macro between two log lines must never endanger the mapping of both lines together.

A single multiline wrapper call, however, is enclosed as one syntactic unit:

```c
#include "trice_sidecar_module_c_073A915E9C4021B8_R2_begin.h" // trice-bind: generated rebase begin K073A915E9C4021B8_R2
LOG_ERROR(
    determineErrorCode(
        fileHandle,
        operation
    )
);
#include "trice_sidecar_module_c_073A915E9C4021B8_R2_end.h" // trice-bind: generated rebase end K073A915E9C4021B8_R2
```

The boundary appears before the call's first line and after the line containing its semicolon. No directive is inserted into the open argument list. If this minimal region itself contains a preprocessor directive, another unassignable Trice site on a boundary line, or a `__COUNTER__` expansion that cannot be safely bounded, `trice bind` rejects the site and changes no regular output files.

A rebase over an entire function, several independent statements, or the whole file is deliberately not generated. Technically, such a region could work as long as exactly the expected Trice macros—and no other expansion—consume `__COUNTER__`. In practice, every included line increases the dependency on unrelated macros, build configurations, and conditional compilation. Minimal enclosure limits a possible error to exactly one source site, and the final check turns a discrepancy into a compiler error instead of a silently incorrect ID.

The include lines and helper headers are related generator artifacts. They must not be moved, renamed, split, or deleted individually. After relevant source changes, `trice bind` must run again; the generator validates existing artifacts and transactionally updates source boundaries, helper headers, descriptors, IDs, and location information. Another bind run can restore missing helper headers. Modified helper headers are rejected with a diagnostic instead of being silently overwritten. Helper headers that are no longer needed are removed.

Older multiline rebase blocks from a previous generator version are still recognized. A successful new bind run replaces them with the compact include boundaries.

<a id="missing-__counter__"></a>
<!-- Keep __COUNTER__ HTML-encoded because mdtoc strips code-span markers from generated ToC labels; the preceding alias preserves existing links. -->
### 18.7. <a id="missing-9595counter9595"></a>Missing &#95;&#95;COUNTER&#95;&#95;

Only the affected rebase region contains a capability guard. If `__COUNTER__` is unavailable, the target compiler stops with a message explicitly stating that normal bind sites are unaffected.

The available alternatives are:

- preferably express an ordinary logging helper as a normal or `static inline` function,
- place multiple direct Trice calls on separate physical lines,
- if the macro form is indispensable, use a target compiler with `__COUNTER__` or the `trice insert` / `trice clean` workflow.

The `static inline` form is especially advisable for frequently called `LOG_ERROR`-style helpers: one function definition replaces rebase includes and helper headers at every individual call site.

Compile-time checks also detect additional counter consumption within the region, an incorrect expansion count, and missing generated descriptors. Such discrepancies stop the build instead of silently selecting a different ID.

### 18.8. <a id="unchanged-interfaces"></a>Unchanged Interfaces

The rebase introduces no mutable runtime state, dynamic allocation, or runtime ID table. `til.json`, `li.json`, the Trice wire format, decoders, and public CLI options remain unchanged.

`TRICE_CLEAN=1` and `TRICE_OFF=1` still disable the Trice macros completely. Generated rebase helper headers are processed without a counter check in these build modes, so a disabled build does not require `__COUNTER__`, even for a file that would otherwise depend on it.

## 19. <a id="supported-boundaries-and-remaining-limitations"></a>Supported Boundaries and Remaining Limitations

ID-free Trice calls with a statically and directly recognizable format string are supported, as are ordinary function-like statement macros with one or more direct Trice calls. A single wrapper call may span several physical lines if its complete region through the semicolon can be enclosed unambiguously and safely.

Within a counter-selected region, the following are still rejected with a precise diagnostic:

- `id(0)`, `Id(0)`, or `ID(0)`, and explicit IDs greater than zero,
- nested or recursive logging wrappers,
- token pasting in the wrapper,
- stringification that generates or changes the format string,
- dynamically composed format strings,
- indirect redefinitions of Trice macros,
- explicit or unbounded additional `__COUNTER__` consumption,
- preprocessor directives within a multiline wrapper call,
- additional Trice or wrapper sites on a physical boundary line of a multiline call,
- expression contexts and control-flow continuations across physical line boundaries that cannot be enclosed safely,
- conflicting wrapper definitions for which no unambiguous common semantics can be determined.

Zero placeholders on ordinary bind sites that are unambiguous by line remain supported. Format strings must still be statically recognizable within the scope of the shared insert/bind parser.

For an unsupported site, `trice bind` does not silently fall back to insert and does not write a numeric ID into the user Trice call.

### 19.1. <a id="bind-limits"></a>bind-limits

When `bind` rejects a source construct, it cannot safely map or support the log sites it contains. The short hint `Search UM for "bind-limits".` points to this section. The error message still includes the file, line and specific cause. A compiler error for a required but unavailable `__COUNTER__` also includes this reference.

Compiler requirements depend on the selected use case:

Use case | Mapping and requirement
--- | ---
Ordinary Bind with direct calls on distinct source lines | File key and source line identify the call; no `__COUNTER__` is needed.
Direct Context Enrichment | The same source-line mapping is sufficient. Each expression must be valid at its selected call site; selected wrapper/rebase sites are rejected.
Bind wrappers or multiple calls requiring rebasing, without CE | Counter-based mapping and consistency checks require compiler support for `__COUNTER__`.
Experimental CE for wrapper/rebase sites | The [extended PoC](context_enrichment.md#extended-poc-for-wrapper-macros-and-counter-rebasing) tests an additional preprocessing pass. It is not a production CLI feature.

The PoC's [compiler matrix](context_enrichment.md#compiler-matrix-and-evidence-limits) records actual evidence, not a promise for every compiler accepting C or C++. In particular, the existing enum-based rebase check fails strict C++20 builds with the tested warning settings, even without CE. This is a rebase limitation, not a general rejection of ordinary direct Bind calls. MSVC, IAR and Arm Compiler/armclang were not tested in that proof. ARM cross-compilation demonstrates that object files can be built; it does not demonstrate execution on an MCU. The `insert/clean` workflow remains available when Bind's mapping or compiler requirements do not fit the project.

For a direct Trice call, the file and source line normally suffice for mapping. Multiple calls on the same line, or a wrapper macro containing multiple calls, may need additional support. Bind uses the compiler counter `__COUNTER__` for this. It counts during compilation; it is neither a runtime counter nor a cycle counter. Not every compiler provides it. Direct, uniquely addressable log sites work without it.

With [Context Enrichment](context_enrichment.md) (`bind -ce`), additional values must be available precisely at the selected log site. A variable that exists only inside one function or block must not also be required at an unrelated site. The existing implementation of complex Bind sites would make the compiler check expressions from other scopes as well. Therefore, CE currently supports only direct sites uniquely addressable by source line. A direct call spanning multiple lines is also possible if none of its lines contains another Bind log site. A selected wrapper/rebase site is rejected before files are changed. The separate architecture proof is documented in the [CE PoC appendix](context_enrichment.md#extended-poc-for-wrapper-macros-and-counter-rebasing); integrating that approach into production remains deferred. Available `__COUNTER__` alone is insufficient. Without a matching CE rule, existing Bind capabilities still apply.

Possible adjustments are:

- **Put direct calls on separate lines.** Change `trice("msg:first"); trice("msg:second");` to:

  ```c
  trice("msg:first");
  trice("msg:second");
  ```

- **Replace suitable wrappers with an ordinary or `static inline` function.** Put each Trice call on its own line within the function. Pass required local values as parameters; a function cannot automatically see its caller's local variables. For example:

  ```c
  static inline void logPosition(int x, int y)
  {
      trice("msg:x=%d, y=%d", x, y);
  }
  ```

  The caller passes its values with `logPosition(pos.x, pos.y);`. This is suitable only for wrappers that need no special macro capabilities. The [guidance on replacing wrappers with functions](#preferred-form-normal-or-static-inline-function) explains the differences.

- **Use the permanently available `insert/clean` workflow.** `trice insert` writes IDs directly into log sites; `trice clean` removes them again. This avoids Bind mapping through source lines or compiler counters. Format strings and calls must still be recognizable by the Trice parser. For already bound projects, first follow [re-migration to `trice insert`](#re-migration-to-trice-insert); do not remove individual generated Bind files or include lines in isolation.

Automatic `insert/clean -ce` is also available: Insert extends recognized source calls, including static wrapper definitions, and Clean removes matching extensions using the same rules. This path needs neither Bind line mapping nor `__COUNTER__`. Additional values can still be specified explicitly in the format string and arguments, for example `trice("msg:Value=%d, x={x}", value, x);`. Examples and removal conditions are described in the [CE chapter](context_enrichment.md#reversible-workflow-with-insert-and-clean).

## 20. <a id="diagnostics-and-troubleshooting"></a>Diagnostics and Troubleshooting

### 20.1. <a id="sidecar-not-found"></a>Sidecar Not Found

Check:

- whether `trice bind` was run after the last source change,
- whether `./generated` exists,
- whether the directory is on the compiler include path,
- whether the sidecar name in the include line is correct.

### 20.2. <a id="file-key-conflict"></a>File-Key Conflict

Typical cause: A source was copied together with its sidecar include line.

Solution: Remove the copied sidecar include from one copy and run `trice bind` again so that a new key is generated.

### 20.3. <a id="file-is-mixed"></a>File Is `mixed`

Either:

- run `trice clean` for this file or managed scope and bind afterwards,
- or keep all Trice calls in this file in the inserted workflow.

### 20.4. <a id="bind-include-is-in-the-wrong-place"></a>Bind Include Is in the Wrong Place

The sidecar must be active when the direct Trice calls of the physical file are expanded. In particular, headers included later can activate a different file key.

### 20.5. <a id="unexpected-message-after-a-source-change"></a>Unexpected Message After a Source Change

Run `trice bind` again. The line number is part of the build-local site name.

<a id="advanced-construct-requires-__counter__"></a>
<!-- Keep __COUNTER__ HTML-encoded because mdtoc strips code-span markers from generated ToC labels; the preceding alias preserves existing links. -->
### 20.6. <a id="advanced-construct-requires-9595counter9595"></a>Advanced Construct Requires &#95;&#95;COUNTER&#95;&#95;

The compiler is processing a generated rebase region but does not provide `__COUNTER__`. Only this source construct is affected. Use one of the alternatives described under [Missing `__COUNTER__`](#missing-__counter__) or a target compiler with local counter support.

### 20.7. <a id="counter-count-or-rebase-descriptor-does-not-match"></a>Counter Count or Rebase Descriptor Does Not Match

The build is using outdated or manually modified generator artifacts, or an additional counter is expanded inside the region. Do not repair generated include boundaries and helper headers manually. Inspect the source construct and run `trice bind` again.

## 21. <a id="result"></a>Result

With `trice bind`, bind-managed user Trice calls remain free of numeric IDs. The stable ID truth remains in `til.json` and `li.json`; a reproducible sidecar passes the ID as a compile-time constant to the existing Trice transport path. Unambiguous sites continue to use the existing line path, while only ambiguous regions are locally rebased and checked at compile time.

---

## 22. <a id="appendix-preprocessor-fundamentals"></a>Appendix: Preprocessor Fundamentals

### 22.1. <a id="local-insertbind-dispatch"></a>Local Insert/Bind Dispatch

A simple `#ifdef TRICE_BIND_FILE_KEY` while reading `trice.h` is insufficient because the sidecar is normally included later.

Instead, the selection is expanded at the actual call site:

```c
#define TRICE_BIND_ROUTE_TRICE_BIND_FILE_KEY INSERT
#define TRICE_BIND_ROUTE_I(key) TRICE_BIND_ROUTE_##key
#define TRICE_BIND_ROUTE(key) TRICE_BIND_ROUTE_I(key)

#define TRICE_DISPATCH_I(route, ...) TRICE_ROUTE_##route(__VA_ARGS__)
#define TRICE_DISPATCH(route, ...) TRICE_DISPATCH_I(route, __VA_ARGS__)

#define trice(...) TRICE_DISPATCH(TRICE_BIND_ROUTE(TRICE_BIND_FILE_KEY), __VA_ARGS__)
```

Without an active sidecar, `TRICE_BIND_FILE_KEY` remains as a token:

```text
TRICE_BIND_ROUTE(TRICE_BIND_FILE_KEY)
→ TRICE_BIND_ROUTE_TRICE_BIND_FILE_KEY
→ INSERT
```

With an active sidecar:

```c
#define TRICE_BIND_FILE_KEY K073A915E9C4021B8
#define TRICE_BIND_ROUTE_K073A915E9C4021B8 BIND
```

it expands to:

```text
TRICE_BIND_ROUTE(TRICE_BIND_FILE_KEY)
→ TRICE_BIND_ROUTE(K073A915E9C4021B8)
→ TRICE_BIND_ROUTE_K073A915E9C4021B8
→ BIND
```

### 22.2. <a id="site-descriptor"></a>Site Descriptor

The site name is formed from the file key and `__LINE__`:

```c
#define TRICE_BIND_SITE_I(key, line) TRICE_BIND_SITE_##key##_L##line
#define TRICE_BIND_SITE(key, line) TRICE_BIND_SITE_I(key, line)
#define TRICE_BIND_SITE_HERE() TRICE_BIND_SITE(TRICE_BIND_FILE_KEY, __LINE__)
```

A descriptor:

```c
#define TRICE_BIND_SITE_K073A915E9C4021B8_L10 TRICE_BIND_REPLACE, id(12346u)
```

provides both the operation and the complete TID expression.

`TRICE_BIND_AUTO` inserts the TID. `TRICE_BIND_REPLACE` discards an existing `id(0)`, `Id(0)`, or `ID(0)` and uses the bound TID.

The [generated-target integration tests](../../internal/id/bindIntegration_test.go) check these mechanisms using current Bind output, including C/C++ compilation, invalid counter sequences and emitted runtime IDs.

---

## 23. <a id="appendix-stable-id-assignment-and-binding-background"></a>Appendix: Stable ID Assignment and Binding Background

The development of `trice bind` is based on separating two tasks.

### 23.1. <a id="stable-id-assignment"></a>Stable ID Assignment

The first task is:

```text
logical Trice site → stable numeric ID
```

It includes:

- recognizing known sites again,
- assigning new IDs,
- ID ranges and policy,
- maintaining `til.json` and `li.json`,
- long-term decodability.

This persistent mapping does not have to reside in the source code.

### 23.2. <a id="transfer-into-the-target-code"></a>Transfer into the Target Code

The second task is:

```text
stable numeric ID → target code
```

`trice insert` solves it with a numeric ID in the user Trice call. `trice bind` solves it with a generated sidecar and standardized preprocessor facilities.

After preprocessing, the compiler likewise sees a normal constant. Therefore, there is:

- no runtime lookup,
- no target-side string search,
- no additional mapping table,
- no change to the wire format.

### 23.3. <a id="why-the-source-scan-remains-authoritative"></a>Why the Source Scan Remains Authoritative

`trice bind` scans the project sources before preprocessing. This allows sites in currently inactive `#if` branches to retain a stable ID as well.

That is beneficial for ID stability: changing the build configuration does not remove the persistent identity of a log site.

A future analysis of the active configuration or the final image would be an additional reporting function. It is not required for binding.

### 23.4. <a id="requirements-met-by-the-sidecar-approach"></a>Requirements Met by the Sidecar Approach

The chosen approach combines:

- ID-free bind-managed user sources,
- stable IDs and the existing ID policy,
- cumulative `til.json` and `li.json`,
- immediate compile-time constants,
- portable C/C++ preprocessor mechanisms,
- direct compilation of the original sources,
- no build-specific ELF requirement for later decoding.

The former standalone architecture paper “Trice IDs Without Source-Code Patching” has been superseded by the current Bind description in this manual. Its conclusions that remain valid are summarized in this appendix.

---

## 24. <a id="appendix-trice_clean-states-at-a-glance"></a>Appendix: `TRICE_CLEAN` States at a Glance

State     |    `TRICE_CLEAN` | Own sidecar active | Effect
-----------|-----------------:|-------------------:|-------------------------------
Inserted  | undefined or `0` |                 no | Explicit TIDs from the source
Bound     | undefined or `0` |                yes | TIDs from the sidecar
Clean/Off |              `1` |         irrelevant | Existing clean/off path

Tool effect when the definition exists:

Tool           | Effect on `TRICE_CLEAN`
----------------|------------------------:
`trice insert` |          sets it to `0`
`trice clean`  |          sets it to `1`
`trice bind`   |          sets it to `0`

If the definition is absent, `trice bind` does not add it.

---

## 25. <a id="appendix-why-bind-uses-local-counter-rebasing"></a>Appendix: Why Bind Uses Local Counter Rebasing

The original design comparison considered three ways to distinguish log sites that share a source line or occur inside a wrapper macro. These were alternatives for transferring an already assigned stable ID into the target code, not alternative ID databases. The existing TIL/LI assignment remains authoritative in all three designs.

A translation unit is one C or C++ source file together with the headers processed for that compilation. `__LINE__` cannot distinguish two calls on the same physical line. `__COUNTER__` can distinguish expansions, but its absolute value also depends on unrelated macros and headers in that translation unit. Binding a stable ID directly to that absolute value would make unrelated source edits affect the mapping.

Approach | How it distinguishes sites | Benefit | Cost and limitation | Current status
--- | --- | --- | --- | ---
Local counter rebase | Record a counter base immediately before a small source region and select IDs by the difference from that base. | Compile the original source; no target-compiler invocation or compilation database is required by `bind`. Earlier unrelated counter use does not affect the region. | Generated begin/end includes are necessary. Counter use inside the region must match exactly; only safely bounded source constructs are accepted. | Implemented for the ordinary Bind constructs described in this chapter.
Exact target-preprocessor pass | Observe expansions using the actual target compiler and the build's options. | Can observe macros and active branches in a particular build configuration. | Requires the real compiler, defines, include paths, forced includes and any precompiled headers. Bind and the later build must agree; multiple configurations may require distinct mappings. Preprocessed text alone does not portably recover every wrapper's definition identity. | Considered as an alternative; not a normal Bind build step. The separate CE PoC investigates a related approach without providing production CE wrapper support.
Generated compiler input | Give sites explicit ordinals in generated copies of sources and headers. | Site selection need not depend on a compiler counter. | The compiler processes generated copies. Build integration must preserve relative includes, dependencies, diagnostics, debugging paths and IDE navigation. | Considered as an alternative; no supported shadow-source mode is provided.

For example, a local base of 87 followed by three Trice counter expansions at 88, 89 and 90 gives local ordinals 0, 1 and 2. If an earlier header consumes another ten counter values, the base and those three values all increase by ten; the ordinals remain unchanged. If an unrelated macro consumes a counter value *inside* the region, that property no longer holds. Generated range and final-count checks turn the discrepancy into a compile error instead of accepting a silently shifted ID.

The implemented rebase selects stable IDs with generated constant expressions. An early design sketch used an ID array, but that sketch is not the current target interface and does not introduce a runtime lookup table. The user maintains neither counters nor ordinals. A wrapper's inner definition sites retain their IDs across invocations; runtime `if` or `switch` decisions do not change the preprocessor expansion order.

Compiler capability is checked in the generated region that needs it. Finding a host compiler on the developer's machine would not prove what an embedded target compiler supports, so `bind` does not use host-compiler discovery as an ID-binding guarantee. Ordinary file-and-line sites require no counter. For affected sites on a compiler without the necessary capability, use separate source lines, a suitable ordinary function, or the explicitly selected `insert/clean` workflow. See [Bind Limits](#bind-limits).

Checking whether a compiler defines `__COUNTER__` and checking whether a region consumes the expected sequence are different tasks; the generated code addresses both. A successful historical PoC run is evidence for that experiment's compiler and language modes, not a promise for every compiler, precompiled-header setup or build configuration. The [CE wrapper/rebase appendix](context_enrichment.md#extended-poc-for-wrapper-macros-and-counter-rebasing) describes its separate evidence and remaining integration limits.

## 26. <a id="appendix-bind-and-insert-test-evidence"></a>Appendix: Bind and Insert Test Evidence

Bind and Insert tests share canonical sources, ID configuration and build/test workers. The workflow wrapper prepares the source state and include paths; the shared worker performs the actual compiler or decoder checks. This avoids maintaining independent copies of `triceCheck.c` or weakening one workflow's expected output. The relevant states are ID-free without active Bind artifacts, Inserted with explicit IDs, and Bound with owner includes and generated headers.

Check or component | What it establishes | Repository entry point
--- | --- | ---
Shared ID settings and transitions | The repository helpers use the same source scope, aliases, TIL, LI, ID policy and generated directory. | [_120_setup_trice_environment.sh](../../scripts/_120_setup_trice_environment.sh), [_130_trice_id_workflow.sh](../../scripts/_130_trice_id_workflow.sh)
Managed state restoration | Snapshot affected source/metadata bytes and generated artifacts; restore the initial state after success, failure, `SIGINT` or `SIGTERM`. Failed restoration makes the wrapper fail. | [_140_trice_test_state.sh](../../scripts/_140_trice_test_state.sh), [portability tests](../../scripts/portability_test.go)
Generator behavior | File ownership, stable IDs, stamps, include placement, idempotence, ordered diagnostics and rollback on rejected input or write failure. | [bindIDs_test.go](../../internal/id/bindIDs_test.go), [bindMVP2_test.go](../../internal/id/bindMVP2_test.go)
Generated target behavior | Compile real generated headers as C/C++; check counter guards, expansion invariants, emitted IDs and canonical Trice macro coverage. | [bindIntegration_test.go](../../internal/id/bindIntegration_test.go), [_500_test_bind.sh](../../scripts/_500_test_bind.sh)
Return from Bound to Inserted | Remove only validated owner and rebase artifacts, correct LI positions and reject ambiguous or modified artifacts without partial changes. | [bindRemigrate_test.go](../../internal/id/bindRemigrate_test.go), [_250_legacy_remigrate_bind_to_clean.sh](../../scripts/_250_legacy_remigrate_bind_to_clean.sh)
Shared PC target matrix | Run the same selected configurations and expectations in Insert and Bind state, with separate logs and restored inputs. | [_160_pc_target_test_worker.sh](../../scripts/_160_pc_target_test_worker.sh), [_630_test_pc_targets_insert.sh](../../scripts/_630_test_pc_targets_insert.sh), [_640_test_pc_targets_bind.sh](../../scripts/_640_test_pc_targets_bind.sh)

The compiler-build matrices also use shared workers for Insert and Bind. `TRICE_OFF` is checked separately because disabling logging does not depend on ID binding. See [Testing the Trice Library C-Code for the Target](target_tests.md) for current selections, required tools, parallelism, logs and failure handling. Standalone build/ID maintenance helpers can intentionally leave a new source state; the restoration contract belongs to the managed test wrappers. An uncatchable process kill or machine failure cannot execute a shell restoration trap, so retained recovery data must be reviewed in that case.



To check current Bind generation and generated C/C++ target code from the repository root:

```sh
./scripts/_500_test_bind.sh
```

To run both PC target workflows with the same quick selection:

```sh
./scripts/_170_pc_target_tests_all_workflows.sh quick
```

These focused checks do not replace the repository's final full regression run. Check each run's reported tool availability: a missing compiler causes a skip, not a successful compiler check.

---



<p align="right">(<a href="#trice-bind">back to top</a>)</p>

