<div id="top"> 

# Trice User Manual

<details><summary>Table of Contents</summary><ol><!-- TABLE OF CONTENTS START -->

<!-- 
Table of Contents Generation:
- Install vsCode extension "Markdown TOC" from dumeng 
- Use Shift-Ctrl-P "markdownTOC:generate" to get the automatic numbering.
- replace "<a id=" with "<a id=" 
-->

<!-- vscode-markdown-toc -->
* 1. [Trice Speed](#trice-speed)
  * 1.1. [Target Implementation Options](#target-implementation-options)
    * 1.1.1. [Trice Use Cases `TRICE_STATIC_BUFFER` and `TRICE_STACK_BUFFER` - direct mode only](#trice-use-cases-`trice_static_buffer`-and-`trice_stack_buffer`---direct-mode-only)
    * 1.1.2. [Trice Use Case `TRICE_DOUBLE_BUFFER` - deferred mode, fastest *trice* execution, more RAM needed](#trice-use-case-`trice_double_buffer`---deferred-mode,-fastest-*trice*-execution,-more-ram-needed)
    * 1.1.3. [Trice Use Case `TRICE_RING_BUFFER` - deferred mode, balanced *trice* execution time and needed RAM](#trice-use-case-`trice_ring_buffer`---deferred-mode,-balanced-*trice*-execution-time-and-needed-ram)
* 2. [Trice memory needs](#trice-memory-needs)
  * 2.1. [Trice Space example](#trice-space-example)
  * 2.2. [Memory needs (Legacy ARM example project)](#memory-needs-(legacy-arm-example-project))
  * 2.3. [Memory needs (example projects)](#memory-needs-(example-projects))

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

</div></ol></details><!-- TABLE OF CONTENTS END -->

##  1. <a id='trice-speed'></a>Trice Speed

 A `TRICE` macro execution can be as cheap like **3-4 Assembler instructions or 6-8 processor clocks**:

* Disassembly: ![./ref/MEASURE_executionCode.PNG](./ref/MEASURE_executionCode.PNG)
* Measurement: The blue SYSTICK clock counts backwards 6 clocks for each `TRICE` macro (on an ARM M0+), what is less than 100 ns @64 MHz MCU clock: ![./ref/MEASURE_executionClocks.PNG](./ref/MEASURE_executionClocks.PNG)

A more realistic (typical) timing with target location and µs timestamps, critical section and parameters is shown here with the STM32F030 M0 core:

![./ref/F030FullTiming.PNG](./ref/F030FullTiming.PNG)

The MCU is clocked with 48 MHz and a *Trice* duration is about 2 µs, where alone the internal ReadUs() call is already nearly 1 µs long:

![./ref/ReadUsF030.PNG](./ref/ReadUsF030.PNG)

###  1.1. <a id='target-implementation-options'></a>Target Implementation Options

All trice macros use internally this sub-macro:

```C
#define TRICE_PUT(x) do{ *TriceBufferWritePosition++ = TRICE_HTOTL(x); }while(0); //! PUT copies a 32 bit x into the TRICE buffer.
```

The usual case is `#define TRICE_HTOTL(x) (x)`. The `uint32_t* TriceBufferWritePosition` points to a buffer, which is codified and used with the *trice* framing sub-macros `TRICE_ENTER` and `TRICE_LEAVE` in dependence of the use case.

####  1.1.1. <a id='trice-use-cases-`trice_static_buffer`-and-`trice_stack_buffer`---direct-mode-only'></a>Trice Use Cases `TRICE_STATIC_BUFFER` and `TRICE_STACK_BUFFER` - direct mode only

1. Each singe *trice* is build inside a common buffer and finally copied inside the sub-macro `TRICE_LEAVE`.
2. Disabled relevant interrupts between `TRICE_ENTER` and `TRICE_LEAVE` are mantadory for `TRICE_STATIC_BUFFER`.
3. Usable for multiple non-blocking physical *trice* channels but **not** recommended for some time blocking channels.
4. A copy call is executed inside `TRICE_LEAVE`.

* With appropriate mapping a direct write to physical output(s) is possible:
  * RTT0 without extra copy.
    * With `TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE` about 100 MCU clocks do the whole work, what is within 1.5 us @ 64 MHz.
  * AUX without extra copy.
  * Not (yet) supported UART transfer loop with polling. With 1MBit baud rate, 4-12 bytes would last 40-120 µs.

####  1.1.2. <a id='trice-use-case-`trice_double_buffer`---deferred-mode,-fastest-*trice*-execution,-more-ram-needed'></a>Trice Use Case `TRICE_DOUBLE_BUFFER` - deferred mode, fastest *trice* execution, more RAM needed

1. Several *trices* are build in a half buffer.
1. No stack used.
1. Disabled interrupts between `TRICE_ENTER` and `TRICE_LEAVE`.
1. Usable for multiple blocking and non-blocking physical *trice* channels.
1. No copy call inside `TRICE_LEAVE` but optionally an additional direct mode is supported.

####  1.1.3. <a id='trice-use-case-`trice_ring_buffer`---deferred-mode,-balanced-*trice*-execution-time-and-needed-ram'></a>Trice Use Case `TRICE_RING_BUFFER` - deferred mode, balanced *trice* execution time and needed RAM

1. Each single *trices* is build in a ring buffer segment.
1. No stack used.
1. Disabled interrupts between `TRICE_ENTER` and `TRICE_LEAVE`.
1. Usable for multiple blocking and non-blocking physical *trice* channels.
1. No copy call inside `TRICE_LEAVE` but optionally an additional direct mode is supported.
1. Allocation call inside `TRICE_ENTER`

<p align="right">(<a href="#top">back to top</a>)</p>

##  2. <a id='trice-memory-needs'></a>Trice memory needs

###  2.1. <a id='trice-space-example'></a>Trice Space example

- STM32CubeMX generated empty default project: `Program Size: Code=2208 RO-data=236 RW-data=4 ZI-data=1636`  
- Same project with default `Trice` instrumentation: `Program Size: Code=2828 RO-data=236 RW-data=44 ZI-data=1836`
- Needed [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory): 620 Bytes
- Needed [RAM](https://en.wikipedia.org/wiki/Random-access_memory): 40 Bytes plus 200 Bytes for the 2 times 100 Bytes double buffer
- With increased/decreased buffers also more/less [RAM](https://en.wikipedia.org/wiki/Random-access_memory) is needed.
- With each additional `TRICE` macro a few additional [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory) bytes, like 10 assembler instructions, are needed.
- No printf-like library code is used anymore.
- No format strings go into the target code anymore.
- In general Trice instrumentation **reduces** the needed memory compared to a printf-like implementation.

###  2.2. <a id='memory-needs-(legacy-arm-example-project)'></a>Memory needs (Legacy ARM example project)

The following numbers are measured with a legacy encoding, showing that the instrumentation code can be even smaller.

| Program Size (STM32-F030R8 demo project)      | trice instrumentation | buffer size | compiler optimize for time | comment                         |
|-----------------------------------------------|-----------------------|-------------|----------------------------|---------------------------------|
| Code=1592 RO-data=236 RW-data= 4 ZI-data=1028 | none                  | 0           | off                        | CubeMX generated, no trice      |
| Code=1712 RO-data=240 RW-data=24 ZI-data=1088 | core                  | 64          | off                        | core added without trices       |
| Code=3208 RO-data=240 RW-data=36 ZI-data=1540 | TriceCheckSet()       | 512         | off                        | TRICE_SHORT_MEMORY is 1 (small) |
| Code=3808 RO-data=240 RW-data=36 ZI-data=1540 | TriceCheckSet()       | 512         | on                         | TRICE_SHORT_MEMORY is 0 (fast)  |

* The core instrumentation needs less 150 bytes FLASH and about 100 bytes RAM when buffer size is 64 bytes.
* The about 50 trices in TriceCheckSet() allocate roughly 2100 (fast mode) or 1500 (small mode) bytes.
* trices are removable without code changes with `#define TRICE_OFF 1` before `incude "trice.h"` on file level or generally on project level.

###  2.3. <a id='memory-needs-(example-projects)'></a>Memory needs (example projects)

| Project                       | Compiler    | Optimization | Link-Time-Optimization | Result                                        | Remark  |
| -                             | -           | -            | -                      | -                                             | -       |
| MDK-ARM_STM32F030R8_generated | CLANG v6.19 | -Oz          | yes                    |Code=1020 RO-data=196 RW-data=0 ZI-data=1024   | This is the plain generated project without trice instrumentation.|
| MDK-ARM_STM32F030R8_instrumented | CLANG v6.19 | -Oz          | yes                    |Code=4726 RO-data=238 RW-data=16 ZI-data=4608  | This is with full trice instrumentation with example messages.|

* The size need is less than 4 KB. See also [./TriceConfigProjectImageSizeOptimization.md](./TriceConfigProjectImageSizeOptimization.md).

<p align="right">(<a href="#top">back to top</a>)</p>
