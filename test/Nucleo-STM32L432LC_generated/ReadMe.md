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

## Status

This folder is just as reference for tracking the manual made Makefile changes performed in [../vsCode_Nucle-L432KC_generated](../vsCode_Nucle-L432KC_generated)

