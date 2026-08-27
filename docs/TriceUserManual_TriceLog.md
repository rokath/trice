<!--
Integration note for the Trice user manual maintainer:

Replace the current subsection "35.6.3. Local Deferred Text Log" in
docs/TriceUserManual.md with this chapter. It belongs below "35.6. Deferred
Out", after the Ring Buffer subsection and before "Direct Transfer". The
headings intentionally contain no manual numbering or generated anchors;
regenerate both with the repository's normal mdtoc workflow after insertion.
-->

#### Local Deferred Text Log

Local deferred logging keeps the time-critical Trice producer path binary but
formats the buffered records into plain text on the target. It is useful when
an RTOS already has a low-priority logging task or when an application needs a
normal UART, USB, or console text stream without running `trice log` on another
computer.

```text
tasks and interrupts
        |
        | trice(...)
        v
ring or double buffer containing binary records
        |
        | one background consumer
        v
TriceLog(applicationBuffer, applicationBufferSize)
        |
        v
application UART, USB, file, or console writer
```

`TriceLog()` and `TriceTransfer()` are alternative consumers of the same
deferred buffer. Do not call both for one buffer configuration.

##### Configuration

Enable one of the deferred buffers and local logging in `triceConfig.h`:

```c
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DIRECT_OUTPUT 0
#define TRICE_LOCAL_LOG 1
```

`TRICE_DOUBLE_BUFFER` can be selected instead of `TRICE_RING_BUFFER`. The
producer does not perform target-side printf work in either case. It only
writes the usual compact binary record.

Local formatting needs a generated ID-to-format table. First make the source
IDs authoritative with either `trice bind` or the legacy `trice insert`, then
generate only the entries currently referenced by the selected source files:

```sh
trice bind -src <source> -til til.json
# Alternatively: trice insert -src <source> -til til.json

trice generate -src <source> -til til.json -logC=build/til.c
```

Repeat `-src` for additional files or directories. For bind-owned sources the
default sidecar directory is `build/triceIDs`; use `-bindDir` only when the
File Key sidecars are stored elsewhere. Missing, stale, or ambiguous IDs stop
generation before the output file is replaced. `-abc` and `-logC` are separate
generation modes and cannot be selected together.

The generated C file and the ordinary Trice sources must be part of the target
build. The format strings in that table consume target Flash. This is the
deliberate size cost of producing readable text locally instead of decoding
the ID on a host.

##### Formatter hook

Install an `snprintf`-compatible function before consuming records:

```c
#include <stdio.h>
#include "trice.h"

UserTriceLogPrintfFn = snprintf;
```

The system, newlib, or picolibc `snprintf`, nanoprintf, eyalroz/printf, and an
snprintf-compatible adapter around stb_sprintf are possible implementations.
The hook must return the number of bytes that would have been written without
the final NUL, including when the destination is too small.

TriceLog invokes the hook once per scalar numeric conversion. It does not
build an intermediate value array. Runtime strings are copied separately and
do not invoke the hook.

When no external formatter is needed, the separately linked minimal formatter
can handle exact `%d` and `%x`:

```c
#define TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER 1
```

Set this switch to `0` when the application always installs a hook. The
minimal formatter then contributes no code even without LTO or linker section
garbage collection. It intentionally does not implement field widths,
floating-point conversions, or a second general-purpose printf.
Literal `%%` and bounded `%s` processing belong to the record formatter and do
not require either printf implementation.

##### Calling TriceLog

One background task typically drains all currently available records:

```c
char text[160];
int length;

while ((length = TriceLog(text, sizeof(text))) > 0) {
    ApplicationTextWrite(text, (size_t)length);
}
```

The result contract is:

- A positive value is the number of visible output bytes, excluding the final
  NUL.
- Zero means no complete printable record is currently available.
- A negative value is one of the `TRICE_LOG_ERR_*` constants declared in
  `triceLog.h`.
- `maxlen` is the complete output capacity including the final NUL.
- For every valid non-NULL buffer with `maxlen > 0`, the result is
  NUL-terminated, including error results.
- A one-byte buffer can represent only an empty C string. A waiting non-empty
  record is consumed and `TRICE_LOG_ERR_OUTPUT_TOO_SMALL` is returned.

Invalid API arguments do not consume a record. A record-local error such as an
unsupported format, an unknown ID, or insufficient output space consumes the
offending complete record so the next call can make progress. Structural
corruption or metadata that makes the next record boundary untrustworthy
clears the deferred queue.

Only one task may consume a given deferred buffer. When producers run in
multiple tasks or interrupts, configure the normal Trice critical-section
macros appropriate for the target. `TriceLog()` performs no dynamic memory
allocation. It reads the binary payload at its current buffer location and
releases the record only after formatting has finished.

##### Integer formats

With an external formatter hook, local logging accepts the scalar conversions
`%d`, `%i`, `%u`, `%o`, `%x`, `%X`, and `%c`. Constant flags, widths, and
precisions are passed to the hook. Source length modifiers are normalized to
the `long` or `long long` argument actually supplied by TriceLog.

Dynamic `*` width or precision is not accepted because it would consume
additional arguments which are not represented as an independent Trice value.

##### Runtime strings and NUL termination

Use `triceS` for a runtime-generated C string:

```c
triceS("text:name=[%s]\n", name);
triceS("text:short=[%-12.5s]\n", name);
```

Local logging supports exactly one `%s` conversion in a dynamic string record.
Literal `%%`, a constant field width, a constant precision, and left alignment
with `-` are supported. Other `%s` flags, wide strings, multiple `%s`
conversions, and dynamic `*` width or precision are rejected.

The input of `triceS` is NUL-terminated, but the terminator is intentionally
not stored in the binary Trice payload:

```text
input C string:  A B C NUL
binary payload:  A B C       with payload length 3
```

The payload length already records where the string ends. Omitting the
redundant NUL saves one byte and is also how the underlying length-based
`triceN` transport works. TriceLog therefore never passes the buffered payload
directly to `snprintf("%s")`, which could read into alignment padding or the
next record while searching for a terminator. Instead, it copies at most the
recorded payload length directly into the application output buffer and adds
the output buffer's final NUL itself.

String-form `triceN` records receive the same bounded treatment. Because
`triceN` may contain arbitrary bytes, an embedded NUL retains normal `%s`
semantics and ends the visible string before the explicit payload length.
Other dynamic buffer formats remain unsupported.

##### Float and double formats

Floating-point values retain the established Trice source convention:

```c
trice32("float:value=%.3f\n", aFloat(valueF));
trice64("float:value=%.9f\n", aDouble(valueD));
```

`aFloat()` preserves a `float` as its 32-bit payload bit pattern. Use it with a
32-bit Trice macro. `aDouble()` preserves a `double` as its 64-bit payload bit
pattern and requires a 64-bit Trice macro. TriceLog reconstructs the value from
that width and passes it as a variadic `double` to the configured formatter.

The conversions `%f`, `%F`, `%e`, `%E`, `%g`, and `%G`, including constant
flags, widths, and precisions, are supported through the external hook. The
source spellings `%lf` and `%Lf` are normalized to the same variadic `double`
call. Floating-point conversion is rejected for 8-bit and 16-bit records and
when only the minimal formatter is available.

Enabling floating-point output can significantly increase the formatter's
Flash use. For example, the G0B1 demonstration enables nanoprintf float support
explicitly; an integer-and-string-only application can leave that option off.

##### Unsupported records

The initial local formatter does not handle dynamic binary buffer rendering,
ABC records, pointer conversions, Trice Boolean extensions, wide characters,
wide strings, or host-side color/tag processing. Unsupported complete records
return `TRICE_LOG_ERR_FORMAT` and are consumed. Their binary wire encoding and
normal host decoding remain unchanged.

##### Examples

[`examples/PC_log`](../examples/PC_log/README.md) is an immediately runnable
host program using the system `snprintf` and standard output.

[`examples/G0B1_log`](../examples/G0B1_log/README.md) demonstrates a FreeRTOS
background task, nanoprintf, a ring buffer, and plain-text USART2 output. Both
examples include runtime strings, string width/precision, 32-bit float values,
64-bit double values, and additional numeric formatting cases.
