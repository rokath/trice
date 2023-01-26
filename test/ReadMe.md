# Testing the `trice` C-code

<!-- vscode-markdown-toc -->
- [Testing the `trice` C-code](#testing-the-trice-c-code)
  - [1. Folder information](#1-folder-information)
  - [2. Package specific information](#2-package-specific-information)
  - [3. todo](#3-todo)
- [Trice demo \& test project info](#trice-demo--test-project-info)
  - [4. Backup folder](#4-backup-folder)
  - [5. Terminal info](#5-terminal-info)
  - [6. RTT info](#6-rtt-info)
  - [7. UART](#7-uart)
  - [8. MDK-ARM projects](#8-mdk-arm-projects)
  - [9. Details](#9-details)
  - [10. Further info](#10-further-info)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  1. <a name='Folderinformation'></a>Folder information

- The packages here are helper for testing the target C-code.
- The packages can have all the same name, only the folder names cannot be equal.
- In each package a different triceConfig.h is used, this way allowing to check all modes automatically, including encryption.
- The file `./testdata/triceCheck.txt` is the master test pattern for all CGO tests and edited manually.
- After editing and before executing the tests, it needs to be copied into `triceCheck.c`, and a `trice u -src triceCheck.c` is needed.
- Than copy the modified `triceCheck.c` into the `./cgo_*` folders.
- THe `til.json` is not needed, it is regenerated from the modified sources inside the RAM filesystem. 
- This somehow unhandy procedere could get a part of a test script. It is is this way because the `trice u` needs to be executed as a precompile script.
- In a postcompile step a `trice z` should restore the `triceCheck.c` unmodified state to be identical to `triceCheck.txt`.
- Unfortunately this has to be done on the os filesystem.

##  2. <a name='Packagespecificinformation'></a>Package specific information

- Each C function gets a Go wrapper which ist tested in appropriate test functions.
- For some reason inside the trice_test.go an 'import "C"' is not possible.
- All C-files in the packages folder referring to the trice sources this way avoiding code duplication.
- The Go functions defined in the packages are not exported. They are called by the Go test functions in this package.
- This way the package test functions are executing the trice C-code compiled with the triceConfig.h there.

##  3. <a name='todo'></a>todo

- repair cgo0_tcobs
- repair cgo1_tcobs
- cgo2_tcobs: ref_cobs.c_?
- Parse "C:\repos\trice\cgo\test\cgo2_tcobs\triceCheck.c" and complete TestTriceCheck


# Trice demo & test project info

Many demo projects are ARMKeil IDE STM32 here but the TRICE tool is easy adaptable to 8 - 64-bit architectures.

##  4. <a name='Backupfolder'></a>Backup folder

- This folder contains unmaintained legacy test projects.
- They are only for reference and need some corrections to work with the current release.
- See also [../docs/TestExamples.md](../docs/TestExamples.md)

##  5. <a name='Terminalinfo'></a>Terminal info

- Open an escape sequence capable terminal in trice root `C:\repos\trice\`opr where you put it.
  - Any directory will do as well but the `til.json` file needs to be found.
  - git-bash will do or also windows-terminal from Microsoft store.
    - Under Windows the DOS or powershell could have display issues with the escaped ASCII codes. Search the internet for answers.

##  6. <a name='RTTinfo'></a>RTT info

- The example projects support both, RTT and UART just for demonstration.
- RTT = Real Time Transfer is a technique developed by SEGGER for background memory access during processor runtime. This is possible for ARM cores over the JTAG or SWD interface if a debug probe is connected.
- Some STM development boards contain a debug probe you can use for the board itself or for another board.
- These are so called on-board ST-LINK debug probes. To use RTT use the `-p STLINK` switch.
  - `stRttLogger.exe` and `libusb-1.0.dll` must be in the PATH.
- You can also flash the on-board debug probe with J-LINK firmware. To use RTT use the `-p JLINK` switch.
  - `JLinkRTTLogger.exe` and `JLinkARM.dll` must be in the PATH.

##  7. <a name='UART'></a>UART

- The default baud rate is 115200 for all test projects and the trice tool assumes that baud rate automatically. Use the trice tool `-baud` switch for other settings.

##  8. <a name='MDK-ARMprojects'></a>MDK-ARM projects

The projects are generated with necessary library files *as reference* to keep them smaller. Therefore, the direct compilation will fail, probably. Projects should compile if you follow these steps:

- Install latest ARMKeil IDE.
  - STM32 M0 cores with up to 32 KB FLASH memory are usable directly.
  - You can get a free ST Microelectronics license with a 256 KB FLASH limitation.
- Install latest STM32CubeMX with the latest target firmware library.
  - It is free of charge.
- Open the project ioc file with STM32CubeMX and re-generate.
- Open the project with ARMKeil IDE and build.

##  9. <a name='Details'></a>Details

- [MDK-ARM_LL_generatedDemo_STM32F030R8-NUCLEO-64\ReadMe.md](MDK-ARM_LL_generatedDemo_STM32F030R8-NUCLEO-64\ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_BARE_STM32F070RB_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_BARE_STM32F070RB_NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_BARE_STM32F091_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_BARE_STM32F091_NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_ESC_STM32F030R8_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_ESC_STM32F030R8_NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_ESC_STM32F070RB_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_ESC_STM32F070RB_NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_WRAP_STM32F030R8-NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_WRAP_STM32F030R8-NUCLEO-64/ReadMe.md)
- [MDK-ARM_RTT0_BARE_STM32F0308-DISCO/ReadMe.md](MDK-ARM_RTT0_BARE_STM32F0308-DISCO/ReadMe.md)
- [MDK-ARM_RTT0_BARE_STM32F03051R8Tx-DISCOVERY/ReadMe.md](MDK-ARM_RTT0_BARE_STM32F03051R8Tx-DISCOVERY/ReadMe.md)
- [MDK-ARM_RTT0_BARE_STM32F03051R8Tx-DISCOVERY/ReadMe.md](MDK-ARM_RTT0_BARE_STM32F03051R8Tx-DISCOVERY/ReadMe.md)
- [MDK-ARM_LL_UART_WRAP_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_WRAP_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md)


##  10. <a name='Furtherinfo'></a>Further info


