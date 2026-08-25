# PC local logging example

This example keeps the normal fast, binary Trice producer path but formats the
records later inside the application. It uses a ring buffer, the system
`snprintf`, and standard output. No serial device or host-side decoder is
needed.

Put the `trice` executable in `PATH`, then run:

```sh
cd examples/PC_log
./build_and_run.sh
```

The script first runs `trice bind`, generates the current target-side log table
with `trice generate -logC`, compiles the example against `../../src`, and runs
it. Generated files stay below `build/`.

`TriceLog()` is intended for one background task. It and `TriceTransfer()` are
alternative consumers of the same deferred buffer and must not be mixed.
