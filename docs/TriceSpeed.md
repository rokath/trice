# *Trice* Speed

> _(Read only you are interested in)_

 A `TRICE` macro execution can be as cheap like **3-4 Assembler instructions or 6-8 processor clocks**:

* Disassembly: ![./ref/MEASURE_executionCode.PNG](./ref/MEASURE_executionCode.PNG)
* Measurement: The blue SYSTICK clock counts backwards 6 clocks for each `TRICE` macro (on an ARM M0+), what is less than 100 ns @64 MHz MCU clock: ![./ref/MEASURE_executionClocks.PNG](./ref/MEASURE_executionClocks.PNG)

A more realistic (typical) timing with target location and µs timestamps, critical section and parameters is shown here with the STM32F030 M0 core:

![./ref/F030FullTiming.PNG](./ref/F030FullTiming.PNG)

The MCU is clocked with 48 MHz and a *Trice* duration is about 2 µs, where alone the internal ReadUs() call is already nearly 1 µs long:

![./ref/ReadUsF030.PNG](./ref/ReadUsF030.PNG)
