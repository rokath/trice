# Trice ABC Implementation Specification

Status: implementation draft for Codex.

Feature name:

```text
Trice ABC - Asynchronous Broadcast Commands
```

Macro suffix:

```text
C  // Command
```

This specification describes the target-side C API, generated files, tool changes, migration from the existing `triceF`/RPC experiment, and open design points.

## 1. Scope

Implement an optional ABC feature that allows selected Trice messages to be interpreted by receiving devices as asynchronous broadcast commands.

The implementation shall provide:

1. New `triceC` macro families for command emission.
2. Optional explicit 16-bit and 32-bit ABC stamps supplied by the user.
3. New `trice generate -abcH -abcC` output files.
4. Generated weak handler declarations and definitions.
5. A generated ABC ID-to-handler table.
6. Optional target-side ABC receive support using a one-deep pending mailbox per ABC entry.
7. A processing routine that calls handlers outside the receive/decoder context.
8. Compatibility with existing Trice binary framing and TIL-based decoding.

The implementation shall not turn Trice into a complete RPC or distributed systems framework.

Out of scope for the ABC core:

- reliable delivery,
- retries,
- acknowledgements,
- wait-for-response logic,
- quorum logic,
- routing,
- discovery,
- authentication,
- authorization,
- consensus,
- persistent command queues.

Applications may implement these policies above ABC.

## 2. Existing state to migrate from

The current repository already contains an experimental remote-function-call style path based on `triceF` and `rpc` naming.

Relevant current concepts:

- The user manual documents `TRICE8_F`, `TRICE16_F`, `TRICE32_F`, `TRICE64_F` and lower/mixed-case `triceF` variants as remote function call syntax support.
- The manual documents `trice generate -rpcH -rpcC` and generated `tilRpc.h` / `tilRpc.c` style files.
- Current generated RPC files use names such as `triceRpcHandler_t`, `triceRpc_t`, `triceRpc[]`, and weak handler functions.
- `TRICE32_F` is currently a wrapper/alias over the buffer-style path (`TRICE32_B`), which ultimately uses the counted buffer transfer mechanism.
- `TRICE_N` writes an ID, count, cycle/count header, and payload buffer.
- Normal Trice ID helper macros in `trice.h` are:
  - `ID(n)`: writes a 14-bit Trice ID plus a 32-bit normal Trice stamp from `TriceStamp32`.
  - `Id(n)`: writes a 14-bit Trice ID plus a 16-bit normal Trice stamp from `TriceStamp16`.
  - `id(n)`: writes a 14-bit Trice ID without stamp.
  - `iD(n)`: parser/helper form returning the raw ID value.

Required migration principle:

```text
Do not modify the existing triceF macros for the ABC implementation.
```

Instead:

- keep `triceF` behavior unchanged,
- mark `triceF` as deprecated in documentation,
- add new `triceC` macros,
- add new `-abcH` and `-abcC` generator options,
- do not include `F` macros in ABC generation by default.

## 3. Terminology

### ABC command

A Trice message emitted through a `triceC` macro family and intended to be interpretable as a command by receivers.

### ABC Trice ID

The normal 14-bit Trice ID associated with an ABC command. This is the primary key used by receivers.

### ABC command name

The C identifier extracted from the ABC format string after removing an optional tag prefix. Example:

```text
cmd:motor_stop -> motor_stop
```

The command name is used by the generator to create handler prototypes and weak definitions.

### ABC tag

Optional prefix before `:`. Example:

```text
cmd:motor_stop
```

`cmd` is a tag. Tags are display and filtering metadata only. They must not be used as the source of ABC semantics.

### ABC stamp

A user-provided correlation value transported in the normal Trice stamp field for ABC messages.

ABC stamps are not necessarily timestamps.

### ABC handler

A local application function generated as weak no-op by default and optionally implemented strongly by the user.

### ABC mailbox

The one-deep pending state slot for a received ABC table entry.

## 4. Naming decisions

### Public documentation name

Use:

```text
Trice ABC - Asynchronous Broadcast Commands
```

### Public macro suffix

Use:

```text
C
```

`C` means Command.

### Internal/generated prefix

Use `Abc` / `ABC` in generated filenames, types, comments, and CLI options.

Recommended names:

```text
-abcH
-abcC
<base>Abc.h
<base>Abc.c
triceAbcHandler_t
triceAbc_t
triceAbc[]
triceAbcElements
triceAbcState[]
TriceAbcOnReceive
TriceAbcProcess
TriceAbcCurrentStamp32
TriceAbcCurrentStamp16
TriceAbcCurrentStampBits
```

Use `C` in the user-facing macro names:

```text
triceC
TriceC
TRiceC
trice8C
Trice8C
TRice8C
...
```

Avoid new public `rpc` names.

## 5. Configuration switches

Add default configuration values, preferably in `triceDefaultConfig.h`.

```c
#ifndef TRICE_ABC_SUPPORT
#define TRICE_ABC_SUPPORT 0
#endif

#ifndef TRICE_ABC_RECEIVE_SUPPORT
#define TRICE_ABC_RECEIVE_SUPPORT 0
#endif

#define TRICE_ABC_PENDING_LAST_WINS 1
#define TRICE_ABC_PENDING_DROP_NEW  2

#ifndef TRICE_ABC_PENDING_POLICY
#define TRICE_ABC_PENDING_POLICY TRICE_ABC_PENDING_LAST_WINS
#endif

#ifndef TRICE_ABC_MAX_PAYLOAD_BYTES
#define TRICE_ABC_MAX_PAYLOAD_BYTES (TRICE_SINGLE_MAX_SIZE - 12)
#endif
```

Initial implementation recommendation:

- implement only `TRICE_ABC_PENDING_LAST_WINS`,
- define the policy constants now,
- leave `DROP_NEW` as future work unless simple to add.

`TRICE_ABC_SUPPORT` controls ABC send macro availability.

`TRICE_ABC_RECEIVE_SUPPORT` controls receive tables, mailbox state, receive helper functions, and `TriceAbcProcess`.

## 6. Public macro API

### 6.1 Clean user source forms

The user manual should show clean source forms without inserted ID arguments.

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

### 6.2 Inserted source forms

Follow the existing `trice insert` convention and add a raw Trice ID argument to C macro calls.

For lower/mixed-case ABC macros, the inserted ID argument shall be a raw ID expression, preferably `iD(n)`, not `ID(n)`, `Id(n)`, or `id(n)`.

Example clean source:

```c
TRice32C(stamp32, "cmd:set_time", p32, cnt);
```

Example inserted source:

```c
TRice32C(iD(12345), stamp32, "cmd:set_time", p32, cnt);
```

Reason: `ID(n)` and `Id(n)` write normal Trice stamps from `TriceStamp32` and `TriceStamp16`. ABC stamped variants must use the user-supplied ABC stamp instead.

No-stamp inserted example:

```c
trice32C(iD(12345), "cmd:set_time", p32, cnt);
```

To be decided:

- Whether the tool should insert raw `iD(n)` or plain numeric `n` for ABC macros. Recommendation: use `iD(n)` for consistency with existing parser/helper conventions.

### 6.3 Explicit-ID uppercase ABC macros

Provide explicit-ID uppercase macros for users and tests that want to write IDs manually.

Recommended syntax:

```c
TRICE_C(iD(12345), stamp32, "cmd:motor_stop");
Trice_C(iD(12345), stamp16, "cmd:motor_stop");
trice_C(iD(12345), "cmd:motor_stop");

TRICE32_C(iD(12345), stamp32, "cmd:set_time", p32, cnt);
Trice32_C(iD(12345), stamp16, "cmd:set_time", p32, cnt);
trice32_C(iD(12345), "cmd:set_time", p32, cnt);
```

To be decided:

- Exact casing for explicit-ID ABC macros. Existing Trice uses all-uppercase `TRICE32_*` for explicit forms, but ABC also needs stamp-width distinction. The recommended direction is to preserve `TRice`/`Trice`/`trice` style for stamp width and use `_C` for explicit-ID forms only if it fits the existing parser.

Hard rule:

```text
Do not require users to pass ID(n) or Id(n) to stamped ABC macros.
```

## 7. Internal target-side send macros

### 7.1 Stamp-only helpers

Define ABC stamp-only helpers with a Trice-specific prefix to avoid collisions:

```c
#define TRICE_C_STAMP32(stamp32)  /* write user supplied 32-bit ABC stamp */
#define TRICE_C_STAMP16(stamp16)  /* write user supplied 16-bit ABC stamp */
```

These macros have no ID semantics.

They shall write the stamp in the same transfer byte order as normal Trice stamps.

### 7.2 ABC ID/header helpers

Add internal ABC ID/header helpers that write the Trice type/ID part without fetching `TriceStamp32` or `TriceStamp16`.

Recommended names:

```c
#define TRICE_C_ID32(n)  /* write S4-style ID/header for following 32-bit ABC stamp */
#define TRICE_C_ID16(n)  /* write S2-style ID/header for following 16-bit ABC stamp */
#define TRICE_C_ID0(n)   /* write S0-style ID/header without stamp */
```

These helpers are not the same as `ID(n)`, `Id(n)`, or `id(n)`.

`TRICE_C_ID32(n)` must write the same ID/header representation that a normal 32-bit-stamped Trice uses before the stamp, but must not write `TriceStamp32`.

`TRICE_C_ID16(n)` must write the same ID/header representation that a normal 16-bit-stamped Trice uses before the stamp, but must not write `TriceStamp16`.

`TRICE_C_ID0(n)` may map to the no-stamp ID writing logic if this is binary-identical to normal `id(n)`.

### 7.3 ABC counted-buffer core

Add ABC-specific counted-buffer cores for stamped ABC messages.

Recommended internal shape:

```c
#define TRICE_C_N32(tid, stamp32, pFmt, buf, nBytes) \
    do {                                             \
        TRICE_UNUSED(pFmt);                          \
        /* length limit and truncation as in TRICE_N */ \
        TRICE_ENTER                                  \
        tid;                                         \
        TRICE_C_STAMP32(stamp32);                    \
        /* count/cycle and payload as in TRICE_N */   \
        TRICE_LEAVE                                  \
    } while (0)

#define TRICE_C_N16(tid, stamp16, pFmt, buf, nBytes) \
    do {                                             \
        TRICE_UNUSED(pFmt);                          \
        TRICE_ENTER                                  \
        tid;                                         \
        TRICE_C_STAMP16(stamp16);                    \
        /* count/cycle and payload as in TRICE_N */   \
        TRICE_LEAVE                                  \
    } while (0)

#define TRICE_C_N0(tid, pFmt, buf, nBytes) \
    TRICE_N(tid, pFmt, buf, nBytes)
```

No-stamp ABC can use `TRICE_N` if `tid` is compatible with existing no-stamp ID writing.

Stamped ABC must not call `TRICE_N` with `ID(n)` or `Id(n)`, because those macros would inject normal Trice timestamp values.

### 7.4 Payload byte conversion

Payload macro wrappers shall convert element count to byte count:

```c
trice8C:  nBytes = 1 * cnt
trice16C: nBytes = 2 * cnt
trice32C: nBytes = 4 * cnt
trice64C: nBytes = 8 * cnt
```

This must match the existing `TRICE8_B`, `TRICE16_B`, `TRICE32_B`, and `TRICE64_B` behavior.

## 8. Host-side TIL and display behavior

### 8.1 TIL type recognition

Extend the parser/generator to recognize the new ABC macro family:

```text
triceC, TriceC, TRiceC
trice8C, Trice8C, TRice8C
trice16C, Trice16C, TRice16C
trice32C, Trice32C, TRice32C
trice64C, Trice64C, TRice64C
explicit-ID variants, once finalized
```

TIL entries shall preserve the original macro type string so the host tool can distinguish ABC entries from normal buffer entries and from legacy `F` entries.

Do not classify `F` entries as ABC by default.

### 8.2 Bit width and parameter count

For ABC payload entries:

```text
trice8C  -> bitWidth = 8
trice16C -> bitWidth = 16
trice32C -> bitWidth = 32
trice64C -> bitWidth = 64
```

For `triceC` without payload:

```text
bitWidth = 0
paramCount = 0
```

For payload ABC entries with runtime count, use the same convention currently used for `B`/`F` buffer-style entries, such as negative or unknown parameter count if applicable.

### 8.3 Display formatting

ABC payload display can reuse the existing `F` display style:

```text
cmd:set_time(00000001)
cmd:set_pwm(0123)(0456)
cmd:motor_stop()
```

For stamped ABC messages, do not label the stamp as `time:`. Recommended display prefix:

```text
stamp:87654321 default: cmd:motor_stop()
```

To be decided:

- Exact display prefix: `stamp:`, `abc:`, or another short label. Recommendation: `stamp:`.

### 8.4 Tags

ABC tags are ordinary Trice tags. Existing tag features such as colors, ID ranges, `-pick`, and `-ban` should continue to work.

The generator must not depend on the tag being `cmd`. ABC semantics are determined by macro type, not by tag string.

## 9. Generator output

### 9.1 CLI options

Add:

```bash
trice generate -abcH -abcC
```

Output names:

```text
<base>Abc.h
<base>Abc.c
```

For example:

```text
demoTIL.json -> demoTILAbc.h, demoTILAbc.c
```

Legacy options:

```bash
-rpcH
-rpcC
```

Recommended behavior:

- Keep `-rpcH` and `-rpcC` as deprecated legacy options for `triceF` users for now.
- Do not make `-rpcH` and `-rpcC` generate ABC files silently.
- Add a deprecation note in help output.
- Prefer `-abcH` and `-abcC` for all new work.

### 9.2 Generated header

Recommended generated header shape:

```c
//! \file demoTILAbc.h
//! Trice generated ABC code - do not edit!

#ifndef DEMOTILABC_H_
#define DEMOTILABC_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*triceAbcHandler_t)(void* payload, int count);

typedef struct {
    uint16_t id;
    uint8_t bitWidth;
    triceAbcHandler_t fn;
} triceAbc_t;

extern const triceAbc_t triceAbc[];
extern const unsigned triceAbcElements;

void motor_stop(void);
void set_time(int32_t* p, int cnt);
void set_pwm(int16_t* p, int cnt);

#ifdef __cplusplus
}
#endif

#endif // DEMOTILABC_H_
```

For zero-payload commands, generate `void name(void)`.

For payload commands, generate:

```c
void name(int8_t* p, int cnt);
void name(int16_t* p, int cnt);
void name(int32_t* p, int cnt);
void name(int64_t* p, int cnt);
```

according to bit width.

### 9.3 Generated source

Recommended generated source shape:

```c
//! \file demoTILAbc.c
//! Trice generated ABC code - do not edit!

#include "demoTILAbc.h"
#include "trice.h"

void motor_stop(void) TRICE_WEAK;
void motor_stop(void) {}

void set_time(int32_t* p, int cnt) TRICE_WEAK;
void set_time(int32_t* p, int cnt) { (void)p; (void)cnt; }

static void triceAbcCall_motor_stop(void* p, int cnt) {
    (void)p;
    (void)cnt;
    motor_stop();
}

static void triceAbcCall_set_time(void* p, int cnt) {
    set_time((int32_t*)p, cnt);
}

const triceAbc_t triceAbc[] = {
    /* Trice type */  /* id, bitWidth, function pointer */
    /* TRiceC   */ { 12345, 0,  triceAbcCall_motor_stop },
    /* trice32C */ { 12346, 32, triceAbcCall_set_time },
};

const unsigned triceAbcElements = sizeof(triceAbc) / sizeof(triceAbc_t);
```

Important: the exact weak syntax must be portable through existing `TRICE_WEAK` where possible. Avoid hard-coding only `__attribute__((weak))`.

### 9.4 Dedupe rules

The generator must deduplicate:

- handler prototypes,
- weak handler definitions,
- wrapper functions.

Multiple IDs may map to the same handler if the command name and signature are identical.

Allowed:

```c
{ 1001, 32, triceAbcCall_set_time },
{ 1002, 32, triceAbcCall_set_time },
```

Not allowed:

```text
same command name with different bit width
same command name once as no-payload and once as payload command
same ID with different command name or different signature
```

Such conflicts must be generator errors, not warnings.

### 9.5 Command name extraction and validation

Given a format string:

```text
cmd:set_time
```

Extract:

```text
tag = cmd
commandName = set_time
```

If no colon exists:

```text
commandName = full string
```

Validate commandName against:

```text
[A-Za-z_][A-Za-z0-9_]*
```

Reject or error on invalid command names.

To be decided:

- Whether multi-colon format strings are rejected or the command name is extracted after the first colon. Recommendation: reject multi-colon ABC command strings initially to avoid ambiguity.

## 10. ABC receive runtime

### 10.1 Generated runtime state

Keep generated constant metadata separate from mutable receive state.

Recommended state type:

```c
typedef struct {
    volatile uint8_t pending;
    uint8_t stampBits;      // 0, 16, or 32
    uint8_t bitWidth;       // 0, 8, 16, 32, or 64
    uint16_t count;         // element count for handler
    uint32_t stamp;         // normalized ABC stamp value
    uint8_t payload[TRICE_ABC_MAX_PAYLOAD_BYTES];
} triceAbcState_t;

extern triceAbcState_t triceAbcState[];
```

The state array length must equal `triceAbcElements`.

For very small targets, RAM cost is a concern. Future generator options may reduce the table to selected commands.

To be decided:

- Whether `triceAbcState[]` is generated in `<base>Abc.c` or in a separate runtime C file.
- Whether payload storage is fixed per entry or globally shared. Recommendation for first implementation: fixed per entry for simplicity.

### 10.2 Receive API

Provide a target-side API that can be called by a decoder or transport bridge when a Trice message was received:

```c
typedef enum {
    TRICE_ABC_RX_IGNORED = 0,
    TRICE_ABC_RX_ACCEPTED = 1,
    TRICE_ABC_RX_OVERWRITTEN = 2,
    TRICE_ABC_RX_DROPPED = 3,
    TRICE_ABC_RX_BAD_PAYLOAD = 4,
} TriceAbcRxResult;

TriceAbcRxResult TriceAbcOnReceive(
    uint16_t id,
    uint8_t stampBits,
    uint32_t stamp,
    const void* payload,
    uint16_t payloadBytes
);
```

Expected behavior:

1. Search `triceAbc[]` for `id`.
2. If no entry exists, return `TRICE_ABC_RX_IGNORED`.
3. Validate payload byte count against `bitWidth`.
4. Convert byte count to element count.
5. Copy payload into `triceAbcState[index].payload`.
6. Store stamp and stampBits.
7. Store count and bitWidth.
8. Set `pending` last.
9. Return accepted/overwritten/dropped result according to policy.

For payload validation:

```text
bitWidth = 0  -> payloadBytes must be 0
bitWidth = 8  -> any payloadBytes <= max is valid, count = payloadBytes
bitWidth = 16 -> payloadBytes % 2 == 0
bitWidth = 32 -> payloadBytes % 4 == 0
bitWidth = 64 -> payloadBytes % 8 == 0
```

### 10.3 Processing API

Provide:

```c
void TriceAbcProcess(void);
```

Behavior:

1. Iterate over all ABC entries.
2. If `pending == 0`, continue.
3. Enter a short critical section.
4. Copy the state slot into a local snapshot.
5. Clear `pending`.
6. Leave the critical section.
7. Set current ABC context from snapshot stamp fields.
8. Call `triceAbc[index].fn(snapshot.payload, snapshot.count)`.
9. Clear current ABC context.

Clearing pending before calling the handler avoids erasing a new command that arrives while the handler runs.

The handler must not be called from the low-level receive context in the default implementation.

### 10.4 Current stamp context

Provide helper functions:

```c
uint8_t  TriceAbcCurrentStampBits(void);
uint16_t TriceAbcCurrentStamp16(void);
uint32_t TriceAbcCurrentStamp32(void);
```

During handler execution:

- `TriceAbcCurrentStampBits()` returns `0`, `16`, or `32`.
- `TriceAbcCurrentStamp16()` returns the low 16 bits of the current stamp.
- `TriceAbcCurrentStamp32()` returns the current normalized 32-bit stamp.

Outside handler execution, the return value is implementation-defined but should be `0` for bits and `0` for stamp values.

To be decided:

- Whether current stamp context is global, thread-local, or user-provided for RTOS builds. Initial bare-metal implementation can use globals guarded by the processing model.

### 10.5 Search strategy

Initial implementation may use linear search over `triceAbc[]`.

Future optimization options:

- sort generated entries by ID and binary search,
- generate a perfect hash or direct index table,
- generate per-ID-range lookup tables.

Do not optimize prematurely. Linear search is acceptable for a first implementation.

## 11. Multiple senders and TIL merging

Do not merge generated `*Abc.c` and `*Abc.h` files.

Merge or import TIL data before generating C code.

Recommended future CLI options:

```bash
trice til merge -o merged.til.json a.til.json b.til.json c.til.json
trice generate -abcH -abcC -i merged.til.json
```

Alternative future generator option:

```bash
trice generate -abcH -abcC -i local.til.json -abcImport remote_a.til.json -abcImport remote_b.til.json
```

Conflict rules:

| Case | Result |
| --- | --- |
| Same ID, same type, same command name | dedupe |
| Same ID, different command name | error |
| Same ID, different bit width | error |
| Same command name, same signature, multiple IDs | allowed |
| Same command name, different signature | error |
| Imported command without local implementation | allowed, weak no-op |

To be decided:

- Exact command name and CLI for TIL merging/import.
- Whether this is part of the first ABC implementation or a follow-up.

## 12. Backward compatibility and deprecation

### 12.1 `triceF`

Do not change `triceF` macro implementation.

Documentation changes:

- Mark `triceF` as deprecated.
- State that `triceF` was an experimental remote-function-call syntax.
- Refer to Trice ABC for new command-style communication.

Implementation changes:

- Do not emit compiler deprecation warnings initially, unless explicitly requested by a config switch.
- Do not make `triceF` generate ABC tables by default.

### 12.2 `-rpcH` / `-rpcC`

Recommended transition:

- Keep `-rpcH` and `-rpcC` as deprecated legacy generator options.
- Keep their old output names and behavior if existing users depend on them.
- Add `-abcH` and `-abcC` as the new supported path.
- Add help text that says new code should use ABC.

To be decided:

- Exact removal timeline for legacy RPC options.

## 13. Tests

Add tests for at least the following.

### 13.1 Parser and TIL tests

- `triceC`, `TriceC`, `TRiceC` recognized.
- `trice8C`, `trice16C`, `trice32C`, `trice64C` recognized.
- Bit width extracted correctly.
- Runtime-count payload represented consistently with `B`/`F` conventions.
- Tags remain tags only.
- Invalid command names are rejected.
- `F` macros are not selected by `-abcH`/`-abcC`.

### 13.2 Code generation tests

- `-abcH` creates expected header.
- `-abcC` creates expected C file.
- Duplicate handlers are generated once.
- Duplicate weak definitions are not emitted.
- Same function name with different bit width is an error.
- Same ID with incompatible command information is an error.
- Zero-payload command generates `void name(void)`.
- Payload command generates typed pointer signature.

### 13.3 Target macro encoding tests

- No-stamp `trice32C` produces binary-compatible no-stamp counted-buffer Trice encoding.
- `Trice32C(stamp16, ...)` writes the user-supplied 16-bit ABC stamp, not `TriceStamp16`.
- `TRice32C(stamp32, ...)` writes the user-supplied 32-bit ABC stamp, not `TriceStamp32`.
- Payload byte length equals element count times element width.
- `TRICE_ABC_SUPPORT == 0` excludes ABC send code.

### 13.4 Receive runtime tests

- Unknown ID ignored.
- Known ID sets pending.
- Payload copied correctly.
- Stamp stored correctly.
- `TriceAbcProcess()` calls the expected handler.
- Pending flag is cleared before handler call.
- Last-wins overwrite works.
- Bad payload size is rejected.

### 13.5 Display tests

- ABC display uses command-style rendering.
- Stamped ABC display does not show `time:` unless explicitly configured.
- `-pick` and `-ban` still work with tags such as `cmd` and `rsp`.

## 14. Recommended implementation order

1. Add documentation scaffolding and mark `triceF` as deprecated.
2. Add parser recognition for `C` macro families.
3. Add TIL representation and command-name validation.
4. Add `-abcH` and `-abcC` generator options.
5. Generate deduplicated weak handler prototypes and definitions.
6. Add no-stamp `triceC`/`trice8C`/`trice16C`/`trice32C`/`trice64C` send path using existing counted-buffer logic where possible.
7. Add stamped ABC send path with `TRICE_C_STAMP16` and `TRICE_C_STAMP32`.
8. Add receive table and one-deep mailbox state.
9. Add `TriceAbcOnReceive`, `TriceAbcProcess`, and current-stamp helpers.
10. Add tests.
11. Add optional TIL merge/import support later.

## 15. Open decisions

The following points are intentionally left as decisions for implementation review.

1. Exact explicit-ID macro spelling for ABC.
2. Whether ABC inserted source uses `iD(n)` or plain numeric IDs.
3. Exact host display label for ABC stamps: recommended `stamp:`.
4. Whether multi-colon command strings are rejected or parsed after first colon: recommended reject.
5. Whether fixed per-entry payload storage is acceptable for all targets: recommended first implementation yes.
6. Whether `DROP_NEW` pending policy is implemented initially: recommended no.
7. Exact CLI for TIL merge/import: recommended as follow-up.
8. Removal timeline for legacy `triceF`, `-rpcH`, and `-rpcC`.
9. RTOS/thread-local behavior of current ABC stamp context.

## 16. Non-negotiable constraints

- `triceF` macros remain untouched in the first ABC implementation.
- New ABC code must not rely on `ID(n)` or `Id(n)` for user-supplied ABC stamps.
- Stamp-only helper macros must be prefixed: `TRICE_C_STAMP32`, `TRICE_C_STAMP16`.
- ABC generation must not select legacy `F` entries by default.
- Generated weak handlers must be deduplicated.
- Handler name/signature conflicts must be generator errors.
- ABC receive must not call user handlers directly from the low-level receive context by default.
- ABC is a primitive, not a full RPC framework.
