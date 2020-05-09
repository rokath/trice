# Trice demo project info
## Compiler
- All in the example used compilers are free of charge for firmware images up to 32 KB size. An ARMKeil-MDK license is available for STM M0 cores free of charge also for bigger firmware images.
## triceConfig
- The examples are using mostly together the `triceConfig.h` inside `srcTrice.C` for more easy maintenance.
- Inside the project settings the TRICE_VARIANT is defined for the differences.
- Own projects should use a copy of `triceConfig.h` and modify it accordingly.
## Important Note for STM example projects
- The ST related DEMO projects are using the ST Microelectronics driver files version `STM32Cube_FW_F0_V1.11.0`.
- The driver files are as links inside the ST example projects.
- It is recommended to download the latet version of [STcubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) and open the *.ioc files with it and generate again with the newest version.
- On Windows the ST firmware packages are installed usually in the user directory.
- You may need to adapt the paths in the project settings or put a copy into `C:\ST\CubeMX\STM32Cube_FW_F0_V1.11.0` to compile without changes.
  - To avoid any trouble you could remove the */Drivers/ directory, if any, and remove all drivers references from the project files before re-generating.
  - You may want to change the expected Firmware location and version.
## Encryption
- When enabling encryption the trice tool needs the commandline switch `-key test`, where `test` is the password for the examples
## Automatic update of trice ID list
- Before Build - User command: `trice u -src .. -src  ../../../srcTrice.C -list ../../til.json -v` should be counfigured with correct path locations
## mbed IDE
- with ST NUCLEO the `STM32 Debug+Mass storage+VCP` firmware must be installed
- Check (https://www.st.com/en/development-tools/stsw-link007.html)[https://www.st.com/en/development-tools/stsw-link007.html]
## Segger RTT
- Segger RTT is a technique to transfer data from target to host without using an interface despite the debug probe.
- The Segger J-Link on-Board firmware is needed or a physical Segger debug probe. This way all target communication interfaces are usable for the application during development.
- Following steps describe the needed steps for a STM NUCLEO board and windows - adapt them to your environment.
  - Get & install [STM32 ST-LINK utility](https://www.st.com/en/development-tools/stsw-link004.html) 
  - Run from default install location `"C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LinkUpgrade.exe"`)
  - Enable checkbox `Change Type` and select radio button `STM32 Debug+Mass storage + VCP` The `STM32 Debug+ VCP` won´t be detected by Segger reflash utility.
  - Check [https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/](https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/)
  - Use `STLinkReflash.exe` to convert NUCLEO from ST-Link on-board to J-Link on-board. `STM32 Debug+ VCP` won´t be detected by Segger reflash utility.
  - Build and flash `triceDemo_NUCLEO-F030RB_LL_SeggerRTT_MDK-ARM`
  - Download [J-Link Software and Documentation Pack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack) and install
  - Start `"C:\Program Files (x86)\SEGGER\JLink\JLinkRTTViewer.exe"` and connect to the J-Link. You only need this as a running server to connect to.
  - Run `trice log -list /c/GitRepos/trice/examples/til.json -device RTT`. It should now connect to JLinkLogViewer.
  - Also `trice receiver -list /c/GitRepos/trice/examples/til.json -device RTT` is possible when trice display server is active (`trice ds`).
  - Now the trice output is visible.
- In the SeggerRTT example projects you see how to setup. 
- Segger offers a SeggerRTT SDK which allows to use more than just channel 0 and you can develop your own tooling with it.
- Maybe `libusb` together with `libjaylink` offer some options too.


