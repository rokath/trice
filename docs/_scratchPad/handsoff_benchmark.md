# Handoff: Reproducible Trice Instrumentation Benchmark

## Goal

Build a permanent, reproducible benchmark project that lets users compare Trice with several `printf`-like implementations, run the tests themselves, change the configuration, and reproduce the results on different targets.

The benchmark must avoid cherry-picking a slow `printf` implementation. It should compare equivalent information and make clear which work is performed on the target and which is deferred to the host.

## Core principles

- Do not make the historic 6-clock result the headline metric.
- Measure current Trice versions and current configurations.
- Treat roughly 16 clocks as a possible optimized-case expectation only until measured.
- Expect realistic Trice settings such as cycle counter, protection and critical sections to cost substantially more; a value around 100 clocks is plausible but must be measured, not assumed.
- Report best case, realistic case and on-target formatting separately.
- Compare several `printf` implementations.
- Separate record creation, formatting and transport costs.
- Prefer reproducible source, scripts and machine-readable result files over hand-maintained tables.
- Distinguish simulated cycles from measurements on real silicon.

## Suggested project layout

```text
examples/TriceVsPrintf/
    common/
        bench.c
        bench.h
        cases.c
        cases.h
        result.c
        result.h

    native/
        Makefile
        triceConfig.h

    stm32g0b1/
        ...
        triceConfig.h

    atmega328p/
        ...
        triceConfig.h

    esp32/
        ...                 # later

    printf/
        nanoprintf/
        mpaland_printf/
        ...

    scripts/
        run_all.sh
        collect_results.py

    results/
        results.csv
        results.md
```

The common benchmark source should be shared across targets as far as practical.

## Benchmark candidates

At minimum:

- Trice optimized/deferred
- Trice typical/deferred
- Trice on-target decoding/formatting
- newlib `printf` / `snprintf`
- newlib-nano
- picolibc
- nanoprintf
- optionally mpaland/printf or another small embedded `printf`

Vendor libraries such as IAR or Keil can be added later if they can be tested reproducibly.

## Trice profiles

### `trice-fast`

Purpose: current optimized record-creation path.

Typical characteristics:

```text
TRICE_CYCLE_COUNTER = 0
TRICE_PROTECT = 0
critical section disabled
no diagnostics
deferred output
fast buffer configuration, probably double buffer
```

This is a controlled best-case configuration. It must be labelled as such.

### `trice-typical`

Purpose: realistic production-style instrumentation.

Typical characteristics:

```text
TRICE_CYCLE_COUNTER = 1
TRICE_PROTECT = 1
critical section enabled
realistic timestamp configuration
normal diagnostics/buffer choices as appropriate
```

This profile is more important than the absolute fastest profile.

### `trice-on-target`

Purpose: compare Trice when text formatting/decoding is performed on the target.

This is the closest architectural comparison with conventional `printf`.

## Common test cases

Use semantically equivalent outputs.

Suggested cases:

```c
EVENT("constant");
EVENT("value=%d", x);
EVENT("hex=%08x", x);
EVENT("x=%d y=%d", x, y);
EVENT("x=%d y=%d z=%d", x, y, z);
EVENT("text=%s", s);
EVENT("f=%.3f", f);      // only where float support is intentionally enabled
```

The constant/no-payload case is useful for minimum instrumentation cost, but it must not dominate the presentation of results.

## Measurements

Collect at least:

- CPU cycles per event
- minimum / median / maximum or a documented deterministic value
- Flash delta relative to an empty/minimal program
- static RAM delta
- stack usage where practical
- bytes produced per event before transport
- bytes on the transport
- maximum sustainable event rate
- behavior inside an ISR / critical path
- compiler and optimization flags
- exact Trice configuration
- exact library versions / commits

Where useful, also measure end-to-end latency, but keep it separate from target-side instrumentation cost.

## Measurement layers

### A. Target-side work only

Compare:

```text
snprintf -> RAM buffer
Trice    -> Trice record buffer
```

No UART, RTT, USB or file I/O in the timed region.

### B. Encoded bytes / bandwidth

Record how many bytes are generated for equivalent information.

Example:

```text
printf text:  "value=12345\n"
Trice:        ID + payload + optional metadata
```

### C. End-to-end path

Measure from instrumentation call until readable host output only as a separate experiment.

Transport latency must not be mixed with the CPU-cost benchmark.

## Timing methodology

### Real STM32 hardware

For STM32G0B1, reuse the existing Trice benchmark infrastructure where possible:

- existing `G0B1_inst`
- existing `triceFast.c`
- TIM2 configured as CPU-clock-synchronous counter
- GCC and Clang builds
- compiler barriers around timing reads
- `DSB` where needed to ensure stores are complete
- repeated straight-line calls to reduce timing-reader overhead

The current G0B1 benchmark is already a good basis and should be generalized rather than replaced.

### LabWired

LabWired is useful for:

- deterministic firmware execution
- automated CI
- users without boards
- UART and other peripheral behavior
- cross-target reproducibility

But simulator cycle counts must not automatically be presented as real-silicon cycle counts.

For Cortex-M, LabWired itself documents that timing fidelity depends on the model and is not universally silicon-cycle-accurate. Therefore label results clearly:

```text
Simulated cycles
Measured hardware cycles
```

where applicable.

### ATmega328P / AVR8

This is an especially interesting second target.

LabWired currently supports an ATmega328P/Arduino Uno path and models AVR instruction timing using the AVR instruction-cycle counts rather than a generic one-instruction/one-cycle assumption.

Advantages:

- 8-bit MCU
- 16 MHz
- 32 KB Flash
- 2 KB SRAM
- avr-gcc
- UART and interrupt support
- easy real-hardware validation with Arduino Uno/Nano

A working Trice benchmark here would demonstrate that Trice is not restricted to 32-bit Cortex-M systems.

## Target priority

Recommended implementation order:

1. Native PC
2. STM32G0B1
3. ATmega328P / Arduino Uno
4. ESP32 later
5. optionally STM32H563 as a strongly LabWired-validated ARM target

### Native PC

Purpose: zero-friction entry point.

Example workflow:

```sh
git clone ...
cd examples/TriceVsPrintf/native
make
./benchmark
```

Native results are useful for experimentation but are not substitutes for MCU timing measurements.

### STM32G0B1

Best first MCU target because Trice already has:

- `G0B1_inst`
- configurable clock benchmarks
- GCC and Clang support
- TIM2-based cycle measurement
- existing real hardware

LabWired also has an STM32G0B1RE model, but it is currently a limited model slice rather than a fully silicon-validated timing reference.

### ATmega328P

Best demonstration of 8-bit portability and resource constraints.

Prefer both:

- LabWired execution for reproducibility
- optional real Uno/Nano measurement for validation

### ESP32

Add later.

LabWired supports classic ESP32/Xtensa paths, but the classic ESP32 model is currently described as structural without silicon diff. It is therefore useful for functional demonstration, but not yet the preferred source of published cycle-accurate comparison numbers.

## Result format

Generate machine-readable and human-readable outputs.

Example:

```text
implementation,target,compiler,case,cycles,flash_delta,ram_delta,stack,bytes
trice-fast,g0b1,gcc,const,17,....
trice-typical,g0b1,gcc,const,....
nanoprintf,g0b1,gcc,int,....
```

Generate `results.md` automatically from `results.csv`.

A result table might look like:

| Implementation | Target | Case | Cycles | Flash Δ | Stack | Bytes |
|---|---|---|---:|---:|---:|---:|
| Trice fast | G0B1 | integer | measured | measured | measured | measured |
| Trice typical | G0B1 | integer | measured | measured | measured | measured |
| Trice on-target | G0B1 | integer | measured | measured | measured | measured |
| nanoprintf | G0B1 | integer | measured | measured | measured | measured |
| newlib-nano | G0B1 | integer | measured | measured | measured | measured |

Do not commit guessed numbers.

## User-facing workflow

The project should encourage experimentation.

Possible interface:

```sh
make TARGET=native IMPL=trice-typical
make TARGET=g0b1 IMPL=trice-fast
make TARGET=avr IMPL=nanoprintf
```

and optionally:

```sh
./scripts/run_all.sh
```

Users should be able to:

- change the benchmark source
- switch Trice features on/off
- switch compiler
- switch `printf` implementation
- add new test cases
- rerun and regenerate the tables

## CI

CI should at least verify:

- native builds on Linux/macOS/Windows where practical
- G0B1 firmware builds
- AVR firmware builds
- LabWired tests for supported targets
- result parser/schema validity
- no stale generated result table

Real-hardware timing results should not be silently regenerated in normal CI unless a dedicated HIL runner exists.

## Fairness requirements

A benchmark change should be rejected if it:

- compares one deliberately weak `printf` implementation against Trice
- times UART or filesystem output inside one implementation but not another
- reports simulated cycles as silicon measurements without qualification
- reports only Trice's fastest configuration
- hides protection/timestamp/critical-section costs
- compares different semantic payloads
- omits compiler/version/configuration metadata

## Key statement

The project should be built around this principle:

> Compare equivalent information, not merely equivalent function-call syntax.

Trice and `printf` have different architectures. The benchmark should make those differences measurable instead of obscuring them.
