# STM32G0B1 FreeRTOS local logging example

This is an independent copy of `../G0B1_inst`. Its CubeMX project, hardware
setup, FreeRTOS task names, priorities, and stack sizes remain directly
comparable with the original example.

The difference is the logging path:

```text
tasks and interrupts -> binary Trice ring buffer
                    -> idle StartTask02
                    -> TriceLog + nanoprintf
                    -> plain text on USART2 at 115200 baud
```

Producer contexts never call printf or wait for USART2. `StartTask02` is the
only consumer and may block while transmitting the already formatted text.
`TriceTransfer()` is not used because it is an alternative consumer of the same
binary buffer.

Build from this directory after putting `trice` and the Arm GNU toolchain in
`PATH`:

```sh
cd examples/G0B1_log
./build.sh
```

The script runs `trice bind`, generates the filtered target table with
`trice generate -logC`, and builds `out.gcc/G0B1_log.elf`. Generated sidecars
and the generated C table stay below `build/`.

Connect a serial terminal to the USART2 virtual COM port at 115200 baud to see
the locally formatted text. No `trice log`, TIL file, or binary decoder is
needed at runtime. The startup records demonstrate integers, a runtime `%s`,
bounded string width/precision, a 32-bit `aFloat()` value, and a 64-bit
`aDouble()` value. Enabling floating-point support in nanoprintf increases this
example's code size; applications that need only integers and strings can
disable that nanoprintf option again.
