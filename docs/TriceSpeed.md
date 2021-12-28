# *Trice* Speed

 A `TRICE` macro execution can be as cheap like **3-4 Assembler instructions or 6-8 processor clocks**:

- Disassembly: ![./ref/MEASURE_executionCode.PNG](./ref/MEASURE_executionCode.PNG)
- Measurement: The blue SYSTICK clock counts backwards 6 clocks for each `TRICE` macro (on an ARM M0+), what is less than 100 ns @64 MHz MCU clock: ![./ref/MEASURE_executionClocks.PNG](./ref/MEASURE_executionClocks.PNG)
