# MDK-ARM_LL_UART_demoBoard_STM32F030F4F4P6

- ~100 clocks (without time optimization)
- See [STM32F030F4P6-DEMO-BOARD](https://stm32-base.org/boards/STM32F030F4P6-STM32F030-DEMO-BOARD-V1.1.html) (same as No logo JF-Xuan STM32F030F4P6 Cortex M0 ARM 32 Bit 48MHz Core System Development Board) for hardware information
  - The board is very sensible against electro-static influence - be careful!
- The next 2 images show part of a NUCLEO-Board used only as ST-Link
  - The 2 jumpers at CN2 are removed
  - RED is 3.3V power supply
  - GREEN is Ground
  - YELLOW is SWCLK
  - ORANGE is SWDIO
  - BROWN is UART-TX out to UART-RX/RS232/USB/virtual UART (Ground cannection is over USB)
    - The small blue covered piece is s normal USB/Serial converter (FTDI)
- photo: ![demoBoard-STM32F030F4P6a](./README.media/demoBoard-STM32F030F4P6a.jpeg)
- photo: ![demoBoard-STM32F030F4P6b](./README.media/demoBoard-STM32F030F4P6b.jpeg)
- output: ![demoBoard-STM32F030F4P6.PNG](./README.media/demoBoard-STM32F030F4P6.PNG)
- photo: ![demoBoard-STM32F030F4P6_USB.jpg](./README.media/demoBoard-STM32F030F4P6_USB.jpg)
- Using the ST-Link it is possible to debug step the code
- An other option would be to download the HEX-File using the internal STM32 ROM-Bootloader over the USB/Serial converter. Then no ST-Link is needed but you can only run the code and not debug.
- Finally: The USB connector on the demoBoard-STM32F030F4P6 ist only used for power supply if no other connections.
