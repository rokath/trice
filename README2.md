# Trice - printf-like embedded logging without target strings

[![License](https://img.shields.io/github/license/rokath/trice)](./LICENSE.md)
[![Latest release](https://img.shields.io/github/v/release/rokath/trice)](https://github.com/rokath/trice/releases/latest)
[![CI](https://github.com/rokath/trice/actions/workflows/trice_lib_ci_full.yml/badge.svg)](https://github.com/rokath/trice/actions/workflows/trice_lib_ci_full.yml)
[![Go Report Card](https://goreportcard.com/badge/github.com/rokath/trice)](https://goreportcard.com/report/github.com/rokath/trice)

**Trice** is a low-overhead logging and tracing system for embedded C/C++ firmware.
It gives you `printf`-style source code while the target sends compact binary records: an ID plus the argument values. The PC reconstructs the readable text by using `til.json`.

Use Trice when ordinary `printf` logging is too slow, too large, or too intrusive for interrupt handlers, timing-sensitive code, or small microcontrollers.

If you know Rust's `defmt`: Trice solves a similar problem for C/C++ firmware.

## What Trice does

```c
trice("adc=%d mV\n", adc_mV);
```

The target does **not** format that string. After ID insertion and compilation, the target sends a compact binary record. The PC-side `trice` tool decodes it back to readable text:

```text
adc=1234 mV
```

This keeps format strings out of the target image, reduces transfer bandwidth, and avoids running a `printf` formatter on the MCU.

## Why use it

- **Fast target-side log calls** - down to only a few CPU cycles in the most optimized configurations; realistic configurations stay far below normal `printf` cost.
- **Small target image** - format strings are kept in `til.json`, not in target FLASH.
- **Good for interrupts and timing work** - use deferred buffering for the shortest target execution path, or direct RTT for very convenient interactive development.
- **Readable C/C++ source** - log statements still look close to `printf`.
- **Long-term field decoding** - keep the matching `til.json` to decode old binary logs later.
- **Portable tooling** - the host tool is written in Go and runs on common desktop platforms.

## The normal workflow

1. Add the Trice target sources from [`src/`](./src) to your firmware project.
2. Add a project-specific `triceConfig.h`.
3. Write Trice calls in your code.
4. Run `trice insert` before compiling. This assigns stable IDs and updates `til.json` and `li.json`.
5. Build and flash the target.
6. Run `trice log` on the PC to decode the binary stream.

Minimal example:

```c
#include "trice.h"

int main(void) {
    BoardInit();
    TriceInit();

    trice("boot\n");
    trice("adc=%d mV\n", adc_mV);

    for (;;) {
        AppRun();
        TriceTransfer(); // needed for deferred outputs
    }
}
```

Create the ID files once, then insert IDs:

```bash
touch til.json li.json
trice insert -src ./ -i ./til.json -li ./li.json
```

Then start logging, for example from a serial/VCOM port:

```bash
trice log -p COM15 -baud 921600 -i ./til.json -li ./li.json
# or, on Linux/macOS:
trice log -p /dev/ttyACM0 -baud 921600 -i ./til.json -li ./li.json
```

## Direct mode and deferred mode

Trice has two main runtime styles.

| Mode | What happens during the log call | Good for |
| --- | --- | --- |
| **Direct mode** | Each Trice record is written immediately to the selected output backend. | Interactive development, especially SEGGER RTT. |
| **Deferred mode** | The log call writes only into a target buffer; `TriceTransfer()` sends later. | UART, USB CDC, existing writer functions, production-style logging, shortest target execution path. |

A practical rule:

- Start with **direct SEGGER RTT** when you have J-Link and want the easiest lab setup.
- Start with **deferred auxiliary 8-bit output** when your project already has a write function.
- Use **deferred ring buffer** for a balanced RAM/speed setup.
- Use **deferred double buffer** when minimizing the target-side log-call cost is more important than RAM.

See [mode and transport selection](./docs/TriceUserManualExtension.md#mode-and-transport-selection) for more detail.

## What about source modification?

Trice assigns IDs by processing source files with `trice insert`. A call such as this:

```c
trice("boot\n");
```

becomes similar to this before compilation:

```c
trice(iD(12345), "boot\n");
```

This is intentional: the target binary contains the ID, while the host keeps the text in `til.json`.

You can choose how visible those IDs are in your workflow:

- keep inserted IDs in source,
- run `trice clean` after the build,
- use the Trice cache to avoid needless rebuilds of unchanged files.

For team projects, define this policy early and keep `til.json` under control like any other build artifact needed for field diagnostics. See [ID workflow notes](./docs/TriceUserManualExtension.md#id-workflow-notes) and [Trice ID management](./docs/TriceUserManual.md#trice-id-management).

## Installation

Use one of these options:

```bash
# macOS with Homebrew
brew install rokath/tap/trice
```

Or download a release binary from [the latest release](https://github.com/rokath/trice/releases/latest) and put `trice` into your `PATH`.

Developers with Go installed can also build/install from source:

```bash
go install github.com/rokath/trice/cmd/trice@latest
```

Check the installation:

```bash
trice --version
trice help -insert
trice help -log
```

## Documentation

- [Trice User Manual](./docs/TriceUserManual.md)
- [Quickstarts and moved README material](./docs/TriceUserManualExtension.md)
- [Generated CLI help](./docs/ref/trice-help-all.txt)
- [Examples](./examples)
- [Target source configuration defaults](./src/triceDefaultConfig.h)

## Advanced capabilities

Trice also supports additional use cases that are useful after the first integration works:

- host and target timestamps,
- binary log files for later decoding,
- display server for multiple targets,
- XTEA-encrypted transport packets,
- language switching by changing the TIL data,
- ABC: asynchronous broadcast commands based on Trice records,
- aliases for legacy logging macros,
- routing different ID ranges to different outputs.

These are intentionally not part of the first-screen quickstart. See [advanced features moved from the old README](./docs/TriceUserManualExtension.md#advanced-features-from-old-readme).

## When Trice is a good fit

Trice is usually a good fit when:

- `printf` changes timing too much,
- format strings waste too much FLASH,
- UART bandwidth is too small for full text logs,
- interrupt-context logging is needed,
- field logs must be decoded long after the firmware was built,
- a C/C++ project needs a `defmt`-like logging approach without moving to Rust.

Trice may be overkill when:

- ordinary `printf` is already fast enough,
- you cannot add a pre-build source-processing step,
- your platform already gives you a tightly integrated logging stack that meets your needs,
- you need a full visual RTOS/event timeline tool rather than compact printf-like logs.

## Project status and support

Trice is usable today and actively maintained. Use the latest release or the `main` branch when building from source. Avoid development branches unless you intentionally want unreleased work.

Contributions are welcome: examples, platform recipes, transport backends, documentation improvements, and reproducible benchmark reports are especially valuable.

If Trice helps a commercial product, consider supporting continued maintenance through the sponsor links on the GitHub project page.

## Related projects

Trice is in the same broad problem space as `printf`, SEGGER RTT/SystemView, ARM ITM/SWO, Zephyr dictionary logging, Rust `defmt`, Pigweed tokenized logging, Memfault compact logs, and other embedded tracing systems. See [related projects and comparison notes](./docs/TriceUserManualExtension.md#related-projects-and-comparison-notes).
