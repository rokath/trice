# 🅃🅁ace 🄸d's 🄲 🄴mbedded

<a id="top"> </a>

![TriceGirlS.png](docs/ref/TriceGirl-167x222.png) **Hi, I am Trice.**

<!-- [github pages](https://rokath.github.io/trice/) -->
[Trice User Manual](https://rokath.github.io/trice/docs/TriceUserManual.html)

<!-- ![GitHub Workflow Status](https://img.shields.io/github/workflow/status/rokath/trice/goreleaser) -->
![GitHub issues](https://img.shields.io/github/issues/rokath/trice)
![GitHub All Releases](https://img.shields.io/github/downloads/rokath/trice/total)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/rokath/trice)
![GitHub watchers](https://img.shields.io/github/watchers/rokath/trice?label=watch)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/rokath/trice)
![GitHub commits since latest release](https://img.shields.io/github/commits-since/rokath/trice/latest)
[![Go Report Card](https://goreportcard.com/badge/github.com/rokath/trice)](https://goreportcard.com/report/github.com/rokath/trice)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com)
[![Coverage Status](https://coveralls.io/repos/github/rokath/trice/badge.svg?branch=main)](https://coveralls.io/github/rokath/trice?branch=main)
[![CodeQL](https://github.com/rokath/trice/actions/workflows/codeql.yml/badge.svg)](https://github.com/rokath/trice/actions/workflows/codeql.yml)

<!-- [![Sponsor rokath](https://github.com/sponsors/rokath/button)](https://github.com/sponsors/rokath) -->
<!-- [![test](https://github.com/shogo82148/actions-goveralls/workflows/_test/badge.svg?branch=main)](https://coveralls.io/github/rokath/trice) -->

Log in (a) trice ([S>G](https://www.screentogif.com/)) ![ ](./docs/ref/life0.gif)

even inside **↯ interrupts** in less than 1 µs❗

## What is Trice?

Trice replaces `printf` or `log` in **C** code and gives you three main benefits:

- **[Speed](./docs/TriceUserManual.md#trice-speed)** - Fast enough to use inside interrupts
- **[Small Size](./docs/TriceUserManual.md#trice-memory-needs)** - Uses less FLASH memory
- **[More Features](./docs/TriceUserManual.md#trice-features-(overview))** - Extra options that help developers

### Main Idea

Instead of storing log strings on your embedded device, Trice keeps them on your PC. This makes logging faster and uses less memory on your device.

```diff
- No version mismatch problems❗
+ The Trice ID List stores all log strings, so the
+ newest version can read logs from all older versions❗
```

### Two Parts of Trice

1. **C code macros** - Works like `printf` but creates very fast trace and log code for your embedded device
2. **Trice tool** - Manages and displays the logs
   - Written in [Go](https://golang.org/) - works on all platforms that Go supports
   - You can also build your own tool to receive Trice packages, replace IDs with text, and display the output

- [Read the Trice User Manual](./docs/TriceUserManual.md)
- Current Status: **_Trice is ready to use!_**

## When to Use Trice

### Logging and Debugging
You can use Trice for `printf` debugging and as a logging system. The advantage is very short messages (no strings) for data transfer. Remember that the file [til.json](./demoTIL.json) is needed to read all output from devices in the field for 10+ years.

- **Optional:** Add [til.json](./demoTIL.json) as a compressed resource to your target image. You can use [SRecord](http://srecord.sourceforge.net/download.html) or provide a download link.

### Data Compression
Trice looks like data compression (IDs instead of strings), which is useful for [IoT](https://en.wikipedia.org/wiki/Internet_of_things) devices, especially [NB-IoT](https://en.wikipedia.org/wiki/Narrowband_IoT) with very low data rates.

### FLASH Memory Storage
[Store Trice messages in FLASH memory](./docs/TriceUserManual.md#writing-the-trice-logs-into-an-sd-card-(or-a-user-specific-output)) for later analysis. A typical `trice` uses only 4 bytes, no matter how long the format string is.

### Encryption
You can **encrypt** Trice transfer packets for security.

- Deliver firmware images with encrypted Trice output that only works with the right key and [til.json](./demoTIL.json)
- [XTEA](https://en.m.wikipedia.org/wiki/XTEA) encryption is available

### Translation
Translate the [til.json](./demoTIL.json) file into **different languages**. Change the language by changing the [til.json](./demoTIL.json) file without changing the target binary.

### Timing Analysis
Trice makes **timing analysis** easy on distributed embedded systems. It supports both host and target timestamps.

## How Trice Works (UART Example)

This simplified [diagram](https://github.com/jgraph/drawio) shows how Trice works. [Read the detailed explanation here](./docs/TriceUserManual.md#how-it-works---the-main-idea).

![trice](docs/ref/triceCOBSBlockDiagram.svg)

## Data Transfer Options

### Implemented Transfer Methods

- [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter) - Can connect to virtual UART over USB
- [RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) - Works with [J-Link](./third_party/segger.com/ReadMe.md) and [ST-Link](./third_party/goST/ReadMe.md)
- TCP4 ([TCP4 input](./docs/TriceUserManual.md#tcp4-input) and [TCP4 output](./docs/TriceUserManual.md#tcp4-output))
- UDP4 ([UDP4 input](./docs/TriceUserManual.md#udp4-input))

### Other Transfer Options

- Use a small separate microcontroller as a bridge from [GPIO](https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/), [I²C](https://en.wikipedia.org/wiki/I%C2%B2C), [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface), [CAN](https://en.wikipedia.org/wiki/CAN_bus), [LIN](https://en.wikipedia.org/wiki/Local_Interconnect_Network) to UART
- Use FTDI chips like [Adafruit FT232H Breakout](https://learn.adafruit.com/adafruit-ft232h-breakout) for easy GPIO, I2C, and SPI access

## Display Server

- Start `trice ds` in a console on your local PC or a remote PC. Then connect several **trice** tool instances using commands like `trice log -p COM15 -ds`
- This allows to see the Trice logs of several devices line-by-line intermixed in one terminal.

## Documentation

- [https://interrupt.memfault.com/blog/trice](https://interrupt.memfault.com/blog/trice) (slightly outdated)
- [Trice User Manual](./docs/TriceUserManual.md) (includes updated information from the interrupt blog)
- Check [issues](https://github.com/rokath/trice/issues) and [discussions](https://github.com/rokath/trice/discussions), including closed items
- Read the target source code, especially [triceDefaultConfig.h](./src/triceDefaultConfig.h)
- View [CLI](https://en.wikipedia.org/wiki/Command-line_interface) options by running `trice help -all` in a terminal or reading [tricehelpall_test.go](./internal/args/tricehelpall_test.go)
- Look at and modify [./internal/emitter/lineTransformerANSI.go](./internal/emitter/lineTransformerANSI.go) if needed (requires running `go install ./cmd/trice/...` afterwards)

## Debugging with VS-Code and Clang

Debug a Trice project in Direct-Out Mode over SEGGER-RTT. (See [Development Environment Setup](./docs/TriceUserManual.md#development-environment-setup) for details.)

<img src="docs/ref/Animation.gif" width="1200">

## Trice Cache

You can use the `-cache` CLI switch with `trice insert` and `trice clean` commands. This only works when you create the **`.trice/cache`** folder in your home directory. ([Trice Cache Details](./docs/TriceUserManual.md#trice-cache-for-compilation-speed))

### When to Use Cache

Use cache when you:
- Use `trice i ...` before compiling
- Use `trice c ...` after compiling
- Want to keep IDs out of your source code when working
- Want faster compilation

### How Cache Works

The Trice cache saves copies of all files after processing them with `trice i` or `trice c`. This avoids inserting and removing IDs repeatedly. **The copies are used to get the same results for files that have not been edited.** Edited files are processed normally and the cache updates afterwards. File modification times do not change, so **the build system does not reprocess unchanged files even when IDs are temporarily removed**.

### Important Note

> **<span style="color:red">Be careful when your build system also modifies source files!</span>**

For example, run an auto-formatter **before** the `trice insert` command.

## Which Mode Should You Use?

- **For development:** Direct mode with SEGGER_RTT is recommended
- **For most use cases:** Deferred mode with TRICE_BUFFER == TRICE_RING_BUFFER (uses less RAM) in TRICE_MULTI_PACK_MODE (transfers less data)

## Project Status

Trice is fully usable. There are no known bugs (see [issues](https://github.com/rokath/trice/issues)).

```diff
+ Please use v1.0, v1.1, or the main branch if you want to build from source.
- Do not use the "dev" branch right now (December 2025) - it may not work properly.
```

## Future Plans

The documentation could be improved. We could add features like remote procedure calls. Or create a separate tlog tool written in C or Python (with AI help). This would allow logging on any platform, not just platforms supported by **Go**.

## Structured Logging

Trice will soon support structured logging. Based on feedback from [#531](https://github.com/rokath/trice/discussions/531), there is now a [specification draft](./docs/TriceUserManual.md#trice-structured-logging). Please provide feedback before implementation starts.

## Support the Project

- Trice takes a lot of my free time. I want to keep it MIT licensed in the future.
- If you make profit using Trice in your products, donations help convince my family to continue improving Trice.
- :star: Star this project! ☺

[Become a Sponsor with your Github Account](https://github.com/sponsors/rokath/)

OR

<a href="https://www.buymeacoffee.com/rokath" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="30" width="120"></a>

OR

<p>
  <a href="https://www.paypal.me/rolfkarlthomas">
      <img src="https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif" alt="paypal" height="40" width="120">
  </a>
</p>

## Clone the Repository

```bash
git clone https://github.com/rokath/trice.git
```

## Similar Projects

- ARM ITM/SWO (hardware-native)
- ARM Keil Event Recorder (hardware-native)
- [baical.net](http://baical.net/up7.html) (C)
- [call stack logger function instrumentation](https://dev.to/taugustyn/call-stack-logger-function-instrumentation-as-a-way-to-trace-programs-flow-of-execution-419a) (trace program execution flow)
- [Debugging with Dynamic Printf Breakpoints](https://mcuoneclipse.com/2022/02/09/debugging-with-dynamic-printf-breakpoints/) (Eclipse IDE option)
- [defmt (Rust)](https://github.com/knurling-rs/defmt)
- [Diagnostic Log and Trace](https://github.com/COVESA/dlt-daemon) (AUTOSAR)
- [Embedded Logger (elog)](https://github.com/martinribelotta/elog) (Embedded logger with minimal footprint and memory usage)
- [Logging with symbols - The Embedonomicon](https://docs.rust-embedded.org/embedonomicon/logging.html)
- [McuLog (McuOnEclipse)](https://mcuoneclipse.com/2020/06/01/mculog-logging-framework-for-small-embedded-microcontroller-systems/)
- [MCUViewer](https://github.com/klonyyy/MCUViewer)
- [Memfault Compact Log Library](https://docs.memfault.com/docs/mcu/compact-logs/#host-decoding) (part of firmware SDK)
<!-- - [Metal Serial Library](https://github.com/metal-ci/_test/tree/master/doc/metal.serial.md) -->
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

[ChatGPT Trice Compare (2025-05-26)](https://htmlpreview.github.io/?https://github.com/rokath/trice/blob/main/docs/ChatGPTo4-mini-high_TriceCompare.html)

<p align="right">(<a href="#top">back to top</a>)</p>
