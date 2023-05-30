# *Trice* memory needs

> _(Read only you are interested in)_

<!-- vscode-markdown-toc -->
* 1. [*Trice* Space example](#TriceSpaceexample)
* 2. [Memory needs (Legacy ARM example project)](#MemoryneedsLegacyARMexampleproject)
* 3. [Memory needs (example projects)](#Memoryneedsexampleprojects)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->
##  1. <a name='TriceSpaceexample'></a>*Trice* Space example

- STM32CubeMX generated empty default project: `Program Size: Code=2208 RO-data=236 RW-data=4 ZI-data=1636`  
- Same project with default `Trice` instrumentation: `Program Size: Code=2828 RO-data=236 RW-data=44 ZI-data=1836`
- Needed [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory): 620 Bytes
- Needed [RAM](https://en.wikipedia.org/wiki/Random-access_memory): 40 Bytes plus 200 Bytes for the 2 times 100 Bytes double buffer
- With increased/decreased buffers also more/less [RAM](https://en.wikipedia.org/wiki/Random-access_memory) is needed.
- With each additional `TRICE` macro a few additional [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory) bytes, like 10 assembler instructions, are needed.
- No printf-like library code is used anymore.
- No format strings go into the target code anymore.
- In general *Trice* instrumentation **reduces** the needed memory compared to a printf-like implementation.

##  2. <a name='MemoryneedsLegacyARMexampleproject'></a>Memory needs (Legacy ARM example project)

The following numbers are measured with a legacy encoding, showing that the instrumentation code can be even smaller.

| Program Size (STM32-F030R8 demo project)      | trice instrumentation | buffer size | compiler optimize for time | comment                         |
|-----------------------------------------------|-----------------------|-------------|----------------------------|---------------------------------|
| Code=1592 RO-data=236 RW-data= 4 ZI-data=1028 | none                  | 0           | off                        | CubeMX generated, no trice      |
| Code=1712 RO-data=240 RW-data=24 ZI-data=1088 | core                  | 64          | off                        | core added without trices       |
| Code=3208 RO-data=240 RW-data=36 ZI-data=1540 | TriceCheckSet()       | 512         | off                        | TRICE_SHORT_MEMORY is 1 (small) |
| Code=3808 RO-data=240 RW-data=36 ZI-data=1540 | TriceCheckSet()       | 512         | on                         | TRICE_SHORT_MEMORY is 0 (fast)  |

* The core instrumentation needs less 150 bytes FLASH and about 100 bytes RAM when buffer size is 64 bytes.
* The about 50 trices in TriceCheckSet() allocate roughly 2100 (fast mode) or 1500 (small mode) bytes.
* trices are removable without code changes by defining `TRICE_OFF` on file or project level.

##  3. <a name='Memoryneedsexampleprojects'></a>Memory needs (example projects)

| Project                       | Compiler    | Optimization | Link-Time-Optimization | Result                                        | Remark  |
| -                             | -           | -            | -                      | -                                             | -       |
| MDK-ARM_STM32F030R8_generated | CLANG v6.19 | -Oz          | yes                    |Code=1020 RO-data=196 RW-data=0 ZI-data=1024   | This is the plain generated project without trice instrumentation.|
| MDK-ARM_STM32F030R8_instrumented | CLANG v6.19 | -Oz          | yes                    |Code=4726 RO-data=238 RW-data=16 ZI-data=4608  | This is with full trice instrumentation with example messages.|

* The size need is less than 4 KB. See also [./TriceConfigProjectImageSizeOptimization.md](./TriceConfigProjectImageSizeOptimization.md).
