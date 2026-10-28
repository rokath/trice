# Target Implementation

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. TRICE Macro structure](#trice-macro-structure)
  * [1.1. TRICE_ENTER](#trice_enter)
  * [1.2. TRICE_PUT](#trice_put)
  * [1.3. TRICE_LEAVE](#trice_leave)
* [2. TRICE_STACK_BUFFER](#trice_stack_buffer)
* [3. TRICE_STATIC_BUFFER](#trice_static_buffer)
* [4. TRICE_DOUBLE_BUFFER](#trice_double_buffer)
* [5. TRICE_RING_BUFFER](#trice_ring_buffer)
* [6. Deferred Out](#deferred-out)
  * [6.1. Double Buffer](#double-buffer)
  * [6.2. Ring Buffer](#ring-buffer)
  * [6.3. Local Deferred Text Log](#local-deferred-text-log)
    * [6.3.1. Basic configuration](#basic-configuration)
    * [6.3.2. Configuration switches](#configuration-switches)
    * [6.3.3. Optional tag presentation](#optional-tag-presentation)
    * [6.3.4. Selecting a printf implementation](#selecting-a-printf-implementation)
    * [6.3.5. Formatter families](#formatter-families)
    * [6.3.6. Optional prefix hook](#optional-prefix-hook)
    * [6.3.7. Calling `TriceLog`](#calling-tricelog)
    * [6.3.8. Size-oriented configurations](#size-oriented-configurations)
    * [6.3.9. Examples](#examples)
* [7. Direct Transfer](#direct-transfer)
* [8. Possible Target Code Improvements](#possible-target-code-improvements)
* [9. Target Code Overview](#target-code-overview)
* [10. Obsolete RPC support](#obsolete-rpc-support)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## 1. <a id="trice-macro-structure"></a>TRICE Macro structure

### 1.1. <a id="trice_enter"></a>TRICE_ENTER

* Optionally disable interrupts.
* Prepare `TriceBufferWritePosition` and keep its initial value.

### 1.2. <a id="trice_put"></a>TRICE_PUT

* Use and increment `TriceBufferWritePosition`.

### 1.3. <a id="trice_leave"></a>TRICE_LEAVE

* Use `TriceBufferWritePosition` and its initial value for data transfer
* Optionally restore interrupt state.

## 2. <a id="trice_stack_buffer"></a>TRICE_STACK_BUFFER

* `TRICE_ENTER`: Allocate stack
* `TRICE_LEAVE`: Call TriceDirectOut()

## 3. <a id="trice_static_buffer"></a>TRICE_STATIC_BUFFER

* This is like `TRICE_STACK_BUFFER` but avoids stack allocation, what is better for many stacks.
* `TRICE_ENTER`: Set TriceBufferWritePosition to buffer start.
* `TRICE_LEAVE`: Call TriceDirectOut().

## 4. <a id="trice_double_buffer"></a>TRICE_DOUBLE_BUFFER

* `TRICE_ENTER`: Keep TriceBufferWritePosition.
* `TRICE_LEAVE`: Optionally call TriceDirectOut().

## 5. <a id="trice_ring_buffer"></a>TRICE_RING_BUFFER

* `TRICE_ENTER`: Keep or wrap TriceBufferWritePosition and add offset.
* `TRICE_LEAVE`: Optionally call TriceDirectOut().

The `TRICE_RING_BUFFER` allocates incremental ring buffer space and each trice location is read by a deferred task.

## 6. <a id="deferred-out"></a>Deferred Out

### 6.1. <a id="double-buffer"></a>Double Buffer

* TriceTransfer
  * TriceOut
  * TriceNonBlockingWrite( triceID, enc, encLen );

### 6.2. <a id="ring-buffer"></a>Ring Buffer

* TriceTransfer
  * lastWordCount = TriceSingleDeferredOut(addr);
    * int triceID = TriceIDAndBuffer( pData, &wordCount, &pStart, &Length );
    * TriceNonBlockingWrite( triceID, pEnc, encLen );

### 6.3. <a id="local-deferred-text-log"></a>Local Deferred Text Log

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

#### 6.3.1. <a id="basic-configuration"></a>Basic configuration

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

trice generate -src <source> -til til.json -logC
```

Repeat `-src` for additional files or directories. Bind sidecars are read from
`./generated` by default; specify `-genDir` for a different sidecar
directory. Bare `-logC` writes `./generated/til.c`; `-logC=build/til.c` chooses an explicit location. `-logC` and `-abc` are separate generator modes and cannot be used
together.

The generated C file includes compile-time guards derived from its Trice type,
format string, and payload width. It therefore uses the same local-log switches
as the formatter. A disabled capability removes the affected format strings
from the compiled target image; regenerating `til.c` for every option change is
not required. Regeneration is required after source IDs, Trice types, or format
strings change.

#### 6.3.2. <a id="configuration-switches"></a>Configuration switches

The switches are deliberately independent of any printf implementation. Their
names are similar to familiar nanoprintf choices where useful, but Trice neither
includes nanoprintf nor defines `NANOPRINTF_*` macros.

Switch | Default | Effect when set to `1`
--- | ---: | ---
`TRICE_LOCAL_LOG` | `0` | Includes the local consumer and formatter.
`TRICE_LOCAL_LOG_USE_PRINTF_HOOK` | `1` | Declares and uses the runtime `UserTriceLogPrintfFn` hook.
`TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER` | `1` | Includes the separate exact `%d`/`%x` fallback.
`TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS` | `1` | Accepts fixed widths such as `%8d`, `%02x`, and `%-12s`.
`TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS` | `1` | Accepts fixed precision such as `%.3f` and `%.5s`.
`TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS` | `1` | Accepts `%e`, `%E`, `%f`, `%F`, `%g`, and `%G` through the hook.
`TRICE_LOCAL_LOG_USE_64_BIT_VALUES` | `1` | Includes 64-bit integer, buffer, and `aDouble()` paths.
`TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS` | `0` | Includes Trice's internal `%b` conversion.
`TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG` | `1` | Accepts `#`, including the internal `%#b` prefix.
`TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS` | `0` | Includes internal `%O`, `%t`, `%p`, and `%q` conversions.
`TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES` | `1` | Includes bounded `triceS` and string-form `triceN`.
`TRICE_LOCAL_LOG_USE_BUFFER_TRICES` | `0` | Includes `TRICE8_B` through `TRICE64_B`.
`TRICE_LOCAL_LOG_USE_PREFIX_HOOK` | `0` | Declares and calls `UserTriceLogPrefixFn` before each message body.
`TRICE_LOCAL_LOG_USE_ANSI_COLORS` | `0` | Colors records which begin with a recognized Trice tag.
`TRICE_LOCAL_LOG_STRIP_LOWER_CASE_TAGS` | `0` | Removes a recognized all-lower-case tag and its first colon.
`TRICE_LOCAL_LOG_KEEP_DISABLED_IDS` | `1` | Keeps ID and shape metadata, but no string, for disabled generated entries.

Every switch is Boolean and invalid values fail during preprocessing. Defaults
preserve the original local-log integer, string, width, precision, float, and
64-bit capabilities. The newer binary, extended, buffer, and prefix paths are
opt-in so an existing target does not acquire avoidable code or data.

`TRICE_LOCAL_LOG_KEEP_DISABLED_IDS == 1` lets `TriceLog()` distinguish a known
ID whose formatter feature is off from an absent or obsolete ID. It returns
`TRICE_LOG_ERR_FEATURE_DISABLED` for the former. Set it to `0` for the smallest
table; such an ID then returns `TRICE_LOG_ERR_ID` because its row is absent.

#### 6.3.3. <a id="optional-tag-presentation"></a>Optional tag presentation

The two presentation switches are independent. With both set to `0`, local
logging returns exactly the formatted record body as before. Enabling only
`TRICE_LOCAL_LOG_STRIP_LOWER_CASE_TAGS` changes `msg:Hello` to `Hello`, but
keeps `MSG:Hello`, `Message:Hello`, and unknown `project:Hello` prefixes. The
test is an exact match against a recognized built-in alias; arbitrary text
before a colon is never removed.

Enabling only `TRICE_LOCAL_LOG_USE_ANSI_COLORS` retains the tag and surrounds
the complete record body with its matching ANSI Select Graphic Rendition
sequence. Enabling both removes a recognized lower-case tag and colors the
remaining body. A reset is inserted before trailing CR and LF bytes in every
record. Color state therefore never crosses a `TriceLog()` call, even when
several tasks produce records or the consumer encounters an error. An optional
prefix hook remains outside the colored body.

The target alias and palette table is in `src/triceLogAnsi.c`. It intentionally
mirrors the established host hints in
`internal/emitter/lineTransformerANSI.go` instead of generating a target
dependency on Go. Reciprocal comments in those files identify the manual,
optional synchronization point. User-defined host labels and host log-level
filtering are not part of local presentation. Applications which compile an
explicit source list must add `triceLogAnsi.c` when either presentation switch
is enabled; source-wildcard builds already pick it up with the other Trice C
files.

ANSI bytes are ordinary output bytes. A compatible serial terminal interprets
them as colors, while a redirected file retains the escape sequences. Disable
the color switch for consumers which require plain text. When both presentation
switches are off, the separately compiled `triceLogAnsi.c` contributes neither
code nor its alias and palette strings, even without link-time optimization.

#### 6.3.4. <a id="selecting-a-printf-implementation"></a>Selecting a printf implementation

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

#### 6.3.5. <a id="formatter-families"></a>Formatter families

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
log-level filtering, user-defined host labels, and host-side location
presentation. These restrictions apply only to `TriceLog()` and its generated
target table; they do not remove the corresponding records or decoder behavior
from normal binary logging.

#### 6.3.6. <a id="optional-prefix-hook"></a>Optional prefix hook

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

#### 6.3.7. <a id="calling-tricelog"></a>Calling `TriceLog`

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

#### 6.3.8. <a id="size-oriented-configurations"></a>Size-oriented configurations

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

#### 6.3.9. <a id="examples"></a>Examples

[`examples/PC_log`](examples.md#pc-local-logging) is an immediately runnable
host program using the system `snprintf` and standard output. After a short
introductory sequence, it scans the complete shared
`_test/testdata/triceCheck.c` producer corpus and fails with the exact selector
and local-log error if an emitted record cannot be formatted.

[`examples/G0B1_log`](examples.md#g0b1-freertos-local-logging) is an independent STM32G0B1
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

## 7. <a id="direct-transfer"></a>Direct Transfer

* TRICE_LEAVE
  * TriceDirectWrite(triceSingleBufferStartWritePosition, wordCount);
    * optional RTT32 with optional XTEAwithCOBS
    * optional RTT8  with optional XTEAwithCOBS
    * optional
      * triceIDAndLen
      * triceDirectEncode
      * triceNonBlockingDirectWrite

## 8. <a id="possible-target-code-improvements"></a>Possible Target Code Improvements

There have been 3 similar implementations for trice encode

```C
static size_t triceDirectEncode(   uint8_t* enc, const uint8_t * buf, size_t len );
       size_t TriceDeferredEncode( uint8_t* enc, const uint8_t * buf, size_t len );

unsigned TriceEncryptAndCobsFraming32( uint32_t * const triceStart, unsigned wordCount ){
```

Now:

```C
size_t TriceEncode( unsigned encrypt, unsigned framing, uint8_t* dst, const uint8_t * buf, size_t len ){
unsigned TriceEncryptAndCobsFraming32( uint32_t * const triceStart, unsigned wordCount ){
```

Currently there are 3 similar implementations for trice buffer reads

```C
static size_t triceIDAndLen(    uint32_t* pBuf,               uint8_t** ppStart, int*      triceID );
static int    TriceNext(        uint8_t** buf,                size_t* pSize,     uint8_t** pStart,    size_t* pLen );
static int    TriceIDAndBuffer( uint32_t const * const pData, int* pWordCount,   uint8_t** ppStart,   size_t* pLength );
```

* The TriceID is only needed for routing and can go in a global variable just for speed.
* The source buffer should be `uint32_t const * const`.
* The destination should be given with `uint32_t * const` and the return value is the trice netto size. For efficiency the result should be ready encoded.

```C
//! \param pTriceID is filled with ID for routing
//! \param pCount is used for double or ring buffer to advance inside the buffer
//! \param dest provides space for the encoded trice
//! \param src is the location of the trice message we want encode
//! \retval is the netto size of the encoded trice data
size_t TriceEncode(int* pTriceID, unsigned int pCount, uint32_t * const dest, uint32_t const * const src );
```

* This function interface is used for all cases.
* First we use the existing code for implementation and then we clean the code.

## 9. <a id="target-code-overview"></a>Target Code Overview

* `./src`: **User Interface**

File                      | description
---------------------------|----------------------------------------------------------------------------------------------------------------------------------------------
[trice.h](../../src/trice.h) | trice runtime lib user interface, `#include trice.h` in project files, where to use Trice macros. Add `./src` to your compiler include path.
`triceConfig.h`           | Create this file to overwrite  [triceDefaultConfig.h](../../src/triceDefaultConfig.h) as needed.

* `./src`: **Internal Components** (only partially needed, add all to your project - the configuration selects automatically)

See [Trice Project Image Size Optimization](memory_usage.md) for compiling `src/*.c` and removing unused library sections from the final image.

File                                                | description
-----------------------------------------------------|----------------------------------------------------------------------------------------------------------------------
[cobs.h](../../src/cobs.h)                             | message packaging, alternatively for tcobs
[cobsEncode.c](../../src/cobsEncode.c)                 | message encoding, alternatively for tcobs
[cobsDecode.c](../../src/cobsDecode.c)                 | message decoding, normally not needed
[trice.c](../../src/trice.c)                           | trice core lib
[trice8McuOrder.h](../../src/trice8McuOrder.h)         | trice MCU endianness lib
[trice8McuReverse.h](../../src/trice8McuReverse.h)     | trice MCU reverse endianness lib
[trice16McuOrder.h](../../src/trice16McuOrder.h)       | trice MCU endianness lib
[trice16McuReverse.h](../../src/trice16McuReverse.h)   | trice MCU reverse endianness lib
[trice32McuOrder.h](../../src/trice32McuOrder.h)       | trice MCU endianness lib
[trice32McuReverse.h](../../src/trice32McuReverse.h)   | trice MCU reverse endianness lib
[trice64McuOrder.h](../../src/trice64McuOrder.h)       | trice MCU endianness lib
[trice64McuReverse.h](../../src/trice64McuReverse.h)   | trice MCU reverse endianness lib
[SEGGER_RTT.h](../../src/SEGGER_RTT.h)                 | Segger RTT code interface
[SEGGER_RTT.c](../../src/SEGGER_RTT.c)                 | Segger RTT code
[SEGGER_RTT_ConfDefaults.h](../../src/SEGGER_RTT_ConfDefaults.h) | SEGGER defaults for settings not supplied by the project
[SEGGER_RTT_Conf.h](../../src/default_conf/SEGGER_RTT_Conf.h) | Small Trice fallback configuration; project headers take precedence
[SEGGER_RTT_LICENSE.md](../../src/SEGGER_RTT_LICENSE.md) | SEGGER license to retain when redistributing its target sources
[tcobs.h](../../src/tcobs.h)                           | message compression and packaging interface
[tcobsv1Encode.c](../../src/tcobsv1Encode.c)           | message encoding and packaging
[tcobsv1Decode.c](../../src/tcobsv1Decode.c)           | message decoding and packaging, normally not needed
[tcobsv1Internal.h](../../src/tcobsv1Internal.h)       | message decoding and packaging internal interface
[trice8.h](../../src/trice8.h)                         | 8-bit trice code interface
[trice8.c](../../src/trice8.c)                         | 8-bit trice code
[trice16.h](../../src/trice16.h)                       | 16-bit trice code interface
[trice16.c](../../src/trice16.c)                       | 16-bit trice code
[trice32.h](../../src/trice32.h)                       | 32-bit trice code interface
[trice32.c](../../src/trice32.c)                       | 32-bit trice code
[trice64.h](../../src/trice64.h)                       | 64-bit trice code interface
[trice64.c](../../src/trice64.c)                       | 64-bit trice code
[triceAuxiliary.c](../../src/triceAuxiliary.c)         | trice code for auxiliary interfaces
[triceDefaultConfig.h](../../src/triceDefaultConfig.h) | This file contains the most probably settings and serves also as a reference for tuning your project *triceConfig.h*
[triceDoubleBuffer.c](../../src/triceDoubleBuffer.c)   | trice runtime lib extension needed for fastest deferred mode
[triceStackBuffer.c](../../src/triceStackBuffer.c)     | trice runtime lib extension needed for direct mode
[triceRingBuffer.c](../../src/triceRingBuffer.c)       | trice runtime lib extension needed for recommended deferred mode
[xtea.h](../../src/xtea.h)                             | XTEA message encryption/decryption interface
[xtea.c](../../src/xtea.c)                             | XTEA message encryption/decryption code

* The *tcobs\*.\** files are copied from [tcobs v1](https://github.com/rokath/tcobs). They are maintained there and extensively tested and probably not a matter of significant change.
* The SEGGER files are copied and you could check for a newer version at [https://www.segger.com/downloads/jlink/](https://www.segger.com/downloads/jlink/).


## 10. <a id="obsolete-rpc-support"></a>Obsolete RPC support

The older RPC implementation is obsolete; it is separate from current [ABC commands](abc_asynchronous_broadcast_commands.md). Some library sources, tests and examples still contain RPC compatibility code. Do not enable it for a new integration. Removing it from example code is a separate task; this documentation reorganization does not change those sources.

