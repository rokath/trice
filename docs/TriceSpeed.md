# *Trice* Speed

> _(Read only you are interested in)_

<details><summary>Table of Contents</summary><ol><!-- TABLE OF CONTENTS START -->

<!-- 
Table of Contents Generation:
- Install vsCode extension "Markdown TOC" from dumeng 
- Use Shift-Ctrl-P "markdownTOC:generate" to get the automatic numbering.
- replace "<a id=" with "<a id=" 
-->

<!-- vscode-markdown-toc -->
* 1. [Target Implementation Options](#target-implementation-options)
  * 1.1. [*Trice* Use Cases `TRICE_STATIC_BUFFER` and `TRICE_STACK_BUFFER` - direct mode only](#*trice*-use-cases-`trice_static_buffer`-and-`trice_stack_buffer`---direct-mode-only)
  * 1.2. [*Trice* Use Case `TRICE_DOUBLE_BUFFER` - deferred mode, fastest *trice* execution, more RAM needed](#*trice*-use-case-`trice_double_buffer`---deferred-mode,-fastest-*trice*-execution,-more-ram-needed)
  * 1.3. [*Trice* Use Case `TRICE_RING_BUFFER` - deferred mode, balanced *trice* execution time and needed RAM](#*trice*-use-case-`trice_ring_buffer`---deferred-mode,-balanced-*trice*-execution-time-and-needed-ram)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

<div id="top"></div></ol></details><!-- TABLE OF CONTENTS END -->

 A `TRICE` macro execution can be as cheap like **3-4 Assembler instructions or 6-8 processor clocks**:

* Disassembly: ![./ref/MEASURE_executionCode.PNG](./ref/MEASURE_executionCode.PNG)
* Measurement: The blue SYSTICK clock counts backwards 6 clocks for each `TRICE` macro (on an ARM M0+), what is less than 100 ns @64 MHz MCU clock: ![./ref/MEASURE_executionClocks.PNG](./ref/MEASURE_executionClocks.PNG)

A more realistic (typical) timing with target location and µs timestamps, critical section and parameters is shown here with the STM32F030 M0 core:

![./ref/F030FullTiming.PNG](./ref/F030FullTiming.PNG)

The MCU is clocked with 48 MHz and a *Trice* duration is about 2 µs, where alone the internal ReadUs() call is already nearly 1 µs long:

![./ref/ReadUsF030.PNG](./ref/ReadUsF030.PNG)

##  1. <a id='target-implementation-options'></a>Target Implementation Options

All trice macros use internally this sub-macro:

```C
#define TRICE_PUT(x) do{ *TriceBufferWritePosition++ = TRICE_HTOTL(x); }while(0); //! PUT copies a 32 bit x into the TRICE buffer.
```

The usual case is `#define TRICE_HTOTL(x) (x)`. The `uint32_t* TriceBufferWritePosition` points to a buffer, which is codified and used with the *trice* framing sub-macros `TRICE_ENTER` and `TRICE_LEAVE` in dependence of the use case.

###  1.1. <a id='*trice*-use-cases-`trice_static_buffer`-and-`trice_stack_buffer`---direct-mode-only'></a>*Trice* Use Cases `TRICE_STATIC_BUFFER` and `TRICE_STACK_BUFFER` - direct mode only

1. Each singe *trice* is build inside a common buffer and finally copied inside the sub-macro `TRICE_LEAVE`.
2. Disabled relevant interrupts between `TRICE_ENTER` and `TRICE_LEAVE` are mantadory for `TRICE_STATIC_BUFFER`.
3. Usable for multiple non-blocking physical *trice* channels but **not** recommended for some time blocking channels.
4. A copy call is executed inside `TRICE_LEAVE`.

* With appropriate mapping a direct write to physical output(s) is possible:
  * RTT0 without extra copy.
    * With `TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE` about 100 MCU clocks do the whole work, what is within 1.5 us @ 64 MHz.
  * AUX without extra copy.
  * Not (yet) supported UART transfer loop with polling. With 1MBit baud rate, 4-12 bytes would last 40-120 µs.

###  1.2. <a id='*trice*-use-case-`trice_double_buffer`---deferred-mode,-fastest-*trice*-execution,-more-ram-needed'></a>*Trice* Use Case `TRICE_DOUBLE_BUFFER` - deferred mode, fastest *trice* execution, more RAM needed

1. Several *trices* are build in a half buffer.
1. No stack used.
1. Disabled interrupts between `TRICE_ENTER` and `TRICE_LEAVE`.
1. Usable for multiple blocking and non-blocking physical *trice* channels.
1. No copy call inside `TRICE_LEAVE` but optionally an additional direct mode is supported.

###  1.3. <a id='*trice*-use-case-`trice_ring_buffer`---deferred-mode,-balanced-*trice*-execution-time-and-needed-ram'></a>*Trice* Use Case `TRICE_RING_BUFFER` - deferred mode, balanced *trice* execution time and needed RAM

1. Each single *trices* is build in a ring buffer segment.
1. No stack used.
1. Disabled interrupts between `TRICE_ENTER` and `TRICE_LEAVE`.
1. Usable for multiple blocking and non-blocking physical *trice* channels.
1. No copy call inside `TRICE_LEAVE` but optionally an additional direct mode is supported.
1. Allocation call inside `TRICE_ENTER`

<p align="right">(<a href="#top">back to top</a>)</p>