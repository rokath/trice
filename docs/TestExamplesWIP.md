# Test examples work in progress

## atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx
### Setup steps
- Generate project with STM32CubeMX
  - Select TrueSTUDUIO as toolchain
  - Other settings as you like

  ![atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_0.PNG](./README.media/atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx_0.PNG)
### Inside trueStudio
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


## atollicTrueSTUDIO_ITM_Printf_DISCOVERY-STM32F407VGTx
This example contains no trice code. it simply emits a `A` over the ITM just for test purpose.
- First steps like in `atollicTrueSTUDIO_RTTD_DISCOVERY-STM32F407VGTx`
- Reflash on-board DEbugger to ST-Link:

![atollicTrueSTUDIO_ITM_Printf_DISCOVERY-STM32F407VGTx_0.PNG](./README.media/atollicTrueSTUDIO_ITM_Printf_DISCOVERY-STM32F407VGTx_0.PNG)
- Follow instructions in [http://blog.atollic.com/cortex-m-debugging-printf-redirection-to-a-debugger-console-using-swv/itm-part-1](http://blog.atollic.com/cortex-m-debugging-printf-redirection-to-a-debugger-console-using-swv/itm-part-1)

![atollicTrueSTUDIO_ITM_Printf_DISCOVERY-STM32F407VGTx_1.PNG](./README.media/atollicTrueSTUDIO_ITM_Printf_DISCOVERY-STM32F407VGTx_1.PNG)

- Done:
![atollicTrueSTUDIO_ITM_Printf_DISCOVERY-STM32F407VGTx_2.PNG](./README.media/atollicTrueSTUDIO_ITM_Printf_DISCOVERY-STM32F407VGTx_2.PNG)
![atollicTrueSTUDIO_ITM_Printf_DISCOVERY-STM32F407VGTx_3.PNG](./README.media/atollicTrueSTUDIO_ITM_Printf_DISCOVERY-STM32F407VGTx_3.PNG)

Additional info: [Atollic_Serial_wire_viewer_realtime_tracing_whitepaper.pdf](https://cta-service-cms2.hubspot.com/ctas/v2/public/cs/c/?cta_guid=a9fb94cd-a492-4ce1-a7a8-1ca091fb448c&placement_guid=c92954ee-825c-40d4-994a-b785f7d7a5c6&portal_id=460400&canon=http%3A%2F%2Fblog.atollic.com%2Fcortex-m-debugging-printf-redirection-to-a-debugger-console-using-swv%2Fitm-part-1&redirect_url=APefjpGyT0W9LE88W87nq6QXwZTwmLnXriQZPLw9IQ8TCzA8_DA6Z5ty7But2KEAkpTgZ8Y93csjKitMoFM8BTWbwe1N6wYunE3w-ahMB52JIb2rGjr__dfnfSaIBFBRRUa3IfRFl0fiWfyZ0dMrn2VOLxj3JHsVV7x4kdeY5w05y51yNkErUXVx1pCuuEIfTUa9SjFosD5vKgLdciZqwe_gJW-AfRYjfI1ESf96OcFYA-BM3VqTHslTpTcWC2BctmccdvxlTcn3Z3QmvhBbaQuJ8FXJmLaXhrTIbg3KideZgcQgfiqk0ov_PoIJdgYu1cExqwSnQ8qtepE0GU0b0NgS8QPttn9l3_ejk-QPSpj8ufWhhoVM-QQmY4K782K09bbp7pHPcfr8XYhjKmTQWNjPXVnCjUM26g&click=3435c228-20ad-462f-bfca-a363163cda1a&hsutk=c23278c474639c1c4facdc8acaf0df14&signature=AAH58kF-CxFiZnIMOZ2P9W3apJogP29Klw&utm_referrer=https%3A%2F%2Fwww.google.com%2F&pageId=2534487833&contentType=blog-post&__hstc=62657011.c23278c474639c1c4facdc8acaf0df14.1591087383657.1591087383657.1591087383657.1&__hssc=62657011.1.1591087383657&__hsfp=2517265284)
## Arduino
- install arduino-cli

## PICkit-44-Pin-DEMO-Board_PIC18F45K20 (triceDemoPIC18F45K20)
- Download & install [MPLAB-X IDE](https://www.microchip.com/mplab/mplab-x-ide)
  - install Plugin MPLAB Code-generator [https://www.microchip.com/mplab/mplab-code-configurator](https://www.microchip.com/mplab/mplab-code-configurator)
- [PIC_C-Compilervergleich](https://www.mikrocontroller.net/articles/PIC_C-Compilervergleich)
- Download & install XC8 compiler [XC8 compiler](https://www.microchip.com/mplab/compilers) 
  - FREE version will do.
- pdf: [PICkit 3 Debug Express PIC18F45K20 - MPLAB C Lessons.pdf](../third_party/PICkit-44-Pin-DEMO-Board_PIC18F45K20PICkit 3 Debug Express PIC18F45K20 - MPLAB C Lessons.pdf)
- photo: ![PICkit-44-Pin-DEMO-Board_PIC18F45K20](./README.media/PICkit-44-Pin-DEMO-Board_PIC18F45K20.jpg)
- schematic: ![44-PIN-Demo-Board-Schematic_PickIt3_Sacban](./README.media/44-PIN-Demo-Board-Schematic_PickIt3_Sacban.jpg)
- [PICkit 3 Debug Express Lessons.zip](../third_party/PICkit%203%20Debug%20Express%20Lessons.zip)
- photo: ![PICkit4Anschluss](./README.media/PICkit4Anschluss.jpeg)
- [PIC18F45K20 overview](https://www.microchip.com/wwwproducts/en/PIC18F45K20)

### LPC800
- [LPC800frontside](./README.media/LPC800frontside.jpg)
- [LPC800fbackside](./README.media/LPC800backside.jpg)
- [https://community.nxp.com/community/general-purpose-mcus/lpc/blog/2017/03/20/lpc800-dip-schematic](https://community.nxp.com/community/general-purpose-mcus/lpc/blog/2017/03/20/lpc800-dip-schematic)
- Device  LPC824M201JHI33  (or alternatively LPC834M101FHI33)
- [www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx](www.silabs.com/products/mcu/Pages/USBtoUARTBridgeVCPDrivers.aspx)
- [https://mcuoneclipse.com/2017/05/13/mcuxpresso-ide-blinky-the-nxp-lpc800-dip-board/](https://mcuoneclipse.com/2017/05/13/mcuxpresso-ide-blinky-the-nxp-lpc800-dip-board/)
- Download, install & start Xpresso
- Press Download and install SDKs

### triceDemo_NUCLEO-F070RB_Zephyr
### triceDemo_NUCLEO-F070RB_mbed
### blinky
### mbedIDE
### tryout
### mbed IDE
