# Trice User Manual Extension

<!--
Integration note for the maintainer:
Place this file temporarily at ./docs/TriceUserManualExtension.md so the rewritten root README links work.
Later, merge useful parts into ./docs/TriceUserManual.md and adjust the README links to the final anchors.

This file contains:
1. Quickstarts that should not overload the root README.
2. Condensed legacy README material that is useful but too broad for first-time readers.
3. Maintainer comments showing where similar content already exists in TriceUserManual.md and where moved README content probably belongs.
-->

## Suggested merge map

<!--
Manual placement hint:
This section is not meant for end-user documentation. It can be deleted after migration.
-->

| This extension section | Suggested final location in `TriceUserManual.md` |
| --- | --- |
| [Quickstart: Existing non-blocking byte writer](#quickstart-existing-byte-writer) | Under `Start with Trice`, before or beside RTT quickstart material. |
| [Quickstart: SEGGER RTT direct mode](#quickstart-segger-rtt-direct) | Under `Trice over RTT`; keep root README short and link here. |
| [Quickstart: UART/VCOM deferred mode](#quickstart-uart-vcom-deferred) | Under `Start with Trice` or `Communication Ports`. |
| [Mode and transport selection](#mode-and-transport-selection) | Early in `Start with Trice`, because it removes first-use ambiguity. |
| [ID workflow notes](#id-workflow-notes) | Near `Trice ID management` and `The Trice Insert Algorithm`. |
| [Advanced features from old README](#advanced-features-from-old-readme) | Split into existing feature chapters instead of keeping in README. |
| [Related projects and comparison notes](#related-projects-and-comparison-notes) | Keep near the end of the manual or create a dedicated comparison page. |

<a id="mode-and-transport-selection"></a>

## Mode and transport selection

<!--
Existing manual coverage:
Related information is currently distributed across `Start with Trice`, `Communication Ports`, `Trice Speed`, `Trice over RTT`, and configuration default comments in `src/triceDefaultConfig.h`.
This section is intended as an early decision guide.
-->

### Direct mode

Direct mode writes each Trice record immediately to the selected output backend.

Good first use:

- SEGGER RTT with J-Link.
- Fast direct auxiliary output where the writer is guaranteed not to block.
- Lab setups where immediate output matters more than keeping the log call as short as possible.

Risk:

- A slow physical output path, especially a blocking UART, can dominate the Trice call time.

### Deferred mode

Deferred mode makes the Trice call write into a target buffer. A later `TriceTransfer()` sends buffered data to the output backend.

Good first use:

- UART and USB CDC/VCOM.
- Existing byte writer functions.
- Production-style logging.
- Shortest target-side Trice call path.

Risk:

- You must call `TriceTransfer()` often enough.
- Buffer sizing and overflow behavior need attention under burst load.

### Buffer choice

| Buffer | Typical use | Trade-off |
| --- | --- | --- |
| `TRICE_STACK_BUFFER` | Simple direct-output setups | no persistent deferred storage; stack use matters |
| `TRICE_STATIC_BUFFER` | Direct output without stack allocation | one static single-message buffer |
| `TRICE_RING_BUFFER` | Balanced deferred output | less RAM, robust general-purpose default |
| `TRICE_DOUBLE_BUFFER` | Minimum target-side Trice execution time | more RAM, best for high-speed deferred extraction |

### Auxiliary hooks

The auxiliary interface is useful when the project already owns the physical transport.

| Switch | Hook | Writer shape | Typical use |
| --- | --- | --- | --- |
| `TRICE_DIRECT_AUXILIARY8` | `UserNonBlockingDirectWrite8AuxiliaryFn` | `const uint8_t*`, byte length | direct byte stream, only if fast/non-blocking |
| `TRICE_DEFERRED_AUXILIARY8` | `UserNonBlockingDeferredWrite8AuxiliaryFn` | `const uint8_t*`, byte length | most universal first integration |
| `TRICE_DIRECT_AUXILIARY32` | `UserNonBlockingDirectWrite32AuxiliaryFn` | `const uint32_t*`, word count | fast word-oriented direct sink |
| `TRICE_DEFERRED_AUXILIARY32` | `UserNonBlockingDeferredWrite32AuxiliaryFn` | `const uint32_t*`, word count | word-oriented deferred sink; also interesting with encryption |

The deferred 8-bit hook is the safest first quickstart because almost every transport can consume bytes.

<a id="id-workflow-notes"></a>

## ID workflow notes

<!--
Existing manual coverage:
See `TriceUserManual.md#trice-id-management`, `TriceUserManual.md#the-trice-insert-algorithm`, and `TriceUserManual.md#trice-cache-for-compilation-speed`.

README legacy placement:
The root README should acknowledge source modification, but details belong here or in the manual.
-->

Trice works by replacing format strings in the target binary with compact IDs. `trice insert` assigns these IDs and updates the ID list.

Example before insertion:

```c
trice("boot\n");
```

Example after insertion:

```c
trice(iD(12345), "boot\n");
```

The source still contains the readable format string, but the compiled target code uses the ID.

### Files

| File | Purpose | Keep? |
| --- | --- | --- |
| `til.json` | maps IDs to format strings | yes; required for decoding |
| `li.json` | maps IDs to source locations | useful for diagnostics |
| binary log file | raw target output | useful for offline/field decoding |

### Three common workflows

#### Keep IDs in source

Good for small projects and maximum robustness.

```bash
trice insert -src ./ -i ./til.json -li ./li.json
# build normally
```

#### Insert before build, clean after build

Good when the repository should stay visually free of IDs.

```bash
trice insert -src ./ -i ./til.json -li ./li.json
# build
trice clean -src ./ -i ./til.json -li ./li.json
```

#### Use the Trice cache

Good when repeated insert/clean would otherwise cause unnecessary recompilation.

```bash
mkdir -p ~/.trice/cache
trice insert -cache -src ./ -i ./til.json -li ./li.json
# build
trice clean -cache -src ./ -i ./til.json -li ./li.json
```

### Team recommendation

Pick one workflow and document it in the project build instructions. Be especially careful if other tools also rewrite source files, such as formatters or code generators. A safe order is usually:

1. generate code,
2. format code,
3. run `trice insert`,
4. build,
5. optionally run `trice clean`.

<a id="advanced-features-from-old-readme"></a>

## Advanced features from old README

<!--
README legacy placement:
The old root README surfaced many advanced capabilities early. They are still useful, but they should not compete with first-use onboarding.
This section keeps the material reachable and gives pointers for later integration into the main manual.
-->

### Logging and debugging

<!--
Existing manual coverage:
See `TriceUserManual.md#abstract`, `#how-it-works---the-main-idea`, `#start-with-trice`, and the CLI examples.
-->

Trice can be used as a fast `printf`-debugging replacement and as a compact logging system. The target sends short binary records; the host produces the readable log. Keep the matching `til.json` so logs from field devices can be decoded later.

### Data compression and FLASH reduction

<!--
Existing manual coverage:
See `TriceUserManual.md#small-size---using-trice-frees-flash-memory`, `#minimal-transfer-bytes-amount`, and `#trice-memory-needs`.
-->

Replacing format strings with IDs acts like compile-time compression:

- long format strings do not need target FLASH,
- transferred records contain only ID, optional stamp, and argument values,
- UART/USB/network bandwidth is used for binary data rather than text.

A typical Trice message can be only a few bytes plus parameters, independent of the human-readable format-string length.

### Writing logs to files or storage

<!--
Existing manual coverage:
See `TriceUserManual.md#writing-the-trice-logs-into-an-sd-card-or-a-user-specific-output`.
-->

The auxiliary deferred write hooks can write Trice binary data to an SD card, flash partition, host file, or any other byte sink. Store `til.json` and optionally `li.json` together with the firmware version or diagnostic package.

### Encryption

<!--
Existing manual coverage:
See `TriceUserManual.md#optional-xtea-encryption`.
-->

Trice supports optional XTEA encryption for transport packets. This can be useful when field logs should only be readable with the matching key and ID list. Enable encryption in `triceConfig.h` and pass the matching password/key option to `trice log`.

### Language switching / translation

<!--
Existing manual coverage:
The manual has a section anchor `#switch-the-language-without-changing-a-bit-inside-the-target-code`.
Suggested action: keep this advanced feature in the manual, not in the root README.
-->

Because the target sends IDs and values, the host-side `til.json` can map an ID to text in different languages. The target binary does not need to change for host-side wording changes, as long as the ID and parameter format remain compatible.

### Timing analysis

<!--
Existing manual coverage:
See `TriceUserManual.md#trice-timestamps`, `#target-timestamps-formatting`, `#target-timestamp-delta-columns`, and `#cycle-counter`.
-->

Trice supports target timestamps and host timestamps. This helps with distributed timing analysis, interrupt timing, and observing target behavior without stopping the firmware in a debugger.

### Display server

<!--
Existing manual coverage:
See `TriceUserManual.md#logging-over-a-display-server` and `#several-targets-at-the-same-time`.
-->

`trice ds` can act as a display server. Multiple `trice log` instances can send completed log lines to one display server so several targets appear in one combined output.

Example:

```bash
trice ds
trice log -p COM15 -ds
trice log -p COM16 -ds
```

### Trice ABC - Asynchronous Broadcast Commands

<!--
Existing manual coverage:
See `TriceUserManual.md#trice-abc---asynchronous-broadcast-commands`.
README placement recommendation:
Mention ABC only as an advanced capability in README; keep the workflow in the manual.
-->

Trice ABC extends normal Trice records into compact command records. A sender emits a Trice command record; receiver-side code generated from `til.json` dispatches selected IDs to local handlers.

ABC is intentionally not a complete RPC framework. Addressing, authentication, acknowledgements, retries, timeouts, and return values remain application policy.

### Trice cache

<!--
Existing manual coverage:
See `TriceUserManual.md#trice-cache-for-compilation-speed`.
-->

The Trice cache avoids unnecessary rebuilds when using `trice insert` and `trice clean` around the build. It keeps cached inserted and cleaned copies of unchanged files.

Typical use:

```bash
mkdir -p ~/.trice/cache
trice insert -cache -src ./ -i ./til.json -li ./li.json
# build
trice clean -cache -src ./ -i ./til.json -li ./li.json
```

### Debugging with VS Code and Clang

<!--
Existing manual coverage:
See `TriceUserManual.md#development-environment-setup` and example project sections.
README legacy placement:
Screenshots and IDE walkthroughs should not be in the root README first screen.
-->

The example projects show Trice integration with VS Code, Makefiles, Clang/GCC, STM32 projects, RTT, and UART-style output. Use bare/instrumented project pairs to see the minimal integration diff.

### Future ideas from the old README

<!--
README legacy placement:
Keep roadmap material in a roadmap/changelog/discussions area, not in the onboarding README.
-->

Potential future work mentioned in previous README material includes:

- a small `tlog` tool in C or Python,
- more structured logging support,
- additional transport recipes,
- clearer advanced ABC and command-style communication examples.

<a id="legacy-code-integration-notes"></a>

## Legacy code integration notes

<!--
Existing manual coverage:
See `TriceUserManual.md#trice-and-legacy-user-code`, including separate physical output, manual adaptation, print wrapping/framing, and aliases.
-->

Existing projects do not have to migrate all logs at once.

Common strategies:

1. Keep legacy `printf` output and send Trice on a separate physical channel.
2. Replace selected `printf` calls with Trice calls where speed or FLASH matters.
3. Use Trice aliases so project-specific log macros are processed by `trice insert` without renaming every call site.
4. Use a shared output channel only if framing and mixed text/binary behavior are explicitly configured and tested.

For a first integration, prefer a small vertical slice:

- one source file,
- one working output path,
- one `til.json`,
- one build pre-step,
- one host `trice log` command.

Only then expand to aliases and larger source trees.

<a id="related-projects-and-comparison-notes"></a>

## Related projects and comparison notes

<!--
README legacy placement:
The old README had a long similar-projects list. Keep a short README mention and move detailed comparisons here or to a dedicated comparison document.
-->

Trice overlaps with several logging, tracing, and tokenization approaches, but it targets a specific niche: compact printf-like logging for embedded C/C++ with host-side decoding.

### Useful comparison axes

| Axis | Questions to ask |
| --- | --- |
| Target language | C/C++, Rust, mixed, generated code? |
| Target overhead | Does a log call format text, copy a string pointer, write an ID, or use hardware trace? |
| Transport | UART, RTT, SWO, USB, network, file, field storage? |
| Host dependency | Open tool, vendor tool, cloud, debugger integration? |
| Field diagnostics | Can old binary logs be decoded after firmware release? |
| Ecosystem | Standalone library, RTOS-integrated logger, vendor debug suite, cloud product? |

### Related tools and approaches

- plain `printf` / UART logging,
- SEGGER RTT,
- SEGGER SystemView,
- ARM ITM/SWO,
- ARM Keil Event Recorder,
- Rust `defmt`,
- Zephyr dictionary logging,
- Pigweed tokenized logging / tracing,
- Memfault compact logs,
- AUTOSAR DLT,
- Percepio Tracealyzer,
- NanoLog,
- Postform,
- qpspy,
- Serial-Studio,
- MCUViewer,
- McuLog,
- project-specific binary loggers.

### Suggested README wording

Use only a short, neutral comparison sentence in the root README:

> Trice is in the same broad problem space as `printf`, SEGGER RTT/SystemView, ARM ITM/SWO, Zephyr dictionary logging, Rust `defmt`, Pigweed tokenized logging, and Memfault compact logs, but focuses on lightweight printf-like embedded C/C++ logging with host-side ID decoding.

<a id="removed-readme-material-map"></a>

## Removed README material map

<!--
Maintainer note:
This is a practical migration checklist for the current root README. Delete after the manual merge is done.
-->

| Removed or shortened from README | Why it was moved | Suggested destination |
| --- | --- | --- |
| TriceGirl/GIF-heavy first screen | Friendly but may look less production-focused to first-time professional users | Project website or manual introduction, not README top |
| Large expandable overview | Too much before the user understands the core idea | Manual TOC |
| Long RTT command block | Useful but makes J-Link look mandatory | RTT quickstart section |
| Logging/debugging feature subsections | Good material, but not onboarding-critical | Feature overview in manual |
| Data compression / FLASH / encryption / translation / timing sections | Advanced value propositions; keep concise in README | Existing feature chapters |
| ABC explanation and diagram | Powerful but too advanced for first screen | ABC manual chapter |
| Transfer options list | Important after user chooses a path | Mode/transport decision guide |
| Display server explanation | Useful advanced host workflow | CLI examples / display server manual section |
| Debugging with VS Code screenshot | Helpful for example users, distracting in root README | Development environment setup |
| Trice cache detailed explanation | Important, but only after source-ID workflow is understood | Trice cache manual chapter |
| Similar projects long list | Good for research, not first success | Dedicated comparison section |
| Donation/support text near the main onboarding flow | Important but should not interrupt evaluation | Bottom of README |

