# Trice Documentation Guide

New to Trice? Follow the [User Manual](./TriceUserManual.md) from your first PC log to your own firmware. The first experiments need no board. The [root README](../README.md) provides the project overview and repository map.

Already using Trice? Jump to a task under [Integration](#integration), [Log output and analysis](#log-output-and-analysis), [Target behavior and transport](#target-behavior-and-transport), or [Troubleshooting and contributions](#troubleshooting-and-contributions). The manuals contain the explanations; this guide helps you find them.

## Document Overview

Document                                                                                             | Content
-----------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------
[trice/README.md](../README.md)                                                                      | Project Overview
[trice/docs/TriceUserManual.md](./TriceUserManual.md)                                                | From your first PC log to your own firmware. The first experiments need no board.
[trice/docs/TriceReferenceManual.md](./TriceReferenceManual.md)                                          | The complete technical reference
[trice/docs/TriceDocsGuide.md](./TriceDocsGuide.md)                                                  | This file
[trice/docs/ref/trice-help-all.txt](./ref/trice-help-all.txt) - CLI help output                      | CLI help snapshot; run `trice help -all` for your installed tool
[trice/src/triceDefaultConfig.h](../src/triceDefaultConfig.h) - Source Code                          | Target configuration switches explained and their default values
[trice/_test/testdata/triceCheck.c](../_test/testdata/triceCheck.c) - Test Code                      | Extensive executable Trice test inputs; not a beginner tutorial
[Project community links](../README.md#project-information)                                      | GitHub discussions and issues for questions, bug reports, and resolved problems
[trice/docs/ref/](./ref) - included material                                                         | documentation images and CLI-help data
[trice/docs/scratchPad/](./scratchPad)                                                               | Internal plans, drafts and historical material - no prerequisites for using Trice.
[trice/CHANGELOG.md](../CHANGELOG.md)                                                                   | Release history
[trice/CONTRIBUTING.md](../CONTRIBUTING.md)                                                          | Contribution and validation guidance

## Integration

What you need                                      | Where to look
---------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------
A guided start with runnable examples              | [User Manual](./TriceUserManual.md), from installation through fields, filters and context to target integration
Setup, syntax, configuration and limits            | [Reference Manual](./TriceReferenceManual.md), the complete technical reference
An example to build and modify                     | [Central example guide](./TriceReferenceManual.md#example-projects-without-and-with-trice-instrumentation), with links to the executable projects
Command-line options                               | [CLI help snapshot](./ref/trice-help-all.txt); run `trice help -all` for your installed tool
Target configuration defaults                      | [triceDefaultConfig.h](../src/triceDefaultConfig.h), explained in the Reference Manual
An existing byte writer, RTT, or UART               | [Quickstarts](./TriceReferenceManual.md#quickstarts)
Use `trice bind`                                   | [Trice Bind](./TriceReferenceManual.md#trice-bind), [build integration](./TriceReferenceManual.md#build-integration), and [bind-limits](./TriceReferenceManual.md#bind-limits)
Trice ID management with `insert` and `clean`       | [ID management](./TriceReferenceManual.md#trice-id-management) and [Insert algorithm and workflow](./TriceReferenceManual.md#the-trice-insert-algorithm)
Which dictionaries and generated files to keep     | [Files to keep](./TriceUserManual.md#know-which-files-to-keep), [dictionary compatibility](./TriceReferenceManual.md#compatibility-with-firmware-and-host-tool-versions), and [Bind artifact report](./TriceReferenceManual.md#bind-artifact-report)
Local text output without a host decoder           | [Local deferred text log](./TriceReferenceManual.md#local-deferred-text-log) and [local logging examples](./TriceReferenceManual.md#local-logging-example-projects); requires target-side formatting and a generated dictionary

## Log output and analysis

| What you need | Where to look |
| --- | --- |
| Log levels | [Tag weights](./TriceReferenceManual.md#tag-weights) and [selecting tags and priority](./TriceReferenceManual.md#selecting-tags-and-priority); these filters operate on the host |
| Select or exclude individual tags | [Selecting tags and priority](./TriceReferenceManual.md#selecting-tags-and-priority) (`-pick`, `-ban`) |
| Tag-specific routing on the target | [ID Routing](./TriceReferenceManual.md#id-routing); ID assignment and output-channel configuration are separate steps |
| Structured fields in JSON or KV | [Structured Logging](./TriceReferenceManual.md#structured-logging); JSON output is NDJSON, one event per line |
| Context Enrichment | [Context Enrichment](./TriceReferenceManual.md#trice-context-enrichment) and [supported log sites and alternatives](./TriceReferenceManual.md#supported-log-sites-and-alternatives) |
| Runtime strings or buffers | [triceS strings](./TriceReferenceManual.md#runtime-generated-0-terminated-strings-transfer-with-trices), [triceN counted strings](./TriceReferenceManual.md#runtime-generated-counted-strings-transfer-with--tricen), and [triceB buffers](./TriceReferenceManual.md#runtime-generated-buffer-transfer-with-triceb) |
| Timing analysis | [Target stamps](./TriceReferenceManual.md#trice-timestamps) and [delta columns](./TriceReferenceManual.md#target-timestamp-delta-columns); stamp units and meaning are application-defined |
| Event counts | [Event statistics](./TriceReferenceManual.md#event-statistics) |
| Graphical visualization | [Visualization output](./TriceReferenceManual.md#visualization-output-with--vis) and [LabPlot demo](./TriceReferenceManual.md#setting-up-the-labplot-demo) |
| Record decoded text or raw bytes | [Logfile output](./TriceReferenceManual.md#logfile-output) and [binary logfile](./TriceReferenceManual.md#binary-logfile) |
| Integrating multiple targets | [Several targets at the same time](./TriceReferenceManual.md#several-targets-at-the-same-time) |

## Target behavior and transport

| What you need | Where to look |
| --- | --- |
| Minimize log-call execution time | [Trice Speed](./TriceReferenceManual.md#trice-speed); compare direct and deferred modes and their RAM needs |
| Reduce target image size | [Memory needs](./TriceReferenceManual.md#trice-memory-needs) and [image size optimization](./TriceReferenceManual.md#trice-project-image-size-optimization); some measurements describe older versions |
| Reduce transferred bytes | [Minimal transfer bytes](./TriceReferenceManual.md#minimal-transfer-bytes-amount), [parameter bit widths](./TriceReferenceManual.md#trice-parameter-bit-widths), and [framing](./TriceReferenceManual.md#framing) |
| SEGGER RTT | [RTT quickstart](./TriceReferenceManual.md#quickstart-segger-rtt-direct-mode-with-j-link) and [Trice over RTT](./TriceReferenceManual.md#trice-over-rtt) |
| SD-card or custom output writer | [SD-card and user-specific output](./TriceReferenceManual.md#writing-the-trice-logs-into-an-sd-card-or-a-user-specific-output) |
| Big-endian targets | [Endianness](./TriceReferenceManual.md#endianness) |
| User protocol packets in the log stream | [typeX0 user packets](./TriceReferenceManual.md#typex0-user-packets) |
| Encryption | [Optional XTEA encryption](./TriceReferenceManual.md#optional-xtea-encryption) |
| Buffer protection and overflow diagnostics | [Trice Protection](./TriceReferenceManual.md#trice-protection) and [avoiding buffer overruns](./TriceReferenceManual.md#avoid-buffer-overruns) |
| Disable logging at compile time | [Switching Trice on and off](./TriceReferenceManual.md#switching-trice-on-and-off) |
| Remote commands with Trice ABC | [Asynchronous Broadcast Commands](./TriceReferenceManual.md#trice-abc---asynchronous-broadcast-commands), including [what ABC is not](./TriceReferenceManual.md#what-abc-is-not) |
| Stimulate the target from the host | [User commands over UART](./TriceReferenceManual.md#stimulate-target-with-a-user-command-over-uart); application code interprets the received command |
| Adapt existing logging and assert macros | [Legacy aliases](./TriceReferenceManual.md#legacy-user-code-option-trice-aliases-adaptation) and [alias example](./TriceReferenceManual.md#alias-example-project); the [assert test configuration](../_test/aliasassert_dblB_de_tcobs_ua/triceConfig.h) demonstrates application-specific assert integration |

## Troubleshooting and contributions

| What you need | Where to look |
| --- | --- |
| Missing or incorrect log output | [User Manual troubleshooting](./TriceUserManual.md#if-something-does-not-work) and [Reference Manual troubleshooting](./TriceReferenceManual.md#trice-trouble-shooting-hints) |
| Bind-specific errors | [Bind diagnostics](./TriceReferenceManual.md#diagnostics-and-troubleshooting) |
| Release changes or contributing | [CHANGELOG](../CHANGELOG.md) and [CONTRIBUTING](../CONTRIBUTING.md) |
| Run tests or add test cases | [Testing the target library](./TriceReferenceManual.md#testing-the-trice-library-c-code-for-the-target) |
| Questions and bug reports | [Project community links](../README.md#project-information) |

The User Manual is the short learning path; the Reference Manual contains the full contracts and detailed example instructions. Example READMEs link to the central manuals so each example has one maintained explanation.
