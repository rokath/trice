# *Trice* Speed

> _(Read only you are interested in)_

 A `TRICE` macro execution can be as cheap like **3-4 Assembler instructions or 6-8 processor clocks**:

* Disassembly: ![./ref/MEASURE_executionCode.PNG](./ref/MEASURE_executionCode.PNG)
* Measurement: The blue SYSTICK clock counts backwards 6 clocks for each `TRICE` macro (on an ARM M0+), what is less than 100 ns @64 MHz MCU clock: ![./ref/MEASURE_executionClocks.PNG](./ref/MEASURE_executionClocks.PNG)

A more realistic (typical) timing with target location and µs timestamps, critical section and parameters is shown here with the STM32F030 M0 core:

![./ref/F030FullTiming.PNG](./ref/F030FullTiming.PNG)

The MCU is clocked with 48 MHz and a *Trice* duration is about 2 µs, where alone the internal ReaUs() call is already nearly 1 µs long:

![./ref/ReadUsF030.PNG](./ref/ReadUsF030.PNG)

## Target Implementation Options

All trice macros use internally this sub-macro:

```C
#define TRICE_PUT(x) do{ *TriceBufferWritePosition++ = TRICE_HTOTL(x); }while(0); //! PUT copies a 32 bit x into the TRICE buffer.
```

The usual case is `#define TRICE_HTOTL(x) (x)`. The `uint32_t* TriceBufferWritePosition` points to a buffer, which is codified and used with the *trice* framing sub-macros `TRICE_ENTER` and `TRICE_LEAVE` in dependence of the use case.

### *Trice* Use Case `TRICE_FIFO_BUFFER` - direct mode, no stack usage

* With appropriate mapping a direct write to a single physical output is possible:
  * UART without extra transfer loop.
  * RTT0 without extra copy.
  * AUX without extra copy.

### *Trice* Use Case `TRICE_SINGLE_BUFFER` - direct mode, no stack usage

1. Each singe *trice* is build inside a common buffer and finally copied inside the sub-macro `TRICE_LEAVE`.
1. No stack used.
1. Disabled interrupts between `TRICE_ENTER` and `TRICE_LEAVE`.
1. Usable for multiple non-blocking physical *trice* channels but **not** recommended for blocking channels.
1. Copy call inside `TRICE_LEAVE`.

### *Trice* Use Case `TRICE_STACK_BUFFER` - direct mode, stack usage, smallest possible interrupt service delay

1. Each singe *trice* is build on the stack and finally copied inside the sub-macro `TRICE_LEAVE`.
1. Stack used.
1. This needs disabled interrupts only inside `TRICE_LEAVE` for the copy call.
1. Usable for multiple non-blocking physical *trice* channels but **not** recommended for blocking channels.
1. Copy call inside `TRICE_LEAVE`.

### *Trice* Use Case `TRICE_DOUBLE_BUFFER` - deferred mode, fastest *trice* execution, more RAM needed

1. Several *trices* are build in a half buffer.
1. No stack used.
1. Disabled interrupts between `TRICE_ENTER` and `TRICE_LEAVE`.
1. Usable for multiple blocking and non-blocking physical *trice* channels.
1. No copy call inside `TRICE_LEAVE`.

### *Trice* Use Case `TRICE_STREAM_BUFFER` - deferred mode, balanced *trice* execution time and needed RAM

1. Each single *trices* is build in a stream buffer.
1. No stack used.
1. Disabled interrupts between `TRICE_ENTER` and `TRICE_LEAVE`.
1. Usable for multiple blocking and non-blocking physical *trice* channels.
1. No copy call inside `TRICE_LEAVE`.
1. Allocation call inside `TRICE_ENTER`

