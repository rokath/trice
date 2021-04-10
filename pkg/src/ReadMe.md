# How to setup a target project

- Copy `intern/triceConfig.h` to local project and adapt.
- Add this folder to the target compiler header include path.
- Do **not** add ./intern to header search path.
- Do `#include "trice.h"` to your project files files where to use `TRICE` macros.
- Add `trice.c` to the target project.
- `triceCheck.c` contains test code and is not needed for production code.
- For SEGGER RTT `../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c` needs to be included and `../third_party/segger.com/SEGGER_RTT/RTT/` should be part of the target compiler header include path.

## Files src.go and src_test.go

- The package src is not needed for the `trice` tool.
- File src_test.go contains test functions to execute the C code during `go test ./...`
- File src.go does the cgo connection. cgo is not supported inside test files.
