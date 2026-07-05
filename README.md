# Trice - Trace IDs for Embedded C/C++ 

> 🅃🅁ace 🄸d's for 🄲 🄴mbedded

[View GitHub Pages](https://rokath.github.io/trice/)

<a id="top"> </a>

<!-- mdtoc off -->
## ![TriceGirlS.png](docs/ref/TriceGirl-167x222.png) *Hi, I am Trice.*
<!-- mdtoc on -->
---

![License](https://img.shields.io/github/license/rokath/trice) ![GitHub release (latest by date)](https://img.shields.io/github/v/release/rokath/trice) ![GitHub commits since latest release](https://img.shields.io/github/commits-since/rokath/trice/latest) ![Downloads](https://img.shields.io/github/downloads/rokath/trice/total) ![GitHub issues](https://img.shields.io/github/issues/rokath/trice) [![TRICE Library CI (Nightly Full)](https://github.com/rokath/trice/actions/workflows/trice_lib_ci_full.yml/badge.svg)](https://github.com/rokath/trice/actions/workflows/trice_lib_ci_full.yml) 
<!--
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](https://makeapullrequest.com) ![Go Version](https://img.shields.io/github/go-mod/go-version/rokath/trice)  [![Go Report Card](https://goreportcard.com/badge/github.com/rokath/trice)](https://goreportcard.com/report/github.com/rokath/trice) [![Coverage](https://coveralls.io/repos/github/rokath/trice/badge.svg?branch=main)](https://coveralls.io/github/rokath/trice?branch=main) 
[![Go Reference](https://pkg.go.dev/badge/github.com/rokath/trice.svg)](https://pkg.go.dev/github.com/rokath/trice) 
[![Go Coverage (PR + Monthly)](https://github.com/rokath/trice/actions/workflows/coverage.yml/badge.svg)](https://github.com/rokath/trice/actions/workflows/coverage.yml) 
[![Clang-Format Check](https://github.com/rokath/trice/actions/workflows/clang-format.yml/badge.svg)](https://github.com/rokath/trice/actions/workflows/clang-format.yml) 
[![TRICE Library CI (Nightly Quick)](https://github.com/rokath/trice/actions/workflows/trice_lib_ci.yml/badge.svg)](https://github.com/rokath/trice/actions/workflows/trice_lib_ci.yml) 
[![Mark stale issues and pull requests](https://github.com/rokath/trice/actions/workflows/stale.yml/badge.svg)](https://github.com/rokath/trice/actions/workflows/stale.yml) 
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/rokath/trice) 
[![CodeQL](https://github.com/rokath/trice/actions/workflows/codeql.yml/badge.svg)](https://github.com/rokath/trice/actions/workflows/codeql.yml) 
![GitHub watchers](https://img.shields.io/github/watchers/rokath/trice?label=watch) 
[![test](https://github.com/shogo82148/actions-goveralls/workflows/_test/badge.svg?branch=main)](https://coveralls.io/github/rokath/trice) 
![GitHub Workflow Status](https://img.shields.io/github/workflow/status/rokath/trice/goreleaser) 
[![Sponsor rokath](https://github.com/sponsors/rokath/button)](https://github.com/sponsors/rokath)

-->

*Log in a trice — with Trice, even from **↯ interrupt handlers** in less than 1 µs ❗*

![ ](./docs/ref/life0.gif) <!-- ([S>G](https://www.screentogif.com/)) -->

**Trice User Manual:** [GitHub](./docs/TriceUserManual.md) • [GH Pages](https://rokath.github.io/trice/docs/TriceUserManual.html) • [PDF](https://github.com/rokath/trice/releases/latest/download/TriceUserManual.pdf)

<!-- mdtoc -->
<!-- numbering=false min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

## What is Trice?

**Trice** is under the hood an ID-based (ultra-low-overhead) logging framework for embedded C/C++. It provides printf-like usability with only a few (down to 6!) CPU cycles per log call, making logging practical even when used from interrupt handlers in real-time firmware systems.

Trice is designed for systems where traditional logging is too slow, too large, or not safe to use in interrupt or real-time contexts.

Trice replaces/re-assigns legacy `printf` logging in embedded C/C++ systems with a much faster and more efficient approach.

Instead of formatting and storing strings on the target, Trice encodes log messages as compact IDs and keeps the actual strings on the host. This reduces runtime overhead, memory usage, and data transfer size.


## How it works

* The target device sends only IDs and data.
* The host reconstructs the original messages using the Trice ID list.
* At build time, Trice assigns each log message a compact ID while the user source code stays unchanged - an ID is inserted temporarily just for the compiler run.

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">What about source code modification?</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

Trice assigns IDs by processing source files with `trice insert`. A call such as this:

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

For team projects, define this policy early and keep `til.json` under control like any other build artifact needed for field diagnostics. See [ID workflow notes](./docs/TriceUserManualExtension.md#id-workflow-notes) and [Trice ID management](./docs/TriceUserManual.md#trice-id-management).

---

</details>

## Key Benefits

* **[Good for interrupts and timing work](./docs/TriceUserManual.md#trice-speed)**: Needs only a few CPU cycles in the most optimized configurations.
* **[Smaller Binaries](./docs/TriceUserManual.md#trice-memory-needs)**: Format strings are kept in `til.json`, not in target FLASH.
* **Minimal log data streams**: A Trice message is one 32-bit value plus optional runtime user values.
* **[Long-term field decoding](./docs/TriceUserManual.md#versions-and-variants-trice-stability)** – decode logs from older firmware any newer til.json.
* **[Easy Migration](./docs/TriceUserManual.md#trice-and-legacy-user-code)** – reuse existing `printf`-style code with minimal changes via the `-alias` option
* **Portable tooling** - the host tool is written in Go and runs on common desktop platforms.
* **[➕ More Features](./docs/TriceUserManual.md#trice-features-overview)** – flexible logging, transport options, and tooling.
* **Fully documented**: [Trice User Manual](./docs/TriceUserManual.md)

## Two Parts of Trice

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">C code macros</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

Provide a familiar `printf`-like interface at the application level, but internally send just ID and values instead of full format strings. The Trice tool maps these IDs back to readable text.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Trice tool</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

Manages and displays the logs
   - Written in [Go](https://go.dev/) - works on all platforms that Go supports
   - You can also build your own tool to receive Trice packages, replace IDs with text, and display the output

---

</details>

## Quickstart: choose your first path 

![Trice log icon](./docs/ref/trice_log_icon_128x128.png)

<details markdown="1"> <!-- parse this block as markdown -->
<summary>(click to expand)</span></summary>

Use the `trice i -alias` option to keep legacy code mostly unchanged. For new projects use `trice` macros instead of `printf`.

The fastest way to evaluate Trice is to start with one proven transport path, not with every feature at once.

* Your project already has a non-blocking byte writer, DMA TX queue, USB CDC writer, socket writer, or file writer:
  * [Quickstart: Existing non-blocking byte writer, deferred auxiliary 8-bit](./docs/TriceUserManual.md#quickstart-existing-non-blocking-byte-writer-deferred-auxiliary-8-bit)
* You have a SEGGER J-Link and want the least target-porting work:
  * [Quickstart: SEGGER RTT direct mode with J-Link](./docs/TriceUserManual.md#quickstart-segger-rtt-direct-mode-with-j-link)
* You want a normal UART or USB virtual COM port:
  * [Quickstart: UART or USB-VCOM deferred output](./docs/TriceUserManual.md#quickstart-uart-or-usb-vcom-deferred-output)
* You want to inspect complete STM32 example projects:
  * [Example projects](./docs/TriceUserManual.md#example-projects-without-and-with-trice-instrumentation)

For most existing projects, the **existing byte writer** path is the least disruptive first integration: Trice writes framed binary data into your already-tested output function.

### Which Mode Should You Use?

- **For development:** Direct mode with SEGGER_RTT is recommended
- **For most use cases:** Deferred mode with TRICE_BUFFER == TRICE_RING_BUFFER (uses less RAM) in TRICE_MULTI_PACK_MODE (transfers less data)

---

</details>

## Further Information

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Logging and Debugging</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Logging and Debugging

Trice can be used as a fast `printf`-debugging replacement and as a compact logging system. The advantage is very short messages (no strings) for data transfer. That can be handy, when you have heavy log loads. Keep your project-specific `til.json` file available to decode field logs later. The repository example file is [demoTIL.json](./demoTIL.json).

- **Optional:** Add your project-specific `til.json` as a compressed resource to your target image. You can use [SRecord](https://srecord.sourceforge.net/download.html) or provide a download link.

Existing manual coverage:

* [Abstract](./docs/TriceUserManual.md#abstract)
* [How it works - the main idea](./docs/TriceUserManual.md#how-it-works---the-main-idea)
* [Start with Trice](./docs/TriceUserManual.md#start-with-trice) 
* CLI examples

 The target sends short binary records; the host produces the readable log. Keep the matching `til.json` so logs from field devices can be decoded later.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Files</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Files

| File            | Purpose                      | Keep?                             |
|-----------------|------------------------------|-----------------------------------|
| `til.json`      | maps IDs to format strings   | yes; required for decoding        |
| `li.json`       | maps IDs to source locations | useful for diagnostics            |
| binary log file | raw target output            | useful for offline/field decoding |

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Three common workflows</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

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

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Direct mode and deferred mode</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Direct mode and deferred mode

Trice has two main runtime styles.

| Mode              | What happens during the log call                                              | Good for                                                                                            |
|-------------------|-------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------|
| **Direct mode**   | Each Trice record is written immediately to the selected output backend.      | Interactive development, especially SEGGER RTT.                                                     |
| **Deferred mode** | The log call writes only into a target buffer; `TriceTransfer()` sends later. | UART, USB CDC, existing writer functions, production-style logging, shortest target execution path. |

A practical rule:

- Start with **direct SEGGER RTT** when you have J-Link and want the easiest lab setup.
- Start with **deferred auxiliary 8-bit output** when your project already has a write function.
- Use **deferred ring buffer** for a balanced RAM/speed setup.
- Use **deferred double buffer** when minimizing the target-side log-call cost is more important than RAM.

See [mode and transport selection](./docs/TriceUserManualExtension.md#mode-and-transport-selection) for more detail.

#### Direct mode

Direct mode writes each Trice record immediately to the selected output backend.

Good first use:

- SEGGER RTT with J-Link.
- Fast direct auxiliary output where the writer is guaranteed not to block.
- Lab setups where immediate output matters more than keeping the log call as short as possible.

Risk:

- A slow physical output path, especially a blocking UART, can dominate the Trice call time.

#### Deferred mode

Deferred mode makes the Trice call write into a target buffer. A later `TriceTransfer()` sends buffered data to the output backend.

Good first use:

- UART and USB CDC/VCOM.
- Existing byte writer functions.
- Production-style logging.
- Shortest target-side Trice call path.

Risk:

- You must call `TriceTransfer()` often enough.
- Buffer sizing and overflow behavior need attention under burst load.


---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Buffer choice</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Buffer choice

| Buffer                | Typical use                              | Trade-off                                         |
|-----------------------|------------------------------------------|---------------------------------------------------|
| `TRICE_STACK_BUFFER`  | Simple direct-output setups              | no persistent deferred storage; stack use matters |
| `TRICE_STATIC_BUFFER` | Direct output without stack allocation   | one static single-message buffer                  |
| `TRICE_RING_BUFFER`   | Balanced deferred output                 | less RAM, robust general-purpose default          |
| `TRICE_DOUBLE_BUFFER` | Minimum target-side Trice execution time | more RAM, best for high-speed deferred extraction |

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Auxiliary hooks</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Auxiliary hooks (Writing logs to files or storage)

Existing [manual coverage](./docs/TriceUserManual.md#writing-the-trice-logs-into-an-sd-card-or-a-user-specific-output).

The auxiliary deferred write hooks can write Trice binary data to an SD card, flash partition, host file, or any other byte sink. Store `til.json` and optionally `li.json` together with the firmware version or diagnostic package.


The auxiliary interface is useful when the project already owns the physical transport.

| Switch                       | Hook                                        | Writer shape                  | Typical use                                                   |
|------------------------------|---------------------------------------------|-------------------------------|---------------------------------------------------------------|
| `TRICE_DIRECT_AUXILIARY8`    | `UserNonBlockingDirectWrite8AuxiliaryFn`    | `const uint8_t*`, byte length | direct byte stream, only if fast/non-blocking                 |
| `TRICE_DEFERRED_AUXILIARY8`  | `UserNonBlockingDeferredWrite8AuxiliaryFn`  | `const uint8_t*`, byte length | most universal first integration                              |
| `TRICE_DIRECT_AUXILIARY32`   | `UserNonBlockingDirectWrite32AuxiliaryFn`   | `const uint32_t*`, word count | fast word-oriented direct sink                                |
| `TRICE_DEFERRED_AUXILIARY32` | `UserNonBlockingDeferredWrite32AuxiliaryFn` | `const uint32_t*`, word count | word-oriented deferred sink; also interesting with encryption |

The deferred 8-bit hook is the safest first quickstart because almost every transport can consume bytes.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">The normal workflow</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### The normal workflow

1. Add the Trice target sources from [`src/`](./src) to your firmware project.
2. Add a project-specific `triceConfig.h`.
3. Write Trice calls in your code.
4. Run `trice insert` before compiling. This assigns stable IDs and updates `til.json` and `li.json`.
5. Build and flash the target.
6. Run `trice log` on the PC to decode the binary stream.

Minimal example:

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

Then start logging, for example from a serial/VCOM port:

```bash
trice log -p COM15 -baud 921600 -i ./til.json -li ./li.json
# or, on Linux/macOS:
trice log -p /dev/ttyACM0 -baud 921600 -i ./til.json -li ./li.json
```

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Data compression and FLASH reduction</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Data compression and FLASH reduction

Replacing format strings with IDs at compile time acts like **strong data compression** for FLASH and transferred log data, while making target-side logging orders of magnitude faster at runtime. The binary log files are also usable for later decoding and decrease the log data amount drastically. 

Existing manual coverage:

* [Small Size - using Trice frees FLASH Memory](./docs/TriceUserManual.md#small-size---using-trice-frees-flash-memory)
* [Minimal Transfer Bytes Amount](./docs/TriceUserManual.md#minimal-transfer-bytes-amount)
* [Trice Memory Needs](./docs/TriceUserManual.md#trice-memory-needs)

Replacing format strings with IDs acts like compile-time compression:

- long format strings do not need target FLASH,
- transferred records contain only ID, optional stamp, and argument values,
- UART/USB/network bandwidth is used for binary data rather than text.

A typical Trice message can be only a few bytes plus parameters, independent of the human-readable format-string length.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">FLASH Memory Storage</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### FLASH Memory Storage

[Store Trice messages in FLASH memory](./docs/TriceUserManual.md#writing-the-trice-logs-into-an-sd-card-(or-a-user-specific-output)) for later analysis. A typical `trice` uses only 4 bytes, no matter how long the format string is.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Encryption</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Encryption

Existing [manual coverage](./docs/TriceUserManual.md#optional-xtea-encryption).

Trice supports optional XTEA encryption for transport packets. This can be useful when field logs should only be readable with the matching key and ID list. Enable encryption in `triceConfig.h` and pass the matching password/key option to `trice log`.

You can **encrypt** Trice transfer packets for security.

- Deliver firmware images with encrypted Trice output that only works with the right key and the matching `til.json`
- [XTEA](https://en.wikipedia.org/wiki/XTEA) encryption is available

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Language switching / translation</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Language switching / translation 

Because the target sends IDs and values, the host-side `til.json` can map an ID to text in different languages. The target binary does not need to change for host-side wording changes, as long as the ID and parameter format remain compatible.

Translate the `til.json` file into **different languages**. Change the language by changing the `til.json` file without changing the target binary. You could use an AI agent for that.

Existing manual coverage: The manual has a [section anchor](./docs/TriceUserManual.md#switch-the-language-without-changing-a-bit-inside-the-target-code).

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Timing Analysis</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Timing Analysis

Trice makes **timing analysis** easy on distributed embedded systems. It supports both host and target timestamps.

Existing manual coverage:

* [Trice Timestamps](./docs/TriceUserManual.md#trice-timestamps)
* [Target Timestamps Formatting](./docs/TriceUserManual.md#target-timestamps-formatting)
* [Target Timestapms Delta Columns](./docs/TriceUserManual.md#target-timestamp-delta-columns)
* [Cycle Counter](./docs/TriceUserManual.md#cycle-counter)

This helps with distributed timing analysis, interrupt timing, and observing target behavior without stopping the firmware in a debugger.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Trice ABC - Asynchronous Broadcast Commands
</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Trice ABC - Asynchronous Broadcast Commands

![Trice ABC overview](./docs/ref/trice_abc_readme_teaser.svg)

**Trice ABC** (*Asynchronous Broadcast Commands*) extends the normal Trice idea from logging to compact command records. A sender emits a normal Trice record, for example:

```c
trice8C("cmd:setLeds", &mask, 1);
```

After `trice insert`, the command name is represented by a numeric ID in `til.json`. With `trice generate -abc`, receiver-side dispatch tables are generated, so each receiver can select which command IDs become local handler calls.

The transport still carries only compact binary data:

```text
ID + optional stamp + optional payload
```

The command string remains in the TIL data. This keeps ABC small, transport-neutral, and suitable for low-bandwidth links, shared buses, and host-native communication demos.

ABC is intentionally **not** a full RPC layer by itself. Addressing, ACKs, retries, authentication, timeouts, and return-value semantics remain application policy.

See also:

* [Trice ABC in the User Manual](./docs/TriceUserManual.md#trice-abc---asynchronous-broadcast-commands)
* [Host-native ABC demo](./examples/TriceAbc)

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">How Trice Works (UART Example)</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### How Trice Works (UART Example)

This simplified [diagram](https://github.com/jgraph/drawio) shows how Trice works. [Read the detailed explanation here](./docs/TriceUserManual.md#how-it-works---the-main-idea).

![trice](docs/ref/triceCOBSBlockDiagram.svg)

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Routing different ID ranges to different Outputs</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Routing different ID ranges to different Outputs

See Trice user manual.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Data Transfer Options</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Data Transfer Options

#### Implemented Transfer Methods

- [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter) - Can connect to virtual UART over USB
- [RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) - Works with [J-Link](./third_party/segger.com/ReadMe.md) and [ST-Link](./third_party/goST/ReadMe.md)
- TCP4 ([TCP4 input](./docs/TriceUserManual.md#tcp4-input) and [TCP4 output](./docs/TriceUserManual.md#tcp4-output))
- UDP4 ([UDP4 input](./docs/TriceUserManual.md#udp4-input))

#### Other Transfer Options

- Use a small separate microcontroller as a bridge from [GPIO](https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/), [I²C](https://en.wikipedia.org/wiki/I%C2%B2C), [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface), [CAN](https://en.wikipedia.org/wiki/CAN_bus), [LIN](https://en.wikipedia.org/wiki/Local_Interconnect_Network) to UART
- Use FTDI chips like [Adafruit FT232H Breakout](https://learn.adafruit.com/adafruit-ft232h-breakout) for easy GPIO, I2C, and SPI access

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Display Server for multiple Targets</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Display Server for multiple Targets

- Start `trice ds` in a console on your local PC or a remote PC. Then connect several **trice** tool instances using commands like `trice log -p COM15 -ds`
- This allows you to see the Trice logs of several devices line-by-line intermixed in one terminal.

Existing manual coverage:

* [Logging Over a Display Server](./docs/TriceUserManual.md#logging-over-a-display-server)
* [Several Targets at the same time](./docs/TriceUserManual.md#several-targets-at-the-same-time)

`trice ds` can act as a display server. Multiple `trice log` instances can send completed log lines to one display server so several targets appear in one combined output.

Example:

```bash
trice ds
trice log -p COM15 -ds
trice log -p COM16 -ds
```

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Documentation</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Documentation

- [Trice User Manual](./docs/TriceUserManual.md)
- [Issues](https://github.com/rokath/trice/issues) including closed items
- [Discussions](https://github.com/rokath/trice/discussions) including closed items
- [Target source configuration defaults](./src/triceDefaultConfig.h)
- [Generated CLI help](./docs/ref/trice-help-all.txt)
- [Examples](./examples)
- Look at and modify [./internal/emitter/lineTransformerANSI.go](./internal/emitter/lineTransformerANSI.go) if needed (requires running `go install ./cmd/trice/...` afterwards)

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Debugging with VS Code and Clang</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Debugging with VS Code and Clang

The example projects show Trice integration with VS Code, Makefiles, Clang/GCC, STM32 projects, RTT, and UART-style output. Use bare/instrumented project pairs to see the minimal integration diff.

Debug a Trice project in Direct-Out Mode over SEGGER-RTT. (See [Development Environment Setup](./docs/TriceUserManual.md#development-environment-setup) for details.)

<img src="docs/ref/Animation.gif" width="1200">

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Trice Cache</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Trice Cache

You can use the `-cache` CLI switch with `trice insert` and `trice clean` commands. This only works when you create the **`.trice/cache`** folder in your home directory. 
See existing [manual coverage](./docs/TriceUserManual.md#trice-cache-for-compilation-speed) dor more details.

The Trice cache avoids unnecessary rebuilds when using `trice insert` and `trice clean` around the build. It keeps cached inserted and cleaned copies of unchanged files.

Typical use:

```bash
mkdir -p ~/.trice/cache
trice insert -cache -src ./ -i ./til.json -li ./li.json
# build
trice clean -cache -src ./ -i ./til.json -li ./li.json
```

#### When to Use Cache

Use cache when you:
- Use `trice i ...` before compiling
- Use `trice c ...` after compiling
- Want to keep IDs out of your source code when working
- Want faster compilation

#### How Cache Works

The Trice cache saves copies of all files after processing them with `trice i` or `trice c`. This avoids inserting and removing IDs repeatedly. **The copies are used to get the same results for files that have not been edited.** Edited files are processed normally and the cache updates afterwards. File modification times do not change, so **the build system does not reprocess unchanged files even when IDs are temporarily removed**.

#### Important Note

```diff
-> Be careful when your build system also modifies source files!
```

For example, run an auto-formatter **before** the `trice insert` command.

---

</details>


<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Project Status</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Project Status

Trice is usable today and actively maintained. Use the latest release or the `main` branch when building from source. Avoid development branches unless you intentionally want unreleased work.

Contributions are welcome: examples, platform recipes, transport backends, documentation improvements, and reproducible benchmark reports are especially valuable.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Future Plans</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Future Ideas

Potential future work includes:

- a small `tlog` tool in C or Python would allow logging on any platform, not just platforms supported by **Go**
- more structured logging support: A [specification draft](./docs/TriceUserManual.md#trice-structured-logging) for **structured logging** in Trice is available, based on feedback from [#531](https://github.com/rokath/trice/discussions/531). 
- additional transport recipes
- Adaption for a full visual RTOS/event timeline tool.

---

</details>


<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Legacy code integration notes</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

## Legacy code integration notes

Existing manual coverage: See [Trice and legacy User Code](.docs/TriceUserManual.md#trice-and-legacy-user-code), including separate physical output, manual adaptation, print wrapping/framing, and aliases.

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

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Installation</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---


## Installation

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

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Support the Project</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Support the Project

- :star: Star this project! ☺
- Contribute to the project.  ☺ ☺
- If Trice helps a commercial product, consider supporting continued maintenance through the sponsor links on the GitHub project page.
  - Support options:
    - [Become a Sponsor with your GitHub account](https://github.com/sponsors/rokath/)
    - <a href="https://buymeacoffee.com/rokath" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="30" width="120"></a>
    - <a href="https://www.paypal.com/paypalme/rolfkarlthomas"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif" alt="PayPal" height="40" width="120"></a>

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Similar Projects</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Similar Projects

Trice overlaps with several logging, tracing, and tokenization approaches, but it targets a specific niche: compact printf-like logging for embedded C/C++ with host-side decoding.

### Useful comparison axes

| Axis              | Questions to ask                                                                        |
|-------------------|-----------------------------------------------------------------------------------------|
| Target language   | C/C++, Rust, mixed, generated code?                                                     |
| Target overhead   | Does a log call format text, copy a string pointer, write an ID, or use hardware trace? |
| Transport         | UART, RTT, SWO, USB, network, file, field storage?                                      |
| Host dependency   | Open tool, vendor tool, cloud, debugger integration?                                    |
| Field diagnostics | Can old binary logs be decoded after firmware release?                                  |
| Ecosystem         | Standalone library, RTOS-integrated logger, vendor debug suite, cloud product?          |


> Trice is in the same broad problem space as `printf`, SEGGER RTT/SystemView, ARM ITM/SWO, Zephyr dictionary logging, Rust `defmt`, Pigweed tokenized logging, and Memfault compact logs, but focuses on lightweight printf-like embedded C/C++ logging with host-side ID decoding.


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
- [Zephyr Dictionary Based Logging](https://docs.zephyrproject.org/3.1.0/services/logging/index.html#dictionary-based-logging)
- Rust `defmt`
- project-specific binary loggers.

<!-- - [Metal Serial Library](https://github.com/metal-ci/_test/tree/master/doc/metal.serial.md) -->

Additional comparison material:

- [Trice Compare (generated 2025-05-26)](https://htmlpreview.github.io/?https://github.com/rokath/trice/blob/main/docs/ChatGPTo4-mini-high_TriceCompare.html) - compact generated comparison with related tools
- [Logging & Tracing Solutions for Embedded Systems (generated 2026-02-16)](https://htmlpreview.github.io/?https://github.com/rokath/trice/blob/main/docs/2026-02-16_ChatGPT5.2ProExtThinking_embedded_logging_tracing_comparison_trice_focus.html) - longer generated overview of the broader tool landscape

---

</details>

<p align="right">(<a href="#top">back to top</a>)</p>
