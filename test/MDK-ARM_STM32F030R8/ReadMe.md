# MDK-ARM_STM32F030R8

Delivers TRICE logs over UART and RTT channel 0. See `TRICE_MODE` in "Core/Inc/triceConfig.h" for settings.

## Usage

- Connect board over USB.
- Enter command `trice s` to discover the new COM port.
- Enter command `trice l -p COM3 -s` when the new COM port is COM3. The `-s` switch shows the raw arriving bytes for issue tracking.
- Enter command `trice l -p ST-LINK` when an ST-LINK debug probe is active. Consider the needed `-args` switch and additional installation.
- Enter command `trice l -p J-LINK` when a SEGGER J-LINK debug probe is active. Consider the needed `-args` switch and additional installation.

## Info

- The transfer goes over RTT channel 0 or UART.
