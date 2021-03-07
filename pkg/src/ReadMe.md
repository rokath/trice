# How to setup a target project

- Copy `intern/triceConfig.h` to local project and adapt.
- Add this folder to the target compiler header include path.
- Do **not** add ./intern to header searchh path.
- Do `#include "trice.h"` to your project files files where to use `TRICE\*` macros.
- Add `trice.c` to the target project.
- The other `trice*.c` files are needed in some cases:
  - `triceXteaCrypto.c` for encryption
  - `triceSeggerRTT.c` for SEGGER RTT. For that also `../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c` needs to be included and `../third_party/segger.com/SEGGER_RTT/RTT/` should be part of the target compiler header include path.
  - `triceCheck.c` contains test code.
  
## Files src.go and src_test.go

- The package src is not needed for the `trice` tool.
- File src_test.go contains test functione to execute the C code during `go test ./...`
- File src.go does the cgo connection. cgo is not supported inside test files.
