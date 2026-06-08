# Trice ABC Implementation Specification

Status: implementation draft for Codex.

Feature name:

```text
Trice ABC - Asynchronous Broadcast Commands
```

Public macro suffix:

```text
C  // Command
```

This document specifies the first implementable Trice ABC version. It uses the simplified `trice generate -abc=<target>` workflow and keeps the existing legacy `triceF` / `-rpcH` / `-rpcC` path unchanged.

The primary design goal is a small, testable building block: a Trice message can be emitted as an asynchronous broadcast command, and a receiving target can map its Trice ID to a locally compiled handler selected for that target.

## Table of Contents

- [1. Goal](#1-goal)
- [2. What ABC deliberately does not implement](#2-what-abc-deliberately-does-not-implement)
- [3. Existing legacy path](#3-existing-legacy-path)
- [4. Terminology](#4-terminology)
- [5. Configuration switches](#5-configuration-switches)
- [6. Public ABC send macros](#6-public-abc-send-macros)
- [7. Target-side ABC stamp writing](#7-target-side-abc-stamp-writing)
- [8. Host-side parser and TIL behavior](#8-host-side-parser-and-til-behavior)
- [9. Generator CLI](#9-generator-cli)
- [10. `<target>_abc.h` selection file](#10-target_abch-selection-file)
- [11. `<target>_abc.c` generated file](#11-target_abcc-generated-file)
- [12. Generator matching and conflict rules](#12-generator-matching-and-conflict-rules)
- [13. ABC receive runtime](#13-abc-receive-runtime)
- [14. Current ABC stamp context](#14-current-abc-stamp-context)
- [15. Multiple devices and TIL management](#15-multiple-devices-and-til-management)
- [16. Security boundary](#16-security-boundary)
- [17. Tests](#17-tests)
- [18. Target-code examples](#18-target-code-examples)
- [19. Recommended implementation order](#19-recommended-implementation-order)
- [20. Follow-up items](#20-follow-up-items)
- [21. Non-negotiable constraints](#21-non-negotiable-constraints)

## 1. Goal

Implement an optional Trice ABC feature that allows selected Trice messages to be interpreted by receiving devices as asynchronous broadcast commands.

A Trice ABC sender emits a normal Trice message with the same binary framing and record structure as other Trice messages:

- normal 14-bit Trice ID,
- normal 2-bit Trice stamp selector,
- optional ABC stamp stored in the existing Trice stamp field,
- normal Trice count/cycle field,
- optional payload bytes.

There is no new package format. The difference between a normal Trice and a Trice ABC message is the macro family used in source code, the TIL metadata, the interpretation of the optional stamp field, and optional receive-side ID-to-handler mapping.

At runtime, the sender does not wait and does not need to know which devices will react. A receiver may map the received Trice ID to a locally compiled handler selected for this target.

Conceptually:

```text
sender emits ABC with Trice ID X and optional payload P

receiver A: ID X -> local handler A
receiver B: ID X -> local handler B
receiver C: ID X -> no selected handler, ignore
```

The same Trice ID may therefore cause different local behavior on different targets, depending on the target-local `<target>_abc.h` selection and application implementation. One specific ID X is mapped to the identical handler name on different devices, but their implementation may differ.

## 2. What ABC deliberately does not implement

The ABC core shall not implement:

- reliable delivery,
- retries,
- acknowledgements,
- wait-for-response logic,
- quorum logic,
- routing,
- discovery,
- authentication or authorization,
- consensus,
- persistent command queues,
- automatic deferred handler scheduling.

Applications may implement these policies above ABC.

ABC is not RPC. It has no direct return value, no required peer, no required reply, and no exactly-one-receiver guarantee.

ABC is not remote code execution. It does not transfer executable code. It only transfers a Trice ID, optional ABC stamp, and payload bytes. Receivers can execute only locally compiled handlers that are selected for that firmware.

ABC is not publish-only messaging. The message is action-oriented: interested receivers may treat it as a command.

## 3. Existing legacy path

The repository already contains an experimental remote-function-call style path based on `triceF` and `rpc` naming.

Existing behavior to preserve initially:

- `triceF`, `TriceF`, `TRiceF`, `TRICE*_F`, and their bit-width variants remain unchanged for existing users.
- `trice generate -rpcH -rpcC` remains unchanged.
- Existing generated RPC filenames and symbols remain unchanged.
- ABC generation does not select `F` entries.

Required changes around the legacy path:

- Documentation marks `triceF` and `-rpcH` / `-rpcC` as deprecated.
- Legacy `triceF` target code is gated behind the new configuration switch `TRICE_LEGACY_RPC_SUPPORT`.
- The default is off.

Default configuration:

```c
#ifndef TRICE_LEGACY_RPC_SUPPORT
#define TRICE_LEGACY_RPC_SUPPORT 0
#endif
```

Existing users who still use `triceF` enable the legacy target code in their project-specific `triceConfig.h`:

```c
#define TRICE_LEGACY_RPC_SUPPORT 1
```

This switch must not change host-side `-rpcH` / `-rpcC` generator behavior.

Do not introduce or use a second legacy switch name such as `TRICE_RPC`.

## 4. Terminology

### 4.1. ABC command

A Trice message emitted through a `triceC` macro family and intended to be interpretable as a command by receivers.

### 4.2. ABC Trice ID

The normal 14-bit Trice ID associated with an ABC command. It is the primary receive-table key.

### 4.3. ABC command name

The C handler name extracted from the ABC format string. The generator uses the part after the last colon:

```text
cmd:motor_stop           -> motor_stop
cmd:deviceA:motor_step   -> motor_step
a:b:c:d:e:f              -> f
MotorStop                -> MotorStop
```

The extracted command name must be a valid C identifier:

```text
[A-Za-z_][A-Za-z0-9_]*
```

### 4.4. ABC tag / prefix

Everything before the last colon is display/filter metadata. It may be used for normal Trice tag behavior such as colors, ID ranges, `-pick`, or `-ban`.

The prefix is not ABC addressing and must not define ABC semantics.

### 4.5. ABC stamp

A user-provided correlation value transported in the existing Trice stamp field for ABC messages.

ABC stamps are not necessarily timestamps. Typical meanings are request ID, response correlation ID, device-plus-sequence value, transaction ID, or another application-defined marker.

### 4.6. ABC handler

A locally compiled application function selected by `<target>_abc.h` and referenced through the generated `<target>_abc.c` table.

The generator does not generate weak no-op handlers for ABC.

## 5. Configuration switches

Add these defaults to `triceDefaultConfig.h` or the equivalent default configuration layer:

```c
#ifndef TRICE_ABC_TRANSMIT_SUPPORT
#define TRICE_ABC_TRANSMIT_SUPPORT 0
#endif

#ifndef TRICE_ABC_RECEIVE_SUPPORT
#define TRICE_ABC_RECEIVE_SUPPORT 0
#endif

#ifndef TRICE_LEGACY_RPC_SUPPORT
#define TRICE_LEGACY_RPC_SUPPORT 0
#endif
```

Meaning:

| Switch                            | Meaning                                                |
|-----------------------------------|--------------------------------------------------------|
| `TRICE_ABC_TRANSMIT_SUPPORT == 1` | Enable ABC send macros.                                |
| `TRICE_ABC_RECEIVE_SUPPORT == 1`  | Enable ABC receive runtime types and helper functions. |
| `TRICE_LEGACY_RPC_SUPPORT == 1`   | Enable deprecated legacy `triceF` target code.         |

Transmit and receive support are independent. Do not use a single `TRICE_ABC_SUPPORT` switch unless it is only a convenience alias that expands to the two explicit switches.

When `TRICE_ABC_TRANSMIT_SUPPORT == 0`, active target code shall not contain usable ABC send macro implementations. This keeps ABC target code out of builds that do not use it. `TRICE_OFF` and `TRICE_CLEAN` behavior must remain compatible with normal Trice behavior.

## 6. Public ABC send macros

The public macro suffix is `C`, meaning Command.

ABC stamped macros take the stamp argument after the format string:

```c
Trice16C("cmd:name", stamp16, payload, count);
TRice16C("cmd:name", stamp32, payload, count);
```

This order is now part of the design.

### 6.1. Clean user source forms

Clean source forms do not contain inserted IDs yet.

No payload:

```c
triceC("cmd:motor_stop");
TriceC("cmd:motor_stop", stamp16);
TRiceC("cmd:motor_stop", stamp32);
```

8-bit payload:

```c
trice8C("cmd:set_leds", p8, cnt);
Trice8C("cmd:set_leds", stamp16, p8, cnt);
TRice8C("cmd:set_leds", stamp32, p8, cnt);
```

16-bit payload:

```c
trice16C("cmd:set_pwm", p16, cnt);
Trice16C("cmd:set_pwm", stamp16, p16, cnt);
TRice16C("cmd:set_pwm", stamp32, p16, cnt);
```

32-bit payload:

```c
trice32C("cmd:set_time", p32, cnt);
Trice32C("cmd:set_time", stamp16, p32, cnt);
TRice32C("cmd:set_time", stamp32, p32, cnt);
```

64-bit payload:

```c
trice64C("cmd:set_epoch", p64, cnt);
Trice64C("cmd:set_epoch", stamp16, p64, cnt);
TRice64C("cmd:set_epoch", stamp32, p64, cnt);
```

The first ABC implementation shall support no-payload `triceC` / `TriceC` / `TRiceC` and width-specific payload forms. A default-width payload form for `triceC` is not required for the first implementation.

### 6.2. Inserted source forms

`trice insert` adds a raw Trice ID argument to ABC macro calls.

For ABC macros, the inserted ID argument must be `iD(n)` or an equivalent raw ID expression. It must not be `ID(n)` or `Id(n)`, because those macros inject normal Trice timestamp sources.

Clean source:

```c
TRice32C("cmd:set_time", stamp32, p32, cnt);
```

Inserted source:

```c
TRice32C(iD(12347), "cmd:set_time", stamp32, p32, cnt);
```

More inserted examples:

```c
triceC(iD(12345), "cmd:motor_stop");
Trice16C(iD(12346), "cmd:set_pwm", stamp16, p16, cnt);
TRice32C(iD(12347), "cmd:set_time", stamp32, p32, cnt);
trice8C(iD(12348), "cmd:set_leds", p8, cnt);
```

`iD(n)` is preferred over a bare numeric literal for consistency with existing Trice parser/helper conventions.

### 6.3. Explicit-ID `_C` macro forms

The explicit-ID forms are useful for tests and for users who intentionally write IDs themselves. They already contain the ID argument and are not clean no-ID source forms.

No payload:

```c
TRIce_C(iD(12345), "cmd:motor_stop");               // no ABC stamp
TRICe_C(iD(12346), "cmd:motor_stop", stamp16);      // 16-bit ABC stamp
TRICE_C(iD(12347), "cmd:motor_stop", stamp32);      // 32-bit ABC stamp
```

8-bit payload:

```c
TRIce8_C(iD(12345), "cmd:set_leds", p8, cnt);
TRICe8_C(iD(12346), "cmd:set_leds", stamp16, p8, cnt);
TRICE8_C(iD(12347), "cmd:set_leds", stamp32, p8, cnt);
```

16-bit payload:

```c
TRIce16_C(iD(12345), "cmd:set_pwm", p16, cnt);
TRICe16_C(iD(12346), "cmd:set_pwm", stamp16, p16, cnt);
TRICE16_C(iD(12347), "cmd:set_pwm", stamp32, p16, cnt);
```

32-bit payload:

```c
TRIce32_C(iD(12345), "cmd:set_time", p32, cnt);
TRICe32_C(iD(12346), "cmd:set_time", stamp16, p32, cnt);
TRICE32_C(iD(12347), "cmd:set_time", stamp32, p32, cnt);
```

64-bit payload:

```c
TRIce64_C(iD(12345), "cmd:set_epoch", p64, cnt);
TRICe64_C(iD(12346), "cmd:set_epoch", stamp16, p64, cnt);
TRICE64_C(iD(12347), "cmd:set_epoch", stamp32, p64, cnt);
```

If a user writes `iD(0)` in an explicit-ID form, `trice insert` may patch it to `iD(n)` as it does for other explicit-ID Trice forms. It must not rewrite stamped ABC explicit forms to `ID(n)` or `Id(n)`.

If easy possible, explicit-ID `_C` macro forms are allowed eo be noted without the `iD(0), ` part. The `trice insert` command inserts then `iD(n), `. The `trice clean` command should remove the `iD(n), ` part completely, if easy achievable. When the code gets too complicated, the `iD(0), ` form is acceptable because these macros are mainly helper macros and the user should avoid using them directly.

## 7. Target-side ABC stamp writing

Normal Trice helper macros behave as follows:

```c
ID(n)  // writes ID/header plus TriceStamp32
Id(n)  // writes ID/header plus TriceStamp16
id(n)  // writes ID/header without stamp
iD(n)  // raw parser/helper ID expression
```

ABC stamped messages must write the user-supplied ABC stamp instead of `TriceStamp16` or `TriceStamp32`.

### 7.1. Stamp-only helpers

Add stamp-only helpers with collision-safe names:

```c
#define TRICE_C_STAMP32(stamp32)  /* write user supplied 32-bit ABC stamp */
#define TRICE_C_STAMP16(stamp16)  /* write user supplied 16-bit ABC stamp */
```

These helpers have no ID semantics.

They shall write the stamp in the same transfer byte order as normal Trice stamps.

### 7.2. ABC ID/header helpers

Add ABC ID/header helpers as needed:

```c
#define TRICE_C_ID32(n)  /* write ID/header for following 32-bit ABC stamp */
#define TRICE_C_ID16(n)  /* write ID/header for following 16-bit ABC stamp */
#define TRICE_C_ID0(n)   /* write ID/header without stamp */
```

These helpers are not equivalent to `ID(n)` or `Id(n)`.

Required behavior:

- `TRICE_C_ID32(n)` writes the same ID/header representation as a normal 32-bit-stamped Trice before the stamp, but does not fetch or write `TriceStamp32`.
- `TRICE_C_ID16(n)` writes the same ID/header representation as a normal 16-bit-stamped Trice before the stamp, but does not fetch or write `TriceStamp16`.
- `TRICE_C_ID0(n)` writes the same no-stamp ID/header representation as `id(n)` if the binary output is identical.

Stamped ABC send paths must be binary-compatible with normal stamped Trice records, except that the stamp field contains the explicit ABC stamp argument.

No-stamp ABC messages may reuse existing no-stamp counted-buffer code if the binary output is identical.

### 7.3. ABC counted-buffer cores

Stamped ABC messages need ABC-specific counted-buffer cores because the stamp value is explicit.

Recommended internal shape:

```c
#define TRICE_C_N32(tid, pFmt, stamp32, buf, nBytes)    \
    do {                                                \
        TRICE_UNUSED(pFmt);                             \
        /* length limit and truncation as in TRICE_N */ \
        TRICE_ENTER                                     \
        tid;                                            \
        TRICE_C_STAMP32(stamp32);                       \
        /* count/cycle and payload as in TRICE_N */     \
        TRICE_LEAVE                                     \
    } while (0)

#define TRICE_C_N16(tid, pFmt, stamp16, buf, nBytes)    \
    do {                                                \
        TRICE_UNUSED(pFmt);                             \
        /* length limit and truncation as in TRICE_N */ \
        TRICE_ENTER                                     \
        tid;                                            \
        TRICE_C_STAMP16(stamp16);                       \
        /* count/cycle and payload as in TRICE_N */     \
        TRICE_LEAVE                                     \
    } while (0)

#define TRICE_C_N0(tid, pFmt, buf, nBytes) \
    TRICE_N(tid, pFmt, buf, nBytes)
```

The exact macro names may follow existing target-code style, but the separation between explicit ABC stamps and normal `TriceStamp*` sources is mandatory.

### 7.4. Payload byte conversion

Payload wrappers convert element count to byte count:

```text
trice8C  / Trice8C  / TRice8C  -> nBytes = 1 * cnt
trice16C / Trice16C / TRice16C -> nBytes = 2 * cnt
trice32C / Trice32C / TRice32C -> nBytes = 4 * cnt
trice64C / Trice64C / TRice64C -> nBytes = 8 * cnt
```

This shall match the existing counted-buffer behavior used by `B` and legacy `F` forms.

## 8. Host-side parser and TIL behavior

### 8.1. Macro recognition

Recognize the new ABC macro family.

Function-like forms:

| No stamp   | 16-bit ABC stamp | 32-bit ABC stamp |
|------------|------------------|------------------|
| `triceC`   | `TriceC`         | `TRiceC`         |
| `trice8C`  | `Trice8C`        | `TRice8C`        |
| `trice16C` | `Trice16C`       | `TRice16C`       |
| `trice32C` | `Trice32C`       | `TRice32C`       |
| `trice64C` | `Trice64C`       | `TRice64C`       |

Explicit-ID `_C` forms:

| No stamp    | 16-bit ABC stamp | 32-bit ABC stamp |
|-------------|------------------|------------------|
| `TRIce_C`   | `TRICe_C`        | `TRICE_C`        |
| `TRIce8_C`  | `TRICe8_C`       | `TRICE8_C`       |
| `TRIce16_C` | `TRICe16_C`      | `TRICE16_C`      |
| `TRIce32_C` | `TRICe32_C`      | `TRICE32_C`      |
| `TRIce64_C` | `TRICe64_C`      | `TRICE64_C`      |

The TIL entry must preserve the original macro type string so ABC entries can be distinguished from normal Trices, buffer Trices, and deprecated `F` entries.

Do not classify `F` entries as ABC.

### 8.2. Argument recognition

For stamped ABC forms, the stamp argument follows the format string and precedes payload arguments.

Examples:

```c
TriceC("cmd:motor_stop", stamp16);
TRice32C("cmd:set_time", stamp32, p32, cnt);
TRICE32_C(iD(1234), "cmd:set_time", stamp32, p32, cnt);
```

The parser must not interpret the ABC stamp as a Trice payload value.

### 8.3. Bit width and payload count

| Macro family                         | Handler signature                | bitWidth |
|--------------------------------------|----------------------------------|----------|
| `triceC` / `TriceC` / `TRiceC`       | `void name(void)`                | 0        |
| `trice8C` / `Trice8C` / `TRice8C`    | `void name(int8_t* p, int cnt)`  | 8        |
| `trice16C` / `Trice16C` / `TRice16C` | `void name(int16_t* p, int cnt)` | 16       |
| `trice32C` / `Trice32C` / `TRice32C` | `void name(int32_t* p, int cnt)` | 32       |
| `trice64C` / `Trice64C` / `TRice64C` | `void name(int64_t* p, int cnt)` | 64       |

Payload count is runtime-sized, analogous to existing buffer-style `B` and legacy `F` behavior.

### 8.4. Display behavior

ABC payload display can use the existing command/function style:

```text
cmd:set_time(00000001)
cmd:set_pwm(0123)(0456)
cmd:motor_stop()
```

Stamped ABC output should eventually use `stamp:` instead of `time:` unless the user explicitly configures otherwise.

This display refinement is not required before the target-side ABC implementation and generator tests are working. Existing tests may temporarily accept current `time:` output. Once the display refinement is implemented, tests should be updated to require `stamp:` for ABC entries.

Existing tag behavior remains unchanged.

## 9. Generator CLI

Add exactly one primary user-facing ABC generation option:

```bash
trice generate -i til.json -abc=<target>
```

Example:

```bash
trice generate -i til.json -abc=deviceA
```

For `-abc=deviceA`, the generator uses:

```text
deviceA_abc.h
deviceA_abc.c
```

Do not add `-abcH` / `-abcC` as the primary workflow.

The `-abc=<target>` option covers both selection-header initialization and generated source regeneration.

## 10. `<target>_abc.h` selection file

`<target>_abc.h` is a user-editable selection file.

It answers this question:

```text
Which ABC commands shall this target receive and execute?
```

### 10.1. First run

If `<target>_abc.h` does not exist, generate it from all ABC commands found in `til.json`.

Example generated file:

```c
//! \file deviceA_abc.h
//! Trice ABC selection file for target deviceA.
//! Generated once; edit this file to select received ABC commands.

#ifndef DEVICEA_ABC_H_
#define DEVICEA_ABC_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void motor_stop(void);
void get_power_state(void);
void set_time(int32_t* p, int cnt);
void set_pwm(int16_t* p, int cnt);

#ifdef __cplusplus
}
#endif

#endif /* DEVICEA_ABC_H_ */
```

The file contains declarations only. It must not contain weak definitions.

Use classic include guards, not `#pragma once`, to keep ABC headers independent of non-standard compiler extensions.

The include guard name is derived from the filename, converted to uppercase C identifier form. Example:

```text
deviceA_abc.h -> DEVICEA_ABC_H_
```

### 10.2. Later runs

If `<target>_abc.h` exists, use it as input and do not overwrite it.

Active, non-commented declarations define the target-local receive set. Deleted or commented declarations are ignored.

New ABC commands added to `til.json` later are not automatically activated for an existing target. The user may add the corresponding declaration manually or delete/regenerate the selection header.

The selection parser shall strip C comments before looking for declarations:

```c
// void ignored(void);
/* void also_ignored(void); */
void selected(void);
```

The parser is not required to evaluate preprocessor conditionals. Users shall not rely on `#if 0`, `#ifdef`, or similar constructs to deselect ABC handlers in `<target>_abc.h`; use comments or deletion instead.

### 10.3. Declaration syntax

Initial parser support shall handle these canonical forms:

```c
void name(void);
void name(int8_t* p, int cnt);
void name(int16_t* p, int cnt);
void name(int32_t* p, int cnt);
void name(int64_t* p, int cnt);
```

Whitespace differences should be tolerated:

```c
void name ( int32_t * p , int cnt ) ;
```

Canonical generated declarations use `intN_t*`. The signedness is not semantically significant in the Trice binary encoding; applications can cast or copy as needed.

Do not require support for `const intN_t*`, `uintN_t*`, application typedefs, or custom parameter names in the first implementation. Such forms may be added later if needed.

### 10.4. Missing-ID warning

If `<target>_abc.h` declares a handler for which no matching ABC command exists in `til.json`, print a warning and do not emit a table entry for it.

Example:

```text
warning: ABC handler 'calibrate_sensor' declared in deviceA_abc.h but no matching ABC command exists in til.json
```

No warning is required for ABC commands in `til.json` that are not declared in `<target>_abc.h`. That is the intended selection mechanism.

## 11. `<target>_abc.c` generated file

`<target>_abc.c` is generated code and must be regenerated on every `trice generate -abc=<target>` run.

Behavior:

- overwrite the file every run,
- if it is read-only, make it writable first if possible,
- write the new file,
- set it read-only afterwards if the platform allows it.

This read-only behavior is best effort and must not make generation fail on platforms where file attributes are unavailable.

### 11.1. Runtime types

Define shared ABC runtime types in the Trice target library, preferably via `trice.h` when receive support is enabled:

```c
typedef void (*triceAbcHandler_t)(void* payload, int count);

typedef struct {
    uint16_t id;
    uint8_t bitWidth;          // 0, 8, 16, 32, 64
    triceAbcHandler_t fn;
} triceAbc_t;

extern const triceAbc_t triceAbc[];
extern const unsigned triceAbcElements;
```

`<target>_abc.h` should not be the only owner of these types, because it is user-editable.

### 11.2. Generated source shape

Example generated source:

```c
//! \file deviceA_abc.c
//! Trice generated ABC code - do not edit!

#include "deviceA_abc.h"
#include "trice.h"

static void triceAbcCall_get_power_state(void* p, int cnt) {
    TRICE_UNUSED(p);
    TRICE_UNUSED(cnt);
    get_power_state();
}

static void triceAbcCall_set_time(void* p, int cnt) {
    set_time((int32_t*)p, cnt);
}

const triceAbc_t triceAbc[] = {
    /* Trice type */  /* id, bitWidth, function pointer */
    /* TRiceC   */ { 14236u, 0u,  triceAbcCall_get_power_state },
    /* trice32C */ { 14235u, 32u, triceAbcCall_set_time },
};

const unsigned triceAbcElements = sizeof(triceAbc) / sizeof(triceAbc[0]);
```

Use wrappers so the table has one uniform function-pointer type. Do not store typed handlers directly in a `void (*)(void*, int)` table.

If no selected declarations match TIL ABC entries, generate a valid empty table representation. Do not generate a non-standard zero-length array.

Recommended empty representation:

```c
const triceAbc_t triceAbc[] = {
    { 0u, 0u, 0 }
};

const unsigned triceAbcElements = 0u;
```

### 11.3. Link behavior

The generator does not generate weak no-op handlers for ABC.

If a handler is selected in `<target>_abc.h` and emitted into `<target>_abc.c`, the application must provide an implementation. Missing implementations produce normal linker errors.

If the user wants no visible action, they can implement an empty function explicitly.

## 12. Generator matching and conflict rules

The generator builds the ABC table from:

```text
ABC entries in til.json intersect active declarations in <target>_abc.h
```

Rules:

| Case                                                                  | Result                                        |
|-----------------------------------------------------------------------|-----------------------------------------------|
| TIL ABC command exists and declaration exists with matching signature | Emit table entry.                             |
| Declaration exists but no TIL ABC command exists                      | Warning, no table entry.                      |
| TIL ABC command exists but no declaration exists                      | Silently ignore.                              |
| Same command name, same signature, multiple IDs                       | Emit multiple entries using the same wrapper. |
| Same command name, different payload signature                        | Error.                                        |
| Same ID, different command name or signature                          | Error.                                        |
| Invalid command name extracted from ABC format string                 | Error.                                        |

Deduplicate generated wrappers. Multiple IDs may point to the same wrapper if the command name and signature are identical.

Conflict errors shall be deterministic and include enough information to locate the conflicting IDs and command names.

## 13. ABC receive runtime

ABC receive support is enabled only when `TRICE_ABC_RECEIVE_SUPPORT == 1`.

The first receive runtime is direct and minimal:

1. Look up the received Trice ID in `triceAbc[]`.
2. Validate payload size against the selected handler bit width.
3. Set the current ABC stamp context.
4. Call the generated wrapper immediately.
5. Restore the previous stamp context.

It does not provide a queue, mailbox, scheduler, or automatic deferral.

If a transport receives data in a context where handler execution is not appropriate, the integration layer or the user handler must defer work explicitly by copying values and setting flags.

### 13.1. Receive API

Recommended API:

```c
typedef enum {
    TRICE_ABC_RX_IGNORED = 0,
    TRICE_ABC_RX_EXECUTED = 1,
    TRICE_ABC_RX_BAD_PAYLOAD = 2,
} TriceAbcRxResult;

TriceAbcRxResult TriceAbcOnReceive(
    uint16_t id,
    uint8_t stampBits,      // 0, 16, or 32
    uint32_t stamp,         // normalized ABC stamp value
    const void* payload,
    uint16_t payloadBytes
);
```

Expected behavior:

1. Search `triceAbc[]` linearly for `id`.
2. If no entry exists, return `TRICE_ABC_RX_IGNORED`.
3. Validate `stampBits` is 0, 16, or 32. Invalid values return `TRICE_ABC_RX_BAD_PAYLOAD`.
4. Validate `payloadBytes` against `bitWidth`.
5. If `payloadBytes > 0`, validate `payload != 0`.
6. Convert `payloadBytes` to element count.
7. Prepare a handler payload pointer.
8. Save the previous current ABC stamp context.
9. Set the current ABC stamp context.
10. Call the table wrapper.
11. Restore the previous current ABC stamp context.
12. Return `TRICE_ABC_RX_EXECUTED`.

### 13.2. Payload validation

| bitWidth | Valid payload           |
|----------|-------------------------|
| 0        | `payloadBytes == 0`     |
| 8        | Any payload byte count. |
| 16       | `payloadBytes % 2 == 0` |
| 32       | `payloadBytes % 4 == 0` |
| 64       | `payloadBytes % 8 == 0` |

If `bitWidth` is not 0, 8, 16, 32, or 64, return `TRICE_ABC_RX_BAD_PAYLOAD`.

### 13.3. Alignment and byte order

The direct receive runtime must not pass an unaligned raw byte pointer to a typed handler on targets that may trap on unaligned access.

Recommended first implementation:

- copy the incoming payload into a local scratch buffer that is aligned for 64-bit access,
- call the wrapper with a pointer into that scratch buffer,
- keep the pointer valid only during the handler call.

Define a receive-side maximum for this scratch buffer:

```c
#ifndef TRICE_ABC_MAX_PAYLOAD_BYTES
#define TRICE_ABC_MAX_PAYLOAD_BYTES TRICE_SINGLE_MAX_SIZE
#endif
```

If `payloadBytes > TRICE_ABC_MAX_PAYLOAD_BYTES`, return `TRICE_ABC_RX_BAD_PAYLOAD`.

The first implementation does not need to perform cross-endian conversion. It may assume that the integration layer supplies payload bytes in the order expected by the receiving target, or that participating targets use compatible Trice transfer order. Mixed-endian ABC networks are a follow-up topic. The Trice tool already supports an CLI switc `-triceEndianness`, so the incoming endiannes on othe receiving devices needs to be defined, but as one last step during the implementation. Network order is big endian, but most MCUs are little endian, so an explicit switch for the transfer endiannes can save a lot computring effort.

## 14. Current ABC stamp context

Provide helpers while a handler runs:

```c
uint8_t  TriceAbcCurrentStampBits(void);
uint16_t TriceAbcCurrentStamp16(void);
uint32_t TriceAbcCurrentStamp32(void);
```

During handler execution:

- `TriceAbcCurrentStampBits()` returns `0`, `16`, or `32`.
- `TriceAbcCurrentStamp16()` returns the low 16 bits of the current stamp.
- `TriceAbcCurrentStamp32()` returns the current normalized 32-bit stamp.

Outside handler execution, these functions return `0`.

The first implementation may use simple globals. `TriceAbcOnReceive()` should save and restore the previous context so nested calls do not permanently corrupt the outer context.

RTOS or thread-local stamp context is a follow-up topic.

## 15. Multiple devices and TIL management

The simplest robust workflow is one common `til.json` for all devices that exchange ABC messages.

If separate code bases produce separate TIL files, align or merge TIL data before generating target-specific ABC files.

Do not hand-merge generated `*_abc.c` files.

TIL merge/import tooling is not required for the first ABC implementation.

## 16. Security boundary

ABC receive allows incoming Trice messages to trigger locally compiled code. ABC is not an authorization system.

Applications using ABC on untrusted transports must provide their own trust boundary, for example by:

- authenticated or encrypted links,
- transport restrictions,
- ID filtering before `TriceAbcOnReceive()`,
- payload validation in every handler,
- disabling ABC receive support in builds that do not need it.

The Trice technique offers optional XTEA encryption out of the box, what could be sufficient for many cases.

## 17. Tests

Tests shall be added incrementally with each implementation step. Do not defer all tests until the end.

### 17.1. Parser and TIL tests

Required tests:

- Recognize all clean `triceC` macro families.
- Recognize all explicit-ID `_C` forms.
- Insert `iD(n)` into clean ABC macros.
- Preserve original macro type in the TIL.
- Extract command name after the last colon.
- Reject invalid C handler names.
- Treat everything before the last colon as tag/prefix metadata.
- Do not classify `F` entries as ABC.
- Do not interpret the ABC stamp argument as payload.

Suggested test fixtures:

```c
triceC("cmd:motor_stop");
TriceC("cmd:motor_stop", stamp16);
TRiceC("cmd:motor_stop", stamp32);
trice16C("cmd:set_pwm", p16, cnt);
Trice16C("cmd:set_pwm", stamp16, p16, cnt);
TRice16C("cmd:set_pwm", stamp32, p16, cnt);
TRIce32_C("cmd:set_time", p32, cnt);
TRICe32_C("cmd:set_time", stamp16, p32, cnt);
TRICE32_C("cmd:set_time", stamp32, p32, cnt);
TRIce32_C(iD(0), "cmd:set_time", p32, cnt);
TRICe32_C(iD(0), "cmd:set_time", stamp16, p32, cnt);
TRICE32_C(iD(0), "cmd:set_time", stamp32, p32, cnt);
```

Expected parser facts:

| Source                                                | commandName  | bitWidth | stampBits |
|-------------------------------------------------------|--------------|----------|-----------|
| `triceC("cmd:motor_stop")`                            | `motor_stop` | 0        | 0         |
| `TriceC("cmd:motor_stop", stamp16)`                   | `motor_stop` | 0        | 16        |
| `TRice32C("cmd:set_time", stamp32, p32, cnt)`         | `set_time`   | 32       | 32        |
| `TRIce32_C("cmd:set_time", p32, cnt)`                 | `set_time`   | 32       | 32        |
| `TRICe32_C("cmd:set_time", stamp16, p32, cnt)`        | `set_time`   | 32       | 32        |
| `TRICE32_C("cmd:set_time", stamp32, p32, cnt)`        | `set_time`   | 32       | 32        |
| `TRIce32_C(iD(0), "cmd:set_time", p32, cnt)`          | `set_time`   | 32       | 32        |
| `TRICe32_C(iD(0), "cmd:set_time", stamp16, p32, cnt)` | `set_time`   | 32       | 32        |
| `TRICE32_C(iD(0), "cmd:set_time", stamp32, p32, cnt)` | `set_time`   | 32       | 32        |

### 17.2. Send encoding tests

Required tests:

- No-stamp ABC output is binary-compatible with existing no-stamp counted-buffer format.
- `Trice*C(..., stamp16, ...)` writes the user-supplied 16-bit stamp, not `TriceStamp16`.
- `TRice*C(..., stamp32, ...)` writes the user-supplied 32-bit stamp, not `TriceStamp32`.
- Payload byte length equals element count times element width.
- `TRICE_ABC_TRANSMIT_SUPPORT == 0` excludes ABC send code.
- `TRICE_LEGACY_RPC_SUPPORT == 0` excludes legacy `triceF` target code.
- `TRICE_LEGACY_RPC_SUPPORT == 1` preserves legacy `triceF` behavior.

Prefer binary encoding assertions for these tests. Host-display text is useful but should not be the only proof that explicit stamps are written.

### 17.3. `-abc=<target>` generator tests

Required tests:

- Missing `<target>_abc.h` is generated from all ABC TIL entries.
- Generated `<target>_abc.h` uses include guards, not `#pragma once`.
- Existing `<target>_abc.h` is not overwritten.
- `<target>_abc.c` is regenerated every run.
- Read-only handling for `<target>_abc.c` is best effort and does not fail on unsupported platforms.
- Only active declarations are emitted into the table.
- Commented-out declarations are ignored.
- Declaration without matching TIL ID produces a warning and no table entry.
- TIL ABC entries absent from the selection header produce no warning.
- Duplicate wrappers are deduplicated.
- Signature conflicts are errors.
- Empty selected set generates valid C, not a zero-length array.

### 17.4. Receive runtime tests

These tests should be pure C target-library tests compiled and executed as host-native test binaries where possible. Do not add CGO execution to the Trice tool for ABC testing.

Required tests:

- Unknown ID returns `TRICE_ABC_RX_IGNORED`.
- Known zero-payload ID calls the correct `void name(void)` handler.
- Known payload ID passes typed payload pointer and element count.
- Bad payload size returns `TRICE_ABC_RX_BAD_PAYLOAD`.
- Payload larger than `TRICE_ABC_MAX_PAYLOAD_BYTES` returns `TRICE_ABC_RX_BAD_PAYLOAD`.
- `payload == 0` with non-zero payload size returns `TRICE_ABC_RX_BAD_PAYLOAD`.
- Current stamp helpers report the stamp during handler execution and zero outside.
- Nested `TriceAbcOnReceive()` calls restore the outer stamp context after the nested call returns.
- Empty `triceAbc[]` table with `triceAbcElements == 0` ignores all IDs.

Suggested test pattern:

1. Provide a tiny hand-written or generated `test_abc.c` table.
2. Provide handlers that increment counters and copy received values into global test variables.
3. Call `TriceAbcOnReceive()` with known IDs and payload arrays.
4. Assert the counters, values, element counts, return codes, and current stamp behavior.

Example test handler:

```c
static int setPwmCalls;
static int setPwmCount;
static int16_t setPwmValue0;
static uint32_t setPwmStamp;

void set_pwm(int16_t* p, int cnt) {
    setPwmCalls++;
    setPwmCount = cnt;
    setPwmValue0 = p[0];
    setPwmStamp = TriceAbcCurrentStamp32();
}
```

### 17.5. Target code transmit examples in `triceCheck.c`

Extend target transmit checks with ABC cases similar to the existing buffer/legacy-F cases. Keep these tests behind `TRICE_ABC_TRANSMIT_SUPPORT == 1`.

Use fixed stamps so output is deterministic:

```c
#if TRICE_ABC_TRANSMIT_SUPPORT == 1
uint16_t stamp16 = 0xeb61u;
uint32_t stamp32 = 0xc0de3020u;
#endif
```

Suggested representative cases for each payload width:

```c
#if TRICE_ABC_TRANSMIT_SUPPORT == 1
break; case __LINE__: TRICE8_C(iD(0), "cmd:Abc8Stamp32", stamp32, b8, sizeof(b8) / sizeof(int8_t)); //exp: ...
break; case __LINE__: TRICe8_C(iD(0), "cmd:Abc8Stamp16", stamp16, b8, sizeof(b8) / sizeof(int8_t));
break; case __LINE__: TRIce8_C(iD(0), "cmd:Abc8NoStamp", b8, sizeof(b8) / sizeof(int8_t));
break; case __LINE__: TRice8C("cmd:Abc8FnStamp32", stamp32, b8, sizeof(b8) / sizeof(int8_t));
break; case __LINE__: Trice8C("cmd:Abc8FnStamp16", stamp16, b8, sizeof(b8) / sizeof(int8_t));
break; case __LINE__: trice8C("cmd:Abc8FnNoStamp", b8, sizeof(b8) / sizeof(int8_t));
#endif
```

Add analogous 16-bit, 32-bit, and 64-bit cases.

For host-display expected text, initial tests may use the current stamp label if the host still prints `time:`. Once ABC display is refined, update expected strings to require `stamp:` for ABC messages.

### 17.6. Legacy tests

Required tests:

- Existing `triceF` macro behavior is unchanged when `TRICE_LEGACY_RPC_SUPPORT == 1`.
- Legacy `triceF` target code is absent or unusable when `TRICE_LEGACY_RPC_SUPPORT == 0`.
- Existing `trice generate -rpcH -rpcC` behavior is unchanged.
- ABC generation ignores `F` entries.
- Documentation/help marks the legacy path as deprecated, but no compile-time deprecation warning is emitted merely because `TRICE_LEGACY_RPC_SUPPORT == 1`.

### 17.7. Example build tests

Add one small buildable example or test fixture that exercises the complete flow:

```bash
trice insert -src <example-src> -i example_til.json
trice generate -i example_til.json -abc=deviceA
cc ... deviceA_abc.c deviceA_handlers.c abc_receive_test.c ...
./abc_receive_test
```

This test should prove that:

- `deviceA_abc.h` is generated on first run,
- pruning `deviceA_abc.h` changes the generated receive table,
- `deviceA_abc.c` compiles,
- selected handlers link and execute,
- missing selected handlers produce linker errors in a negative test if practical.

Do not require actual embedded hardware for this test. You can create a subfolder inside ./examples or ./_test for that.

## 18. Target-code examples

### 18.1. Example: request power state from a swarm

A sender broadcasts a request. The stamp is application-defined; here it contains a 16-bit device ID and a 16-bit sequence counter.

```c
#include "trice.h"

#define DEVICE_ID 0x1234u

static uint16_t abcSeq;

void AskPowerState(void) {
    uint32_t stamp = ((uint32_t)DEVICE_ID << 16) | (uint32_t)abcSeq++;
    TRiceC("cmd:get_power_state", stamp);
}
```

A receiver selects `get_power_state` in `deviceA_abc.h`:

```c
#ifndef DEVICEA_ABC_H_
#define DEVICEA_ABC_H_

#include <stdint.h>

void get_power_state(void);

#endif /* DEVICEA_ABC_H_ */
```

The receiver implements the handler:

```c
#include "trice.h"
#include "deviceA_abc.h"

static uint16_t BoardPowerMillivolt(void) {
    return 3710u;
}

void get_power_state(void) {
    uint32_t stamp = TriceAbcCurrentStamp32();
    uint16_t mv = BoardPowerMillivolt();
    stamp =  ((uint32_t)DEVICE_A_ID << 16) | (uint16_t)stamp; // Use sequence number and path own device ID.
    TRice16C("rsp:power_state", stamp, &mv, 1);
}
```

All devices that select `power_state` may process the response:

```c
#include "trice.h"
#include "collector_abc.h"

#define MAX_DEVICES 16

static uint16_t powerMv[MAX_DEVICES];
static unsigned powerCount;

void power_state(int16_t* p, int cnt) {
    if (cnt != 1 || powerCount >= MAX_DEVICES) {
        return;
    }

    uint32_t stamp = TriceAbcCurrentStamp32();
    uint16_t sourceDevice = (uint16_t)(stamp >> 16);

    (void)sourceDevice; /* Use as needed. */
    powerMv[powerCount++] = (uint16_t)p[0];
}
```

ABC itself does not wait for responses and does not decide when enough responses arrived. The application may stop collecting after two responses, after a timeout, or by any other rule.

### 18.2. Example: defer work in the user handler

ABC receive calls handlers directly in the first implementation. If the receive integration runs in a context where real work is not allowed, the handler should only copy data and set a flag.

```c
#include <stdint.h>
#include "trice.h"
#include "motor_abc.h"

static volatile int motorStepPending;
static int8_t motorStep[4];

void motor_step(int8_t* p, int cnt) {
    if (cnt != 4) {
        return;
    }

    motorStep[0] = p[0];
    motorStep[1] = p[1];
    motorStep[2] = p[2];
    motorStep[3] = p[3];
    motorStepPending = 1;
}

void MainLoop(void) {
    for (;;) {
        if (motorStepPending) {
            motorStepPending = 0;
            MotorApplyStep(motorStep[0], motorStep[1], motorStep[2], motorStep[3]);
        }

        TriceTransfer();
        ApplicationStep();
    }
}
```

This keeps ABC small and avoids adding a queue or scheduler to the Trice core.

### 18.3. Example: host-native receive test

This example shows the intended unit-test shape without CGO.

`test_abc.h`:

```c
#ifndef TEST_ABC_H_
#define TEST_ABC_H_

#include <stdint.h>

void set_pwm(int16_t* p, int cnt);
void ping(void);

#endif /* TEST_ABC_H_ */
```

`test_abc.c` generated shape:

```c
#include "test_abc.h"
#include "trice.h"

static void triceAbcCall_set_pwm(void* p, int cnt) {
    set_pwm((int16_t*)p, cnt);
}

static void triceAbcCall_ping(void* p, int cnt) {
    TRICE_UNUSED(p);
    TRICE_UNUSED(cnt);
    ping();
}

const triceAbc_t triceAbc[] = {
    { 1001u, 16u, triceAbcCall_set_pwm },
    { 1002u, 0u,  triceAbcCall_ping },
};

const unsigned triceAbcElements = sizeof(triceAbc) / sizeof(triceAbc[0]);
```

`abc_receive_test.c`:

```c
#include <stdint.h>
#include <stdio.h>
#include "trice.h"
#include "test_abc.h"

static int setPwmCalls;
static int pingCalls;
static int16_t pwm0;
static uint32_t seenStamp;

void set_pwm(int16_t* p, int cnt) {
    if (cnt == 2) {
        setPwmCalls++;
        pwm0 = p[0];
        seenStamp = TriceAbcCurrentStamp32();
    }
}

void ping(void) {
    pingCalls++;
}

int main(void) {
    int16_t pwm[2] = { 123, 456 };

    if (TriceAbcOnReceive(9999u, 0u, 0u, 0, 0u) != TRICE_ABC_RX_IGNORED) {
        return 1;
    }

    if (TriceAbcOnReceive(1001u, 32u, 0xc0de3020u, pwm, sizeof(pwm)) != TRICE_ABC_RX_EXECUTED) {
        return 2;
    }

    if (setPwmCalls != 1 || pwm0 != 123 || seenStamp != 0xc0de3020u) {
        return 3;
    }

    if (TriceAbcOnReceive(1002u, 0u, 0u, 0, 0u) != TRICE_ABC_RX_EXECUTED) {
        return 4;
    }

    if (pingCalls != 1) {
        return 5;
    }

    if (TriceAbcCurrentStampBits() != 0u || TriceAbcCurrentStamp32() != 0u) {
        return 6;
    }

    return 0;
}
```

## 19. Recommended implementation order

Implement in small steps and add tests after each step.

1. Add documentation/help deprecation wording for legacy `triceF` and `-rpcH` / `-rpcC`.
2. Add `TRICE_ABC_TRANSMIT_SUPPORT`, `TRICE_ABC_RECEIVE_SUPPORT`, and `TRICE_LEGACY_RPC_SUPPORT` defaults.
3. Gate existing legacy `triceF` target code behind `TRICE_LEGACY_RPC_SUPPORT` without changing legacy generator behavior.
4. Add parser recognition for ABC macro families and `_C` explicit forms.
5. Add ABC TIL classification, bit-width detection, stamp-width detection, and command-name extraction.
6. Add `trice insert` / `trice clean` behavior for ABC macros using `iD(n)`.
7. Implement `trice generate -abc=<target>` first-run selection-header generation.
8. Implement existing-header parsing and missing-ID warnings.
9. Implement `<target>_abc.c` generation with wrappers, table, deduplication, and conflict errors.
10. Add no-stamp ABC send macros using existing counted-buffer logic where possible.
11. Add stamped ABC send macros using `TRICE_C_STAMP16` and `TRICE_C_STAMP32`.
12. Add direct ABC receive runtime and current-stamp helpers.
13. Add host-native receive runtime tests.
14. Add `triceCheck.c` target transmit tests.
15. Add a small end-to-end generator/build example.
16. Add host display refinement from `time:` to `stamp:` for ABC entries if desired.

## 20. Follow-up items

The following items are not required for the first implementation:

- TIL merge/import tooling.
- Queued or deferred ABC receive helpers.
- RTOS/thread-local current stamp context.
- Optimized lookup beyond linear search.
- Mixed-endian ABC receive normalization.
- Support for `const intN_t*`, `uintN_t*`, typedef-based declarations, or custom parameter names in `<target>_abc.h`.
- Default-width ABC payload form for `triceC`.
- Removal of legacy `triceF`, `-rpcH`, or `-rpcC`.

## 21. Non-negotiable constraints

- Existing `triceF` macros remain functionally unchanged for the ABC implementation.
- Existing `trice generate -rpcH -rpcC` behavior remains unchanged initially.
- Legacy `triceF` target code is gated behind `TRICE_LEGACY_RPC_SUPPORT`, default `0`.
- Use `TRICE_LEGACY_RPC_SUPPORT` as the single legacy switch name; do not also introduce `TRICE_RPC`.
- ABC stamped macros must not rely on `ID(n)` or `Id(n)` for user-supplied stamps.
- Stamp-only helper macros are named `TRICE_C_STAMP32` and `TRICE_C_STAMP16`.
- The primary ABC generator workflow is `trice generate -abc=<target>`.
- `<target>_abc.h` is generated only when missing and then user-editable.
- ABC headers use classic include guards, not `#pragma once`.
- `<target>_abc.c` is generated every run and should be treated as read-only generated code.
- ABC generation selects only active declarations from `<target>_abc.h`.
- A `<target>_abc.h` declaration without matching TIL ABC ID produces a warning and no table entry.
- ABC generation does not select legacy `F` entries by default.
- ABC does not generate weak handlers.
- ABC receive runtime is direct in the first implementation; no queue or mailbox is part of the ABC core.
- ABC is a primitive, not a full RPC or distributed-systems framework.
