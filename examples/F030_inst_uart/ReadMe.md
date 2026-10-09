# First instrumentation with UART

Compare this folder with [F030_bare](../F030_bare/) to see the changes required
for UART logging. The vendor files are identical. `main.c` adds the Trice include,
initialization and two log calls. Its loop calls `TriceTransfer` to encode and
send queued records. USART2 uses 115200 baud, 8N1 instead of the bare project's
9-bit setting. Its interrupt handler serves the transmit interrupt.

[triceConfig.h](Core/Inc/triceConfig.h) selects deferred UART output, while
[triceUart.h](Core/Inc/triceUart.h) connects the four transmit operations to
the STM32 LL API. The Makefile adds the Trice library and generated headers.
No RTT logging, receive backchannel or RPC support is enabled.

Put `trice`, `tlog`, `make` and the Arm GNU tools on your PATH. Run:

```sh
cd examples/F030_inst_uart
./build.sh
```

[build.sh](build.sh) runs `trice bind` and `make`. It creates regenerable sidecars
in `out/sidecars` and firmware in `out/`. Keep `til.json` and `triceConfig.h` in
version control; `li.json` describes the current source locations.

Flash `out/F030_inst_uart.elf` onto a NUCLEO-F030R8 board using your debugger.
USART2 TX is PA2, connected to the board's ST-LINK virtual COM port.
Run [log.sh](log.sh) with that port, then reset the board:

```sh
./log.sh COM7
# For example, on Linux:
./log.sh /dev/ttyACM0
```

The two startup records print `Firmware started` and `Value=42`.
The UART sends asynchronously through TX-empty interrupts.

For the corresponding RTT steps, compare [F030_inst_rtt](../F030_inst_rtt/).
The existing [F030_inst](../F030_inst/) demonstrates RTT and UART in parallel.
