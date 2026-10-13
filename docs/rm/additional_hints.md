# Additional hints

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Pre-built executables are available](#pre-built-executables-are-available)
* [2. Configuration file triceConfig.h](#configuration-file-triceconfigh)
* [3. Setting up the very first connection](#setting-up-the-very-first-connection)
* [4. Avoid buffer overruns](#avoid-buffer-overruns)
* [5. Buffer Macros](#buffer-macros)
* [6. Logfile viewing](#logfile-viewing)
* [7. Using the Trice tool with 3rd party tools](#using-the-trice-tool-with-3rd-party-tools)
* [8. Several targets at the same time](#several-targets-at-the-same-time)
* [9. Executing go test -race -count 100 ./...](#executing-go-test--race--count-100-)
* [10. TRICESTACKBUFFER could cause stack overflow with -o0 optimization](#tricestackbuffer-could-cause-stack-overflow-with--o0-optimization)
* [11. Cycle Counter](#cycle-counter)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## 1. <a id="pre-built-executables-are-available"></a>Pre-built executables are available

See [https://github.com/rokath/trice/releases](https://github.com/rokath/trice/releases).

## 2. <a id="configuration-file-triceconfigh"></a>Configuration file triceConfig.h

* When setting up your first project you need a `triceConfig.h` file.
* You should **not** use the `./_test/cgo.../triceConfig.h` directly, because these are customized for internal tests with CGO. But you can use their settings as helper for a starting point.
* Please choose one of these files as starting point:
  *  [../examples/F030_inst/Core/Inc/triceConfig.h](../../examples/F030_inst/Core/Inc/triceConfig.h)
  *  [../examples/G0B1_inst/Core/Inc/triceConfig.h](../../examples/G0B1_inst/Core/Inc/triceConfig.h)
  *  [../examples/L432_inst/Core/Inc/triceConfig.h](../../examples/L432_inst/Core/Inc/triceConfig.h)
* Comparing them and understandig the differences helps quick starting.
* The file [triceDefaultConfig.h](../../src/triceDefaultConfig.h) contains all possible config keys with descriptions.

## 3. <a id="setting-up-the-very-first-connection"></a>Setting up the very first connection

If you see nothing in the beginning, what is normal ;-), add the `-s` (`-showInputBytes`) switch to see if any data arrive. There is also a switch `-debug` showing you the received packages, if you are interested in.

## 4. <a id="avoid-buffer-overruns"></a>Avoid buffer overruns

It is your responsibility to produce less data than transmittable. If this is not guarantied, a data loss is not avoidable or you have to slow down the user application. The buffers have an optional overflow protection (`TRICE_PROTECT`), which is enabled by default. Recommendation: Make the buffer big and emit the maxDepth cyclically, every 10 or 1000 seconds. Then you know the needed size. It is influenced by the max Trice data burst and the buffer switch interval. See [./examples/exampleData/triceLogDiagData.c](../../examples/exampleData/triceLogDiagData.c) for help.

If the target application produces more Trice data than transmittable, a buffer overrun can let the target crash, because for performance reasons no overflow check is implemented in versions before v0.65.0. Such a check is added now per default using `TRICE_PROTECT`, but the Trice code can only throw data away in such case. Of course you can disable this protection to get more speed.

Configuring the ring buffer option with `TRICE_PROTECT == 0` makes buffer overruns not completely impossible, because due to partial Trice log overwrites, false data are not excluded anymore and overwriting the buffer boundaries is possible, because of wrong length information. Also losses will occur when producing more data than transmittable. This is detectable with the cycle counter. The internal 8-bit cycle counter is usually enabled. If Trice data are lost, the receiver side will detect that because the cycle counter is not as expected. There is a chance of 1/256 that the detection does not work for a single case. You can check the detection by unplugging the trice UART cable for a time. Also resetting the target during transmission should display a cycle error.

Gennerally it is recommended to enable `TRICE_PROTECT` during development and to disable it for performance, if you are 100% sure, that not more data are producable than transmittable.

Important to know: If the `TRICE_PROTECT` code inhibits the writing into a buffer, there will be later no cycle error because a non existing Trice cannot cause a cycle error. Therefore the `TriceDirectOverflowCount` and `TriceDeferredOverflowCount` values exist, which could be monitored.

## 5. <a id="buffer-macros"></a>Buffer Macros

(Examples in [../_test/testdata/triceCheck.c](../../_test/testdata/triceCheck.c))

| Macro Name                                      | Description                                                                                                                                                                                                                                                                                                                   |
|-------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `triceS`  \|`TriceS`  \|`TRiceS`  \|`TRICE_S`   | Output of runtime generated 0-terminated strings.                                                                                                                                                                                                                                                                             |
| `triceN`  \|`TriceN`  \|`TRiceN`  \|`TRICE_N`   | Is for byte buffer output as string until the specified size. It allows limiting the string size to a specific value and does not rely on a terminating 0. If for example len = 7 is given and "Hello\0World\n" is in the buffer, the byte sequence "Hello\0W" is transmitted but the trice tool probably shows only "Hello". |
| `triceB`  \|`TriceB`  \|`TRiceB`  \|`TRICE_B`   | Is buffer output according to the given format specifier for a default unit according to configuration (8\|16\|32\|64-bit value) - default is `#define TRICE_B TRICE8_B`.                                                                                                                                                     |
| `trice8B` \|`Trice8B` \|`TRice8B` \|`TRICE8_B`  | Is for byte buffer output according to the given format specifier for a single byte.                                                                                                                                                                                                                                          |
| `trice16B`\|`Trice16B`\|`TRice16B`\|`TRICE16_B` | Is for 16-bit buffer output according to the given format specifier for a 16-bit value.                                                                                                                                                                                                                                       |
| `trice32B`\|`Trice32B`\|`TRice32B`\|`TRICE32_B` | Is for 32-bit buffer output according to the given format specifier for a 32-bit value.                                                                                                                                                                                                                                       |
|                                                 | See chapter [Trice ABC - Asynchronous Broadcast Commands](trice_abc_asynchronous_broadcast_commands.md) for the following lines.                                                                                                                                                                                              |
| `triceC`  \|`TriceC`  \|`TRiceC`  \|`TRICE_C`   | Is for ABC buffer output according to the given function handler for a default unit according to configuration (8\|16\|32\|64-bit value) - default is `#define TRICE_C TRICE8_C`.                                                                                                                                             |
| `trice8C` \|`Trice8C` \|`TRice8C` \|`TRICE8_C`  | Is for ABC byte buffer output according to the given function handler.                                                                                                                                                                                                                                                        |
| `trice16C`\|`Trice16C`\|`TRice16C`\|`TRICE16_C` | Is for ABC 16-bit buffer output according to the given function handler for a 16-bit wide buffer.                                                                                                                                                                                                                             |
| `trice32C`\|`Trice32C`\|`TRice32C`\|`TRICE32_C` | Is for ABC 32-bit buffer output according to the given function handler for a 32-bit wide buffer.                                                                                                                                                                                                                             |

## 6. <a id="logfile-viewing"></a>Logfile viewing

Logfiles, Trice tool generated with sub-command switch `-color off`, are normal ASCII files. If they are with color codes, these are ANSI escape sequences.

* Simply `cat trice.log`. One view option is also `less -R trice.log`. The Linux command `less` is also available inside the windows git bash.
* Under Windows one could also download and use [ansifilter](https://sourceforge.net/projects/ansifilter/) for logfile viewing. A monospaced font is recommended.
* See also [Color issues under Windows](trice_tags_color_and_weights.md#color-issues-under-windows)

## 7. <a id="using-the-trice-tool-with-3rd-party-tools"></a>Using the Trice tool with 3rd party tools

Parallel output as logfile, TCP or binary logfile is possible. See examples above.

## 8. <a id="several-targets-at-the-same-time"></a>Several targets at the same time

You can connect each target over its transmit channel with an own Trice instance and integrate all transmissions line by line in an additional Trice instance acting as display server. See [https://github.com/rokath/trice#display-server-option](https://github.com/rokath/trice#display-server-option).

## 9. <a id="executing-go-test--race--count-100-"></a>Executing `go test -race -count 100 ./...`

The C-code is executed during some tests. Prerequisite is an installed GCC.

## 10. <a id="tricestackbuffer-could-cause-stack-overflow-with--o0-optimization"></a>TRICE_STACK_BUFFER could cause stack overflow with -o0 optimization

As discussed in [issue #294](https://github.com/rokath/trice/issues/294) it can happen, that several TRICE macros within one function call increase the stack usage more than expected, when compiler optimization is totally switched off.

## 11. <a id="cycle-counter"></a>Cycle Counter

* The trice tool expects the first cycle counter to start with 0xC0 (=192). If the target is already running and you connect the trice tool then, the first message is marked with "CYCLE: ? not equal expected value 192 - adjusting. Now 1 CycleEvents".
* If the target is resetted asynchronous, the trice tool receives a cycle counter 192. Most probably the last cycle counter was not 191, so this triggers also a message  with "CYCLE: 192 not equal expected value ?- adjusting. Now n CycleEvents".
* In the Trice tool is some heuristics to suppress such obvious false positives.

<p align="right">(<a href="#additional-hints">back to top</a>)</p>

