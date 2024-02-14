# Trice on LibOpenCM3

[LibOpenCM3](https://libopencm3.org/) is a hardware abstraction library for many microcontrollers.

This folder is an exampe using STM's [STM32F411 Nucleo](https://www.st.com/en/evaluation-tools/nucleo-f411re.html) board.

## Prerequisites

- Suitable ARM GCC cross compiler (`arm-none-eabi-gcc`) found in your system's PATH
- GNU Make, or compatible
- Environment variable `OPENCM3_DIR` points to the base install of libopencm3.
  This is e.g. the libopencm3 source directory, if you also built it in the source directory.
- OpenOCD

## Usage

- Run `make direct_mode.elf` to compile with Trice mode 0.
- Run `make flash_direct_mode` to program the board.
- Run trice: `trice l -p /dev/ttyACM0`.

## Hint

- This project is an older contribution and not maintained right now.
- To use it, copy for example [../MDK-ARM_STM32F030R8/Core/Inc/triceConfig.h](../MDK-ARM_STM32F030R8/Core/Inc/triceConfig.h) and replace the compiler adaption and hardware specific parts from the [./triceConfig.h](./triceConfig.h) here.
- The trice instrumention code resides now in ../src/ instead of ../pkg/src/ and is divided into several files. This needs to be adapted too to activate this project again.
- Pull requests are welcome.
