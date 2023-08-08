# Example Projects without and with Trice Instrumentation

- All used tools are **Open Source**.
- All provided information is just as example and needs adaption to your needs.
- There is no need to setup the environment in the given order.

## Examples

| Project Name | Description |
| - | - |
| [MDK-ARM_STM32F030R8_generated](./MDK-ARM_STM32F030R8_generated) | For the [NUCLEO-STM32F030R8](https://www.st.com/en/evaluation-tools/nucleo-f030r8.html) with the free [CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) generated empty project as compare base for the trice instrumentation. |
| [MDK-ARM_STM32F030R8_instrumented](./MDK-ARM_STM32F030R8_instrumented) | The [MDK-ARM_STM32F030R8_generated](./MDK-ARM_STM32F030R8_generated) project was trice instrumented for parallel usage of RTT and UART transfer. |
|||
| [OpenCM3_STM32F411_Nucleo](./OpenCM3_STM32F411_Nucleo) | See the [Readme.md](./OpenCM3_STM32F411_Nucleo/Readme.md) in this folder. |
|||
| [Nucleo-STM32L432KC_generated](./Nucleo-STM32L432KC_generated) | This is a minimal STM32CubeMX generated Makefile project. |
| [Nucleo-STM32L432KC_adapted_toClang](./Nucleo-STM32L432KC_adapted_toClang) | This is a minimal STM32CubeMX generated Makefile project extended to compile also with Clang trying to perform minimal changes. It produces some warnings, because it is not finetuned. The [vsCode_Nucleo-L432KC_generated](./vsCode_Nucleo-L432KC_generated) project is then a next step performable. |
| [vsCode_Nucleo-L432KC_generated](./vsCode_Nucleo-L432KC_generated) | This is a minimal FreeRTOS STM32CubeMX generated Makefile project adapted to Clang and GCC. |
| [vsCode_Nucleo-L432KC_instrumented](./vsCode_Nucleo-L432KC_instrumented) | This is a minimal FreeRTOS STM32CubeMX generated Makefile project adapted to Clang and GCC and afterward instrumented with the Trice library. |
|||
| [vsCode_Nucleo-G0B1_generated](./vsCode_Nucleo-G0B1_generated) | This is a minimal FreeRTOS STM32CubeMX generated Makefile project adapted to Clang and GCC. |
| [vsCode_Nucleo-G0B1_instrumented](./vsCode_Nucleo-G0B1_instrumented) | This is a minimal FreeRTOS STM32CubeMX generated Makefile project adapted to Clang and GCC and afterward instrumented with the Trice library. |
|||

## Important to know

The [ARM-Keil µVision IDE](https://www2.keil.com/mdk5/uvision/) does sometimes not recognize external file modifications. That means for example: After editing `main.c` by adding a `trice( "Hi!\n" )` and executing `trice u` as pre-compile step it could happen, that an updated `trice( iD(12345), "Hi!\n" )`  was inserted and correct compiled but the update in `main.c` is not shown. Simply close and reopen `main.c` before editing again. This seem to be a [ARM-Keil µVision IDE](https://www2.keil.com/mdk5/uvision/) "feature".

## Animation

(The trice ID occurs just during the compilation.)

![x](./Animation.gif)

## Setup PC

Setting up a PC is for for Linux mostly straightforward but Windows PCs are more problematic. The steps shown here are just one example.

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
- Setup J-Link if you use this debug probe as hardware or software (see below).
  - Install SEGGER [J-Link Software and Documentation Pack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack)

### Setup Trice

- from inside folder `repos` clone trice repo with `git clone https://github.com/rokath/trice.git`.
- Run `go install ./...` from folder `repos/trice`.

OR

- Download the latest release archive and extract.
- Put trice binary into C:\bin.
- Put trice/src into `repos` if you want access the trice library code from several projects and have it only once.
  - Alternatively copy it into your project.

### Setup ARM Environment

#### Install ARM GCC

- Uninstall existing ARM GCC compilers optionally.
- Check if $PATH is clean.
  - In fact you can leave is as it is.
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

#### Install ARM Clang (optional)

With the ARM Clang you get a quicker compilation runs and smaller images.

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

#### Check Project Makefile (if it already exists)

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

### Setup STM32

#### Generate Base Project

- Install and start STM32CubeMX code generator.
- Board-Selector -> NUCLEO-STM32G0B1KE` or `NUCLEO-STM32L432KC` or ...
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

#### Update NUCLEO Onboard Debugger (other ST evaluation boards too)

This step is recommended before re-flashing with the J-Link onboard debugger software.

- Connect STM evaluation board over USB
- Start ST-Link Upgrade (trice\third_party\st.com or look for a newer version at STM.).
  - Device Connect
  - Upgrade Firmware (select version **with** mass storage option)
    - Selecting the other option, would not allow to update with the SEGGER STLinkReflash tool.
  - Close

### Setup Onboard J-Link on NUCLEO (other ST evaluation boards too)

Using the J-Link onboard debugger software allows parallel debugging and RTT usage.

Unfortunately this is not possible with **v3** onboard debugger hardware! But you can use a J-Link hardware instead. Also it is possible to use a v2 onboard debugger from a different evaluation board or a "Bluepill" Development Board Module with ARM Cortex M3 processor".

- Start STLinkReflash (trice\third_party\segger.com)
  - Accept and Accept
  - 1: Upgrade to J-Link
  - 0: Quit
- Download, extract & start https://github.com/rokath/trice/blob/master/third_party/segger.com/STLinkReflash_190812.zip
  - Re-Flash onboard debugger.
    - You can undo this step anytime. 

### Setup VS-Code

- Start VS Code
  - Install Go rich language support if you want to use Go as well (not needed for ARM debugging).
  - Install "Cortex Debug" extension.
  - Open the generated project directory.
  - Click on Run and Debug.
    - Click Generate launch.json and select "Cortex Debug"
  - Open and edit .vscode/launch.json
    - change "executable" value into: "./build/Nucleo-STM32G0B1KE_generated.elf" (example)
  - add lines:
    - `"device": "STM32G0B1KE",` or `"STM32L432KC"` or ...
    - `"svdFile": "./STM32G0B1KE.svd",` or `"./STM32L4x2.svd"` or ...
    - `"runToMain": true`
  - Set the commas right. 
- Download file [`./STM32G0B1KE.svd`](./STM32G0B1KE.svd) from https://www.st.com/resource/en/svd/stm32G0_svd.zip (example)
- Download file [`./STM32L4x2.svd`](./STM32L4x2.svd) from https://www.st.com/resource/en/svd/stm32l4_svd.zip (example)

## Makefile with Clang too

- After STM32 CubeMX code generation the Makefile was edited and spitted.
- STM32 CubeMX code generation accepts the edited Makefile, so re-generation is no issue.
  - It modifies the settings according to the changes.

## Download Locations

* [NUCLEO L432 User Manual](../../ref/dm00231744-stm32-nucleo32-boards-mb1180-stmicroelectronics.pdf) (example)

### Clang

https://releases.llvm.org/download.html -> https://github.com/llvm/llvm-project/releases/tag/llvmorg-16.0.0 (example)

### GCC

https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain -> https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads (example))

## Install Locations

Do not use locations containing spaces, like `C:\Program Files`. Take `C:\bin` for example. This avoids trouble caused by spaces inside path names.

## Environment Variables

Extend the path variable:

- PATH += `C:\bin\ArmGNUToolchain\bin`
- PATH += `C:\Program Files\SEGGER\JLink`.

## Build command

- Clang: `make` or to get it faster `make -j8`.
- GCC: `make GCC`.

## Run & Debug

- In terminal after `make` click Run&Debug & click green triangle.

## Logging

- In terminal type `make log`. This executes the command in project folder:

`trice l -p JLINK -args="-Device STM32G0B1RE -if SWD -Speed 4000 -RTTChannel 0" -pf none -ts ms -d16` (example)

![x](./Animation.gif)

## Setting up a new project

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
