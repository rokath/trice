# Trice — Trace IDs for Embedded C

[![License](https://img.shields.io/github/license/rokath/trice)](./LICENSE.md)
[![Latest release](https://img.shields.io/github/v/release/rokath/trice)](./docs/TriceReferenceManual.md#download-and-install-trice)
[![Downloads](https://img.shields.io/github/downloads/rokath/trice/total)](./docs/TriceReferenceManual.md#download-and-install-trice)
[![Target library CI](https://github.com/rokath/trice/actions/workflows/trice_lib_ci_full.yml/badge.svg)](./.github/workflows/trice_lib_ci_full.yml)

<img align="right" src="docs/ref/TriceGirl-167x222.png" width="115" alt="Trice project mascot">

**Log and trace in as few as 6 CPU clocks per call.**[^speed]<br>
Readable calls. Compact records. Even from interrupt handlers.[^design]

**[Explore Trice](./docs/TriceGuide.md)** · **[Quickstart](./docs/TriceUserManual.md#see-your-first-log)** · **[Downloads](./docs/TriceReferenceManual.md#download-and-install-trice)**

Trace firmware activity, inspect measurements, and interact with your target through [Asynchronous Broadcast Commands](./docs/TriceReferenceManual.md#trice-abc---asynchronous-broadcast-commands).

![Colored Trice output with source locations and timestamps](./docs/ref/life0.gif)

## Discover the features

### Observe your firmware

- [Logging and tracing with printf-like calls](./docs/TriceReferenceManual.md#trice-similarities-and-differences-to-printf-usage)
- [Interrupts and critical sections](./docs/TriceReferenceManual.md#trice-checks)
- [Target stamps, host timestamps, and time differences](./docs/TriceReferenceManual.md#trice-timestamps)
- [Source-file and line information](./docs/TriceReferenceManual.md#location-information)
- [Colored tags and aliases](./docs/TriceReferenceManual.md#trice-tags-color-and-weights)
- [Log levels and selection](./docs/TriceReferenceManual.md#selecting-tags-and-priority) · [Event statistics](./docs/TriceReferenceManual.md#event-statistics)
- [Multiple targets in one log](./docs/TriceReferenceManual.md#several-targets-at-the-same-time)

### Work with data and commands

- [Structured fields with text, JSON, and KV output](./docs/TriceReferenceManual.md#structured-logging)
- [Context Enrichment for task IDs and application values](./docs/TriceReferenceManual.md#trice-context-enrichment)
- [Live plots and the LabPlot demo](./docs/TriceReferenceManual.md#setting-up-the-labplot-demo)
- [Remote commands and responses with Trice ABC](./docs/TriceReferenceManual.md#trice-abc---asynchronous-broadcast-commands)
- [Target stimulation over UART](./docs/TriceReferenceManual.md#stimulate-target-with-a-user-command-over-uart)
- [Runtime strings, buffers, floats, and extended formatting](./docs/TriceReferenceManual.md#trice-similarities-and-differences-to-printf-usage)
- [User protocol packets alongside Trice records](./docs/TriceReferenceManual.md#typex0-user-packets)
- [Text recordings](./docs/TriceReferenceManual.md#logfile-output) · [Binary captures](./docs/TriceReferenceManual.md#binary-logfile)

### Fit your target

- [Measured execution speed and direct/deferred modes](./docs/TriceReferenceManual.md#trice-speed)
- [Target memory needs](./docs/TriceReferenceManual.md#trice-memory-needs) · [Image-size optimization](./docs/TriceReferenceManual.md#trice-project-image-size-optimization)
- [No heap allocation in the target logging path](./docs/TriceReferenceManual.md#no-dynamic-memory-management-needed)
- [Compact records, parameter widths, and COBS/TCOBS framing](./docs/TriceReferenceManual.md#binary-encoding)
- [UART/USB or an existing byte writer](./docs/TriceReferenceManual.md#quickstarts)
- [SEGGER RTT](./docs/TriceReferenceManual.md#trice-over-rtt)
- [SD-card and custom outputs](./docs/TriceReferenceManual.md#writing-the-trice-logs-into-an-sd-card-or-a-user-specific-output)
- [Local text output without a host decoder](./docs/TriceReferenceManual.md#local-deferred-text-log)
- [Tag-specific output routing](./docs/TriceReferenceManual.md#id-routing)
- [Big-endian targets](./docs/TriceReferenceManual.md#endianness)
- [Optional XTEA encryption](./docs/TriceReferenceManual.md#optional-xtea-encryption)
- [Buffer protection and overflow diagnostics](./docs/TriceReferenceManual.md#trice-protection)
- [Compile-time logging switches](./docs/TriceReferenceManual.md#switching-trice-on-and-off)

### Keep the workflow comfortable

- [ID-free calls with Bind](./docs/TriceReferenceManual.md#trice-bind) · [Insert/Clean](./docs/TriceReferenceManual.md#the-trice-insert-algorithm)
- [ID management and dictionaries for released firmware](./docs/TriceReferenceManual.md#trice-id-management)
- [Generated data, local format tables, and artifact reports](./docs/TriceReferenceManual.md#trice-generate)
- [Adapt existing logging and assert macros](./docs/TriceReferenceManual.md#legacy-user-code-option-trice-aliases-adaptation)
- [Compiler support and Bind limits](./docs/TriceReferenceManual.md#bind-limits)
- [Build caching](./docs/TriceReferenceManual.md#trice-cache-for-compilation-speed)

## Try it and explore

**[First PC log — no board needed](./docs/TriceUserManual.md#see-your-first-log)** · **[PC and STM32 examples](./docs/TriceReferenceManual.md#example-projects-without-and-with-trice-instrumentation)** · **[Firmware quickstarts](./docs/TriceReferenceManual.md#quickstarts)**

Find your next topic in the **[Trice Guide](./docs/TriceGuide.md)**, follow the **[User Manual](./docs/TriceUserManual.md)**, or look up details in the **[Reference Manual](./docs/TriceReferenceManual.md)**.

## Project information

**[Downloads and installation](./docs/TriceReferenceManual.md#download-and-install-trice)** · **[Alternative projects](./docs/TriceReferenceManual.md#alternative-projects-and-related-approaches)** · **[Support and sponsoring](./docs/TriceReferenceManual.md#support-and-sponsoring)**

[Community and contributions](./docs/TriceReferenceManual.md#community-and-contributions) · [Release notes](./CHANGELOG.md) · [MIT License](./LICENSE.md) · [Project folders](./docs/TriceGuide.md#project-folder-structure)

[^speed]: Record creation in an optimized configuration; transmission is handled separately. See the [measurement and configuration](./docs/TriceReferenceManual.md#trice-speed).
[^design]: See [how Trice works](./docs/TriceReferenceManual.md#how-it-works---the-main-idea) and [interrupt protection](./docs/TriceReferenceManual.md#trice-checks). Configure critical sections when interrupts or multiple tasks share a buffer.
