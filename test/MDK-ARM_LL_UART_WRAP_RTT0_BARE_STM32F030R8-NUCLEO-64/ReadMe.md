# MDK-ARM_LL_UART_WRAP_RTT0_BARE_STM32F030R8-NUCLEO-64

Delivers TRICE logs over UART and RTT channel 0 in "bareL" encoding.

- The UART output is switchable from "bareL" to "wrapL" by defining `TRICE_OUTPUT_WRAPPED`:

```c
#ifdef TRICE_OUTPUT_WRAPPED
	triceServeFifoWrappedToBytesBuffer();
#else
	triceServeFifoSyncedToBytesBuffer();
#endif
```
 
- optionally the "wrapL" output is encryptable by defining `ENCRYPT` this way:

```c
// Enabling next line leads to XTEA encryption  with the key. Only wrapped barel over UART us encrypted right now.
#define ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret
```

## Usage

- Connect board over USB.
- Enter command `trice s` to discover the new COM port.
- When a (SEGGER) JLINK debug probe is active:
  - Enter command `trice l -e bareL -p JLINK` or simply `trice l -e bareL`
- When a STLINK debug probe is active:
  - Enter command `trice l -e bareL -p STLINK` 
- Assumed the serial port is COM3 and TRICE_OUTPUT_WRAPPED **is not** defined:
  - Enter command `trice l -e bareL -p COM3` 
- Assumed the serial port is COM3 and TRICE_OUTPUT_WRAPPED **is** but ÈNCRYPT **is not** defined:
  - Enter command `trice l -e wrapL -p COM3
- Assumed the serial port is COM3 and TRICE_OUTPUT_WRAPPED **is** and ÈNCRYPT **is** defined:
  - Enter command `trice l -e wrapL -p COM3 -password MySecret` 
  - Optionally add `-showKey` to display the used encryption key.
  
## Infos

- The trices are stored in `bareL` format and read out in 8 byte packages.
- The transfer goes over RTT channel 0 and UART.
- A sync package is inserted from time to time in UART transfer:

```c
//! syncLevelLimit is the max amount of trices bulk transmitted without a sync trice injected.
//! Assuming triceServeBareFifoToBytesBuffer() is called every ms this is also the max ms abount without a sync trice.
//! Bigger numbers decrease the bandwidth needs but increase the possible loss of trices in case of
//! re-sync need. A number of 10-100 is recommended: every 10-100 ms or 10-100 trices a sync trice is transmitted.
//! It increases the bandwidth need by 10-1%.
int const syncLevelLimit = 100;
```

Currently this is not done when encryption is active.
