
## `TRICE`
It is avoiding all the internal overhead (space and time) of a `printf()` 
statement but is easy to use. For example instead of writing 

```
printf("time is %d:%d:%d\n", hour, min, sec);
```
you can write
```
TRICE8("time is %d:%d:%d\n", hour, min, sec);
```
into a source file of your project. The `8` stays here for 8 bit values (`0`, `16` and `32` also possibe). Only values of the same size are allowed in one TRICE* statement.

When performing  ```trice update``` this line changes to
```
TRICE8_3( Id(12345), "time is %d:%d:%d\n", hour, min, sec);
```
where ```12345``` is an as ID generated 16 bit random number not used so far. Automatically
the ID is added to an [ID list](../examples/triceDemoF030R8/MDK-ARM/til.json) together with the appropriate format string 
information. The TRICE`8_3` means 3 bytes as parameters in
this example and allows efficient code and a compile time check.

*Right now the automatic replacement of TRICE8( ... ) with TRICE8_3( Id(12345), ...) is not fully implemented yet, so write TRICE8_3( Id(0), "time is %d:%d:%du\n", hour, min, sec); Also the total amount of data is currently limitated to 8 bytes, but this is easy to extend.*

When the embedded project is compiled, only the ID goes into the source image
but not the format string, what results in a smaller memory footprint.

During TRICE* runtime, inside the microcontroller only the ID (together with the 
parameters like hour, min, sec) is copied to a buffer. Execution time for a TRICE16_1
(as example) on a 48 MHz ARM is about 16 systicks resulting in 250 nanoseconds duration,
so you can use `trice` also inside interrupts. The needed buffer space is
one 32 bit word per normal trice (for up to 2 data bytes). Just in case the internal fifo overflows, the data are still in sync, you simply loose traces.

Slightly delayed in the background the TRICE trace goes to the communication port,
what is also fast compared to all the actions behind a `printf()` statement.

The buffered 4 byte trice is transmitted as an 8 byte packet allowing start byte, sender and
receiver addresses and CRC8 check to be used later in parallel with different
software protocols.

Please understand, that when debugging code containing TRICE* statements, during a 
TRICE* step-over only  one ore more 32 bit values go into the internal fifo buffer and no output
is visible because of the stopped target.

## `trice`

Executing `trice update` at the root of your project source updates the TRICE* statements inside the source code and the ID list (only where changes occured).

With `trice log -port COM12 -baud 115200` you can visualize the trices on the PC, 
if for example `COM12` is receiving the data from the embedded device.

The following capture output comes from an example project inside`../examples`

![](README.media/life.gif)

See [triceCheck.c](../examples/triceDemoF030R8/Src/triceCheck.c) for reference.
The trices can come mixed from inside interrupts (white `ISR:...`) or from normal code. For usage with a RTOS protect TRICE* against breaks. Regard the differences in the read SysTick values inside the GIF above These differeces are the MCU clocks needed for one trice (~0,25µs@48MHz).

Use `-color off` switch for piping output in a file or `-color alternate` for a different color set. *(color set designs are welcome, see func colorSetAlternate() in [emit.go](../pkg/emit/emit.go))*

## Setup

### Project structure
   name        | info                                                    |
---------------|---------------------------------------------------------|
cmd/           | the `trice` sources                                     |
pkg/           | the internal `trice`packages                            |
srcC/          | C sources for your embedded project                     | 
examples/      | example target projects                                 |
doc/           | documentation                                           |

### Check the `trice` binary
- Copy command trice into a path directory.
- Run inside a shell `trice check -list path/to/trice/examples/triceDemoF030R8/MDK-ARM/`[til.json](../examples/triceDemoF030R8/MDK-ARM/til.json). You should see output like this:
![](./README.media/Check.PNG)

### Instrument a target source code project (How to use trice in your project)

  - Include [trice.c](../scrC/trice.c) unchanged into your project and make sure the [trice.h](../scrC/trice.h) header file is found by your compiler.
- Add `#include "trice.h"` to your main.c[pp] and put `TRICE0( Id(0), "msg:Hello world!\n" );` after your initialization code.
- Run `trice u` at the root of your source code. Afterwards:
    - The `Id(0)` should have changed into `Id(12345)` as example. (The `12345` stays here for a 16-bit non-zero random number).
    - A file [til.json](../examples/triceDemoF030R8/MDK-ARM/til.json)  (**t**race **i**d **l**ist) should be generated.
    - Running `trice check` should show your message, indicating everything is fine so far.
- `trice help` is your friend if something fails.
- For help have a look at the differences between these 2 projects or into [ReadMeDemoF030R8.md](./ReadMeDemoF030R8.md)
  - `../examples/generatedDemoF030R8` - It is just the STM32 CubeMX generated code.
  - `../examples/traceLDemoF030R8` - It is a copy of the above enhanced with trice check code.
```
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
  Call triceTxHandler() cyclicylly in sufficient long intervals like 1 ms
```

- After compiling and flashing run `trice -port COMn -baud m` with n and m set to correct values
- Now start your device and you should see the hello world message coming from your target.
- If you use a legacy project containing `printf()` statements you can  simply transform them to **TRICE\*** statements.
- `printf(...)` statements containing string format specifier are quickly portable by simply using `tricePrintfAdapter(...)` but without speed advantage. Enable `TRICE_PRINTF_ADAPTER` in `config.h` and include [printf.c](https://github.com/mpaland/printf/blob/master/printf.c) or the like to your project for that.
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

## ID management internals & hints
- During `trice update` so far unknown IDs are added to the ID list (case new sources added) with a `Created` utc timestamp.
- If an ID was deleted inside the source tree (or file removal) the appropriate ID's stay inside the ID list but get a `Removed` utc timestamp.
- If the same ID appears again the appropriate `Removed` timestamp is deleted inside the ID list and the ID is aktive again.
- If duplicate ID's with different format strings found inside the source tree (case several developers) the newer ID is replaced by a new ID. The probability for such case is low, because of the random ID generation.
- If the format string was modified, the ID goes into the `Removed` state and a new ID is generated.
- Keeping obsolete IDs makes it more comfortable during development to deal with different firmware variants at the same time.
- This way you can simply copy a TRICE* statement and modify it without dealing with the ID. The trice tool will do for you.
- The ID list should go into the version control repository of your project.
- For a firmware release it makes sense to remove all unused IDs (development garbage) from til.json.
  - This could be done by deleting til.json, getting the legacy til.json from the former firmware release from the source control system and enhance it with the actual release software IDs by simply calling 'trice update'.

## Build `trice` from Go sources
- Install Go, download the trice sources and cd into the `trice` directory
- Testing
```
go test ./...
```
- Build & Install
```
go install ./...
```
Afterwards you should find an executable `trice` inside $GOPATH/bin/
- Running
```
trice help
```

## Demo project setup
- see [ReadMeDemoF030R8.md](./ReadMeDemoF030R8.md)
