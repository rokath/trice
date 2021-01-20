
# How to use trice without UART

Because the trice tool needs only to receive a single UART-TX pin will do. But is is also possible to use a GPIO-Pin for trice messages transfer without occupying a UART ressource.

- This slow path is usable because trice needs only few bytes for transmission.
- You can transmit each basic trice (4 or 8 bytes) as bare messages over one pin:

  ![manchester1.PNG](./README.media/manchester1.PNG)
  ![manchester2.PNG](./README.media/manchester2.PNG)

- The 2 images are taken from [https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/](https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/). See there for more information.
- As trace dongle you can use any spare microcontroller board with an UART together with an FTDI USB converter.
  - This allowes also any other data path - method does'nt matter: [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter), [I²C](https://en.wikipedia.org/wiki/I%C2%B2C), [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface), [GPIO](https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/), [RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/), [CAN](https://en.wikipedia.org/wiki/CAN_bus), [LIN](https://en.wikipedia.org/wiki/Local_Interconnect_Network), ...
