# Trice IDs and Dictionaries

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. ID number selection](#id-number-selection)
  * [1.1. Allocation internals](#allocation-internals)
* [2. ID number usage and stability](#id-number-usage-and-stability)
* [3. Trice ID 0](#trice-id-0)
* [4. Different IDs for same Trices](#different-ids-for-same-trices)
* [5. Files, ownership and history](#files-ownership-and-history)
* [6. Trice location information file li.json](#trice-location-information-file-lijson)
* [7. Exporting decoder metadata](#exporting-decoder-metadata)
* [8. Recovering explicit source IDs](#recovering-explicit-source-ids)
* [9. Compatibility with firmware and host-tool versions](#compatibility-with-firmware-and-host-tool-versions)
* [10. Migrating 1.x scripts and projects](#migrating-1x-scripts-and-projects)
* [11. Long Time Availability](#long-time-availability)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

Bind maintains the mapping between source calls, generated compiler descriptors and the host dictionary. The [UM file checklist](../TriceUserManual.md#know-which-files-to-keep) gives the short everyday rule; this chapter defines ownership, history and compatibility.

## 1. <a id="id-number-selection"></a>ID number selection

TREX records have 14 ID bits. IDs `1..16383` identify messages; zero is a source placeholder, not an assigned message ID. Bind places the assignment in a sidecar, while the source can remain `trice("Hi!\n");`.

Select new IDs with `-IDMethod random` (default), `upward` or `downward`. For example:

```sh
trice bind -src application -IDMin 1000 -IDMax 1999 -IDMethod upward
```

The allocator excludes IDs already used in TIL. Tag-specific ranges and their target-channel use are described in [Target Routing](target_routing.md). Changing the allowed range can change an active ID; historical dictionary entries are retained.

### 1.1. <a id="allocation-internals"></a>Allocation internals

The random allocator builds the eligible ID pool, removes used entries, selects a random remaining index and removes that entry from the pool. Ordering identical sites is a separate operation below.

## 2. <a id="id-number-usage-and-stability"></a>ID number usage and stability

The format and transport type, including parameter width, identify a message. Different textual sites need separate IDs for unambiguous locations. Changing the parameter width or format can require a new ID; selecting a target stamp does not by itself change the message's parameter type. A deleted source call does not delete its old TIL entry.

Existing mappings are reused when eligible. They are not an unconditional promise that a site keeps its number: range changes, ownership and ordering of identical calls can reassign IDs. Do not use hexadecimal inline IDs to bypass parsing as a stability mechanism.

## 3. <a id="trice-id-0"></a>Trice ID 0

Bind supports ID-free calls and explicit zero placeholders. Lowercase/mixed-case names select no, 16-bit or 32-bit stamps; uppercase calls can use `id(0)`, `Id(0)` or `ID(0)` to select the stamp explicitly. See [ID and stamp forms](bind.md#id-and-stamp-forms).

## 4. <a id="different-ids-for-same-trices"></a>Different IDs for same Trices

Every active textual Trice site needs its own ID so that location information remains unambiguous. Bind assigns IDs per source site. In the deprecated Insert workflow, copying a call with its explicit ID does not create shared ownership: Insert resolves the duplicate and allocates or reuses another eligible ID.

Both `trice insert` and `trice bind` then order **identical, interchangeable Trices** by normalized relative file path, numeric line number, and position from left to right within the line. Paths use `/` separators and an ordinal, case-sensitive comparison (`A.c` precedes `a.c`), independent of locale. The root is `-liRoot`, or the directory containing the selected LI file by default; the absolute checkout directory does not determine the order. Overlapping `-src` roots visit each physical path only once.

Within each group, the IDs already selected by normal allocation are sorted numerically ascending and assigned to the sites in that order. Identical means the same normalized transport type and exact canonical format, including any structured field schema and Context Enrichment. Equal visible messages with different types or field names are separate groups. Existing tag-specific ID ranges still apply. New ID selection through `-IDMethod random`, `upward`, or `downward` is unchanged: this rule orders the selected pool; it does not introduce a new global allocation strategy or renumber unrelated Trices.

For example, assume these three IDs belong to one group:

Source state | Site in path order | Assigned ID
--- | --- | ---:
Before | `b.c:10` | 100
Before | `d.c:20` | 200
After adding `a.c:5`, with ID 300 also selected | `a.c:5` | 100
After | `b.c:10` | 200
After | `d.c:20` | 300

Both existing sites may change IDs when an identical site is added, removed, or moved. This is intentional. An unchanged source selection with the same selected ID pool produces the same mapping on subsequent runs, regardless of scan-root order or worker completion. The same rule applies after Clean/Insert; Clean itself does not allocate IDs. An explicit ID in an Insert source is not a request to exempt that site from ordering. Bind changes its own generated descriptors and leaves Insert-owned source files and their IDs alone.

A partial scan orders only its selected sites. IDs recorded in the primary LI as belonging to files outside that selection, including excluded files, are reserved and cannot be taken by the selected sites. Keep the shared LI available for partial scans: without location data, ownership of unscanned IDs cannot be inferred from TIL alone. For one order across the whole project, scan the complete intended source tree. A rename can leave the old path recorded in LI; its ID stays reserved while another eligible ID is selected for the new path.

The final assignment is written consistently to Insert sources or Bind sidecars and to LI. Historical TIL mappings remain available for older recordings. Rebuild firmware after an assignment changes and retain the matching metadata for recordings whose exact old source positions matter.

The [behavioral tests](../../internal/id/orderedIDs_test.go) cover group changes, partial scans, path and column order, cache reuse, and ownership. The [target/decoder integration](../../internal/args/ordered_ids_test.go) checks actual C/C++ records from both workflows against the generated mapping and decoded fields.

## 5. <a id="files-ownership-and-history"></a>Files, ownership and history

| File | During development | For firmware and recordings |
| --- | --- | --- |
| `til.json` | Keep under version control; extend it without discarding old entries. Bind requires an existing file. | A newer cumulative TIL can retain old mappings; template and host compatibility still matters. |
| `li.json` | Keep locally between ID runs, including partial scans. It records site ownership as well as locations. | Keep the build's LI when exact source locations matter. |
| Bind sidecars | Generated build inputs; normally not versioned. Bind recreates them from the selected sources and metadata. | Not a substitute for the long-lived TIL or firmware metadata. Use the [artifact report](generate.md#bind-artifact-report) before cleaning unfamiliar generated trees. |

For a genuinely new project, `touch til.json li.json` creates initially empty files without replacing existing data. Missing dictionaries are rejected to avoid silently assigning a new history. Do not delete TIL to remove unused entries: old firmware and captures may still need them. Use separate dictionaries for independent projects or deliberately share one accumulated dictionary with consistent ownership and ID policy.

## 6. <a id="trice-location-information-file-lijson"></a>Trice location information file li.json

* If the generated `li.json` is available, the Trice tool automatically displays file name and line number. But that is accurate only with the exact matching firmware version. That usually is the case right after compiling and of most interest at the developers table.
* The Trice tool will silently not display location information, if the `li.json` file is not found. For in-field logging, the option `-showID "inf:%5d"` could be used. This allows later an easy location of the relevant source code.
* Another option is to record the binary trice messages (`trice log -p com1 -blf aFileName`) and to play them later with the Trice tool using the correct `li.json` (`trice log -p FILEBUFFER -args aFileName` ).
* Each entry contains only `File` and `Line`. `File` retains the normalized source path relative to `-liRoot`; the default root is the directory containing `li.json`.
* Use an explicit project root when `li.json` lives in a build directory but stable project-relative paths are wanted: `trice bind -src app -genDir build/sidecars -i til.json -li build/demoLI.json -liRoot .`.
* `File` uses `/` separators on every platform. Required leading `..` components are preserved. If Windows cannot make a source path relative across volumes, the normalized absolute path is stored.
* `trice log` and `tlog` show only the filename by default. `-liMaxDirs=N` adds at most `N` immediately preceding parent directories. Leading `..` components are never displayed or counted.
* The v1.3.0 option `-liPath` is replaced by `-liRoot` for storage and `-liMaxDirs` for display. Both v1.3.0 and the current location files use `File` and `Line`.
* The Trice repository uses `-liRoot .` while generating the shared `demoLI.json`, so all stored paths remain relative to the repository root across supported platforms.
* Active demo and example LI files, including `demoLI.json`, are generated locally rather than versioned. Build and ID-workflow scripts initialize a missing file and retain existing LI evidence. The historical `_test/testdata/testLI.json` fixture remains versioned.
* Keep local LI between ID-processing runs even if source locations are unwanted in logs. `trice log -li off` suppresses their loading and display while Bind and Insert can continue using LI for ID ownership and reuse. See [Location Information](cli.md#location-information).

## 7. <a id="exporting-decoder-metadata"></a>Exporting decoder metadata

Run Bind, then `trice generate -src application -logC` to export currently referenced formats for a local or custom decoder. See [Generate](generate.md) for output paths and options. This is also useful where a Go host executable is unavailable, for example [QNX environments](https://github.com/rokath/trice/discussions/263#discussioncomment-4180692).

## 8. <a id="recovering-explicit-source-ids"></a>Recovering explicit source IDs

[Add versus Insert](insert_clean_cache.md#importing-existing-ids-with-add) explains how to collect IDs already present in legacy sources without modifying those files. Neither command can recreate history absent from both sources and dictionaries.

## 9. <a id="compatibility-with-firmware-and-host-tool-versions"></a>Compatibility with firmware and host-tool versions

Keep each released firmware together with its `til.json`, matching `li.json` if needed, host-tool version, and decoding options such as framing, byte order, and default value width. Retaining an ID preserves its dictionary entry; it does not make every dictionary compatible with every host version. Location information must describe the firmware actually running, even when one cumulative TIL covers multiple firmware versions.

Firmware and dictionary | Host tool | Supported use
--- | --- | ---
v1.3.0 firmware with its unchanged dictionaries | v1.3.0 | Reproduce historical output with the corresponding original options.
Existing firmware with classic printf formats and retained IDs | Current host | Supported when the formats are valid under the current template syntax and transport settings match. Literal braces are a relevant exception; see the examples below.
Current firmware and current dictionaries, including structured fields or CE | Current host | Supported within the documented record families and CE limits. Current instrumentation and its matching target sources are required when building these features.
A dictionary containing current named fields or doubled literal braces | v1.3.0 | Not a supported interpretation of the new templates. The old host does not understand fields and prints doubled braces literally.
C headers, implementation files, or generated Bind sidecars mixed from different releases | Any host | No general source/build compatibility guarantee. Use target files from one release and regenerate sidecars with the corresponding host tool.

The following examples use the same unstamped TREX record layout and ID in both host versions. `Strg` is the stored dictionary string; the scalar case carries the value `7`.

Stored `Strg` | Record payload | v1.3.0 interpretation | Current interpretation
--- | --- | --- | ---
`msg:count=%d` | One 32-bit value | `count=7` | `count=7`
`hi` | No values | `hi` | `hi`; JSON/KV classify it as `untagged` without inserting a tag into `message`.
`msg:literal={x}` | No values | `literal={x}` | `{x}` requires a value; the record is rejected with a diagnostic.
`msg:set={1,2}` | No values | `set={1,2}` | Invalid field name; the record is rejected with a diagnostic.
`msg:literal={{x}}` | No values | `literal={{x}}` | `literal={x}`
`msg:value={x}` | One 32-bit value | Not a supported one-value printf format | `value=7`, with field `x=7` in structured output.

For unchanged historical firmware that used literal braces, replay with its archived dictionary and matching old host tool. In sources built with the current tools, write literal braces as `{{` and `}}`, then regenerate IDs/dictionaries and rebuild. Logging does not rewrite supplied dictionaries, and historical dictionaries are not automatically converted. An unchanged binary record layout alone does not establish template compatibility. Replay checks must inspect output and diagnostics: a rejected record does not necessarily make the logger exit with a nonzero status.

## 10. <a id="migrating-1x-scripts-and-projects"></a>Migrating 1.x scripts and projects

Review scripts written for 1.x before using the v2 tools. These changes are relative to v1.3.0 and apply to both `trice log` and `tlog` where relevant:

Previous usage | Required adjustment
--- | ---
`trice generate -tilC` | Use `trice generate -src Core -logC`, selecting your own sources. Run Insert or Bind first. Only currently referenced IDs are emitted, not the whole historical TIL. The default output is now `generated/til.c`; use `-logC=build/til.c` to choose a path.
`trice generate -abc deviceX` followed by compiling `deviceX.c` | A name without a directory now produces `generated/deviceX.h` and `.c`. Compile the file from its new location, select another directory with `-genDir`, or specify the target path, for example `-abc=custom/deviceX`. ABC itself was already included in v1.3.0.
ID management with `-liPath` | Use `-liRoot <project-root>` to choose the directory that stored paths are relative to. The default is the directory containing `li.json`. Its `File` field now contains a source path, for example `Core/Src/main.c`, rather than only `main.c`; adjust scripts that read it.
Logging with `-liPath` | Remove it. Use `-liMaxDirs N` to show up to N parent directories; the default `0` shows only the filename.
`-pick err:wrn` or `-ban dbg:trace` | Repeat the switch: `-pick err -pick wrn` or `-ban dbg -ban trace`. Colon lists are rejected; `-pick` and `-ban` cannot be combined. `-pick 600` selects only weight 600; use `-logLevel 600` for weight 600 and above.
`-ulabel alpha:beta` to register two tags | Use `-ulabel alpha -ulabel beta`. A colon now assigns one tag's weight or color, for example `-ulabel alpha:650`. User-defined tags match exact spelling.
Built-in one-letter tags or selectors, or `OK` as a Message alias | Use descriptive names such as `err`, `wrn`, `inf`, `dbg` and `msg`, including in `-IDRange`. Built-in names ignore case. The old aliases are removed; unregistered prefixes count as `untagged`, and unknown selectors are errors. `rx` now selects Receive and `tx` selects Transmit, without the former overlap with Read/Write.
`-logLevel` based on the old tag-table order | Recheck which messages pass: selection now uses a minimum weight. `-logLevel wrn` includes Warning and messages with higher priority; `-loglevel 600` is equivalent with default weights. Messages without a recognized tag are also filtered. Tags used for timestamps or other output columns do not decide whether a message is shown.

Further changes can affect builds and automated checks:

- **Literal braces and replay:** use `{{` and `}}` in new source formats. For old firmware and dictionaries, follow the [compatibility rules](id_management.md#compatibility-with-firmware-and-host-tool-versions) above. Replay can reject a record and still exit successfully; scripts must also check diagnostics and expected records.
- **ID ranges and identical calls:** Insert/Bind can change existing IDs when their allowed range changes or identical calls are added, removed or moved. Rebuild the firmware and keep its matching `li.json`; see [ID assignment and ranges](id_management.md#different-ids-for-same-trices).
- **Location information:** keep `li.json` between ID-management runs, including partial scans. To hide locations during logging, use `trice log -li off`; passing `-li off` to Insert/Bind also disables source ownership information. See [location information](cli.md#location-information).
- **RTT builds:** put project and generated header directories first, then `src`, then `src/default_conf` on the include path. Update the three SEGGER source/header files together and retain the license and your project-specific configuration; see [RTT integration](rtt.md).
- **Custom encoding calls:** if `TriceEncode()` uses a framing format not selected by an active output, enable `TRICE_COBS_ENCODE_SUPPORT` or `TRICE_TCOBS_ENCODE_SUPPORT` and link that encoder. Unavailable framing returns `0`; see [encoder selection](memory_usage.md).
- **Output parsing:** statistics include messages hidden by host filters. For JSON/KV, read application records from standard output and diagnostics from standard error; see [Structured Logging](structured_logging.md) and [event statistics](tags_color_and_weights.md#event-statistics).
- **Repository automation:** helper/test scripts use numbered filenames, for example `scripts/_530_test_links.sh` replaces `_testAll_06_LinkCheck.sh`. Update hard-coded helper and documentation paths. See [Running the Tests](target_tests.md) for the current default, selections and failure policy.
- **Installation:** source builds require Go 1.25 or later. Install v2 from its release archive or build a checkout of its tag; remote `go install` with a v2 version tag is not supported because the module path has no `/v2` suffix. Betas do not update the stable Homebrew tap; see [beta installation](tool_build_release.md#install-a-beta-or-a-stable-release).

## 11. <a id="long-time-availability"></a>Long Time Availability

* You could place a download link for the Trice tool and the used **til.json** list.
* Optionally add the (compressed/encrypted) ID reference list as resource into the target FLASH memory to be sure not to loose it in the next 200 years.

