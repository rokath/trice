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

This specification updates the earlier ABC draft to the simplified `trice generate -abc=<target>` workflow and keeps the existing `triceF` / `-rpcH` / `-rpcC` legacy path unchanged.

## 1. Goal

Implement an optional Trice ABC feature that allows selected Trice messages to be interpreted by receiving devices as asynchronous broadcast commands.

A Trice ABC sender emits a normal Trice message with identical binary format:

- a normal 14-bit Trice ID,
- a normal 2-bit Trice selector,
- optional ABC stamp data in the existing Trice stamp field,
- normal 16-bit Trice count field (NC) with default cycle counter,
- optional payload data.

There is no package difference between Trice and TriceABC messages, despite the stamp contence and the ID interpretation. A receiver may map the ID to a locally compiled handler selected for this target. The sender does not wait and does not know which devices will react.

The ABC core is intentionally small. It shall **not** implement:

- reliable delivery,
- retries,
- acknowledgements,
- wait-for-response logic,
- quorum logic,
- routing,
- discovery,
- authentication or authorization,
- consensus,
- persistent command queues.

Applications may implement these policies above ABC.

## 2. Existing legacy path

The current repository already contains an experimental remote-function-call style path based on `triceF` and `rpc` naming.

Existing behavior to preserve:

- `triceF`, `TriceF`, `TRiceF`, `TRICE*_F`, and their bit-width variants remain unchanged initially.
- `trice generate -rpcH -rpcC` remains unchanged initially.
- Existing generated RPC filenames and symbols remain unchanged initially.
- Existing users can continue using the legacy path when explicitly enabled.

Required changes around the legacy path:

- Documentation marks `triceF` and `-rpcH` / `-rpcC` as deprecated.
- ABC generation does not select `F` macros.
- `triceF` target code shall get a compile-time switch, recommended name (in _triceDefaultConfig.h_):

```c
#ifndef TRICE_RPC
#define TRICE_RPC 0
#endif
```

When `TRICE_RPC == 0`, legacy `triceF` target code parts are not compiled. Existing users who still use `triceF` set in project specific _triceConfig.h_:

```c
#define TRICE_RPC 1
```

This switch must not change the host-side `-rpcH` / `-rpcC` generator behavior.

## 3. Terminology

### ABC command

A Trice message emitted through a `triceC` macro family and intended to be interpretable as a command by receivers.

### ABC Trice ID

The normal 14-bit Trice ID associated with an ABC command. It is the primary receive-table key.

### ABC command name

The C handler name extracted from the ABC format string. The generator uses the part after the last colon:

```text
cmd:motor_stop          -> motor_stop
cmd:deviceA:motor_step  -> motor_step
a:b:c:d:e:f             -> f
```

The command name must be a valid C identifier.

### ABC tag / prefix

Everything before the last colon is display/filter metadata. It may be used for normal Trice tag behavior such as coloring, ID ranges, `-pick`, or `-ban`. It is not ABC addressing and must not define ABC semantics.

### ABC stamp

A user-provided correlation value transported in the existing Trice stamp field for ABC messages. ABC stamps are not necessarily timestamps.

### ABC handler

A locally compiled application function selected by `<target>_abc.h` and referenced through the generated `<target>_abc.c` table.

## 4. Configuration switches

Add defaults to `triceDefaultConfig.h` or the equivalent default configuration layer.

```c
#ifndef TRICE_ABC_TRANSMIT_SUPPORT
#define TRICE_ABC_TRANSMIT_SUPPORT 0
#endif

#ifndef TRICE_ABC_RECEIVE_SUPPORT
#define TRICE_ABC_RECEIVE_SUPPORT 0
#endif

#ifndef TRICE_RPC
#define TRICE_RPC 0 // depreciated, only for legacy projects
#endif
```

Meaning:

- `TRICE_ABC_TRANSMIT_SUPPORT == 1`: enables ABC send macros.
- `TRICE_ABC_RECEIVE_SUPPORT == 1`: enables ABC receive runtime declarations and helper functions.
- `TRICE_RPC == 1`: enables deprecated legacy `triceF` target code.

Do not use a single `TRICE_ABC_SUPPORT` switch unless it is only a convenience alias. Transmit and receive support are independent.

## 5. Public ABC send macros

### 5.1 Clean user source forms

No payload:

```c
triceC("cmd:motor_stop");
TriceC(stamp16, "cmd:motor_stop");
TRiceC(stamp32, "cmd:motor_stop");
```

8-bit payload:

```c
trice8C("cmd:set_leds", p8, cnt);
Trice8C(stamp16, "cmd:set_leds", p8, cnt);
TRice8C(stamp32, "cmd:set_leds", p8, cnt);
```

16-bit payload:

```c
trice16C("cmd:set_pwm", p16, cnt);
Trice16C(stamp16, "cmd:set_pwm", p16, cnt);
TRice16C(stamp32, "cmd:set_pwm", p16, cnt);
```

32-bit payload:

```c
trice32C("cmd:set_time", p32, cnt);
Trice32C(stamp16, "cmd:set_time", p32, cnt);
TRice32C(stamp32, "cmd:set_time", p32, cnt);
```

64-bit payload:

```c
trice64C("cmd:set_epoch", p64, cnt);
Trice64C(stamp16, "cmd:set_epoch", p64, cnt);
TRice64C(stamp32, "cmd:set_epoch", p64, cnt);
```

The default-width `triceC` payload form, if implemented, follows the same default-width mechanism as other Trice default-width macros. User documentation should recommend width-specific forms when payload exists.

### 5.2 Inserted source forms

`trice insert` adds a raw Trice ID argument to ABC macro calls. For ABC macros, the inserted ID argument must not be `ID(n)` or `Id(n)`, because those macros inject normal Trice timestamp sources.

Recommended inserted form:

```c
TRice32C(iD(12345), stamp32, "cmd:set_time", p32, cnt);
Trice16C(iD(12346), stamp16, "cmd:set_pwm", p16, cnt);
triceC(iD(12347), "cmd:motor_stop");
```

`iD(n)` is preferred over a bare numeric literal for consistency with existing Trice parser/helper conventions.

### 5.3 Explicit-ID uppercase forms

If explicit-ID `_C` forms are implemented, they must also use user-supplied ABC stamps. They require `ID(n)` or `Id(n)` for stamped ABC messages and `id(n)` for unstamped ABC messages.

This spelling may be finalized during implementation review. It is acceptable to implement lower/mixed-case ABC forms first and add explicit-ID `_C` forms later. Look at the `_F` forms as an example.

## 6. Target-side ABC stamp writing

Normal Trice `ID(n)` and `Id(n)` write `TriceStamp32` and `TriceStamp16`. ABC stamped messages must instead write the user-supplied ABC stamp.

Add stamp-only helpers with collision-safe names:

```c
#define TRICE_C_STAMP32(stamp32)  /* write user supplied 32-bit ABC stamp */
#define TRICE_C_STAMP16(stamp16)  /* write user supplied 16-bit ABC stamp */
```

These helpers have no ID semantics.

Add ABC ID/header helpers as needed:

```c
#define TRICE_C_ID32(n)  /* write ID/header for following 32-bit ABC stamp */
#define TRICE_C_ID16(n)  /* write ID/header for following 16-bit ABC stamp */
#define TRICE_C_ID0(n)   /* write ID/header without stamp */
```

The stamped ABC send path must write binary-compatible Trice records, but use the explicit stamp argument instead of `TriceStamp16` or `TriceStamp32`.

No-stamp ABC messages may reuse existing no-stamp counted-buffer code if the binary output is identical.

## 7. Host-side parser and TIL behavior

Recognize the new `C` macro family:

```text
triceC, TriceC, TRiceC
trice8C, Trice8C, TRice8C
trice16C, Trice16C, TRice16C
trice32C, Trice32C, TRice32C
trice64C, Trice64C, TRice64C
```

The TIL entry must preserve the original macro type string so ABC entries can be distinguished from normal Trices, buffer Trices, and deprecated `F` entries.

Do not classify `F` entries as ABC.

Bit width rules:

| Macro family | bitWidth |
| --- | --- |
| `triceC` no payload | 0 |
| default-width `triceC` with payload | configured default width |
| `trice8C` | 8 |
| `trice16C` | 16 |
| `trice32C` | 32 |
| `trice64C` | 64 |

Payload count is runtime-sized, analogous to existing buffer-style `B` / `F` behavior.

Display behavior:

- ABC payload display can use the current command/function style, for example `cmd:set_time(00000001)`.
- Stamped ABC output should use `stamp:` rather than `time:` unless the user explicitly configures otherwise.
- Existing tag behavior remains unchanged.

## 8. Generator CLI

Add exactly one user-facing ABC generation option:

```bash
trice generate -i til.json -abc=<target>
```

Example:

```bash
trice generate -i til.json -abc=deviceA
```

For `<target> = deviceA`, the generator uses:

```text
deviceA_abc.h
deviceA_abc.c
```

Do not add `-abcH` / `-abcC` as the primary workflow. If they exist from earlier drafts or experiments, keep them internal or remove them before release.

## 9. `<target>_abc.h` selection file

`<target>_abc.h` is a user-editable selection file.

### 9.1 First run

If the file does not exist, generate it from all ABC commands found in `til.json`:

```c
//! \file deviceA_abc.h
//! Trice ABC selection file for target deviceA.
//! Generated once; edit this file to select received ABC commands.

#ifndef DEVICEA_ABC_H_
#define DEVICEA_ABC_H_

#include <stdint.h>

void motor_stop(void);
void get_power_state(void);
void set_time(int32_t* p, int cnt);
void set_pwm(int16_t* p, int cnt);

#endif /* DEVICEA_ABC_H_ */
```

The file must not contain weak definitions. It contains declarations only.

The generated selection header shall use classic include guards, not `#pragma once`, to keep ABC headers independent of non-standard compiler extensions. The guard name is derived from `<target>_abc.h`, converted to uppercase C identifier form, for example `DEVICEA_ABC_H_`.

### 9.2 Later runs

If `<target>_abc.h` exists, use it as input and do not overwrite it.

Active, non-commented declarations define the target-local receive set. Deleted or commented declarations are ignored.

New ABC commands added to `til.json` later are not automatically activated for an existing target. The user may add the corresponding declaration manually or delete/regenerate the selection header.

### 9.3 Declaration syntax

Initial parser support shall handle these canonical forms:

```c
void name(void);
void name(int8_t* p, int cnt);
void name(int16_t* p, int cnt);
void name(int32_t* p, int cnt);
void name(int64_t* p, int cnt);
```

Whitespace differences should be tolerated. Additional forms may be supported later.

Canonical generated declarations use `intN_t*`. The signedness is not semantically significant in the Trice binary encoding; applications can cast or copy as needed.

### 9.4 Missing-ID warning

If `<target>_abc.h` declares a handler for which no matching ABC command exists in `til.json`, print exactly a warning and do not emit a table entry for it.

Example:

```text
warning: ABC handler 'calibrate_sensor' declared in deviceA_abc.h but no matching ABC command exists in til.json
```

No warning is required for ABC commands in `til.json` that are not declared in `<target>_abc.h`. That is the intended selection mechanism.

## 10. `<target>_abc.c` generated file

`<target>_abc.c` is generated code and must be regenerated on every `trice generate -abc=<target>` run.

Behavior:

- overwrite the file every run,
- if it is read-only, make it writable first if possible,
- write the new file,
- set it read-only afterwards if the platform allows it.

This read-only behavior is best effort and must not make generation fail on platforms where file attributes are unavailable.

### 10.1 Runtime types

Define shared ABC runtime types in the Trice target library, preferably via `trice.h` when receive support is enabled:

```c
typedef void (*triceAbcHandler_t)(void* payload, int count);

typedef struct {
    uint16_t id;
    uint8_t bitWidth; // 0, 8, 16, 32, 64
    triceAbcHandler_t fn;
} triceAbc_t;

extern const triceAbc_t triceAbc[];
extern const unsigned triceAbcElements;
```

`<target>_abc.h` should not be the only owner of these types, because it is user-editable.

### 10.2 Generated source shape

Example:

```c
//! \file deviceA_abc.c
//! Trice generated ABC code - do not edit!

#include "deviceA_abc.h"
#include "trice.h"

static void triceAbcCall_get_power_state(void* p, int cnt) {
    (void)p;
    (void)cnt;
    get_power_state();
}

static void triceAbcCall_set_time(void* p, int cnt) {
    set_time((int32_t*)p, cnt);
}

const triceAbc_t triceAbc[] = {
    /* Trice type */ /* id, bitWidth, function pointer */
    /*   TriceC   */ { 14236,  0, triceAbcCall_get_power_state },
    /*  trice32C  */ { 14235, 32, triceAbcCall_set_time },
};

const unsigned triceAbcElements = sizeof(triceAbc) / sizeof(triceAbc[0]);
```

Use wrappers so the table has one uniform function-pointer type. Do not store typed handlers directly in a `void (*)(void*, int)` table.

### 10.3 Link behavior

The generator does not generate weak no-op handlers for ABC.

If a handler is selected in `<target>_abc.h` and emitted into `<target>_abc.c`, the application must provide an implementation. Missing implementations produce normal linker errors. If the user wants no visible action, they can implement an empty function explicitly.

## 11. Generator matching and conflict rules

The generator builds the ABC table from:

```text
ABC entries in til.json  ∩  active declarations in <target>_abc.h
```

Rules:

| Case | Result |
| --- | --- |
| TIL ABC command exists and declaration exists with matching signature | emit table entry |
| Declaration exists but no TIL ABC command exists | warning, no table entry |
| TIL ABC command exists but no declaration exists | silently ignore |
| Same command name, same signature, multiple IDs | emit multiple entries using the same wrapper |
| Same command name, different payload signature | error |
| Same ID, different command name or signature | error |
| Invalid command name extracted from ABC format string | error |

Deduplicate generated wrappers. Multiple IDs may point to the same wrapper if the command name and signature are identical.

## 12. ABC receive runtime

ABC receive support is enabled only when `TRICE_ABC_RECEIVE_SUPPORT == 1`.

The default receive runtime should be simple and direct: it looks up the received ID and calls the generated wrapper. It does not provide a queue, mailbox, scheduler, or automatic deferral.

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
    void* payload,
    uint16_t payloadBytes
);
```

Expected behavior:

1. Search `triceAbc[]` linearly for `id`.
2. If no entry exists, return `TRICE_ABC_RX_IGNORED`.
3. Validate payload length against `bitWidth`.
4. Convert `payloadBytes` to element count.
5. Set the current ABC stamp context.
6. Call the table wrapper.
7. Clear the current ABC stamp context.
8. Return `TRICE_ABC_RX_EXECUTED`.

Payload validation:

| bitWidth | valid payload |
| --- | --- |
| 0 | `payloadBytes == 0` |
| 8 | any payload byte count |
| 16 | `payloadBytes % 2 == 0` |
| 32 | `payloadBytes % 4 == 0` |
| 64 | `payloadBytes % 8 == 0` |

If a transport receives data in a context where handler execution is not appropriate, the integration layer or the user handler must defer work explicitly by copying values and setting flags.

## 13. Current ABC stamp context

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

Outside handler execution, these functions should return `0`.

Initial implementation may use simple globals. RTOS/thread-local behavior is a later extension if required.

## 14. Multiple devices and TIL management

The simplest robust workflow is one common `til.json` for all devices that exchange ABC messages.

If separate code bases produce separate TIL files, align or merge TIL data before generating target-specific ABC files. Do not hand-merge generated `*_abc.c` files.

TIL merge/import tooling is a follow-up feature and not required for the first ABC implementation.

## 15. Security boundary

ABC receive allows incoming Trice messages to trigger locally compiled code. ABC is not an authorization system.

Applications using ABC on untrusted transports must provide their own trust boundary, for example by authenticated/encrypted links, transport restrictions, ID filtering, and payload validation in handlers.

## 16. Tests

### 16.1 Parser and TIL tests

- Recognize all clean `triceC` macro families.
- Insert `iD(n)` into ABC macros.
- Preserve original macro type in the TIL.
- Extract command name after the last colon.
- Reject invalid C handler names.
- Do not classify `F` entries as ABC.

### 16.2 Send encoding tests

- No-stamp ABC output is binary-compatible with the existing no-stamp counted-buffer format.
- Stamped ABC output writes the user-supplied stamp, not `TriceStamp16` or `TriceStamp32`.
- Payload byte length equals element count times element width.
- `TRICE_ABC_TRANSMIT_SUPPORT == 0` disables ABC send code.

### 16.3 `-abc=<target>` generator tests

- Missing `<target>_abc.h` is generated from all ABC TIL entries.
- Existing `<target>_abc.h` is not overwritten.
- `<target>_abc.c` is regenerated every run.
- Read-only handling for `<target>_abc.c` is best effort.
- Only active declarations are emitted into the table.
- Commented-out declarations are ignored.
- Declaration without matching TIL ID produces a warning and no table entry.
- TIL ABC entries absent from the selection header produce no warning.
- Duplicate wrappers are deduplicated.
- Signature conflicts are errors.

### 16.4 Receive runtime tests

- Unknown ID is ignored.
- Known ID calls the correct wrapper.
- Zero-payload command calls `void name(void)` wrapper.
- Payload command passes typed payload pointer and element count.
- Bad payload size returns `TRICE_ABC_RX_BAD_PAYLOAD`.
- Current stamp helpers report the stamp during handler execution and zero outside.

### 16.5 Legacy tests

- Existing `triceF` macro behavior is unchanged when `TRICE_RPC == 1`.
- `triceF` target code is disabled when `TRICE_RPC == 0`.
- Existing `trice generate -rpcH -rpcC` behavior is unchanged.
- ABC generation ignores `F` entries.

## 17. Recommended implementation order

1. Add documentation updates and deprecation wording.
2. Add `TRICE_ABC_TRANSMIT_SUPPORT`, `TRICE_ABC_RECEIVE_SUPPORT`, and `TRICE_RPC` defaults.
3. Add parser recognition for `C` macro families.
4. Add ABC TIL classification and command-name extraction.
5. Add no-stamp ABC send macros using existing counted-buffer logic where possible.
6. Add stamped ABC send macros using `TRICE_C_STAMP16` and `TRICE_C_STAMP32`.
7. Implement `trice generate -abc=<target>`.
8. Generate `<target>_abc.h` only when missing.
9. Always generate `<target>_abc.c` with wrappers and table.
10. Add direct ABC receive runtime and current-stamp helpers.
11. Add tests.
12. Add optional TIL merge/import support later if needed.

## 18. Open decisions

1. Exact spelling and scope of explicit-ID uppercase `_C` forms.
2. Exact host display label for ABC stamps; recommended default is `stamp:`.
3. Whether the declaration parser should accept `uintN_t*` and `const intN_t*` in addition to canonical `intN_t*`.
4. Whether `TRICE_RPC` should emit compile-time deprecation warnings when enabled.
5. Whether future receive integrations should offer optional queued/deferred processing helpers.

## 19. Non-negotiable constraints

- Existing `triceF` macros remain unchanged for the ABC implementation.
- Existing `trice generate -rpcH -rpcC` behavior remains unchanged initially.
- Legacy `triceF` target code is gated behind `TRICE_RPC`, default `0`.
- ABC stamped macros must not rely on `ID(n)` or `Id(n)` for user-supplied stamps.
- Stamp-only helper names are `TRICE_C_STAMP32` and `TRICE_C_STAMP16`.
- Primary ABC generator workflow is `trice generate -abc=<target>`.
- `<target>_abc.h` is generated only when missing and then user-editable.
- ABC headers use classic include guards, not `#pragma once`.
- `<target>_abc.c` is generated every run and should be treated as read-only generated code.
- ABC generation selects only active declarations from `<target>_abc.h`.
- A declaration without matching TIL ABC ID produces a warning and no table entry.
- ABC is a primitive, not a full RPC or distributed-systems framework.
