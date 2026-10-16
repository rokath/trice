# Trice Overview and Concepts

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. How it works](#how-it-works)
* [2. Direct and deferred output](#direct-and-deferred-output)
* [3. Compact records](#compact-records)
* [4. Portability and modularity](#portability-and-modularity)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

**New to Trice?** Follow the [User Manual](../TriceUserManual.md) for your first PC log and firmware integration. This reference explains the complete options, rules and limits. Use the [Guide](../TriceGuide.md) to find a task.

## 1. <a id="how-it-works"></a>How it works

A Trice call records an ID and binary values. The host looks up the format in `til.json` and formats the result. Ordinary host-decoded logging therefore keeps format strings and printf formatting out of the firmware. [Local text logging](target_implementation.md#local-deferred-text-log) is a separate configuration that performs formatting on the target.

```text
Source calls → trice bind → sidecar headers + dictionaries → compiler
Target records → byte transport → host decoder + til.json → readable events
```

[Bind](bind.md) is the normal pre-build step. It creates compiler headers and maintains their includes in the source; log calls remain ID-free. Preserve the accumulated dictionary and local location information according to [IDs and dictionaries](id_management.md). The older [Insert/Clean workflow](insert_clean_cache.md) is deprecated.

![Trice encoding and decoding](../ref/triceCOBSBlockDiagram.svg)

## 2. <a id="direct-and-deferred-output"></a>Direct and deferred output

| Mode | Work done by a log call | Application responsibility |
| --- | --- | --- |
| Direct | Creates a record and passes it through the configured direct output path. | Provide an output callback with acceptable execution time. |
| Deferred | Stores a record in a ring or double buffer. | Call `TriceTransfer()` regularly to encode and drain pending records. |

Both modes can be enabled together; see [F030_inst](examples.md#f030inst). Deferred mode reduces the immediate work at the call site but still needs time for transfer. Choose buffers using the [performance measurements](performance.md) and [memory reference](memory_usage.md); the fastest measured configuration is not a guarantee for every target.

## 3. <a id="compact-records"></a>Compact records

Without a target stamp or payload, a record needs four bytes. Stamps, payload, padding and transport framing add bytes. See [binary layout and size limits](binary_encoding.md) for the exact representation, including the distinction between the format limit and the C buffer allowance.

## 4. <a id="portability-and-modularity"></a>Portability and modularity

The Go host tools run on Linux, macOS and Windows. The target C library supports different integer widths and byte orders; your project supplies hardware-specific configuration and transport functions. Compiler-dependent wrapper features have explicit [Bind limits](bind.md#bind-limits).

Use [UART, RTT or a custom writer](custom_output.md) to carry the bytes. [typeX0](typex0_user_packets.md) allows application packets alongside Trice records. Host [filters](tags_color_and_weights.md), [structured output](structured_logging.md), [context enrichment](context_enrichment.md) and [visualization](visualization.md) have their own reference chapters.

The project uses the [MIT license](../../LICENSE.md); see [resources and support](project_resources.md).

