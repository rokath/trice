# Example L432KC_gen_ad_toClang_ed_instr

![./Nucleo_STM32L432KC.jpg](./Nucleo_STM32L432KC.jpg)

## Description

- This is a with Trice instrumented project.
- It is for easy compare with  [../L432KC_gen_ad_toClang_ed](../L432KC_gen_ad_toClang_ed) to figure out the needed setup changes.

## Setting Up

- See and adapt steps in [../F030R8_gen/ReadMe.md](../F030R8_gen/ReadMe.md).
- Then add/modify the files to reach this folder layout.

## "Hardware" Changes

- The used evaluation board is delivered with an on-board ST-Link software for debugging.
- This was changed to an on-board J-Link software for better debugging and RTT support.
- See [../../docs/TriceOverRTT.md](../../docs/TriceOverRTT.md) about that.

## Using RTT with on-board J-Link and JLinkRTTLogger

- You need to install the "J-Link Software and Documentation pack" for yout OS.
- [./Core/Inc/triceConfig.h](./Core/Inc/triceConfig.h) contains example Trice log commands.

## Using RTT with on-board J-Link and OpenOCD

### With Windows not possible

- OpenOCD does not support the installed JLink driver.
![./JLinkConfig0.png](./JLinkConfig0.png)
- Changing to the WinUSB buld device driver is here not supported :-(

### Darwin

- See **OpenOCD with Darwin** in [../../docs/TriceOverRTT.md](../../docs/TriceOverRTT.md)

## Using RTT with on-board ST-Link and OpenOCD

**Terminal 1:**

```bash
ms@LenovoP51Win11 MINGW64 /e/repos/trice/examples/L432KC_gen_ad_toClang_ed_inst (devel)
$ openocd -f STLinkOpenOCD.cfg 
Open On-Chip Debugger 0.12.0 (2024-09-16) [https://github.com/sysprogs/openocd]
Licensed under GNU GPL v2
libusb1 d52e355daa09f17ce64819122cb067b8a2ee0d4b
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : The selected transport took over low-level target control. The results might differ compared to plain JTAG/SWD
Info : clock speed 100 kHz
Info : STLINK V2J24M11 (API v2) VID:PID 0483:374B
Info : Target voltage: 72.811768
Info : [stm32l4x.cpu] Cortex-M4 r0p1 processor detected
Info : [stm32l4x.cpu] target has 6 breakpoints, 4 watchpoints
Info : [stm32l4x.cpu] Examination succeed
Info : [stm32l4x.cpu] starting gdb server on 3333
Info : Listening on port 3333 for gdb connections
Info : rtt: Searching for control block 'SEGGER RTT'
Info : rtt: Control block found at 0x2000145c
Info : Listening on port 9090 for rtt connections
Channels: up=1, down=3
Up-channels:
0: Terminal 1024 0
Down-channels:
0: Terminal 16 0
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
```

**Terminal2:**

```bash
ms@LenovoP51Win11 MINGW64 /e/repos/trice/examples/L432KC_gen_ad_toClang_ed_inst (devel)
$ trice l -p TCP4 -args localhost:9090  -pf none -d16
Nov 16 20:38:12.376056  TCP4:       triceExamples.c    10        1_595  Hello! 👋🙂
Nov 16 20:38:12.376056  TCP4:
Nov 16 20:38:12.376056  TCP4:         ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨
Nov 16 20:38:12.376056  TCP4:         🎈🎈🎈🎈  𝕹𝖀𝕮𝕷𝕰𝕺-L432KC   🎈🎈🎈🎈
Nov 16 20:38:12.376056  TCP4:         🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃
Nov 16 20:38:12.376056  TCP4:
Nov 16 20:38:12.376056  TCP4:
Nov 16 20:38:13.891033  TCP4:       triceExamples.c    16       43_439 2.71828182845904523536 <- float number as string
Nov 16 20:38:14.874024  TCP4:       triceExamples.c    17       44_949 2.71828182845904509080 (double with more ciphers than precision)
Nov 16 20:38:15.692614  TCP4:       triceExamples.c    18       45_802 2.71828174591064453125 (float  with more ciphers than precision)
Nov 16 20:38:16.323665  TCP4:       triceExamples.c    19       46_536 2.718282 (default rounded float)
```

## Using On-board ST-Link and VS-Code Cortex-Debug Extension

- [https://www.st.com/resource/en/user_manual/um2576-stm32cubeide-stlink-gdb-server-stmicroelectronics.pdf](https://www.st.com/resource/en/user_manual/um2576-stm32cubeide-stlink-gdb-server-stmicroelectronics.pdf)
- Downloaded and installed
  - [en.stm32cubeprg-win64-v2-17-0.zip]()
  - [en.st-link-server-v2-1-1.zip]()
    - PATH variable extended with `C:\Program Files (x86)\STMicroelectronics\stlink_server`
    - Copied 
      - From: "C:\Program Files (x86)\STMicroelectronics\stlink_server\stlinkserver.exe"
      - To: "C:\Program Files (x86)\STMicroelectronics\stlink_server\ST-LINK_gdbserver.exe"


