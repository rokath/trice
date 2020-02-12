# **trice** - **TR**ace **I**ds **C** **E**mbedded 
embedded device C printf-like trace code and real-time PC logging (trace ID visualization) over serial port

## About
- C trace (`traceLog`) code and real-time PC logging with `trice` (Go sources).
- Communication over serial port without strings
- "log in (a) trice" ![](./doc/README.media/life0.gif)

## `traceLog`
- Real fast (**~16 clocks per trace!!!**) and small loggging technique, a tracer in software usable 
  - for debugging dynamic behaviour during development, 
  - as runtime logger or simply for narrow bandwidth logging in the field even with encryption.
- Usage is similar to 'printf()', but the format strings go not into the target image.
- The needed code instrumentation is minimal:
  - Adapt a config file (hardware specific).
  - Add one small C-file to your project and include a C-header where traceLogs are used.

## `trice`
- PC tool written in Go
- Manages traceLogs inside a C|C++ source tree during target compile time.
- Displays traceLogs in realtime during target runtime.

## Quick setup
- add [traceLog.c](./scrC/traceLog.c) as is to your project
- #include [traceLog.h](./scrC/traceLog.h) as is in your source file to use tracelog
- copy [proj_traceLogConfig.h](./scrC/proj_traceLogConfig.h), rename to `trceLogConfig.h` and adapt to your needs
- [traceLogCheck.c](./examples/traceLogDemoF030R8/Src/traceLogCheck.c) 
is example code and for testing
- run `trice u` in root of your C|Cpp source project after code instrumentation with TL* statements
- compile, flash & run `trice log -port COMm -baud n` with coccert values m and n

## Documentation
- see [./doc/ReadMe.md](./doc/ReadMe.md)
