# Target Routing

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. ID Routing](#id-routing)
* [2. Possibility to create new tags without modifying trice tool source](#possibility-to-create-new-tags-without-modifying-trice-tool-source)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

Target routing selects output channels on the firmware side. Host switches `-logLevel`, `-pick` and `-ban` only select received messages for display; see [host filters](tags_color_and_weights.md).

## 1. <a id="id-routing"></a>ID Routing

With `trice bind`, `-IDRange tag:min,max` assigns a range to a complete [tag group](tags_color_and_weights.md). The same option exists for legacy Insert. Register a free tag with `-ulabel` first; option order does not matter. Groups without a specific range use `-IDMin` through `-IDMax`.

Tag-specific ranges include both endpoints and must satisfy `1 <= min <= max <= 16383`. They must not overlap the general range or any other tag range. A shared endpoint is an overlap; adjacent disjoint ranges and a range containing one ID are valid. Two aliases of the same group cannot define two separate ranges. Missing parts, malformed numbers, unknown tags, reversed bounds, and out-of-range values are rejected before source, dictionary, or bind-artifact changes. All supplied rules are validated together.

```sh
trice bind -src application -IDMin 1000 -IDMax 1999 -IDRange err:10,99
```

The current policy also applies to IDs recovered from sources, location data, or the dictionary. An active Error site with ID `250` is reassigned to a usable ID in `10..99` on the next run. Its old mapping remains in `til.json` so recordings from older firmware can still be decoded. A subsequent unchanged run retains the corrected ID. Only the selected source scope is processed. Bind updates its generated descriptors; an insert-owned source requiring correction must first be processed by `trice insert`. Rebuild affected firmware after reassignment.

With `-v`, each run reports historical dictionary entries outside the current policy in one additional warning: their total count, the smallest affected ID as one example, and its expected range. The historical `250` remains part of that count after reassignment; the new compliant ID does not. No warning is emitted without `-v` or when every entry complies. The warning neither changes the dictionary nor turns a successful command into a failure. A historical entry alone does not prove that an ID is still active.

Target routing is configured separately in the project-specific `triceConfig.h`. For the enabled deferred UARTA, UARTB, Auxiliary8, Auxiliary32, and SEGGER RTT 8-bit outputs, the corresponding `*_MIN_ID` and `*_MAX_ID` select inclusive ranges. Missing bounds default to zero. `0/0` disables ID selection for that output, so its ordinary output path accepts all IDs; it does not disable the output itself. Exactly one nonzero bound, reversed bounds, or bounds outside `1..16383` cause a compile-time error naming the defines.

Active deferred ID selection requires `TRICE_DEFERRED_TRANSFER_MODE` to be `TRICE_SINGLE_PACK_MODE`, with both ring and double buffers. Multi-pack is allowed when no ID range is active. This requirement concerns deferred ID routing, not TCOBS framing in general or custom/direct routing. See [triceDefaultConfig.h](../../src/triceDefaultConfig.h) for the output-specific define names.

## 2. <a id="possibility-to-create-new-tags-without-modifying-trice-tool-source"></a>Possibility to create new tags without modifying trice tool source

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
trice bind -src main.c -genDir build/sidecars -IDMin 1004 -IDMax 6999 -IDRange wife:16000,16009 -IDRange man:1000,1003 -ulabel man -ulabel wife
```

Bind adds its managed include to `main.c`; the calls stay ID-free. Add `build/sidecars` to the compiler include path. Example assignments are `msg → 5778`, `man → 1002`, `wife → 16004`, `any → 2184`; the actual random choices may differ. These assignments alone do not enable a target output filter: configure the output's ID bounds separately as described above.

