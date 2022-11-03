# Target Setup

* Do not add files inside `./cgo`, they used by CGO for trice testing internally.
  * As a starting point for your project specific `triceConfig.h` grab into the trice/test folder and copy `triceConfig.h` from a test project to your project and adapt it then.

* Add this (`trice\pkg\src`) folder to the target compiler include path OR, if you prefer, copy the files you need.
* Include the used *.c` files to your project.
* Files overview:
  
  | File                                               | description |
  | -                                                  | -           |
  | `trice.h` & `trice.c`                              | trice runtime lib core, `#include trice.h` in project files, where to use `TRICE` macros. |
  | `triceStackBuffer.c`                               | trice runtime lib extension needed for direct mode |
  | `triceDoubleBuffer.c`                              | trice runtime lib extension needed for fastest indirect mode |
  | `triceStreamBuffer.c`                              | trice runtime lib extension needed for recommended indirect mode |
  | `tcobs.h`, `tcobsv1Internal.h` & `tcobsv1Encode.c` | message compression and packaging, the `*.h` files need **no** `#include ...` |
  | `xtea.h` & `xtea.c`                                | UNTESTED with TREX, needed for XTEA message encryption, if enabled |
  | `triceCheck.c` | contains test code and is not needed for production code, but you can temporarily add it to your project for checking |
  | `trice_test.c` | ignore, this file is used from Go when `go test ./...` is executed. |

* The TCOBS files are copied from [https://github.com/rokath/tcobs/tree/master/TCOBSv1](https://github.com/rokath/tcobs/tree/master/TCOBSv1). They are maintained there and extensively tested and probably not a matter of significant change.
* For SEGGER RTT usage, the file `../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c` needs to be included and `../../third_party/segger.com/SEGGER_RTT/RTT/` should be part of the target compiler include path. You could check for a newer version at [https://www.segger.com/downloads/jlink/](https://www.segger.com/downloads/jlink/).

## Files `src.go` and `src_test.go`

* The package src is not needed for the `trice` tool.
* File src_test.go contains test functions to execute the C code during `go test ./...`
* File `src.go` does the cgo connection. cgo is not supported inside test files.
