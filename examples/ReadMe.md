# Example Projects without and with Trice Instrumentation


<details><summary>Table of Contents</summary><ol><!-- TABLE OF CONTENTS START -->

<!-- 
Table of Contents Generation:
- Install vsCode extension "Markdown TOC" from dumeng 
- Use Shift-Ctrl-P "markdownTOC:generate" to get the automatic numbering.
- replace "<a id=" with "<a id=" 
-->

<!-- vscode-markdown-toc -->
* 1. [About](#about)
* 2. [Examples](#examples)
* 3. [Important to know](#important-to-know)
* 4. [Animation](#animation)
* 5. [Setup PC](#setup-pc)
  * 5.1. [Setup Trice](#setup-trice)
  * 5.2. [Setup ARM Environment](#setup-arm-environment)
    * 5.2.1. [Install ARM GCC](#install-arm-gcc)
    * 5.2.2. [MacOS](#macos)
    * 5.2.3. [Install ARM Clang (optional)](#install-arm-clang-(optional))
    * 5.2.4. [Check Project Makefile (if it already exists)](#check-project-makefile-(if-it-already-exists))
  * 5.3. [Setup STM32](#setup-stm32)
    * 5.3.1. [Generate Base Project](#generate-base-project)
    * 5.3.2. [Update NUCLEO Onboard Debugger (other ST evaluation boards too)](#update-nucleo-onboard-debugger-(other-st-evaluation-boards-too))
  * 5.4. [Setup Onboard J-Link on NUCLEO (other ST evaluation boards too)](#setup-onboard-j-link-on-nucleo-(other-st-evaluation-boards-too))
  * 5.5. [Setup VS-Code](#setup-vs-code)
* 6. [Makefile with Clang too](#makefile-with-clang-too)
* 7. [Download Locations](#download-locations)
  * 7.1. [Clang](#clang)
  * 7.2. [GCC](#gcc)
* 8. [Install Locations](#install-locations)
* 9. [Environment Variables](#environment-variables)
* 10. [Build command](#build-command)
* 11. [Run & Debug](#run-&-debug)
* 12. [Logging](#logging)
* 13. [Setting up a new project](#setting-up-a-new-project)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

<div id="top"></div></ol></details><!-- TABLE OF CONTENTS END -->

##  1. <a id='about'></a>About

- All used tools are **Open Source**.
- All provided information is just as example and needs adaption to your needs.
- There is no need to setup the environment in the given order.

##  2. <a id='examples'></a>Examples

| Project Name | Description |
| - | - |
|||
| [F030R8_gen](./F030R8_gen) | This is a minimal STM32CubeMX generated Makefile project adapted to Clang and GCC. It serves as a reference for diff to [F030R8_inst](./F030R8_inst) so see quickly the needed instrumentation steps you need for your own project. |
| [F030R8_inst](./F030R8_inst) | This is a minimal STM32CubeMX generated Makefile project adapted to Clang and GCC and afterward instrumented with the Trice library. Compare it with [F030R8_gen](./F030R8_gen) to see quickly how to instrument your project. |
|||
| [G0B1_gen](./G0B1_gen) | This is a minimal FreeRTOS STM32CubeMX generated Makefile project adapted to Clang and GCC. |
| [G0B1_inst](./G0B1_inst) | This is a minimal FreeRTOS STM32CubeMX generated Makefile project adapted to Clang and GCC and afterward instrumented with the Trice library. |
|||
| [L432KC_gen](./L432KC_gen) | This is a minimal STM32CubeMX generated Makefile project. |
| [L432KC_gen_ad_toClang](./L432KC_gen_ad_toClang) | This is a minimal STM32CubeMX generated Makefile project adapted to Clang and GCC. |
| [L432KC_gen_ad_toClang_ed](./L432KC_gen_ad_toClang_ed) | This is a minimal FreeRTOS STM32CubeMX generated Makefile project extended to compile also with Clang trying to perform minimal changes. It produces some warnings, because it is not finetuned. The [L432KC_gen_ad_toClang_ed_instr](./L432KC_gen_ad_toClang_ed_instr) project is then a next step performable. |
| [L432KC_gen_ad_toClang_ed_instr](./L432KC_gen_ad_toClang_ed_instr) | This is a minimal FreeRTOS STM32CubeMX generated Makefile project adapted to Clang and GCC and afterward instrumented with the Trice library. |
|||
| [OpenCM3_STM32F411_Nucleo](./OpenCM3_STM32F411_Nucleo) | See the [Readme.md](./OpenCM3_STM32F411_Nucleo/Readme.md) in this folder. |
|||

##  3. <a id='important-to-know'></a>Important to know

The [ARM-Keil µVision IDE](https://www2.keil.com/mdk5/uvision/) does sometimes not recognize external file modifications. That means for example: After editing `main.c` by adding a `trice( "Hi!\n" )` and executing `trice insert` as pre-compile step it could happen, that an updated `trice( iD(12345), "Hi!\n" )`  was inserted and correct compiled but the update in `main.c` is not shown. Simply close and reopen `main.c` before editing again. This seems to be a [ARM-Keil µVision IDE](https://www2.keil.com/mdk5/uvision/) "feature" or be caused Windows not signaling a file change.

##  4. <a id='animation'></a>Animation

(The trice IDs occur just during the compilation.)

  <img src="./Animation.gif" width="1200">

##  5. <a id='setup-pc'></a>Setup PC

Setting up a PC is for Linux mostly straightforward but Windows PCs are more problematic. The steps shown here are just one example.

- Create folder `repos` in your home directory.
  - Clone all repositories here.
- Create `C:\bin` folder.
  - When installing toolchains, put them here then and avoid spaces in created paths.
- Add `C:\bin` to PATH variable at the beginning.
  - This allows to copy tools like `trice.exe` simply into `C:\bin`.
- Install "Git for windows" from https://git-scm.com/downloads to get the neat git bash.
  - Select the Standalone Installer. This gives you useful context menu entries in the Windows explorer.
- BTW: For managing git repositories I like https://www.gitkraken.com/. Its free of charge for open source programs.
- Install VS-Code
  - This is my favorite editor with many optional Add-Ons. It is used for debugging as well.
- Install Go if you wish to compile Go programs.
  - Install latest [TDM GCC](https://jmeubank.github.io/tdm-gcc/download/).
    - TDM GCC is needed only if you wish to use CGO as well.
    - If you just want compile the Trice tool TDM GCC is not needed. But for performing all tests it is nessecary.
- Setup J-Link if you use this debug probe as hardware or software (see below).
  - Install SEGGER [J-Link Software and Documentation Pack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)
- Install [Make for Windows](https://sourceforge.net/projects/gnuwin32/) and add its installation bin folder location to the PATH variable.

###  5.1. <a id='setup-trice'></a>Setup Trice

- from inside folder `repos` clone trice repo with `git clone https://github.com/rokath/trice.git`.
- Run `go install ./cmd/trice/...` from folder `repos/trice`.

OR

- Download the latest release archive and extract.
- Put trice binary into C:\bin.
- Put trice/src into `repos` if you want access the trice library code from several projects and have it only once.
  - Alternatively copy it into your project.

###  5.2. <a id='setup-arm-environment'></a>Setup ARM Environment

####  5.2.1. <a id='install-arm-gcc'></a>Install ARM GCC

- Uninstall existing ARM GCC compilers optionally.
- Check if $PATH is clean.
  - In fact you can leave it as it is.
- Check if `C_INCLUDE_PATH` is not defined.
  - It would cause issues performing Go tests with CGO.
  - The environment variable `C_INCLUDE_PATH` is needed by Clang, but is set temporarily inside the Makefile.
- Open a console.
  - `which arm-none-eabi-gcc` should return `no arm-none-eabi-gcc` if no legacy compilers installed.
- Download latest version from https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads.
- Install exactly into `C:\bin\ArmGNUToolchain` and add `C:\bin\ArmGNUToolchain\bin` it to the **beginning** of the path variable.
  - The path is temporarily extendable inside the manually edited Makefiles as well, but for debugging the Windows path is used directly.
  - The path is used directly also in generated Makefiles.
  - Extending the path variable at the beginning avoids accidentally usage of installed legacy compilers with the same name.
  - To use exactly `C:\bin\ArmGNUToolchain` as install location allows a more easy project collaboration.

####  5.2.2. <a id='macos'></a>MacOS

- In terminal `brew install arm-none-eabi-gcc`
- Restart terminal
- In teminal `arm-non-eabi-gcc --version` delivers `arm-none-eabi-gcc (GCC) 14.2.0`
- In terminal `brew install arm-none-eabi-clang`
- Restart terminal
- In teminal `clang -target arm-none-eabi --version` delivers:
    ```bash
    Apple clang version 15.0.0 (clang-1500.3.9.4)
    Target: arm-none-unknown-eabi
    Thread model: posix
    InstalledDir: /Library/Developer/CommandLineTools/usr/bin
    ```
- In terminal `brew install arm-none-eabi-gdb`
- In terminal `brew install --cask gcc-arm-embedded`

####  5.2.3. <a id='install-arm-clang-(optional)'></a>Install ARM Clang (optional)

With the ARM Clang you get quicker compilation runs and smaller images.

- You need to install ARM GCC as well to use ARM Clang.
  - ARM Clang uses the GCC libraries. For that it looks for `C_INCLUDE_PATH`.
  - ARM Clang uses the GCC debugger. For that it looks into the Windows path variable directly.
- Uninstall existing ARM clang compilers or make sure they are hidden.
- Check if $PATH is clean.
  - In fact you can leave is as it is.
- Check if `C_INCLUDE_PATH` is not defined.
  - It would cause issues performing Go tests with CGO.
- Open a console.
  - `which clang` should return `no clang`.
  - If you have other clang compilers installed, do not touch them.
- Download latest version from https://github.com/llvm/llvm-project/releases.
- Install exactly into `C:\bin\ArmClang` and do **not** add it to path variable.
  - The path is extended temporarily inside the Makefile for the compiler run.

####  5.2.4. <a id='check-project-makefile-(if-it-already-exists)'></a>Check Project Makefile (if it already exists)

- The Makefile should start with these lines:

```mak
# Put ARM Clang first in path temporary to avoid compiler variants issues.
export PATH := C:\bin\ArmClang\bin:$(PATH)

# ARM Clang uses the ARM GNU toolchain libraries and finds them over C_INCLUDE_PATH.
export C_INCLUDE_PATH := C:\bin\ArmGNUToolchain\arm-none-eabi\include
```

The `C:\bin\ArmGNUToolchain\bin:` is in fact not needed, because it must be in the path anyway for debugging.

- `make version` should give output like that:

```bash
$ make version
/c/bin/ArmGNUToolchain/bin/arm-none-eabi-gcc
arm-none-eabi-gcc (Arm GNU Toolchain 12.3.Rel1 (Build arm-12.35)) 12.3.1 20230626
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/c/bin/ArmClang/bin/clang
clang version 17.0.0
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: C:\bin\ArmClang\bin
```

The paths must match with the installation locations.

###  5.3. <a id='setup-stm32'></a>Setup STM32

####  5.3.1. <a id='generate-base-project'></a>Generate Base Project

- Install and start STM32CubeMX code generator.
- Board-Selector -> STM32G0B1KE` or `STM32L432KC` or ...
- (Auto-)Initialize with default values.
- Clock-Generation -> Change PLL *N from "X 16" to "X 32" to get 64 MHz clocks.
  - Running at max clock speed and using `WFE` instructions in wait loops is slightly more energy efficient.
- Project Manager
  - Project
    - Set Project Name
    - Select Project Location
    - Toolchain / IDE -> Select Makefile
  - Code Generator
    - Select "Copy only the necessary library files".
  - Advanced Settings
    - Switch from HAL to LL at least for UART
- Generate Code as Makefile project

####  5.3.2. <a id='update-nucleo-onboard-debugger-(other-st-evaluation-boards-too)'></a>Update NUCLEO Onboard Debugger (other ST evaluation boards too)

(https://www.st.com/en/development-tools/stsw-link007.html)

This step is recommended before re-flashing with the J-Link onboard debugger software.

- Connect STM evaluation board over USB
- Start ST-Link Upgrade (trice\third_party\st.com or look for a newer version at STM.).
  - Device Connect
  - Upgrade Firmware (select version **with** mass storage option)
    - Selecting the other option, would not allow to update with the SEGGER STLinkReflash tool.
  - Close

###  5.4. <a id='setup-onboard-j-link-on-nucleo-(other-st-evaluation-boards-too)'></a>Setup Onboard J-Link on NUCLEO (other ST evaluation boards too)

(https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/)

Using the J-Link onboard debugger software allows parallel debugging and RTT usage.

Unfortunately this is not possible with **v3** onboard debugger hardware! But you can use a J-Link hardware instead. Also it is possible to use a v2 onboard debugger from a different evaluation board or a "Bluepill" Development Board Module with ARM Cortex M3 processor".

- Start STLinkReflash (trice\third_party\segger.com)
  - Accept and Accept
  - 1: Upgrade to J-Link
  - 0: Quit
- Download, extract & start https://github.com/rokath/trice/blob/master/third_party/segger.com/STLinkReflash_190812.zip
  - Re-Flash onboard debugger.
    - You can undo this step anytime. 

###  5.5. <a id='setup-vs-code'></a>Setup VS-Code

- Start VS Code
  - Install Go rich language support if you want to use Go as well (not needed for ARM debugging).
  - Install "Cortex Debug" extension.
  - Open the generated project directory.
  - Click on Run and Debug.
    - Click Generate launch.json and select "Cortex Debug"
  - Open and edit .vscode/launch.json
    - change "executable" value into: "./build/STM32G0B1KE_generated.elf" (example)
  - add lines:
    - `"device": "STM32G0B1KE",` or `"STM32L432KC"` or ...
    - `"svdFile": "./STM32G0B1KE.svd",` or `"./STM32L4x2.svd"` or ...
    - `"runToMain": true`
  - Set the commas right.
- Latest SVD Files can be found here: https://www.st.com/content/st_com/en/search.html#q=svd-t=resources-page=1
- Download file [`./STM32G0B1.svd`](./STM32G0B1.svd) from https://www.st.com/resource/en/svd/stm32G0_svd.zip (example)
  - Alternatively copy it from `"C:\ST\STM32CubeIDE_1.13.1\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.productdb.debug_2.1.0.202306151215\resources\cmsis\STMicroelectronics_CMSIS_SVD\STM32G0B1.svd"` if you have the STM32CubeIDE installed.
  - Download file [`./STM32L4x2.svd`](./STM32L4x2.svd) from https://www.st.com/resource/en/svd/stm32l4_svd.zip (example)
- Installing the **Cortex Debug** extension allow you to debug the target code.

##  6. <a id='makefile-with-clang-too'></a>Makefile with Clang too

- After STM32 CubeMX code generation the Makefile was edited and spitted.
- STM32 CubeMX code generation accepts the edited Makefile, so re-generation is no issue.
  - It modifies the settings according to the changes.

##  7. <a id='download-locations'></a>Download Locations

* [NUCLEO L432 User Manual](../../ref/dm00231744-stm32-nucleo32-boards-mb1180-stmicroelectronics.pdf) (example)

###  7.1. <a id='clang'></a>Clang

https://releases.llvm.org/download.html -> https://github.com/llvm/llvm-project/releases/tag/llvmorg-16.0.0 (example)

###  7.2. <a id='gcc'></a>GCC

https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain -> https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads (example))

##  8. <a id='install-locations'></a>Install Locations

Do not use locations containing spaces, like `C:\Program Files`. Take `C:\bin` for example. This avoids trouble caused by spaces inside path names.

##  9. <a id='environment-variables'></a>Environment Variables

Extend the path variable:

- PATH += `C:\bin\ArmGNUToolchain\bin`
- PATH += `C:\Program Files\SEGGER\JLink`.

##  10. <a id='build-command'></a>Build command

- Clang: `make` or to get it faster `make -j8`.
- GCC: `make GCC`.

##  11. <a id='run-&-debug'></a>Run & Debug

- In terminal after `make` click Run&Debug & click green triangle.

##  12. <a id='logging'></a>Logging

- In terminal type `make log`. This executes the command in project folder:

`trice l -p JLINK -args="-Device STM32G0B1RE -if SWD -Speed 4000 -RTTChannel 0" -pf none -ts ms -d16` (example)

  <img src="./Animation.gif" width="1000">

##  13. <a id='setting-up-a-new-project'></a>Setting up a new project

- Copy this project folder under a new name like `myAwesomeNewProject` or name it as you like.
- Make a temporary folder `myTemp` and generate with STM CubeMX the base project.
- Copy the *.ioc file from `myTemp` to `myAwesomeNewProject` and name it to the project name.
- Compare `myTemp\Makefile` with `myAwesomeNewProject\Makefile` and overwrite/extend in `myAwesomeNewProject\Makefile` the relevant settings, mainly the filenames, include path settings and DEFINES.
- Replace all generated files in `myAwesomeNewProject` with the ones in `myTemp`
- Replace the *.svd file if the MCU is different. You can find it in the internet.
- Run `make -j8` inside `myAwesomeNewProject` to check if all is ok.
- Open the copied *ioc file inside `myAwesomeNewProject` and re-generate and re-build to check.
- Compare the relevant files like `main.c` with the starting project and edit accordingly.
- Adapt `.vscode/launch.json` to the used MCU.
- Than the awesome new project should be ready to go for development.  
