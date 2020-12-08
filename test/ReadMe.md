# Trice demo project info

Many are ARMKeil IDE STM32 demo projects here but the TRICE tool is easy adaptable to 8 - 64 bit architectures.

## Terminal info

- Open a escape sequence capable terminal in trice root, like `C:\repos\trice\`. 
  - Any directory will do as well but the `til.json` file needs to be found.
  - git-bash will do or also windows-terminal
  - Under Windows the DOS or powershell could have display issues with the escaped ASCII codes. Search the internet for answers.

## RTT info
- RTT = Real Time Transfer is a technique developed by SEGGER for background memory access during processor runtime. This is possible for ARM cores over the JTAG or SWD interface if a debug probe is connected.
- Some of the STM developmemt boards contain a debug probe you can use for the board itself or for an othe board.
- These are so called on-board STLINK debug probes. To use RTT use the `-p STLINK` switch.
  - `stRttLogger.exe` and `libusb-1.0.dll` must be in the PATH.
- You can also reflash the on-board debug probe with JLINK firmware. To use RTT use the `-p JLINK` switch.
  - `JLinkRTTLogger.exe` and `JLinkARM.dll` must be in the PATH.
## MDK-ARM projects

The projects are generated with necessary library files as reference to keep them smaller. Therefore the direct compilation will fail probably. Projects should compile if you follow these steps:

- Install latest ARMKeil IDE.
  - STM32 M0 cores with up to 32 KB FLASH memory are usable directly.
  - You can get a free ST Microelectronics license with a 256 KB FLASH limitation.
- Install latest STM32CubeMX with latest target firmware library.
  - It is free of charge.
- Open the project ioc file with STM32CubeMX and re-generate.
- Open the project with ARMKeil IDE and build.

## MDK-ARM_LL_UART_RTT0_BARE_STM32F091_NUCLEO-64

Delivers TRICE logs over UART and RTT channel 0 in "bare" encoding:

- Connect STM32F091_NUCLEO-64 board over USB.
- Enter command `trice s` to discover the new COM port.
- Enter command `trice l -e bare -p COM3 -s` when the new COM port is COM3. The `-s` switch shows the raw arriving bytes for issue tracking.
- Enter command `trice l -e bare -p JLINK` when a (SEGGER) JLINK debug probe is active.
- Enter command `trice l -e bare -p STLINK` when a STLINK debug probe is active.


## MDK-ARM_LL_UART_BARE_NUCLEO-F030R8 and MDK-ARM_LL_UART_BARE_NUCLEO-F070RB

- TRICE macros are writing trice atoms with TRICE_PUSH direct into output FIFOs
- Output FIFOs are UART and SeggerRTT

## MDK-ARM_LL_UART_ESC_NUCLEO-F030R8 and MDK-ARM_LL_UART_ESC_NUCLEO-F070RB

- TRICE macros are writing escaped trice bytes with TRICE_PUSH_BYTE direct into output FIFOs
- Output FIFOs are UART and SeggerRTT

## Further info

- See also [../docs/TestExamples.md](../docs/TestExamples.md)

