# Target Setup

* Copy `triceConfig.h` from a test project to local project and adapt.
* Do not add files inside `./cgo`, they used by CGO for trice testing internally.
  * As a starting point for your project specific `triceConfig.h` grab into the trice/test folder.
* Add this (`trice\pkg\src`) folder to the target compiler include path.
  * If you prefer a copy, you need for trice TREX encoding:

  ```bash
  tcobs.h
  tcobsv1Encode.c
  tcobsv1Internal.h
  trice.c
  trice.h
  triceXTEA.c
  triceXTEA.h 
  ```

* The XTEA files ar only needed if you use XTEA encryption.
* The TCOBS files are copied from [https://github.com/rokath/tcobs/tree/master/TCOBSv1](https://github.com/rokath/tcobs/tree/master/TCOBSv1). They are maintained there.
* Do `#include "trice.h"` to your project files where to use `TRICE` macros.
* Add only and only `trice.c` to the target project.
* `triceCheck.c` contains test code and is not needed for production code, but you can temporarily add it to your project for checking.
* Ignore `trice_test.c` - this is used from Go when `go test ./...` is executed.
* For SEGGER RTT usage the file `../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c` needs to be included and `../../third_party/segger.com/SEGGER_RTT/RTT/` should be part of the target compiler include path.

## Files `src.go` and `src_test.go`

* The package src is not needed for the `trice` tool.
* File src_test.go contains test functions to execute the C code during `go test ./...`
* File `src.go` does the cgo connection. cgo is not supported inside test files.
