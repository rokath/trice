
Delivers TRICE logs over UART and RTT channel 0 in "esc" encoding:

- Connect board over USB.
- Enter command `trice s` to discover the new COM port.
- Enter command `trice l -e esc -p JLINK` when a (SEGGER) JLINK debug probe is active.
- Enter command `trice l -e esc -p STLINK` when a STLINK debug probe is active.
