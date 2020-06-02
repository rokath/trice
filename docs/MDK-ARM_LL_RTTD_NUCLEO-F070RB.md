# MDK-ARM_LL_RTTD_NUCLEO-F070RB
- About: [SeggerRTT.md](SeggerRTT.md)
- ~ 270 clocks: This is significant slower than with RTT but still so fast, that it is ok for many cases.
- The main advantage here is, that the trices go directly out and need no triceServe in target code.
- The SEGGER-write function is optimizable for speed in special cases.

![triceDemo_NUCLEO-F030RB_LL_SeggerRTTD_MDK-ARM.PNG](./README.media/triceDemo_NUCLEO-F030RB_LL_SeggerRTTD_MDK-ARM.PNG)

- ~ 230 clocks

  ![MDK-ARM_LL_RTTD_NUCLEO-F070RB_0.PNG](./README.media/MDK-ARM_LL_RTTD_NUCLEO-F070RB_0.PNG)