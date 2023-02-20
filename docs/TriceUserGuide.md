# *Trice* user guide

> _(Read this)
>
> SORRY: This document is currently a mess and partially outdated.

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
    - [2.4. Port it](#24-port-it)
      - [2.4.1. Target Code Overview](#241-target-code-overview)
  - [3. Build `trice` tool from Go sources (you can skip that)](#3-build-trice-tool-from-go-sources-you-can-skip-that)
  - [4.  Embedded system code configuratio](#4--embedded-system-code-configuratio)
  - [5. `trice` tool in logging action](#5-trice-tool-in-logging-action)
  - [6. Encryption](#6-encryption)
  - [7. CLI Options for `trice` tool](#7-cli-options-for-trice-tool)
  - [8. *Trice* command line examples](#8-trice-command-line-examples)
    - [8.1. Common information](#81-common-information)
    - [8.2. Further examples](#82-further-examples)
      - [8.2.1. Automated pre-build update command example](#821-automated-pre-build-update-command-example)
      - [8.2.2. Some Log examples](#822-some-log-examples)
      - [8.2.3. Logging over a display server](#823-logging-over-a-display-server)
      - [8.2.4. Logfile output](#824-logfile-output)
      - [8.2.5. Binary Logfile](#825-binary-logfile)
      - [8.2.6. TCP output](#826-tcp-output)
      - [8.2.7. Set all IDs in a directory tree to 0](#827-set-all-ids-in-a-directory-tree-to-0)
      - [8.2.8. Stimulate target with a user command over UART](#828-stimulate-target-with-a-user-command-over-uart)
      - [8.2.9. Explpore and modify channels and their colors](#829-explpore-and-modify-channels-and-their-colors)
      - [8.2.10. Location Information](#8210-location-information)
  - [9. Additional hints](#9-additional-hints)
    - [9.1. Pre-built executables are available](#91-pre-built-executables-are-available)
    - [9.2. Configuration file `triceConfig.h`](#92-configuration-file-triceconfigh)
    - [9.3. Setting up the very first connection](#93-setting-up-the-very-first-connection)
    - [9.4. Avoid buffer overruns](#94-avoid-buffer-overruns)
    - [9.5. Limitation "trice u" requires TRICE macros on a single line](#95-limitation-trice-u-requires-trice-macros-on-a-single-line)
    - [9.6. Limitation TRICE in TRICE not possible](#96-limitation-trice-in-trice-not-possible)
    - [9.7. Dynamic strings/buffers only as variable inside `TRICE` macros](#97-dynamic-stringsbuffers-only-as-variable-inside-trice-macros)
    - [9.8. Logfile viewing](#98-logfile-viewing)
    - [9.9. Using the `trice` tool with 3rd party tools](#99-using-the-trice-tool-with-3rd-party-tools)
    - [9.10. Several targets at the same time](#910-several-targets-at-the-same-time)
    - [9.11. Executing `go test -race -count 100 ./...`](#911-executing-go-test--race--count-100-)
    - [9.12. Direct TRICE Out (TRICE\_MODE 0) could cause stack overflow with -o0 optimization](#912-direct-trice-out-trice_mode-0-could-cause-stack-overflow-with--o0-optimization)
  - [10. Target side *Trice* On-Off](#10-target-side-trice-on-off)
  - [11. Host side *Trice* On-Off](#11-host-side-trice-on-off)
  - [12. Using a different encoding](#12-using-a-different-encoding)
  - [13. Testing](#13-testing)
    - [13.1. Folder information](#131-folder-information)
    - [13.2. Package specific information](#132-package-specific-information)
    - [13.3. todo](#133-todo)
  - [14. Trice demo \& test project info](#14-trice-demo--test-project-info)
    - [14.1. Backup folder](#141-backup-folder)
    - [14.2. Terminal info](#142-terminal-info)
    - [14.3. RTT info](#143-rtt-info)
    - [14.4. UART](#144-uart)
    - [14.5. MDK-ARM projects](#145-mdk-arm-projects)
  - [15. Further info](#15-further-info)
  - [16. Third party Software](#16-third-party-software)
  - [17. alacritty](#17-alacritty)
    - [17.1. goST](#171-gost)
    - [17.2. keil.com](#172-keilcom)
    - [17.3. segger.com](#173-seggercom)
      - [17.3.1. SEGGER downloaded Software](#1731-segger-downloaded-software)
      - [17.3.2. JLink](#1732-jlink)
      - [17.3.3. SEGGER\_RTT](#1733-segger_rtt)
      - [17.3.4. STLinkReflash\_190812.zip](#1734-stlinkreflash_190812zip)
  - [18. st.com](#18-stcom)

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
| cmd/stim     | (do not use) target stimulation tool tryout code|
| docs/        | documentation                                   |
| internal/    | `trice` tool internal Go packages               |
| pkg/         | `trice` tool common Go packages                 |
| src/         | C sources for trice instrumentation             |
| test/        | example target projects and tests               |
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
* `trice` does not make any assumptions about the target processor - 8-bit to 64-bit and any endianness are supported.
* The ARM µVision MDK is free downloadable and free usable for STM M0/M0+ MCUs, like the `./test/MDK-ARM_STM32F030R8` project.
  * Even if you do not have such hardware, you can download ARM µVision MDK and compile the `./test/MDK-ARM_STM32F030R8` project just to get started.
  * When adding or modifying `trice` macros inside `.test/MDK-ARM_STM32F030R8/Core/Src/main.c` and recompiling you should see automatically changed ID numbers inside the code.
  
###  2.4. <a name='Portit'></a>Port it 

Compair folder `./test/MDK-ARM_STM32F030R8_generated` with `./test/MDK-ARM_STM32F030R8` to see in a quick way any needed adaptions for your target project to port trice to it.

Main steps are:

* Add `./src/trice.c` to your project. It includes the files in ./src/box automatically.
* Add `./src/` to your library include path.
* Copy file `./test/MDK-ARM_STM32F030R8/Core/Inc/triceConfig.h` to your embedded project and adapt it to your needs.
  * Other `triceConfig.h` files are usable as well, but the above is usually the most actual one.
* Copy file `./test/MDK-ARM_STM32F030R8/Core/Inc/SEGGER_RTT_Conf.h` to your embedded project and adapt it to your needs, when using RTT.
* Add 2 functions to your project:
  
    ```c
    ///////////////////////////////////////////////////////////////////////////////
    // (time) stamp funktions, rewrite these hardware specific functions in your project.
    // 
    
    // 16-bit us stamp, wraps after 10 milliseconds
    uint16_t TriceStamp16( void ){
        return 0x1616;
    }
    
    // 32-bit us stamp, wraps after 71,58 seconds
    uint32_t TriceStamp32( void ){
        return 0x32323232;
    }
    
    //
    ///////////////////////////////////////////////////////////////////////////////
    ```

* Optionally copy all or parts parts of `./test/testdata/triceCheck.c` to your project if you wish to perform some checks.
  * Do not inlucde it directly, because it gets overwritten when `updateTestData.sh` is executed inside the `./test` folder.
* In your source.c files add line `#include "trice.h"`
* In a function write a trice message like: `TRice( "1/11 = %g\n", aFloat( 1.0/11 ) );`.
* In **project root**:
  * Create empty file: `touch til.json`.
  * Witch correct CLI switches `trice u` should perform **automatically** these things (The numbers are just examples.):
>>>    * Patch source.c to `TRice( iD(12363), "1/11 = %g\n", aFloat( 1.0/11 ) );`
>>>      * C & H files containing TRICE macros, are only modified if needed (missing or obsolete ID)
>>>    * Extend `til.json`
>>>      * If no `til.json` is found nothing happens. At least an empty file is needed (Safety feature).
* When the program runs later, it should output something similar to ![./ref/1div11.PNG](./ref/1div11.PNG)
* It is up to the user to provide the 2 functions `void TriceStamp16(void)` and `void TriceStamp32(void)`.
  * They are usually provide time stamps but could be used in any manner.
  * They are defined as `__WEAK` functions, so that you can skip their coding in the beginning.
* For RTT the SEGGER source is already included.
* For UART transfer add UART write functionality.

####  2.4.1. <a name='TargetCodeOverview'></a>Target Code Overview

* `./src`:

| File                                  | description |
| -                                     | -           |
| `trice.h` & `trice.c`                 | trice runtime lib user interface, `#include trice.h` in project files, where to use `TRICE` macros. |

* `./src/box`:

| File                                  | description |
| -                                     | -           |
| `cobs*.*`                             | message packaging, alternatively for tcobs |
| `trice.h` & `trice.c`                 | trice runtime lib user interface, `#include trice.h` in project files, where to use `TRICE` macros. |
| `core.c`                              | trice core lib |
| `SEGGER_RTT.*`                        | Segger RTT code |
| `tcobs*.*`                            | message compression and packaging |
| `trice8.*`                            | 8-bit trice code |
| `trice16.*`                           | 16-bit trice code |
| `trice32.*`                           | 32-bit trice code |
| `trice64.*`                           | 64-bit trice code |
| `triceDoubleBuffer.c`                 | trice runtime lib extension needed for fastest indirect mode |
| `triceModbusBuffer.c`                 | trice runtime lib extension needed for Modbus mode (not usable yet) |
| `triceStackBuffer.c`                  | trice runtime lib extension needed for direct mode |
| `triceStreamBuffer.c`                 | trice runtime lib extension needed for recommended indirect mode |
| `xtea.*`                              | UNTESTED with TREX, needed for XTEA message encryption, if enabled |
  
* The TCOBS files are copied from [https://github.com/rokath/tcobs/tree/master/TCOBSv1](https://github.com/rokath/tcobs/tree/master/TCOBSv1). They are maintained there and extensively tested and probably not a matter of significant change.
* The SEGGER files are copied from and you could check for a newer version at [https://www.segger.com/downloads/jlink/](https://www.segger.com/downloads/jlink/).

<p align="right">(<a href="#top">back to top</a>)</p>

##  3. <a name='BuildtricetoolfromGosourcesyoucanskipthat'></a>Build `trice` tool from Go sources (you can skip that)

* Install [Go](https://golang.org/).
* On Windows you need to install [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/).
  * Take the 64-bit variant when Go is 64-bit or take the 32-bit variant when Go is 32-bit. If mixed installations work I doubt.
  * Recommendation: Minimal online installer.
  * GCC is only needed to test the target C-code on the host.
  * Make sure TDM-GCC is found first in the path, if you have several compilers installed.
  * Other gcc variants could work also but not tested.
* Open a console inside the `trice` directory, recommended is the git-bash, when using Windows.
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

The last tests can last quite a while, depending on your machine.
Afterwards you should find an executable `trice` inside $GOPATH/bin/ and you can modify its source code.

<p align="right">(<a href="#top">back to top</a>)</p>

##  4. <a name='Embeddedsystemcodeconfiguratio'></a> Embedded system code configuratio

Check comments inside `triceConfig.h`.

<!--
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

-##  5. <a name='Adaptyourlegacysourcecode'></a>Adapt your legacy source code

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

When you compare the needed FLASH size before and after you probably will see more free space afterwards, because the *Trice* code is less than 1 KB, no format strings anymore inside the target and you do not need a printf library anymore. Be aware that `TRICE` is a macro adding each time it is used some code.


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

##  5. <a name='tricetoolinloggingaction'></a>`trice` tool in logging action

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

##  6. <a name='Encryption'></a>Encryption

* You can deliver your device with encrypted trices. This way only the service is able to read the *Trices*.
* Implemented is XTEA but this is easy exchangeable.
* The to 8 byte padded blocks can get encrypted by enabling `#define ENCRYPT...` inside *triceConfig.h*. You need to add `-key test` as **log** switch and you're done.
* Any password is usable instead of `test`. Simply add once the `-show` switch and copy the displayed passphrase into the *config.h* file.
* The encryption takes part **before** the [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) encoding.

<p align="right">(<a href="#top">back to top</a>)</p>

##  7. <a name='CLIOptionsfortricetool'></a>CLI Options for `trice` tool

The trice tool is very easy to use even it has a plenty of options. Most of them normally not needed.
The trice tool can be started in several modes (sub-commands), each with several mandatory or optional switches. Switches can have parameters or not.

```b
trice sub-command -switch1 -switch2 parameter -switch3 ...
```

Which sub-command switches are usable for each sub-command is shown with `trice help -all`. This gives also information about their default values.

Info for a special sub-command is shown with `trice h -l`, `trice h -u`, ... .

<p align="right">(<a href="#top">back to top</a>)</p>

##  8. <a name='Tricecommandlineexamples'></a>*Trice* command line examples

* The **trice** tool has many command line options, but is easy to use with default values.
* No [config file](./TriceConfigFile.md) implemented yet. But the command history is usable for example inside the bash, simply enter CTRL-R and start typing `trice...` and you can select from the history.

###  8.1. <a name='Commoninformation'></a>Common information

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

###  8.2. <a name='Furtherexamples'></a>Further examples

####  8.2.1. <a name='Automatedpre-buildupdatecommandexample'></a>Automated pre-build update command example

* Scan directories `../src`, `../lib/src` and `./` to update the IDs there and extend list file `../../../til.json`

```bash
trice u -v -i ../../../til.json -src ../src -src ../lib/src -src ./
```

This is a typical line you can add to your project as an automatic pre-compile step.

####  8.2.2. <a name='SomeLogexamples'></a>Some Log examples

* Log trice messages on COM3 8N1 115200 baud

```bash
trice log -i ./myProject/til.json -p=COM3
```

* Log trice messages on COM3 8N1 9600 baud and use default til.json

```bash
trice l -s COM3 -baud=9600
```

####  8.2.3. <a name='Loggingoveradisplayserver'></a>Logging over a display server

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

####  8.2.4. <a name='Logfileoutput'></a>Logfile output

```bash
trice l -p COM3 -logfile auto
```

This creates a new logfile `2022-05-16_2216-40_trice.log` with the actual timestamp on each **trice** start.

```bash
trice l -p COM3 -logfile trice.log
```

This creates a new logfile `trice.log` on first start and appends to it on each next **trice** start.

Logfiles are text files one can see with 3rd party tools. Example: `cat trice.log`. They contain also the PC reception timestamps if where enabled.

####  8.2.5. <a name='BinaryLogfile'></a>Binary Logfile

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
####  8.2.6. <a name='TCPoutput'></a>TCP output

```bash
trice l -p COM3 -tcp 127.0.0.1:23
```

This additionally sends **trice** output to a 3rd party TCP listener, for example like Putty:

![./ref/PuttyConfig1.PNG](./ref/PuttyConfig1.PNG) ![./ref/PuttyConfig2.PNG](./ref/PuttyConfig2.PNG)
![./ref/Putty.PNG](./ref/Putty.PNG)

####  8.2.7. <a name='SetallIDsinadirectorytreeto0'></a>Set all IDs in a directory tree to 0

```bash
trice zeroSourceTreeIds -src ./ 
```

![./ref/ZeroIDsExample.PNG](./ref/ZeroIDsExample.PNG)

* Normally nobody uses that. But if you intend to integrate some existing sources into a project using [ID management](./TriceIDManagement.md) options, this could be a need.
* Calling `trice u` afterwards will assign new IDs, but calling `trice u -shared IDs` will assign the same IDs again.

####  8.2.8. <a name='StimulatetargetwithausercommandoverUART'></a>Stimulate target with a user command over UART

Sometimes it is handy to stimulate the target during development. For that a 2nd screen is helpful what is possible using the display server option:

![./ref/UARTCommandAnimation.gif](./ref/UARTCommandAnimation.gif)

####  8.2.9. <a name='Explporeandmodifychannelsandtheircolors'></a>Explpore and modify channels and their colors

See file [./TriceColor.md](./TriceColor.md)

####  8.2.10. <a name='LocationInformation'></a>Location Information

When running  `trice update`, a file `li.json` is created, what you can control with the `-locationInformation` switch. During logging, when `li.json` is found, automatically the filename and line number is displayed in front of each log line, controllable with the `-liFmt` switch. This information is correct only with the right version of the `li.json` file. That is usually the case on the PC during development. Out in the field only the `til.json` reference is of importance. It serves an an accumulator of all firmware versions and usually the latest version of this file is the best fit. The `li.json` file should stay with the software developer only and needs no version control in the usual case because it is rebuild with each compilation, when `trice u` is a prebuild step.

<p align="right">(<a href="#top">back to top</a>)</p>

##  9. <a name='Additionalhints'></a>Additional hints

###  9.1. <a name='Pre-builtexecutablesareavailable'></a>Pre-built executables are available

See [https://github.com/rokath/trice/releases](https://github.com/rokath/trice/releases).
###  9.2. <a name='ConfigurationfiletriceConfig.h'></a>Configuration file `triceConfig.h`

* When setting up your first project you need a `triceConfig.h` file.
* You should **not** use the `./pkg/src/inc/triceConfig.h` because it is customized for internal tests with CGO.
* Please choose one of the `./test/.../triceConfig.h` files as starting point.
* Comparing them and understandig the differences helps quick starting.

###  9.3. <a name='Settinguptheveryfirstconnection'></a>Setting up the very first connection

If you see nothing in the beginning, what is normally ;-), add the `-s` (`-showInputBytes`) switch to see if any data arrive. There is also a switch `-debug` showing you the received packages, if you are interested in.

###  9.4. <a name='Avoidbufferoverruns'></a>Avoid buffer overruns

It is your responsibility to produce less data than transmittable. If this is not guarantied a data loss is not avoidable or you have to slow down the program. The double buffer as fastest solution has no overflow check. My recommendation: Make the buffer big and emit the maxDepth cyclically, every 10 or 1000 seconds. Then you know the needed size. It is influenced by the max trice burst and the buffer switch interval.

If the target application produces more *Trice* data than transmittable, a buffer overrun can let the target crash, because for performance reasons no overflow check is implemented in the double buffer. Also if such a check is added, the *Trice* code can only throw data away in such case.

It is planned to make the code secure against buffer overruns in the future. But anyway data losses will occur when producing more data than transmittable.
That is detectable with the cycle counter. The internal 8-bit cycle counter is usually enabled. If *Trice* data are lost, the receiver side will detect that because the cycle counter is not as expected. There is a chance of 1/256 that the detection does not work. You can check the detection by unplugging the trice UART cable for a time. Also resetting the target during transmission should display a cycle error.


###  9.5. <a name='LimitationtriceurequiresTRICEmacrosonasingleline'></a>Limitation "trice u" requires TRICE macros on a single line

* The implemented parser (currently) does not support `TRICE` macros over several source code lines. Each `TRICE` macro needs to be completely on one line.
* It is possible to have several (complete) `TRICE` macros on one source code line.

###  9.6. <a name='LimitationTRICEinTRICEnotpossible'></a>Limitation TRICE in TRICE not possible

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

###  9.7. <a name='DynamicstringsbuffersonlyasvariableinsideTRICEmacros'></a>Dynamic strings/buffers only as variable inside `TRICE` macros

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

###  9.8. <a name='Logfileviewing'></a>Logfile viewing

Logfiles, **trice** tool generated with sub-command switch `-color off`, are normal ASCII files. If they are with color codes, these are ANSI escape sequences.

* Simply `cat trice.log`. One view option is also `less -R trice.log`. The Linux command `less` is also available inside the windows git bash.
* Under Windows one could also download and use [ansifilter](https://sourceforge.net/projects/ansifilter/) for logfile viewing. A monospaced font is recommended.
* See also [Color issues under Windows](./TriceColor.md#color-issues-under-windows)

###  9.9. <a name='Usingthetricetoolwith3rdpartytools'></a>Using the `trice` tool with 3rd party tools

Parallel output as logfile, TCP or binary logfile is possible. See examples above.

###  9.10. <a name='Severaltargetsatthesametime'></a>Several targets at the same time

You can connect each target over its transmit channel with an own **trice** instance and integrate all transmissions line by line in an additional **trice** instance acting as display server. See [https://github.com/rokath/trice#display-server-option](https://github.com/rokath/trice#display-server-option).

###  9.11. <a name='Executinggotest-race-count100....'></a>Executing `go test -race -count 100 ./...`

The C-code is executed during some tests. Prerequisite is a installed GCC.

###  9.12. <a name='DirectTRICEOutTRICE_MODE0couldcausestackoverflowwith-o0optimization'></a>Direct TRICE Out (TRICE_MODE 0) could cause stack overflow with -o0 optimization

As discussed in [issue #294](https://github.com/rokath/trice/issues/294) it can happen, that several TRICE macros within one function call increase the stack usage more than expected, when compiler optimization is totally switched off.

<p align="right">(<a href="#top">back to top</a>)</p>

##  10. <a name='TargetsideTriceOn-Off'></a>Target side *Trice* On-Off

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

##  11. <a name='HostsideTriceOn-Off'></a>Host side *Trice* On-Off

* The PC **trice** tool offers command line switches to `-pick` or `-ban` for *trice* channels and will be extended with display switches.
* A **trice** tool logLevel switch is usable too (Issue [#236](https://github.com/rokath/trice/issues/236)).

<p align="right">(<a href="#top">back to top</a>)</p>

##  12. <a name='Usingadifferentencoding'></a>Using a different encoding

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

##  13. <a name='Testing'></a>Testing


###  13.1. <a name='Folderinformation'></a>Folder information

- The folders here, despite `testdata`, are helper "projects" for testing the target C-code located in `trice/src/`.
- Some folders are hardware specific implementations and some are Go packages. The Go packages can have all the same name, only the folder names are not equal.
- In each Go package a different triceConfig.h is used, this way allowing to check all modes automatically, including encryption.
- The file `./testdata/triceCheck.c.txt` is the master test pattern for all CGO tests and edited manually. It has the extension `.txt` to avoid accidentally modification by the `trice u` command.
- After editing and before executing the tests, `./updateTestData.sh` needs to be executed. It copies into `./testdate/generated_triceCheck.c`, and a `trice u -src triceCheck.c` is needed. Than the modified `./testdata/generated_triceCheck.c` is compiled into the test executables in the `./cgo_*` folders.
- The file `./testdata/generated_triceCheck.c` is copied into the memory filesystem and used there to extract the expected results (//exp: comments).
- The fresh `./testdata/til.json` is used inside the memory filesystem during the tests.
- They execute `cgo.TriceCheck(i)` this way activating the target code which writes into a buffer. The buffer is copied into a FILEBUFFER inside the memory file system and the trice tool is reading it.


- In a post-compile step a `trice z` should restore the `triceCheck.c` unmodified state to be identical to `./testdata/triceCheck_EditThisFile_NotTheTriceCheckDotC.txt`.
- Unfortunately this has to be done on the os filesystem.

###  13.2. <a name='Packagespecificinformation'></a>Package specific information

- Each C function gets a Go wrapper which ist tested in appropriate test functions.
- For some reason inside the trice_test.go an 'import "C"' is not possible.
- All C-files in the packages folder referring to the trice sources this way avoiding code duplication.
- The Go functions defined in the packages are not exported. They are called by the Go test functions in this package.
- This way the package test functions are executing the trice C-code compiled with the triceConfig.h there.

###  13.3. <a name='todo'></a>todo

- repair cgo0_tcobs
- repair cgo1_tcobs
- cgo2_tcobs: ref_cobs.c_?
- Parse "C:\repos\trice\cgo\test\cgo2_tcobs\triceCheck.c" and complete TestTriceCheck

<p align="right">(<a href="#top">back to top</a>)</p>

##  14. <a name='Tricedemotestprojectinfo'></a>Trice demo & test project info

Many demo projects are ARMKeil IDE STM32 here but the TRICE tool is easy adaptable to 8 - 64-bit architectures.

###  14.1. <a name='Backupfolder'></a>Backup folder

- This folder contains unmaintained legacy test projects.
- They are only for reference and need some corrections to work with the current release.
- See also [../docs/TestExamples.md](../docs/TestExamples.md)

###  14.2. <a name='Terminalinfo'></a>Terminal info

- Open an escape sequence capable terminal in trice root `C:\repos\trice\`opr where you put it.
  - Any directory will do as well but the `til.json` file needs to be found.
  - git-bash will do or also windows-terminal from Microsoft store.
    - Under Windows the DOS or powershell could have display issues with the escaped ASCII codes. Search the internet for answers.

###  14.3. <a name='RTTinfo'></a>RTT info

- The example projects support both, RTT and UART just for demonstration.
- RTT = Real Time Transfer is a technique developed by SEGGER for background memory access during processor runtime. This is possible for ARM cores over the JTAG or SWD interface if a debug probe is connected.
- Some STM development boards contain a debug probe you can use for the board itself or for another board.
- These are so called on-board ST-LINK debug probes. To use RTT use the `-p STLINK` switch.
  - `stRttLogger.exe` and `libusb-1.0.dll` must be in the PATH.
- You can also flash the on-board debug probe with J-LINK firmware. To use RTT use the `-p JLINK` switch.
  - `JLinkRTTLogger.exe` and `JLinkARM.dll` must be in the PATH.

###  14.4. <a name='UART'></a>UART

- The default baud rate is 115200 for all test projects and the trice tool assumes that baud rate automatically. Use the trice tool `-baud` switch for other settings.

###  14.5. <a name='MDK-ARMprojects'></a>MDK-ARM projects

The projects are generated with necessary library files *as reference* to keep them smaller. Therefore, the direct compilation will fail, probably. Projects should compile if you follow these steps:

- Install latest ARMKeil IDE.
  - STM32 M0 cores with up to 32 KB FLASH memory are usable directly.
  - You can get a free ST Microelectronics license with a 256 KB FLASH limitation.
- Install latest STM32CubeMX with the latest target firmware library.
  - It is free of charge.
- Open the project ioc file with STM32CubeMX and re-generate.
- Open the project with ARMKeil IDE and build.

<!-- ###  16.6. <a name='Details'></a>Details


- [MDK-ARM_LL_generatedDemo_STM32F030R8-NUCLEO-64\ReadMe.md](MDK-ARM_LL_generatedDemo_STM32F030R8-NUCLEO-64\ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_BARE_STM32F070RB_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_BARE_STM32F070RB_NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_BARE_STM32F091_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_BARE_STM32F091_NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_ESC_STM32F030R8_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_ESC_STM32F030R8_NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_ESC_STM32F070RB_NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_ESC_STM32F070RB_NUCLEO-64/ReadMe.md)
- [MDK-ARM_LL_UART_RTT0_WRAP_STM32F030R8-NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_RTT0_WRAP_STM32F030R8-NUCLEO-64/ReadMe.md)
- [MDK-ARM_RTT0_BARE_STM32F0308-DISCO/ReadMe.md](MDK-ARM_RTT0_BARE_STM32F0308-DISCO/ReadMe.md)
- [MDK-ARM_RTT0_BARE_STM32F03051R8Tx-DISCOVERY/ReadMe.md](MDK-ARM_RTT0_BARE_STM32F03051R8Tx-DISCOVERY/ReadMe.md)
- [MDK-ARM_RTT0_BARE_STM32F03051R8Tx-DISCOVERY/ReadMe.md](MDK-ARM_RTT0_BARE_STM32F03051R8Tx-DISCOVERY/ReadMe.md)
- [MDK-ARM_LL_UART_WRAP_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md](MDK-ARM_LL_UART_WRAP_RTT0_BARE_STM32F030R8-NUCLEO-64/ReadMe.md)

-->- [*Trice* user guide](#trice-user-guide)
- [*Trice* user guide](#trice-user-guide)
  - [1. Project structure](#1-project-structure)
  - [2. Get started](#2-get-started)
    - [2.1. Get it](#21-get-it)
    - [2.2. Install It](#22-install-it)
    - [2.3. Use It](#23-use-it)
    - [2.4. Port it](#24-port-it)
      - [2.4.1. Target Code Overview](#241-target-code-overview)
  - [3. Build `trice` tool from Go sources (you can skip that)](#3-build-trice-tool-from-go-sources-you-can-skip-that)
  - [4.  Embedded system code configuratio](#4--embedded-system-code-configuratio)
  - [5. `trice` tool in logging action](#5-trice-tool-in-logging-action)
  - [6. Encryption](#6-encryption)
  - [7. CLI Options for `trice` tool](#7-cli-options-for-trice-tool)
  - [8. *Trice* command line examples](#8-trice-command-line-examples)
    - [8.1. Common information](#81-common-information)
    - [8.2. Further examples](#82-further-examples)
      - [8.2.1. Automated pre-build update command example](#821-automated-pre-build-update-command-example)
      - [8.2.2. Some Log examples](#822-some-log-examples)
      - [8.2.3. Logging over a display server](#823-logging-over-a-display-server)
      - [8.2.4. Logfile output](#824-logfile-output)
      - [8.2.5. Binary Logfile](#825-binary-logfile)
      - [8.2.6. TCP output](#826-tcp-output)
      - [8.2.7. Set all IDs in a directory tree to 0](#827-set-all-ids-in-a-directory-tree-to-0)
      - [8.2.8. Stimulate target with a user command over UART](#828-stimulate-target-with-a-user-command-over-uart)
      - [8.2.9. Explpore and modify channels and their colors](#829-explpore-and-modify-channels-and-their-colors)
      - [8.2.10. Location Information](#8210-location-information)
  - [9. Additional hints](#9-additional-hints)
    - [9.1. Pre-built executables are available](#91-pre-built-executables-are-available)
    - [9.2. Configuration file `triceConfig.h`](#92-configuration-file-triceconfigh)
    - [9.3. Setting up the very first connection](#93-setting-up-the-very-first-connection)
    - [9.4. Avoid buffer overruns](#94-avoid-buffer-overruns)
    - [9.5. Limitation "trice u" requires TRICE macros on a single line](#95-limitation-trice-u-requires-trice-macros-on-a-single-line)
    - [9.6. Limitation TRICE in TRICE not possible](#96-limitation-trice-in-trice-not-possible)
    - [9.7. Dynamic strings/buffers only as variable inside `TRICE` macros](#97-dynamic-stringsbuffers-only-as-variable-inside-trice-macros)
    - [9.8. Logfile viewing](#98-logfile-viewing)
    - [9.9. Using the `trice` tool with 3rd party tools](#99-using-the-trice-tool-with-3rd-party-tools)
    - [9.10. Several targets at the same time](#910-several-targets-at-the-same-time)
    - [9.11. Executing `go test -race -count 100 ./...`](#911-executing-go-test--race--count-100-)
    - [9.12. Direct TRICE Out (TRICE\_MODE 0) could cause stack overflow with -o0 optimization](#912-direct-trice-out-trice_mode-0-could-cause-stack-overflow-with--o0-optimization)
  - [10. Target side *Trice* On-Off](#10-target-side-trice-on-off)
  - [11. Host side *Trice* On-Off](#11-host-side-trice-on-off)
  - [12. Using a different encoding](#12-using-a-different-encoding)
  - [13. Testing](#13-testing)
    - [13.1. Folder information](#131-folder-information)
    - [13.2. Package specific information](#132-package-specific-information)
    - [13.3. todo](#133-todo)
  - [14. Trice demo \& test project info](#14-trice-demo--test-project-info)
    - [14.1. Backup folder](#141-backup-folder)
    - [14.2. Terminal info](#142-terminal-info)
    - [14.3. RTT info](#143-rtt-info)
    - [14.4. UART](#144-uart)
    - [14.5. MDK-ARM projects](#145-mdk-arm-projects)
  - [15. Further info](#15-further-info)
  - [16. Third party Software](#16-third-party-software)
  - [17. alacritty](#17-alacritty)
    - [17.1. goST](#171-gost)
    - [17.2. keil.com](#172-keilcom)
    - [17.3. segger.com](#173-seggercom)
      - [17.3.1. SEGGER downloaded Software](#1731-segger-downloaded-software)
      - [17.3.2. JLink](#1732-jlink)
      - [17.3.3. SEGGER\_RTT](#1733-segger_rtt)
      - [17.3.4. STLinkReflash\_190812.zip](#1734-stlinkreflash_190812zip)
  - [18. st.com](#18-stcom)

<p align="right">(<a href="#top">back to top</a>)</p>

##  15. <a name='Furtherinfo'></a>Further info

<p align="right">(<a href="#top">back to top</a>)</p>

##  16. <a name='ThirdpartySoftware'></a>Third party Software

<p align="right">(<a href="#top">back to top</a>)</p>

##  17. <a name='alacritty'></a>alacritty

- A fast, cross-platform, OpenGL terminal emulator

###  17.1. <a name='goST'></a>goST

- see Segger RTT over STLINK

###  17.2. <a name='keil.com'></a>keil.com

- Tooling info

<!--- ## microchip.com

- Planned 8 bit example

-->

<!--- ### nxp.com

- Planned NXP example

-->

###  17.3. <a name='segger.com'></a>segger.com

- Tooling around Segger RTT, Download latest version from SEGGER web site.

####  17.3.1. <a name='SEGGERdownloadedSoftware'></a>SEGGER downloaded Software

- Check in the Internet for newer versions.

####  17.3.2. <a name='JLink'></a>JLink

- Download and install [J-LinkSoftwareAndDocumentationPack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack) or simply use `JLinkRTTLogger.exe` and accompanying `JLinkARM.dll` copied from default install location `C:\Program Files (x86)\SEGGER\JLink`. Both files are inside `JLinkRTTLogger.zip` You need to put to a location in \$PATH or extend \$PATH.

####  17.3.3. <a name='SEGGER_RTT'></a>SEGGER_RTT

- Target code is expected inside SEGGER_RTT. This is the extracted SEGGER_RTT_V....zip.
- Optionally check for a newer version.

####  17.3.4. <a name='STLinkReflash_190812.zip'></a>STLinkReflash_190812.zip

- Tool for exchanging ST-LINK and J-LINK software on STM32 evaluation boards.
  - Works not for v3 Hardware but well for v2 Hardware.
  - In case of not accepting the ST-Link firmware use [../st.com/en.stsw-link007_V2-37-26.zip](../st.com/en.stsw-link007_V2-37-26.zip) for updating the ST-Link firmware first. It could be you need to exchange the ST-Link firmware variant into the variant with mass storage.

<p align="right">(<a href="#top">back to top</a>)</p>

##  18. <a name='st.com'></a>st.com

- STMicroelectronics

<p align="right">(<a href="#top">back to top</a>)</p>