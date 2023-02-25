# MDK-ARM_STM32F030R8

Delivers parallel TRICE logs over UART and RTT channel 0. See [./Core/Inc/triceConfig.h](./Core/Inc/triceConfig.h) for settings.

## Compile

- Download and install ARM-MDK uVision. It is free for ARM M0/M0+ cores.
- Connect [NUCLEO-STM32F030R8]() board over USB.
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

