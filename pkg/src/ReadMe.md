# How to setup a target project

## Files and Folders

- Files starting with `_` (underscore) are obsolete.

### ./Config/ and ./RTT/

- Copied from "C:\Program Files\SEGGER\JLink\Samples\RTT\SEGGER_RTT_V756a.zip" after installing [https://www.segger.com/downloads/jlink/JLink_Windows_V756a_x86_64.exe](https://www.segger.com/downloads/jlink/JLink_Windows_V756a_x86_64.exe)
- Exchange with latest version

### Target Setup

- Copy `./_triceConfig.h` as `triceConfig.h` to local project and adapt.
- Add this (`trice\pkg\src`) folder to the target compiler header include path.
- Do `#include "trice.h"` to your project files files where to use `TRICE` macros.
- Add `trice.c` to the target project.
- `triceCheck.c` contains test code and is not needed for production code.
- For SEGGER RTT usage the file `./RTT/SEGGER_RTT.c` needs to be included and `./RTT/` should be part of the target compiler header include path.

## Files src.go and src_test.go

- The package src is not needed for the `trice` tool.
- File src_test.go contains test functions to execute the C code during `go test ./...`
- File src.go does the cgo connection. cgo is not supported inside test files.
