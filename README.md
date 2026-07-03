# Trice - Trace IDs for Embedded C/C++ 

> 🅃🅁ace 🄸d's for 🄲 🄴mbedded

[View GitHub Pages](https://rokath.github.io/trice/)

<a id="top"> </a>

<!-- mdtoc off -->
## ![TriceGirlS.png](docs/ref/TriceGirl-167x222.png) *Hi, I am Trice.*
<!-- mdtoc on -->
---

**Trice** is an ultra-low-overhead logging framework for embedded C/C++. It provides printf-like usability with only ~6–100 CPU cycles per log call, making logging practical even when used from interrupt handlers in real-time firmware systems.

![License](https://img.shields.io/github/license/rokath/trice) ![GitHub release (latest by date)](https://img.shields.io/github/v/release/rokath/trice) ![GitHub commits since latest release](https://img.shields.io/github/commits-since/rokath/trice/latest) ![Downloads](https://img.shields.io/github/downloads/rokath/trice/total) ![GitHub issues](https://img.shields.io/github/issues/rokath/trice) [![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](https://makeapullrequest.com) ![Go Version](https://img.shields.io/github/go-mod/go-version/rokath/trice)  [![Go Report Card](https://goreportcard.com/badge/github.com/rokath/trice)](https://goreportcard.com/report/github.com/rokath/trice) [![Coverage](https://coveralls.io/repos/github/rokath/trice/badge.svg?branch=main)](https://coveralls.io/github/rokath/trice?branch=main) [![TRICE Library CI (Nightly Full)](https://github.com/rokath/trice/actions/workflows/trice_lib_ci_full.yml/badge.svg)](https://github.com/rokath/trice/actions/workflows/trice_lib_ci_full.yml) 
<!--
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
  - [Key Benefits](#key-benefits)
  - [How it works](#how-it-works)
  - [Result](#result)
  - [Two Parts of Trice](#two-parts-of-trice)
- [Quickstart: choose your first path](#quickstart-choose-your-first-path)
- [When to Use Trice](#when-to-use-trice)
  - [Logging and Debugging](#logging-and-debugging)
  - [Data Compression](#data-compression)
  - [FLASH Memory Storage](#flash-memory-storage)
  - [Encryption](#encryption)
  - [Translation](#translation)
  - [Timing Analysis](#timing-analysis)
  - [Trice ABC - Asynchronous Broadcast Commands](#trice-abc---asynchronous-broadcast-commands)
- [How Trice Works (UART Example)](#how-trice-works-uart-example)
- [Data Transfer Options](#data-transfer-options)
  - [Implemented Transfer Methods](#implemented-transfer-methods)
  - [Other Transfer Options](#other-transfer-options)
- [Display Server](#display-server)
- [Documentation](#documentation)
- [Debugging with VS Code and Clang](#debugging-with-vs-code-and-clang)
- [Trice Cache](#trice-cache)
  - [When to Use Cache](#when-to-use-cache)
  - [How Cache Works](#how-cache-works)
  - [Important Note](#important-note)
- [Which Mode Should You Use?](#which-mode-should-you-use)
- [Project Status](#project-status)
- [Future Plans](#future-plans)
- [Support the Project](#support-the-project)
- [Similar Projects](#similar-projects)

<!-- numbering=false min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## <a id="what-is-trice"></a>What is Trice?

Trice is designed for systems where traditional logging is too slow, too large, or not safe to use in interrupt or real-time contexts.

Trice replaces `printf`-style logging in embedded C/C++ systems with a much faster and more efficient approach.

Instead of formatting and storing strings on the target, Trice encodes log messages as compact IDs and keeps the actual strings on the host. This reduces runtime overhead, memory usage, and data transfer size.

### <a id="key-benefits"></a>Key Benefits

* **[🚀 Speed](./docs/TriceUserManual.md#trice-speed)** – ~6–100 CPU cycles per log call. This makes logging practical even when used from interrupt handlers in real-time firmware systems.
* **[📦 Small Size](./docs/TriceUserManual.md#trice-memory-needs)** – no format strings stored in target FLASH  
* **[🧱 Version Stability](./docs/TriceUserManual.md#versions-and-variants-trice-stability)** – decode logs from older firmware without requiring matching tool versions
* **[🛠 Easy Migration](./docs/TriceUserManual.md#trice-and-legacy-user-code)** – reuse existing `printf`-style code with minimal changes via the `-alias` option
* **[➕ More Features](./docs/TriceUserManual.md#trice-features-(overview))** – flexible logging, transport options, and tooling  

### <a id="how-it-works"></a>How it works

1. Use Trice macros instead of `printf` in your firmware (or use the `trice i -alias` option).
2. At build time, Trice assigns each log message a compact ID while the user source code stays unchanged.
3. The target device sends only IDs and data.
4. The host reconstructs the original messages using the Trice ID list.

```diff
- printf("Temperature: %d°C", t);
+ trice("Temperature: %d°C", t);   // fast, compact, ID-based
```

### <a id="result"></a>Result

* Faster execution
* Smaller binaries
* Reliable logging in time-critical code paths

### <a id="two-parts-of-trice"></a>Two Parts of Trice

1. **C code macros** - Provide a familiar `printf`-like interface at the application level, but internally send just ID and values instead of full format strings. The Trice tool maps these IDs back to readable text.
2. **Trice tool** - Manages and displays the logs
   - Written in [Go](https://go.dev/) - works on all platforms that Go supports
   - You can also build your own tool to receive Trice packages, replace IDs with text, and display the output

***Ready to use:*** [Start with Trice](./docs/TriceUserManual.md#start-with-trice)

## <a id="quickstart-choose-your-first-path"></a>Quickstart: choose your first path 

![Trice log icon](./docs/ref/trice_log_icon_128x128.png)

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

## <a id="when-to-use-trice"></a>When to Use Trice

### <a id="logging-and-debugging"></a>Logging and Debugging

You can use Trice for `printf` debugging and as a logging system. The advantage is very short messages (no strings) for data transfer. Keep your project-specific `til.json` file available to decode field logs later. The repository example file is [demoTIL.json](./demoTIL.json).

- **Optional:** Add your project-specific `til.json` as a compressed resource to your target image. You can use [SRecord](https://srecord.sourceforge.net/download.html) or provide a download link.

### <a id="data-compression"></a>Data Compression

Replacing format strings with IDs at compile time acts like **strong data compression** for FLASH and transferred log data, while making target-side logging orders of magnitude faster at runtime.

### <a id="flash-memory-storage"></a>FLASH Memory Storage

[Store Trice messages in FLASH memory](./docs/TriceUserManual.md#writing-the-trice-logs-into-an-sd-card-(or-a-user-specific-output)) for later analysis. A typical `trice` uses only 4 bytes, no matter how long the format string is.

### <a id="encryption"></a>Encryption

You can **encrypt** Trice transfer packets for security.

- Deliver firmware images with encrypted Trice output that only works with the right key and the matching `til.json`
- [XTEA](https://en.wikipedia.org/wiki/XTEA) encryption is available

### <a id="translation"></a>Translation

Translate the `til.json` file into **different languages**. Change the language by changing the `til.json` file without changing the target binary.

### <a id="timing-analysis"></a>Timing Analysis

Trice makes **timing analysis** easy on distributed embedded systems. It supports both host and target timestamps.

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

## <a id="how-trice-works-uart-example"></a>How Trice Works (UART Example)

This simplified [diagram](https://github.com/jgraph/drawio) shows how Trice works. [Read the detailed explanation here](./docs/TriceUserManual.md#how-it-works---the-main-idea).

![trice](docs/ref/triceCOBSBlockDiagram.svg)

## <a id="data-transfer-options"></a>Data Transfer Options

### <a id="implemented-transfer-methods"></a>Implemented Transfer Methods

- [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter) - Can connect to virtual UART over USB
- [RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) - Works with [J-Link](./third_party/segger.com/ReadMe.md) and [ST-Link](./third_party/goST/ReadMe.md)
- TCP4 ([TCP4 input](./docs/TriceUserManual.md#tcp4-input) and [TCP4 output](./docs/TriceUserManual.md#tcp4-output))
- UDP4 ([UDP4 input](./docs/TriceUserManual.md#udp4-input))

### <a id="other-transfer-options"></a>Other Transfer Options

- Use a small separate microcontroller as a bridge from [GPIO](https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/), [I²C](https://en.wikipedia.org/wiki/I%C2%B2C), [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface), [CAN](https://en.wikipedia.org/wiki/CAN_bus), [LIN](https://en.wikipedia.org/wiki/Local_Interconnect_Network) to UART
- Use FTDI chips like [Adafruit FT232H Breakout](https://learn.adafruit.com/adafruit-ft232h-breakout) for easy GPIO, I2C, and SPI access

## <a id="display-server"></a>Display Server

- Start `trice ds` in a console on your local PC or a remote PC. Then connect several **trice** tool instances using commands like `trice log -p COM15 -ds`
- This allows you to see the Trice logs of several devices line-by-line intermixed in one terminal.

## <a id="documentation"></a>Documentation

The [Trice User Manual](./docs/TriceUserManual.md) includes all information from the [Memfault Interrupt Blog](https://interrupt.memfault.com/blog/trice) which is slightly outdated.
- Check [issues](https://github.com/rokath/trice/issues) and [discussions](https://github.com/rokath/trice/discussions), including closed items
- Read the target source code, especially [triceDefaultConfig.h](./src/triceDefaultConfig.h)
- View [CLI](https://en.wikipedia.org/wiki/Command-line_interface) options by running `trice help -all` in a terminal or reading the generated file [trice-help-all.txt](./docs/ref/trice-help-all.txt)
- Refresh the checked-in CLI help documentation with `go generate ./internal/args`
- Look at and modify [./internal/emitter/lineTransformerANSI.go](./internal/emitter/lineTransformerANSI.go) if needed (requires running `go install ./cmd/trice/...` afterwards)

## <a id="debugging-with-vs-code-and-clang"></a>Debugging with VS Code and Clang

Debug a Trice project in Direct-Out Mode over SEGGER-RTT. (See [Development Environment Setup](./docs/TriceUserManual.md#development-environment-setup) for details.)

<img src="docs/ref/Animation.gif" width="1200">

## <a id="trice-cache"></a>Trice Cache

<details markdown="1"> <!-- parse this block as markdown -->
<summary>(click to expand)</summary>

---

You can use the `-cache` CLI switch with `trice insert` and `trice clean` commands. This only works when you create the **`.trice/cache`** folder in your home directory. ([Trice Cache Details](./docs/TriceUserManual.md#trice-cache-for-compilation-speed))

### <a id="when-to-use-cache"></a>When to Use Cache

Use cache when you:
- Use `trice i ...` before compiling
- Use `trice c ...` after compiling
- Want to keep IDs out of your source code when working
- Want faster compilation

### <a id="how-cache-works"></a>How Cache Works

The Trice cache saves copies of all files after processing them with `trice i` or `trice c`. This avoids inserting and removing IDs repeatedly. **The copies are used to get the same results for files that have not been edited.** Edited files are processed normally and the cache updates afterwards. File modification times do not change, so **the build system does not reprocess unchanged files even when IDs are temporarily removed**.

### <a id="important-note"></a>Important Note

```diff
-> Be careful when your build system also modifies source files!
```

For example, run an auto-formatter **before** the `trice insert` command.

---

</details>

## <a id="which-mode-should-you-use"></a>Which Mode Should You Use?

- **For development:** Direct mode with SEGGER_RTT is recommended
- **For most use cases:** Deferred mode with TRICE_BUFFER == TRICE_RING_BUFFER (uses less RAM) in TRICE_MULTI_PACK_MODE (transfers less data)


## <a id="project-status"></a>Project Status

Trice is fully usable.

```diff
+ Use the latest release or main branch to build from source.
- Do not use the "dev" branch. It may not work properly.
```

## <a id="future-plans"></a>Future Plans

- The documentation could better explain advanced use cases, such as **deferred remote procedure calls**.
- A **separate tlog tool** written in C or Python would allow logging on any platform, not just platforms supported by **Go**.
- A [draft specification](./docs/TriceUserManual.md#trice-structured-logging) for **structured logging** in Trice is available, based on feedback from [#531](https://github.com/rokath/trice/discussions/531).
- Proposals and feedback on these topics are welcome.

## <a id="support-the-project"></a>Support the Project

- Trice takes a lot of my free time. I want to keep it MIT licensed in the future.
- If you make profit using Trice in your products, donations help convince my family to continue improving Trice.
- :star: Star this project! ☺
- Support options:
  - [Become a Sponsor with your GitHub account](https://github.com/sponsors/rokath/)
  - <a href="https://buymeacoffee.com/rokath" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="30" width="120"></a>
  - <a href="https://www.paypal.com/paypalme/rolfkarlthomas"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif" alt="PayPal" height="40" width="120"></a>

## <a id="similar-projects"></a>Similar Projects

<details markdown="1"> <!-- parse this block as markdown -->
<summary>(click to expand)</summary>

---

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

</details>

<p align="right">(<a href="#top">back to top</a>)</p>
