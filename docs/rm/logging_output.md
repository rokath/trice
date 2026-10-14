# Trice Logging Output

With `trice log -port COM12` you can visualize the trices on the PC, if for example `COM12` is receiving the data from the embedded device at the 115200 default baudrate.

The following capture output comes from an (old) example project inside [../examples](../../examples).

[![Colored Trice log output with timestamps](../ref/life-preview.png)](../ref/life.gif)

Select the preview to open the full animation.

See [../_test/testdata/triceCheck.c](../../_test/testdata/triceCheck.c) for reference. The *Trices* can come mixed from inside interrupts (light blue `ISR:...`) or from normal code. For usage with a RTOS, *Trices* are protected against breaks (`TRICE_ENTER_CRITICAL_SECTION`, `TRICE_LEAVE_CRITICAL_SECTION`). Regard the differences in the read SysTick values inside the linked animation. These differences are the MCU clocks needed for one trice (~0,25µs@48MHz).

Use the `-color off` switch for piping output in a file. More convenient is the `-lf auto` switch.

<p align="right">(<a href="#trice-logging-output">back to top</a>)</p>

