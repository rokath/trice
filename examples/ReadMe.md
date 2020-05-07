# Trice demo project info
## Compiler
- All in the example used compilers are free of charge for firmware images up to 32 KB size. An ARMKeil-MDK license is available for STM M0 cores free of charge also for bigger firmware images.
## triceConfig
- The examples are using mostly together the `triceConfig.h` inside `srcTrice.C` for more easy maintenance.
- Inside the project settings the TRICE_VARIANT is defined for the differences.
- Own projects should use a copy of `triceConfig.h` and modify it accordingly.
## Important Note!
- The ST related DEMO projects are using the ST Microelectronics driver files version `STM32Cube_FW_F0_V1.11.0`.
- Fore more easy complilation the driver files are as copies inside the ST example projects.
- It is recommended to download the latet version of [STcubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) and open the *.ioc files with it and generate again with the newest version.
  - To avoid any trouble you could remove the */Drivers/ directory and remove all drivers references from the project files before re-generating.
  - You may want to change the expected Firmware location and version.
## Encryption
- When enabling encryption the trice tool needs the commandline switch `-key test`, where `test` is the password for the examples
## Automatic update of trice ID list
- Before Build - User command: `trice u -src .. -src  ../../../srcTrice.C -list ../../til.json -v` should be counfigured with correct path locations
## mbed IDE
- with ST NUCLEO the `STM32 Debug+Mass storage+VCP` firmware must be installed
- Check (https://www.st.com/en/development-tools/stsw-link007.html)[https://www.st.com/en/development-tools/stsw-link007.html]
## Segger RTT
- The Segger J-Link on-Board firmware is needed or a physical Segger debug probe
- Check (https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/)[https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/]

