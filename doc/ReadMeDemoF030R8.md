# TraceLog Demo Project NUCLEO-F030R8
## Prerequisites (as example)
- ST Microelectronic evaluation board NUCLEO-F030R8 or similar
  - costs about 10 EUR
- ST Microelectronic CubeMX
  - free
- ARMkeil IDE
  - free for small projects, also free license for some STM devices

## Project setup steps
- Connect evaluation board over USB with PC and detect the virtual COM port `trice log -port COMscan`
- In CubeMx:
  - New project, select NUCLEO-F030R8
    ![](README.media/CubeMX_1.PNG) answer "Yes"
  - Enable USART2 interrupt: ![](README.media/CubeMX_2.PNG)
  - Select LL instead of HAL for USART: ![](README.media/CubeMX_3.PNG)
  - Necessary library files as reference: ![](README.media/CubeMX_4.PNG)
  - Set project name, location and toolchain: ![](README.media/CubeMX_5.PNG)
  - Adjust baudrate of UART2 if you like. In the example project 115200 is used.
  - Generate Code and open project: ![](README.media/CubeMX_6.PNG)
- Inside ARMkeil:
  - To see what to do compare these directories:
    - `./trice/testdata/generatedDemoF030R8`
    - `./trice/testdata/traceLogDemoF030R8`
  - some steps are:
      - Add `traceLog.c` to the project
      - Extend include path with traceLog folder
      - Edit project settings: ![](README.media/ARMkeil_8.PNG)
      - Add to main: ![](README.media/ARMkeil_9.PNG) ![](README.media/ARMkeil_A.PNG)
  