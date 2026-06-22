# Trice ABC examples

This directory contains a host-native multi-node Trice ABC demonstration. The
goal is not to replace the embedded target integration. The goal is to make the
receive path, the ABC generator output, the compact TIL-C output, and the bus
behavior visible on one PC with a small and inspectable code base.

`BcSim` stays protocol-neutral. It transports only bytes. All Trice-specific
framing, parsing, ID resolution, ABC dispatch, and demo behavior live above it
inside `NodeLib/` and the node `main.c` files.

## Quick start

From this directory:

```sh
./build.sh
./demo.sh
```

The demo script starts the receive-capable nodes first and the pure transmit
nodes afterwards. This ordering matters because `BcSim` joins the live stream at
the current end of `abc.bus`. Late joiners intentionally do not replay old bus
traffic.

## Folder overview

```text
TriceAbc/
  ReadMe.md
  triceRxConfig.h

  BcSim/        protocol-neutral file-backed broadcast simulation library
  BcSimChk/     standalone BcSim-only check program

  NodeLib/      shared host runtime, shared ABC selection file, shared TIL-C file

  N1_tx/        transmit-only demo node
  N2_tx/        transmit-only demo node
  N3_bi/        bidirectional demo node
  N4_rx/        receive-only demo node
  N5_rx/        receive-only demo node
  N6_rx/        receive-only demo node with normal Trice logging
  N7_bi/        bidirectional demo node with normal Trice logging
```

The shared generator input is the repository-level TIL file:

```text
../../demoTIL.json
```

## Runtime files

The Trice ABC demo uses these runtime files in this directory:

```text
abc.bus        binary framed Trice byte stream
abc.log        human-readable BcSim traffic log
abc.bus.lock/  temporary writer lock directory
```

These names are separate from `BcSimChk/` on purpose so both demos can coexist.
`NodeLib` additionally uses a temporary `abc.console.lock/` directory to keep
one terminal line from one process together while several nodes print at once.
`demo.sh` uses that same lock for its own status lines, so shell messages do not
cut into node output either. The lock deliberately waits until it becomes free;
there is no timeout fallback to unlocked terminal writes, because occasional
waiting is preferable to mixed half-lines.

## Node roles

The suffix describes bus capability only:

```text
tx  transmit-only: can send but never reads the bus
rx  receive-only: can read and execute but never writes the bus
bi  bidirectional: can send and can receive
```

The current demo roles are:

```text
N1_tx  emits normal Trices, counted typeX0 buffers, and ABC commands
N2_tx  emits normal Trices, counted typeX0 buffers, and ABC commands
N3_bi  emits traffic, receives ABC, and can answer over the bus
N4_rx  receives ABC and executes local actions only
N5_rx  receives ABC and executes local actions only
N6_rx  receives ABC, executes local actions, and prints received normal Trices
N7_bi  emits traffic, receives ABC, answers over the bus, and prints received normal Trices
```

## Demonstrated commands

The demo intentionally uses a small command set that shows different payload
shapes and bus semantics:

```text
cmd:setLeds      payload: one 8-bit LED bit mask
cmd:getLeds      payload: none
cmd:setKey       payload: counted 8-bit byte buffer
cmd:logState     payload: none
cmd:divide       payload: two 32-bit float values

abc:LedsState    payload: one 8-bit LED bit mask
abc:DivideResult payload: one 32-bit float value
```

Important behavior choices:

- `setLeds` updates the local simulated LED bar of every receive-capable node.
- `getLeds` is a broadcast request. Every other node that can send answers with
  `abc:LedsState`. Receive-only nodes do not answer because they have no bus
  transmit capability.
- `setKey` stores a small byte-string key locally and is useful to show counted
  8-bit ABC payload transfer.
- `logState` is only a local `printf` request. It does not emit a Trice or ABC
  response because the point is to keep the overview readable and to show that
  ABC can trigger non-Trice side effects.
- `divide` demonstrates data flowing back over the same bus: a bidirectional
  node can receive `cmd:divide`, compute locally, and answer with
  `abc:DivideResult`.

## Normal Trice and typeX0 traffic

The demo does not send only ABC commands. The transmitting nodes also emit:

- normal Trice log messages
- counted selector-0 `triceX0()` messages

`N6_rx` and `N7_bi` additionally decode and print the normal Trice log traffic
without location data. The example log printer is intentionally small and
demo-focused. It uses `triceRx` plus the generated `til.c` metadata but it does
not try to be a full replacement for the Go `trice log` tool.

Selector-0 counted buffers are shown separately as raw byte payloads. This is
useful because they intentionally have no Trice ID and therefore no TIL lookup.

## Self echo and display policy

Each process writes to `abc.bus` via `BcSim`. `BcSim` remembers the bus-file
offset ranges written by that same process and suppresses them on later reads.
That means:

- a node does not receive its own frames back
- self-generated logs do not appear in the same node's receive output
- the demo can show bidirectional behavior without special Trice-stack hacks

This is the reason why the demo can keep the normal Trice transmit path intact.
The host bridge only replaces the physical output device with `BcSim`.

## Configuration split

`triceRxConfig.h` holds bus-wide choices that must match for every participant.
The most important one is the framing:

```c
#define TRICE_BUS_FRAMING TRICE_FRAMING_COBS
```

The node-specific `triceConfig.h` files only describe the local role:

- whether the node has normal TX support
- whether it has ABC RX support
- whether it additionally resolves normal Trice log traffic
- whether host-side `TRICE_CGO` direct output is needed

The node configs intentionally do not repeat unrelated global defaults.

## Generator workflow

The demo uses one shared generated ABC pair and one shared generated TIL-C file
inside `NodeLib/`.

Generator steps:

```sh
trice generate -i ../../demoTIL.json -abc NodeLib/nodeAbc
trice generate -i ../../demoTIL.json -tilC
# move generated ../../demoTIL.c to NodeLib/til.c
```

This yields:

```text
NodeLib/nodeAbc.h   shared user-owned ABC selection header
NodeLib/nodeAbc.c   shared generated ABC table
NodeLib/til.c       shared generated compact log metadata table
```

`-tilC` currently derives the output file name from the input JSON base name.
With `../../demoTIL.json` as input the generated file is therefore
`../../demoTIL.c`. `build.sh` relocates that file to `NodeLib/til.c` so the
demo can keep a small and predictable local source layout.

Why one shared ABC pair is enough:

- all receive-capable demo nodes understand the same command vocabulary
- the real generated handler names are implemented once in `NodeLib/node.c`
  instead of being wrapped again in every node `main.c`
- whether a node answers on the bus is runtime state (`canSend`), so no
  node-local ABC forwarding layer is needed

## Shared runtime design

`NodeLib/` provides the temporary host runtime that glues the generic Trice TX
and RX pieces together for this demo:

```text
normal Trice send macro / triceX0()
  -> TriceWriteDeviceCgo()
  -> bcSimWrite()
  -> abc.bus
  -> bcSimRead()
  -> COBS frame collector
  -> triceParseNextRecord()
  -> triceResolveAbc() / triceResolveLog()
  -> node handler or small demo log printer
```

Design decisions:

- `TriceWriteDeviceCgo()` is used as the host bridge because it lets the normal
  Trice transmit code stay unchanged.
- COBS is used because the demo should show explicit framed packets and because
  the receiver can delimit records with a simple zero-byte scan.
- `TriceAbcOnReceive()` is not used as the primary demo entry point. The shared
  runtime parses once and then decides whether a record should be treated as ABC,
  normal log traffic, counted typeX0, or unknown traffic. That keeps the example
  readable and avoids double parsing.
- The generated ABC handler names such as `setLeds()` and `divide()` are
  implemented once in `NodeLib/node.c`. The node `main.c` files therefore only
  show role-specific traffic generation and polling loops.
- `TRICE_ENTER_CRITICAL_SECTION` and `TRICE_LEAVE_CRITICAL_SECTION` are not
  used as the console-output guard. They are process-local target-side
  protection hooks. The host demo runs several independent processes, so
  `NodeLib` adds its own small directory-based console lock for `printf`-style
  output.
- `nodeSleepMs()` is shared in `NodeLib` as well, because demo pacing is a
  runtime concern and not node-specific logic.
- The receive-side normal log printer is intentionally small. It is based on the
  generated `til.c` facts and only supports the concrete demo message shapes.
  That keeps the example understandable while still showing why `til.c` is
  useful.

## Output style

The demo aims at readable, line-oriented terminal output:

```text
N4_rx: leds=[**  *   ]
N6_rx: key=bravo7 leds=[***     ]
N7_bi: abc:DivideResult=3.140000
N6_rx: log:tick=4
N7_bi: x0 5 bytes: 10 11 12 13 14
```

The LED bar uses `*` for on and space for off. This is deliberately simple, so
it remains readable when several processes print concurrently. To make that
work in practice, each node first formats a whole line and then prints it under
the shared console lock.
