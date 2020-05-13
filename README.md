# **trice** - **TR**ace **I**ds **C** **E**mbedded *(printf() - replacement)*
embedded device C printf-like trace code and real-time PC logging (trace ID visualization) over serial port

## Info shields
![GitHub Workflow Status](https://img.shields.io/github/workflow/status/rokath/trice/goreleaser)
![GitHub issues](https://img.shields.io/github/issues/rokath/trice)
![GitHub All Releases](https://img.shields.io/github/downloads/rokath/trice/total)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/rokath/trice)
![GitHub watchers](https://img.shields.io/github/watchers/rokath/trice?label=watch)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/rokath/trice)
![GitHub commits since latest release](https://img.shields.io/github/commits-since/rokath/trice/latest)

## Link shields
[![Go Report Card](https://goreportcard.com/badge/github.com/rokath/trice)](https://goreportcard.com/report/github.com/rokath/trice) 
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com)
[![Coverage Status](https://coveralls.io/repos/github/rokath/trice/badge.svg)](https://coveralls.io/github/rokath/trice)

## Search counters
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/trace)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/instrumentation)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/embedded)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/logging)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/real-time)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/debugging)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/monitoring)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/terminal)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/cli)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/diagnostics)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/tool)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/data-recording)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/rtos)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/multi-language-support)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/compression)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/timing-analysis)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/time-measurement)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/golang)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/printf)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/encryption)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/serial)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/C)

## About
- C trace code (`TRICE` macros)  and real-time PC logging with `trice` (tool written in **Go**).
- Communication without string transfer, just with IDs. Prerequisite: byte transmission to PC, low bandwidth is ok:
  - method does'nt matter: serial port, i2c, spi, DAC->ADC, toggle pin, ...
- "log in (a) trice" ![](./docs/README.media/life0.gif)
- Main idea: Logging strings **not** into an embedded device to display them later on a PC but keep usage comfortable and simple. The `TRICE` macros look like printf() but work under the hood completely different.

## `TRICE` macros for C|C++ code
- Real fast (**~16 CPU clocks per trace!!!**) and small loggging technique, a tracer in software usable 
  - for debugging dynamic behaviour during development, 
  - as runtime logger or simply for narrow bandwidth logging in the field even with encryption.
- Usage is similar to 'printf()', but the format strings go not into the target image.
  - Using TRICE in your code reduces the needed FLASH memory.
- The needed code instrumentation is minimal:
  - Adapt a [config file](./srcTrice.C/triceConfig.h) (hardware specific).
  - Add [small C-files](./srcTrice.C/trice.c) to your project and include a [C-header](./srcTrice.C/trice.h) where trices are used.
  - Core instrumentation needs less 150 bytes FLASH and about 100 bytes RAM.
## How it works
- Write `TRICE16( "msg:%d degree\n", temperature );` in source .
- `trice update` changes this line to  `TRICE16( Id(12345), "msg:%d degree\n", temperature );` in source code and adds the *ID 12345* together with *"msg:%d degree\n"* into a trice ID list, a JSON referece file named `til.json`.
- During compilation the TRICE16 macro is expanded only to a *12345* reference and the variable *temperature* and the format string never sees the target.
![trice](./docs/README.media/triceBlockDiagram.svg)
- When the programflow passes the line `TRICE16( "msg:%d degree\n", temperature );` the 16 bit ID *12345* and the 16 bit *-5* are transfered as a 32 bit value into the triceFifo, what goes really fast. This way the program flow is nearly undisturbed, so TRICE macros are usable also inside interrupts or the scheduler.
- For visualization a background service is needed. The triceServe takes the 4 bytes trice values from the triceFifo, adds 4 bytes control information and puts that into the triceWriteBuffer, with at least 8 bytes size.
- The bytes go from triceWriteBuffer to the PC and there the `trice` tool receives them.
- With the help of the `til.json` file the trices get then visualized on the PC.
- It is also possible to let the debug probe transfer the buffer to the PC (see *SeggerRTT* explanation)



## `trice` PC tool
- Manages `TRICE` macro IDs inside a C|C++ source tree and extracts the strings in an ID-string list during target device compile time.
- Displays `TRICE` macros like printf() output in realtime during target device runtime. The received IDs and parameters are printed out.
- Written in Go, simply usage, no installer.

## Quick setup
- add [trice.c](./srcTrice.C/trice.c) and acompanying files as they are to your project
- #include [trice.h](./srcTrice.C/trice.h) as is in your source file to use trice
- copy [triceConfig.h](./srcTrice.C/triceConfig.h) and adapt to your needs
- [triceCheck.c](./srcTrice.C/triceCheck.c) 
is example code and for testing
- run `trice u` in root of your C|Cpp source project after code instrumentation with TRICE* statements to generate a project specific til.json file 
- compile, flash & run `trice log -port COMm -baud n` with correct values m and n

## Possible Use Cases
- Using trice not only for **dynamic debugging** but also as **logging** technique
    is possible and gives the advantage to have very short messages (no strings) for transmission, 
    but keep in mind that the file `til.json` is the key to read all output if your devices in the field for 10 or more years.
- You can consider TRICE also as **a kind of intelligent data compression** what could be interesting for IoT things, especially NB-IoT, where you have very low data rates.
- Also it is possible to **encrypt the 8 byte transfer packets** to get a reasonable protection for many cases.
  - This way you can deliver firmware images with encrypted TRICE output only readable with the appropriate key and til.json.
  - XTEA is a recommendation and implemented as option.
- You can even translate the til.json in **different languages**, so changing a language is just changing the til.json file.
- trice has intentionally no timestamps for performance reasons. But you can add own **timestamps as parameters**. Having several devices with trice timestamps, **network timing measurements** are possible.
- Using trice with an **RTOS** gives the option for detailed **task timing analysis**. Because of the very short execution time of a trice you could add `TRICE16( Id(0), "tim:%d us, task=%d\n", us, nexTask );` to the scheduler and vizualize the output on PC. The same is possible for **interrupt timing analysis**.
- `TRICE16( "%d us\n", sysTick );` before and after a function call lets you easy measure the function execution time.
- As graphical vizualisation you could use a tool similar to https://github.com/sqshq/sampler.

## Documentation
### Common
- see [./docs/Common.md](./docs/Common.md)
### RealTimeTransfer
- see [./docs/SeggerRTT.md](./docs/SeggerRTT.md)
### Examples
- see [./docs/TestExamples.md](./docs/TestExamples.md) and [./docs/triceDemo_NUCLEO-F030R8_LL_MDK-ARM.md](./docs/triceDemo_NUCLEO-F030R8_LL_MDK-ARM.md)
