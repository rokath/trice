# Trice User Manual

Trice gives C code readable log calls without the need storing their format strings in the binary image. The target sends 4-byte binary records containing an ID with cycle & payload size followed by runtime values. The host tool combines them with a dictionary, `til.json`, to display text or structured data.

Start on your PC: no board, probe or serial cable is needed. This guide then takes the same workflow into your firmware. Use the [Reference Manual](./TriceReferenceManual.md) for complete syntax, configuration and limits.

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

- [1. Get lib and tools](#get-lib-and-tools)
- [2. See your first log](#see-your-first-log)
- [3. Try fields, filters and context](#try-fields-filters-and-context)
  - [3.1. Keep values as fields](#keep-values-as-fields)
  - [3.2. Select messages by tag or priority](#select-messages-by-tag-or-priority)
  - [3.3. Read target stamps](#read-target-stamps)
  - [3.4. Add context with one build rule](#add-context-with-one-build-rule)
- [4. Bring Trice into your firmware](#bring-trice-into-your-firmware)
  - [4.1. Add the library and configuration](#add-the-library-and-configuration)
  - [4.2. Connect the writer and generate IDs](#connect-the-writer-and-generate-ids)
  - [4.3. Decode the bytes](#decode-the-bytes)
- [5. Know which files to keep](#know-which-files-to-keep)
- [6. Choose your next example](#choose-your-next-example)
- [7. If something does not work](#if-something-does-not-work)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## 1. <a id="get-lib-and-tools"></a>Get lib and tools

For the PC examples you need:

- A checkout or extracted source snapshot of Trice, including `demo`, `examples` and the library `src`.
- Bash and the usual shell utilities. On Windows, use Git Bash or s.th. similar.
- The host tools from [GitHub Releases](https://github.com/rokath/trice/releases) in the `PATH`, CLI check: `trice --version`.
- A **native host C compiler** named `cc` or `gcc` with its matching runtime libraries and headers; see [compiler setup](./TriceReferenceManual.md#development-environment-setup).

`trice` manages IDs and decodes logs; the companion `tlog` is a shortcut for `trice log`. The examples below use `trice`.

## 2. <a id="see-your-first-log"></a>See your first log

From the repository root:

```sh
cd demo
./demo_deferred.sh
```

The [script](../demo/demo_deferred.sh) assigns IDs, compiles the PC program, runs it and decodes its `log.bin` file. Ignoring optional prefix and source-location columns, you should see:

```text
Hello from deferred mode.
Deferred value=42.
```

**Deferred output** first puts records in a buffer. The application calls `TriceTransfer()` later to send them.

Open [demo/deferred/main.c](../demo/deferred/main.c), change the value `42` to `43`, and run `./demo_deferred.sh` again from `demo`. The message now reports `43`. You have changed firmware input, rebuilt it and decoded the resulting binary record.

**Bind** maintains the generated header includes; your calls stay readable, such as `trice("att:Deferred value=%d.\n", 43);`.

The same folder also contains a Direct Mode demo and a continuously running Live demo. See the [Reference Manual](./TriceReferenceManual.md#minimal-pc-demos-direct-and-deferred) for those examples and the full configuration details.

## 3. <a id="try-fields-filters-and-context"></a>Try fields, filters and context

The next project collects several small experiments in one program. Starting in `demo` after the previous section:

```sh
cd ../examples/PC_features
./build_and_run.sh
./show_text.sh
./show_json.sh
./show_kv.sh
```

These scripts decode the same `capture.bin`; choosing another output format does not rebuild the target. Relevant messages include `Device pump A`, two `Supply` readings, `Retry 2`, `Humidity 55 percent` and `A message without a tag`.

### 3.1. <a id="keep-values-as-fields"></a>Keep values as fields

The [source](../examples/PC_features/main.c) contains:

```c
TRice32("info:ctx:Supply {voltage_mv:%u} mV\n", voltage_mv);
```

`{voltage_mv:%u}` both displays an unsigned integer and gives it the field name `voltage_mv`. In JSON the first Supply record contains:

```json
"message":"Supply 3300 mV, cycle=7\n",
"fields":{"voltage_mv":3300,"cycle":7}
```

This is an excerpt; the complete event also contains tag and other enabled metadata. The `cycle` field comes from the context rule explained below. `-logFormat json` produces **NDJSON**: one complete JSON object per event, on one output line. KV produces readable `key=value` entries. Numeric fields remain numbers; the device-name call uses `TriceS` to send the runtime string `pump A`.

Try changing `3300u` in the first `emit_sample` call to `3400u`, rebuild, then run `./show_json.sh` again. Both the message and `fields.voltage_mv` change. Plain `%u` still works when you only need a displayed value. Named fields are supported for scalar calls and strings; named buffer fields are rejected. See [Structured Logging](./TriceReferenceManual.md#structured-logging) for field types, literal braces and buffer output.

### 3.2. <a id="select-messages-by-tag-or-priority"></a>Select messages by tag or priority

A tag is the prefix before the first colon, for example `info:` or `wrn:`. Try these from `examples/PC_features`:

```sh
./show_text.sh -pick info
./show_json.sh -logLevel wrn
./show_json.sh -ulabel sensor:650 -logLevel wrn
```

The first shows INFO events only. The second selects Warning and higher-priority events, including `Retry 2`. The third raises this tour's custom `sensor` tag above the Warning threshold, so `Humidity 55 percent` also appears. This is useful when you want all serious messages rather than a list of individual tags. Higher weights mean higher priority; a lower threshold admits more events.

Built-in tag aliases such as `wrn`, `WARNING` and `Wrn` identify the same group for selection. Lowercase tags normally disappear from the displayed message; mixed/uppercase ones remain visible. A message without a recognized tag remains as written: `untagged` is classification metadata, not a prefix the tool invents in its message. Details: [tags, weights and selection](./TriceReferenceManual.md#trice-tags-color-and-weights).

### 3.3. <a id="read-target-stamps"></a>Read target stamps

Macro capitalization selects the stamp carried by a record: `trice` has none, `Trice` has 16 bits and `TRice` has 32 bits. A numeric suffix such as `32` in `TRice32` instead selects the parameter bit width.

This tour deliberately uses two different meanings. `TriceStamp16` reads `pc_sample_phase`; `TriceStamp32` reads `pc_sample_milliseconds`, as defined in [triceConfig.h](../examples/PC_features/triceConfig.h). The host scripts choose how to display each. The two Supply calls carry 32-bit values `100` and `125`, shown as milliseconds; the second has a delta of `25`. Humidity carries a 16-bit phase value of `7`.

```sh
./show_json.sh -ts16 'phase:%d' -ts32 ms -ts32delta ms
```

Change `pc_sample_milliseconds = 125u` to `150u`, rebuild and decode again: the second Supply stamp and its delta become `150` and `50`. The host cannot infer units; a 16-bit stamp could equally represent a temperature or application counter. See [timestamps and deltas](./TriceReferenceManual.md#trice-timestamps).

### 3.4. <a id="add-context-with-one-build-rule"></a>Add context with one build rule

**Context Enrichment (CE)** appends application values to selected log calls during ID generation. The [build script](../examples/PC_features/build_and_run.sh) already passes:

```sh
-ce 'ctx:", cycle={cycle:%u}", pc_sample_phase'
```

This is an option fragment for `trice bind`, not a separate shell command. `ctx:` selects the second tag in `info:ctx:Supply ...`. The quoted text is appended to the message, and `pc_sample_phase` is evaluated at that call site. The same `emit_sample` function is called with phase values `7` and `11`, so its two records gain different `cycle` values without duplicating the format text at every caller.

Try changing `cycle` to `phase` in both places inside the CE format (`phase={phase:%u}`), then rebuild and decode. The displayed label and JSON field name change together. Every selected call must be able to access the expression. Current `bind -ce` supports direct, uniquely addressable calls; selected wrapper/rebase sites are rejected. CE does not append runtime parameters to `triceS`/`triceN`; use their own string argument for runtime text. These boundaries and the alternative `insert/clean -ce` workflow are explained in [Context Enrichment](./TriceReferenceManual.md#trice-context-enrichment).

When you edit an example, its output check may correctly fail because it still expects the original values. Run `./check_output.sh` on the unchanged tour; after intentional edits, inspect the new output and update the corresponding expectations. The [guide to adapting these checks](./TriceReferenceManual.md#updating-the-pc-tours-output-checks) explains where they come from.

## 4. <a id="bring-trice-into-your-firmware"></a>Bring Trice into your firmware

Start with deferred logging through an existing byte-output function, such as a UART/USB transmit queue. The PC deferred demo demonstrates the same separation between collecting and sending records. If you already use J-Link, [the RTT quickstart](./TriceReferenceManual.md#quickstart-segger-rtt-direct-mode-with-j-link) is an alternative.

### 4.1. <a id="add-the-library-and-configuration"></a>Add the library and configuration

Copy or reference the [target library](../src/) from the same checkout as your host tool. Add its C implementation files to your build, excluding the optional SEGGER RTT implementation unless you use it. Compile the library as C even when your application uses C++. Keep your own `triceConfig.h` in the application's include directory; do not edit the library defaults.

For a first single-context application, the essential configuration is:

```c
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_AUXILIARY8 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_TCOBS

#endif
```

Before logging from interrupts or multiple tasks, define real `TRICE_ENTER_CRITICAL_SECTION` / `TRICE_LEAVE_CRITICAL_SECTION` protection appropriate to your MCU and scheduler. The default empty protection is only a starting point for a single context. Buffer sizing and the complete setup are in the [byte-writer quickstart](./TriceReferenceManual.md#quickstart-existing-non-blocking-byte-writer-deferred-auxiliary-8-bit).

### 4.2. <a id="connect-the-writer-and-generate-ids"></a>Connect the writer and generate IDs

Adapt the following outline to your existing initialization and main loop:

```c
#include "trice.h"

static void WriteTriceBytes(const uint8_t* data, size_t length) {
    /* Copy into the application's bounded transmit queue before returning. */
    ExistingTxQueueWrite(data, length);
}

void AppInit(void) {
    BoardInit();
    TriceInit();
    UserNonBlockingDeferredWrite8AuxiliaryFn = WriteTriceBytes;
    trice("info:Firmware started\n");
}

void AppMainLoop(void) {
    for (;;) {
        AppRun();
        TriceTransfer();
    }
}
```

`BoardInit`, `AppRun` and `ExistingTxQueueWrite` are your application's functions. The writer must accept the complete chunk or implement an explicit overflow policy. It must not retain the supplied pointer after returning; a DMA queue needs its own copy or another correctly managed transport adapter. Trice's target logging path needs no heap allocator, but that says nothing about allocations in your writer or other application functions.

Use **Bind** as the pre-build step. For a project whose application sources and configuration live in `application`, run from that project's root:

```sh
touch til.json li.json
trice bind -src application -genDir generated -til til.json -li li.json
```

For a new project, `touch` creates the initially empty dictionaries; it leaves existing contents intact. Bind deliberately rejects a missing TIL instead of silently replacing a lost dictionary with new IDs.

Add the application configuration and `generated` directories first, then the library's `src` directory, and finally `src/default_conf` to the compiler's include path. This keeps your own `SEGGER_RTT_Conf.h` ahead of the [RTT fallback configuration](./TriceReferenceManual.md#trice-over-rtt). Bind inserts generated includes and writes the corresponding **sidecars** (headers alongside the normal source tree). Run it again before compilation whenever log sources change; make it a dependency of compilation in your build system. Then build and flash normally.

Direct calls on separate source lines do not require `__COUNTER__`. If a more complex construct is rejected, search the RM for [bind-limits](./TriceReferenceManual.md#bind-limits). [Insert/clean](./TriceReferenceManual.md#trice-id-management) remains a supported alternative that writes IDs into source calls. For an already bound project, follow [re-migration](./TriceReferenceManual.md#re-migration-to-trice-insert) rather than mixing the two workflows by hand.

### 4.3. <a id="decode-the-bytes"></a>Decode the bytes

From the firmware project root, use its matching dictionary and your actual serial port and baud rate:

```sh
trice log -p COM7 -baud 115200 -pf TCOBSv1 -til til.json -li li.json
```

Replace `COM7` with your device's port name; this command assumes the framed auxiliary-byte configuration above. The result includes `Firmware started`. Binary bytes are expected on the wire, so a plain terminal will not display the original text. Other transport/configuration choices need matching decoder settings; copy the log script of the corresponding [working example](./TriceReferenceManual.md#example-projects-without-and-with-trice-instrumentation).

The same captured records can later be decoded with `-logFormat json` or `-logFormat kv`. Keep the relevant dictionary with the firmware or capture. While logging, the host automatically reloads changes to its existing TIL/LI files; see [automatic reload](./TriceReferenceManual.md#easy-to-use).

## 5. <a id="know-which-files-to-keep"></a>Know which files to keep

| File or directory | What to do with it |
| --- | --- |
| Application sources and `triceConfig.h` | Keep under version control, including the generated includes added to sources. |
| `til.json` | Keep the accumulated ID-to-format dictionary. Preserve a matching copy with firmware/captures you need to decode later. |
| `li.json` | Keep the corresponding source-location table when using locations. |
| `generated/` | Default `-genDir`, relative to where you run the command. Contains sidecars and the field registry; add it to the compiler include path. |
| Executables, object files and `capture.bin` / `log.bin` | Build/run outputs; retain captures if they contain useful diagnostic data. |

Do not make deleting all of `generated/` your routine cleanup: ABC can place a user-edited command-selection header there, and old sidecars can retain ID history. `trice generate -bindReport` gives a read-only inventory; it is not permission to delete every unreferenced file. Details and optional `-logC`/ABC outputs are in the [generated-file layout](./TriceReferenceManual.md#command-line).

## 6. <a id="choose-your-next-example"></a>Choose your next example

| What you want to try | Project and next experiment | Full instructions |
| --- | --- | --- |
| Task context on an MCU | [G0B1_features](../examples/G0B1_features/): the same logging function called by two FreeRTOS tasks gains different task handles through CE. | [Build, flash and capture](./TriceReferenceManual.md#g0b1-feature-tour) |
| Integration changes in an STM32 project | Compare [F030_bare](../examples/F030_bare/) with [F030_inst](../examples/F030_inst/). | [Project setup](./TriceReferenceManual.md#f030inst) |
| Text generated on the target | [PC_log](../examples/PC_log/): build and run the local formatter without a host decoder. | [Local logging](./TriceReferenceManual.md#pc-local-logging) |
| Live plots | [LabPlotDemo](../examples/LabPlotDemo/): forward selected measurements to LabPlot. | [Plotting setup](./TriceReferenceManual.md#setting-up-the-labplot-demo) |
| Commands between nodes | [TriceAbc](../examples/TriceAbc/): run a host-native broadcast demo and change a command. | [ABC example](./TriceReferenceManual.md#example-examplestriceabc) |

These projects have different prerequisites. In particular, the G0B1 build needs an ARM toolchain; flashing and capturing require the board and probe. A successful cross-build does not mean firmware has run on hardware.

## 7. <a id="if-something-does-not-work"></a>If something does not work

| Symptom | Next check |
| --- | --- |
| `trice` is not found or a documented option is unknown | Check `PATH` and `trice --version`; build from the same checkout as the example. |
| PC compilation fails on standard headers | Use a native host compiler with its runtime/SDK, not `arm-none-eabi-gcc`. Follow the [compiler setup](./TriceReferenceManual.md#development-environment-setup). |
| A generated header is missing or points at a wrong call | Run Bind from the intended project root and check the generated include path. Do not edit individual sidecar definitions. See [Bind diagnostics](./TriceReferenceManual.md#diagnostics-and-troubleshooting). |
| The target runs but no bytes arrive | Check the writer, output configuration and regular `TriceTransfer()` calls for deferred mode before changing decoder options. |
| Bytes arrive but cannot be decoded | Check the matching `til.json`, port speed, framing and options against the project's log script. |
| Messages disappear after adding a filter | Remove `-pick`, `-ban` and `-logLevel` temporarily; check the tag or custom-tag weight. |
| CE reports an unknown identifier or unsupported site | Make the expression accessible at every selected call; follow [bind-limits](./TriceReferenceManual.md#bind-limits) for wrappers and alternatives. |
| An example's output check fails after an edit | Compare actual output with the new intended values, then [update the expectations](./TriceReferenceManual.md#updating-the-pc-tours-output-checks). |

For further examples and checks, inspect [triceCheck.c](../_test/testdata/triceCheck.c), the [PC tour's output assertions](../examples/PC_features/check_output.sh), and the [logging-feature test entry point](../scripts/_515_test_logging_features.sh). They are optional follow-up reading. The [Reference Manual](./TriceReferenceManual.md) and `trice help -all` remain the complete lookup sources.
