# Trice demo & test project info

Demo projects are ARMKeil IDE STM32 here but TRICE is easy adaptable to 8 - 64-bit architectures.

## Backup folder

- This folder contains unmaintained legacy test projects.
- They are only for reference and need some corrections to work with the current release.
- See also [../docs/TestExamples.md](../docs/TestExamples.md)

## Terminal info

- Open an escape sequence capable terminal in trice root `C:\repos\trice\`opr where you put it.
  - Any directory will do as well but the `til.json` file needs to be found.
  - git-bash will do or also windows-terminal from Mirosoft store.
    - Under Windows the DOS or powershell could have display issues with the escaped ASCII codes. Search the internet for answers.

## RTT info

- The example projects support both, RTT and UART just for demonstration.
- RTT = Real Time Transfer is a technique developed by SEGGER for background memory access during processor runtime. This is possible for ARM cores over the JTAG or SWD interface if a debug probe is connected.
- Some of the STM developmemt boards contain a debug probe you can use for the board itself or for an othe board.
- These are so called on-board STLINK debug probes. To use RTT use the `-p STLINK` switch.
  - `stRttLogger.exe` and `libusb-1.0.dll` must be in the PATH.
- You can also reflash the on-board debug probe with JLINK firmware. To use RTT use the `-p JLINK` switch.
  - `JLinkRTTLogger.exe` and `JLinkARM.dll` must be in the PATH.

## UART

- The default baud rate is 115200 for all test projects and the trice tool assumes that baud rate automatically. Use the ztice tool `-baud` switch for other settings.

## MDK-ARM projects

The projects are generated with necessary library files *as reference* to keep them smaller. Therefore the direct compilation will fail probably. Projects should compile if you follow these steps:

- Install latest ARMKeil IDE.
  - STM32 M0 cores with up to 32 KB FLASH memory are usable directly.
  - You can get a free ST Microelectronics license with a 256 KB FLASH limitation.
- Install latest STM32CubeMX with latest target firmware library.
  - It is free of charge.
- Open the project ioc file with STM32CubeMX and re-generate.
- Open the project with ARMKeil IDE and build.

## Details

- [MDK-ARM_LL_generatedDemo_STM32F030R8-NUCLEO-64\ReadMe.md](MDK-ARM_LL_generatedDemo_STM32F030R8-NUCLEO-64\ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_BARE_STM32F091_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_BARE_STM32F091_NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_ESC_STM32F030R8_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_ESC_STM32F030R8_NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_WRAP_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_WRAP_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md)


## Further info


