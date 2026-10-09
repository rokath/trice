# First instrumentation with RTT

Compare this folder with [F030_bare](../F030_bare/) to see the changes required
for RTT logging. The vendor files are identical. `main.c` adds the Trice include,
initialization and two log calls; [triceConfig.h](Core/Inc/triceConfig.h) selects
direct RTT output. The Makefile adds the Trice library and generated headers.
No UART logging or RPC support is enabled.

Put `trice`, `tlog`, `make` and the Arm GNU tools on your PATH. Run:

```sh
cd examples/F030_inst_rtt
./build.sh
```

[build.sh](build.sh) runs `trice bind` and `make`. It creates regenerable sidecars
in `out/sidecars` and firmware in `out/`. Keep `til.json` and `triceConfig.h` in
version control; `li.json` describes the current source locations.

Flash `out/F030_inst_rtt.elf` onto a NUCLEO-F030R8 board using your debugger.
With the SEGGER J-Link tools installed and a J-Link connection to the board,
run [log.sh](log.sh), then reset the board:

```sh
./log.sh
```

The two startup records print `Firmware started` and `Value=42`. RTT writes
directly to its RAM buffer; no transfer call or UART interrupt is needed.

For the corresponding UART steps, compare [F030_inst_uart](../F030_inst_uart/).
The existing [F030_inst](../F030_inst/) demonstrates RTT and UART in parallel.
