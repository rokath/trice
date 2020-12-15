# atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx
  - Segger RTT in Direct mode
## Setup steps
- Generate project with STM32CubeMX
  - Select TrueSTUDUIO as toolchain
  - Other settings as you like

  ![atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_0.PNG](./README.media/atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_0.PNG)

## Inside trueStudio
- Select workspace

  ![atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_1.PNG](./README.media/atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_1.PNG)
- Import existing project (do not copy into workspace)

  ![atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_2.PNG](./README.media/atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_2.PNG)
- Build to verify
- New Source Folder `SeggerRTT`
  - Import file system
  - Create Links!

  ![atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_3.PNG](./README.media/atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_3.PNG)
- New Source folder `trice`
  - Import file system
  - Create Links!
  ![atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_4.PNG](./README.media/atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_4.PNG)
- Add include paths
- Add TRICE_VARIANT=SEGGER_RTTD in project settings
- Pre-build steps are not directly supported in trueSTUDIO, so simplply run `trice u` from inside the test folder to update the trice IDs.
- Edit Run/Debug settings and select `Segger J-Link` as Debug probe (you need to convert to the on-board J-Link firmware too, what is a restoreable process)

 ![atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_5.PNG](./README.media/atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_5.PNG)

- Start J-Link RTT Viewer
- Start trice:
  - ~200 clocks

  ![atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_6.PNG](./README.media/atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_6.PNG)


