# Trice ID management

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Trice inside source code](#trice-inside-source-code)
  * [1.1. Trice in source code comments](#trice-in-source-code-comments)
  * [1.2. Trice parser exclusion markers](#trice-parser-exclusion-markers)
  * [1.3. Different IDs for same Trices](#different-ids-for-same-trices)
  * [1.4. Same IDs for different Trices](#same-ids-for-different-trices)
  * [1.5. ID Routing](#id-routing)
  * [1.6. Possibility to create new tags without modifying trice tool source](#possibility-to-create-new-tags-without-modifying-trice-tool-source)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## 1. <a id="trice-inside-source-code"></a>Trice inside source code

### 1.1. <a id="trice-in-source-code-comments"></a>Trice in source code comments

* Trice macros commented out, are **visible** for the `trice insert` command and therefore regarded.
  * Example: `// trice("Hi!\n");` is still regarded by the `trice i`.
* During `trice insert` commented out Trice macros, are treated in the same way as active Trice macros. Even after deletion their content stays inside til.json. This is intensionally to get best stability across several firmware versions or variants.
* The trice tool does treat trice statements inside comments or excluded by compiler switches also.

### 1.2. <a id="trice-parser-exclusion-markers"></a>Trice parser exclusion markers

Use `TRICE_INSERT_OFF` and `TRICE_INSERT_ON` markers to exclude a source section from `trice insert`, `trice clean`, `trice add` and ID refresh parsing.

```C
// TRICE_INSERT_OFF
TRice("This text is ignored by the trice tool.");
// TRICE_INSERT_ON
```

The markers are case-sensitive, must be written as comments, and affect only the Trice tool parser. `TRICE_INSERT_OFF` without a following `TRICE_INSERT_ON` disables Trice parsing until the end of the file. The marker state is local to the scanned file and does not affect source files including that file. This is useful for Trice target sources or other source sections containing Trice-like comments or helper macros that should not create or change IDs.

### 1.3. <a id="different-ids-for-same-trices"></a>Different IDs for same Trices

Every active textual Trice site needs its own ID so that location information remains unambiguous. Copying a call with its explicit ID does not create shared ownership: `trice insert` resolves the duplicate and allocates or reuses another eligible ID.

Both `trice insert` and `trice bind` then order **identical, interchangeable Trices** by normalized relative file path, numeric line number, and position from left to right within the line. Paths use `/` separators and an ordinal, case-sensitive comparison (`A.c` precedes `a.c`), independent of locale. The root is `-liRoot`, or the directory containing the selected LI file by default; the absolute checkout directory does not determine the order. Overlapping `-src` roots visit each physical path only once.

Within each group, the IDs already selected by normal allocation are sorted numerically ascending and assigned to the sites in that order. Identical means the same normalized transport type and exact canonical format, including any structured field schema and Context Enrichment. Equal visible messages with different types or field names are separate groups. Existing tag-specific ID ranges still apply. New ID selection through `-IDMethod random`, `upward`, or `downward` is unchanged: this rule orders the selected pool; it does not introduce a new global allocation strategy or renumber unrelated Trices.

For example, assume these three IDs belong to one group:

| Source state | Site in path order | Assigned ID |
| --- | --- | ---: |
| Before | `b.c:10` | 100 |
| Before | `d.c:20` | 200 |
| After adding `a.c:5`, with ID 300 also selected | `a.c:5` | 100 |
| After | `b.c:10` | 200 |
| After | `d.c:20` | 300 |

Both existing sites may change IDs when an identical site is added, removed, or moved. This is intentional. An unchanged source selection with the same selected ID pool produces the same mapping on subsequent runs, regardless of scan-root order or worker completion. The same rule applies after Clean/Insert; Clean itself does not allocate IDs. An explicit ID in an Insert source is not a request to exempt that site from ordering. Bind changes its own generated descriptors and leaves Insert-owned source files and their IDs alone.

A partial scan orders only its selected sites. IDs recorded in the primary LI as belonging to files outside that selection, including excluded files, are reserved and cannot be taken by the selected sites. Keep the shared LI available for partial scans: without location data, ownership of unscanned IDs cannot be inferred from TIL alone. For one order across the whole project, scan the complete intended source tree. A rename can leave the old path recorded in LI; its ID stays reserved while another eligible ID is selected for the new path.

The final assignment is written consistently to Insert sources or Bind sidecars and to LI. Historical TIL mappings remain available for older recordings. Rebuild firmware after an assignment changes and retain the matching metadata for recordings whose exact old source positions matter.

The [behavioral tests](../../internal/id/orderedIDs_test.go) cover group changes, partial scans, path and column order, cache reuse, and ownership. The [target/decoder integration](../../internal/args/ordered_ids_test.go) checks actual C/C++ records from both workflows against the generated mapping and decoded fields.

### 1.4. <a id="same-ids-for-different-trices"></a>Same IDs for different Trices

* If duplicate ID's with different format strings found inside the source tree (case several developers or source code merging) one ID is replaced by a new ID. The probability for such case is low, because of the default random ID generation.
* Also you can simply copy a Trice statement and modify it without dealing with the ID.
* The Trice tool will detect the 2nd (or 3rd) usage of this ID and assign a new one, also extending the ID list.
* That is done silently for you during the next `trice insert`.
* When you use the [Trice Cache](trice_features_overview.md#trice-cache), the IDs are invisible and all happens in the background automatically.

### 1.5. <a id="id-routing"></a>ID Routing

With `trice insert` or `trice bind`, `-IDRange tag:min,max` assigns a range to a complete [tag group](trice_tags_color_and_weights.md). Register a free tag with `-ulabel` first; option order does not matter. Groups without a specific range use `-IDMin` through `-IDMax`.

Tag-specific ranges include both endpoints and must satisfy `1 <= min <= max <= 16383`. They must not overlap the general range or any other tag range. A shared endpoint is an overlap; adjacent disjoint ranges and a range containing one ID are valid. Two aliases of the same group cannot define two separate ranges. Missing parts, malformed numbers, unknown tags, reversed bounds, and out-of-range values are rejected before source, dictionary, or bind-artifact changes. All supplied rules are validated together.

```sh
trice insert -src src -IDMin 1000 -IDMax 1999 -IDRange err:10,99
```

The current policy also applies to IDs recovered from sources, location data, or the dictionary. An active Error site with ID `250` is reassigned to a usable ID in `10..99` on the next run. Its old mapping remains in `til.json` so recordings from older firmware can still be decoded. A subsequent unchanged run retains the corrected ID. Only the selected source scope is processed. Bind updates its generated descriptors; an insert-owned source requiring correction must first be processed by `trice insert`. Rebuild affected firmware after reassignment.

With `-v`, each run reports historical dictionary entries outside the current policy in one additional warning: their total count, the smallest affected ID as one example, and its expected range. The historical `250` remains part of that count after reassignment; the new compliant ID does not. No warning is emitted without `-v` or when every entry complies. The warning neither changes the dictionary nor turns a successful command into a failure. A historical entry alone does not prove that an ID is still active.

Target routing is configured separately in the project-specific `triceConfig.h`. For the enabled deferred UARTA, UARTB, Auxiliary8, Auxiliary32, and SEGGER RTT 8-bit outputs, the corresponding `*_MIN_ID` and `*_MAX_ID` select inclusive ranges. Missing bounds default to zero. `0/0` disables ID selection for that output, so its ordinary output path accepts all IDs; it does not disable the output itself. Exactly one nonzero bound, reversed bounds, or bounds outside `1..16383` cause a compile-time error naming the defines.

Active deferred ID selection requires `TRICE_DEFERRED_TRANSFER_MODE` to be `TRICE_SINGLE_PACK_MODE`, with both ring and double buffers. Multi-pack is allowed when no ID range is active. This requirement concerns deferred ID routing, not TCOBS framing in general or custom/direct routing. See [triceDefaultConfig.h](../../src/triceDefaultConfig.h) for the output-specific define names.

### 1.6. <a id="possibility-to-create-new-tags-without-modifying-trice-tool-source"></a>Possibility to create new tags without modifying trice tool source

According to the demand in [541](https://github.com/rokath/trice/issues/541) a CLI switch `-ulabel` exists now.

Use `-ulabel` for additional user labels. Try this example in an empty folder:

* File *main.c*:

```C
#include "trice.h"

int main(void){
    trice("msg:hi\n");
    trice("man:hi\n");
    trice("wife:hi\n");
    trice("any:hi\n");
}
```

* Bash:

```bash
touch til.json li.json
trice i -IDMin 1004 -IDMax 6999 -IDRange wife:16000,16009 -IDRange man:1000,1003 -ulabel man -ulabel wife
```

* File *main.c*:

```C
#include "trice.h"

int main(void){
    trice(iD(5778), "msg:hi\n");
    trice(iD(1002), "man:hi\n");
    trice(iD(16004), "wife:hi\n");
    trice(iD(2184), "any:hi\n");
}
```

<p align="right">(<a href="#trice-id-management">back to top</a>)</p>

