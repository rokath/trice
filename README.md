# **trice** - **TR**ace **I**ds **C** **E**mbedded 
embedded device C printf-like trace code and real-time PC logging (trace ID visualization)

## About
- C trace (`traceLog`) code and real-time PC logging with `trice` (Go sources).
- Communication over serial port
- "log in (a) trice"

## `traceLog`
- Real fast (~16 clocks per trace!!!) and small loggging technique, a tracer in software usable for analyzing dynamic states during development, runtime or simply for narrow bandwidth logging in the field.
  - Usage is similar to 'printf()', but the format strings go not into the target image.
  - The needed code instrumentation is minimal:
    - Adapt a config file and the hardware layer.
    - Add one small C-file to your project and include a C-header where traceLogs are used.

## `trice`
- PC tool written in Go
- Manages traceLogs inside a C|C++ source tree during target compile time.
- Displays traceLogs in realtime during target runtime.

## Documentation
- see [./doc/README.md](./doc/README.md)
