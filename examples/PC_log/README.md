# PC local logging example

This example keeps the normal fast, binary Trice producer path but formats the
records later inside the application. It uses a ring buffer, the system
`snprintf`, and standard output. No serial device or host-side decoder is
needed. The emitted records demonstrate integers, a runtime `%s`, bounded
string width/precision, 32-bit `aFloat()` and 64-bit `aDouble()` values, Trice
specific conversions, and a buffer Trice. It then executes the shared
`_test/testdata/triceCheck.c` producer corpus line by line, using the same
source as the regular PC target tests and installed MCU examples. Its explicit
switches in `triceConfig.h` are a readable full-feature configuration example.

Put the `trice` executable in `PATH`, then run:

```sh
cd examples/PC_log
./build_and_run.sh
```

The script first runs `trice bind`, generates the current target-side log table
with `trice generate -logC`, compiles the example against `../../src`, and runs
it. The command/RPC and selector-0 transport cases that are not local text logs
are disabled only by this target configuration. The two dynamic-string byte
dump forms handled only by the host decoder are guarded only when
`TRICE_LOCAL_LOG` is active. Ordinary `triceCheck.c` users therefore retain
their existing behavior. Generated files stay below `build/`.

`TriceLog()` is intended for one background task. It and `TriceTransfer()` are
alternative consumers of the same deferred buffer and must not be mixed.
