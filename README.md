# Trice - Trace IDs for Embedded C/C++ 

> The name combines **TR**ace **ID**s, **C**/**C++**, and **E**mbedded systems.

[View GitHub Pages](https://rokath.github.io/trice/)

<a id="top"> </a>

[![License](https://img.shields.io/github/license/rokath/trice)](./LICENSE.md)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/rokath/trice)
![Downloads](https://img.shields.io/github/downloads/rokath/trice/total)
[![TRICE Library CI (Nightly Full)](https://github.com/rokath/trice/actions/workflows/trice_lib_ci_full.yml/badge.svg)](https://github.com/rokath/trice/actions/workflows/trice_lib_ci_full.yml)

<img align="right" src="docs/ref/TriceGirl-167x222.png" width="115" alt="Trice project mascot"><br>
_Hi, I am Trice._

**Trice** is a compact ID-based logging framework for embedded C/C++.
Firmware code uses `printf`-like calls, but the target sends only small binary records: an ID plus optional runtime values.
The PC-side `trice` tool reconstructs the readable text using `til.json`.

Use Trice when normal `printf` logging is too slow, too large, too intrusive in interrupt contexts, or too inefficient for field diagnostics.

**User Manual:** [GitHub](./docs/TriceUserManual.md) - [GitHub Pages](https://rokath.github.io/trice/docs/TriceUserManual.html) - [PDF](https://github.com/rokath/trice/releases/latest/download/TriceUserManual.pdf) - [Project page](https://rokath.github.io/trice/)

---

*Log in a trice — even from **↯ interrupt handlers** in a few CPU cycles in optimized configurations❗*

<details><summary>Small demo animation</summary>

![Trice demo](./docs/ref/life0.gif)  <!-- ([S>G](https://www.screentogif.com/)) -->

</details>

## Quickstart

Choose one first path:

| Situation | Start here |
|---|---|
| You already have a non-blocking byte writer, DMA TX queue, USB CDC writer, socket writer, pipe, or file writer | [Existing non-blocking byte writer](./docs/TriceUserManual.md#quickstart-existing-non-blocking-byte-writer-deferred-auxiliary-8-bit) |
| You have a SEGGER J-Link and want the quickest lab setup | [SEGGER RTT direct mode](./docs/TriceUserManual.md#quickstart-segger-rtt-direct-mode-with-j-link) |
| You want UART or USB-VCOM output | [UART / USB-VCOM deferred output](./docs/TriceUserManual.md#quickstart-uart-or-usb-vcom-deferred-output) |
| You want complete STM32 examples | [Example projects](./docs/TriceUserManual.md#example-projects-without-and-with-trice-instrumentation) |

For most existing projects, the non-blocking byte writer path is the least disruptive first integration.

## Details and advanced topics

### Basics

<details><summary>Minimal source example</summary>

Trice gives embedded firmware a fast logging path with a familiar source-code style:

```c
trice("boot\n");
trice("adc=%d mV\n", adc_mV);
```

Instead of formatting strings on the target, Trice stores the human-readable text on the host and transfers compact binary records from the target.
This reduces target FLASH usage, transfer bandwidth, log files and log-call execution time.
</details>

<details><summary>How ID-based decoding works</summary>

1. Write `trice(...)` calls in firmware code.
2. Run `trice insert` before compilation.
3. The compiler sees stable numeric IDs.
4. The target emits compact binary Trice records.
5. The PC tool decodes them with the matching `til.json`.

A typical source-level idea is:

```diff
- printf("Temperature: %d C\n", t);
+ trice("Temperature: %d C\n", t);
```
</details>

<details><summary>What about source code modification?</summary>

Trice assigns IDs by processing source files with `trice insert`.
A call such as this:

```c
trice("boot\n");
```

becomes similar to this before compilation:

```c
trice(iD(12345), "boot\n");
```

This is intentional: the target binary contains the ID, while the host keeps the text in `til.json`.

You can choose how visible those IDs are in your workflow:

- keep inserted IDs in source,
- run `trice clean` after the build,
- use the Trice cache to avoid needless rebuilds of unchanged files.

For team projects, define this policy early and treat `til.json` like any other build artifact needed for field diagnostics.
See [Trice ID management](./docs/TriceUserManual.md#trice-id-management).

</details>

<details><summary>Key benefits</summary>

* **[Easy Migration](./docs/TriceUserManual.md#trice-and-legacy-user-code)** – reuse existing `printf`-style code with minimal changes via the `-alias` option
* **[Long-term field decoding](./docs/TriceUserManual.md#versions-and-variants-trice-stability)** – decode logs from released firmware when the matching or accumulated `til.json` is preserved.
* **[Reduced target FLASH](./docs/TriceUserManual.md#trice-memory-needs)** - format strings are kept in `til.json`, not in the target image.
* **Very low target overhead** - down to a few CPU cycles in optimized configurations; see [Trice Speed](./docs/TriceUserManual.md#trice-speed).
* **Transport friendly** - UART, RTT, TCP/UDP, files, or your own non-blocking byte writer.
* **[Compact transfer](./docs/TriceUserManual.md#minimal-transfer-bytes-amount)** – a Trice record contains one 32-bit ID plus optional runtime values, reducing bandwidth and log-file size.
* **Portable tooling** - the host tool is written in Go and runs on common desktop platforms.
* **Further features** - like encryption, timestamps, flexible logging, transport options and tooling are described [here](./docs/TriceUserManual.md#trice-features-overview).
* **Fully documented**: [Trice User Manual](./docs/TriceUserManual.md)

</details>

<details><summary>Which mode should you start with?</summary>

| Mode / path | Use when |
|---|---|
| **Deferred auxiliary 8-bit** | You already have a non-blocking byte writer. This is usually the most universal first integration. |
| **Direct SEGGER RTT** | You have J-Link/RTT and want a fast interactive lab setup. |
| **Deferred UART / USB-VCOM** | Your board exposes a serial path and you want a conventional PC input stream. |
| **Deferred ring buffer** | You want a balanced default for RAM usage and runtime behavior. |
| **Deferred double buffer** | You want the shortest target-side log-call path and can spend more RAM. |

Direct mode writes each record immediately to the selected backend.
Deferred mode writes first into a target buffer; `TriceTransfer()` sends later.
</details>

### Integration workflow

<details><summary>Two parts of Trice</summary>

Trice has two cooperating parts:

- **Target-side C/C++ macros** provide a familiar `printf`-like interface but emit IDs and values instead of full format strings.
- **The PC-side `trice` tool** manages IDs, receives binary data, and reconstructs readable log output.

The host tool is written in [Go](https://go.dev/) and runs on common desktop platforms.
You can also build your own receiver that reads Trice packages, maps IDs to format strings, and displays or stores the decoded output.

</details>

<details><summary>Files: <code>til.json</code>, <code>li.json</code>, and binary logs</summary>

| File | Purpose | Keep? |
|---|---|---|
| `til.json` | Maps IDs to format strings | Yes; required for decoding |
| `li.json` | Maps IDs to source locations | Useful for diagnostics |
| Binary log file | Raw target output | Useful for offline or field decoding |

Keep the matching `til.json` for every released firmware version.
The repository example file is [demoTIL.json](./demoTIL.json).

</details>

<details><summary>Three common ID workflows</summary>

**Keep IDs in source** - good for small projects and maximum robustness.

```bash
trice insert -src ./ -i ./til.json -li ./li.json
# build normally
```

**Insert before build, clean after build** - good when the repository should stay visually free of IDs.

```bash
trice insert -src ./ -i ./til.json -li ./li.json
# build
trice clean -src ./ -i ./til.json -li ./li.json
```

**Use the Trice cache** - good when repeated insert/clean would otherwise cause unnecessary recompilation.

```bash
mkdir -p ~/.trice/cache
trice insert -cache -src ./ -i ./til.json -li ./li.json
# build
trice clean -cache -src ./ -i ./til.json -li ./li.json
```

Pick one workflow and document it in the project build instructions.
If other tools also rewrite source files, such as formatters or code generators, run them before `trice insert`.

</details>

<details><summary>Trice cache</summary>

The Trice cache avoids unnecessary rebuilds when using `trice insert` and `trice clean` around the build.
It keeps cached inserted and cleaned copies of unchanged files.

Typical use:

```bash
mkdir -p ~/.trice/cache
trice insert -cache -src ./ -i ./til.json -li ./li.json
# build
trice clean -cache -src ./ -i ./til.json -li ./li.json
```

Use cache when you want to keep IDs out of your source code while avoiding repeated recompilation of unchanged files.
Be careful when your build system also modifies source files; for example, run auto-formatters before `trice insert`.

How Cache Works:

The Trice cache saves copies of all files after processing them with `trice i` or `trice c`. This avoids inserting and removing IDs repeatedly. **The copies are used to get the same results for files that have not been edited.** Edited files are processed normally and the cache updates afterwards. File modification times do not change, so **the build system does not reprocess unchanged files even when IDs are temporarily removed**.

See [Trice Cache for Compilation Speed](./docs/TriceUserManual.md#trice-cache-for-compilation-speed).

</details>

<details><summary>Normal workflow</summary>

1. Add the Trice target sources from [`src/`](./src) to your firmware project.
2. Add a project-specific `triceConfig.h`.
3. Write Trice calls in your code.
4. Run `trice insert` before compiling. This assigns stable IDs and updates `til.json` and `li.json`.
5. Build and flash the target.
6. Run `trice log` on the PC to decode the binary stream.

Minimal firmware shape:

```c
#include "trice.h"

int main(void) {
    BoardInit();
    TriceInit();

    trice("boot\n");
    trice("adc=%d mV\n", adc_mV);

    for (;;) {
        AppRun();
        TriceTransfer(); // needed for deferred outputs
    }
}
```

Create the ID files once, then insert IDs:

```bash
touch til.json li.json
trice insert -src ./ -i ./til.json -li ./li.json
```

Start logging from a serial or USB-VCOM port:

```bash
trice log -p COM15 -baud 921600 -i ./til.json -li ./li.json
# or, on Linux/macOS:
trice log -p /dev/ttyACM0 -baud 921600 -i ./til.json -li ./li.json
```

</details>

<details><summary>Legacy code integration</summary>

Existing projects do not have to migrate all logs at once.
Common strategies:

1. Keep legacy `printf` output and send Trice on a separate physical channel.
2. Replace selected `printf` calls with Trice calls where speed, bandwidth, or FLASH matters.
3. Use Trice aliases so project-specific log macros are processed by `trice insert` without renaming every call site.
4. Use a shared output channel only if framing and mixed text/binary behavior are explicitly configured and tested.

For a first integration, prefer a small vertical slice:

- one source file,
- one working output path,
- one `til.json`,
- one build pre-step,
- one host `trice log` command.

Then expand to aliases and larger source trees.
See [Trice and legacy User Code](./docs/TriceUserManual.md#trice-and-legacy-user-code).

</details>

### Runtime modes and output paths

<details><summary>Direct mode and deferred mode</summary>

| Mode | What happens during the log call | Good for |
|---|---|---|
| **Direct mode** | Each Trice record is written immediately to the selected output backend. | Interactive development, especially SEGGER RTT. |
| **Deferred mode** | The log call writes only into a target buffer; `TriceTransfer()` sends later. | UART, USB CDC, existing writer functions, production-style logging, shortest target execution path. |

A practical rule:

- Start with **deferred auxiliary 8-bit output** when your project already has a write function.
- Start with **direct SEGGER RTT** when you have J-Link and want the easiest lab setup.
- Use **deferred ring buffer** for a balanced RAM/speed setup.
- Use **deferred double buffer** when minimizing the target-side log-call cost is more important than RAM.

A slow physical output path, especially a blocking UART, can dominate direct-mode timing.
Deferred mode avoids this during the log call, but you must call `TriceTransfer()` often enough and size buffers for burst load.

</details>

<details><summary>Buffer choice</summary>

| Buffer | Typical use | Trade-off |
|---|---|---|
| `TRICE_STACK_BUFFER` | Simple direct-output setups | No persistent deferred storage; stack use matters |
| `TRICE_STATIC_BUFFER` | Direct output without stack allocation | One static single-message buffer |
| `TRICE_RING_BUFFER` | Balanced deferred output | Less RAM, robust general-purpose default |
| `TRICE_DOUBLE_BUFFER` | Minimum target-side Trice execution time | More RAM, best for high-speed deferred extraction |

See [Trice Speed](./docs/TriceUserManual.md#trice-speed) and [Trice memory needs](./docs/TriceUserManual.md#trice-memory-needs).

</details>

<details><summary>Auxiliary hooks</summary>

Auxiliary hooks are useful when the project already owns the physical transport.
They can write Trice binary data to an SD card, flash partition, UART/USB queue, socket, pipe, file, or other byte sink.

| Switch | Hook | Writer shape | Typical use |
|---|---|---|---|
| `TRICE_DIRECT_AUXILIARY8` | `UserNonBlockingDirectWrite8AuxiliaryFn` | `const uint8_t*`, byte length | Direct byte stream, only if fast/non-blocking |
| `TRICE_DEFERRED_AUXILIARY8` | `UserNonBlockingDeferredWrite8AuxiliaryFn` | `const uint8_t*`, byte length | Most universal first integration |
| `TRICE_DIRECT_AUXILIARY32` | `UserNonBlockingDirectWrite32AuxiliaryFn` | `const uint32_t*`, word count | Fast word-oriented direct sink |
| `TRICE_DEFERRED_AUXILIARY32` | `UserNonBlockingDeferredWrite32AuxiliaryFn` | `const uint32_t*`, word count | Word-oriented deferred sink; also useful with encryption |

The deferred 8-bit hook is usually the safest first quickstart because almost every transport can consume bytes.
See [Writing the Trice logs into an SD-card or a user-specific output](./docs/TriceUserManual.md#writing-the-trice-logs-into-an-sd-card-or-a-user-specific-output).

</details>

<details><summary>Data transfer options</summary>

Implemented transfer methods include:

- [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter), including virtual UART over USB,
- [RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/), including [J-Link](./third_party/segger.com/ReadMe.md) and [ST-Link](./third_party/goST/ReadMe.md) related paths,
- TCP4 input/output,
- UDP4 input.

Other possible transport approaches include a small bridge microcontroller or FTDI-style adapters for GPIO, I2C, SPI, CAN, LIN, UART, FIFO, or file-based capture.
Start with a supported path first; optimize transport later.

</details>

<details><summary>Display server for multiple targets</summary>

`trice ds` can act as a display server.
Multiple `trice log` instances can send completed log lines to one display server so several targets appear in one combined output.

```bash
trice ds
trice log -p COM15 -ds
trice log -p COM16 -ds
```

See [Logging Over a Display Server](./docs/TriceUserManual.md#logging-over-a-display-server) and [Several Targets at the same time](./docs/TriceUserManual.md#several-targets-at-the-same-time).

</details>

### Features beyond basic logging

<details><summary>FLASH and bandwidth reduction</summary>

Replacing format strings with IDs at compile time acts like compression for target FLASH and transferred log data.
A typical Trice record can be only a few bytes plus parameters, independent of the human-readable format-string length.

See also:

- [Small Size - using Trice frees FLASH Memory](./docs/TriceUserManual.md#small-size---using-trice-frees-flash-memory)
- [Minimal Transfer Bytes Amount](./docs/TriceUserManual.md#minimal-transfer-bytes-amount)
- [Trice Memory Needs](./docs/TriceUserManual.md#trice-memory-needs)

</details>

<details><summary>Field logs and diagnostics</summary>

Trice can be used as a fast `printf`-debugging replacement and as a compact logging system for field diagnostics.
The target emits short binary records; the host produces the readable log later.

Keep the matching `til.json` so logs from field devices can be decoded after release.
Optionally store project-specific `til.json` and `li.json` together with firmware images or diagnostic packages.

</details>

<details><summary>Storing Trice logs in FLASH, SD-card, or files</summary>

Trice binary logs can be written to FLASH, SD-card, a file, or another project-specific output for later analysis.
This can be useful when a target is not permanently connected to a PC.

See [Writing the Trice logs into an SD-card or a user-specific output](./docs/TriceUserManual.md#writing-the-trice-logs-into-an-sd-card-or-a-user-specific-output).

</details>

<details><summary>Encryption</summary>

Trice supports optional XTEA encryption for transport packets.
This can be useful when field logs should only be readable with the matching key and ID list.

Enable encryption in `triceConfig.h` and pass the matching password/key option to `trice log`.
See [Optional XTEA encryption](./docs/TriceUserManual.md#optional-xtea-encryption).

</details>

<details><summary>Host-side wording and translation</summary>

Because the target sends IDs and values, the host-side `til.json` can map an ID to different text as long as the ID and parameter format remain compatible.
This allows host-side wording changes or language variants without changing the target binary.

See [Switch the language without changing a bit inside the target code](./docs/TriceUserManual.md#switch-the-language-without-changing-a-bit-inside-the-target-code).

</details>

<details><summary>Timing analysis</summary>

Trice supports host and target timestamps and can help with distributed timing analysis, interrupt timing, and observing target behavior without stopping firmware in a debugger.

See:

- [Trice Timestamps](./docs/TriceUserManual.md#trice-timestamps)
- [Target Timestamps Formatting](./docs/TriceUserManual.md#target-timestamps-formatting)
- [Target Timestamp Delta Columns](./docs/TriceUserManual.md#target-timestamp-delta-columns)
- [Cycle Counter](./docs/TriceUserManual.md#cycle-counter)

</details>

<details><summary>Trice ABC - Asynchronous Broadcast Commands</summary>

**Trice ABC** extends the normal Trice idea from logging to compact command records.

![Trice ABC overview](./docs/ref/trice_abc_readme_teaser.svg)

A sender emits a normal Trice record, for example:

```c
trice8C("cmd:setLeds", &mask, 1);
```

After `trice insert`, the command name is represented by a numeric ID in `til.json`.
With `trice generate -abc`, receiver-side dispatch tables are generated so each receiver can select which command IDs become local handler calls.

ABC is intentionally **not** a full RPC layer by itself.
Addressing, ACKs, retries, authentication, timeouts, and return-value semantics remain application policy.

See [Trice ABC in the User Manual](./docs/TriceUserManual.md#trice-abc---asynchronous-broadcast-commands) and the [host-native ABC demo](./examples/TriceAbc).

</details>

<details><summary>UART example diagram</summary>

This simplified [draw.io](https://github.com/jgraph/drawio) diagram shows how Trice works over a UART-style path.
Read the detailed explanation in [How it works - the main idea](./docs/TriceUserManual.md#how-it-works---the-main-idea).

![Trice UART block diagram](docs/ref/triceCOBSBlockDiagram.svg)

</details>

<details><summary>Routing different ID ranges to different outputs</summary>

Trice can route different ID ranges or tagged messages to different outputs depending on configuration.
See the User Manual for the current configuration details and examples.

</details>

### Project information

<details><summary>Installation</summary>

Use one of these options:

```bash
# macOS with Homebrew
brew install rokath/tap/trice
```

Or download a release binary from [the latest release](https://github.com/rokath/trice/releases/latest) and put `trice` into your `PATH`.

Developers with Go installed can also build/install from source:

```bash
go install github.com/rokath/trice/cmd/trice@latest
```

Check the installation:

```bash
trice --version
trice help -insert
trice help -log
```

</details>

<details><summary>Documentation and examples</summary>

- [Trice User Manual](./docs/TriceUserManual.md)
- [GitHub Pages documentation](https://rokath.github.io/trice/docs/TriceUserManual.html)
- [Generated CLI help](./docs/ref/trice-help-all.txt)
- [Target source configuration defaults](./src/triceDefaultConfig.h)
- [Examples](./examples)
- [Issues](https://github.com/rokath/trice/issues), including closed items
- [Discussions](https://github.com/rokath/trice/discussions), including closed items

</details>

<details><summary>Development setup and debugging</summary>

The example projects show Trice integration with VS Code, Makefiles, Clang/GCC, STM32 projects, RTT, and UART-style output.
Use bare/instrumented project pairs to inspect the minimal integration diff.

Debug a Trice project in Direct-Out Mode over SEGGER-RTT.

<img src="docs/ref/Animation.gif" width="1200">

See [Development Environment Setup](./docs/TriceUserManual.md#development-environment-setup).

</details>

<details><summary>Project status</summary>

![GitHub commits since latest release](https://img.shields.io/github/commits-since/rokath/trice/latest)
![GitHub issues](https://img.shields.io/github/issues/rokath/trice)
[![Coverage Status](https://coveralls.io/repos/github/rokath/trice/badge.svg?branch=main)](https://coveralls.io/github/rokath/trice?branch=main)
[![CodeQL](https://github.com/rokath/trice/actions/workflows/codeql.yml/badge.svg)](https://github.com/rokath/trice/actions/workflows/codeql.yml) 

Trice is usable today and actively maintained.
Use the latest release or the `main` branch when building from source.
Avoid development branches unless you intentionally want unreleased work.

Contributions are welcome: examples, platform recipes, transport backends, documentation improvements, and reproducible benchmark reports are especially valuable.

</details>

<details><summary>Future ideas</summary>

Potential future work includes:

- a small `tlog` tool in C, Python, Rust, TinyGo, Wasm, or another runtime for environments where Go is not the best fit,
- more structured logging support; see the [structured logging specification draft](./docs/TriceUserManual.md#trice-structured-logging),
- additional transport recipes,
- adaptation for a visual RTOS/event timeline tool.

</details>

<details><summary>Support the project</summary>

- Star this project if it helps you.
- Contribute examples, fixes, documentation, or platform recipes.
- If Trice helps a commercial product, consider supporting continued maintenance through the sponsor links on the GitHub project page.

Support options:

- [Become a Sponsor with your GitHub account](https://github.com/sponsors/rokath/)
- <a href="https://buymeacoffee.com/rokath" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="30" width="120"></a>
- <a href="https://www.paypal.com/paypalme/rolfkarlthomas"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif" alt="PayPal" height="40" width="120"></a>

</details>

<details><summary>Similar projects</summary>

Trice overlaps with several logging, tracing, and tokenization approaches, but it targets a specific niche: compact `printf`-like logging for embedded C/C++ with host-side ID decoding.

- plain `printf` / UART logging
- ARM ITM/SWO (hardware-native)
- ARM Keil Event Recorder (hardware-native)
- [baical.net](https://baical.net/up7.html) (C)
- [call stack logger function instrumentation](https://dev.to/taugustyn/call-stack-logger-function-instrumentation-as-a-way-to-trace-programs-flow-of-execution-419a) (trace program execution flow)
- [Debugging with Dynamic Printf Breakpoints](https://mcuoneclipse.com/2022/02/09/debugging-with-dynamic-printf-breakpoints/) (Eclipse IDE option)
- [defmt (Rust)](https://github.com/knurling-rs/defmt)
- [Diagnostic Log and Trace](https://github.com/COVESA/dlt-daemon) (AUTOSAR)
- [Embedded Logger (elog)](https://github.com/martinribelotta/elog) (Embedded logger with minimal footprint and memory usage)
- [Logging with symbols - The Embedonomicon](https://docs.rust-embedded.org/embedonomicon/logging.html)
- [McuLog (McuOnEclipse)](https://mcuoneclipse.com/2020/06/01/mculog-logging-framework-for-small-embedded-microcontroller-systems/)
- [MCUViewer](https://github.com/klonyyy/MCUViewer)
- [Memfault Compact Log Library](https://docs.memfault.com/docs/mcu/compact-logs/#host-decoding) (part of firmware SDK)
- [Minimal Structured Logging for Autonomous Vehicles](https://youtu.be/FyJI4Z6jD4w) (C++, closed source talk)
- [NanoLog (C++11)](https://github.com/PlatformLab/NanoLog) (Linux C++)
- [Percepio Tracealyzer (with TraceRecorder)](https://percepio.com/tracealyzer/) (Visual Trace Diagnostics)
- [Pigweed Trace Tokenized](https://pigweed.dev/pw_trace_tokenized/)
- [Postform](https://github.com/Javier-varez/Postform) (Postponed formatting experiments with string interning in C++, inspired by the defmt Rust crate)
- [qpspy](https://www.state-machine.com/qtools/qpspy.html) (C/C++)
- [Real-time binary data logging/tracing toolkit](https://github.com/RTEdbg/RTEdbg)
- SEGGER RTT
- [SEGGER System View](https://www.segger.com/products/development-tools/systemview/technology/what-is-systemview/)
- [Serial-Studio](https://github.com/Serial-Studio/Serial-Studio) (Data visualisation)
- [Tonbandgerät](https://github.com/schilkp/Tonbandgeraet) (Small embedded systems tracer with support for bare-metal and FreeRTOS-based targets)
- [Traces](https://github.com/yotamr/traces) (API tracing framework for Linux C/C++ applications)
- [uLog (RD Poor)](https://github.com/rdpoor/ulog)
- [Zephyr Dictionary Based Logging](https://docs.zephyrproject.org/latest/services/logging/#dictionary-based-logging)
- project-specific binary loggers.

Additional generated comparison material:

- [Trice Compare (generated 2025-05-26)](https://htmlpreview.github.io/?https://github.com/rokath/trice/blob/main/docs/ChatGPTo4-mini-high_TriceCompare.html) - compact generated comparison with related tools
- [Logging & Tracing Solutions for Embedded Systems (generated 2026-02-16)](https://htmlpreview.github.io/?https://github.com/rokath/trice/blob/main/docs/2026-02-16_ChatGPT5.2ProExtThinking_embedded_logging_tracing_comparison_trice_focus.html) - longer generated overview of the broader tool landscape

</details>

<p align="right">(<a href="#top">back to top</a>)</p>
