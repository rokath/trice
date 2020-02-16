# **trice** - **TR**ace **I**ds **C** **E**mbedded 
embedded device C printf-like trace code and real-time PC logging (trace ID visualization) over serial port

## About
- C trace (`traceLog`) code and real-time PC logging with `trice` (Go sources).
- Communication over serial port without strings, just with IDs
- "log in (a) trice" ![](./doc/README.media/life0.gif)
- Main idea: Logging strings **not** into an embedded device to display them later on a PC but keep usage simple.

## `traceLog`
- Real fast (**~16 clocks per trace!!!**) and small loggging technique, a tracer in software usable 
  - for debugging dynamic behaviour during development, 
  - as runtime logger or simply for narrow bandwidth logging in the field even with encryption.
- Usage is similar to 'printf()', but the format strings go not into the target image.
- The needed code instrumentation is minimal:
  - Adapt a [config file](./scrC/proj_traceLogConfig.h) (hardware specific).
  - Add [one small C-file](./scrC/traceLog.c) to your project and include a [C-header](./scrC/traceLog.h) where traceLogs are used.
  - Core instrumentation needs less 150 bytes FLASH and about 100 bytes RAM.

## `trice`
- PC tool written in Go
- Manages traceLogs inside a C|C++ source tree during target compile time.
- Displays traceLogs in realtime during target runtime.

## Quick setup
- add [traceLog.c](./scrC/traceLog.c) as is to your project
- #include [traceLog.h](./scrC/traceLog.h) as is in your source file to use traceLog
- copy [proj_traceLogConfig.h](./scrC/proj_traceLogConfig.h), rename to `trceLogConfig.h` and adapt to your needs
- [traceLogCheck.c](./examples/traceLogDemoF030R8/Src/traceLogCheck.c) 
is example code and for testing
- run `trice u` in root of your C|Cpp source project after code instrumentation with TL* statements to generate a prroject specific til.json file 
- compile, flash & run `trice log -port COMm -baud n` with correct values m and n

## Possible Use Cases
- Using traceLog not only for **dynamic debugging** but also as **logging** technique
    is possible and gives the advantage to have very short messages (no strings) for transmission, 
    but keep in mind that the file `til.json` is the key to read all output if your devices in the field for 10 or more years.
- You can consider Trice also as **a kind of intelligent data compression** what could be interesting for IoT things, especially NB-IoT, where you have very low data rates.
- Also it is possible to **encrypt the 8 byte transfer packets** to get a reasonable protection for many cases.
  - Treyfer is a recommendation and planned as a coming option.
- You can even translate the til.json in **different languages**, so changing a language is just changing the til.json file.
- traceLog has intentionally no timestamps for performance reasons. But you can add own **timestamps as parameters**. Having several devices with traceLog timestamps, **network timing measurements** are possible.
- Using Trice with an **RTOS** gives the option for detailed **task timing analysis**. Because of the very short execution time of a traceLog you could add `TL16( Id(0), "tim:%d us, task=%d\n", us, nexTask );` to the scheduler and vizualize the output on PC. The same is possible for **interrupt timing analysis**.
- As graphical vizualisation you could use a tool similar to https://github.com/sqshq/sampler.

## Documentation
- see [./doc/ReadMe.md](./doc/ReadMe.md)
