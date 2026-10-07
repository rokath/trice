# Trice — Trace IDs for Embedded C

[![License](https://img.shields.io/github/license/rokath/trice)](./LICENSE.md)
[![Latest release](https://img.shields.io/github/v/release/rokath/trice)](./docs/TriceGuide.md#project-resources)
[![Downloads](https://img.shields.io/github/downloads/rokath/trice/total)](./docs/TriceGuide.md#project-resources)
[![Target library CI](https://github.com/rokath/trice/actions/workflows/trice_lib_ci_full.yml/badge.svg)](./.github/workflows/trice_lib_ci_full.yml)

<div align="right">

*Hi, I am Trice.*

</div>
<img align="right" src="docs/ref/TriceGirl-167x222.png" width="115" alt="Trice project mascot">

**Log and trace in as few as 6 CPU clocks per call.**<sup>[1](#note-speed)</sup><br>
Printf-like calls.<sup>[2](#note-calls)</sup> Compact records.<sup>[3](#note-records)</sup> Even from interrupt handlers.<sup>[4](#note-interrupts)</sup><br>
Compiler-independent.<sup>[5](#note-compiler)</sup> ID-free user code.<sup>[6](#note-ids)</sup> Aliases for legacy code.<sup>[7](#note-aliases)</sup>

**[Explore Trice in the Guide](./docs/TriceGuide.md)**

See the moments that matter: a fleeting interrupt, a changing sensor value, or the response to a command.<sup>[8](#note-commands)</sup> Trice brings them into view while your firmware runs. Read events as text,<sup>[9](#note-text)</sup> work with structured values,<sup>[10](#note-fields)</sup> or watch measurements come alive in a plot.<sup>[11](#note-plots)</sup>

The Guide helps you choose your first experiment, discover the features, and find the details for your own project. Start on your PC, then explore what Trice can do on your hardware.

<details markdown="1"><summary>Small demo animation</summary>

![Colored Trice output with source locations and timestamps](./docs/ref/life0.gif)

</details>

If Trice helps your work, [support the project](./docs/TriceReferenceManual.md#support-and-sponsoring).

---

Footnotes:
<!-- Keep these links in an ordinary list: GitHub does not rewrite relative document links inside Markdown footnotes. -->

1. <a id="note-speed"></a>Record creation in an optimized configuration; transmission is handled separately. See the [measurement and configuration](./docs/TriceReferenceManual.md#trice-speed).
2. <a id="note-calls"></a>[Printf-like syntax and supported formats](./docs/TriceReferenceManual.md#trice-similarities-and-differences-to-printf-usage).
3. <a id="note-records"></a>[Compact binary records and transfer size](./docs/TriceReferenceManual.md#minimal-transfer-bytes-amount).
4. <a id="note-interrupts"></a>Trice calls can be used directly inside interrupt handlers. The [example output](./docs/TriceReferenceManual.md#trice-tool-in-logging-action) shows interrupt-originated `ISR:` messages mixed with events from normal application code.
5. <a id="note-compiler"></a>[Target-code portability and modularity](./docs/TriceReferenceManual.md#portability-and-modularity).
6. <a id="note-ids"></a>[Bind](./docs/TriceReferenceManual.md#trice-bind) keeps numeric IDs out of user log calls; see its [supported constructs and compiler requirements](./docs/TriceReferenceManual.md#bind-limits).
7. <a id="note-aliases"></a>[Aliases for existing logging and assert macros](./docs/TriceReferenceManual.md#legacy-user-code-option-trice-aliases-adaptation).
8. <a id="note-commands"></a>[Asynchronous Broadcast Commands and responses](./docs/TriceReferenceManual.md#trice-abc---asynchronous-broadcast-commands).
9. <a id="note-text"></a>[Text output and event boundaries](./docs/TriceReferenceManual.md#output-formats-and-event-boundaries).
10. <a id="note-fields"></a>[Structured Logging with named values](./docs/TriceReferenceManual.md#structured-logging).
11. <a id="note-plots"></a>[Live visualization with the LabPlot demo](./docs/TriceReferenceManual.md#setting-up-the-labplot-demo).
