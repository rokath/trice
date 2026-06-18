

# Trice RX, Trice ABC, and Future C `tlog` Architecture Proposal

Status: design discussion draft.  
Scope: no implementation in this document.  
Primary target language for Trice ABC and Trice RX: **C**.  
Future `tlog` implementation may use C++ internally if that later proves useful, but Trice ABC should remain plain C.

---
<h2>Table of Contents</h2>
<!-- mdtoc -->

- [1. Purpose](#purpose)
- [2. Current WIP Starting Point](#current-wip-starting-point)
  - [2.1. Existing ABC receive draft](#existing-abc-receive-draft)
  - [2.2. Existing configuration state](#existing-configuration-state)
  - [2.3. Existing C decoder pieces](#existing-c-decoder-pieces)
  - [2.4. Existing Go tlog decoder](#existing-go-tlog-decoder)
- [3. Design Principles](#design-principles)
  - [3.1. Plain C for Trice ABC](#plain-c-for-trice-abc)
  - [3.2. RX must be independent of TX](#rx-must-be-independent-of-tx)
  - [3.3. Use one common receive record type](#use-one-common-receive-record-type)
  - [3.4. Parsing is not resolving](#parsing-is-not-resolving)
  - [3.5. No payload copy by default](#no-payload-copy-by-default)
  - [3.6. Use triceDefaultConfig.h as the central default layer](#use-tricedefaultconfigh-as-the-central-default-layer)
  - [3.7. Keep BcSim protocol-neutral](#keep-bcsim-protocol-neutral)
- [4. Proposed High-Level Layering](#proposed-high-level-layering)
- [5. Proposed Core Types](#proposed-core-types)
  - [5.1. triceRx_t](#tricerx_t)
  - [5.2. triceAbcEntry_t](#triceabcentry_t)
  - [5.3. triceLogEntry_t](#tricelogentry_t)
- [6. Proposed Function APIs](#proposed-function-apis)
  - [6.1. Parse one decoded record](#parse-one-decoded-record)
  - [6.2. Resolve ABC metadata](#resolve-abc-metadata)
  - [6.3. Resolve log metadata](#resolve-log-metadata)
  - [6.4. Future location resolver](#future-location-resolver)
  - [6.5. Dispatch ABC handler](#dispatch-abc-handler)
- [7. Configuration Model](#configuration-model)
  - [7.1. Rename ABC-specific support switches](#rename-abc-specific-support-switches)
  - [7.2. Log switches](#log-switches)
  - [7.3. Bit width unknown](#bit-width-unknown)
  - [7.4. Framing and XTEA receive settings](#framing-and-xtea-receive-settings)
  - [7.5. Example-specific bus configuration](#example-specific-bus-configuration)
- [8. Record Parser Details](#record-parser-details)
  - [8.1. Transfer order](#transfer-order)
  - [8.2. Selector interpretation](#selector-interpretation)
  - [8.3. Consumed bytes](#consumed-bytes)
  - [8.4. Buffer-too-short](#buffer-too-short)
  - [8.5. Payload validation](#payload-validation)
- [9. Resolver Details](#resolver-details)
  - [9.1. Resolver input tables use explicit counts](#resolver-input-tables-use-explicit-counts)
  - [9.2. Bit-width conflict rule](#bit-width-conflict-rule)
- [10. Trice ABC Migration Plan](#trice-abc-migration-plan)
  - [10.1. Stage A: Introduce generic RX types without breaking ABC](#stage-a-introduce-generic-rx-types-without-breaking-abc)
  - [10.2. Stage B: Extract parsing](#stage-b-extract-parsing)
  - [10.3. Stage C: Add ABC resolver and dispatcher](#stage-c-add-abc-resolver-and-dispatcher)
  - [10.4. Stage D: Adapt TriceAbcOnReceive() as compatibility wrapper](#stage-d-adapt-triceabconreceive-as-compatibility-wrapper)
  - [10.5. Stage E: Retire ABC-specific receive type](#stage-e-retire-abc-specific-receive-type)
- [11. Future C tlog Architecture](#future-c-tlog-architecture)
- [12. Example Tree Direction](#example-tree-direction)
  - [12.1. Node suffixes](#node-suffixes)
  - [12.2. BcSim remains neutral](#bcsim-remains-neutral)
- [13. Test Strategy](#test-strategy)
  - [13.1. Parser unit tests](#parser-unit-tests)
  - [13.2. Resolver tests](#resolver-tests)
  - [13.3. ABC dispatch tests](#abc-dispatch-tests)
  - [13.4. Receive-only dependency test](#receive-only-dependency-test)
  - [13.5. Existing transmit tests](#existing-transmit-tests)
  - [13.6. Frame/decrypt tests](#framedecrypt-tests)
  - [13.7. BcSim tests](#bcsim-tests)
  - [13.8. End-to-end demo tests](#end-to-end-demo-tests)
- [14. Recommended Implementation Order](#recommended-implementation-order)
- [15. Risks and Design Warnings](#risks-and-design-warnings)
  - [15.1. Risk: doing full C tlog too early](#risk-doing-full-c-tlog-too-early)
  - [15.2. Risk: coupling RX to TX accidentally](#risk-coupling-rx-to-tx-accidentally)
  - [15.3. Risk: XTEA compile conditions](#risk-xtea-compile-conditions)
  - [15.4. Risk: conflating parse and resolve](#risk-conflating-parse-and-resolve)
  - [15.5. Risk: treating ABC follow-up commands as protocol-level responses](#risk-treating-abc-follow-up-commands-as-protocol-level-responses)
  - [15.6. Risk: payload lifetime](#risk-payload-lifetime)
- [16. Open Decisions](#open-decisions)
- [17. Suggested Near-Term Acceptance Criteria](#suggested-near-term-acceptance-criteria)
- [18. Short Recommendation](#short-recommendation)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->
---

## 1. <a id="purpose"></a>Purpose

The near-term goal is to implement a robust, tested **Trice RX** foundation in C and use it to complete **Trice ABC** receive functionality. The longer-term goal is a standalone C/C++ `tlog` implementation that can run on systems where Go is unavailable or undesirable, for example QNX, custom embedded Linux variants, or even constrained bare-metal environments.

The immediate implementation focus should not be TCP/IP, UDP, RTT, GUI code, or a full C `tlog`. The first useful milestone is a clean, testable C receive path that can:

1. receive bytes through a user-supplied read function,
2. handle framing/decryption layers in a modular way,
3. parse decoded Trice records into a common receive structure,
4. resolve IDs against generated ABC and/or log metadata,
5. dispatch ABC handlers or feed a later C log formatter,
6. be compiled in receive-only configurations without pulling in transmit/output code.

This paper proposes the software architecture, the module boundaries, the configuration model, and a staged test strategy.

---

## 2. <a id="current-wip-starting-point"></a>Current WIP Starting Point

The `wip` branch already contains several important pieces that should be treated as prototypes or inputs, not as final architecture.

### 2.1. <a id="existing-abc-receive-draft"></a>Existing ABC receive draft

`src/triceAbcReceive.h` currently defines `triceAbcRx_t`, `triceAbcHandler_t`, `triceAbc_t`, `triceAbc[]`, `triceAbcElements`, and `TriceAbcOnReceive()` under `TRICE_ABC_RECEIVE_SUPPORT`. It is intentionally small and independent from normal Trice output.

`src/triceAbcReceive.c` currently performs several responsibilities in one place:

- reads 16- and 32-bit values in configured Trice transfer order,
- parses one decoded Trice record,
- handles selector/stamp fields,
- computes payload byte length,
- validates payload length against the selected ABC table bit width,
- finds an ABC handler table entry,
- calls the selected handler directly.

This is a good proof of concept, but the responsibilities should be split before the code becomes the basis for ABC and future C `tlog`.

### 2.2. <a id="existing-configuration-state"></a>Existing configuration state

`src/triceDefaultConfig.h` already has many relevant defaults, including normal transmit/output settings, framing settings, XTEA settings, transfer-order handling, `TRICE_LEGACY_RPC_SUPPORT`, and currently also `TRICE_ABC_TRANSMIT_SUPPORT` / `TRICE_ABC_RECEIVE_SUPPORT`.

The next architecture should avoid local fallback defaults inside feature headers when the value belongs in `triceDefaultConfig.h`. Feature headers may defensively default very narrow constants only if they are deliberately usable without the Trice default config, but global feature switches should have one central default location.

### 2.3. <a id="existing-c-decoder-pieces"></a>Existing C decoder pieces

The repository already contains C code for `COBSDecode()`, `TCOBSDecode()`, and XTEA encryption/decryption. These should be reused where possible, but receive-only use must not accidentally depend on transmit backends.

One issue to watch: the current XTEA implementation is compiled under conditions that include active backend/transmit-related switches. A future receive-only decrypt path should be separable from direct/deferred transmit encryption.

### 2.4. <a id="existing-go-tlog-decoder"></a>Existing Go `tlog` decoder

The Go decoder stack is the tested reference for stream handling, byte order, TIL lookups, format-string handling, and output behavior. The Go package has a `Decoder` interface and `DecoderData` with an input reader, unprocessed input buffer, decoded package buffers, endian-aware `ReadU16`/`ReadU32`/`ReadU64`, LUT access, location information, and formatting helpers.

The C architecture should not blindly port the Go code one-to-one. Instead, it should copy the proven layering:

```text
byte input -> deframe/decrypt -> parse record -> resolve ID metadata -> consume record
```

---

## 3. <a id="design-principles"></a>Design Principles

### 3.1. <a id="plain-c-for-trice-abc"></a>Plain C for Trice ABC

Trice ABC and the receive core should remain plain C. This keeps it compatible with the existing target code style and with constrained toolchains. C++ can later be considered inside a standalone C/C++ `tlog` program, but C++ should not become a dependency for Trice ABC target-side code.

### 3.2. <a id="rx-must-be-independent-of-tx"></a>RX must be independent of TX

Receive-only devices are a real use case. A target that only receives Trice ABC commands should not need:

- `trice()` / `Trice()` / `TRice()` send macros,
- Trice output buffers,
- UART/RTT/auxiliary transmit backends,
- `TriceStamp16` or `TriceStamp32`,
- ABC transmit macros.

It may need small shared definitions, transfer-order helpers, optional frame decoders, and generated receive metadata.

### 3.3. <a id="use-one-common-receive-record-type"></a>Use one common receive record type

The current ABC-specific `triceAbcRx_t` should be replaced by a more general `triceRx_t`. ABC is one consumer of received Trice records; log output is another. The record parser should not be ABC-specific.

### 3.4. <a id="parsing-is-not-resolving"></a>Parsing is not resolving

A parser can read what is physically present in a Trice record: ID, selector/stamp, payload pointer, and payload byte length. It cannot know the `bitWidth`, `pFmt`, location, or handler function without an ID lookup.

Therefore:

```text
parse   -> fill fields from bytes only
resolve -> fill metadata from generated tables
consume -> dispatch ABC handler or format log output
```

### 3.5. <a id="no-payload-copy-by-default"></a>No payload copy by default

`triceRx_t.payload` should point into the interpreted input buffer. This avoids RAM use and copies. The pointer is valid only until the input buffer is overwritten or freed.

Immediate ABC handlers can read it directly. Any delayed handler must copy the payload if it needs it later.

### 3.6. <a id="use-tricedefaultconfigh-as-the-central-default-layer"></a>Use `triceDefaultConfig.h` as the central default layer

Compile-time switches belong in `triceDefaultConfig.h` unless they are strictly private to a local example or test. Example-specific `triceConfig.h` files should only define deliberate overrides.

### 3.7. <a id="keep-bcsim-protocol-neutral"></a>Keep BcSim protocol-neutral

`examples/TriceAbc/BcSim` should stay a protocol-neutral broadcast simulation. It should not mention or depend on Trice, ABC, TCOBS, COBS, XTEA, IDs, stamps, or format strings. Trice-specific use of BcSim belongs in `examples/TriceAbc/ReadMe.md` and in the node/demo code above BcSim.

---

## 4. <a id="proposed-high-level-layering"></a>Proposed High-Level Layering

The future receive stack should be decomposed into the following layers.

```text
+---------------------------------------------------------------+
| Application layer                                              |
|   - ABC handlers                                               |
|   - log formatter                                              |
|   - demo node behavior                                         |
+---------------------------------------------------------------+
| Resolver layer                                                 |
|   - triceResolveAbc()                                          |
|   - triceResolveLog()                                          |
|   - future triceResolveLocation()                              |
+---------------------------------------------------------------+
| Record parser                                                  |
|   - triceParseNextRecord()                                     |
|   - parses one decoded Trice record                            |
+---------------------------------------------------------------+
| Frame/decrypt layer                                            |
|   - none                                                       |
|   - COBS                                                       |
|   - TCOBS/TCOBSv1                                             |
|   - optional XTEA decrypt                                      |
+---------------------------------------------------------------+
| Byte source                                                    |
|   - user read callback                                         |
|   - file, memory, UART, CAN, BcSim, test fixture, etc.          |
+---------------------------------------------------------------+
```

TCP/IP, UDP, RTT, and GUI transports are not needed for the first RX/ABC milestone. They can later be implemented as byte sources or byte sinks without changing the parser/resolver design.

---

## 5. <a id="proposed-core-types"></a>Proposed Core Types

### 5.1. <a id="tricerx_t"></a>`triceRx_t`

A single receive record type should live in `src/triceRx.h`.

Recommended shape:

```c
typedef struct triceRx_t {
    uint16_t id;
    uint8_t  bitWidth;
    uint8_t  stampBits;
    uint32_t stamp;

    const uint8_t* payload;
    uint16_t payloadBytes;

#if TRICE_LOG_ENABLE == 1
    const char* pFmt;
#endif

#if TRICE_LOG_WITH_LOCATION == 1
    const char* file;
    uint32_t line;
#endif

    void (*fn)(const struct triceRx_t* rx);
} triceRx_t;
```

Notes:

- `bitWidth` is part of the structure but is not filled by the parser.
- `bitWidth` starts as `TRICE_BIT_WIDTH_UNKNOWN`, recommended value `0xffu`.
- Normal bit widths are `0`, `8`, `16`, `32`, and `64`.
- `stampBits` is `0`, `16`, or `32`.
- `stamp` is normalized to 32-bit storage.
- `payload` points into the input buffer.
- `payloadBytes` is byte count, not element count.
- `pFmt` is present only when log resolution is enabled.
- `file` and `line` are present only when location resolution is enabled.
- `fn` is intentionally present even for logging-only use. Since `triceRx_t` is a transient stack/local object and not used in large arrays, the unused pointer is acceptable.

The self-referencing typedef can be written in the compact valid C form:

```c
typedef struct triceRx_t {
    /* fields */
    void (*fn)(const struct triceRx_t* rx);
} triceRx_t;
```

This avoids separate names such as `TriceRx` and `triceRx_t`.

### 5.2. <a id="triceabcentry_t"></a>`triceAbcEntry_t`

ABC metadata should be generated from `trice generate -abc path/target` and should contain ID, bit width, and function pointer.

```c
typedef void (*triceFn_t)(const triceRx_t* rx);

typedef struct {
    uint16_t id;
    uint8_t bitWidth;
    triceFn_t fn;
} triceAbcEntry_t;
```

This type can initially live in a receive/ABC header. Later it may become more general if both log and ABC metadata share a common table shape.

### 5.3. <a id="tricelogentry_t"></a>`triceLogEntry_t`

A future log resolver table may contain ID, bit width, and format string:

```c
typedef struct {
    uint16_t id;
    uint8_t bitWidth;
#if TRICE_LOG_ENABLE == 1
    const char* pFmt;
#endif
} triceLogEntry_t;
```

File/line should not be part of this resolver. A later `triceResolveLocation()` should fill location data from a generated/transformed location table derived from `li.json`.

---

## 6. <a id="proposed-function-apis"></a>Proposed Function APIs

### 6.1. <a id="parse-one-decoded-record"></a>Parse one decoded record

Recommended name:

```c
int triceParseNextRecord(triceRx_t* rx, const uint8_t* buf, size_t len);
```

Return value:

```text
>= 0   number of consumed bytes for the next record
<  0   negative error code
```

The function parses exactly one decoded Trice record at `buf[0]`. It does not parse multiple records, does not deframe, does not decrypt, and does not use a TIL table.

It fills:

```text
id
stampBits
stamp
payload
payloadBytes
```

It initializes or leaves as unknown:

```text
bitWidth = TRICE_BIT_WIDTH_UNKNOWN
fn = NULL
pFmt = NULL, if present
file = NULL, if present
line = 0, if present
```

Important: `payload` points into `buf`; no payload copy is made.

### 6.2. <a id="resolve-abc-metadata"></a>Resolve ABC metadata

```c
int triceResolveAbc(triceRx_t* rx, const triceAbcEntry_t* list, size_t count);
```

The resolver searches `list[0..count-1]` for `rx->id`. If found, it fills:

```text
rx->bitWidth
rx->fn
```

If not found, it should return a non-fatal “not found” code. Unknown IDs are normal in mixed streams.

If `rx->bitWidth` is still `TRICE_BIT_WIDTH_UNKNOWN`, the resolver sets it. If it is already set and differs from the table entry, the resolver returns a bit-width conflict error.

### 6.3. <a id="resolve-log-metadata"></a>Resolve log metadata

```c
int triceResolveLog(triceRx_t* rx, const triceLogEntry_t* list, size_t count);
```

The resolver searches for `rx->id`. If found, it fills:

```text
rx->bitWidth
rx->pFmt, if TRICE_LOG_ENABLE == 1
```

It does not fill file/line.

The same bit-width conflict rule applies.

### 6.4. <a id="future-location-resolver"></a>Future location resolver

Not part of the first milestone, but the design should reserve the concept:

```c
int triceResolveLocation(triceRx_t* rx, const triceLocationEntry_t* list, size_t count);
```

This will later fill:

```text
rx->file
rx->line
```

The data source would be generated or transformed from `li.json`. This is orthogonal to ABC and log formatting.

### 6.5. <a id="dispatch-abc-handler"></a>Dispatch ABC handler

A small helper can dispatch resolved ABC records:

```c
int triceDispatchAbc(const triceRx_t* rx);
```

Semantics:

- if `rx == NULL`: error,
- if `rx->fn == NULL`: ignored/not selected,
- if payload length is incompatible with `rx->bitWidth`: error,
- otherwise call `rx->fn(rx)` and return success.

This separates parsing, resolving, and dispatching. It also makes tests much easier.

---

## 7. <a id="configuration-model"></a>Configuration Model

### 7.1. <a id="rename-abc-specific-support-switches"></a>Rename ABC-specific support switches

The desired long-term names should be:

```c
#ifndef TRICE_TRANSMIT_SUPPORT
#define TRICE_TRANSMIT_SUPPORT 0
#endif

#ifndef TRICE_RECEIVE_SUPPORT
#define TRICE_RECEIVE_SUPPORT 0
#endif
```

Rationale:

There is no essential difference between “Trice transmit” and “Trice ABC transmit” at the stack level. Both use the Trice transmit stack. If a future mini-ABC-transmit path is implemented without the full Trice transmit stack, that special path can be enabled separately while `TRICE_TRANSMIT_SUPPORT == 0`.

The existing `TRICE_ABC_TRANSMIT_SUPPORT` / `TRICE_ABC_RECEIVE_SUPPORT` names can be kept temporarily as compatibility aliases, but new code should use the more general names.

Example transitional compatibility:

```c
#ifndef TRICE_TRANSMIT_SUPPORT
#ifdef TRICE_ABC_TRANSMIT_SUPPORT
#define TRICE_TRANSMIT_SUPPORT TRICE_ABC_TRANSMIT_SUPPORT
#else
#define TRICE_TRANSMIT_SUPPORT 0
#endif
#endif

#ifndef TRICE_RECEIVE_SUPPORT
#ifdef TRICE_ABC_RECEIVE_SUPPORT
#define TRICE_RECEIVE_SUPPORT TRICE_ABC_RECEIVE_SUPPORT
#else
#define TRICE_RECEIVE_SUPPORT 0
#endif
#endif
```

After migration, documentation should prefer only `TRICE_TRANSMIT_SUPPORT` and `TRICE_RECEIVE_SUPPORT`.

### 7.2. <a id="log-switches"></a>Log switches

Recommended defaults:

```c
#ifndef TRICE_LOG_ENABLE
#define TRICE_LOG_ENABLE 0
#endif

#ifndef TRICE_LOG_WITH_LOCATION
#define TRICE_LOG_WITH_LOCATION 0
#endif
```

`TRICE_LOG_ENABLE` controls `pFmt` and log resolver/formatter-related code.

`TRICE_LOG_WITH_LOCATION` controls `file` and `line` fields and location resolution.

Do not use older or ambiguous names such as `TRICE_WITH_FILE_LINE` or `TRICE_RECORD_WITH_LOG_META`.

### 7.3. <a id="bit-width-unknown"></a>Bit width unknown

```c
#ifndef TRICE_BIT_WIDTH_UNKNOWN
#define TRICE_BIT_WIDTH_UNKNOWN 0xffu
#endif
```

### 7.4. <a id="framing-and-xtea-receive-settings"></a>Framing and XTEA receive settings

The existing transmit/output framing settings are named around direct/deferred output. For receive, introduce receive-oriented settings instead of overloading output settings.

Recommended future defaults:

```c
#ifndef TRICE_RX_FRAMING
#define TRICE_RX_FRAMING TRICE_FRAMING_NONE
#endif

#ifndef TRICE_RX_XTEA_DECRYPT
#define TRICE_RX_XTEA_DECRYPT 0
#endif
```

Open naming question: whether to reuse `XTEA_DECRYPT` or introduce `TRICE_RX_XTEA_DECRYPT`. The existing `XTEA_DECRYPT` is already present and can be kept, but a `TRICE_RX_XTEA_DECRYPT` alias would be clearer for the receive pipeline.

### 7.5. <a id="example-specific-bus-configuration"></a>Example-specific bus configuration

For `examples/TriceAbc`, use node-specific `triceConfig.h` files and a root-level `triceBusConfig.h`.

Example:

```text
examples/TriceAbc/triceBusConfig.h
examples/TriceAbc/N1_tx/triceConfig.h
examples/TriceAbc/N4_rx/triceConfig.h
```

`triceBusConfig.h` contains bus-wide decisions:

```c
#define TRICE_BUS_FRAMING TRICE_FRAMING_TCOBS
/* future: XTEA, d16, transfer-order profile */
```

A node-specific config includes it and sets node capabilities:

```c
#include "../triceBusConfig.h"

#define TRICE_CLEAN 0
#define TRICE_TRANSMIT_SUPPORT 1
#define TRICE_RECEIVE_SUPPORT 0
#define TRICE_DIRECT_OUT_FRAMING TRICE_BUS_FRAMING
```

`TRICE_CLEAN` should be explicitly present in each node-specific `triceConfig.h`.

---

## 8. <a id="record-parser-details"></a>Record Parser Details

`triceParseNextRecord()` should be derived from the current `TriceAbcOnReceive()` parsing behavior, but it should not dispatch handlers.

### 8.1. <a id="transfer-order"></a>Transfer order

Use byte-wise reads controlled by `TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN`. Do not cast unaligned pointers to `uint16_t*` or `uint32_t*`.

```c
static uint16_t triceReadU16(const uint8_t* p);
static uint32_t triceReadU32(const uint8_t* p);
```

These helpers should be shared between record parser and tests.

### 8.2. <a id="selector-interpretation"></a>Selector interpretation

The first 16-bit word contains selector bits and 14-bit ID. The parser should extract:

```text
selector = firstWord >> 14
id       = firstWord & 0x3fff
```

Selector behavior:

```text
1 -> no stamp
2 -> 16-bit stamp
3 -> 32-bit stamp
0 -> typeX0 or unsupported, depending on config
```

Selector 0 should remain a deliberate policy point. If typeX0 support is enabled, it may represent counted user packets. If not, it should return a clear unsupported/bad-record code.

### 8.3. <a id="consumed-bytes"></a>Consumed bytes

The parser must return the logical record length. It should not consume alignment padding after a record. Alignment or multi-record stepping is the caller’s responsibility.

### 8.4. <a id="buffer-too-short"></a>Buffer-too-short

If the buffer does not contain enough bytes for the next complete record, return a negative buffer-too-short code. The caller must keep the current bytes and retry when more data arrives.

### 8.5. <a id="payload-validation"></a>Payload validation

The parser itself cannot validate payload alignment against `bitWidth`, because `bitWidth` is unknown until resolution.

Payload byte count is parsed structurally only. Later, `triceDispatchAbc()` or a validation helper checks:

- payload with `bitWidth == 0` is invalid,
- multi-byte widths require complete elements,
- `bitWidth == TRICE_BIT_WIDTH_UNKNOWN` cannot be dispatched.

---

## 9. <a id="resolver-details"></a>Resolver Details

### 9.1. <a id="resolver-input-tables-use-explicit-counts"></a>Resolver input tables use explicit counts

Generated lists are compile-time known, but resolver functions should still take a count:

```c
int triceResolveAbc(triceRx_t* rx, const triceAbcEntry_t* list, size_t count);
int triceResolveLog(triceRx_t* rx, const triceLogEntry_t* list, size_t count);
```

Rationale:

- no sentinel value needed,
- no global list length required,
- easier unit testing with small local arrays,
- no reserved dummy ID needed,
- generator does not need delimiter entries.

### 9.2. <a id="bit-width-conflict-rule"></a>Bit-width conflict rule

Use a shared helper internally:

```c
int triceSetBitWidth(triceRx_t* rx, uint8_t bitWidth);
```

Rules:

```text
rx->bitWidth == TRICE_BIT_WIDTH_UNKNOWN
    -> set to new value

rx->bitWidth == new value
    -> ok

rx->bitWidth != new value
    -> conflict error
```

This makes it possible to call both ABC and log resolvers on the same record during diagnostics or future `tlog` operation.

---

## 10. <a id="trice-abc-migration-plan"></a>Trice ABC Migration Plan

### 10.1. <a id="stage-a-introduce-generic-rx-types-without-breaking-abc"></a>Stage A: Introduce generic RX types without breaking ABC

Add `src/triceRx.h` and `src/triceRx.c` with:

- `triceRx_t`,
- `TRICE_BIT_WIDTH_UNKNOWN`,
- error codes,
- optional field guards,
- utility initialization function, if useful.

At this point, keep `triceAbcReceive.*` as-is except possibly including `triceRx.h` for shared constants.

### 10.2. <a id="stage-b-extract-parsing"></a>Stage B: Extract parsing

Add:

```c
int triceParseNextRecord(triceRx_t* rx, const uint8_t* buf, size_t len);
```

Move record parsing out of `TriceAbcOnReceive()` logic. Tests should prove that the new parser returns the same logical lengths and extracted fields as the current ABC receive tests expect.

### 10.3. <a id="stage-c-add-abc-resolver-and-dispatcher"></a>Stage C: Add ABC resolver and dispatcher

Add:

```c
int triceResolveAbc(triceRx_t* rx, const triceAbcEntry_t* list, size_t count);
int triceDispatchAbc(const triceRx_t* rx);
```

Make the generated ABC table use `triceRx_t` handler signature:

```c
void handler(const triceRx_t* rx);
```

### 10.4. <a id="stage-d-adapt-triceabconreceive-as-compatibility-wrapper"></a>Stage D: Adapt `TriceAbcOnReceive()` as compatibility wrapper

To avoid breaking current tests and users, keep:

```c
int TriceAbcOnReceive(const uint8_t* pBuf, int len);
```

but implement it as:

```c
triceRx_t rx;
int n = triceParseNextRecord(&rx, pBuf, len);
if (n < 0) return n;
if (triceResolveAbc(&rx, triceAbc, triceAbcElements) == TRICE_RX_NOT_FOUND) return n;
if (triceDispatchAbc(&rx) < 0) return error;
return n;
```

This wrapper preserves the current direct-dispatch model while moving the architecture toward generic RX.

### 10.5. <a id="stage-e-retire-abc-specific-receive-type"></a>Stage E: Retire ABC-specific receive type

Once all generated code and tests use `triceRx_t`, remove or alias `triceAbcRx_t`.

Temporary compatibility option:

```c
typedef triceRx_t triceAbcRx_t;
```

This can ease migration if the generator or examples still use old names.

---

## 11. <a id="future-c-tlog-architecture"></a>Future C `tlog` Architecture

A standalone C/C++ `tlog` should reuse the same receive layers:

```text
read source -> frame/decrypt -> triceParseNextRecord -> triceResolveLog -> formatter
```

For embedded systems without Go-capable OS support, the application should be able to provide a read function:

```c
typedef int (*triceReadFn_t)(void* ctx, uint8_t* dst, size_t max);
```

A stream object can buffer incomplete data:

```c
typedef struct {
    triceReadFn_t read;
    void* ctx;
    uint8_t* buf;
    size_t len;
    size_t cap;
} triceRxStream_t;
```

Possible future API:

```c
int triceRxStreamNext(triceRxStream_t* s, triceRx_t* rx);
```

This is not needed for the first ABC milestone. The first milestone should implement and test the lower-level parser and resolver functions first. Once stable, the stream layer can be built on top.

The C `tlog` formatter should be a separate later effort. It will need:

- transformed TIL metadata in a C-friendly form,
- format-string parser/formatter in C/C++,
- optional location metadata from `li.json`,
- support for Trice-specific format modifiers,
- statistics and display formatting if desired.

---

## 12. <a id="example-tree-direction"></a>Example Tree Direction

The current `examples/TriceAbc` tree is an early demonstrator. It should stay useful but not define the core architecture.

Recommended direction:

```text
examples/TriceAbc/
  ReadMe.md
  triceBusConfig.h
  BcSim/
  BcSimChk/
  NodeLib/              temporary incubation area for demo-only common code
  N1_tx/
  N2_tx/
  N3_bi/
  N4_rx/
  N5_rx/
  N6_rx/
  N7_bi/
```

Use `../../demoTIL.json`, not a local `examples/TriceAbc/til.json`.

The generator workflow is now:

```sh
trice generate -abc path/target
```

This creates:

```text
path/target.h
path/target.c
```

For example:

```sh
trice generate -abc N1_tx/N1_tx
```

creates:

```text
N1_tx/N1_tx.h
N1_tx/N1_tx.c
```

No `_abc` suffix is needed because the target path already names the generated files.

### 12.1. <a id="node-suffixes"></a>Node suffixes

Use the suffixes as bus access capabilities only:

```text
rx  receive-only: receives broadcast commands only; never writes to the broadcast medium
tx  transmit-only: emits broadcast commands only; does not read from the broadcast medium, so no feedback is possible
bi  bidirectional: emits and receives broadcast commands
```

Important wording:

```text
Trice ABC has no protocol-level response concept. Every bus message is an asynchronous broadcast command. A higher application layer may interpret later commands as answers, acknowledgements, or status reports, but that interpretation is outside the Trice ABC layer.
```

### 12.2. <a id="bcsim-remains-neutral"></a>BcSim remains neutral

`BcSim` should remain a general broadcast-simulation module. Its README should not mention Trice. `examples/TriceAbc/ReadMe.md` explains how Trice ABC uses it.

For the TriceAbc demo, the bus files should live under `examples/TriceAbc/`:

```text
abc.bus
abc.log
abc.bus.lock/
```

`BcSimChk` remains an isolated check/demo for BcSim itself and can continue to use `bc.bus`, `bc.log`, and `bc.bus.lock/` inside its own folder.

---

## 13. <a id="test-strategy"></a>Test Strategy

The implementation must proceed in small testable steps. The test plan should separate transmit tests, record parser tests, resolver tests, ABC dispatch tests, frame/decrypt tests, and future C `tlog` tests.

### 13.1. <a id="parser-unit-tests"></a>Parser unit tests

Create host-native tests for `triceParseNextRecord()`.

Input: decoded Trice record byte buffers.

Assertions:

- no-stamp record extracts ID, `stampBits=0`, `stamp=0`, payload pointer, payload length,
- 16-bit stamp record extracts low 16-bit stamp into normalized 32-bit field,
- 32-bit stamp record extracts 32-bit stamp,
- short-count and long-count payload lengths work,
- returned value is logical consumed length without padding,
- incomplete buffers return `TRICE_RX_E_SHORT`,
- selector 0 behavior matches config,
- transfer-order big/little variants both pass.

These tests can follow the style of `_test/abc_rx_host`, which already builds C fixtures through Go tests and checks C-side behavior.

### 13.2. <a id="resolver-tests"></a>Resolver tests

Create C fixture tests with tiny local arrays.

`triceResolveAbc()`:

- known ID fills `bitWidth` and `fn`,
- unknown ID returns not-found and leaves fields unchanged,
- matching bitWidth after prior log resolve is accepted,
- conflicting bitWidth is rejected.

`triceResolveLog()`:

- known ID fills `bitWidth` and `pFmt` when `TRICE_LOG_ENABLE == 1`,
- unknown ID returns not-found,
- conflict behavior matches ABC resolver.

Compile variants:

```text
TRICE_LOG_ENABLE=0
TRICE_LOG_ENABLE=1
TRICE_LOG_ENABLE=1, TRICE_LOG_WITH_LOCATION=1
```

### 13.3. <a id="abc-dispatch-tests"></a>ABC dispatch tests

Adapt the current `_test/abc_rx_host` tests to the new layering.

Instead of testing only `TriceAbcOnReceive()`, test:

```text
triceParseNextRecord()
triceResolveAbc()
triceDispatchAbc()
TriceAbcOnReceive() compatibility wrapper
```

Keep the current scenario coverage:

- no-payload ABC,
- 8/16/32/64-bit payload widths,
- long-count payload,
- unknown ID ignored,
- payload misalignment rejected after resolve,
- truncated buffer rejected,
- nested dispatch does not corrupt the outer stack-local record.

### 13.4. <a id="receive-only-dependency-test"></a>Receive-only dependency test

Build a small RX-only test that includes the receive headers and generated ABC metadata but does not link normal Trice transmit/output modules.

This test proves that `TRICE_RECEIVE_SUPPORT == 1` does not pull in:

- `trice()` send macros,
- output buffers,
- UART/RTT transmit backends,
- `TriceStamp16` / `TriceStamp32` dependencies.

### 13.5. <a id="existing-transmit-tests"></a>Existing transmit tests

Keep the existing `triceCheck.c` / `//exp:` infrastructure for transmit-side behavior. It is already designed to build target-like C code with multiple configurations and compare generated tool output against expected strings.

ABC transmit tests should remain in that framework because they test real target macro output and host decoding together.

### 13.6. <a id="framedecrypt-tests"></a>Frame/decrypt tests

Initially, keep frame/decrypt tests separate from `triceParseNextRecord()`.

Suggested sequence:

1. no framing: parser sees decoded records directly,
2. COBS frame decode into decoded records,
3. TCOBS frame decode into decoded records,
4. XTEA decrypt plus frame decode.

For XTEA, ensure decrypt support can be compiled in receive-only mode. The current XTEA C file is tied to transmit/backend conditions and may need refactoring before this test can be clean.

### 13.7. <a id="bcsim-tests"></a>BcSim tests

BcSim tests should remain protocol-neutral. They should not depend on Trice. They should verify:

- append-only file writes,
- read offset behavior,
- self-echo filtering by file range,
- text log output,
- no metadata in the binary bus file,
- optional read-lock mode.

### 13.8. <a id="end-to-end-demo-tests"></a>End-to-end demo tests

Later, after core parsing and ABC dispatch are stable, an end-to-end example can run:

```text
BcSim byte stream -> optional TCOBS/XTEA layer -> triceParseNextRecord -> triceResolveAbc -> dispatch
```

This should be a demo acceptance test, not the first unit test.

---

## 14. <a id="recommended-implementation-order"></a>Recommended Implementation Order

1. **Freeze design terms**
   - `TRICE_TRANSMIT_SUPPORT`, `TRICE_RECEIVE_SUPPORT`
   - `TRICE_LOG_ENABLE`, `TRICE_LOG_WITH_LOCATION`
   - `triceRx_t`
   - `triceParseNextRecord()`

2. **Add config defaults**
   - Add new defaults to `triceDefaultConfig.h`.
   - Keep temporary compatibility with current ABC-specific switches if needed.

3. **Introduce `src/triceRx.h/.c`**
   - Define `triceRx_t`, error codes, init helper, bit-width helper.
   - No frame decoding yet.

4. **Implement and test `triceParseNextRecord()`**
   - Pure record parser.
   - Host-native C tests.
   - Little/big transfer-order variants.

5. **Implement and test resolvers**
   - `triceResolveAbc()`.
   - `triceResolveLog()`.
   - Bit-width conflict handling.

6. **Refactor ABC receive**
   - Rebuild `TriceAbcOnReceive()` as parse + resolve + dispatch wrapper.
   - Migrate generated ABC handler signature to `const triceRx_t*`.
   - Keep old type aliases temporarily if needed.

7. **Update generator output**
   - `trice generate -abc path/target` emits `target.h` and `target.c` using `triceRx_t`.
   - Include count variables for resolver calls.

8. **Extend tests**
   - Parser tests.
   - Resolver tests.
   - Wrapper compatibility tests.
   - Receive-only dependency tests.

9. **Add frame/decrypt receive layer**
   - None first.
   - COBS/TCOBS next.
   - XTEA after receive-only decrypt is cleanly separated.

10. **Start future C `tlog` work**
    - Use parser/resolver foundation.
    - Add log resolver tables.
    - Add formatting later.

---

## 15. <a id="risks-and-design-warnings"></a>Risks and Design Warnings

### 15.1. <a id="risk-doing-full-c-tlog-too-early"></a>Risk: doing full C `tlog` too early

A full C `tlog` formatter is much larger than Trice ABC receive. It involves TIL transformation, format parsing, integer/float/string formatting, location data, display policies, and statistics. It should not block the first receive core.

### 15.2. <a id="risk-coupling-rx-to-tx-accidentally"></a>Risk: coupling RX to TX accidentally

This is the main architecture risk. Every new receive file should be tested in a receive-only build. Header includes must be watched carefully. If including `trice.h` pulls in too much, create smaller receive headers.

### 15.3. <a id="risk-xtea-compile-conditions"></a>Risk: XTEA compile conditions

Current XTEA code may be tied to transmit/backend build conditions. Receive-only decrypt needs either refactoring or a separate compile path.

### 15.4. <a id="risk-conflating-parse-and-resolve"></a>Risk: conflating parse and resolve

Do not let the parser fill `bitWidth`, `pFmt`, or function pointers. That makes it impossible to reuse the parser for both ABC and logging.

### 15.5. <a id="risk-treating-abc-follow-up-commands-as-protocol-level-responses"></a>Risk: treating ABC follow-up commands as protocol-level responses

Documentation and demos must avoid saying ABC has responses. ABC has only asynchronous broadcast commands. Higher layers may interpret later commands as responses.

### 15.6. <a id="risk-payload-lifetime"></a>Risk: payload lifetime

Because payload pointers reference the input buffer, delayed processing requires user-side copying. This must be documented in API comments and examples.

---

## 16. <a id="open-decisions"></a>Open Decisions

1. **Final parser name**

   Recommended: `triceParseNextRecord()`.

2. **Transitional ABC compatibility names**

   Decide whether to keep `triceAbcRx_t` as an alias to `triceRx_t` for one transition period.

3. **Error code naming**

   Define one coherent enum for `TRICE_RX_E_*`, `TRICE_RX_NOT_FOUND`, and bit-width conflict.

4. **Receive framing config names**

   Decide whether receive framing uses `TRICE_RX_FRAMING` or a bus-specific example macro only until the full C `tlog` work starts.

5. **XTEA receive switch**

   Decide between reusing `XTEA_DECRYPT` or introducing `TRICE_RX_XTEA_DECRYPT` as clearer receive-side switch.

6. **Location resolver timing**

   `triceResolveLocation()` should be postponed until log formatting work begins.

---

## 17. <a id="suggested-near-term-acceptance-criteria"></a>Suggested Near-Term Acceptance Criteria

The first “done” milestone should be smaller than full ABC + tlog:

- `triceRx_t` exists in `src`.
- `triceParseNextRecord()` is tested for stamp variants, payload variants, transfer order, short buffers, and logical length return.
- `triceResolveAbc()` and `triceResolveLog()` are tested independently.
- `TriceAbcOnReceive()` is implemented as wrapper over parse/resolve/dispatch.
- Existing ABC receive host tests still pass.
- A receive-only build test proves no normal transmit/output dependency.
- ABC transmit tests remain covered by existing `triceCheck.c`/`//exp:` style tests.
- `examples/TriceAbc` remains a demo area, not the source of core architecture.

Once this milestone is reached, adding COBS/TCOBS/XTEA receive integration and then a standalone C/C++ `tlog` becomes much less risky.

---

## 18. <a id="short-recommendation"></a>Short Recommendation

Build the future C receive architecture in this order:

```text
triceRx_t
  -> triceParseNextRecord()
  -> triceResolveAbc() / triceResolveLog()
  -> triceDispatchAbc()
  -> TriceAbcOnReceive() compatibility wrapper
  -> receive-only dependency tests
  -> frame/decrypt integration
  -> future C/C++ tlog formatter
```

This keeps Trice ABC small and testable while making the later standalone C/C++ `tlog` a natural extension instead of a separate rewrite.
