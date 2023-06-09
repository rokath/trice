# NUCLEO-STM32L432KC Demo Project

## Setup

- Install and start STM32CubeMX code generator.
- Board-Selector -> NUCLEO-STM32L432KC
- (Auto-)Initialize with default values.
- Clock-Generation -> Change PLL *N from "X 16" to "X 32" to get 64 MHz clocks.
- Project Manager
  - Project
    - Set Project Name
    - Select Project Location
    - Toolchain / IDE -> Select Makefile
  - Code Generator
    - Select "Copy only the necessary library files".
  - Advanced Settings
    - Switch from HAL to LL at least for UART
- Generate Code
- Connect NUCLEO-STM32L432KC over USB
- Start ST-Link Upgrade
  - Device Connect
  - Upgrade Firmware
  - Close
- Start STLinkReflash
  - Accept and Accept
  - 1: Upgrade to J-Link
  - 0: Quit
- Start VS Code
  - Install "Cortex Debug" extension
  - Open the generated project directory
  - Click on Run and Debug
    - Click Generate launch.json and select "Cortex Debug"
  - Open and edit .vscode/launch.json
    - change "executable" value into: "./build/Nucleo-STM32L432LC_generated.elf"
  - add lines:
    - `"device": "STM32L432KC",`
    - `"svdFile": "./STM32L4x2.svd",`
    - `"runToMain": true`
  - Set the commas right. 
- Download file [`./STM32L4x2.svd`](./STM32L4x2.svd) from https://www.st.com/resource/en/svd/stm32l4_svd.zip
- In terminal: `make`
- Click Run&Debug & click green triangle

## Makfile with Clang too

- Afterwards the Makefile was edited and splitted.

## Download Locations

* [NUCLEO L432 User Manual](../../ref/dm00231744-stm32-nucleo32-boards-mb1180-stmicroelectronics.pdf)

### Clang

https://releases.llvm.org/download.html -> https://github.com/llvm/llvm-project/releases/tag/llvmorg-16.0.0

### GCC

https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain -> https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

## Install Locations

Do not use locations containing spaces, like "C:\Program Files". Take "C:\bin" for example. This avoids trouble cause by spaces inside path names.

## Environment Variables

- PATH += C:\bin\LLVM\bin
- PATH += C:\bin\12.2_mpacbti-rel1\bin
- C_INLUDE_PATH = C:\bin\12.2_mpacbti-rel1\arm-none-eabi\include
  - if clang isssue: "fatal error: 'stdlib.h' file not found"

## Build

`make` or to get it faster `make -j8`.
