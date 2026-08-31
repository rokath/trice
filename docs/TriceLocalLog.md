<!--
Insertion recommendation: replace the current "Local Deferred Text Log"
subsection below "Deferred Out" in TriceUserManual.md with this chapter. Its
heading levels are already chosen for that location. Regenerate the manual TOC
only after insertion.
-->

#### Local Deferred Text Log

Local deferred logging keeps every time-critical Trice producer binary and
short, but turns complete records into plain text later on the target. A common
use is a low-priority FreeRTOS logging task which writes to UART, USB, a file,
or an existing application console without a host-side `trice log` process.

```text
tasks and interrupts
        |
        | trice(...)
        v
ring or double buffer containing ordinary binary Trice records
        |
        | one background consumer
        v
TriceLog(applicationBuffer, applicationBufferSize)
        |
        v
application UART, USB, file, or console writer
```

`TriceLog()` and `TriceTransfer()` are alternative consumers of the same
deferred buffer. Never call both for one buffer. The producer-side wire format,
the normal host decoder, `til.json`, and existing `trice insert`/`trice bind`
workflows are unchanged.

##### Basic configuration

Applications set local-log options in their ordinary `triceConfig.h`. The
library defaults and the detailed description of every switch live in
`src/triceLogDefaultConfig.h`; applications do not copy or edit that file.

```c
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DIRECT_OUTPUT 0
#define TRICE_LOCAL_LOG 1
```

`TRICE_DOUBLE_BUFFER` is supported as an alternative. Producer tasks and
interrupts still only append compact records. Formatting begins when the
background consumer calls `TriceLog()`.

Local formatting needs an ID-to-format table in the target build. First make
source IDs authoritative with either `trice bind` or legacy `trice insert`,
then generate a table containing only the currently selected sources:

```sh
trice bind -src <source> -til til.json
# Alternatively: trice insert -src <source> -til til.json

trice generate -src <source> -til til.json -logC=build/til.c
```

Repeat `-src` for additional files or directories. Bind sidecars are read from
`build/triceIDs` by default; specify `-bindDir` only for a different sidecar
directory. `-logC` and `-abc` are separate generator modes and cannot be used
together.

The generated C file includes compile-time guards derived from its Trice type,
format string, and payload width. It therefore uses the same local-log switches
as the formatter. A disabled capability removes the affected format strings
from the compiled target image; regenerating `til.c` for every option change is
not required. Regeneration is required after source IDs, Trice types, or format
strings change.

##### Configuration switches

The switches are deliberately independent of any printf implementation. Their
names are similar to familiar nanoprintf choices where useful, but Trice neither
includes nanoprintf nor defines `NANOPRINTF_*` macros.

| Switch | Default | Effect when set to `1` |
| --- | ---: | --- |
| `TRICE_LOCAL_LOG` | `0` | Includes the local consumer and formatter. |
| `TRICE_LOCAL_LOG_USE_PRINTF_HOOK` | `1` | Declares and uses the runtime `UserTriceLogPrintfFn` hook. |
| `TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER` | `1` | Includes the separate exact `%d`/`%x` fallback. |
| `TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS` | `1` | Accepts fixed widths such as `%8d`, `%02x`, and `%-12s`. |
| `TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS` | `1` | Accepts fixed precision such as `%.3f` and `%.5s`. |
| `TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS` | `1` | Accepts `%e`, `%E`, `%f`, `%F`, `%g`, and `%G` through the hook. |
| `TRICE_LOCAL_LOG_USE_64_BIT_VALUES` | `1` | Includes 64-bit integer, buffer, and `aDouble()` paths. |
| `TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS` | `0` | Includes Trice's internal `%b` conversion. |
| `TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG` | `1` | Accepts `#`, including the internal `%#b` prefix. |
| `TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS` | `0` | Includes internal `%O`, `%t`, `%p`, and `%q` conversions. |
| `TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES` | `1` | Includes bounded `triceS` and string-form `triceN`. |
| `TRICE_LOCAL_LOG_USE_BUFFER_TRICES` | `0` | Includes `TRICE8_B` through `TRICE64_B`. |
| `TRICE_LOCAL_LOG_USE_PREFIX_HOOK` | `0` | Declares and calls `UserTriceLogPrefixFn` before each message body. |
| `TRICE_LOCAL_LOG_KEEP_DISABLED_IDS` | `1` | Keeps ID and shape metadata, but no string, for disabled generated entries. |

Every switch is Boolean and invalid values fail during preprocessing. Defaults
preserve the original local-log integer, string, width, precision, float, and
64-bit capabilities. The newer binary, extended, buffer, and prefix paths are
opt-in so an existing target does not acquire avoidable code or data.

`TRICE_LOCAL_LOG_KEEP_DISABLED_IDS == 1` lets `TriceLog()` distinguish a known
ID whose formatter feature is off from an absent or obsolete ID. It returns
`TRICE_LOG_ERR_FEATURE_DISABLED` for the former. Set it to `0` for the smallest
table; such an ID then returns `TRICE_LOG_ERR_ID` because its row is absent.

##### Selecting a printf implementation

The optional hook has the `snprintf` contract:

```c
typedef int (*TriceLogPrintfFn_t)(
    char *buffer,
    size_t size,
    const char *format,
    ...);
```

When hook support is enabled, install the function before consuming records:

```c
#include <stdio.h>
#include "trice.h"

UserTriceLogPrintfFn = snprintf;
```

Possible implementations include a system or C-library `snprintf`, newlib,
picolibc, nanoprintf, eyalroz/printf, and an `snprintf`-compatible adapter
around stb_sprintf. The hook must return the number of bytes which would have
been written without the final NUL, including when its destination is too
small. A negative hook result becomes `TRICE_LOG_ERR_PRINTF`.

Trice calls the hook once per ordinary scalar conversion. It does not build an
argument array. This keeps stack use independent of the number of values and
lets the user formatter handle its normal `%d`, `%u`, `%x`, or float path. The
following features are deliberately handled inside Trice and never delegated:

- bounded dynamic `%s`, because the binary payload has a length but no required
  trailing NUL;
- `%b`, because it is not portable across printf implementations;
- `%O`, `%t`, `%p`, and `%q`, because Trice host semantics differ from standard
  C printf implementations;
- repeated Buffer-Trice iteration and prefix placement.

The small internal formatter lives in `triceLogMinimal.c`, separately from the
record decoder. With `TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER == 1`, a null printf
hook supports only exact `%d` and lowercase `%x`. Set the option to `0` when an
external hook is always installed. The fallback then contributes no code even
without LTO. Literal text, `%%`, and enabled dynamic strings need neither
formatter implementation.

For nanoprintf, configure both layers consistently. For example, float output
needs both `TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS == 1` and
`NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS == 1`. Trice's field-width, precision,
64-bit, and alternative-form switches describe what records Trice retains and
accepts; the corresponding nanoprintf switches describe what the selected hook
can actually print. Trice's internal `%b` does not require nanoprintf binary
support.

##### Formatter families

With a printf hook, ordinary fixed scalar Trices support `%d`, `%i`, `%u`,
`%o`, `%x`, `%X`, and `%c`. Constant flags, widths, and precisions are passed to
the hook when their Trice switches are enabled. Source integer length modifiers
are normalized to the `long` or `long long` argument supplied by Trice. Dynamic
`*` width or precision remains unsupported because it would consume an
additional argument which is not represented as a separate Trice value.

`TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS` adds `%b`. The formatter supports
fixed width, precision, zero padding, left alignment, and the `0b` prefix for
`%#b` when the related switches are enabled.

`TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS` adds the established host
meanings:

- `%O` prints octal with a `0o` prefix;
- `%t` prints `true` or `false`;
- `%p` prints the transported fixed-width value as lowercase hexadecimal, not
  a C pointer; `#` adds `0x` locally;
- scalar `%q` prints a quoted character and dynamic `%q` prints an escaped,
  quoted string.

`TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES` accepts exactly one `%s`, or one
`%q` when extended formats are enabled, in a dynamic string record. The encoded
payload length is always the read boundary. An embedded NUL in an explicitly
sized `triceN` retains normal string semantics and ends visible text early.
Fixed width, precision, and left alignment are supported when enabled. Wide
strings and multiple dynamic conversions are rejected.

Buffer Trices use one scalar item conversion:

```c
uint16_t samples[] = {1u, 0x2au, 0x1234u};
TRICE16_B("buffer:%04x \n", samples, 3u);
```

Text through the first colon is written once. The remainder, without its final
newline, is repeated for each aligned payload element; one newline is appended
after all elements. The configured item conversion and its payload width must
also be enabled. The record stays at its ring- or double-buffer location while
the elements are interpreted.

For floating point, retain the established source convention:

```c
trice32("float:value=%.3f\n", aFloat(valueF));
trice64("float:value=%.9f\n", aDouble(valueD));
```

Trice reconstructs `aFloat()` from 32 payload bits and `aDouble()` from 64 bits,
then passes a promoted `double` to the hook. `%f`, `%F`, `%e`, `%E`, `%g`, and
`%G` are accepted. Float formats need hook support; `aDouble()` additionally
needs 64-bit support. The selected hook controls decimal rendering quality and
its Flash cost.

Dynamic function (`F`) and ABC records are not local text records and remain
unsupported. Other deliberate exclusions are `%n`, dynamic `*`, wide `%lc` and
`%ls`, byte-slice `%x`/`% x` formatting of dynamic string records, host
color/tag routing, and host-side location presentation. These restrictions
apply only to `TriceLog()` and its generated target table; they do not remove
the corresponding records or decoder behavior from normal binary logging.

##### Optional prefix hook

An application can prepend presentation data without adding it to every format
string:

```c
#define TRICE_LOCAL_LOG_USE_PREFIX_HOOK 1

static int LocalPrefix(
    char *buffer,
    size_t size,
    uint16_t id,
    uint8_t stampBits,
    uint32_t stamp) {
    return snprintf(buffer, size, "[%u:%lu] ",
                    (unsigned)id, (unsigned long)stamp);
}

UserTriceLogPrefixFn = LocalPrefix;
```

The hook receives the parsed ID and raw stamp facts. It deliberately does not
assume that every stamp is time. It follows the same `snprintf` size contract;
a negative result becomes `TRICE_LOG_ERR_PREFIX`. A null hook produces no
prefix.

##### Calling `TriceLog`

One background task usually drains all complete records:

```c
char text[160];
int length;

while ((length = TriceLog(text, sizeof(text))) > 0) {
    ApplicationTextWrite(text, (size_t)length);
}
```

The API contract is:

- A positive result is the number of visible bytes, excluding the final NUL.
- Zero means no complete printable record is available.
- A negative result is a `TRICE_LOG_ERR_*` value from `triceLog.h`.
- `maxlen` is the complete destination capacity including the final NUL.
- Every valid non-null destination with `maxlen > 0` is NUL-terminated on every
  return path.
- With `maxlen == 1`, only an empty C string fits. A waiting non-empty record is
  consumed and `TRICE_LOG_ERR_OUTPUT_TOO_SMALL` is returned.

Invalid API arguments do not consume a record. Success and record-local errors
consume exactly one complete record so the next call can progress. Structural
corruption or conflicting metadata clears the queue because the next record
boundary is not trustworthy. Unknown IDs, disabled features, insufficient
output space, and formatter-hook errors consume only their current record.

There is no dynamic allocation. Payload data is interpreted at its current
deferred-buffer location and released only after formatting completes. The
application owns the separate text buffer and may immediately pass its first
`length` bytes to an existing writer.

##### Size-oriented configurations

A literal/string-oriented target without any printf code can use:

```c
#define TRICE_LOCAL_LOG 1
#define TRICE_LOCAL_LOG_USE_PRINTF_HOOK 0
#define TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER 0
#define TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_64_BIT_VALUES 0
```

An integer target can leave the exact `%d`/`%x` minimal formatter enabled and
disable the hook. A feature-rich console can enable hook, width, precision,
float, 64-bit, binary, extended, string, and buffer switches explicitly. The
two approaches can use the same generated `til.c`; its preprocessor guards
select only rows valid for the active `triceConfig.h`.

##### Examples

[`examples/PC_log`](../examples/PC_log/README.md) is an immediately runnable
host program using the system `snprintf` and standard output. After a short
introductory sequence, it scans the complete shared
`_test/testdata/triceCheck.c` producer corpus and fails with the exact selector
and local-log error if an emitted record cannot be formatted.

[`examples/G0B1_log`](../examples/G0B1_log/README.md) is an independent STM32G0B1
FreeRTOS project using nanoprintf in its background task and plain-text USART2
output. Its existing default and diagnostics tasks retain their CubeMX names,
priorities, and stack sizes; the default task scans the same shared producer
corpus while the diagnostics task drains it in the background. Both examples
explicitly configure and exercise dynamic strings, float, double,
Trice-specific conversions, and a Buffer Trice. Their target configurations
disable command/RPC and selector-0 transport families, and `triceCheck.c`
guards its two host-only dynamic-byte-string conversions with
`TRICE_LOCAL_LOG`; ordinary host-decoder and legacy test configurations remain
unchanged.
