# MDK-ARM_RTTD_STM32F0300-DISCO
- About: [SeggerRTT.md](SeggerRTT.md)
- ~250 ticks
- Inside the SysTickISR each millisecond a TRICE message is produced and every 20 milliseconds the full test bunch runs.
- The ISR:alive @20ms message is visible at about 2 3rd of the screen
- SeggerRTT gets the messages every 10ms. Therefore with is traffic the internal buffer is needed to be 7KB. Normally you get away with much less RAM needs.
- Output: ![STM32F0300-DISCO_SeggerRTTD_MDK-ARM](./README.media/STM32F0300-DISCO_SeggerRTTD_MDK-ARM.PNG)
