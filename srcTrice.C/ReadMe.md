# How to setup a target project

- Copy `_triceConfig.h` to local project, rename to `triceConfig.h` and adapt.
- Add this folder to the target compiler header include path
- Do not add ./intern.
- Do `#include "trice.h"` to your project files files where to use `TRICE\*` macros.
- Add `triceFifo.c` to the target project.
- The other `trice*.c` files are needed in some cases:
  - `triceFifoToBytesBuffer.c` for `wrap` and also for enryption
  - `triceXteaCrypto.c` for encryption
  - `triceSeggerRTT.c` for SEGGER RTT. For that also `../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c` needs to be included and `../third_party/segger.com/SEGGER_RTT/RTT/` should be part of the target compiler header include path.
  - `triceCheck.c` contains test code.
