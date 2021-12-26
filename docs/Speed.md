# *Trice* Speed

 A `TRICE` macro execution can be as cheap like **3-4 Assembler instructions or 6-8 processor clocks**:

- Disassembly: ![./README.media/MEASURE_executionCode.PNG](./README.media/MEASURE_executionCode.PNG)
- Measurement: The blue SYSTICK clock counts backwards 6 clocks for each `TRICE` macro (on an ARM M0+), what is less than 100 ns @64 Mhz MCU clock: ![./README.media/MEASURE_executionClocks.PNG](./README.media/MEASURE_executionClocks.PNG)
