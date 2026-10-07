# Trice Guide for Orientation

<h2> Table of Contents </h2>

<!-- mdtoc -->

* [1. Document Overview](#document-overview)
* [2. Entry Points](#entry-points)
  * [2.1. Integration](#integration)
  * [2.2. Log output and analysis](#log-output-and-analysis)
  * [2.3. Target behavior and transport](#target-behavior-and-transport)
  * [2.4. Troubleshooting and contributions](#troubleshooting-and-contributions)
  * [2.5. Project resources](#project-resources)
* [3. Project Folder Structure](#project-folder-structure)
  * [3.1. Sources, examples, and documentation](#sources-examples-and-documentation)
  * [3.2. Repository configuration and history](#repository-configuration-and-history)
  * [3.3. Generated and local files](#generated-and-local-files)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

New to Trice? Follow the [User Manual](./TriceUserManual.md) from your first PC log to your own firmware. The first experiments need no board. 

Already using Trice? Jump to a task under [2. Entry Points](#entry-points). The manuals contain the explanations; this guide helps you find them.

## 1. <a id="document-overview"></a>Document Overview

Document                                                                        | Content
--------------------------------------------------------------------------------|-----------------------------------------------------------------------------------
[trice/README.md](../README.md)                                                 | Project Entry
[trice/docs/TriceUserManual.md](./TriceUserManual.md)                           | From your first PC log to your own firmware. The first experiments need no board.
[trice/docs/TriceReferenceManual.md](./TriceReferenceManual.md)                 | The complete technical reference
[trice/docs/TriceGuide.md](./TriceGuide.md)                                     | This file
[trice/docs/ref/trice-help-all.txt](./ref/trice-help-all.txt) - CLI help output | CLI help snapshot; run `trice help -all` for your installed tool
[trice/src/triceDefaultConfig.h](../src/triceDefaultConfig.h) - Source Code     | Target configuration switches explained and their default values
[trice/_test/testdata/triceCheck.c](../_test/testdata/triceCheck.c) - Test Code | Extensive executable Trice test inputs; not a beginner tutorial
[trice/docs/ref/](./ref) - included material                                    | documentation images and CLI-help data
[trice/docs/scratchPad/](./scratchPad)                                          | Internal plans, drafts and historical material - no prerequisites for using Trice.
[trice/CHANGELOG.md](../CHANGELOG.md)                                           | Release history
[trice/CONTRIBUTING.md](../CONTRIBUTING.md)                                     | Contribution and validation guidance

## 2. <a id="entry-points"></a>Entry Points

### 2.1. <a id="integration"></a>Integration

What you need                                  | Where to look
-----------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
A guided start with runnable examples          | [User Manual](./TriceUserManual.md), from installation through fields, filters and context to target integration
Setup, syntax, configuration and limits        | [Reference Manual](./TriceReferenceManual.md), the complete technical reference
Printf-like calls and their differences | [Trice and printf](./TriceReferenceManual.md#trice-similarities-and-differences-to-printf-usage), including floating-point values and extended formatting
An example to build and modify                 | [Central example guide](./TriceReferenceManual.md#example-projects-without-and-with-trice-instrumentation), with links to the executable projects
Command-line options                           | [CLI help snapshot](./ref/trice-help-all.txt); run `trice help -all` for your installed tool
Target configuration defaults                  | [triceDefaultConfig.h](../src/triceDefaultConfig.h), explained in the Reference Manual
An existing byte writer, RTT, or UART          | [Quickstarts](./TriceReferenceManual.md#quickstarts)
Use `trice bind`                               | [Trice Bind](./TriceReferenceManual.md#trice-bind), [build integration](./TriceReferenceManual.md#build-integration), and [bind-limits](./TriceReferenceManual.md#bind-limits)
Trice ID management with `insert` and `clean`  | [ID management](./TriceReferenceManual.md#trice-id-management) and [Insert algorithm and workflow](./TriceReferenceManual.md#the-trice-insert-algorithm)
Which dictionaries and generated files to keep | [Files to keep](./TriceUserManual.md#know-which-files-to-keep), [dictionary compatibility](./TriceReferenceManual.md#compatibility-with-firmware-and-host-tool-versions), and [Bind artifact report](./TriceReferenceManual.md#bind-artifact-report)
Local text output without a host decoder       | [Local deferred text log](./TriceReferenceManual.md#local-deferred-text-log) and [local logging examples](./TriceReferenceManual.md#local-logging-example-projects); requires target-side formatting and a generated dictionary
Generate supporting files | [Trice Generate](./TriceReferenceManual.md#trice-generate): local C format tables, readable JSON views, and artifact reports
Avoid unnecessary recompilation | [Trice Cache](./TriceReferenceManual.md#trice-cache-for-compilation-speed)

### 2.2. <a id="log-output-and-analysis"></a>Log output and analysis

| What you need                      | Where to look                                                                                                                                                                                                                                                                                                       |
|------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Log levels                         | [Tag weights](./TriceReferenceManual.md#tag-weights) and [selecting tags and priority](./TriceReferenceManual.md#selecting-tags-and-priority); these filters operate on the host                                                                                                                                    |
| Tag colors and aliases | [Trice tags and colors](./TriceReferenceManual.md#trice-tags-color-and-weights) and [user-defined tags](./TriceReferenceManual.md#user-defined-tags-weights-and-colors) |
| Select or exclude individual tags  | [Selecting tags and priority](./TriceReferenceManual.md#selecting-tags-and-priority) (`-pick`, `-ban`)                                                                                                                                                                                                              |
| Tag-specific routing on the target | [ID Routing](./TriceReferenceManual.md#id-routing); ID assignment and output-channel configuration are separate steps                                                                                                                                                                                               |
| Structured fields in JSON or KV    | [Structured Logging](./TriceReferenceManual.md#structured-logging); JSON output is NDJSON, one event per line                                                                                                                                                                                                       |
| Context Enrichment                 | [Context Enrichment](./TriceReferenceManual.md#trice-context-enrichment) and [supported log sites and alternatives](./TriceReferenceManual.md#supported-log-sites-and-alternatives)                                                                                                                                 |
| Runtime strings or buffers         | [triceS strings](./TriceReferenceManual.md#runtime-generated-0-terminated-strings-transfer-with-trices), [triceN counted strings](./TriceReferenceManual.md#runtime-generated-counted-strings-transfer-with--tricen), and [triceB buffers](./TriceReferenceManual.md#runtime-generated-buffer-transfer-with-triceb) |
| Timing analysis                    | [Target stamps](./TriceReferenceManual.md#trice-timestamps) and [delta columns](./TriceReferenceManual.md#target-timestamp-delta-columns); stamp units and meaning are application-defined                                                                                                                          |
| Source-file and line information | [Location information](./TriceReferenceManual.md#location-information) |
| Event counts                       | [Event statistics](./TriceReferenceManual.md#event-statistics)                                                                                                                                                                                                                                                      |
| Graphical visualization            | [Visualization output](./TriceReferenceManual.md#visualization-output-with--vis) and [LabPlot demo](./TriceReferenceManual.md#setting-up-the-labplot-demo)                                                                                                                                                          |
| Record decoded text or raw bytes   | [Logfile output](./TriceReferenceManual.md#logfile-output) and [binary logfile](./TriceReferenceManual.md#binary-logfile)                                                                                                                                                                                           |
| Integrating multiple targets       | [Several targets at the same time](./TriceReferenceManual.md#several-targets-at-the-same-time)                                                                                                                                                                                                                      |

### 2.3. <a id="target-behavior-and-transport"></a>Target behavior and transport

| What you need                              | Where to look                                                                                                                                                                                                                                                                                                |
|--------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Minimize log-call execution time           | [Trice Speed](./TriceReferenceManual.md#trice-speed); compare direct and deferred modes and their RAM needs                                                                                                                                                                                                  |
| Logging from interrupts or multiple tasks | [Critical sections and Trice checks](./TriceReferenceManual.md#trice-checks) |
| Work without heap allocation | [No dynamic memory management in the target logging path](./TriceReferenceManual.md#no-dynamic-memory-management-needed) |
| Reduce target image size                   | [Memory needs](./TriceReferenceManual.md#trice-memory-needs) and [image size optimization](./TriceReferenceManual.md#trice-project-image-size-optimization); some measurements describe older versions                                                                                                       |
| Reduce transferred bytes                   | [Minimal transfer bytes](./TriceReferenceManual.md#minimal-transfer-bytes-amount), [parameter bit widths](./TriceReferenceManual.md#trice-parameter-bit-widths), and [framing](./TriceReferenceManual.md#framing)                                                                                            |
| Understand binary records | [Binary encoding](./TriceReferenceManual.md#binary-encoding), including record layout and COBS/TCOBS framing |
| SEGGER RTT                                 | [RTT quickstart](./TriceReferenceManual.md#quickstart-segger-rtt-direct-mode-with-j-link) and [Trice over RTT](./TriceReferenceManual.md#trice-over-rtt)                                                                                                                                                     |
| SD-card or custom output writer            | [SD-card and user-specific output](./TriceReferenceManual.md#writing-the-trice-logs-into-an-sd-card-or-a-user-specific-output)                                                                                                                                                                               |
| Big-endian targets                         | [Endianness](./TriceReferenceManual.md#endianness)                                                                                                                                                                                                                                                           |
| User protocol packets in the log stream    | [typeX0 user packets](./TriceReferenceManual.md#typex0-user-packets)                                                                                                                                                                                                                                         |
| Encryption                                 | [Optional XTEA encryption](./TriceReferenceManual.md#optional-xtea-encryption)                                                                                                                                                                                                                               |
| Buffer protection and overflow diagnostics | [Trice Protection](./TriceReferenceManual.md#trice-protection) and [avoiding buffer overruns](./TriceReferenceManual.md#avoid-buffer-overruns)                                                                                                                                                               |
| Disable logging at compile time            | [Switching Trice on and off](./TriceReferenceManual.md#switching-trice-on-and-off)                                                                                                                                                                                                                           |
| Remote commands with Trice ABC             | [Asynchronous Broadcast Commands](./TriceReferenceManual.md#trice-abc---asynchronous-broadcast-commands), including [what ABC is not](./TriceReferenceManual.md#what-abc-is-not)                                                                                                                             |
| Stimulate the target from the host         | [User commands over UART](./TriceReferenceManual.md#stimulate-target-with-a-user-command-over-uart); application code interprets the received command                                                                                                                                                        |
| Adapt existing logging and assert macros   | [Legacy aliases](./TriceReferenceManual.md#legacy-user-code-option-trice-aliases-adaptation) and [alias example](./TriceReferenceManual.md#alias-example-project); the [assert test configuration](../_test/aliasassert_dblB_de_tcobs_ua/triceConfig.h) demonstrates application-specific assert integration |

### 2.4. <a id="troubleshooting-and-contributions"></a>Troubleshooting and contributions

| What you need                   | Where to look                                                                                                                                                                 |
|---------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Missing or incorrect log output | [User Manual troubleshooting](./TriceUserManual.md#if-something-does-not-work) and [Reference Manual troubleshooting](./TriceReferenceManual.md#trice-trouble-shooting-hints) |
| Bind-specific errors            | [Bind diagnostics](./TriceReferenceManual.md#diagnostics-and-troubleshooting)                                                                                                 |
| Release changes or contributing | [CHANGELOG](../CHANGELOG.md) and [CONTRIBUTING](../CONTRIBUTING.md)                                                                                                           |
| Run tests or add test cases     | [Testing the target library](./TriceReferenceManual.md#testing-the-trice-library-c-code-for-the-target)                                                                       |
| Questions and bug reports       | [Community and contributions](./TriceReferenceManual.md#community-and-contributions)                                                                                           |

### 2.5. <a id="project-resources"></a>Project resources

| What you need | Where to look |
| --- | --- |
| Downloads and installation | [Get Trice](./TriceReferenceManual.md#download-and-install-trice): released host tools, source builds, and version compatibility |
| Compare approaches | [Alternative projects](./TriceReferenceManual.md#alternative-projects-and-related-approaches) for logging, tracing, and visualization |
| Support the project | [Support and sponsoring](./TriceReferenceManual.md#support-and-sponsoring), including GitHub Sponsors, Buy Me a Coffee, and PayPal |
| Licensing | [MIT License](../LICENSE.md) |

The User Manual is the short learning path; the Reference Manual contains the full contracts and detailed example instructions. Example READMEs link to the central manuals so each example has one maintained explanation.

## 3. <a id="project-folder-structure"></a>Project Folder Structure

Start with [demo](../demo/) to see a first log, [examples/PC_features](../examples/PC_features/) to experiment, or [src](../src/) to integrate the target library. Most firmware users do not need to explore the host implementation or repository maintenance files.

### 3.1. <a id="sources-examples-and-documentation"></a>Sources, examples, and documentation

| Folder | Purpose and useful entry point |
| --- | --- |
| [src](../src/) | The target C library. Include `trice.h` and provide a project-specific `triceConfig.h`; [firmware integration](./TriceUserManual.md#bring-trice-into-your-firmware) shows the steps. `triceDefaultConfig.h` documents the defaults. |
| [demo](../demo/) | Minimal direct and deferred PC demos. Start with the [first-log walkthrough](./TriceUserManual.md#see-your-first-log). |
| [examples](../examples/) | PC and STM32 applications, plus shared example producers in `exampleData`. Choose a project through the [central example guide](./TriceReferenceManual.md#example-projects-without-and-with-trice-instrumentation); board projects can include their own vendor code. |
| [docs](./) | This Guide, the short User Manual, and the detailed Reference Manual. `ref` holds images and the CLI-help snapshot. |
| [cmd](../cmd/) | Go entry points for `trice`, the `tlog` logging shortcut, and developer utilities. Use [buildTriceTool.sh](../scripts/buildTriceTool.sh) to build the host tools. |
| [internal](../internal/) | Host-tool implementation: argument handling, ID workflows, receiving, decoding, and output. Relevant Go tests live beside the code. |
| [pkg](../pkg/) | Supporting Go packages, including cipher, message, and test helpers. Target firmware integrates `src`, not these host packages. |
| [_test](../_test/) | C target configurations, test harnesses, and shared cases such as `testdata/triceCheck.c`. See [testing the target library](./TriceReferenceManual.md#testing-the-trice-library-c-code-for-the-target). |
| [scripts](../scripts/) | Host-tool builds, formatting, validation, and release helpers. `testAll.sh` runs the repository checks; [CONTRIBUTING](../CONTRIBUTING.md) explains development workflows. |
| [third_party](../third_party/) | Retained external packages and tool archives. Their licenses and setup requirements are separate; see [third-party packages and retained versions](./TriceReferenceManual.md#third-party-packages-and-retained-versions). |

### 3.2. <a id="repository-configuration-and-history"></a>Repository configuration and history

| Location | Purpose |
| --- | --- |
| [.github](../.github/) | CI, release, and documentation workflows, issue templates, and funding configuration. See the [workflow reference](./TriceReferenceManual.md#the-github-folder--purpose-and-contents). |
| [.vscode](../.vscode/) and [.idea](../.idea/) | Optional VS Code and GoLand project settings; not target-library dependencies. |
| [.code_snippets](../.code_snippets/) | Retained legacy helper snippets; not the recommended integration path. |
| [docs/scratchPad](./scratchPad/) | Internal plans and drafts. Its `obsolete` subtree is archived history, not current usage guidance or a build prerequisite. |
| `experiments/` | Experimental or local working material, if present. Use the documented examples for supported entry points. |
| [go.mod](../go.mod) and [go.sum](../go.sum) | Go version, module requirements, and dependency checksums for the host tools. |
| [Root metadata](./TriceReferenceManual.md#trice-project-structure-files-and-folders) | Release notes, licenses, contribution guidance, formatter/linter settings, and publishing configuration. The Reference Manual lists individual files. |

### 3.3. <a id="generated-and-local-files"></a>Generated and local files

These directories may appear after building or testing; they are not additional source libraries to integrate.

| Location | Typical contents and handling |
| --- | --- |
| `generated/` | Default `-genDir` output: Bind sidecars, the structured-field registry, optional one-line JSON views, and generated C tables. Bind projects add it to the compiler include path. See [persistent and generated files](./TriceReferenceManual.md#persistent-and-generated-files). |
| `build/` or example-specific output folders | Compiler output. The example's build script determines the path. |
| `dist/` | Local GoReleaser archives and release artifacts. |
| `temp/` | Local recordings, test logs, and recovery snapshots. Failure details from `testAll.sh` are under `temp/log`. |
| `.gocache/`, `coverage.out`, and other local tool outputs | Build/test caches and coverage results; not firmware inputs. |

`til.json` and `li.json` are different from disposable compiler output: the dictionary maps IDs to messages, and location data connects IDs to source positions. The root `demoTIL.json` and `demoLI.json` serve repository examples and tests; your application should use its own files. Preserve dictionaries needed for released firmware and recordings. Generated sidecars can also contain historical ID bindings, so do not assume every file in `generated` can be deleted without reviewing its role. See [which files to keep](./TriceUserManual.md#know-which-files-to-keep) and the read-only [Bind artifact report](./TriceReferenceManual.md#bind-artifact-report).
