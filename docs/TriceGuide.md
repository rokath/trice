# Trice Guide for Orientation

New to Trice? Follow the [User Manual](./TriceUserManual.md) from your first PC log to your own firmware. The first experiments need no board. 

Already using Trice? Jump to a task under [2. Entry Points](#entry-points). The manuals contain the explanations; this guide helps you find them.

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

## 1. <a id="document-overview"></a>Document Overview

Document                                                                        | Content
--------------------------------------------------------------------------------|-----------------------------------------------------------------------------------
[trice/README.md](../README.md)                                                 | Project Entry
[trice/docs/TriceUserManual.md](./TriceUserManual.md)                           | From your first PC log to your own firmware. The first experiments need no board.
[trice/docs/rm/README.md](rm/README.md)                 | The complete technical reference
[trice/docs/TriceGuide.md](./TriceGuide.md)                                     | This file
[trice/docs/ref/trice-help-all.txt](./ref/trice-help-all.txt) - CLI help output | CLI help snapshot; run `trice help -all` for your installed tool
[trice/src/triceDefaultConfig.h](../src/triceDefaultConfig.h) - Source Code     | Target configuration switches explained and their default values
[trice/_test/testdata/triceCheck.c](../_test/testdata/triceCheck.c) - Test Code | Extensive executable Trice test inputs; not a beginner tutorial
[trice/docs/ref/](./ref) - included material                                    | documentation images and CLI-help data
[trice/docs/_scratchPad/](./_scratchPad)                                        | Internal plans, drafts and historical material - no prerequisites for using Trice.
[trice/CHANGELOG.md](../CHANGELOG.md)                                           | Release history
[trice/CONTRIBUTING.md](../CONTRIBUTING.md)                                     | Contribution and validation guidance

## 2. <a id="entry-points"></a>Entry Points

### 2.1. <a id="integration"></a>Integration

What you need                                  | Where to look
-----------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
A guided start with runnable examples          | [User Manual](./TriceUserManual.md), from installation through fields, filters and context to target integration
Setup, syntax, configuration and limits        | [Reference Manual](rm/README.md), the complete technical reference
Printf-like calls and their differences | [Trice and printf](rm/printf_comparison.md), including floating-point values and extended formatting
An example to build and modify                 | [Central example guide](rm/examples.md), with links to the executable projects
Command-line options                           | [CLI help snapshot](./ref/trice-help-all.txt); run `trice help -all` for your installed tool
Target configuration defaults                  | [triceDefaultConfig.h](../src/triceDefaultConfig.h), explained in the Reference Manual
An existing byte writer, RTT, or UART          | [Quickstarts](rm/quickstarts.md)
Use `trice bind`                               | [Trice Bind](rm/bind.md), [build integration](rm/bind.md#build-integration), and [bind-limits](rm/bind.md#bind-limits)
Trice ID management with `insert` and `clean`  | [ID management](rm/id_management.md) and [Insert algorithm and workflow](rm/the_trice_insert_algorithm.md)
Which dictionaries and generated files to keep | [Files to keep](./TriceUserManual.md#know-which-files-to-keep), [dictionary compatibility](rm/id_reference_list_til_json.md#compatibility-with-firmware-and-host-tool-versions), and [Bind artifact report](rm/generate.md#bind-artifact-report)
Local text output without a host decoder       | [Local deferred text log](rm/target_implementation.md#local-deferred-text-log) and [local logging examples](rm/examples.md#local-logging-example-projects); requires target-side formatting and a generated dictionary
Generate supporting files | [Trice Generate](rm/generate.md): local C format tables, readable JSON views, and artifact reports
Avoid unnecessary recompilation | [Trice Cache](rm/insert_clean_cache.md)

### 2.2. <a id="log-output-and-analysis"></a>Log output and analysis

What you need                      | Where to look
------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Log levels                         | [Tag weights](rm/tags_color_and_weights.md#tag-weights) and [selecting tags and priority](rm/tags_color_and_weights.md#selecting-tags-and-priority); these filters operate on the host
Tag colors and aliases | [Trice tags and colors](rm/tags_color_and_weights.md) and [user-defined tags](rm/tags_color_and_weights.md#user-defined-tags-weights-and-colors)
Select or exclude individual tags  | [Selecting tags and priority](rm/tags_color_and_weights.md#selecting-tags-and-priority) (`-pick`, `-ban`)
Tag-specific routing on the target | [ID Routing](rm/id_management.md#id-routing); ID assignment and output-channel configuration are separate steps
Structured fields in JSON or KV    | [Structured Logging](rm/structured_logging.md); JSON output is NDJSON, one event per line
Context Enrichment                 | [Context Enrichment](rm/context_enrichment.md) and [supported log sites and alternatives](rm/context_enrichment.md#supported-log-sites-and-alternatives)
Runtime strings or buffers         | [triceS strings](rm/printf_comparison.md#runtime-generated-0-terminated-strings-transfer-with-trices), [triceN counted strings](rm/printf_comparison.md#runtime-generated-counted-strings-transfer-with--tricen), and [triceB buffers](rm/printf_comparison.md#runtime-generated-buffer-transfer-with-triceb)
Timing analysis                    | [Target stamps](rm/stamps.md) and [delta columns](rm/stamps.md#target-timestamp-delta-columns); stamp units and meaning are application-defined
Source-file and line information | [Location information](rm/cli.md#location-information)
Event counts                       | [Event statistics](rm/tags_color_and_weights.md#event-statistics)
Graphical visualization            | [Visualization output](rm/cli.md#visualization-output-with--vis) and [LabPlot demo](rm/cli.md#setting-up-the-labplot-demo)
Record decoded text or raw bytes   | [Logfile output](rm/cli.md#logfile-output) and [binary logfile](rm/cli.md#binary-logfile)
Integrating multiple targets       | [Several targets at the same time](rm/usage_tips.md#several-targets-at-the-same-time)

### 2.3. <a id="target-behavior-and-transport"></a>Target behavior and transport

What you need                              | Where to look
--------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Minimize log-call execution time           | [Trice Speed](rm/performance.md); compare direct and deferred modes and their RAM needs
Logging from interrupts or multiple tasks | [Critical sections and Trice checks](rm/getting_started.md#trice-checks)
Work without heap allocation | [No dynamic memory management in the target logging path](rm/features.md#no-dynamic-memory-management-needed)
Reduce target image size                   | [Memory needs](rm/memory_usage.md) and [image size optimization](rm/code_size.md); some measurements describe older versions
Reduce transferred bytes                   | [Minimal transfer bytes](rm/features.md#minimal-transfer-bytes-amount), [parameter bit widths](rm/getting_started.md#trice-parameter-bit-widths), and [framing](rm/framing.md)
Understand binary records | [Binary encoding](rm/binary_encoding.md), including record layout and COBS/TCOBS framing
SEGGER RTT                                 | [RTT quickstart](rm/quickstarts.md#quickstart-segger-rtt-direct-mode-with-j-link) and [Trice over RTT](rm/rtt.md)
SD-card or custom output writer            | [SD-card and user-specific output](rm/custom_output.md)
Big-endian targets                         | [Endianness](rm/endianness.md)
User protocol packets in the log stream    | [typeX0 user packets](rm/typex0_user_packets.md)
Encryption                                 | [Optional XTEA encryption](rm/optional_xtea_encryption.md)
Buffer protection and overflow diagnostics | [Trice Protection](rm/features.md#trice-protection) and [avoiding buffer overruns](rm/usage_tips.md#avoid-buffer-overruns)
Disable logging at compile time            | [Switching Trice on and off](rm/switching_trice_on_and_off.md)
Remote commands with Trice ABC             | [Asynchronous Broadcast Commands](rm/abc_asynchronous_broadcast_commands.md), including [what ABC is not](rm/abc_asynchronous_broadcast_commands.md#what-abc-is-not)
Stimulate the target from the host         | [User commands over UART](rm/cli.md#stimulate-target-with-a-user-command-over-uart); application code interprets the received command
Adapt existing logging and assert macros   | [Legacy aliases](rm/legacy_user_code.md#legacy-user-code-option-trice-aliases-adaptation) and [alias example](rm/legacy_user_code.md#alias-example-project); the [assert test configuration](../_test/aliasassert_dblB_de_tcobs_ua/triceConfig.h) demonstrates application-specific assert integration

### 2.4. <a id="troubleshooting-and-contributions"></a>Troubleshooting and contributions

What you need                   | Where to look
---------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Missing or incorrect log output | [User Manual troubleshooting](./TriceUserManual.md#if-something-does-not-work) and [Reference Manual troubleshooting](rm/troubleshooting.md)
Bind-specific errors            | [Bind diagnostics](rm/bind.md#diagnostics-and-troubleshooting)
Release changes or contributing | [CHANGELOG](../CHANGELOG.md) and [CONTRIBUTING](../CONTRIBUTING.md)
Run tests or add test cases     | [Testing the target library](rm/target_tests.md)
Questions and bug reports       | [Community and contributions](rm/project_resources.md#community-and-contributions)

### 2.5. <a id="project-resources"></a>Project resources

What you need | Where to look
--- | ---
Downloads and installation | [Get Trice](rm/project_resources.md#download-and-install-trice): released host tools, source builds, and version compatibility
Compare approaches | [Alternative projects](rm/project_resources.md#alternative-projects-and-related-approaches) for logging, tracing, and visualization
Support the project | [Support and sponsoring](rm/project_resources.md#support-and-sponsoring), including GitHub Sponsors, Buy Me a Coffee, and PayPal
Licensing | [MIT License](../LICENSE.md)

The User Manual is the short learning path; the Reference Manual contains the full contracts and detailed example instructions. Example READMEs link to the central manuals so each example has one maintained explanation.

## 3. <a id="project-folder-structure"></a>Project Folder Structure

Start with [demo](../demo/) to see a first log, [examples/PC_features](../examples/PC_features/) to experiment, or [src](../src/) to integrate the target library. Most firmware users do not need to explore the host implementation or repository maintenance files.

### 3.1. <a id="sources-examples-and-documentation"></a>Sources, examples, and documentation

Folder | Purpose and useful entry point
--- | ---
[src](../src/) | The target C library. Include `trice.h` and provide a project-specific `triceConfig.h`; [firmware integration](./TriceUserManual.md#bring-trice-into-your-firmware) shows the steps. `triceDefaultConfig.h` documents the defaults.
[src/default_conf](../src/default_conf/) | Fallback RTT configuration. Search this directory last, after project/generated headers and `src`, so your own `SEGGER_RTT_Conf.h` takes precedence. See [Trice over RTT](rm/rtt.md).
[demo](../demo/) | Minimal direct and deferred PC demos. Start with the [first-log walkthrough](./TriceUserManual.md#see-your-first-log).
[examples](../examples/) | PC and STM32 applications, plus shared example producers in `exampleData`. Choose a project through the [central example guide](rm/examples.md); board projects can include their own vendor code.
[docs](./) | This Guide, the short User Manual, and the detailed Reference Manual. `ref` holds images and the CLI-help snapshot.
[cmd](../cmd/) | Go entry points for `trice`, the `tlog` logging shortcut, and developer utilities. Use [buildTriceTool.sh](../scripts/buildTriceTool.sh) to build the host tools.
[internal](../internal/) | Host-tool implementation: argument handling, ID workflows, receiving, decoding, and output. Relevant Go tests live beside the code.
[pkg](../pkg/) | Supporting Go packages, including cipher, message, and test helpers. Target firmware integrates `src`, not these host packages.
[_test](../_test/) | C target configurations, test harnesses, and shared cases such as `testdata/triceCheck.c`. See [testing the target library](rm/target_tests.md).
[scripts](../scripts/) | Host-tool builds, formatting, validation, and release helpers. `testAll.sh` runs the repository checks; [CONTRIBUTING](../CONTRIBUTING.md) explains development workflows.
[third_party](../third_party/) | Retained external packages and tool archives. Their licenses and setup requirements are separate; see [third-party packages and retained versions](rm/development_environment_setup.md#third-party-packages-and-retained-versions).

### 3.2. <a id="repository-configuration-and-history"></a>Repository configuration and history

Location | Purpose
--- | ---
[.github](../.github/) | CI, release, and documentation workflows, issue templates, and funding configuration. See the [workflow reference](rm/maintenance.md#the-github-folder--purpose-and-contents).
[.vscode](../.vscode/) and [.idea](../.idea/) | Optional VS Code and GoLand project settings; not target-library dependencies.
[.code_snippets](../.code_snippets/) | Retained legacy helper snippets; not the recommended integration path.
[docs/_scratchPad](./_scratchPad/) | Internal plans and drafts. Its `obsolete` subtree is archived history, not current usage guidance or a build prerequisite.
`experiments/` | Experimental or local working material, if present. Use the documented examples for supported entry points.
[go.mod](../go.mod) and [go.sum](../go.sum) | Go version, module requirements, and dependency checksums for the host tools.
[Root metadata](rm/maintenance.md#trice-project-structure-files-and-folders) | Release notes, licenses, contribution guidance, formatter/linter settings, and publishing configuration. The Reference Manual lists individual files.

### 3.3. <a id="generated-and-local-files"></a>Generated and local files

These directories may appear after building or testing; they are not additional source libraries to integrate.

Location | Typical contents and handling
--- | ---
`generated/` | Default `-genDir` output: Bind sidecars, the structured-field registry, optional one-line JSON views, and generated C tables. Bind projects add it to the compiler include path. See [persistent and generated files](rm/bind.md#persistent-and-generated-files).
`build/` or example-specific output folders | Compiler output. The example's build script determines the path.
`dist/` | Local GoReleaser archives and release artifacts.
`temp/` | Local recordings, test logs, and recovery snapshots. Failure details from `testAll.sh` are under `temp/log`.
`.gocache/`, `coverage.out`, and other local tool outputs | Build/test caches and coverage results; not firmware inputs.

`til.json` maps IDs to messages; `li.json` connects IDs to source positions and helps ID processing reuse existing assignments, including identical messages at different sites. Keep the local LI between builds even when logging without source locations: `trice log -li off` disables location output without disabling LI use during `bind` or `insert`.

The root `demoTIL.json` serves repository examples and tests. Their build and ID-workflow scripts initialize a missing `demoLI.json` or project-local `li.json` and regenerate it; these active demo location tables are ignored by Git. The historical test fixture remains versioned. Your application should use its own files. Preserve dictionaries and matching location tables needed for released firmware and recordings. Generated sidecars can also contain historical ID bindings, so do not assume every file in `generated` can be deleted without reviewing its role. See [which files to keep](./TriceUserManual.md#know-which-files-to-keep) and the read-only [Bind artifact report](rm/generate.md#bind-artifact-report).
