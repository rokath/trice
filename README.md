# Trice — Trace IDs for Embedded C

[![License](https://img.shields.io/github/license/rokath/trice)](./LICENSE.md)
[![Latest release](https://img.shields.io/github/v/release/rokath/trice)](./docs/TriceGuide.md#project-resources)
[![Downloads](https://img.shields.io/github/downloads/rokath/trice/total)](./docs/TriceGuide.md#project-resources)
[![Target library CI](https://github.com/rokath/trice/actions/workflows/trice_lib_ci_full.yml/badge.svg)](./.github/workflows/trice_lib_ci_full.yml)

<div align="right">

*Hi, I am Trice.*

</div>
<img align="right" src="docs/ref/TriceGirl-167x222.png" width="115" alt="Trice project mascot">

**Log and trace in as few as 6 CPU clocks per call.**[^speed]<br>
Printf-like calls.[^calls] Compact records.[^records] Even from interrupt handlers.[^interrupts]<br>
Compiler-independent.[^compiler] ID-free user code.[^ids] Aliases for legacy code.[^aliases]

**[Explore Trice in the Guide](./docs/TriceGuide.md)**

See the moments that matter: a fleeting interrupt, a changing sensor value, or the response to a command.[^commands] Trice brings them into view while your firmware runs. Read events as text,[^text] work with structured values,[^fields] or watch measurements come alive in a plot.[^plots]

The Guide helps you choose your first experiment, discover the features, and find the details for your own project. Start on your PC, then explore what Trice can do on your hardware.

<details markdown="1"><summary>Small demo animation</summary>

![Colored Trice output with source locations and timestamps](./docs/ref/life0.gif)

</details>

If Trice helps your work, [support the project](./docs/TriceReferenceManual.md#support-and-sponsoring).

[^speed]: Record creation in an optimized configuration; transmission is handled separately. See the [measurement and configuration](./docs/TriceReferenceManual.md#trice-speed).
[^calls]: [Printf-like syntax and supported formats](./docs/TriceReferenceManual.md#trice-similarities-and-differences-to-printf-usage).
[^records]: [Compact binary records and transfer size](./docs/TriceReferenceManual.md#minimal-transfer-bytes-amount).
[^interrupts]: Trice calls can be used directly inside interrupt handlers. The [example output](./docs/TriceReferenceManual.md#trice-tool-in-logging-action) shows interrupt-originated `ISR:` messages mixed with events from normal application code.
[^compiler]: [Target-code portability and modularity](./docs/TriceReferenceManual.md#portability-and-modularity).
[^ids]: [Bind](./docs/TriceReferenceManual.md#trice-bind) keeps numeric IDs out of user log calls; see its [supported constructs and compiler requirements](./docs/TriceReferenceManual.md#bind-limits).
[^aliases]: [Aliases for existing logging and assert macros](./docs/TriceReferenceManual.md#legacy-user-code-option-trice-aliases-adaptation).
[^commands]: [Asynchronous Broadcast Commands and responses](./docs/TriceReferenceManual.md#trice-abc---asynchronous-broadcast-commands).
[^text]: [Text output and event boundaries](./docs/TriceReferenceManual.md#output-formats-and-event-boundaries).
[^fields]: [Structured Logging with named values](./docs/TriceReferenceManual.md#structured-logging).
[^plots]: [Live visualization with the LabPlot demo](./docs/TriceReferenceManual.md#setting-up-the-labplot-demo).
