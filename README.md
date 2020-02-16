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

## Documentation
- see [./doc/ReadMe.md](./doc/ReadMe.md)
