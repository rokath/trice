# Trice demo project info
## General Infos
## Third party
- SEGGER_RTT_V672b referenced source code by *SeggerRTT* projects
- About: [SeggerRTT.md](SeggerRTT.md)

### Compiler
Many in the examples used compilers are free of charge for firmware images up to 32 KB size, if not otherwise stated. An ARMKeil-MDK license is available for STM M0/M0+ cores free of charge also for bigger firmware images (256KB) but demands generally a paid license for non M0 cores.
- IAR works also for M4 for example but free only up to 32 KB images.
- TASKING provides a free eval license for about 2 weeks
- The PIC compilers available in paid and free versions

### trice configuration
- The examples are using mostly together the `triceConfig.h` inside `srcTrice.C` for more easy maintenance.
- Inside the project settings the TRICE_VARIANT is defined for the differences.
- Own projects should use a copy of `triceConfig.h` and modify it accordingly.

### Important Note for STM example projects
- The ST evaluation boards usually com with an on-board ST-Link probe (firmware).
- The example projects are configured for on-board J-Link firmware. 
  - One can either re-configure the examples for ST-Link usage or reflash the own board with J-Link on-board debug firmware. The way back is always open. If the Segger flash tool does not recognize the ST-Link firmware use the ST-Link tool to reflash with the 2nd option (the one with memory support)
- The ST related DEMO M0 projects are using the ST Microelectronics driver files version `STM32Cube_FW_F0_V1.11.0`.
- The driver files are as links inside the ST example projects.
- It is recommended to download the latet version of [STcubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) and open the *.ioc files with it and generate again with the newest version.
- On Windows the ST firmware packages are installed usually in the user directory.
- You may need to adapt the paths in the project settings or put a copy into `C:\ST\CubeMX\STM32Cube_FW_F0_V1.11.0` to compile without changes.
  - To avoid any trouble you could remove the */Drivers/ directory, if any, and remove all drivers references from the project files before re-generating.
  - You may want to change the expected Firmware location and version.
- If you use trice only during the development RTTD (Segger-RTT trice Direct mode) is recommended as interface.

### Encryption
- When enabling encryption the trice tool needs the commandline switch `-key test`, where `test` is the password for the examples

### Automatic update of trice ID list
- Before Build - User command: `trice u -src .. -src  ../../../srcTrice.C -list ../../til.json` should be counfigured with correct path locations

- The `til.json` file is the common trice ID list file for all demo projects.

## Specific Infos (alphabetical order)
- [TestExamplesARM-MDK.md](TestExamplesARM-MDK.md) (done)
- [TestExamplesAtollicSTM32.md](TestExamplesAtollicSTM32.md) (done)
- [TestExamplesIAR-EWARM.md](TestExamplesIAR-EWARM.md) (done)
- [TestExamplesSTM32CubeIDE.md](TestExamplesSTM32CubeIDE.md) (todo)
- [TestExamplesTASKING.md](TestExamplesTASKING.md) (half)

## TODO projects (work in progress)
- [TestExamplesWIP.md](TestExamplesWIP.md)
