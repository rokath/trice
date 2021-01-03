
Delivers TRICE logs over UART and RTT channel 0 in "bare" encoding:

- Connect board over USB.
- Enter command `trice s` to discover the new COM port.
- Enter command `trice l -e bare -p COM3 -s` when the new COM port is COM3. The `-s` switch shows the raw arriving bytes for issue tracking.
- Enter command `trice l -e bare -p JLINK` when a (SEGGER) JLINK debug probe is active.
- Enter command `trice l -e bare -p STLINK` when a STLINK debug probe is active.
