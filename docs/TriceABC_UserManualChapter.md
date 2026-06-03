# Trice ABC - Asynchronous Broadcast Commands

Status: draft user manual chapter.

Suggested integration points:

- Add this chapter to `docs/TriceUserManual.md` near the existing buffer and remote-call related sections.
- Replace the current `triceF` remote-function-call wording with a short deprecation note and a reference to this chapter.
- Add a matching generator subsection near the current `trice generate` documentation.

## Purpose

Trice ABC is an optional Trice extension for asynchronous command-style communication between devices.

A Trice ABC message is a normal Trice message with a command-oriented Trice ID and payload. Any device that receives the message may interpret that ID as a local command key. The receiving device may have a generated ABC table that maps the received Trice ID to a local command handler. If the handler exists and is implemented by the application, it can be executed later by the ABC processing routine. The sender does not wait for a direct return value and does not need to know which devices will react.

The central idea is:

```text
sender emits Trice ABC message with ID X and optional payload P

receiver A: ID X -> local handler A, or no action
receiver B: ID X -> local handler B, or no action
receiver C: ID X -> local handler C, or no action
```

Different receivers may associate the same Trice ID with the same action, with different actions, or with no action at all. A handler may answer with zero, one, or more Trice messages. Such responses are ordinary Trice messages and may again be ABC messages.

Trice ABC is meant as a small building block. It does not prescribe a complete distributed protocol. Applications can build their own schemes for request/response, acknowledgements, timeouts, voting, quorum handling, swarm behavior, or routing on top of it.

## What Trice ABC is not

### Not RPC

Trice ABC is not Remote Procedure Call.

An RPC call normally implies a particular callee, a procedure invocation model, and often a direct result or a future-like response object. Trice ABC intentionally does not provide those semantics. The sender emits an asynchronous command message and continues execution. Receivers independently decide whether the ID is relevant and what local code, if any, should run.

There is no built-in call stack relation, no direct return value, no required reply, and no guarantee that exactly one receiver acts.

### Not dispatch in the router sense

The word dispatch often suggests a dispatcher that routes one message to one selected handler. Trice ABC has no required central dispatcher and no required address selection.

The command is carried by a Trice message. Receivers inspect the Trice ID locally. The local table lookup and handler activation are implementation mechanisms, not the user-level communication model.

### Not publish-only messaging

The word publish describes only that a message is emitted. Trice ABC is action-oriented: the message is intended to be interpretable as a command by receivers. For that reason the feature name uses Command, not Publish.

### Not remote code execution

Trice ABC must not be described as RCE. It does not transfer executable code. It transfers a Trice ID and payload. Receivers can only execute locally compiled handlers that are present in their firmware and are enabled by their local generated ABC table and application code.

## Why the name Trice ABC

ABC means Asynchronous Broadcast Commands.

- Asynchronous: the sender does not block waiting for the receiver or for responses.
- Broadcast: the command may be received by multiple devices; the sender does not imply exactly one peer.
- Commands: the message asks interested receivers to do something.

The name describes the observable behavior rather than the implementation. It avoids RPC terminology and does not expose implementation details such as function pointers, handler tables, or local lookups.

The recommended full name is:

```text
Trice ABC - Asynchronous Broadcast Commands
```

The recommended short term is:

```text
ABC
```

The recommended C macro suffix is:

```text
C
```

In macro names, `C` means Command.

## Basic use

An ABC command name is written in the Trice format string position. The command name must be usable as a generated C function name after an optional tag prefix has been removed.

Examples:

```c
triceC("cmd:motor_stop");
TRiceC(0x87654321u, "cmd:motor_stop");

uint32_t unixTime = 1777777777u;
trice32C("cmd:set_time", &unixTime, 1);

uint16_t requestStamp = 0x4321u;
Trice8C(requestStamp, "cmd:get_power_state", NULL, 0);
```

The part before `:` is a normal Trice tag. It can be used for coloring, ID ranges, filtering with `-pick` or `-ban`, and host-side display. The tag does not define ABC behavior.

The command symbol after the optional tag must be a valid C identifier:

```text
[A-Za-z_][A-Za-z0-9_]*
```

Valid examples:

```text
cmd:motor_stop
cmd:get_power_state
cmd:SetMotorSpeed
abc:SwarmPing
MotorStop
```

Invalid examples:

```text
cmd:motor-stop
cmd:motor stop
cmd:123_start
cmd:foo.bar
cmd:foo()
```

Leading underscores should be avoided because C reserves several underscore-prefixed identifier classes.

## Macro families

The ABC macro names follow the existing Trice naming style.

### ABC without payload

```c
triceC("cmd:motor_stop");
TriceC(stamp16, "cmd:motor_stop");
TRiceC(stamp32, "cmd:motor_stop");
```

### ABC with 8-bit payload elements

```c
trice8C("cmd:set_leds", p8, count);
Trice8C(stamp16, "cmd:set_leds", p8, count);
TRice8C(stamp32, "cmd:set_leds", p8, count);
```

### ABC with 16-bit payload elements

```c
trice16C("cmd:set_pwm", p16, count);
Trice16C(stamp16, "cmd:set_pwm", p16, count);
TRice16C(stamp32, "cmd:set_pwm", p16, count);
```

### ABC with 32-bit payload elements

```c
trice32C("cmd:set_time", p32, count);
Trice32C(stamp16, "cmd:set_time", p32, count);
TRice32C(stamp32, "cmd:set_time", p32, count);
```

### ABC with 64-bit payload elements

```c
trice64C("cmd:set_epoch", p64, count);
Trice64C(stamp16, "cmd:set_epoch", p64, count);
TRice64C(stamp32, "cmd:set_epoch", p64, count);
```

The number in the macro name is the payload element width. The capitalization still follows the Trice stamp convention:

| Macro spelling | ABC stamp field |
| --- | --- |
| `trice...C` | no ABC stamp |
| `Trice...C` | 16-bit ABC stamp |
| `TRice...C` | 32-bit ABC stamp |

For ABC messages, a stamp is not necessarily a time stamp. It is an application-defined correlation value.

## ABC stamps

Normal Trice messages often use the Trice stamp field for target time, cycle information, or another configured timing source.

ABC messages reuse the same technical stamp field but assign it ABC semantics:

```text
ABC stamp = user-defined correlation value transported in the Trice stamp field
```

Typical uses are:

- request identifier
- response correlation identifier
- source-device plus sequence value
- transaction identifier
- application-defined marker

The Trice ABC core does not define how stamp bits must be split. For example, an application may choose:

```c
uint32_t stamp32 = ((uint32_t)deviceId << 16) | (sequenceCounter++ & 0xffffu);
TRice32C(stamp32, "cmd:request_status", payload, payloadCount);
```

Another application may use a timestamp, a random value, a bus-specific sequence number, or no stamp at all.

The host-side display should avoid labeling ABC stamps as `time:`. A display label such as `stamp:` or `abc:` is more accurate.

To be decided:

- Exact host display label for ABC stamps: recommended default is `stamp:`.

## Receiving ABC messages

ABC receiving is optional. Sending ABC messages and receiving ABC messages are separate capabilities.

A receiver that wants to act on ABC messages compiles generated ABC files into its firmware:

```bash
trice generate -abcH -abcC -i til.json
```

The generated files provide:

- a table of ABC Trice IDs,
- generated handler prototypes,
- weak empty handler definitions,
- wrapper functions or table entries needed by the runtime,
- the ABC table size.

If an application does nothing, the generated weak handlers are empty and no visible action happens. If the application provides a non-weak implementation of a generated handler, that implementation is called by the ABC processing routine when the matching ABC message was received.

Example generated handler signatures:

```c
void motor_stop(void);
void set_time(int32_t* p, int cnt);
void set_pwm(int16_t* p, int cnt);
```

Example application implementation:

```c
void motor_stop(void) {
    MotorStopNow();
}

void set_time(int32_t* p, int cnt) {
    if (cnt == 1) {
        ClockSetUnixTime((uint32_t)p[0]);
    }
}
```

## Deferred handler execution

An ABC receiver should not call application handlers directly from a transport interrupt, DMA callback, or low-level decoder context. The recommended minimal receive model is a one-deep command mailbox per generated ABC table entry.

On receive:

```text
receive Trice frame
parse ID, stamp, payload
find matching ABC table entry
copy stamp and payload into the entry state
set pending flag
```

Later, application code calls:

```c
TriceAbcProcess();
```

The processing routine scans pending flags, clears each flag, and calls the generated/local handler.

The default buffering model is intentionally small:

```text
one pending slot per ABC command ID
```

If the same ABC ID arrives again before the pending slot is processed, the default policy should be last-wins overwrite. This matches state-setting commands such as `set_pwm`, `set_mode`, `set_time`, or `request_status`.

Applications that need queues, exact-once execution, duplicate detection, or loss-free command counting must implement that policy above ABC.

To be decided:

- Whether the initial implementation exposes alternative pending policies such as drop-new or immediate-call. The recommended first implementation uses only last-wins overwrite.

## Responses

ABC does not define a response protocol. A handler may send zero, one, or more Trice messages as responses.

If an ABC stamp was received, the runtime should make it available during handler execution. A handler can reuse it in a response:

```c
void request_status(int32_t* p, int cnt) {
    uint32_t stamp = TriceAbcCurrentStamp32();
    int32_t status = GetStatusWord();

    TRice32C(stamp, "rsp:status", &status, 1);
}
```

The sender can collect responses with the same stamp if it wants to implement request/response behavior. Trice ABC itself does not wait, retry, count responses, or decide when enough responses have arrived.

## Multiple senders and multiple code bases

ABC works best when all devices that should understand a command share the same Trice ID for that command. If several devices have different code bases, they may generate different TIL files.

The recommended approach is to merge or import TIL files and then generate ABC files from the merged TIL data:

```text
TIL files -> merged TIL -> generated ABC C/H files
```

Generated C files should not be hand-merged. The TIL data is the source of truth.

Possible future tool flow:

```bash
trice til merge -o swarm.til.json device_a.til.json device_b.til.json device_c.til.json
trice generate -abcH -abcC -i swarm.til.json
```

or:

```bash
trice generate -abcH -abcC -i local.til.json -abcImport device_a.til.json -abcImport device_b.til.json
```

To be decided:

- Exact CLI shape for TIL merging or ABC imports.

## Security and trust boundary

ABC receiving allows incoming Trice messages to trigger locally compiled application code. Do not enable ABC receiving on untrusted transports without an application-level trust model.

Possible protections include:

- restricting transports that can feed `TriceAbcOnReceive`,
- filtering allowed IDs,
- using authenticated or encrypted links,
- validating command payloads in every handler,
- disabling ABC receive support in production builds that do not need it.

ABC is a command primitive, not an authorization system.

## Legacy `triceF` status

The existing `triceF` macros were introduced as remote-function-call syntax support. They should not be extended into ABC.

Recommended transition:

- Keep existing `triceF` behavior unchanged for compatibility.
- Mark `triceF` as deprecated in the documentation.
- Do not use `triceF` for new ABC examples.
- Introduce new `triceC` macros for ABC.
- Introduce new generator options `-abcH` and `-abcC`.
- Keep `-rpcH` and `-rpcC` only as legacy/deprecated options for old `triceF` users, or remove them in a later major cleanup.

New code should use `triceC`, `trice8C`, `trice16C`, `trice32C`, or `trice64C`.

## Minimal complete example

Sender:

```c
#include "trice.h"

static uint16_t abcSeq16;

void AskPowerState(void) {
    uint16_t stamp = abcSeq16++;
    TriceC(stamp, "cmd:get_power_state");
}
```

Receiver:

```c
#include "trice.h"
#include "tilAbc.h"

void get_power_state(void) {
    uint16_t stamp = TriceAbcCurrentStamp16();
    uint32_t value = BoardPowerState();

    TRice32C((uint32_t)stamp, "rsp:power_state", &value, 1);
}

void main_loop(void) {
    for (;;) {
        TriceTransfer();
        TriceAbcProcess();
        ApplicationStep();
    }
}
```

The example uses a 16-bit request stamp and sends a response with a related stamp. This is only an application pattern. ABC itself does not impose request/response behavior.

## Summary

Trice ABC turns selected Trice IDs into asynchronous broadcast commands. The same Trice message can be ignored by one receiver, activate a handler on another receiver, and activate a different handler on a third receiver. The sender keeps running and may optionally correlate later responses by using ABC stamps.

ABC is intentionally small: Trice ID, optional ABC stamp, payload, generated local handler table, one pending slot per command, and explicit application control over all higher-level protocol behavior.
