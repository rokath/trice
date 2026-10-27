# Target Configuration

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Configuration and include order](#configuration-and-include-order)
* [2. Critical sections and checks](#critical-sections-and-checks)
* [3. Target side compile-time Trice On-Off](#target-side-compile-time--trice-on-off)
* [4. Buffer protection and diagnostics](#buffer-protection-and-diagnostics)
* [5. Trice Diagnostics](#trice-diagnostics)
* [6. Avoid buffer overruns](#avoid-buffer-overruns)
* [7. Host filters and target routing](#host-filters-and-target-routing)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

Keep application settings in your own `triceConfig.h`. The documented defaults are in [triceDefaultConfig.h](../../src/triceDefaultConfig.h); do not edit library defaults to configure a project. For first integration follow the [UM](../TriceUserManual.md#bring-trice-into-your-firmware), then the transport-specific [recipes](quickstarts.md).

## 1. <a id="configuration-and-include-order"></a>Configuration and include order

Put application configuration and generated header directories first, then `src`, then `src/default_conf`. This makes project headers take precedence over fallback configuration. [Bind build integration](bind.md#build-integration) gives the complete rule.

Start from a working [example](examples.md), not an internal CGO test configuration. In particular, [F030 RTT](../../examples/F030_inst_rtt/Core/Inc/triceConfig.h) and [F030 UART](../../examples/F030_inst_uart/Core/Inc/triceConfig.h) show small configurations.

## 2. <a id="critical-sections-and-checks"></a>Critical sections and checks

Before logging from interrupts or multiple tasks, define `TRICE_ENTER_CRITICAL_SECTION` and `TRICE_LEAVE_CRITICAL_SECTION` for your MCU and scheduler. Empty defaults are suitable only when no competing context can access the shared state. The protected region must restore the previous interrupt state rather than blindly enabling interrupts.

For targeted experiments, adapt individual cases from [triceCheck.c](../../_test/testdata/triceCheck.c); do not include the complete test harness in firmware. For example:

```c
TRice("1/11 = %g\n", aFloat(1.0f / 11));
```

![Floating-point example output](../ref/1div11.PNG)

Run Bind before building, as explained in [build integration](bind.md#build-integration). The [macro reference](printf_comparison.md) describes argument conversion and widths.

## 3. <a id="target-side-compile-time--trice-on-off"></a>Target side compile-time  Trice On-Off

* If your code works well after checking, you can add `#define TRICE_OFF 1` just before the `#include "trice.h"` line and no Trice code is generated anymore for that file, so no need to delete or comment out Trice macros: <!-- ![./ref/TRICE_OFF.PNG](./ref/TRICE_OFF.PNG) -->

```C
#define TRICE_OFF 1
#include "trice.h"
void fn(void) {
    trice( "Hi");          // Will generate code only, when TRICE_OFF == 0.
    trice( "Lo");          // Will generate code only, when TRICE_OFF == 0.
}
```

Use `-DTRICE_OFF=1` to disable calls throughout a build. The ID tool can still collect these source calls into the dictionary. There is no built-in target runtime on/off protocol; add application conditions when needed. Host filters only hide received messages and do not save target execution or transfer work.

## 4. <a id="buffer-protection-and-diagnostics"></a>Buffer protection and diagnostics

Keep `TRICE_PROTECT=1` unless you have established that your application cannot exhaust the configured buffers. Protection can drop a record when there is insufficient space; it does not make an undersized transport lossless. Monitor `TriceDirectOverflowCount` and `TriceDeferredOverflowCount`. A rejected record need not create a cycle error, so receiver cycle checks alone are insufficient.

## 5. <a id="trice-diagnostics"></a>Trice Diagnostics

A trice statement produces 4 bytes buffer data plus optional values data. When for example `TRice16("Voltage=%u\n", x);` is called inside the ms system-tick interrupt every 5th time, 10 bytes data are generated each 5 millisecond. This needs a transfer baudrate of at least 20.000 bit/s. A UART running at 115.200 baud can easily handle that.
Anyway after 100 ms, a 200 Bytes buffer is filled and the question arises what is the optimal Trice buffer size. A calculation is error prone, so measuring is better. So configure the buffer sizes bigger than estimated and watch the max depth of their usage. In [./examples/exampleData/triceLogDiagData.c](../../examples/exampleData/triceLogDiagData.c) an option is shown. After you optimized your buffer sizes, you can deactivate the Trice diagnostics in your project specific _triceConfig.h_ with `#define TRICE_DIAGNOSTICS 0`.

## 6. <a id="avoid-buffer-overruns"></a>Avoid buffer overruns

Measure the largest bursts and the time between transfers; average bandwidth alone is insufficient. Start with generous buffers and monitor their maximum usage with [triceLogDiagData.c](../../examples/exampleData/triceLogDiagData.c).

If production exceeds transport capacity, increase capacity, reduce logging or explicitly accept loss. Disabling `TRICE_PROTECT` can allow corrupt records and writes beyond buffer boundaries; keep it enabled unless the required capacity is proven. Cycle checks help detect transmission loss, but are not a substitute for the overflow counters above.

## 7. <a id="host-filters-and-target-routing"></a>Host filters and target routing

Host [log levels and tag selection](tags_color_and_weights.md) do not disable firmware calls. [Target routing](target_routing.md) selects configured output channels by ID range. Compile-time disabling above removes calls; these are separate mechanisms.

