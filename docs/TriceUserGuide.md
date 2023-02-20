# *Trice* user guide

> _(Read this)

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>

<!-- vscode-markdown-toc -->
- [*Trice* user guide](#trice-user-guide)
  - [1. Project structure](#1-project-structure)
  - [2. Get started](#2-get-started)
    - [2.1. Get it](#21-get-it)
    - [2.2. Install It](#22-install-it)
    - [2.3. Use It](#23-use-it)
  - [3. Build `trice` tool from Go sources (you can skip that)](#3-build-trice-tool-from-go-sources-you-can-skip-that)
  - [4. Embedded system code setup](#4-embedded-system-code-setup)
  - [5. Adapt your legacy source code](#5-adapt-your-legacy-source-code)
  - [6. `trice` tool in logging action](#6-trice-tool-in-logging-action)
  - [7. Encryption](#7-encryption)
  - [8. CLI Options for `trice` tool](#8-cli-options-for-trice-tool)
  - [9. *Trice* command line examples](#9-trice-command-line-examples)
    - [9.1. Common information](#91-common-information)
    - [9.2. Further examples](#92-further-examples)
      - [9.2.1. Automated pre-build update command example](#921-automated-pre-build-update-command-example)
      - [9.2.2. Some Log examples](#922-some-log-examples)
      - [9.2.3. Logging over a display server](#923-logging-over-a-display-server)
      - [9.2.4. Logfile output](#924-logfile-output)
      - [9.2.5. Binary Logfile](#925-binary-logfile)
      - [9.2.6. TCP output](#926-tcp-output)
      - [9.2.7. Set all IDs in a directory tree to 0](#927-set-all-ids-in-a-directory-tree-to-0)
      - [9.2.8. Stimulate target with a user command over UART](#928-stimulate-target-with-a-user-command-over-uart)
      - [9.2.9. Explpore and modify channels and their colors](#929-explpore-and-modify-channels-and-their-colors)
      - [9.2.10. Location Information](#9210-location-information)
  - [10. Additional hints](#10-additional-hints)
    - [10.1. Pre-built executables are available](#101-pre-built-executables-are-available)
    - [10.2. Configuration file `triceConfig.h`](#102-configuration-file-triceconfigh)
    - [10.3. Setting up the very first connection](#103-setting-up-the-very-first-connection)
    - [10.4. Avoid buffer overruns](#104-avoid-buffer-overruns)
    - [10.5. Limitation "trice u" requires TRICE macros on a single line](#105-limitation-trice-u-requires-trice-macros-on-a-single-line)
    - [10.6. Limitation TRICE in TRICE not possible](#106-limitation-trice-in-trice-not-possible)
    - [10.7. Dynamic strings/buffers only as variable inside `TRICE` macros](#107-dynamic-stringsbuffers-only-as-variable-inside-trice-macros)
    - [10.8. Logfile viewing](#108-logfile-viewing)
    - [10.9. Using the `trice` tool with 3rd party tools](#109-using-the-trice-tool-with-3rd-party-tools)
    - [10.10. Several targets at the same time](#1010-several-targets-at-the-same-time)
    - [10.11. Executing `go test -race -count 100 ./...`](#1011-executing-go-test--race--count-100-)
    - [10.12. Direct TRICE Out (TRICE\_MODE 0) could cause stack overflow with -o0 optimization](#1012-direct-trice-out-trice_mode-0-could-cause-stack-overflow-with--o0-optimization)
  - [11. Target side *Trice* On-Off](#11-target-side-trice-on-off)
  - [12. Host side *Trice* On-Off](#12-host-side-trice-on-off)
  - [13. Using a different encoding](#13-using-a-different-encoding)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc --><div id="top"></div>

  </ol>
</details>

##  1. <a name='Projectstructure'></a>Project structure

| name         | info                                            |
|--------------|-------------------------------------------------|
| cmd/trice    | `trice` tool command Go sources                 |
| cmd/cui      | (do not use) command user interface tryout code |
| docs/        | documentation                                   |
| internal/    | `trice` tool internal Go packages               |
| pkg/         | `trice` tool common Go packages                 |
| pkg/src/     | C sources for trice instrumentation             |
| test/        | example target projects                         |
| third_party/ | external components                             |

<p align="right">(<a href="#top">back to top</a>)</p>

##  2. <a name='Getstarted'></a>Get started

###  2.1. <a name='Getit'></a>Get it

* OR [Download](https://github.com/rokath/trice/releases) latest release assets for your system: Source code and compressed binaries.
* OR Clone the repo: `git clone https://github.com/rokath/trice.git`
  * Later inside folder `trice`: `git pull` to get the latest changes.
* OR use the ![./ref/Fork.PNG](./ref/Fork.PNG) button

###  2.2. <a name='InstallIt'></a>Install It

* Place the **trice** binary somewhere in your [PATH](https://en.wikipedia.org/wiki/PATH_(variable)).

###  2.3. <a name='UseIt'></a>Use It

* In a console type `trice help -all`. You should see the complete **trice** tool [CLI](https://en.wikipedia.org/wiki/Command-line_interface) documentation.
  * Don´t worry, most of it you will never need.
  * There are only 2 important commands: `trice u[pdate]` and `trice l[og]`. Call them with the right CLI switches.
    * `trice h -u[pdate]` and `trice h -l[og]` show partial help.
* Add `./src/trice.c` to your project.
* Add `./src/` to your library include path.
* Copy file `./test/MDK-ARM_STM32F030R8/Core/Inc/triceConfig.h` to your embedded project and adapt it to your needs.
  * Other `triceConfig.h` files are usable as well, but the above is usually the most actual one.
* Optionally copy `./test/testdata/triceCheck.c` to your project if you wish to perform some checks.
  * Copy it, because it gets overwritten when `updateTestData.sh` is executed inside the `./test` folder.
* In your source.c files add line `#include "trice.h"`
* In a function write: `TRice( "1/11 = %g\n", aFloat( 1.0/11 ) );` or s.th. similar.
* In **project root**:
  * Create empty file: `touch til.json`.
  * Run `trice u` should perform **automatically** these things (The numbers are just examples.):
>>>    * Patch source.c to `TRice( iD(12363), "1/11 = %g\n", aFloat( 1.0/11 ) );`
>>>      * C & H files containing TRICE macros, are only modified if needed (missing or obsolete ID)
>>>    * Extend `til.json`
>>>      * If no `til.json` is found nothing happens. At least an empty file is needed (Safety feature).
* When the program runs later it should output something similar to![./ref/1div11.PNG](./ref/1div11.PNG)
* It is up to the user to provide the 2 functions `void Stamp16(void)` and `void Stamp32(void)`.
* For RTT add SEGGER source, for UART add UART write function. 

<p align="right">(<a href="#top">back to top</a>)</p>

##  3. <a name='BuildtricetoolfromGosourcesyoucanskipthat'></a>Build `trice` tool from Go sources (you can skip that)

* Install [Go](https://golang.org/).
* On Windows you need to install [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/).
  * Take the 64-bit variant when Go is 64-bit or take the 32-bit variant when Go is 32-bit. If mixed installations work I doubt.
  * Recommendation: Minimal online installer.
  * GCC is only needed for [./pkg/src/src.go](https://github.com/rokath/trice/blob/master/pkg/src/src.go), what gives the option to test the C-code on the host.
  * Make sure TDM-GCC is found first in the path.
  * Other gcc variants could work also but not tested.
* Open a console inside the `trice` directory.
* Check and install:

```b
ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (master)
$ go clean -cache

ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (master)
$ go vet ./...

ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (master)
$ go test ./...
?       github.com/rokath/trice/cmd/cui [no test files]
ok      github.com/rokath/trice/cmd/stim        0.227s
ok      github.com/rokath/trice/cmd/trice       0.577s
ok      github.com/rokath/trice/internal/args   0.232s
ok      github.com/rokath/trice/internal/charDecoder    0.407s
ok      github.com/rokath/trice/internal/com    1.148s
ok      github.com/rokath/trice/internal/decoder        0.412s [no tests to run]
?       github.com/rokath/trice/internal/do     [no test files]
ok      github.com/rokath/trice/internal/dumpDecoder    0.388s
ok      github.com/rokath/trice/internal/emitter        0.431s
ok      github.com/rokath/trice/internal/id     0.421s
ok      github.com/rokath/trice/internal/keybcmd        0.431s
ok      github.com/rokath/trice/internal/link   0.404s
ok      github.com/rokath/trice/internal/receiver       0.409s
ok      github.com/rokath/trice/internal/tleDecoder     0.398s
?       github.com/rokath/trice/internal/translator     [no test files]
ok      github.com/rokath/trice/internal/trexDecoder    0.391s
ok      github.com/rokath/trice/pkg/cipher      0.377s
ok      github.com/rokath/trice/pkg/endian      0.302s
ok      github.com/rokath/trice/pkg/msg 0.299s
ok      github.com/rokath/trice/pkg/tst 0.406s
ok      github.com/rokath/trice/test/cgo_stackBuffer_noCycle_cobs       40.910s
ok      github.com/rokath/trice/test/cgo_stackBuffer_noCycle_tcobs      40.926s

ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (master)
$ go install ./...

ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (master)
$
```

Afterwards you should find an executable `trice` inside $GOPATH/bin/ and you can modify its source code.

<!--
* The used serial Go driver package is Linux & Windows tested but should work on MacOS soon too.
* For historical reasons there are 2 serial drivers inside the **trice** tool. This will be changed.
-->

<p align="right">(<a href="#top">back to top</a>)</p>

##  4. <a name='Embeddedsystemcodesetup'></a>Embedded system code setup

* Each project gets its own [triceConfig.h](../test/MDK-ARM_STM32F030R8/Core/Inc/triceConfig.h) file.
* Modify [triceConfig.h](../test/MDK-ARM_STM32F030R8/Core/Inc/triceConfig.h) according your needs. Choose the *Trice* mode here:
  * Immediate mode: Straight output inside `TRICE` macro at the cost of the time it takes.
    * With `#define TRICE_MODE 0` (immediate mode) just provide a **putchar()** function but no inside interrupts *Trices*!
    * Or use [SEGGER_RTT](./TriceOverRTT.md) allowing *Trices* also inside interrupts. An other big plus during new hardware setup is, no need to connect an additional wire. All information goes thru the debug probe. Internally only a `memcpy` transfers maybe 16 bytes to the SEGGER_RTT buffer and *Trice* does even not need a own buffer and no background task.
  * Deferred mode: Output outside `TRICE` macro, a background output some milliseconds later is needed at the cost of RAM buffer.
    * Compare the **not** instrumented test project [./test/MDK-ARM_STM32F030R8_generated]([./test/MDK-ARM_STM32F030R8_generated) with the instrumented test project [./test/MDK-ARM_STM32F030R8]([./test/MDK-ARM_STM32F030R8) to see an implementation example.
* Recommendation:
  * [SEGGER_RTT](./TriceOverRTT.md) transfer: Immediate *Trice* mode.
  * None-[SEGGER_RTT](./TriceOverRTT.md) transfer (mostly UART):
    * *Trice* Kick-off: Immediate *Trice* mode, but **no** `TRICE` macros inside interrupts.
    * *Trice* usage:  Deferred mode which **allows to use** `TRICE` macros also inside interrupts.
    * In some cases, when logging a huge amount of data without timing constraints the immediate mode is a better choice.
* If speed **and** log volume is needed, care must be taken to avoid *Trice* buffer overflow for example by time triggering.
* Set options inside [triceConfig.h](../test/MDK-ARM_STM32F030R8/Core/Inc/triceConfig.h):
  * Target timestamps time base
  * A cycle counter is per default active.
    * `#define TRICE_CYCLE_COUNTER 0` to deactivate it for a bit more speed (and less code).
  * Allow `TRICE` usage inside interrupts for a bit less speed (and more code):
    * `#define TRICE_ENTER TRICE_ENTER_CRITICAL_SECTION`
    * `#define TRICE_LEAVE TRICE_LEAVE_CRITICAL_SECTION`
  * Buffer size (use function `TriceDepthMax()` to check the used buffer depth):
    * Immediate mode: `#define TRICE_STACK_BUFFER_MAX_SIZE 128` - space for one *Trice*
    * Deferred mode: `#define TRICE_HALF_BUFFER_SIZE 1000`- space for *Trices* within ~100ms
  * *Trice* output over UART 
    * `#define TRICE_UART USART2`:  In project root a command like `trice l -p COM14` is needed. It should show something similar to![./ref/1div11.PNG](./ref/1div11.PNG) after app start.
  * *Trice* output over RTT: Please refer to the [./TriceOverRTT.md](./TriceOverRTT.md) document.
  * Further *Trice* output options: Please refer to the [./TriceOverOneWire.md](./TriceOverOneWire.md) document.
* All compiler and hardware specific adaption should be possible inside `triceConfig.h`
* Compile, load and start your app.
* Look in [triceCheck.c](../pkg/src/triceCheck.c) for usage examples.
  * It contains `TRICE` macros in different variants.
* Because of performance reasons there is no function call inside `TRICE`. Too much `TRICE` macros could take too much FLASH space. But you can deactivate the code generation for all`TRICE` macros on file granularity. See [Target side *Trice* On-Off](#TargetsideTriceOn-Off) below.

* It is sufficient for most cases just to use the `TRICE` macro with max 0 to 12 parameters as a replacement for `printf` and to use the default settings.
  * For more compact transfer consider `TRICE8` & `TRICE16` macros or if `double` is needed use `TRICE64`.
  * Further reading: [TriceVsPrintfSimilaritiesAndDifferences.md](TriceVsPrintfSimilaritiesAndDifferences.md).

<p align="right">(<a href="#top">back to top</a>)</p>

##  5. <a name='Adaptyourlegacysourcecode'></a>Adapt your legacy source code

For example change the legacy source code line

```c
printf( "%d Kelvin\n", k );
```

into

```c
TRICE( "%d Kelvin\n", k );
```

This you could do automatically using a word processor. A `trice update` (run it later in the tool chain to keep everything automatically up-to-date) inserts the *Trice* IDs:

```c
TRICE( Id(12345), "%d Kelvin\n", k );
```

Care must be taken in the following cases:

* More than 12 printf parameters: use several printf-calls
* float numbers: surround each with `aFloat()`
* double numbers: surround each with `aDouble()` and use the `TRICE64` macro
* runtime generated strings: Each needs its own `TRICE_S` macro, example: 
  * Legacy code: `printf( "Entered name is %20s %30s, favorite numbers %d, %f\n", "Paul", "Luap", 42, 3.14159 );`
  * Trice code 1: `name = "Paul"; TRICE_S( "Entered name is %20s", name );`
  * Trice code 2: `surname = "Luap";  TRICE_S( " %30s, ", surname );`
  * Trice code 3: `TRICE( "favorite numbers %d, %f\n", 42, aFloat(3.14159) );`


<!--
and adds for example the *ID 12345* together with *"%d Kelvin\n"* into a **t**rice **I**D **l**ist, a [JSON](https://www.json.org/json-en.html) reference file named [til.json](../til.json). The *12345* is a random or policy generated ID not used so far. During compilation the `TRICE` macro is translated to only a *12345* reference and the variable *k*. The format string never sees the target.
-->

When you compare the needed FLASH size before and after you probably will see more free space afterwards, because the *Trice* code is less than 1 KB, no format strings anymore inside the target and you do not need a printf library anymore. Be aware that `TRICE` is a macro adding each time it is used some code.

<!--

A `TRICE` macro is avoiding all the `printf()` internal overhead (space and time) but is nearly as easy to use. For example instead of writing

```c
printf("time is %d:%d:%d\n", hour, min, sec);
```

you can write

```c
TRICE8("time is %d:%d:%d\n", hour, min, sec);
```

into a source file of your project. The `8` stands here for 8 bit values (`16`, `32` and `64` also possible). Values of mixed size are allowed in one `TRICE` macro, so you can use `TRICE` consequently to match most cases for the prize of little data overhead.

Side note: If you look in detail at the *Trice* code you will see that a `TRICE8( "%d", 1 )` takes the same amount of transfer data as `TRICE( "%d", 1 )` and is even a bit slower because of the internal masking. But using `TRICE8( "%d%d%d%d%d%d%d%d%d%d%d%d", 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ;` instead of `TRICE( "%d%d%d%d%d%d%d%d%d%d%d%d", 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ;` will reduce the transfer bytes by 24 bytes.

When performing `trice update` the source (tree) is parsed and in result this line changes to

```c
TRICE8( Id(12345), "time is %d:%d:%d\n", hour, min, sec);
```
or 
```c
TRICE8_3( Id(12345), "time is %d:%d:%d\n", hour, min, sec);
```

as you like where `12345` is an as ID generated 16-bit random (upward|downward also possible) number not used so far. The TRICE8`_3` means 3 parameters in this example and allows efficient code and a compile time check. Per default the macro name `TRICE8` is not changed for a slightly more readable code. If you wish a compile time parameter count check use `-addParamCount` to the update command line, to convert a `TRICE8` into a `TRICE8_3` in the above example. Legacy code with valid IDs is not modified (You can use sub-command `zeroSourceTreeIds` to go around that.)

The total amount of data is currently limited to 12 parameters but this is easy to extend if needed.

When the embedded project is compiled, only the ID goes to the binary but not the format string, what results in a smaller memory footprint usually compared to a printf-like function call, because of the stored string.

On execution the ID is pushed into a buffer together with the optional *Trice* parameters and that is the real fast and important part which could be finished within 6-8 processor clocks ([measured](./TriceSpeed.md) on a ARM M0+). At 64 MHz in the time needed light travels about 30 meters. Slightly delayed in the background the *Trice* goes to the communication port, what is also fast compared to all the actions behind a `printf()` statement.

Please understand, that when debugging code containing `TRICE`macros, during a `TRICE` step-over only one ore more 32 bit values go into the internal buffer and no serial output immediately is visible because of the stopped target. But the SEGGER debug probe reads out the RTT memory and this way also during debug stepping real-time trice output is visible. That is (right now) not true for the ST-Link interface because there is only one USB endpoint.

trice instructions: `TRICE`, `Trice` or `trice` with or without ending letter 'i'?

There are several types of trice statements. All trice statements can have an additional letter 'i'. This means **i**nside critical section. \
You can use these when it is sure not to get interrupted by other trices. If for example an interrupt contains a trice statement this can be \
an i-trice but other trices not allowed to be an i-trice, they need to be normal trices, which are protected against interruption. \
If you are not sure it is always safe to use normal trices (without ending 'i'). The i-trices are a bit faster what is not relevant in most cases because of the general speed.

* Mixed case `Trice0`, `Trice8_1`, `Trice16_1` and `Trice8_2` are so called short trice macros.\
They use internal a smaller encoding and have only a 15-bit ID size, means ID's 1-32767 are usable.\
These are the fastest trices and with them the speed limit is reached.\
![x](./ref/Trice16_1-Code.PNG)\
![x](./ref/Trice16_1i-Code.PNG)\
![x](./ref/Trice16_1i.PNG)\
The number in the blue lines is the current processor tick. For `Trice16_1i` the difference between neighbors is about 13 clocks. \
Short trices need 'id(0)' instead 'Id(0)' as important difference to normal trices. The `trice` tool will handle that for you.
* Upper case `TRICE0`, `TRICE8_1`, ... `TRICE8_8`, `TRICE16_1`, ... `TRICE16_4`, `TRICE32_1`, ... `TRICE32_4`, `TRICE64_1`, `TRICE64_2` are normal trice macros. \
They insert code directly (no function call) for better performance but the drawback is the rising code amount when many trices are used.
* Lower case `trice0`, `trice8_1`, ... `trice8_8`, `trice16_1`, ... `trice16_4`, `trice32_1`, ... `trice32_4`, `trice64_1`, `trice64_2` are normal trice functions. \
The function call overhead is reasonable and the advantage is significant less code amount when many trices are used.
* For most flexibility the code for each trice function can be enabled or not inside the triceConfig.h.

-->

<p align="right">(<a href="#top">back to top</a>)</p>

##  6. <a name='tricetoolinloggingaction'></a>`trice` tool in logging action

<!--
Executing `trice update` at the root of your project source updates in case of changes, the *Trice* statements inside the source code and the ID list. The `-src` switch can be used multiple times to keep the amount of parsed data small for better speed.
-->

With `trice log -port COM12 -baud 921600` you can visualize the trices on the PC, if for example `COM12` is receiving the data from the embedded device at this baudrate.

The following capture output comes from an example project inside`../test`

![life.gif](./ref/life.gif)

See [triceCheck.c](../pkg/src/triceCheck.c) for reference. The *Trices* can come mixed from inside interrupts (light blue `ISR:...`) or from normal code. For usage with a RTOS *Trices* are protected against breaks (`TRICE_ENTER_CRITICAL_SECTION`, `TRICE_LEAVE_CRITICAL_SECTION`). Regard the differences in the read SysTick values inside the GIF above These differences are the MCU clocks needed for one trice (~0,25µs@48MHz).

Use the `-color off` switch for piping output in a file.

<!---
//###  6.2. <a name='Checkthetricebinary'></a>Check the `trice` binary
* Copy command trice into a path directory.
* Run inside a shell `trice check -list path/to/trice/examples/triceDemoF030R8/MDK-ARM/`[til.json](../examples/triceDemoF030R8/MDK-ARM/til.json). You should see output like this:
![](./ref/Check.PNG)

//###  6.2. <a name='InstrumentatargetsourcecodeprojectHowtousetriceinyourproject'></a>Instrument a target source code project (How to use trice in your project)

Look at one of the appropriate test projects as example. In general:

* Copy [triceConfig.h](https://github.com/rokath/trice/tree/master/pkg/src/intern/triceConfig.h) and adapt to your needs.
* Make sure the [trice.h](https://github.com/rokath/trice/blob/master/pkg/src/trice.h) header file is found by your compiler.
* Include [trice.c](https://github.com/rokath/trice/blob/master/pkg/src/trice.c)

Next steps:

* Add `#include "trice.h"` to your project files where to use TRICE macros and put `TRICE0( "msg:Hello world!\n" );` after your initialization code.
* Run `trice u` at the root of your source code. Afterwards:
  * It should have changed into `TRICE0( Id(12345), "msg:Hello world!\n" );` as example. (The `12345` stays here for a 20-bit non-zero random number).
  * A file [til.json](https://github.com/rokath/trice/blob/master/til.json)  (**t**race **i**d **l**ist) should have been generated.
* Set up timer and UART interrupt and main loop in the right way. Analyze the test example projects for advice.
* For help have a look at the differences between these 2 projects:
  * `../test/MDK-ARM_LL_generatedDemo_STM32F030R8-NUCLEO-64` * It is just the STM32 CubeMX generated code.
  * `../test/MDK-ARM_LL_UART_RTT0_FLEX_STM32F030R8-NUCLEO-64` * It is a copy of the above enhanced with trice check code.

* Add your compiler definitions to `trice/pkg/src/intern/triceConfigCompiler.h`
* Make a copy of `trice/pkg/src/intern/triceUART_LL_STM32.h`, rename the copy appropriate an fill these 4 functions with the hardware specific code:

```C
TRICE_INLINE uint32_t triceTxDataRegisterEmpty(void){
}

TRICE_INLINE void triceTransmitData8(uint8_t v) {
}

TRICE_INLINE void triceEnableTxEmptyInterrupt(void) {
}

TRICE_INLINE void triceDisableTxEmptyInterrupt(void) {
}
```

Quick workaround:

```C
* Leave these definitions empty: 
  * triceTxDataRegisterEmpty()
  * triceEableTxEmptyInterrupt()
  * triceDisableTxEmptyInterrupt()
* Use:
  * void triceTransmitData8( uint8_t d ){
    my_putchar( (char)d); // your code
  }
  Call TxStart();TxContinue(); cyclically in sufficient long intervals like 1 ms
```

* After compiling and flashing run `trice -port COMn -baud m` with n and m set to correct values
* Now start your device and you should see the hello world message coming from your target. In fact the hello-world string never went to the embedded device, only the ID comes from  there and the string is found in the [til.json](https://github.com/rokath/trice/blob/master/til.json) file of your project.
* If you use a legacy project containing `printf()` statements you can simply transform them to **TRICE\*** statements. TRICE32 will do in most cases but for better performance take **TRICE8** or **TRICE16** where possible.
* `printf(...)` statements containing string format specifier are quickly portable by using `TRICE_P(...)` but without the trice space and speed advantage. The TRICE_P() is intended only for the few dynamic strings in a ported  project.  Enable `TRICE_PRINTF_ADAPTER` increases the needed code size by a few KB.
* It could be helpful to add `trice u ...` as prebuild step into your toolchain for each file or for the project as a whole.
  This way you cannot forget the update step, it performs automatically.
-->

<p align="right">(<a href="#top">back to top</a>)</p>

##  7. <a name='Encryption'></a>Encryption

* You can deliver your device with encrypted trices. This way only the service is able to read the *Trices*.
* Implemented is XTEA but this is easy exchangeable.
* The to 8 byte padded blocks can get encrypted by enabling `#define ENCRYPT...` inside *triceConfig.h*. You need to add `-key test` as **log** switch and you're done.
* Any password is usable instead of `test`. Simply add once the `-show` switch and copy the displayed passphrase into the *config.h* file.
* The encryption takes part **before** the [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) encoding.

<p align="right">(<a href="#top">back to top</a>)</p>

##  8. <a name='CLIOptionsfortricetool'></a>CLI Options for `trice` tool

The trice tool is very easy to use even it has a plenty of options. Most of them normally not needed.
The trice tool can be started in several modes (sub-commands), each with several mandatory or optional switches. Switches can have parameters or not.

```b
trice sub-command -switch1 -switch2 parameter -switch3 ...
```

Which sub-command switches are usable for each sub-command is shown with `trice help -all`. This gives also information about their default values.

Info for a special sub-command is shown with `trice h -l`, `trice h -u`, ... .

<p align="right">(<a href="#top">back to top</a>)</p>

##  9. <a name='Tricecommandlineexamples'></a>*Trice* command line examples

* The **trice** tool has many command line options, but is easy to use with default values.
* No [config file](./TriceConfigFile.md) implemented yet. But the command history is usable for example inside the bash, simply enter CTRL-R and start typing `trice...` and you can select from the history.

###  9.1. <a name='Commoninformation'></a>Common information

* `trice h -all` shows all options of the current version.
* `trice ver` prints version information.
* `trice s` shows you all found serial ports for your convenience.
* `trice l -p COM17` could fail if s.th. is wrong. Additional switches are for help tracking the issue:
  * Use log witch `-s[howInputBytes]` to check if any bytes are received at all. ![./ref/ShowInputBytesExample.PNG](./ref/ShowInputBytesExample.PNG)
  * With `-debug` you can see the COBS and decoded and single *Trice* packages. ![./ref/DebugSwitchExample.PNG](./ref/DebugSwitchExample.PNG)

<!--
* `trice u` in the root of your project parses all source files for `TRICE` macros, adds automatically ID´s if needed and updates a file named **til.json** containing all ID´s with their format string information. To start simply generate an empty file named **til.json** in your project root. You can add `trice u` to your build process and need no further manual execution.

* `trice ds` starts a display server listening on default ip address *127.0.0.1:61487* or any specified value, so also on a remote device, lets say with ip address 192.168.1.200.
* `trice l -p COM18 -ds` sends the log strings to a display server with default ip address *127.0.0.1:61487* or any specified value, if for example `-ipa 192.168.1.200` the trice logs go to the remote device. You can start several trice log instances, all transmitting to the same display server.
-->

###  9.2. <a name='Furtherexamples'></a>Further examples

####  9.2.1. <a name='Automatedpre-buildupdatecommandexample'></a>Automated pre-build update command example

* Scan directories `../src`, `../lib/src` and `./` to update the IDs there and extend list file `../../../til.json`

```bash
trice u -v -i ../../../til.json -src ../src -src ../lib/src -src ./
```

This is a typical line you can add to your project as an automatic pre-compile step.

####  9.2.2. <a name='SomeLogexamples'></a>Some Log examples

* Log trice messages on COM3 8N1 115200 baud

```bash
trice log -i ./myProject/til.json -p=COM3
```

* Log trice messages on COM3 8N1 9600 baud and use default til.json

```bash
trice l -s COM3 -baud=9600
```

####  9.2.3. <a name='Loggingoveradisplayserver'></a>Logging over a display server

* Start displayserver on ip 127.0.0.1 (localhost) and port 61497

```bash
trice ds
```

* Log trice messages on COM3 and display on display server

```bash
trice l -ds -p COM3
```

* Shutdown remote display server on IP 192.168.1.23 port 45678

```bash
trice sd -r 192.168.1.23:45678
```

####  9.2.4. <a name='Logfileoutput'></a>Logfile output

```bash
trice l -p COM3 -logfile auto
```

This creates a new logfile `2022-05-16_2216-40_trice.log` with the actual timestamp on each **trice** start.

```bash
trice l -p COM3 -logfile trice.log
```

This creates a new logfile `trice.log` on first start and appends to it on each next **trice** start.

Logfiles are text files one can see with 3rd party tools. Example: `cat trice.log`. They contain also the PC reception timestamps if where enabled.

####  9.2.5. <a name='BinaryLogfile'></a>Binary Logfile

```bash
trice l -p COM3 -binaryLogfile auto
```

This creates a new binary logfile `2022-05-16_2216-40_trice.bin` with the actual timestamp on each **trice** start.

```bash
trice l -p COM3 -binaryLogfile trice.bin
```

This creates a new binary logfile `trice.bin` on first start and appends to it on each next **trice** start.

Binary logfiles store the **trice** messages as they come out of the target in binary form. They are much smaller than normal logfiles, but the **trice** tool with the *til.sjon* is needed for displaying them and the PC timestamps are the displaying time: `trice -p FILEBUFFER -args trice.log`.

Binary logfiles are handy in the field for long data recordings.
####  9.2.6. <a name='TCPoutput'></a>TCP output

```bash
trice l -p COM3 -tcp 127.0.0.1:23
```

This additionally sends **trice** output to a 3rd party TCP listener, for example like Putty:

![./ref/PuttyConfig1.PNG](./ref/PuttyConfig1.PNG) ![./ref/PuttyConfig2.PNG](./ref/PuttyConfig2.PNG)
![./ref/Putty.PNG](./ref/Putty.PNG)

####  9.2.7. <a name='SetallIDsinadirectorytreeto0'></a>Set all IDs in a directory tree to 0

```bash
trice zeroSourceTreeIds -src ./ 
```

![./ref/ZeroIDsExample.PNG](./ref/ZeroIDsExample.PNG)

* Normally nobody uses that. But if you intend to integrate some existing sources into a project using [ID management](./TriceIDManagement.md) options, this could be a need.
* Calling `trice u` afterwards will assign new IDs, but calling `trice u -shared IDs` will assign the same IDs again.

####  9.2.8. <a name='StimulatetargetwithausercommandoverUART'></a>Stimulate target with a user command over UART

Sometimes it is handy to stimulate the target during development. For that a 2nd screen is helpful what is possible using the display server option:

![./ref/UARTCommandAnimation.gif](./ref/UARTCommandAnimation.gif)

####  9.2.9. <a name='Explporeandmodifychannelsandtheircolors'></a>Explpore and modify channels and their colors

See file [./TriceColor.md](./TriceColor.md)

####  9.2.10. <a name='LocationInformation'></a>Location Information

When running  `trice update`, a file `li.json` is created, what you can control with the `-locationInformation` switch. During logging, when `li.json` is found, automatically the filename and line number is displayed in front of each log line, controllable with the `-liFmt` switch. This information is correct only with the right version of the `li.json` file. That is usually the case on the PC during development. Out in the field only the `til.json` reference is of importance. It serves an an accumulator of all firmware versions and usually the latest version of this file is the best fit. The `li.json` file should stay with the software developer only and needs no version control in the usual case because it is rebuild with each compilation, when `trice u` is a prebuild step.

<p align="right">(<a href="#top">back to top</a>)</p>

##  10. <a name='Additionalhints'></a>Additional hints

###  10.1. <a name='Pre-builtexecutablesareavailable'></a>Pre-built executables are available

See [https://github.com/rokath/trice/releases](https://github.com/rokath/trice/releases).
###  10.2. <a name='ConfigurationfiletriceConfig.h'></a>Configuration file `triceConfig.h`

* When setting up your first project you need a `triceConfig.h` file.
* You should **not** use the `./pkg/src/inc/triceConfig.h` because it is customized for internal tests with CGO.
* Please choose one of the `./test/.../triceConfig.h` files as starting point.
* Comparing them and understandig the differences helps quick starting.

###  10.3. <a name='Settinguptheveryfirstconnection'></a>Setting up the very first connection

If you see nothing in the beginning, what is normally ;-), add the `-s` (`-showInputBytes`) switch to see if any data arrive. There is also a switch `-debug` showing you the received packages, if you are interested in.

###  10.4. <a name='Avoidbufferoverruns'></a>Avoid buffer overruns

It is your responsibility to produce less data than transmittable. If this is not guarantied a data loss is not avoidable or you have to slow down the program. The double buffer as fastest solution has no overflow check. My recommendation: Make the buffer big and emit the maxDepth cyclically, every 10 or 1000 seconds. Then you know the needed size. It is influenced by the max trice burst and the buffer switch interval.

If the target application produces more *Trice* data than transmittable, a buffer overrun can let the target crash, because for performance reasons no overflow check is implemented in the double buffer. Also if such a check is added, the *Trice* code can only throw data away in such case.

It is planned to make the code secure against buffer overruns in the future. But anyway data losses will occur when producing more data than transmittable.
That is detectable with the cycle counter. The internal 8-bit cycle counter is usually enabled. If *Trice* data are lost, the receiver side will detect that because the cycle counter is not as expected. There is a chance of 1/256 that the detection does not work. You can check the detection by unplugging the trice UART cable for a time. Also resetting the target during transmission should display a cycle error.


###  10.5. <a name='LimitationtriceurequiresTRICEmacrosonasingleline'></a>Limitation "trice u" requires TRICE macros on a single line

* The implemented parser (currently) does not support `TRICE` macros over several source code lines. Each `TRICE` macro needs to be completely on one line.
* It is possible to have several (complete) `TRICE` macros on one source code line.

###  10.6. <a name='LimitationTRICEinTRICEnotpossible'></a>Limitation TRICE in TRICE not possible

* No-Good Example:

```C
int f0( void ){ TRICE( "msg:f0\n"); return 0; }
void f1( void ){ TRICE( "No; %d", f0() ); }
```

* This will compile normally but corrupt TRICE output.

The reason is: When f1() gets active, the "No" *Trice* header is created, than the f0() *Trice* is executed and afterwards the "No" *Trice* tail is written. This works well during compile time but causes a mismatch during runtime.

* Good Workaround:

```C
int f0( void ){ TRICE( "msg:f0\n"); return 0; }
void f1( void ){ int x = f0(); TRICE( "Yes: %d", x ); }
```

###  10.7. <a name='DynamicstringsbuffersonlyasvariableinsideTRICEmacros'></a>Dynamic strings/buffers only as variable inside `TRICE` macros

* No-Good Example:

```C
void f0( void ){ TRICE_S( "msg:%s\n", "Hello" ); } // will not work
```

* Possible, but not recommended Workaround:

```C
void f0( void ){ const char* s = "Hello"; TRICE_S( "msg:%s\n", s ); } // works, but ineffective
```

The above code line causes the string "Hello" to be transferred bytewise. One can rewrite it to much more effective and smaller code:

```C
void f0( void ){ TRICE( "msg:Hello\n" ); }
```

By the way, string concatenation within TRICE macros is untested and expected not to work. The reason lays inside the way the update tool works (right now):

```C
void f0( void ){ TRICE( "msg:" ## "Hello\n" ); } // ERROR!
```

**Usage intention and recommendation:** (given by @escherstair)

```C
char dynamic_string[50];
fillDynamicStringFromSomewhere(dynamic_string);   /* the content of dynamic_string is filled at run time */
TRICE_S( "msg:This part of the string is known at compile time. This part is dynamic: %s\n", dynamic_string);
```
All the string literals (i.e. compile-time known strings) should be put inside the format string.
Only the dynamic strings should be used as variables in TRICE_S macro.

###  10.8. <a name='Logfileviewing'></a>Logfile viewing

Logfiles, **trice** tool generated with sub-command switch `-color off`, are normal ASCII files. If they are with color codes, these are ANSI escape sequences.

* Simply `cat trice.log`. One view option is also `less -R trice.log`. The Linux command `less` is also available inside the windows git bash.
* Under Windows one could also download and use [ansifilter](https://sourceforge.net/projects/ansifilter/) for logfile viewing. A monospaced font is recommended.
* See also [Color issues under Windows](./TriceColor.md#color-issues-under-windows)

###  10.9. <a name='Usingthetricetoolwith3rdpartytools'></a>Using the `trice` tool with 3rd party tools

Parallel output as logfile, TCP or binary logfile is possible. See examples above.

###  10.10. <a name='Severaltargetsatthesametime'></a>Several targets at the same time

You can connect each target over its transmit channel with an own **trice** instance and integrate all transmissions line by line in an additional **trice** instance acting as display server. See [https://github.com/rokath/trice#display-server-option](https://github.com/rokath/trice#display-server-option).

###  10.11. <a name='Executinggotest-race-count100....'></a>Executing `go test -race -count 100 ./...`

The C-code is executed during some tests. Prerequisite is a installed GCC.

###  10.12. <a name='DirectTRICEOutTRICE_MODE0couldcausestackoverflowwith-o0optimization'></a>Direct TRICE Out (TRICE_MODE 0) could cause stack overflow with -o0 optimization

As discussed in [issue #294](https://github.com/rokath/trice/issues/294) it can happen, that several TRICE macros within one function call increase the stack usage more than expected, when compiler optimization is totally switched off.

<p align="right">(<a href="#top">back to top</a>)</p>

##  11. <a name='TargetsideTriceOn-Off'></a>Target side *Trice* On-Off

* If your code works well after checking, you can add `#define TRICE_OFF` just before the `#include "trice.h"` line and no *trice* code is generated anymore for that file, so no need to delete or comment out `TRICE` macros: : ![./ref/TRICE_OFF.PNG](./ref/TRICE_OFF.PNG)
* No runtime On-Off switch is implemented for  several reasons:
  * Would need a control channel to the target.
  * Would add little performance and code overhead.
  * Would sligtly change target timing (testing).
  * User can add its own switches anywhere.
  * The short `TRICE` macro code is negligible.
  * The trice output is encryptable, if needed.
* Because of the low *Trice* bandwidth needs and to keep the target code as clear as possible the runtime On-Off decision should be done by the **trice** tool.
* See also issue [#243](https://github.com/rokath/trice/issues/243).

<p align="right">(<a href="#top">back to top</a>)</p>

##  12. <a name='HostsideTriceOn-Off'></a>Host side *Trice* On-Off

* The PC **trice** tool offers command line switches to `-pick` or `-ban` for *trice* channels and will be extended with display switches.
* A **trice** tool logLevel switch is usable too (Issue [#236](https://github.com/rokath/trice/issues/236)).

<p align="right">(<a href="#top">back to top</a>)</p>

##  13. <a name='Usingadifferentencoding'></a>Using a different encoding

It is possible to exchange the code behind the `TRICE` macros with a different encoding and to add an appropriate decoder to the **trice** tool.
The ID assignment is adjustable with `-IDMin` and `-IDMax`.

<!--

<p align="right">(<a href="#top">back to top</a>)</p>

//##  14. <a name='Sub-commandcheckNotimplemented'></a>Sub-command `check` (Not implemented!)

* `trice check` will check the JSON list and emit all TRICE statements inside the list once with a dataset.

//###  14.1. <a name='checkswitch-datasetNotimplemented'></a>`check` switch '-dataset' (Not implemented!)

* This is a `string` switch. It has one parameter. Its default value is `position`. That means each parameter has a different value. This is useful for testing.
* The `negative` value is uses a dataset with negative values for testing.
* Running `trice check` should show your message, indicating everything is fine so far.
-->
