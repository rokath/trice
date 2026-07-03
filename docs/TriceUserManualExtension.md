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

<a id="quickstarts"></a>

## Quickstarts

<!--
README placement hint:
The root README should only show a small "choose your path" table and link into this section.
Detailed commands belong here or in dedicated docs/quickstart-*.md files.
-->

<a id="quickstart-existing-byte-writer"></a>

### Quickstart: Existing non-blocking byte writer, deferred auxiliary 8-bit

<!--
Existing manual coverage:
`TriceUserManual.md#writing-the-trice-logs-into-an-sd-card-or-a-user-specific-output` already documents `TRICE_DEFERRED_AUXILIARY8`, `TRICE_DEFERRED_AUXILIARY32`, and `UserNonBlockingDeferredWrite8AuxiliaryFn`.
This section reframes that information as a first-use quickstart, not as an SD-card special case.

README legacy placement:
This should replace the README's first-time dependence on SEGGER RTT when the user already has a byte output path.
-->

Use this path when your project already has a tested output function, for example:

- a non-blocking UART TX queue,
- a USB CDC/VCOM TX queue,
- a DMA-backed byte stream,
- a socket or pipe in a host-native test program,
- a file writer in a PC-side demo.

This is often the most universal first integration because Trice does not need to know your peripheral driver.
It only needs a function that accepts a byte buffer and length.

#### 1. Add Trice target sources

Add the complete [`src`](../src) folder to your target project unchanged and add `src` to the compiler include path.
Create a project-specific `triceConfig.h` in your application include path.

#### 2. Configure deferred auxiliary 8-bit output

Minimal `triceConfig.h` starting point:

```c
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_AUXILIARY8 1

/* Good default for byte streams such as UART, USB CDC, files, pipes. */
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_TCOBS
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

/* Adapt these to your target if Trice can run from interrupts or multiple contexts. */
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }

#endif /* TRICE_CONFIG_H_ */
```

Notes:

- `TRICE_RING_BUFFER` is a balanced default.
- `TRICE_DOUBLE_BUFFER` can be faster for the Trice call itself, at the cost of more RAM and different buffer behavior.
- Keep the writer non-blocking or at least tightly bounded. A blocking writer moves the latency problem into `TriceTransfer()`.

#### 3. Assign your writer function

Example:

```c
#include "trice.h"

static void MyNonBlockingByteWrite(const uint8_t* data, size_t len) {
    /* Replace this with your project's existing writer. */
    ExistingTxQueueWrite(data, len);
}

void AppInit(void) {
    BoardInit();
    TriceInit();

    UserNonBlockingDeferredWrite8AuxiliaryFn = MyNonBlockingByteWrite;

    trice("boot\n");
}

void AppMainLoop(void) {
    for (;;) {
        AppRun();
        TriceTransfer();
    }
}
```

`TriceTransfer()` moves accumulated Trice records from the deferred buffer to your writer. Call it cyclically from the main loop, a low-priority task, or another context that is safe for your output driver.

#### 4. Insert IDs before compiling

From your project root:

```bash
touch til.json li.json
trice insert -src ./ -i ./til.json -li ./li.json
```

Then build and flash your target.

#### 5. Decode on the PC

For a serial or USB virtual COM port:

```bash
trice log -p COM15 -baud 921600 -i ./til.json -li ./li.json
```

On Linux/macOS, adapt the port:

```bash
trice log -p /dev/ttyACM0 -baud 921600 -i ./til.json -li ./li.json
```

If your writer produces a file, pipe, TCP stream, or another source, use the matching `trice log -p ...` input port.

#### 6. Common first checks

- If you see no output, first confirm that your writer is called from `TriceTransfer()`.
- If raw data appears but does not decode, check framing settings on target and host.
- If output stops under burst load, increase `TRICE_DEFERRED_BUFFER_SIZE`, call `TriceTransfer()` more often, or improve the non-blocking writer queue.
- If interrupts or multiple tasks can call Trice, provide real critical-section macros.

#### 7. Why this quickstart matters

The old README led many first-time readers toward SEGGER RTT because it is convenient and fast.
That path is still valuable, but it also implies J-Link hardware and SEGGER tooling.
The auxiliary writer path is more portable: many projects already have a byte-stream output, and Trice can reuse it.

<a id="quickstart-segger-rtt-direct"></a>

### Quickstart: SEGGER RTT direct mode with J-Link

<!--
Existing manual coverage:
See `TriceUserManual.md#trice-over-rtt`, especially the RTT/J-Link sections and the notes around `TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE`.

README legacy placement:
This was previously the dominant README quickstart. It should remain a strong quickstart, but no longer be the only obvious first path.
-->

Use this path when you have a SEGGER J-Link and want the smallest amount of target-specific transport code.

#### 1. Install tools

- Install the `trice` host tool.
- Install the SEGGER J-Link software package so that `JLinkRTTLogger` or the relevant J-Link tools are in `PATH`.

#### 2. Add target sources

Add the complete [`src`](../src) folder to your target project unchanged and add `src` to the compiler include path.

#### 3. Configure direct RTT

Minimal `triceConfig.h`:

```c
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

#endif /* TRICE_CONFIG_H_ */
```

#### 4. Add a first Trice call

```c
#include "trice.h"

int main(void) {
    BoardInit();
    TriceInit();

    trice("Hello RTT\n");

    for (;;) {
        AppRun();
    }
}
```

Direct RTT does not require `TriceTransfer()` for the normal direct output path.

#### 5. Insert, build, flash

```bash
touch til.json li.json
trice insert -src ./ -i ./til.json -li ./li.json
```

Build and flash the target.

#### 6. Log through J-Link RTT

Example command; adapt the device name and speed:

```bash
trice log -p JLINK \
  -args "-Device STM32G0B1RE -if SWD -Speed 4000 -RTTChannel 0" \
  -pf none -prefix off -hs off -d16 \
  -i ./til.json -li ./li.json
```

Alternative file-based workflow:

```bash
rm -f ./temp/trice.bin
mkdir -p ./temp
touch ./temp/trice.bin
JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin
```

In a second terminal:

```bash
trice log -p FILE -args ./temp/trice.bin \
  -pf none -prefix off -hs off -d16 \
  -i ./til.json -li ./li.json
```

#### 7. When this path is ideal

Direct RTT is excellent for lab development because the target writes to RTT memory and the probe drains it. It avoids UART setup and usually feels close to `printf` debugging.

#### 8. When this path is not ideal

It depends on J-Link/RTT infrastructure. If that hardware or closed host tooling is a blocker, start with the [existing byte writer quickstart](#quickstart-existing-byte-writer) or a UART/VCOM deferred path.

<a id="quickstart-uart-vcom-deferred"></a>

### Quickstart: UART or USB-VCOM deferred output

<!--
Existing manual coverage:
See `TriceUserManual.md#communication-ports`, the example projects, and the UART-related configuration examples.
This section is intentionally short because UART setup is MCU/vendor-specific.
-->

Use this path when the target has a UART, USB CDC/VCOM, or board-specific serial path and you want Trice to use the built-in UART backend rather than an auxiliary writer.

Minimal shape of `triceConfig.h`:

```c
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#include "main.h" /* or your MCU/vendor header */

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2 /* adapt to your project */

#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_TCOBS
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

#endif /* TRICE_CONFIG_H_ */
```

Application shape:

```c
#include "trice.h"

int main(void) {
    BoardInit();
    UartInit();
    TriceInit();

    trice("boot\n");

    for (;;) {
        AppRun();
        TriceTransfer();
    }
}
```

Host side:

```bash
trice log -p COM15 -baud 921600 -i ./til.json -li ./li.json
```

On Linux/macOS:

```bash
trice log -p /dev/ttyACM0 -baud 921600 -i ./til.json -li ./li.json
```

For a quick first success, the [existing byte writer quickstart](#quickstart-existing-byte-writer) may be easier if your project already has a working serial or USB write function.

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

