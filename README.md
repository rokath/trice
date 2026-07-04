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

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.25em;">Overview</strong> <span style="font-size: 0.66em;">(click to expand)</span></summary>

<!-- mdtoc -->

- [What is Trice?](#what-is-trice)
- [How it works](#how-it-works)
- [Key Benefits](#key-benefits)
- [Two Parts of Trice](#two-parts-of-trice)
- [Quickstart: choose your first path](#quickstart-choose-your-first-path)
  - [Which Mode Should You Use?](#which-mode-should-you-use)
- [Further Information](#further-information)
  - [Logging and Debugging](#logging-and-debugging)
  - [Data Compression](#data-compression)
  - [FLASH Memory Storage](#flash-memory-storage)
  - [Encryption](#encryption)
  - [Output Translation in different languages](#output-translation-in-different-languages)
  - [Timing Analysis](#timing-analysis)
  - [Trice ABC - Asynchronous Broadcast Commands](#trice-abc---asynchronous-broadcast-commands)
  - [How Trice Works (UART Example)](#how-trice-works-uart-example)
  - [Routing different ID ranges to different Outputs](#routing-different-id-ranges-to-different-outputs)
  - [Data Transfer Options](#data-transfer-options)
    - [Implemented Transfer Methods](#implemented-transfer-methods)
    - [Other Transfer Options](#other-transfer-options)
  - [Display Server for multiple Targets](#display-server-for-multiple-targets)
  - [Documentation](#documentation)
  - [Debugging with VS Code and Clang](#debugging-with-vs-code-and-clang)
  - [Trice Cache](#trice-cache)
    - [When to Use Cache](#when-to-use-cache)
    - [How Cache Works](#how-cache-works)
    - [Important Note](#important-note)
  - [Project Status](#project-status)
  - [Future Plans](#future-plans)
  - [Support the Project](#support-the-project)
  - [Similar Projects](#similar-projects)

<!-- numbering=false min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## <a id="what-is-trice"></a>What is Trice?

**Trice** is under the hood an ID-based (ultra-low-overhead) logging framework for embedded C/C++. It provides printf-like usability with only a few (down to 6!) CPU cycles per log call, making logging practical even when used from interrupt handlers in real-time firmware systems.

Trice is designed for systems where traditional logging is too slow, too large, or not safe to use in interrupt or real-time contexts.

Trice replaces/re-assigns legacy `printf` logging in embedded C/C++ systems with a much faster and more efficient approach.

Instead of formatting and storing strings on the target, Trice encodes log messages as compact IDs and keeps the actual strings on the host. This reduces runtime overhead, memory usage, and data transfer size.


## <a id="how-it-works"></a>How it works

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

## <a id="key-benefits"></a>Key Benefits

* **[Good for interrupts and timing work](./docs/TriceUserManual.md#trice-speed)**: Needs only a few CPU cycles in the most optimized configurations.
* **[Smaller Binaries](./docs/TriceUserManual.md#trice-memory-needs)**: Format strings are kept in `til.json`, not in target FLASH.
* **Minimal log data streams**: A Trice message is one 32-bit value plus optional runtime user values.
* **[Long-term field decoding](./docs/TriceUserManual.md#versions-and-variants-trice-stability)** – decode logs from older firmware any newer til.json.
* **[Easy Migration](./docs/TriceUserManual.md#trice-and-legacy-user-code)** – reuse existing `printf`-style code with minimal changes via the `-alias` option
* **Portable tooling** - the host tool is written in Go and runs on common desktop platforms.
* **[➕ More Features](./docs/TriceUserManual.md#trice-features-overview)** – flexible logging, transport options, and tooling.
* **Fully documented**: [Trice User Manual](./docs/TriceUserManual.md)

## <a id="two-parts-of-trice"></a>Two Parts of Trice

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

## <a id="quickstart-choose-your-first-path"></a>Quickstart: choose your first path 

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

### <a id="which-mode-should-you-use"></a>Which Mode Should You Use?

- **For development:** Direct mode with SEGGER_RTT is recommended
- **For most use cases:** Deferred mode with TRICE_BUFFER == TRICE_RING_BUFFER (uses less RAM) in TRICE_MULTI_PACK_MODE (transfers less data)

---

</details>

## <a id="further-information"></a>Further Information

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Logging and Debugging</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="logging-and-debugging"></a>Logging and Debugging

You can use Trice for `printf` debugging and as a logging system. The advantage is very short messages (no strings) for data transfer. That can be handy, when you have heavy log loads. Keep your project-specific `til.json` file available to decode field logs later. The repository example file is [demoTIL.json](./demoTIL.json).

- **Optional:** Add your project-specific `til.json` as a compressed resource to your target image. You can use [SRecord](https://srecord.sourceforge.net/download.html) or provide a download link.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Direct mode and deferred mode</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### Direct mode and deferred mode

Trice has two main runtime styles.

| Mode | What happens during the log call | Good for |
| --- | --- | --- |
| **Direct mode** | Each Trice record is written immediately to the selected output backend. | Interactive development, especially SEGGER RTT. |
| **Deferred mode** | The log call writes only into a target buffer; `TriceTransfer()` sends later. | UART, USB CDC, existing writer functions, production-style logging, shortest target execution path. |

A practical rule:

- Start with **direct SEGGER RTT** when you have J-Link and want the easiest lab setup.
- Start with **deferred auxiliary 8-bit output** when your project already has a write function.
- Use **deferred ring buffer** for a balanced RAM/speed setup.
- Use **deferred double buffer** when minimizing the target-side log-call cost is more important than RAM.

See [mode and transport selection](./docs/TriceUserManualExtension.md#mode-and-transport-selection) for more detail.


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
<summary><strong style="font-size: 1.00em;">Data Compression</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="data-compression"></a>Data Compression

Replacing format strings with IDs at compile time acts like **strong data compression** for FLASH and transferred log data, while making target-side logging orders of magnitude faster at runtime. The binary log files are also usable for later decoding and decrease the log data amount drastically. 

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">FLASH Memory Storage</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="flash-memory-storage"></a>FLASH Memory Storage

[Store Trice messages in FLASH memory](./docs/TriceUserManual.md#writing-the-trice-logs-into-an-sd-card-(or-a-user-specific-output)) for later analysis. A typical `trice` uses only 4 bytes, no matter how long the format string is.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Encryption</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="encryption"></a>Encryption

You can **encrypt** Trice transfer packets for security.

- Deliver firmware images with encrypted Trice output that only works with the right key and the matching `til.json`
- [XTEA](https://en.wikipedia.org/wiki/XTEA) encryption is available

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Translation</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="output-translation-in-different-languages"></a>Output Translation in different languages 

Translate the `til.json` file into **different languages**. Change the language by changing the `til.json` file without changing the target binary. You could use an AI agent for that.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Timing Analysis</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="timing-analysis"></a>Timing Analysis

Trice makes **timing analysis** easy on distributed embedded systems. It supports both host and target timestamps.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Trice ABC - Asynchronous Broadcast Commands
</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="trice-abc---asynchronous-broadcast-commands"></a>Trice ABC - Asynchronous Broadcast Commands

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

### <a id="how-trice-works-uart-example"></a>How Trice Works (UART Example)

This simplified [diagram](https://github.com/jgraph/drawio) shows how Trice works. [Read the detailed explanation here](./docs/TriceUserManual.md#how-it-works---the-main-idea).

![trice](docs/ref/triceCOBSBlockDiagram.svg)

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Routing different ID ranges to different Outputs</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="routing-different-id-ranges-to-different-outputs"></a>Routing different ID ranges to different Outputs

See Trice user manual.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Data Transfer Options</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="data-transfer-options"></a>Data Transfer Options

#### <a id="implemented-transfer-methods"></a>Implemented Transfer Methods

- [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter) - Can connect to virtual UART over USB
- [RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) - Works with [J-Link](./third_party/segger.com/ReadMe.md) and [ST-Link](./third_party/goST/ReadMe.md)
- TCP4 ([TCP4 input](./docs/TriceUserManual.md#tcp4-input) and [TCP4 output](./docs/TriceUserManual.md#tcp4-output))
- UDP4 ([UDP4 input](./docs/TriceUserManual.md#udp4-input))

#### <a id="other-transfer-options"></a>Other Transfer Options

- Use a small separate microcontroller as a bridge from [GPIO](https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/), [I²C](https://en.wikipedia.org/wiki/I%C2%B2C), [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface), [CAN](https://en.wikipedia.org/wiki/CAN_bus), [LIN](https://en.wikipedia.org/wiki/Local_Interconnect_Network) to UART
- Use FTDI chips like [Adafruit FT232H Breakout](https://learn.adafruit.com/adafruit-ft232h-breakout) for easy GPIO, I2C, and SPI access

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Display Server for multiple Targets</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="display-server-for-multiple-targets"></a>Display Server for multiple Targets

- Start `trice ds` in a console on your local PC or a remote PC. Then connect several **trice** tool instances using commands like `trice log -p COM15 -ds`
- This allows you to see the Trice logs of several devices line-by-line intermixed in one terminal.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Documentation</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="documentation"></a>Documentation

The [Trice User Manual](./docs/TriceUserManual.md) includes all information from the [Memfault Interrupt Blog](https://interrupt.memfault.com/blog/trice) which is slightly outdated.
- Check [issues](https://github.com/rokath/trice/issues) and [discussions](https://github.com/rokath/trice/discussions), including closed items
- Read the target source code, especially [triceDefaultConfig.h](./src/triceDefaultConfig.h)
- View [CLI](https://en.wikipedia.org/wiki/Command-line_interface) options by running `trice help -all` in a terminal or reading the generated file [trice-help-all.txt](./docs/ref/trice-help-all.txt)
- Refresh the checked-in CLI help documentation with `go generate ./internal/args`
- Look at and modify [./internal/emitter/lineTransformerANSI.go](./internal/emitter/lineTransformerANSI.go) if needed (requires running `go install ./cmd/trice/...` afterwards)
- [Examples](./examples)

<!--
- [Trice User Manual](./docs/TriceUserManual.md)
- [Quickstarts and moved README material](./docs/TriceUserManualExtension.md)
- [Generated CLI help](./docs/ref/trice-help-all.txt)
- [Target source configuration defaults](./src/triceDefaultConfig.h)
-->

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Debugging with VS Code and Clang</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="debugging-with-vs-code-and-clang"></a>Debugging with VS Code and Clang

Debug a Trice project in Direct-Out Mode over SEGGER-RTT. (See [Development Environment Setup](./docs/TriceUserManual.md#development-environment-setup) for details.)

<img src="docs/ref/Animation.gif" width="1200">

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Trice Cache</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="trice-cache"></a>Trice Cache

You can use the `-cache` CLI switch with `trice insert` and `trice clean` commands. This only works when you create the **`.trice/cache`** folder in your home directory. ([Trice Cache Details](./docs/TriceUserManual.md#trice-cache-for-compilation-speed))

#### <a id="when-to-use-cache"></a>When to Use Cache

Use cache when you:
- Use `trice i ...` before compiling
- Use `trice c ...` after compiling
- Want to keep IDs out of your source code when working
- Want faster compilation

#### <a id="how-cache-works"></a>How Cache Works

The Trice cache saves copies of all files after processing them with `trice i` or `trice c`. This avoids inserting and removing IDs repeatedly. **The copies are used to get the same results for files that have not been edited.** Edited files are processed normally and the cache updates afterwards. File modification times do not change, so **the build system does not reprocess unchanged files even when IDs are temporarily removed**.

#### <a id="important-note"></a>Important Note

```diff
-> Be careful when your build system also modifies source files!
```

For example, run an auto-formatter **before** the `trice insert` command.

---

</details>


<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Project Status</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="project-status"></a>Project Status

Trice is usable today and actively maintained. Use the latest release or the `main` branch when building from source. Avoid development branches unless you intentionally want unreleased work.

Contributions are welcome: examples, platform recipes, transport backends, documentation improvements, and reproducible benchmark reports are especially valuable.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Future Plans</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="future-plans"></a>Future Plans

- A **separate tlog tool** written in C or Python would allow logging on any platform, not just platforms supported by **Go**.
- A [specification draft](./docs/TriceUserManual.md#trice-structured-logging) for **structured logging** in Trice is available, based on feedback from [#531](https://github.com/rokath/trice/discussions/531). 
- Adaption for a full visual RTOS/event timeline tool.

---

</details>

<details markdown="1"> <!-- parse this block as markdown -->
<summary><strong style="font-size: 1.00em;">Support the Project</strong><span style="font-size: 0.66em;">(click to expand)</span></summary>

---

### <a id="support-the-project"></a>Support the Project

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

### <a id="similar-projects"></a>Similar Projects

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
- [SEGGER System View](https://www.segger.com/products/development-tools/systemview/technology/what-is-systemview/)
- [Serial-Studio](https://github.com/Serial-Studio/Serial-Studio) (Data visualisation)
- [Tonbandgerät](https://github.com/schilkp/Tonbandgeraet) (Small embedded systems tracer with support for bare-metal and FreeRTOS-based targets)
- [Traces](https://github.com/yotamr/traces) (API tracing framework for Linux C/C++ applications)
- [uLog (RD Poor)](https://github.com/rdpoor/ulog)
- [Zephyr Dictionary Based Logging](https://docs.zephyrproject.org/3.1.0/services/logging/index.html#dictionary-based-logging)

<!-- - [Metal Serial Library](https://github.com/metal-ci/_test/tree/master/doc/metal.serial.md) -->

Additional comparison material:

- [Trice Compare (generated 2025-05-26)](https://htmlpreview.github.io/?https://github.com/rokath/trice/blob/main/docs/ChatGPTo4-mini-high_TriceCompare.html) - compact generated comparison with related tools
- [Logging & Tracing Solutions for Embedded Systems (generated 2026-02-16)](https://htmlpreview.github.io/?https://github.com/rokath/trice/blob/main/docs/2026-02-16_ChatGPT5.2ProExtThinking_embedded_logging_tracing_comparison_trice_focus.html) - longer generated overview of the broader tool landscape

---

</details>

<p align="right">(<a href="#top">back to top</a>)</p>
