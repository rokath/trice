# Common informa

(work in progress...)

## `TRICE0` |`TRICE8` |`TRICE16` |`TRICE32` |`TRICE64` macro

It is avoiding all the internal overhead (space and time) of a `printf()` 
statement but is easy to use. For example instead of writing 

```c
printf("time is %d:%d:%d\n", hour, min, sec);
```

you can write

```c
TRICE8("time is %d:%d:%d\n", hour, min, sec);
```

into a source file of your project. The `8` stands here for 8 bit values (`0`, `16`, `32` and `64` also possibe). Only values of the same size are allowed in one TRICE* statement, but you can use `TRICE32` consequently to match most cases for the prize of little overhead.

When performing `trice update` the source (tree) is parsed and in result this line changes to

```c
TRICE8_3( Id(12345), "time is %d:%d:%d\n", hour, min, sec);
```

where ```12345``` is an as ID generated 16 bit random number not used so far. About 65000 different trice messages are possible per default.  recommended **pack[L]** encoding suppoerts 24-bit IDs, so more than 1048000 different trice IDs usable.
Automatically the ID is added to an [ID list](https://github.com/rokath/trice/blob/master/til.json) together with the appropriate format string information. The TRICE`8_3` means 3 bytes as parameters in this example and allows efficient code and a compile time check.

*The total amount of data is currently limitated to 8 parameters for TRICE8 or 4 parameters for TRICE16 and TRICE32 and two parameters for TRICE64, but this is easy to extend if needed.*

When the embedded project is compiled, only the ID goes to the binary
but not the format string, what results in a smaller memory footprint.

Slightly delayed in the background the TRICE trace goes to the communication port, what is also fast compared to all the actions behind a `printf()` statement.

Please understand, that when debugging code containing TRICE\* statements, during a TRICE\* step-over only  one ore more 32 bit values go into the internal fifo buffer and no serial output
is visible because of the stopped target. But the SEGGER debug probe reads out the RTT memory and this way also during debug stepping realtime trice output is visible. That is (right now) not true for the STLINK interface because the is only one USB enpoint.

## `trice`

Executing `trice update` at the root of your project source updates the TRICE* statements inside the source code and the ID list (only where changes occured). The `-src` switch can be used multiple times to keep the amount of parsed data small for better speed.

With `trice log -port COM12 -baud 115200` you can visualize the trices on the PC, 
if for example `COM12` is receiving the data from the embedded device.

The following capture output comes from an example project inside`../test`

![life.gif](./README.media/life.gif)

See [triceCheck.c](https://github.com/rokath/trice/blob/master/srcTrice.C/triceCheck.c) for reference.
The trices can come mixed from inside interrupts (light blue `ISR:...`) or from normal code. For usage with a RTOS protect TRICE* against breaks. Regard the differences in the read SysTick values inside the GIF above These differeces are the MCU clocks needed for one trice (~0,25µs@48MHz).

Use the `-color off` switch for piping output in a file.

## Setup

### Project structure

   name        | info                                                    |
---------------|---------------------------------------------------------|
cmd/trice      | `trice` tool command Go sources                         |
docs/          | documentation                                           |
internal/      | `trice` tool internal Go packages                       |
pkg/           | `trice` tool common Go packages                         |
srcTrice.C/    | C sources for trice instrumentation                     | 
test/          | example target projects                                 |
third_party/   | external components                                     |

<!---
### Check the `trice` binary
- Copy command trice into a path directory.
- Run inside a shell `trice check -list path/to/trice/examples/triceDemoF030R8/MDK-ARM/`[til.json](../examples/triceDemoF030R8/MDK-ARM/til.json). You should see output like this:
![](./README.media/Check.PNG)
--->

 Instrument a target source code project (How to use trice in your project)

Look at one of the appropriate test projects as example. In general:

- Copy [_triceConfig.h](https://github.com/rokath/trice/tree/master/srcTrice.C/_triceConfig.h), rename to `triceConfig.h` and adapt to your needs.

- Make sure the [trice.h](https://github.com/rokath/trice/blob/master/srcTrice.C/trice.h) header file is found by your compiler and for

  - bare or wrap transfer format
    Include [triceBareFifo.c](https://github.com/rokath/trice/blob/master/srcTrice.C/triceBareFifo.c) together with [triceBareFifoToBytesBuffer.c](https://github.com/rokath/trice/blob/master/srcTrice.C/triceBareFifoToBytesBuffer.c) into your project.

  - esc transfer format
    Include [triceEscFifo.c](https://github.com/rokath/trice/blob/master/srcTrice.C/triceEscFifo.c) into your project.

Next steps:

- Add `#include "trice.h"` to your project files where to use TRICE macros and put `TRICE0( Id(0), "msg:Hello world!\n" );` after your initialization code.
- Run `trice u` at the root of your source code. Afterwards:
  - The `Id(0)` should have changed into `Id(12345)` as example. (The `12345` stays here for a 16-bit non-zero random number).
  - A file [til.json](https://github.com/rokath/trice/blob/master/til.json)  (**t**race **i**d **l**ist) should be generated.
- Set up timer and UART interrupt and main loop in the right way. Analyze the test example projects for advice.

<!---    - Running `trice check` should show your message, indicating everything is fine so far.--->

- For help have a look at the differences between these 2 projects or into [DemoF030R8.md](./DemoF030R8.md)
  - `../examples/generatedDemoF030R8` - It is just the STM32 CubeMX generated code.
  - `../examples/traceLDemoF030R8` - It is a copy of the above enhanced with trice check code.

```b
Quick and dirty option
======================
- Leave these definitions empty: 
  - TRICE_ENTER_CRITICAL_SECTION & TRICE_LEAVE_CRITICAL_SECTION
  - triceTxDataRegisterEmpty()
  - triceEableTxEmptyInterrupt() & triceDisableTxEmptyInterrupt()
- Use:
  - void triceTransmitData8( uint8_t d ){
    my_putchar( (char)d); // your code
  }
  Call TxStart();TxContinue(); cyclicylly in sufficient long intervals like 1 ms
```

- After compiling and flashing run `trice -port COMn -baud m` with n and m set to correct values
- Now start your device and you should see the hello world message coming from your target. In fact the hello-world string never went to the embedded device, only the ID comes from  there and the string is found in the [til.json](../test/til.json) file of your project.
- If you use a legacy project containing `printf()` statements you can simply transform them to **TRICE\*** statements. TRICE32 will do in most cases but for better performance take **TRCE8** or **TRICE16** where possible.
- `printf(...)` statements containing string format specifier are quickly portable by using `TRICE_P(...)` but without the trice space and speed advantage. The TRICE_P() is intended only for the few dynamic strings in a ported  projekt.  Enable `TRICE_PRINTF_ADAPTER` increases the needed code size by a few KB.
- It could be helpful to add `trice u ...` as prebuild step into your toolchain for each file or for the project as a whole. 
  This way you cannot forget the update step, it performs automatically.

## Memory needs (ARM example project)

Program Size (STM32-F030R8 demo project)     |trice instrumentation|buffer size|compiler optimze for time| comment
---------------------------------------------|------------------------|-----------|-------------------------|-----------------------------
Code=1592 RO-data=236 RW-data= 4 ZI-data=1028|        none            |        0  |         off             | CubeMX generated, no trice
Code=1712 RO-data=240 RW-data=24 ZI-data=1088|        core            |       64  |         off             | core added without trices
Code=3208 RO-data=240 RW-data=36 ZI-data=1540|    TriceCheckSet()  |      512  |         off             | TRICE_SHORT_MEMORY is 1 (small)
Code=3808 RO-data=240 RW-data=36 ZI-data=1540|    TriceCheckSet()  |      512  |         on              | TRICE_SHORT_MEMORY is 0 (fast)

- The core instrumentation needs less 150 bytes FLASH and about 100 bytes RAM when buffer size is 64 bytes.
- The about 50 trices in TriceCheckSet() allocate roughly 2100 (fast mode) or 1500 (small mode) bytes.
- trices are removable without code changes by defining `TRICE_OFF` on file or project level. 

## 

- You can deliver your device with encrypted trices. This way nobody is able to read the trices despite the service guy.
- Implementd is XTEA but this is easy exchangeable.
- The 8 byte blocks can get enrypted by enabling `#define ENRYPT...` inside *triceConfig.h*. You need to add `-key test` as **log** switch and you're done.
- Any password is usable instead of `test`. Simply add once the `-show` switch and copy the displayed passphrase into the *config.h* file.

## Otions for `trice` tool

The trice tool can be started in several modes (subcommands), each with several mantadory or optional switches. Switches can have parameters or not.

```b
trice subcommand -switch1 -switch2 parameter -switch3 ...
```

Which subcommand switches are usable for each subcommand is shown with `trice help`. This gives also information about their default values.

Output of `trice h`: (Actual version could slightly differ)

```b

```

<!---
### Subcommand `check` 
- `trice check` will check the JSON list and emit all TRICE statements inside the list once with a dataset.

#### `check` switch '-dataset'
- This is a `string` switch. It has one parameter. Its default value is `position`. That means each parameter has a different value. This is useful for testing.
- The `negative` value is uses a dataset with negative values for testing.
--->

## Additional hints

### Logfile viewing

`trice` generated logfiles with subcommand switch `-color off` are normal ASCII files. If they are with color codes, these are ANSI excape sequences.

- One easy view option is `less -R trice.log`. The linux command `less` is also available inside the VScode terminal. 
- Under Windows one could also download and use [ansifilter](https://sourceforge.net/projects/ansifilter/) for logfile viewing. A monospaced font is recommended. 

### Color issues under Windows

**Currently CMD console colors are not enabled by default in Win10**, so if you see no color but escape sequences on your powershell or cmd window, please refer to
[Windows console with ANSI colors handling](https://superuser.com/questions/413073/windows-console-with-ansi-colors-handling/1050078#1050078) or simply use a Linux like terminal under windows, like git-bash. One option is also to install Microsoft *Windows Terminal (Preview)* from inside the Microsoft store and to start trice inside there. Unfortunally this can not be done automatically right now because of missing commandline switches.
