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
- C trace code (`TRICE` macros)  and real-time PC logging with `trice` (tool with Go sources).
- Communication over serial port without strings, just with IDs
- "log in (a) trice" ![](./doc/README.media/life0.gif)
- Main idea: Logging strings **not** into an embedded device to display them later on a PC but keep usage simple.

## `TRICE` macros for C|C++ code
- Real fast (**~16 clocks per trace!!!**) and small loggging technique, a tracer in software usable 
  - for debugging dynamic behaviour during development, 
  - as runtime logger or simply for narrow bandwidth logging in the field even with encryption.
- Usage is similar to 'printf()', but the format strings go not into the target image.
- The needed code instrumentation is minimal:
  - Adapt a [config file](./src.C/proj_config.h) (hardware specific).
  - Add [one small C-file](./src.C/trice.c) to your project and include a [C-header](./src.C/trice.h) where trices are used.
  - Core instrumentation needs less 150 bytes FLASH and about 100 bytes RAM.

## `trice` PC tool
- Manages trices inside a C|C++ source tree during target device compile time.
- Displays trices in realtime during target device runtime.
- Written in Go

## Quick setup
- add [trice.c](./src.C/trice.c) as is to your project
- #include [trice.h](./src.C/trice.h) as is in your source file to use trice
- copy [proj_config.h](./src.C/proj_config.h), rename to `config.h` and adapt to your needs
- [triceCheck.c](./examples/triceDemoF030R8/Src/triceCheck.c) 
is example code and for testing
- run `trice u` in root of your C|Cpp source project after code instrumentation with TRICE* statements to generate a prroject specific til.json file 
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
- see [./doc/ReadMe.md](./doc/ReadMe.md)
