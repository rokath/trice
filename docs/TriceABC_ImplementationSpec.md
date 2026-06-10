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

This document specifies the first implementable Trice ABC version. It uses the simplified `trice generate -abc=<target>` workflow and keeps the existing legacy `triceF` / `-rpcH` / `-rpcC` path functionally unchanged.

The primary design goal is a small, testable building block: a Trice message can be emitted as an asynchronous broadcast command, and a receiving target can map its Trice ID to a locally compiled handler selected for that target.

<h2>Table of Contents</h2>

<!-- mdtoc -->

- [1. Goal](#goal)
- [2. What ABC deliberately does not implement](#what-abc-deliberately-does-not-implement)
- [3. Existing legacy path](#existing-legacy-path)
- [4. Terminology](#terminology)
  - [4.1. ABC command](#abc-command)
  - [4.2. ABC Trice ID](#abc-trice-id)
  - [4.3. ABC command name](#abc-command-name)
  - [4.4. ABC tag / prefix](#abc-tag--prefix)
  - [4.5. ABC stamp](#abc-stamp)
  - [4.6. ABC handler](#abc-handler)
- [5. Configuration switches](#configuration-switches)
- [6. Build profiles and dependency boundaries](#build-profiles-and-dependency-boundaries)
- [7. Public ABC send macros](#public-abc-send-macros)
  - [7.1. Clean user source forms](#clean-user-source-forms)
  - [7.2. Inserted source forms](#inserted-source-forms)
  - [7.3. Explicit-ID _C macro forms](#explicit-id-_c-macro-forms)
- [8. Target-side ABC stamp writing](#target-side-abc-stamp-writing)
  - [8.1. Stamp-only helpers](#stamp-only-helpers)
  - [8.2. ABC ID/header helpers](#abc-idheader-helpers)
  - [8.3. ABC counted-buffer cores](#abc-counted-buffer-cores)
  - [8.4. Payload byte conversion](#payload-byte-conversion)
- [9. Host-side parser and TIL behavior](#host-side-parser-and-til-behavior)
  - [9.1. Macro recognition](#macro-recognition)
  - [9.2. Argument recognition](#argument-recognition)
  - [9.3. Bit width and payload count](#bit-width-and-payload-count)
  - [9.4. Display behavior](#display-behavior)
- [10. Generator CLI](#generator-cli)
- [11. targetabc.h selection file](#targetabch-selection-file)
  - [11.1. First run](#first-run)
  - [11.2. Later runs](#later-runs)
  - [11.3. Declaration syntax](#declaration-syntax)
  - [11.4. Missing-ID warning](#missing-id-warning)
- [12. targetabc.c generated file](#targetabcc-generated-file)
  - [12.1. Runtime types](#runtime-types)
  - [12.2. Generated source shape](#generated-source-shape)
  - [12.3. Link behavior](#link-behavior)
- [13. Generator matching and conflict rules](#generator-matching-and-conflict-rules)
- [14. ABC receive runtime](#abc-receive-runtime)
  - [14.1. Receive API](#receive-api)
  - [14.2. Payload validation](#payload-validation)
  - [14.3. Alignment and byte order](#alignment-and-byte-order)
- [15. ABC handler context](#abc-handler-context)
- [16. Multiple devices and TIL management](#multiple-devices-and-til-management)
- [17. Security boundary](#security-boundary)
- [18. Tests](#tests)
  - [18.1. Parser, insert, clean, and TIL tests](#parser-insert-clean-and-til-tests)
  - [18.2. Generator workflow tests for -abc=deviceA](#generator-workflow-tests-for--abcdevicea)
    - [18.2.1. First-run header generation](#first-run-header-generation)
    - [18.2.2. Later-run selection behavior](#later-run-selection-behavior)
    - [18.2.3. Generated source assertions](#generated-source-assertions)
    - [18.2.4. Conflict tests](#conflict-tests)
  - [18.3. Target transmit tests using the existing CGO infrastructure](#target-transmit-tests-using-the-existing-cgo-infrastructure)
  - [18.4. XTEA-specific transmit/decode tests](#xtea-specific-transmitdecode-tests)
  - [18.5. Host-native C receive runtime tests](#host-native-c-receive-runtime-tests)
  - [18.6. Receive-only dependency test](#receive-only-dependency-test)
  - [18.7. Receive tests under different build configurations](#receive-tests-under-different-build-configurations)
  - [18.8. Optional target-side inbound decode integration test](#optional-target-side-inbound-decode-integration-test)
  - [18.9. Legacy tests](#legacy-tests)
  - [18.10. End-to-end generator/build example test](#end-to-end-generatorbuild-example-test)
- [19. Target-code examples](#target-code-examples)
  - [19.1. Example: request power state from a swarm](#example-request-power-state-from-a-swarm)
    - [19.1.1. Sender command](#sender-command)
    - [19.1.2. Receiver selection](#receiver-selection)
    - [19.1.3. Receiver handler implementation](#receiver-handler-implementation)
    - [19.1.4. Response collector](#response-collector)
  - [19.2. Example: defer work in the user handler](#example-defer-work-in-the-user-handler)
  - [19.3. Example: host-native receive runtime test](#example-host-native-receive-runtime-test)
  - [19.4. Example: encrypted transmit remains ordinary Trice transport](#example-encrypted-transmit-remains-ordinary-trice-transport)
- [20. Recommended implementation order](#recommended-implementation-order)
- [21. Follow-up items](#follow-up-items)
- [22. Non-negotiable constraints](#non-negotiable-constraints)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

## 1. <a id="goal"></a>Goal

Implement an optional Trice ABC feature that allows selected Trice messages to be interpreted by receiving devices as asynchronous broadcast commands.

A Trice ABC sender emits a normal Trice message with the same binary framing and record structure as other Trice messages:

- normal 14-bit Trice ID,
- normal 2-bit Trice stamp selector,
- optional ABC stamp stored in the existing Trice stamp field,
- normal Trice count/cycle field,
- optional payload bytes.

There is no new package format. The difference between a normal Trice and a Trice ABC message is the source macro family, the TIL metadata, the interpretation of the optional stamp field, and optional receive-side ID-to-handler mapping.

At runtime, the sender does not wait and does not need to know which devices will react. A receiver may map the received Trice ID to a locally compiled handler selected for this target.

Conceptually:

```text
sender emits ABC with Trice ID X and optional payload P

receiver A: ID X -> local handler A
receiver B: ID X -> local handler B
receiver C: ID X -> no selected handler, ignore
```

The same Trice ID may therefore cause different local behavior on different targets, depending on the target-local `<target>_abc.h` selection and application implementation. The Trice ID maps to the same ABC command name in the shared TIL, but each target supplies its own handler implementation or no handler at all.

## 2. <a id="what-abc-deliberately-does-not-implement"></a>What ABC deliberately does not implement

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

## 3. <a id="existing-legacy-path"></a>Existing legacy path

The repository already contains an experimental remote-function-call style path based on `triceF` and `rpc` naming.

Existing behavior to preserve initially:

- `triceF`, `TriceF`, `TRiceF`, `TRICE*_F`, and their bit-width variants remain functionally unchanged for existing users.
- `trice generate -rpcH -rpcC` remains functionally unchanged.
- Existing generated RPC filenames and symbols remain unchanged.
- ABC generation does not select `F` entries.

Required changes around the legacy path:

- Documentation marks `triceF` and `-rpcH` / `-rpcC` as deprecated.
- Legacy `triceF` target code is gated behind the configuration switch `TRICE_LEGACY_RPC_SUPPORT`.
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

## 4. <a id="terminology"></a>Terminology

### 4.1. <a id="abc-command"></a>ABC command

A Trice message emitted through a `triceC` macro family and intended to be interpretable as a command by receivers.

### 4.2. <a id="abc-trice-id"></a>ABC Trice ID

The normal 14-bit Trice ID associated with an ABC command. It is the primary receive-table key.

### 4.3. <a id="abc-command-name"></a>ABC command name

The C handler name extracted from the ABC format string. The generator uses the part after the last colon:

```text
cmd:motor_stop           -> motor_stop
cmd:deviceA:motor_step   -> motor_step
a:b:c:d:e:f             -> f
MotorStop                -> MotorStop
```

The extracted command name must be a valid C identifier:

```text
[A-Za-z_][A-Za-z0-9_]*
```

### 4.4. <a id="abc-tag--prefix"></a>ABC tag / prefix

Everything before the last colon is display/filter metadata. It may be used for normal Trice tag behavior such as colors, ID ranges, `-pick`, or `-ban`.

The prefix is not ABC addressing and must not define ABC semantics.

### 4.5. <a id="abc-stamp"></a>ABC stamp

A user-provided correlation value transported in the existing Trice stamp field for ABC messages.

ABC stamps are not necessarily timestamps. Typical meanings are request ID, response correlation ID, device-plus-sequence value, transaction ID, or another application-defined marker.

### 4.6. <a id="abc-handler"></a>ABC handler

A locally compiled application function selected by `<target>_abc.h` and referenced through the generated `<target>_abc.c` table.

The generator does not generate weak no-op handlers for ABC.

## 5. <a id="configuration-switches"></a>Configuration switches

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
| `TRICE_ABC_RECEIVE_SUPPORT == 1`  | Enable ABC receive runtime types and direct handler dispatch. |
| `TRICE_LEGACY_RPC_SUPPORT == 1`   | Enable deprecated legacy `triceF` target code.         |

Transmit and receive support are independent. Do not use a single `TRICE_ABC_SUPPORT` switch unless it is only a convenience alias that expands to the two explicit switches.

When `TRICE_ABC_TRANSMIT_SUPPORT == 0`, active target code shall not contain usable ABC send macro implementations. This keeps ABC target code out of builds that do not use it. `TRICE_OFF` and `TRICE_CLEAN` behavior must remain compatible with normal Trice behavior.

When `TRICE_ABC_RECEIVE_SUPPORT == 1`, the ABC receive core shall be buildable without enabling ABC transmit support. It shall also not require normal Trice output buffers, Trice transfer backends, `TriceStamp16`, `TriceStamp32`, UART/RTT output code, or normal `trice()` send macros.

## 6. <a id="build-profiles-and-dependency-boundaries"></a>Build profiles and dependency boundaries

ABC transmit and ABC receive are independent build profiles. This is an architectural requirement, not only a code-size optimization.

A target may be a command receiver without being a Trice logger or ABC sender. Such a target needs only the pieces required to transform an incoming Trice record into an ABC handler call.

| Build profile              | Required pieces                                                                                                                 | Must not require                                                                                                        |
|----------------------------|---------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------|
| Normal Trice logging only  | Existing Trice send library and configured output path.                                                                         | ABC receive runtime.                                                                                                    |
| ABC transmit only          | Existing counted-buffer send path plus `triceC` macros and ABC stamp helpers.                                                   | ABC receive table or receive runtime.                                                                                   |
| ABC receive core only      | `triceAbcReceive.h`, `TriceAbcOnReceive()`, generated `<target>_abc.c`, user `<target>_abc.h`, and small shared types/macros. | Full `trice.h` include tree, `TRICE_SINGLE_MAX_SIZE`, Trice output buffers, transfer backends, normal Trice send macros, `TriceStamp16`, `TriceStamp32`, ABC transmit macros. |
| ABC receive with responses | ABC receive core plus whichever Trice/ABC transmit support the application uses for responses.                                  | Built-in response policy.                                                                                               |
| Legacy `triceF`            | Existing legacy `triceF` target code enabled by `TRICE_LEGACY_RPC_SUPPORT=1`.                                                   | ABC generation or ABC receive runtime.                                                                                  |

The ABC receive core starts at this interface:

```c
TriceAbcOnReceive(id, stampBits, stamp, payload, payloadBytes);
```

It receives already decoded values. Deframing, decryption, raw Trice binary parsing, and transport-specific byte collection are integration-layer responsibilities. If such an integration layer is implemented later, it shall call `TriceAbcOnReceive()` after it has decoded the Trice ID, optional ABC stamp, and payload bytes.

The implementation may share small headers, constants, endian helpers, or utility macros with existing Trice target code. It must not accidentally pull in the send/output implementation merely because ABC receive is enabled.

## 7. <a id="public-abc-send-macros"></a>Public ABC send macros

The public macro suffix is `C`, meaning Command.

ABC stamped macros take the stamp argument after the format string:

```c
Trice16C("cmd:name", stamp16, payload, count);
TRice16C("cmd:name", stamp32, payload, count);
```

This order is part of the design.

### 7.1. <a id="clean-user-source-forms"></a>Clean user source forms

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

### 7.2. <a id="inserted-source-forms"></a>Inserted source forms

`trice insert` adds a raw Trice ID argument to ABC macro calls. For ABC macros, the inserted ID argument must be `iD(n)` or an equivalent raw ID expression. It must not be `ID(n)` or `Id(n)`, because those macros inject normal Trice timestamp sources.

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

### 7.3. <a id="explicit-id-_c-macro-forms"></a>Explicit-ID `_C` macro forms

The explicit-ID forms are useful for tests and for users who intentionally write IDs themselves. They already contain the ID argument and are not clean no-ID source forms.

No payload:

```c
TRIce_C(iD(12345), "cmd:motor_stop");           // no ABC stamp
TRICe_C(iD(12346), "cmd:motor_stop", stamp16);  // 16-bit ABC stamp
TRICE_C(iD(12347), "cmd:motor_stop", stamp32);  // 32-bit ABC stamp
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

If easy to implement, explicit-ID `_C` macro forms may also be accepted without the initial `iD(0),` argument. In that case `trice insert` inserts `iD(n),` and `trice clean` removes it again. If this adds too much parser complexity, supporting only the explicit `iD(0),` form is acceptable because these macros are mainly helper/test macros and users should normally prefer the clean source forms.

## 8. <a id="target-side-abc-stamp-writing"></a>Target-side ABC stamp writing

Normal Trice helper macros behave as follows:

```c
ID(n)  // writes ID/header plus TriceStamp32
Id(n)  // writes ID/header plus TriceStamp16
id(n)  // writes ID/header without stamp
iD(n)  // raw parser/helper ID expression
```

ABC stamped messages must write the user-supplied ABC stamp instead of `TriceStamp16` or `TriceStamp32`.

### 8.1. <a id="stamp-only-helpers"></a>Stamp-only helpers

Add stamp-only helpers with collision-safe names:

```c
#define TRICE_C_STAMP32(stamp32)  /* write user supplied 32-bit ABC stamp */
#define TRICE_C_STAMP16(stamp16)  /* write user supplied 16-bit ABC stamp */
```

These helpers have no ID semantics. They shall write the stamp in the same transfer byte order as normal Trice stamps.

### 8.2. <a id="abc-idheader-helpers"></a>ABC ID/header helpers

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

Stamped ABC send paths must be binary-compatible with normal stamped Trice records, except that the stamp field contains the explicit ABC stamp argument. No-stamp ABC messages may reuse existing no-stamp counted-buffer code if the binary output is identical.

### 8.3. <a id="abc-counted-buffer-cores"></a>ABC counted-buffer cores

Stamped ABC messages need ABC-specific counted-buffer cores because the stamp value is explicit.

Recommended internal shape:

```c
#define TRICE_C_N32(tid, pFmt, stamp32, buf, nBytes)  \
    do {                                             \
        TRICE_UNUSED(pFmt);                          \
        /* length limit and truncation as in TRICE_N */ \
        TRICE_ENTER                                  \
        tid;                                         \
        TRICE_C_STAMP32(stamp32);                    \
        /* count/cycle and payload as in TRICE_N */   \
        TRICE_LEAVE                                  \
    } while (0)

#define TRICE_C_N16(tid, pFmt, stamp16, buf, nBytes)  \
    do {                                             \
        TRICE_UNUSED(pFmt);                          \
        /* length limit and truncation as in TRICE_N */ \
        TRICE_ENTER                                  \
        tid;                                         \
        TRICE_C_STAMP16(stamp16);                    \
        /* count/cycle and payload as in TRICE_N */   \
        TRICE_LEAVE                                  \
    } while (0)

#define TRICE_C_N0(tid, pFmt, buf, nBytes) \
    TRICE_N(tid, pFmt, buf, nBytes)
```

The exact macro names may follow existing target-code style, but the separation between explicit ABC stamps and normal `TriceStamp*` sources is mandatory.

### 8.4. <a id="payload-byte-conversion"></a>Payload byte conversion

Payload wrappers convert element count to byte count:

```text
trice8C  / Trice8C  / TRice8C  -> nBytes = 1 * cnt
trice16C / Trice16C / TRice16C -> nBytes = 2 * cnt
trice32C / Trice32C / TRice32C -> nBytes = 4 * cnt
trice64C / Trice64C / TRice64C -> nBytes = 8 * cnt
```

This shall match the existing counted-buffer behavior used by `B` and legacy `F` forms.

## 9. <a id="host-side-parser-and-til-behavior"></a>Host-side parser and TIL behavior

### 9.1. <a id="macro-recognition"></a>Macro recognition

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

### 9.2. <a id="argument-recognition"></a>Argument recognition

For stamped ABC forms, the stamp argument follows the format string and precedes payload arguments.

Examples:

```c
TriceC("cmd:motor_stop", stamp16);
TRice32C("cmd:set_time", stamp32, p32, cnt);
TRICE32_C(iD(1234), "cmd:set_time", stamp32, p32, cnt);
```

The parser must not interpret the ABC stamp as a Trice payload value.

### 9.3. <a id="bit-width-and-payload-count"></a>Bit width and payload count

| Macro family                         | Handler declaration                     | bitWidth |
|--------------------------------------|-----------------------------------------|----------|
| `triceC` / `TriceC` / `TRiceC`       | `void name(const triceAbcRx_t* rx)`     | 0        |
| `trice8C` / `Trice8C` / `TRice8C`    | `void name(const triceAbcRx_t* rx)`     | 8        |
| `trice16C` / `Trice16C` / `TRice16C` | `void name(const triceAbcRx_t* rx)`     | 16       |
| `trice32C` / `Trice32C` / `TRice32C` | `void name(const triceAbcRx_t* rx)`     | 32       |
| `trice64C` / `Trice64C` / `TRice64C` | `void name(const triceAbcRx_t* rx)`     | 64       |

Payload length is runtime-sized and passed as bytes in `rx->payloadBytes`. The selected TIL bit width validates that byte length, but the runtime does not convert it to a separate `count` argument.

### 9.4. <a id="display-behavior"></a>Display behavior

ABC payload display can use the existing command/function style:

```text
cmd:set_time(00000001)
cmd:set_pwm(0123)(0456)
cmd:motor_stop()
```

Stamped ABC output should eventually use `stamp:` instead of `time:` unless the user explicitly configures otherwise. This display refinement is not required before the target-side ABC implementation and generator tests are working. Existing tests may temporarily accept current `time:` output.

Once the display refinement is implemented, tests should require `stamp:` for ABC entries. Existing tag behavior remains unchanged.

## 10. <a id="generator-cli"></a>Generator CLI

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

Do not add `-abcH` / `-abcC` as the primary workflow. The `-abc=<target>` option covers both selection-header initialization and generated source regeneration.

## 11. <a id="targetabch-selection-file"></a>_target_abc.h_ selection file

`<target>_abc.h` is a user-editable selection file. It answers this question:

```text
Which ABC commands shall this target receive and execute?
```

### 11.1. <a id="first-run"></a>First run

If `<target>_abc.h` does not exist, generate it from all ABC commands found in `til.json`.

Example generated file:

```c
//! \file deviceA_abc.h
//! Trice ABC selection file for target deviceA.
//! Generated once; edit this file to select received ABC commands.

#ifndef DEVICEA_ABC_H_
#define DEVICEA_ABC_H_

#include "triceAbcReceive.h"

#ifdef __cplusplus
extern "C" {
#endif

void motor_stop(const triceAbcRx_t* rx);
void get_power_state(const triceAbcRx_t* rx);
void set_time(const triceAbcRx_t* rx);
void set_pwm(const triceAbcRx_t* rx);

#ifdef __cplusplus
}
#endif

#endif /* DEVICEA_ABC_H_ */
```

The file contains declarations only. It must not contain weak definitions.

Use classic include guards, not `#pragma once`, to keep ABC headers independent of non-standard compiler extensions. The include guard name is derived from the filename, converted to uppercase C identifier form.

Example:

```text
deviceA_abc.h -> DEVICEA_ABC_H_
```

### 11.2. <a id="later-runs"></a>Later runs

If `<target>_abc.h` exists, use it as input and do not overwrite it. Active, non-commented declarations define the target-local receive set. Deleted or commented declarations are ignored.

New ABC commands added to `til.json` later are not automatically activated for an existing target. The user may add the corresponding declaration manually or delete/regenerate the selection header.

The selection parser shall strip C comments before looking for declarations:

```c
// void ignored(void);
/* void also_ignored(void); */
void selected(const triceAbcRx_t* rx);
```

The parser is not required to evaluate preprocessor conditionals. Users shall not rely on `#if 0`, `#ifdef`, or similar constructs to deselect ABC handlers in `<target>_abc.h`; use comments or deletion instead.

### 11.3. <a id="declaration-syntax"></a>Declaration syntax

Initial parser support shall handle these canonical forms:

```c
void name(const triceAbcRx_t* rx);
```

Whitespace differences should be tolerated:

```c
void name ( const triceAbcRx_t * rx ) ;
```

Canonical generated declarations use `const triceAbcRx_t*`. The receive context contains the selected bit width, stamp information, a byte pointer, and the payload byte length.

Do not require support for typed payload declarations such as `intN_t*`, application typedefs, or custom context typedefs in the first implementation. Alternate declaration forms may be added later if needed.

### 11.4. <a id="missing-id-warning"></a>Missing-ID warning

If `<target>_abc.h` declares a handler for which no matching ABC command exists in `til.json`, print a warning and do not emit a table entry for it.

Example:

```text
warning: ABC handler 'calibrate_sensor' declared in deviceA_abc.h but no matching ABC command exists in til.json
```

No warning is required for ABC commands in `til.json` that are not declared in `<target>_abc.h`. That is the intended selection mechanism.

## 12. <a id="targetabcc-generated-file"></a>_target_abc.c_ generated file

`<target>_abc.c` is generated code and must be regenerated on every `trice generate -abc=<target>` run.

Behavior:

- overwrite the file every run,
- if it is read-only, make it writable first if possible,
- write the new file,
- set it read-only afterwards if the platform allows it.

This read-only behavior is best effort and must not make generation fail on platforms where file attributes are unavailable.

### 12.1. <a id="runtime-types"></a>Runtime types

Define shared ABC receive runtime types in `triceAbcReceive.h`. This header is intentionally smaller than `trice.h` so receive-only targets can include it without pulling in normal Trice transmit/output declarations:

```c
typedef struct {
    uint16_t id;
    uint8_t stampBits;     // 0, 16, or 32
    uint8_t bitWidth;      // 0, 8, 16, 32, 64
    uint32_t stamp;        // normalized ABC stamp value
    const uint8_t* payload;
    uint16_t payloadBytes;
} triceAbcRx_t;

typedef void (*triceAbcHandler_t)(const triceAbcRx_t* rx);

typedef struct {
    uint16_t id;
    uint8_t bitWidth;  // 0, 8, 16, 32, 64
    triceAbcHandler_t fn;
} triceAbc_t;

extern const triceAbc_t triceAbc[];
extern const unsigned triceAbcElements;
```

`<target>_abc.h` should not be the only owner of these types, because it is user-editable.

### 12.2. <a id="generated-source-shape"></a>Generated source shape

Example generated source:

```c
//! \file deviceA_abc.c
//! Trice generated ABC code - do not edit!

#include "deviceA_abc.h"
#include "triceAbcReceive.h"

static void triceAbcCall_get_power_state(const triceAbcRx_t* rx) {
    get_power_state(rx);
}

static void triceAbcCall_set_time(const triceAbcRx_t* rx) {
    set_time(rx);
}

const triceAbc_t triceAbc[] = {
    /* Trice type */  /* id, bitWidth, function pointer */
    /* TRiceC   */ { 14236u, 0u,  triceAbcCall_get_power_state },
    /* trice32C */ { 14235u, 32u, triceAbcCall_set_time },
};

const unsigned triceAbcElements = sizeof(triceAbc) / sizeof(triceAbc[0]);
```

Use wrappers so the table has one uniform function-pointer type. Do not store typed payload handlers in the table and do not require generated code to cast payload bytes.

If no selected declarations match TIL ABC entries, generate a valid empty table representation. Do not generate a non-standard zero-length array.

Recommended empty representation:

```c
const triceAbc_t triceAbc[] = {
    { 0u, 0u, 0 }
};

const unsigned triceAbcElements = 0u;
```

### 12.3. <a id="link-behavior"></a>Link behavior

The generator does not generate weak no-op handlers for ABC.

If a handler is selected in `<target>_abc.h` and emitted into `<target>_abc.c`, the application must provide an implementation. Missing implementations produce normal linker errors. If the user wants no visible action, they can implement an empty function explicitly.

## 13. <a id="generator-matching-and-conflict-rules"></a>Generator matching and conflict rules

The generator builds the ABC table from:

```text
ABC entries in til.json intersect active declarations in <target>_abc.h
```

Rules:

| Case                                                                  | Result                                        |
|-----------------------------------------------------------------------|-----------------------------------------------|
| TIL ABC command exists and declaration exists with matching context declaration | Emit table entry.                             |
| Declaration exists but no TIL ABC command exists                      | Warning, no table entry.                      |
| TIL ABC command exists but no declaration exists                      | Silently ignore.                              |
| Same command name, same context declaration, multiple IDs              | Emit multiple entries using the same wrapper. |
| Same command name, different ABC bit width in the TIL                  | Error.                                        |
| Same ID, different command name or ABC bit width                       | Error.                                        |
| Invalid command name extracted from ABC format string                 | Error.                                        |

Deduplicate generated wrappers by command name. Multiple IDs may point to the same wrapper if the command name and ABC bit width are identical.

Conflict errors shall be deterministic and include enough information to locate the conflicting IDs and command names.

## 14. <a id="abc-receive-runtime"></a>ABC receive runtime

ABC receive support is enabled only when `TRICE_ABC_RECEIVE_SUPPORT == 1`. It must be usable in an ABC receive-only build, without enabling Trice transmit/output code.

The first receive runtime is direct and minimal:

1. Look up the received Trice ID in `triceAbc[]`.
2. Validate payload size against the selected handler bit width.
3. Build a small `triceAbcRx_t` context on the stack.
4. Call the generated wrapper immediately with a pointer to that context.

It does not provide a queue, mailbox, scheduler, or automatic deferral. If a transport receives data in a context where handler execution is not appropriate, the integration layer or the user handler must defer work explicitly by copying values and setting flags.

### 14.1. <a id="receive-api"></a>Receive API

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
    const uint8_t* payload,
    uint16_t payloadBytes
);
```

Expected behavior:

1. Search `triceAbc[]` linearly for `id`.
2. If no entry exists, return `TRICE_ABC_RX_IGNORED`.
3. Validate `stampBits` is 0, 16, or 32. Invalid values return `TRICE_ABC_RX_BAD_PAYLOAD`.
4. Validate `payloadBytes` against `bitWidth`.
5. If `payloadBytes > 0`, validate `payload != 0`.
6. Prepare `triceAbcRx_t` with ID, stamp metadata, bit width, payload pointer, and payload byte length.
7. Call the table wrapper.
8. Return `TRICE_ABC_RX_EXECUTED`.

### 14.2. <a id="payload-validation"></a>Payload validation

| bitWidth | Valid payload           |
|----------|-------------------------|
| 0        | `payloadBytes == 0`     |
| 8        | Any payload byte count. |
| 16       | `payloadBytes % 2 == 0` |
| 32       | `payloadBytes % 4 == 0` |
| 64       | `payloadBytes % 8 == 0` |

If `bitWidth` is not 0, 8, 16, 32, or 64, return `TRICE_ABC_RX_BAD_PAYLOAD`.

### 14.3. <a id="alignment-and-byte-order"></a>Alignment and byte order

The direct receive runtime passes the decoded payload as a byte pointer and must not cast it to a typed pointer. This avoids alignment traps, avoids a large stack scratch buffer, and keeps receive-only targets independent from transmit-side size limits.

The runtime does not define a receive-side maximum payload size. Any limit belongs to the inbound decoder, transport buffer, or application policy that owns the decoded payload buffer. While `TriceAbcOnReceive()` runs, the integration layer must keep the pointed-to bytes stable and must not overwrite that buffer.

Handlers that need typed values must decode from `rx->payload` explicitly, for example by validating `rx->payloadBytes` and copying bytes into local typed objects before use. The signedness of such typed objects is an application decision; the Trice binary payload carries bytes plus the TIL bit width.

The first implementation does not need to perform cross-endian conversion. It may assume that the integration layer supplies payload bytes in the order expected by the receiving target, or that participating targets use compatible Trice transfer order. Mixed-endian ABC networks are a follow-up topic.

The Trice tool already has CLI control for Trice endianness. If ABC receive is later connected to a target-side Trice stream decoder, the incoming payload endianness must be specified at that decoder/integration layer, not inside individual ABC handlers.

## 15. <a id="abc-handler-context"></a>ABC handler context

ABC handlers receive all decoded metadata through `triceAbcRx_t`:

```c
void set_pwm(const triceAbcRx_t* rx) {
    if (rx == 0 || rx->bitWidth != 16u || rx->payloadBytes != 4u) {
        return;
    }
    /* Decode rx->payload here before using typed values. */
}
```

`rx->stampBits` is `0`, `16`, or `32`. `rx->stamp` is the normalized 32-bit stamp value; for a 16-bit ABC stamp, the relevant value is in the low 16 bits. A handler that does not need correlation information can ignore these fields.

There is no global "current stamp" state in the first implementation. Nested receive calls get their own stack context, and they do not overwrite the caller's context object.

## 16. <a id="multiple-devices-and-til-management"></a>Multiple devices and TIL management

The simplest robust workflow is one common `til.json` for all devices that exchange ABC messages.

If separate code bases produce separate TIL files, align or merge TIL data before generating target-specific ABC files. Do not hand-merge generated `*_abc.c` files.

TIL merge/import tooling is not required for the first ABC implementation.

## 17. <a id="security-boundary"></a>Security boundary

ABC receive allows incoming Trice messages to trigger locally compiled code. ABC is not an authorization system.

Applications using ABC on untrusted transports must provide their own trust boundary, for example by:

- authenticated or encrypted links,
- transport restrictions,
- ID filtering before `TriceAbcOnReceive()`,
- payload validation in every handler,
- disabling ABC receive support in builds that do not need it.

The Trice technique offers optional XTEA encryption. XTEA protects the Trice byte stream before decoding; it does not change the semantics of `TriceAbcOnReceive()`, which receives already decoded ID/stamp/payload data.

## 18. <a id="tests"></a>Tests

Tests shall be added incrementally with each implementation step. Do not defer all tests until the end.

The test strategy has four layers:

1. **Go parser/TIL/generator tests** prove that the tool understands ABC source code and generates correct files.
2. **Existing CGO target-output tests** prove that ABC send macros emit correct binary Trice data under real target configurations, including XTEA.
3. **Host-native C receive tests** prove that `TriceAbcOnReceive()` and generated ABC tables call the correct handlers with correct data.
4. **Example build tests** prove that the documented user workflow can be built and executed without hardware.

Keep these layers separate. ABC receive runtime starts after a Trice record was decoded into `id`, `stampBits`, `stamp`, and `payload`. Therefore XTEA/framing tests belong primarily to the send/decode layer unless a target-side inbound Trice stream decoder is explicitly implemented later.

### 18.1. <a id="parser-insert-clean-and-til-tests"></a>Parser, insert, clean, and TIL tests

**Location:** Go unit tests near the existing parser and ID-management tests, such as `internal/id/*_test.go` or the current package that owns Trice macro parsing and TIL update behavior.

**Input artifact:** a small in-memory or temporary C source file containing clean and explicit-ID ABC macro forms.

Example input fragment:

```c
void AbcParserFixture(void) {
    uint16_t stamp16 = 0xeb61u;
    uint32_t stamp32 = 0xc0de3020u;
    int8_t b8[4] = { 1, 2, 3, 4 };
    int16_t b16[2] = { 100, 200 };
    int32_t b32[2] = { 1000, 2000 };

    triceC("cmd:motor_stop");
    TriceC("cmd:motor_stop16", stamp16);
    TRiceC("cmd:motor_stop32", stamp32);

    trice8C("cmd:set_leds", b8, 4);
    Trice16C("cmd:set_pwm16", stamp16, b16, 2);
    TRice32C("cmd:set_time32", stamp32, b32, 2);

    TRIce32_C(iD(0), "cmd:explicit_no_stamp", b32, 2);
    TRICe32_C(iD(0), "cmd:explicit_stamp16", stamp16, b32, 2);
    TRICE32_C(iD(0), "cmd:explicit_stamp32", stamp32, b32, 2);

    TRice8F("rpc:legacy_not_abc", b8, 4);
}
```

**Operation:** run the same code path that `trice insert` and TIL update use.

**Expected output and assertions:**

- Clean ABC forms are patched with `iD(n)`, not `ID(n)` or `Id(n)`.
- `trice clean` removes inserted `iD(n)` again for clean source forms.
- Explicit-ID `_C` forms with `iD(0)` are patched to `iD(n)` if the existing explicit-ID patching mechanism supports this.
- The TIL entry preserves the macro type string, for example `TRice32C` or `TRICE32_C`.
- `commandName` is extracted after the last colon.
- The ABC stamp argument is not counted as payload.
- Legacy `F` entries are not classified as ABC.

Expected parser facts:

| Source form                                                 | commandName         | bitWidth | stampBits | Notes                     |
|-------------------------------------------------------------|---------------------|---------:|----------:|---------------------------|
| `triceC("cmd:motor_stop")`                                  | `motor_stop`        |        0 |         0 | no payload                |
| `TriceC("cmd:motor_stop16", stamp16)`                       | `motor_stop16`      |        0 |        16 | stamp is not payload      |
| `TRiceC("cmd:motor_stop32", stamp32)`                       | `motor_stop32`      |        0 |        32 | stamp is not payload      |
| `trice8C("cmd:set_leds", b8, 4)`                            | `set_leds`          |        8 |         0 | runtime payload count     |
| `Trice16C("cmd:set_pwm16", stamp16, b16, 2)`                | `set_pwm16`         |       16 |        16 | runtime payload count     |
| `TRice32C("cmd:set_time32", stamp32, b32, 2)`               | `set_time32`        |       32 |        32 | runtime payload count     |
| `TRIce32_C(iD(0), "cmd:explicit_no_stamp", b32, 2)`         | `explicit_no_stamp` |       32 |         0 | explicit ID, no stamp     |
| `TRICe32_C(iD(0), "cmd:explicit_stamp16", stamp16, b32, 2)` | `explicit_stamp16`  |       32 |        16 | explicit ID, 16-bit stamp |
| `TRICE32_C(iD(0), "cmd:explicit_stamp32", stamp32, b32, 2)` | `explicit_stamp32`  |       32 |        32 | explicit ID, 32-bit stamp |

**Negative parser tests:**

- Invalid command names produce deterministic errors.
- Same command name with incompatible signatures produces deterministic errors during generation.
- Stamped ABC forms must never be rewritten to `ID(n)` or `Id(n)`.

### 18.2. <a id="generator-workflow-tests-for--abcdevicea"></a>Generator workflow tests for `-abc=deviceA`

**Location:** Go generator tests near the existing `trice generate` tests, preferably in the package that owns `internal/id/generate.go` and the legacy RPC generator tests.

**Input artifacts:**

- temporary `til.json` with representative ABC entries,
- missing or pre-existing `deviceA_abc.h`,
- temporary output directory or afero memory filesystem.

Minimal TIL fixture content can be produced by the normal parser from a source fixture or hand-written if that is already common in existing generator tests. It must include at least:

|   ID | Type       | Format string         | Expected handler                |
|-----:|------------|-----------------------|---------------------------------|
| 1001 | `TRiceC`   | `cmd:get_power_state` | `get_power_state(void)`         |
| 1002 | `trice16C` | `cmd:set_pwm`         | `set_pwm(int16_t* p, int cnt)`  |
| 1003 | `TRice32C` | `cmd:set_time`        | `set_time(int32_t* p, int cnt)` |
| 1004 | `trice32F` | `rpc:legacy_not_abc`  | not selected by ABC             |

#### 18.2.1. <a id="first-run-header-generation"></a>First-run header generation

**Input:** no `deviceA_abc.h` exists.

**Operation:**

```bash
trice generate -i til.json -abc=deviceA
```

**Expected files:**

```text
deviceA_abc.h
deviceA_abc.c
```

**Expected `deviceA_abc.h`:**

- generated because it was missing,
- contains classic include guards, not `#pragma once`,
- contains `triceAbcReceive.h`,
- contains only declarations,
- contains all ABC commands from the TIL,
- does not contain legacy `F` entries,
- does not contain weak definitions.

Example expected declarations:

```c
void get_power_state(const triceAbcRx_t* rx);
void set_pwm(const triceAbcRx_t* rx);
void set_time(const triceAbcRx_t* rx);
```

#### 18.2.2. <a id="later-run-selection-behavior"></a>Later-run selection behavior

**Input:** pre-existing `deviceA_abc.h`:

```c
#ifndef DEVICEA_ABC_H_
#define DEVICEA_ABC_H_

#include "triceAbcReceive.h"

void get_power_state(const triceAbcRx_t* rx);
// void set_pwm(const triceAbcRx_t* rx);
void set_time(const triceAbcRx_t* rx);
void unknown_local_handler(const triceAbcRx_t* rx);

#endif /* DEVICEA_ABC_H_ */
```

**Operation:**

```bash
trice generate -i til.json -abc=deviceA
```

**Expected behavior:**

- `deviceA_abc.h` is not overwritten.
- `deviceA_abc.c` is regenerated.
- Table entries are emitted only for `get_power_state` and `set_time`.
- `set_pwm` is ignored because it is commented out.
- `unknown_local_handler` produces one warning and no table entry.
- TIL ABC entries absent from the selection header produce no warning.

Expected warning text, exact wording may differ but must contain target file and handler name:

```text
warning: ABC handler 'unknown_local_handler' declared in deviceA_abc.h but no matching ABC command exists in til.json
```

#### 18.2.3. <a id="generated-source-assertions"></a>Generated source assertions

`deviceA_abc.c` shall:

- include `deviceA_abc.h`,
- include `triceAbcReceive.h`,
- generate one wrapper per selected unique command name,
- use a uniform `triceAbcHandler_t` wrapper signature,
- not store typed handlers directly in the table,
- emit `const triceAbc_t triceAbc[]`,
- emit `const unsigned triceAbcElements`,
- generate a valid empty table if no selected handler matches.

Expected selected table shape:

```c
static void triceAbcCall_get_power_state(const triceAbcRx_t* rx) {
    get_power_state(rx);
}

static void triceAbcCall_set_time(const triceAbcRx_t* rx) {
    set_time(rx);
}

const triceAbc_t triceAbc[] = {
    { 1001u, 0u,  triceAbcCall_get_power_state },
    { 1003u, 32u, triceAbcCall_set_time },
};

const unsigned triceAbcElements = sizeof(triceAbc) / sizeof(triceAbc[0]);
```

#### 18.2.4. <a id="conflict-tests"></a>Conflict tests

Required generator error tests:

- same command name once as `trice16C` and once as `trice32C`,
- same command name once as no-payload and once as payload,
- same ID used by different ABC command names,
- same ID used by different ABC bit widths,
- invalid extracted command name.

Each error must identify the conflicting command name and/or ID.

### 18.3. <a id="target-transmit-tests-using-the-existing-cgo-infrastructure"></a>Target transmit tests using the existing CGO infrastructure

**Purpose:** prove that ABC send macros generate correct Trice bytes under real target-code configurations.

**Existing infrastructure to reuse:** the current `_test` packages compile `./_test/testdata/*.c` together with `../src` and a package-specific `triceConfig.h`; the Go test calls C functions via CGO, reads the generated target buffer, runs the Trice tool on that buffer, and compares output against the `//exp:` text.

**Recommended artifact:** add ABC transmit cases to `./_test/testdata/triceCheck.c` if ABC transmit support is enabled in all configurations that scan `triceCheck.c`. If ABC should only run in selected configurations, create a separate `./_test/testdata/triceAbcCheck.c` and a matching CGO helper so non-ABC packages do not collect `//exp:` lines for code that is compiled out.

Do not place `//exp:` ABC lines behind `#if TRICE_ABC_TRANSMIT_SUPPORT == 1` unless the Go expected-result scanner is updated to ignore inactive preprocessor regions. The current scanner reads comments from the source file and does not evaluate C preprocessor conditions.

**Recommended first approach:** enable `TRICE_ABC_TRANSMIT_SUPPORT 1` in the CGO target-test configurations that run `triceCheck.c`, then add ABC cases directly to `triceCheck.c`. This gives broad coverage across direct/deferred, stack/static/ring/double buffer, COBS/TCOBS/no framing, and XTEA configurations.

**Required config updates:**

- Add `#define TRICE_ABC_TRANSMIT_SUPPORT 1` to every CGO test config that shall execute ABC transmit `//exp:` lines.
- Add `#define TRICE_LEGACY_RPC_SUPPORT 1` only to those CGO configs that still execute legacy `triceF` test lines.
- Keep at least one build/configuration test where `TRICE_LEGACY_RPC_SUPPORT == 0` proves legacy `triceF` target code is disabled.

**Input in `triceCheck.c`:** use deterministic stamp values and existing payload arrays.

Example group:

```c
#if TRICE_ABC_TRANSMIT_SUPPORT == 1
uint16_t abcStamp16 = 0xeb61u;
uint32_t abcStamp32 = 0xc0de3020u;
#endif
```

Representative test lines:

```c
break; case __LINE__: TRICE8_C(iD(0), "cmd:Abc8Stamp32", abcStamp32, b8, sizeof(b8) / sizeof(int8_t));    //exp: "..."
break; case __LINE__: TRICe8_C(iD(0), "cmd:Abc8Stamp16", abcStamp16, b8, sizeof(b8) / sizeof(int8_t));    //exp: "..."
break; case __LINE__: TRIce8_C(iD(0), "cmd:Abc8NoStamp", b8, sizeof(b8) / sizeof(int8_t));                //exp: "..."

break; case __LINE__: TRice8C("cmd:Abc8FnStamp32", abcStamp32, b8, sizeof(b8) / sizeof(int8_t));          //exp: "..."
break; case __LINE__: Trice8C("cmd:Abc8FnStamp16", abcStamp16, b8, sizeof(b8) / sizeof(int8_t));          //exp: "..."
break; case __LINE__: trice8C("cmd:Abc8FnNoStamp", b8, sizeof(b8) / sizeof(int8_t));                      //exp: "..."
```

Add analogous 16-bit, 32-bit, and 64-bit cases, but do not test every spelling in every bit width if runtime becomes too long. A balanced minimal set is:

| Coverage target                | Required representative cases                     |
|--------------------------------|---------------------------------------------------|
| No-payload commands            | `triceC`, `TriceC`, `TRiceC`                      |
| Function-like payload commands | `trice8C`, `Trice16C`, `TRice32C`, `trice64C`     |
| Explicit-ID payload commands   | `TRIce8_C`, `TRICe16_C`, `TRICE32_C`, `TRICE64_C` |
| Stamp widths                   | 0, 16, 32                                         |
| Payload widths                 | 8, 16, 32, 64                                     |

**Expected output:**

- Display text uses the ABC format string and payload values.
- For 16-bit and 32-bit ABC stamps, the displayed stamp value equals the explicit `abcStamp16` or `abcStamp32`, not `TriceStamp16` or `TriceStamp32`.
- Until display refinement is implemented, the expected output may still use the current `time:` label. After display refinement, update `//exp:` to require `stamp:` for ABC entries.

Example expected strings after `stamp:` display refinement:

```c
//exp: "stamp:c0de3020default: cmd:Abc32FnStamp32(00000000)(ffffffff)(fffffffe)(33445555)\n"
//exp: "stamp:    eb61default: cmd:Abc16FnStamp16(0000)(ffff)(fffe)(3344)\n"
//exp: "stamp:        default: cmd:Abc8FnNoStamp(00)(ff)(fe)(33)...\n"
```

**What this proves:**

- macro expansion compiles in each selected target configuration,
- ABC messages use the normal Trice output path,
- explicit ABC stamps are encoded correctly,
- payload byte counts are correct,
- the host tool can decode ABC TIL entries,
- XTEA/framing configurations still carry ABC messages correctly because the generated ABC bytes are encrypted/framed and then decoded by the same test path as normal Trices.

### 18.4. <a id="xtea-specific-transmitdecode-tests"></a>XTEA-specific transmit/decode tests

> Comment: It is assumed, that the [Target transmit tests using the existing CGO infrastructure](#target-transmit-tests-using-the-existing-cgo-infrastructure) already test this. So deal with it only, if necessary.

**Purpose:** prove ABC messages survive encrypted transport.

**Recommended configurations:** reuse existing XTEA CGO configurations such as double-buffer or ring-buffer deferred output with COBS/TCOBS and XTEA enabled. At least these matrix points should execute some ABC transmit lines:

| Configuration kind                                                  | What it proves                   |
|---------------------------------------------------------------------|----------------------------------|
| deferred double buffer + COBS + XTEA                                | common encrypted UART-like path  |
| deferred double buffer + TCOBS + XTEA                               | encrypted compressed/framed path |
| ring buffer + COBS/TCOBS + XTEA                                     | different buffer implementation  |
| one direct+deferred mixed configuration with XTEA if already stable | coexistence of output paths      |

**Input:** same ABC transmit lines in `triceCheck.c` or `triceAbcCheck.c`.

**Transfer simulation:**

1. CGO calls the compiled C test function for one `//exp:` line.
2. Target Trice code writes encrypted/framed bytes into the shared C/Go buffer according to `triceConfig.h`.
3. Go passes the byte buffer to the Trice tool handler with matching `-pw=...` and `-pf=...` options.
4. The Trice tool decrypts/deframes/decodes and emits display text.
5. The test compares against `//exp:`.

**Expected result:** same display text as non-XTEA configurations. XTEA is transparent after successful decryption.

**What this does not prove:** it does not prove that an embedded receiver can decrypt inbound XTEA and call ABC handlers. That requires a target-side inbound decode integration that accepts encrypted/framed bytes and forwards decoded records to `TriceAbcOnReceive()`. That integration is not part of the first ABC core unless explicitly added.

### 18.5. <a id="host-native-c-receive-runtime-tests"></a>Host-native C receive runtime tests

**Purpose:** prove the ABC receive runtime and generated table behavior independently from transport, framing, encryption, and the Trice host display.

**Location:** add a small dedicated CGO or C-host test package, for example:

```text
_test/abc_rx_host/
    cgo_test.go
    triceConfig.h
    generated_cgoPackage.go or local CGO wrapper
_test/testdata/triceAbcReceiveTest.c
_test/testdata/triceAbcReceiveTest_abc.h
_test/testdata/triceAbcReceiveTest_abc.c
```

The exact package name may follow existing `_test` naming conventions. It should not be part of the Trice tool binary.

**Configuration:**

```c
#define TRICE_ABC_RECEIVE_SUPPORT 1
#define TRICE_ABC_TRANSMIT_SUPPORT 0
#define TRICE_LEGACY_RPC_SUPPORT 0
```

No UART/RTT output configuration is required for the receive runtime itself. The preferred test build compiles only the ABC receive core, generated ABC files, and the minimum shared headers/helpers. It must not depend on normal Trice output buffers, UART/RTT transfer code, `TriceStamp16`, `TriceStamp32`, or ABC transmit macros. If the first implementation temporarily compiles more files, this shall be marked as technical debt and not treated as the intended dependency model.

**Input artifacts:**

`triceAbcReceiveTest_abc.h`:

```c
#ifndef TRICEABCRECEIVETEST_ABC_H_
#define TRICEABCRECEIVETEST_ABC_H_

#include "triceAbcReceive.h"

void ping(const triceAbcRx_t* rx);
void set_pwm(const triceAbcRx_t* rx);
void set_time(const triceAbcRx_t* rx);
void nested_outer(const triceAbcRx_t* rx);
void nested_inner(const triceAbcRx_t* rx);

#endif /* TRICEABCRECEIVETEST_ABC_H_ */
```

`triceAbcReceiveTest_abc.c`, either generated by `trice generate -abc=triceAbcReceiveTest` during the test setup or checked in as a generated fixture for the first runtime test:

```c
#include "triceAbcReceiveTest_abc.h"
#include "triceAbcReceive.h"

static void triceAbcCall_ping(const triceAbcRx_t* rx) {
    ping(rx);
}

static void triceAbcCall_set_pwm(const triceAbcRx_t* rx) {
    set_pwm(rx);
}

static void triceAbcCall_set_time(const triceAbcRx_t* rx) {
    set_time(rx);
}

static void triceAbcCall_nested_outer(const triceAbcRx_t* rx) {
    nested_outer(rx);
}

static void triceAbcCall_nested_inner(const triceAbcRx_t* rx) {
    nested_inner(rx);
}

const triceAbc_t triceAbc[] = {
    { 1001u, 0u,  triceAbcCall_ping },
    { 1002u, 16u, triceAbcCall_set_pwm },
    { 1003u, 32u, triceAbcCall_set_time },
    { 1004u, 0u,  triceAbcCall_nested_outer },
    { 1005u, 0u,  triceAbcCall_nested_inner },
};

const unsigned triceAbcElements = sizeof(triceAbc) / sizeof(triceAbc[0]);
```

`triceAbcReceiveTest.c` implements handlers and a callable test function:

```c
#include <stdint.h>
#include <string.h>
#include "triceAbcReceive.h"
#include "triceAbcReceiveTest_abc.h"

static int pingCalls;
static int setPwmCalls;
static int setTimeCalls;
static int nestedOuterCalls;
static int nestedInnerCalls;
static int16_t pwm0;
static int pwmCnt;
static int32_t time0;
static uint32_t seenStamp32;
static uint16_t seenStamp16;
static uint8_t seenStampBits;
static uint32_t outerStampAfterNested;

static void resetAbcRxTestState(void) {
    pingCalls = 0;
    setPwmCalls = 0;
    setTimeCalls = 0;
    nestedOuterCalls = 0;
    nestedInnerCalls = 0;
    pwm0 = 0;
    pwmCnt = 0;
    time0 = 0;
    seenStamp32 = 0;
    seenStamp16 = 0;
    seenStampBits = 0;
    outerStampAfterNested = 0;
}

void ping(const triceAbcRx_t* rx) {
    pingCalls++;
    seenStampBits = rx->stampBits;
}

void set_pwm(const triceAbcRx_t* rx) {
    setPwmCalls++;
    pwmCnt = (int)(rx->payloadBytes / 2u);
    if (rx->payloadBytes >= 2u) {
        memcpy(&pwm0, rx->payload, sizeof(pwm0));
    }
    seenStampBits = rx->stampBits;
    seenStamp16 = (uint16_t)rx->stamp;
    seenStamp32 = rx->stamp;
}

void set_time(const triceAbcRx_t* rx) {
    setTimeCalls++;
    if (rx->payloadBytes >= 4u) {
        memcpy(&time0, rx->payload, sizeof(time0));
    }
}

void nested_inner(const triceAbcRx_t* rx) {
    nestedInnerCalls++;
}

void nested_outer(const triceAbcRx_t* rx) {
    const triceAbcRx_t* outerRx = rx;
    nestedOuterCalls++;
    (void)TriceAbcOnReceive(1005u, 32u, 0x22222222u, 0, 0u);
    outerStampAfterNested = outerRx->stamp;
}

int TriceAbcReceiveTest(int tc) {
    resetAbcRxTestState();

    switch (tc) {
    case 1: {
        if (TriceAbcOnReceive(9999u, 0u, 0u, 0, 0u) != TRICE_ABC_RX_IGNORED) return 1;
        if (pingCalls != 0) return 2;
        return 0;
    }
    case 2: {
        if (TriceAbcOnReceive(1001u, 0u, 0u, 0, 0u) != TRICE_ABC_RX_EXECUTED) return 1;
        if (pingCalls != 1 || seenStampBits != 0u) return 2;
        return 0;
    }
    case 3: {
        int16_t pwm[2] = { 123, 456 };
        if (TriceAbcOnReceive(1002u, 16u, 0xeb61u, (const uint8_t*)pwm, sizeof(pwm)) != TRICE_ABC_RX_EXECUTED) return 1;
        if (setPwmCalls != 1 || pwmCnt != 2 || pwm0 != 123) return 2;
        if (seenStampBits != 16u || seenStamp16 != 0xeb61u || seenStamp32 != 0xeb61u) return 3;
        return 0;
    }
    case 4: {
        int32_t t[1] = { 1777777777 };
        if (TriceAbcOnReceive(1003u, 32u, 0xc0de3020u, (const uint8_t*)t, sizeof(t)) != TRICE_ABC_RX_EXECUTED) return 1;
        if (setTimeCalls != 1 || time0 != 1777777777) return 2;
        return 0;
    }
    case 5: {
        uint8_t bad[3] = { 1, 2, 3 };
        if (TriceAbcOnReceive(1003u, 32u, 0u, bad, sizeof(bad)) != TRICE_ABC_RX_BAD_PAYLOAD) return 1;
        if (setTimeCalls != 0) return 2;
        return 0;
    }
    case 6: {
        if (TriceAbcOnReceive(1002u, 16u, 0u, 0, 2u) != TRICE_ABC_RX_BAD_PAYLOAD) return 1;
        if (setPwmCalls != 0) return 2;
        return 0;
    }
    case 7: {
        if (TriceAbcOnReceive(1001u, 7u, 0u, 0, 0u) != TRICE_ABC_RX_BAD_PAYLOAD) return 1;
        return 0;
    }
    case 8: {
        if (TriceAbcOnReceive(1004u, 32u, 0x11111111u, 0, 0u) != TRICE_ABC_RX_EXECUTED) return 1;
        if (nestedOuterCalls != 1 || nestedInnerCalls != 1) return 2;
        if (outerStampAfterNested != 0x11111111u) return 3;
        return 0;
    }
    default:
        return 999;
    }
}
```

The matching Go test calls `C.TriceAbcReceiveTest(n)` for all test cases and expects `0`.

**Transfer simulation:** none. These tests call `TriceAbcOnReceive()` directly with already decoded values. This is intentional: receive runtime correctness must not depend on UART, COBS, TCOBS, XTEA, or the host display tool.

**What this proves:**

- unknown IDs are ignored,
- known no-payload IDs call context handlers with zero payload bytes,
- known payload IDs pass the original decoded payload byte pointer and byte length,
- bad payload sizes do not call handlers,
- `payload == 0` with non-zero payload is rejected,
- stamp metadata is available through the receive context,
- nested receive calls do not mutate the outer receive context,
- generated table wrappers work with the uniform handler type.

### 18.6. <a id="receive-only-dependency-test"></a>Receive-only dependency test

**Purpose:** prove that an ABC receive-only target can be built without the Trice transmit/output library.

**Location:** add a host build test next to the receive runtime tests, for example:

```text
_test/abc_rx_only_build/
    cgo_test.go
    triceConfig.h
_test/testdata/triceAbcReceiveOnlyTest.c
_test/testdata/triceAbcReceiveOnlyTest_abc.h
_test/testdata/triceAbcReceiveOnlyTest_abc.c
```

**Configuration:**

```c
#define TRICE_ABC_RECEIVE_SUPPORT 1
#define TRICE_ABC_TRANSMIT_SUPPORT 0
#define TRICE_LEGACY_RPC_SUPPORT 0
#define TRICE_OFF 1 /* allowed if needed to suppress unrelated send macros */
```

**Build rule:** compile the receive test without normal Trice output implementation files such as UART/RTT transfer code, double/ring/static output buffer implementation, and ABC transmit macro implementation objects. The exact file list depends on the final source layout, but the test shall fail if `TriceAbcOnReceive()` requires `TriceStamp16`, `TriceStamp32`, `TriceTransfer`, output buffers, or a physical output backend.

**Input:** direct calls to `TriceAbcOnReceive()` as in the host-native receive runtime test.

**Expected result:** the test links and all receive cases pass.

**What this proves:** ABC receive is a standalone command-handler primitive. A device can receive and execute ABC commands without being a Trice logger or ABC sender.

### 18.7. <a id="receive-tests-under-different-build-configurations"></a>Receive tests under different build configurations

The first receive runtime is transport-independent. It should be compiled and executed under a small matrix of build configurations to catch accidental dependencies:

| Test package                                                                 | Important defines                                                                | Purpose                                                      |
|------------------------------------------------------------------------------|----------------------------------------------------------------------------------|--------------------------------------------------------------|
| `abc_rx_only_build`                                                          | `TRICE_ABC_RECEIVE_SUPPORT=1`, `TRICE_ABC_TRANSMIT_SUPPORT=0`, no output backend | standalone receive core                                      |
| `abc_rx_plain`                                                               | `TRICE_ABC_RECEIVE_SUPPORT=1`, no XTEA                                           | minimal receive runtime with normal host-test environment    |
| `abc_rx_xtea_build`                                                          | `TRICE_ABC_RECEIVE_SUPPORT=1`, XTEA enabled                                      | receive runtime coexists with encrypted-output configuration |
| `abc_rx_be_or_reverse` if an existing big-endian/reverse config is available | matching existing endianness defines                                             | compile-time coverage for byte-order variants                |

Expected result for all: the same `TriceAbcReceiveTest(n)` cases return `0`.

Do not claim that `abc_rx_xtea_build` proves inbound encrypted ABC receive. It proves only that ABC receive runtime compiles and behaves the same when XTEA support is enabled in the target build. Inbound encrypted receive requires an additional decoder-to-ABC integration.

### 18.8. <a id="optional-target-side-inbound-decode-integration-test"></a>Optional target-side inbound decode integration test

This is optional for the first ABC implementation. Add it only if there is a target-side API that can consume a raw incoming Trice byte stream, deframe/decrypt/decode it, and call `TriceAbcOnReceive()`.

**Proposed artifact if such an API exists later:**

```text
_test/abc_rx_stream_xtea_cobs/
    triceConfig.h
    cgo_test.go
_test/testdata/triceAbcStreamRxTest.c
```

**Input:** an encrypted and framed byte sequence that represents a known ABC command. It can be generated by the existing target transmit path or by a small Go helper using the same Trice encoding rules.

**Operation:**

1. Feed the raw encrypted/framed bytes to the target-side receive decoder.
2. The receive decoder decrypts/deframes/decodes.
3. The integration layer calls `TriceAbcOnReceive(id, stampBits, stamp, payload, payloadBytes)`.
4. Test handlers record calls and values.
5. The Go test checks the recorded values.

**Expected result:** same handler effects as the direct `TriceAbcOnReceive()` tests.

**What this proves:** inbound transport integration. It is not required for the ABC core if no such receive decoder exists yet.

### 18.9. <a id="legacy-tests"></a>Legacy tests

Required tests:

- Existing `triceF` macro behavior is unchanged when `TRICE_LEGACY_RPC_SUPPORT == 1`.
- Legacy `triceF` target code is absent or unusable when `TRICE_LEGACY_RPC_SUPPORT == 0`.
- Existing `trice generate -rpcH -rpcC` behavior is unchanged.
- ABC generation ignores `F` entries.
- Documentation/help marks the legacy path as deprecated.
- No compile-time deprecation warning is emitted merely because `TRICE_LEGACY_RPC_SUPPORT == 1`.

Recommended artifact split:

- keep existing `triceF` transmit/display tests in configurations that explicitly set `TRICE_LEGACY_RPC_SUPPORT 1`,
- add one negative compile test or preprocessor test proving `triceF` declarations are not available when `TRICE_LEGACY_RPC_SUPPORT 0`.

### 18.10. <a id="end-to-end-generatorbuild-example-test"></a>End-to-end generator/build example test

Add one small buildable example or test fixture that exercises the complete generator-to-receive flow without hardware.

Recommended location:

```text
_test/abc_flow/
    cgo_test.go or shell-driven test
    triceConfig.h
    abc_flow_source.c
    deviceA_handlers.c
```

**Input source before insert:**

```c
#include "trice.h"

void AbcFlowSource(void) {
    uint32_t t = 1777777777u;
    TRiceC("cmd:get_power_state", 0xc0de0001u);
    trice32C("cmd:set_time", &t, 1);
}
```

**Test operation:**

```bash
trice insert -src . -i abc_flow_til.json
trice generate -i abc_flow_til.json -abc=deviceA
# prune deviceA_abc.h or replace it with a fixture selecting only get_power_state
trice generate -i abc_flow_til.json -abc=deviceA
cc ... deviceA_abc.c deviceA_handlers.c abc_receive_test.c ...
./abc_receive_test
```

**Expected result:**

- first generate creates `deviceA_abc.h`,
- existing `deviceA_abc.h` is not overwritten,
- pruning `deviceA_abc.h` changes generated table contents,
- `deviceA_abc.c` compiles,
- selected handlers link and execute,
- missing selected handlers produce linker errors in a negative test if practical.

This fixture doubles as user-facing reference code.

## 19. <a id="target-code-examples"></a>Target-code examples

Examples should be complete enough that a user can copy the structure into a project. Keep them small; ABC is a primitive, not a framework.

### 19.1. <a id="example-request-power-state-from-a-swarm"></a>Example: request power state from a swarm

This example shows the intended application pattern:

1. A requester broadcasts `cmd:get_power_state` with an application-defined 32-bit stamp.
2. Devices that selected `get_power_state` execute their local handler.
3. Each responding device emits `rsp:power_state` using a stamp that identifies the responder and keeps the request sequence.
4. Any device that selected `power_state` can collect responses.

#### 19.1.1. <a id="sender-command"></a>Sender command

`requester.c`:

```c
#include <stdint.h>
#include "trice.h"

#define REQUESTER_DEVICE_ID 0x1234u

static uint16_t abcSeq;

void AskPowerState(void) {
    uint32_t stamp = ((uint32_t)REQUESTER_DEVICE_ID << 16) | (uint32_t)abcSeq++;

    // Clean source form. `trice insert` will add iD(n).
    TRiceC("cmd:get_power_state", stamp);
}
```

After `trice insert`, the source becomes conceptually:

```c
TRiceC(iD(14236), "cmd:get_power_state", stamp);
```

The numeric ID is stored in `til.json` and is the key used by receivers.

#### 19.1.2. <a id="receiver-selection"></a>Receiver selection

Run:

```bash
trice generate -i til.json -abc=deviceA
```

On first run the generator creates `deviceA_abc.h`. The user keeps only commands this target shall handle:

```c
#ifndef DEVICEA_ABC_H_
#define DEVICEA_ABC_H_

#include "triceAbcReceive.h"

void get_power_state(const triceAbcRx_t* rx);

#endif /* DEVICEA_ABC_H_ */
```

The generator also creates or updates `deviceA_abc.c`, which contains the ID-to-wrapper table. The user does not edit `deviceA_abc.c`.

#### 19.1.3. <a id="receiver-handler-implementation"></a>Receiver handler implementation

`deviceA_handlers.c`:

```c
#include <stdint.h>
#include "trice.h"
#include "deviceA_abc.h"

#define DEVICE_A_ID 0x8765u

static uint16_t BoardPowerMillivolt(void) {
    return 3710u;
}

void get_power_state(const triceAbcRx_t* rx) {
    uint32_t requestStamp = rx->stamp;
    uint16_t requestSeq = (uint16_t)requestStamp;
    uint16_t mv = BoardPowerMillivolt();

    // Response stamp: responder ID in the high 16 bits, request sequence in the low 16 bits.
    uint32_t responseStamp = ((uint32_t)DEVICE_A_ID << 16) | requestSeq;

    TRice16C("rsp:power_state", responseStamp, &mv, 1);
}
```

#### 19.1.4. <a id="response-collector"></a>Response collector

A device that wants to collect responses selects `power_state` in its own ABC header:

```c
#ifndef COLLECTOR_ABC_H_
#define COLLECTOR_ABC_H_

#include "triceAbcReceive.h"

void power_state(const triceAbcRx_t* rx);

#endif /* COLLECTOR_ABC_H_ */
```

`collector_handlers.c`:

```c
#include <stdint.h>
#include <string.h>
#include "trice.h"
#include "collector_abc.h"

#define MAX_POWER_RESPONSES 16

typedef struct {
    uint16_t deviceId;
    uint16_t millivolt;
} PowerResponse;

static PowerResponse powerResponses[MAX_POWER_RESPONSES];
static unsigned powerResponseCount;

void power_state(const triceAbcRx_t* rx) {
    int16_t millivolt;
    if (rx->payloadBytes != sizeof(millivolt) || powerResponseCount >= MAX_POWER_RESPONSES) {
        return;
    }
    memcpy(&millivolt, rx->payload, sizeof(millivolt));

    uint32_t stamp = rx->stamp;
    uint16_t sourceDevice = (uint16_t)(stamp >> 16);

    powerResponses[powerResponseCount].deviceId = sourceDevice;
    powerResponses[powerResponseCount].millivolt = (uint16_t)millivolt;
    powerResponseCount++;
}
```

ABC itself does not wait for responses and does not decide when enough responses arrived. The application may stop collecting after two responses, after a timeout, or by any other rule.

### 19.2. <a id="example-defer-work-in-the-user-handler"></a>Example: defer work in the user handler

The first ABC receive runtime calls handlers directly. If the receive integration runs in a context where real work is not allowed, the handler should only copy data and set a flag.

`motor_abc.h`:

```c
#ifndef MOTOR_ABC_H_
#define MOTOR_ABC_H_

#include "triceAbcReceive.h"

void motor_step(const triceAbcRx_t* rx);

#endif /* MOTOR_ABC_H_ */
```

`motor_handlers.c`:

```c
#include <stdint.h>
#include <string.h>
#include "trice.h"
#include "motor_abc.h"

static volatile int motorStepPending;
static int8_t motorStep[4];

void motor_step(const triceAbcRx_t* rx) {
    if (rx->payloadBytes != sizeof(motorStep)) {
        return;
    }

    memcpy(motorStep, rx->payload, sizeof(motorStep));
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

### 19.3. <a id="example-host-native-receive-runtime-test"></a>Example: host-native receive runtime test

This example is both documentation and a template for [Host-native C receive runtime tests](#host-native-c-receive-runtime-tests).

`test_abc.h`:

```c
#ifndef TEST_ABC_H_
#define TEST_ABC_H_

#include "triceAbcReceive.h"

void set_pwm(const triceAbcRx_t* rx);
void ping(const triceAbcRx_t* rx);

#endif /* TEST_ABC_H_ */
```

`test_abc.c`, generated shape:

```c
#include "test_abc.h"
#include "triceAbcReceive.h"

static void triceAbcCall_set_pwm(const triceAbcRx_t* rx) {
    set_pwm(rx);
}

static void triceAbcCall_ping(const triceAbcRx_t* rx) {
    ping(rx);
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
#include <string.h>
#include "triceAbcReceive.h"
#include "test_abc.h"

static int setPwmCalls;
static int pingCalls;
static int16_t pwm0;
static uint32_t seenStamp;
static uint8_t seenStampBits;

void set_pwm(const triceAbcRx_t* rx) {
    if (rx->payloadBytes == 4u) {
        setPwmCalls++;
        memcpy(&pwm0, rx->payload, sizeof(pwm0));
        seenStamp = rx->stamp;
        seenStampBits = rx->stampBits;
    }
}

void ping(const triceAbcRx_t* rx) {
    pingCalls++;
}

int main(void) {
    int16_t pwm[2] = { 123, 456 };

    if (TriceAbcOnReceive(9999u, 0u, 0u, 0, 0u) != TRICE_ABC_RX_IGNORED) {
        return 1;
    }

    if (TriceAbcOnReceive(1001u, 32u, 0xc0de3020u, (const uint8_t*)pwm, sizeof(pwm)) != TRICE_ABC_RX_EXECUTED) {
        return 2;
    }

    if (setPwmCalls != 1 || pwm0 != 123 || seenStamp != 0xc0de3020u || seenStampBits != 32u) {
        return 3;
    }

    if (TriceAbcOnReceive(1002u, 0u, 0u, 0, 0u) != TRICE_ABC_RX_EXECUTED) {
        return 4;
    }

    if (pingCalls != 1) {
        return 5;
    }

    return 0;
}
```

Compile this with the Trice target sources and a `triceConfig.h` that sets `TRICE_ABC_RECEIVE_SUPPORT 1`.

### 19.4. <a id="example-encrypted-transmit-remains-ordinary-trice-transport"></a>Example: encrypted transmit remains ordinary Trice transport

ABC does not need special encryption logic. The sender emits an ABC Trice record. The normal Trice output path then frames and optionally encrypts the byte stream according to `triceConfig.h`.

Example transmit configuration fragment:

```c
#define TRICE_ABC_TRANSMIT_SUPPORT 1
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS
```

Example sender:

```c
#include <stdint.h>
#include "trice.h"

void SendEncryptedAbcExample(void) {
    uint32_t stamp = 0xc0de3020u;
    int32_t payload[2] = { 17, 23 };

    TRice32C("cmd:encrypted_example", stamp, payload, 2);
}
```

In the existing CGO transmit tests, the generated byte stream is read from the shared buffer and passed to the Trice tool with matching options, for example password and package framing. If the decoded output matches the expected `//exp:` string, the test proves that ABC messages survive the configured XTEA/framing path.

This example does not imply inbound encrypted ABC receive. Inbound encrypted receive requires a decoder integration that converts encrypted/framed bytes into decoded `TriceAbcOnReceive()` calls.

## 20. <a id="recommended-implementation-order"></a>Recommended implementation order

Implement in small steps and add tests after each step.

1. Keep legacy `triceF` / `-rpcH` / `-rpcC` behavior unchanged and add documentation/help deprecation wording.
2. Add `TRICE_ABC_TRANSMIT_SUPPORT`, `TRICE_ABC_RECEIVE_SUPPORT`, and `TRICE_LEGACY_RPC_SUPPORT` defaults.
3. Gate existing legacy `triceF` target code behind `TRICE_LEGACY_RPC_SUPPORT` without changing legacy generator behavior.
4. Add parser recognition for ABC macro families and `_C` explicit forms.
5. Add ABC TIL classification, bit-width detection, stamp-width detection, and command-name extraction.
6. Add `trice insert` / `trice clean` behavior for ABC macros using `iD(n)`.
7. Add parser/TIL tests from [Parser, insert, clean, and TIL tests](#parser-insert-clean-and-til-tests).
8. Implement `trice generate -abc=<target>` first-run selection-header generation.
9. Implement existing-header parsing and missing-ID warnings.
10. Implement `<target>_abc.c` generation with wrappers, table, deduplication, and conflict errors.
11. Add generator workflow tests from [18.2. Generator workflow tests for -abc=deviceA](#generator-workflow-tests-for--abcdevicea).
12. Add no-stamp ABC send macros using existing counted-buffer logic where possible.
13. Add stamped ABC send macros using `TRICE_C_STAMP16` and `TRICE_C_STAMP32`.
14. Add ABC transmit cases to the CGO target-output tests, including at least one XTEA configuration.
15. Add direct ABC receive runtime with `triceAbcRx_t` handler context.
16. Add host-native receive runtime tests from [Host-native C receive runtime tests](#host-native-c-receive-runtime-tests).
17. Add the receive-only dependency test from [Receive-only dependency test](#receive-only-dependency-test).
18. Add the small end-to-end generator/build example test from [End-to-end generator/build example test](#end-to-end-generatorbuild-example-test).
18. Add host display refinement from `time:` to `stamp:` for ABC entries if desired.
19. Add optional inbound stream decode integration tests only if such an integration is implemented.

## 21. <a id="follow-up-items"></a>Follow-up items

The following items are not required for the first implementation:

- TIL merge/import tooling.
- Queued or deferred ABC receive helpers.
- Optional RTOS/thread-local dispatch metadata if a later API needs it.
- Optimized lookup beyond linear search.
- Mixed-endian ABC receive normalization.
- Target-side inbound Trice stream decoder integration for encrypted/framed ABC receive.
- Support for `const intN_t*`, `uintN_t*`, typedef-based declarations, or custom parameter names in `<target>_abc.h`.
- Default-width ABC payload form for `triceC`.
- Removal of legacy `triceF`, `-rpcH`, or `-rpcC`.

## 22. <a id="non-negotiable-constraints"></a>Non-negotiable constraints

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
- ABC receive core must be buildable without ABC transmit support, normal Trice output buffers, transfer backends, or `TriceStamp16`/`TriceStamp32` dependencies.
- ABC receive tests call `TriceAbcOnReceive()` with decoded ID/stamp/payload data; encrypted/framed inbound stream receive is a separate integration layer.
- ABC is a primitive, not a full RPC or distributed-systems framework.

## 23. <a id="appendix-buffer-based-abc-rx-discussion-result"></a>Appendix: Buffer-based ABC RX discussion result

This appendix captures a later design discussion result. It is appended intentionally without rewriting the earlier sections.

### 23.1. Agreed target direction

The preferred first-class ABC receive API direction is a buffer-based entry point that parses one already decoded Trice binary record directly from a caller-provided buffer:

```c
int TriceAbcOnReceive(const uint8_t* pBuf, int len);
```

The return value semantics should be:

- `> 0`: success; this many bytes were consumed from `pBuf`
- `0`: the record was structurally valid ABC input but the ID was not selected locally in `triceAbc[]`
- `< 0`: parse or validation error

This allows retry on short buffers in `no package framing` style integrations where not all bytes may have arrived yet.

### 23.2. Intended internal processing order

The preferred parsing/dispatch sequence is:

1. `getSelector(...)`
2. derive `stampBits` from the selector (`0`, `16`, or `32`)
3. `getID(...)`
4. search `id` in `triceAbc[]`; return `0` if not found
5. get expected `bitWidth` from the found `triceAbc[]` entry
6. `getStamp(...)`
7. `getCount(...)`
8. check `len`; if too small, return a negative error value
9. fill a stack-local `triceAbcRx_t abc`
10. call the selected user handler `fn_ptr(&abc)`
11. return the consumed byte count

The parsing helpers are generic runtime helpers, not generated per target.

### 23.3. Source of bit width

The payload element width is a generated local property and should come from `triceAbc[]`, not from the received buffer.

Therefore a helper such as `triceAbcPayloadElementSize()` is considered unnecessary in this model. The runtime can derive the element size directly from the generated `bitWidth` value, for example with `bitWidth / 8`.

### 23.4. Handler context object

The runtime should fill a stack-local `triceAbcRx_t` instance containing only metadata and a payload pointer:

- `id`
- `stamp`
- `stampBits`
- `bitWidth`
- `payload`
- `payloadBytes`

The payload bytes themselves should not be copied into a temporary stack buffer. `abc.payload` points directly into `pBuf`. A handler that needs the data later must copy it on its own side.

### 23.5. Handler signature and generated table

The preferred handler signature is:

```c
void Handler(const triceAbcRx_t* rx);
```

With that uniform signature, generator-created wrapper functions are considered unnecessary. The generated dispatch table can point directly to the user handlers.

The generator-owned files should be clearly recognizable as generator output. In particular, `<target>_abc.h` and `<target>_abc.c` in tests should look exactly like normal `trice generate -abc=<target>` results, including their file comments and recognizable generated structure.

The generated table shape is expected to stay simple:

```c
const triceAbc_t triceAbc[] = {
    /* TRIce32_C */ { 15374u, 32u, FunctionNameYa }
};

const unsigned triceAbcElements = sizeof(triceAbc) / sizeof(triceAbc[0]);
```

### 23.6. Runtime placement

The generic ABC receive runtime should stay separate from the generated `<target>_abc.c` data. The target-specific generated files should provide selection and dispatch data, while the parsing and validation logic should remain shared runtime code.

This separation keeps one fix in one place and still allows "ABC receive only" devices to compile with a small dedicated ABC receive module instead of the larger normal Trice output stack.
