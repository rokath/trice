# Stamp expressions and continuous counters

Use an expression in the existing stamp switch to convert raw hardware ticks on the PC. For a 48 MHz counter counting from 47999 down to 0, this displays continuous microseconds:

```bash
# Bash / Git Bash
trice log -p COM15 -baud 921600 -ts off \
  -ts16 'printf("msg:%12.3f us", unwrap(47999-ts,48000)/48)' \
  -ts16delta 'printf("dt:%10.3f us", delta(unwrap(47999-ts,48000))/48)'
```

The [`unwrap` reference](#unwrap-reference) defines the parameters, return value, state changes, and error behavior used in this example.

**Reading a raw hardware counter directly can significantly reduce target CPU work, especially when logging in interrupt handlers.** The target captures and transmits the raw stamp; normalization of the count direction, wrap reconstruction, unit conversion, and formatting run on the host. This removes the corresponding arithmetic from stamp capture and can shorten interrupt execution time. The same raw capture can later be decoded with different formulas or display units without rebuilding the firmware.

The host interprets the stamp in four steps:

| Value | Meaning | Example |
| --- | --- | --- |
| Raw stamp | Received counter value | `ts` |
| Forward phase | Position within one counter period | `47999-ts` |
| Unwrapped ticks | Continuous count across observed wraps | [`unwrap(47999-ts,48000)`](#unwrap-reference) |
| Visible delta | Difference from the previous emission of this delta field | [`delta(unwrap(47999-ts,48000))`](#unwrap-reference) |

## Syntax and compatibility

The four switches `-ts16`, `-ts32`, `-ts16delta`, and `-ts32delta` accept `printf("format", expression, ...)`. The inner format is a double-quoted Go string. The exact prefix `printf(` at the start of the argument selects expression mode; malformed expressions fail before the input opens. An old literal beginning with that prefix must be written explicitly, for example `printf("printf(%d)",ts)`.

| Context | Names and functions |
| --- | --- |
| `-ts16`, `-ts16delta` | `ts`, `ts16`, [`unwrap(phase,modulus[,initial])`](#unwrap-reference) |
| `-ts32`, `-ts32delta` | `ts`, `ts32`, [`unwrap(phase,modulus[,initial])`](#unwrap-reference) |
| Delta columns only | Also `delta(expression)` |

`ts` always means the current raw unsigned stamp, including inside a delta expression. Wrong-width aliases, payload names such as `v0`, unknown functions, nested `delta`, and stateful functions inside an `unwrap` phase are rejected. `unwrap` and `delta` are unavailable in `-vis`.

Numeric literals, parentheses, unary minus, and `+`, `-`, `*`, `/` use the shared `-vis` arithmetic and formatting rules. Arithmetic uses `float64`, so `/48` already permits a fractional result. Supported conversions are `%d`, `%b`, `%o`, `%x`, `%X`, `%f`, `%e`, `%E`, `%g`, `%G`, and `%v`, with literal widths and precisions; `%%` prints a percent sign. Dynamic widths, argument indexes, and mismatched expression counts fail during startup.

Plain formats and presets such as `-ts16 "msg:%8d us"`, `-ts16 us`, `-ts16 ms`, `-ts16 ""`, and `-ts32 ""` retain their meanings. The `us` and `ms` presets do not discover clock frequency, count direction, or reload value. Plain delta formats still subtract raw stamps modulo `2^16` or `2^32`, as described above. `-ts` sets general presets only. `-ts0` and `-ts0delta` remain alignment facilities; absent stamps never become zero samples.

## unwrap reference

**Purpose:** Extend a forward-running counter phase across successive wraps into a continuous unsigned 64-bit count. The function runs on the PC as part of a stamp expression. Its two signatures are:

```text
unwrap(phase, modulus)          -> uint64
unwrap(phase, modulus, initial) -> uint64
```

The return type corresponds to C `uint64_t`. Here, `UINT64_MAX` is `18446744073709551615`.

Available in expression-mode `-ts16`, `-ts32`, `-ts16delta`, and `-ts32delta`. See [Syntax and compatibility](#syntax-and-compatibility) for the permitted surrounding expressions.

### Parameters

| Input parameter | Meaning and valid values | How to choose it |
| --- | --- | --- |
| `phase` | Stateless expression evaluated for each matching raw stamp. Its result must be finite, integral, and satisfy `0 <= phase < modulus`. It may use `ts`, the matching `ts16` or `ts32` alias, numeric literals, and basic arithmetic. It cannot contain `unwrap` or `delta`. | Supply a phase that advances in the counting direction. For an up-counter use `ts`. For a down-counter running from `R` to `0`, use `R-ts`. Keep the phase in counter ticks; convert units after `unwrap`. |
| `modulus` | Required exact integer constant in `1..UINT64_MAX`, fixed during CLI initialization. It uses the same unit as `phase`. | Supply the actual number of states in one counter period. A counter running through `0..47999` has modulus `48000`; full-range 16-bit and 32-bit counters use `65536` and `4294967296`. The transmitted stamp width alone does not determine the modulus. |
| `initial` | Optional exact integer constant in `0..UINT64_MAX`. It sets the first returned accumulated count, in the same unit as `phase`. | Omit it to preserve the first usable phase. Use `0` for elapsed ticks from the first usable sample, or another value for a chosen starting offset. The first returned value is exactly `initial`, without adding the first phase to it. |

`modulus` and `initial` cannot depend on a changing stamp. Constant arithmetic is evaluated exactly before the integer range check. Integral floating spellings such as `48000.0` are accepted; fractional values such as `48000.5` are rejected. An initial value of `9007199254740993` remains exact.

`initial` is an accumulated starting count and may exceed `modulus`. It does not change the phase expression or the modulus.

Dividing by `48` outside the function converts reconstructed ticks to microseconds for a 48 MHz counter. Dividing the phase by `48` inside the function can produce fractional phases, which are rejected.

### Return value and state changes

On success, return the current accumulated count `A` as `uint64`. No time-unit conversion or display rounding occurs inside `unwrap`.

For the first usable sample with phase `p`:

| Form | First return value `A` | Saved previous phase |
| --- | --- | --- |
| `unwrap(phase,modulus)` | `p` | `p` |
| `unwrap(phase,modulus,initial)` | `initial` | `p` |

Thus, `initial` changes the returned origin while the actual first phase remains the starting point for later differences. A first sample hidden by an output filter still initializes this state.

For each subsequent usable sample, let `p` be the current phase, `q` the saved previous phase, and `M` the modulus:

```text
if overflow_latched:
    return unavailable

if p >= q:
    step = p - q
else:
    step = M - q + p

if step > UINT64_MAX - A:
    overflow_latched = true
    return unavailable

A = A + step
previous_phase = p
return A
```

Each step is in `0..M-1`. Equal consecutive phases add zero; they do not imply a complete wrap. Accumulation and the overflow check use integer arithmetic. See [Origin, modulus, and integer precision](#origin-modulus-and-integer-precision) for the two origins side by side and precision limits when using the result in later arithmetic.

### Preconditions and observation lifecycle

The caller must provide samples in capture order from the same counter, with a stable modulus and no unannounced reset. Between consecutive usable observations, the counter must advance by **fewer than `modulus` ticks**. Unobserved complete periods cannot be reconstructed. See the [sampling and continuity contract](#sampling-and-continuity-contract) for the exact condition and capture-time examples.

Each compiled occurrence has its own state for its source and stamp width. Every usable matching decoded stamp advances it once, before output filtering, including empty messages and intermediate calls in a composed line. Missing stamps and diagnostics supply no sample. Rendering reads the saved result and does not advance the counter again. Starting a new input session, including reconnecting, clears the history. See [Invisible samples and visible deltas](#invisible-samples-and-visible-deltas) for a worked example.

### Errors

| Condition | When detected | Result and effect on state |
| --- | --- | --- |
| Invalid argument count, a stateful phase, or an invalid/nonconstant `modulus` or `initial` | CLI initialization | Reject the configuration before opening input. |
| Phase evaluation fails, or the phase is nonfinite, fractional, negative, outside the modulus, or not representable by `uint64` | Observation | Report a diagnostic and make the affected field unavailable. Keep the previous valid phase and accumulated count. A later valid sample may continue if the sampling contract still holds across the usable samples. |
| Adding `step` would exceed `UINT64_MAX` | Observation | Report accumulator overflow and keep this instance unavailable until a new tracking session. The accumulator never wraps silently. |

An unavailable field displays `?` markers instead of a numeric result; the application message remains available. A failure in arithmetic or formatting **after** a valid `unwrap` observation does not undo that observation. See [Errors and unavailable fields](#errors-and-unavailable-fields) for field-level behavior.

## A 48 MHz down-counter

The sequence `47999, 47998, ..., 1, 0, 47999, ...` has **48000 states**. Its nominal period is 1000 microseconds. Choose `47999-ts` as the forward phase; using `48000-ts` would shift the origin by one tick.

| Raw `ts` | Phase ticks | Phase microseconds | `%d` | `%.3f` |
| ---: | ---: | ---: | ---: | ---: |
| 47999 | 0 | 0 | 0 | 0.000 |
| 47998 | 1 | 0.020833333... | 0 | 0.021 |
| 47951 | 48 | 1 | 1 | 1.000 |
| 23999 | 24000 | 500 | 500 | 500.000 |
| 0 | 47999 | 999.979166666... | 999 | 999.979 |
| 47999 after reload | 0 | 0 | 0 | 0.000 |

```bash
# Bash / Git Bash: within-period phase, without wrap reconstruction
-ts16 'printf("msg:%8d us", (47999-ts)/48)'
-ts16 'printf("msg:%8.3f us", (47999-ts)/48)'
-ts16 'printf("msg:%8.0f us", (47999-ts)/48)'

# Inspect raw, phase, and continuous time together
-ts16 'printf("time:raw=%5d phase=%8.3f us total=%12.3f us", ts, (47999-ts)/48, unwrap(47999-ts,48000)/48)'
```

In the continuous-time argument, [`unwrap`](#unwrap-reference) receives the forward phase `47999-ts` and modulus `48000`; the following `/48` converts its returned ticks to microseconds.

`%d` truncates a computed result toward zero after checking its range; `%.0f` rounds. Immediately before reload these examples display `999`, `999.979`, and `1000`. Decimal places do not improve the hardware resolution of one tick, here `1/48 us`.

With [`unwrap(47999-ts,48000)`](#unwrap-reference), the same counter continues across successive wraps:

| Raw `ts` | Forward phase | Accumulated ticks | Microseconds with `%.3f` |
| ---: | ---: | ---: | ---: |
| 47999 | 0 | 0 | 0.000 |
| 23999 | 24000 | 24000 | 500.000 |
| 0 | 47999 | 47999 | 999.979 |
| 47999 | 0 | 48000 | 1000.000 |
| 23999 | 24000 | 72000 | 1500.000 |
| 47999 | 0 | 96000 | 2000.000 |

## Origin, modulus, and integer precision

[`unwrap(phase,modulus)`](#unwrap-reference) starts at the first usable phase. [`unwrap(phase,modulus,initial)`](#unwrap-reference) returns the selected initial value for that first sample, then adds subsequent phase advances. For raw values `23999, 0, 47999`:

| Expression | Tick results | Microsecond results after `/48` |
| --- | --- | --- |
| `unwrap(47999-ts,48000)` | 24000, 47999, 48000 | 500, 999.979166..., 1000 |
| `unwrap(47999-ts,48000,0)` | 0, 23999, 24000 | 0, 499.979166..., 500 |

An initial zero measures elapsed time from the first usable received sample, even if that sample is hidden. Neither form reveals periods before that sample or target uptime.

The [`unwrap` reference](#unwrap-reference) defines the parameter ranges, initialization, and checked integer update rule.

For visible intervals, subtract the ticks returned by [`unwrap`](#unwrap-reference) **before** converting units:

```bash
# Bash / Git Bash
-ts16delta 'printf("dt:%12.6f us", delta(unwrap(47999-ts,48000))/48)'
```

This preserves a one-tick difference above `2^53` accumulated ticks. `delta(unwrap(...)/48)` first converts absolute counts to floating point and can lose that difference. General arithmetic, including absolute `unwrap(...)/48`, retains normal `float64` precision limits. Direct integer formatting preserves all 64 bits. Other integral delta operands are also subtracted exactly; negative differences are retained when representable. `delta(ts)` alone does not correct raw counter wraps.

## Other counter configurations

Each example follows the [`unwrap` parameter rules](#unwrap-reference): normalize the phase to advance forward and use the counter's actual number of states as the modulus.

| Counter | Bash / Git Bash argument | Example raw values | Unwrapped ticks |
| --- | --- | --- | --- |
| Full-range 16-bit up | `-ts16 'printf("time:%12d ticks",unwrap(ts,65536))'` | 65534, 65535, 0, 1 | 65534, 65535, 65536, 65537 |
| Full-range 16-bit down | `-ts16 'printf("time:%12d ticks",unwrap(65535-ts,65536))'` | 1, 0, 65535, 65534 | 65534, 65535, 65536, 65537 |
| Full-range 32-bit up | `-ts32 'printf("time:%15d ticks",unwrap(ts,4294967296))'` | 4294967295, 0, 1 | 4294967295, 4294967296, 4294967297 |
| 0..9999 up | `-ts16 'printf("msg:%12d counts",unwrap(ts,10000))'` | 9998, 9999, 0, 1 | 9998, 9999, 10000, 10001 |

For a 48 MHz full-width 32-bit up-counter, [`unwrap`](#unwrap-reference) returns continuous ticks. Choose the display unit with the divisor:

```bash
# Bash / Git Bash
-ts32 'printf("time:%15.3f us", unwrap(ts,4294967296)/48)'
-ts32 'printf("time:%15.6f ms", unwrap(ts,4294967296)/48000)'
-ts32 'printf("time:%15.9f s", unwrap(ts,4294967296)/48000000)'
```

## Sampling and continuity contract

For each pair of consecutive **usable received samples** of the same counter, the user must guarantee `0 <= C[n]-C[n-1] < modulus`, where `C` is the conceptual unbounded tick count. The PC cannot infer unobserved complete periods. Equal phases can mean zero advances or an unknown number of whole periods; the stated contract permits only the first interpretation.

For the down-counter above, fewer than 48000 advances may separate usable samples. Capture comfortably more often than once per millisecond, for example with a suitable heartbeat every 0.5 ms. One sample in each fixed millisecond window is insufficient: captures at 0.1 ms and 1.9 ms are 1.8 ms apart. Use an operational margin rather than relying on a time gap arbitrarily close to one period.

The contract applies after losses, malformed records, and unavailable phases. Target readings that never reach the tracker cannot help. Preserve capture order and counter configuration without unannounced resets. The PC cannot reliably detect missed periods, counter resets, changed reload values, or reordered samples from raw phases alone.

Buffered UART/USB transport and file replay are allowed: target capture times matter, not PC arrival times. Host timestamps are not used to guess wraps. Each logger input session represents one source; unidentified multiplexed targets must not be combined into one history. The 16-bit and 32-bit counters and each compiled expression have independent histories. Starting a new session, including reconnecting the transport, resets them.

## Invisible samples and visible deltas

All usable decoded stamps update [`unwrap`](#unwrap-reference) before `-pick`, `-ban`, `-logLevel`, visualization `log=drop`, and line-start selection. Empty messages and intermediate calls in a composed line also update it. Missing stamps and diagnostics do not. A present zero stamp is valid.

```bash
# Bash / Git Bash: hidden non-error heartbeat messages still maintain the clock
trice log -p COM15 -baud 921600 -pick err -ts off \
  -ts16 'printf("time:%12.3f us", unwrap(47999-ts,48000)/48)' \
  -ts16delta 'printf("dt:%12.3f us", delta(unwrap(47999-ts,48000))/48)'
```

| Capture time, us | Raw stamp | Accumulated ticks | Visible? |
| ---: | ---: | ---: | --- |
| 0 | 47999 | 0 | Yes |
| 500 | 23999 | 24000 | No |
| 1000 | 47999 | 48000 | No |
| 1500 | 23999 | 72000 | No |
| 2000 | 47999 | 96000 | No |
| 2500 | 23999 | 120000 | No |
| 3000 | 47999 | 144000 | Yes |

The second visible message displays `3000.000 us` for both the continuous stamp and its visible delta. Comparing only the visible raw values would give zero. Suppressing heartbeat records on the target cannot provide this result.

The first eligible output event with valid delta operands establishes the baseline. Text displays aligned `-` markers; JSON/KV omit the undefined first delta member. Hidden events do not change this baseline. A failed render preserves all previous valid delta baselines; multiple deltas in one field commit together. Valid nested `unwrap` observers continue even if later arithmetic fails.

Text keeps the stamp of the existing first-call line selection. Later calls cannot overwrite that saved value. JSON/KV emit one record per event, so their deltas follow record-field emissions. Repeated rendering and alignment queries use cached values without adding samples or advancing baselines. Delta-only operation uses the same [`unwrap` state and parameter rules](#unwrap-reference):

```bash
# Bash / Git Bash
-ts16 "" -ts16delta 'printf("dt:%10.3f us", delta(unwrap(47999-ts,48000))/48)'
```

## Errors and unavailable fields

Startup rejects malformed expressions, unknown names, wrong-width aliases, unsupported functions, invalid argument counts, and invalid constant arguments. Runtime failures such as an out-of-range or fractional phase, division by zero, nonfinite arithmetic, or unsafe numeric conversion produce a diagnostic and aligned `?` markers for the affected field. The application message remains available. A raw stamp is never substituted under a converted unit label.

Invalid phases do not advance or reset the tracker. Later usable phases continue from the last valid one, subject to the same sampling contract across that gap. Accumulator overflow stays unavailable until a new session; it never silently wraps. This limited expression language executes neither shell commands nor arbitrary Go code.

## Shell quoting

Keep the double quotes around the inner Go format string. The examples above use Bash/Git Bash quoting. Windows shells use different native-argument rules; choose the following form that matches your shell and argument mode.

All shell forms below pass the same [`unwrap` expression](#unwrap-reference); its parameter meanings and return value are independent of shell quoting.

Microsoft documents the native argument modes and stop-parsing token in [PowerShell argument parsing](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_parsing).

PowerShell 7.3 or later with standard native argument passing:

```powershell
$PSNativeCommandArgumentPassing = 'Standard'
trice log -ts off -ts16 'printf("msg:%12.3f us",unwrap(47999-ts,48000)/48)'
```

Windows PowerShell 5.1 / legacy native argument passing, using the Windows stop-parsing token:

```powershell
trice --% log -ts off -ts16 "printf(\"msg:%12.3f us\",unwrap(47999-ts,48000)/48)"
```

Interactive `cmd.exe`:

```bat
trice log -ts off -ts16 "printf(\"msg:%12.3f us\",unwrap(47999-ts,48000)/48)"
```

In a `.bat` or `.cmd` file, double each format percent sign:

```bat
trice log -ts off -ts16 "printf(\"msg:%%12.3f us\",unwrap(47999-ts,48000)/48)"
```

