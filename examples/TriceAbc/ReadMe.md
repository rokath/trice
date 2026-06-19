# Trice ABC examples

This directory is prepared for small host-native examples that show Trice ABC in
action without requiring several embedded boards. The examples use `BcSim` as a
file-backed broadcast medium. `BcSim` itself is protocol-neutral; all Trice- and
ABC-specific code lives outside `BcSim`.

## Folder overview

```text
TriceAbc/
  ReadMe.md
  triceRxConfig.h

  BcSim/        protocol-neutral file-backed broadcast simulation library
  BcSimChk/     standalone check program for BcSim only

  NodeLib/      common helper code for the Trice ABC node examples

  N1_tx/        transmit-only node
  N2_tx/        transmit-only node
  N3_bi/        bidirectional node
  N4_rx/        receive-only node
  N5_rx/        receive-only node
  N6_rx/        receive-only node
  N7_bi/        bidirectional node
```

The examples use the repository-level TIL file:

```text
../../demoTIL.json
```

when commands are run from this `examples/TriceAbc` directory.

## Bus files

The Trice ABC examples shall use these runtime files in this directory:

```text
abc.bus        binary bus stream
abc.log        human-readable traffic log
abc.bus.lock/  temporary writer lock directory
```

These names are intentionally different from the `BcSimChk` check program,
which stays isolated in its own directory and continues to use `bc.bus`,
`bc.log`, and `bc.bus.lock/`.

## Node suffixes

The suffixes describe only bus access capability. They do not define protocol
roles.

```text
rx  receive-only: receives broadcast commands only; never writes to the broadcast medium
tx  transmit-only: emits broadcast commands only; does not read from the broadcast medium, so no feedback is possible
bi  bidirectional: emits and receives broadcast commands
```

Node overview:

```text
N1_tx  transmit-only: emits broadcast commands only; no feedback is possible
N2_tx  transmit-only: emits broadcast commands only; no feedback is possible
N3_bi  bidirectional: emits and receives broadcast commands
N4_rx  receive-only: receives broadcast commands only; local action only
N5_rx  receive-only: receives broadcast commands only; local action only
N6_rx  receive-only: receives broadcast commands only; local action only
N7_bi  bidirectional: emits and receives broadcast commands
```

## No protocol-level responses

Trice ABC has no protocol-level response concept. Every bus message is an
asynchronous broadcast command. A higher application layer may interpret later
commands as answers, acknowledgements, status reports, or other follow-up
messages, but that interpretation is outside the Trice ABC layer.

## Configuration split

`triceRxConfig.h` contains settings that must be identical for all nodes on the
same bus, for example the selected bus framing or later encryption-related
settings.

Each node has its own `triceConfig.h`. A node-specific config includes
`../triceRxConfig.h`, sets `TRICE_CLEAN`, and states whether the node uses the
normal Trice transmit stack and/or the normal Trice receive stack:

```c
#define TRICE_TRANSMIT_SUPPORT 1
#define TRICE_RECEIVE_SUPPORT  0
```

The node-specific files intentionally do not repeat unrelated defaults from
`triceDefaultConfig.h`.

## Generator workflow

The generator can create node-specific ABC selection and table files with:

```sh
trice generate -abc path/target
```

The command writes:

```text
path/target.h
path/target.c
```

without an `_abc` suffix. Example from this directory:

```sh
trice generate -i ../../demoTIL.json -abc N1_tx/N1_tx
trice generate -i ../../demoTIL.json -abc N2_tx/N2_tx
trice generate -i ../../demoTIL.json -abc N3_bi/N3_bi
trice generate -i ../../demoTIL.json -abc N4_rx/N4_rx
trice generate -i ../../demoTIL.json -abc N5_rx/N5_rx
trice generate -i ../../demoTIL.json -abc N6_rx/N6_rx
trice generate -i ../../demoTIL.json -abc N7_bi/N7_bi
```

The `.h` file is the editable node selection file. The `.c` file is generated
from `../../demoTIL.json` and the selected handlers.

## NodeLib role

`NodeLib/` contains common helper code for node examples. It is not a replacement
for the final Trice target library. It exists so the examples can share a small
receive object type, record parser interface, and resolver interfaces while the
larger C-side tooling evolves.

The planned processing stack is:

```text
BcSim read/write
  -> selected framing/decryption layer
  -> triceParseNextRecord()
  -> triceResolveAbc() and/or triceResolveLog()
  -> node application handler
```

`triceParseNextRecord()` parses one already decoded Trice record at the start of
a byte buffer and returns the number of consumed bytes or a negative error code.
It does not fill format strings, file/line information, handler pointers, or
bit width. Resolver functions add metadata by using the parsed ID.
