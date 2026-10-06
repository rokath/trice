# Trice — compact logging for embedded C/C++

Write readable log calls in your firmware. Trice sends compact binary records containing an ID and runtime values; the host tool turns them into readable messages or structured data. Format strings stay in the host dictionary, saving target flash and transport bandwidth.

```c
trice("info:Supply {voltage_mv:%u} mV", voltage_mv);
```

With `voltage_mv = 3300`, the same record can be shown as text:

```text
Supply 3300 mV
```

Or as JSON, with a numeric field ready for another tool:

```json
{"tag":"INFO","level":"INFO","message":"Supply 3300 mV","fields":{"voltage_mv":3300}}
```

These examples omit optional timestamp, source-location and prefix columns. The [User Manual](./docs/TriceUserManual.md#keep-values-as-fields) lets you try structured output on your PC.

## Start here

Follow the **[User Manual](./docs/TriceUserManual.md)** from your first PC log through fields, filters and context to your own firmware. No hardware is needed for the first experiments. The **[Reference Manual](./docs/TriceReferenceManual.md)** supplies complete syntax, configuration and limits when you need them.

For this checkout, build the matching host tools from the repository root using Go (the version required by [go.mod](./go.mod)) and Bash:

```sh
./scripts/buildTriceTool.sh
trice --version
```

The script prints where it installs `trice` and `tlog`; add that directory to `PATH`. The PC demos also need a native C compiler named `cc` or `gcc`. On Windows, run the shell scripts in Git Bash with a Windows host compiler.

Prebuilt tools are available from [GitHub Releases](https://github.com/rokath/trice/releases). Use the target sources and documentation belonging to the same release; a released binary may not contain the features of a newer checkout. See [firmware and host-tool compatibility](./docs/TriceReferenceManual.md#compatibility-with-firmware-and-host-tool-versions).

## What you can do

- **Keep firmware calls readable.** [Bind](./docs/TriceReferenceManual.md#trice-bind) generates ID sidecar headers before compilation. The source gets the required includes while its log calls remain ID-free. [Insert/clean](./docs/TriceReferenceManual.md#trice-id-management) remains a supported alternative.
- **Read logs or process fields.** [Structured Logging](./docs/TriceReferenceManual.md#structured-logging) produces text, key/value output or one JSON object per event (NDJSON) from the same records.
- **Add context to selected messages.** [Context Enrichment](./docs/TriceReferenceManual.md#trice-context-enrichment) can append task, cycle or application values through a build-time rule such as `-ce 'ctx:", cycle={cycle:%u}", pc_sample_phase'`. The [feature tours](./docs/TriceReferenceManual.md#pc-feature-tour) make the result visible.
- **Focus on the events you need.** Use [tags and severity thresholds](./docs/TriceReferenceManual.md#trice-tags-color-and-weights), [timestamps and deltas](./docs/TriceReferenceManual.md#trice-timestamps), and source locations.
- **Choose a suitable output path.** Integrate an existing byte writer, UART/USB serial or SEGGER RTT; use direct or buffered output. Trice's target logging path [needs no heap allocation](./docs/TriceReferenceManual.md#no-dynamic-memory-management-needed).
- **Go further when needed.** Explore [local target formatting](./docs/TriceReferenceManual.md#local-logging-example-projects), [live plotting](./docs/TriceReferenceManual.md#setting-up-the-labplot-demo), or [Asynchronous Broadcast Commands](./docs/TriceReferenceManual.md#trice-abc---asynchronous-broadcast-commands).

Preserve your project's `til.json`: it maps IDs to messages and is needed to decode captured records. The [generated-file layout](./docs/TriceReferenceManual.md#command-line) explains which other files to keep and how `generated/` fits into the build.

## Find your way around

| Your next task | Start here |
| --- | --- |
| Try a small application without hardware | [First-log walkthrough](./docs/TriceUserManual.md#see-your-first-log) using [demo](./demo/) |
| Experiment with fields, context and filters | [Guided experiments](./docs/TriceUserManual.md#try-fields-filters-and-context) using [PC_features](./examples/PC_features/) |
| Integrate Trice into firmware | [Integration steps](./docs/TriceUserManual.md#bring-trice-into-your-firmware); [src](./src/) contains the target library and [examples](./examples/) has STM32 projects |
| Look up syntax, configuration and limits | [Reference Manual](./docs/TriceReferenceManual.md) and [CLI help](./docs/ref/trice-help-all.txt) |
| Build or change the host tools | [Build script](./scripts/buildTriceTool.sh); entry points in [cmd](./cmd/), implementation in [internal](./internal/) and shared packages in [pkg](./pkg/) |
| Run tests or contribute | [CONTRIBUTING](./CONTRIBUTING.md); [testAll.sh](./scripts/testAll.sh), target configurations in [_test](./_test/), and helpers in [scripts](./scripts/) |

## Project information

Trice is open source under the [MIT License](./LICENSE.md). The [documentation guide](./docs/TriceDocsGuide.md) helps you choose the right reference. [Release notes](./CHANGELOG.md) describe published changes; compiler and advanced Bind limitations are documented under [bind-limits](./docs/TriceReferenceManual.md#bind-limits).

Questions and examples are welcome in [Discussions](https://github.com/rokath/trice/discussions). For a reproducible problem, open an [issue](https://github.com/rokath/trice/issues); for a change, start with [CONTRIBUTING](./CONTRIBUTING.md). You can also [sponsor the project](https://github.com/sponsors/rokath).
