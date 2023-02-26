# MDK-ARM_STM32F030R8

Delivers parallel TRICE logs over UART and RTT channel 0. See [./Core/Inc/triceConfig.h](./Core/Inc/triceConfig.h) for settings.

## Folders & Files

| Name | Content |
| -    | -       |
| [./Core](./Core) | [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) generated code. 2 files added: [./Core/Inc/triceConfig.h](./Core/Inc/triceConfig.h) and  [./Core/Inc/SEGGER_RTT_Conf.h](./Core/Inc/SEGGER_RTT_Conf.h) |
| [./Drivers](./Drivers) | [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) generated code. |
| [./MDK-ARM](./MDK-ARM) | [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) generated code. `./MDK-ARM/MDK-ARM_STM32F030R8.uvprojx` is the project file, which was edited with the [µVision IDE](https://www2.keil.com/mdk5/uvision/). As debug probe was J-Link set.|


## Compile

- Download and install ARM-MDK uVision. It is free for ARM M0/M0+ cores.
- Connect [NUCLEO-STM32F030R8](https://www.st.com/en/evaluation-tools/nucleo-f030r8.html) board over USB.
- Open `/MDK-ARM/MDK-ARM_STM32F030R8.uvprojx` compile and download.

## UART Logging

- Enter command `trice s` to discover the new COM port.
- Enter command `trice l -p COM3` when the new COM port is COM3.

## RTT Logging

See [TriceOverRTT](../../docs/TriceOverRTT.md).

### ST-LINK 

- Install ST-LINK software. 
- Enter command `trice l -p ST-LINK -args "-Device STM32F030R8 -If SWD -Speed 4000 -RTTChannel 0"`

### J-LINK

- Install J-LINK software. 
- Enter command `trice l -p J-LINK -args "-Device STM32F030R8 -If SWD -Speed 4000 -RTTChannel 0"`

