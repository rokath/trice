# *Trice* user guide

> _(Read this)_

<!-- vscode-markdown-toc -->
* 1. [Project structure](#Projectstructure)
* 2. [Get started](#Getstarted)
	* 2.1. [Get it](#Getit)
	* 2.2. [Install It](#InstallIt)
	* 2.3. [Use It](#UseIt)
* 3. [Build `trice` tool from Go sources (you can skip that)](#BuildtricetoolfromGosourcesyoucanskipthat)
* 4. [Embedded system code setup](#Embeddedsystemcodesetup)
* 5. [Adapt your legacy source code](#Adaptyourlegacysourcecode)
* 6. [`trice` tool in logging action](#tricetoolinloggingaction)
* 7. [Encryption](#Encryption)
* 8. [CLI Options for `trice` tool](#CLIOptionsfortricetool)
* 9. [*Trice* command line examples](#Tricecommandlineexamples)
	* 9.1. [Common information](#Commoninformation)
	* 9.2. [Further examples](#Furtherexamples)
		* 9.2.1. [Automated pre-build update command example](#Automatedpre-buildupdatecommandexample)
		* 9.2.2. [Some Log examples](#SomeLogexamples)
		* 9.2.3. [Logging over a display server](#Loggingoveradisplayserver)
		* 9.2.4. [Set all IDs in a directory tree to 0](#SetallIDsinadirectorytreeto0)
* 10. [Additional hints](#Additionalhints)
	* 10.1. [Limitation TRICE in TRICE not possible!](#LimitationTRICEinTRICEnotpossible)
	* 10.2. [Logfile viewing](#Logfileviewing)
* 11. [Target side *Trice* On-Off](#TargetsideTriceOn-Off)
* 12. [Host side *Trice* On-Off](#HostsideTriceOn-Off)
* 13. [Using a different encoding](#Usingadifferentencoding)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

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
  * There are only 2 important commands: `trice u[pdate]` and `trice l[og]`.
    * `trice h -u[pdate]` and `trice h -l[og]` show partial help.
* Copy 3 files to your embedded project:
  * `./pkg/src/trice.h`
  * `./pkg/src/trice.c`
  * `./test/.../triceConfig.h`
* In your source.c files add line `#include "trice.h"`
* In a function write: `TRICE( "1/11 = %g\n", aFloat( 1.0/11 ) );`
* In project root:
  * Create empty file: `touch til.json`.
  * Run `trice u` should perform automatically these things (The numbers are just examples.):
>>>    * Patch source.c to `TRICE( Id(60363), "1/11 = %g\n", aFloat( 1.0/11 ) );`
>>>      * C & H files containing TRICE macros are only modified if needed (missing or obsolete Id(n))
>>>    * Extend `til.json`
>>>      * If no `til.json` is found nothing happens. At least an empty file is needed (Safety feature).
>>>    * Add a line `#define TRICE_FILE Id(54321)` after the `#include "trice.h"` line in your source.c files.
>>>      * Hint: Only source.c files with a `#include "trice.h"` line get patched.
* When the program runs later it should output something similar to![./ref/1div11.PNG](./ref/1div11.PNG)

##  3. <a name='BuildtricetoolfromGosourcesyoucanskipthat'></a>Build `trice` tool from Go sources (you can skip that)

* Install [Go](https://golang.org/).
* On Windows you need to install [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/) 
  * Recommendation: Minimal online installer.
  * GCC is only needed for [./pkg/src/src.go](https://github.com/rokath/trice/blob/master/pkg/src/src.go), what gives the option to test the C-code on the host.
  * Make sure TDM-GCC is found first in the path.
  * Other gcc variants could work also but not tested.
* Open a console inside the `trice` directory.
* Check and install:

```b
go vet ./...
go test ./...
go install ./...
```

Afterwards you should find an executable `trice` inside $GOPATH/bin/ and you can modify its source code.

* The used serial Go driver package is Linux & Windows tested but should work on MacOS soon too.
* For historical reasons there are 2 serial drivers inside the **trice** tool. This will be changed.

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
  * Target timestamps and their time base
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

##  5. <a name='Adaptyourlegacysourcecode'></a>Adapt your legacy source code

For example change the legacy source code line

```c
printf( "%d Kelvin\n", k );
```

into

```c
TRICE( "%d Kelvin\n", k );
```

This you could do automatically using a word processor. Care must be taken in the following cases:

* More than 12 printf parameters: use several printf-calls
* float numbers: surround each with `aFloat()`
* double numbers: surround each with `aDouble` and use the `TRICE64` macro
* runtime generated strings: Each needs its own `TRICE_S` macro: `TRICE_S( Id(11223), "Entered name is %20s\n", "Paul" );`

A `trice update` (run it later in the tool chain to keep everything automatically up-to-date) inserts the *Trice* IDs:

```c
TRICE( Id(12345), "%d Kelvin\n", k );
```

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

##  7. <a name='Encryption'></a>Encryption

* You can deliver your device with encrypted trices. This way only the service is able to read the *Trices*.
* Implemented is XTEA but this is easy exchangeable.
* The to 8 byte padded blocks can get encrypted by enabling `#define ENCRYPT...` inside *triceConfig.h*. You need to add `-key test` as **log** switch and you're done.
* Any password is usable instead of `test`. Simply add once the `-show` switch and copy the displayed passphrase into the *config.h* file.
* The encryption takes part **before** the [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) encoding.

##  8. <a name='CLIOptionsfortricetool'></a>CLI Options for `trice` tool

The trice tool is very easy to use even it has a plenty of options. Most of them normally not needed.
The trice tool can be started in several modes (sub-commands), each with several mandatory or optional switches. Switches can have parameters or not.

```b
trice sub-command -switch1 -switch2 parameter -switch3 ...
```

Which sub-command switches are usable for each sub-command is shown with `trice help -all`. This gives also information about their default values.

Output of `trice h -all`: (Actual version could slightly differ)

```bash
$ trice h -all
syntax: 'trice sub-command' [params]
sub-command 'ds|displayServer': Starts a display server.
        Use in a separate console. On Windows use wt (https://github.com/microsoft/terminal) or a linux shell like git-bash to avoid ANSI color issues.
        Running "trice ds" inside a console opens a display server to be used for displaying the TRICE logs remotely.
        Several instances of 'trice l -ds -port ...' (for different ports) will send output there in parallel.
  -color string
        The format strings can start with a lower or upper case channel information.
        See https://github.com/rokath/trice/blob/master/pkg/src/triceCheck.c for examples. Color options:
        "off": Disable ANSI color. The lower case channel information is kept: "w:x"-> "w:x"
        "none": Disable ANSI color. The lower case channel information is removed: "w:x"-> "x"
        "default|color": Use ANSI color codes for known upper and lower case channel info are inserted and lower case channel information is removed.
         (default "default")
  -ipa string
        IP address like '127.0.0.1'.
        You can specify this switch if you intend to use the remote display option to show the output on a different PC in the network.
         (default "localhost")
  -ipp string
        16 bit IP port number.
        You can specify this switch if you want to change the used port number for the remote display functionality.
         (default "61497")
  -logfile string
        Append all output to logfile. Options are: 'off|none|filename|auto':
        "off": no logfile (same as "none")
        "none": no logfile (same as "off")
        "auto": Use as logfile name "2006-01-02_1504-05_trice.log" with actual time.
        "filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
        All trice output of the appropriate subcommands is appended per default into the logfile trice additionally to the normal output.
        Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
         (default "off")
example: 'trice ds': Start display server.
sub-command 'h|help': For command line usage.
        "trice h" will print this help text as a whole.
  -all
        Show all help.
  -displayserver
        Show ds|displayserver specific help.
  -ds
        Show ds|displayserver specific help.
  -h    Show h|help specific help.
  -help
        Show h|help specific help.
  -l    Show l|log specific help.
  -log
        Show l|log specific help.
  -logfile string
        Append all output to logfile. Options are: 'off|none|filename|auto':
        "off": no logfile (same as "none")
        "none": no logfile (same as "off")
        "auto": Use as logfile name "2006-01-02_1504-05_trice.log" with actual time.
        "filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
        All trice output of the appropriate subcommands is appended per default into the logfile trice additionally to the normal output.
        Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
         (default "off")
  -r    Show r|refresh specific help.
  -refresh
        Show r|refresh specific help.
  -renew
        Show renew specific help.
  -s    Show s|scan specific help.
  -scan
        Show s|scan specific help.
  -sd
        Show sd|shutdown specific help.
  -shutdown
        Show sd|shutdown specific help.
  -u    Show u|update specific help.
  -update
        Show u|update specific help.
  -v    short for verbose
  -ver
        Show ver|version specific help.
  -verbose
        Gives more informal output if used. Can be helpful during setup.
        For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -version
        Show ver|version specific help.
  -z    Show zeroSourceTreeIds specific help.
  -zeroSourceTreeIds
        Show zeroSourceTreeIds specific help.
example 'trice h': Print short help.
example 'trice h -all': Print all help.
example 'trice h -log': Print log help.
sub-command 'l|log': For displaying trice logs coming from port. With "trice log" the trice tool display mode is activated.
  -args string
        Use to pass port specific parameters. The "default" value depends on the used port:
        port "COMn": default="", use "TARM" for a different driver. (For baud rate settings see -baud.)
        port "J-LINK": default="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000",
                The -RTTSearchRanges "..." need to be written without "" and with _ instead of space.
                For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.
        port "ST-LINK": default="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000",
                The -RTTSearchRanges "..." need to be written without "" and with _ instead of space.
                For args options see JLinkRTTLogger in SEGGER UM08001_JLink.pdf.
        port "BUFFER": default="0 0 0 0", Option for args is any byte sequence.
         (default "default")
  -ban value
        Channel(s) to ignore. This is a multi-flag switch. It can be used several times with a colon separated list of channel descriptors not to display.
        Example: "-ban dbg:wrn -ban diag" results in suppressing all as debug, diag and warning tagged messages. Not usable in conjunction with "-pick".
  -baud int
        Set the serial port baudrate.
        It is the only setup parameter. The other values default to 8N1 (8 data bits, no parity, one stopbit).
         (default 115200)
  -color string
        The format strings can start with a lower or upper case channel information.
        See https://github.com/rokath/trice/blob/master/pkg/src/triceCheck.c for examples. Color options:
        "off": Disable ANSI color. The lower case channel information is kept: "w:x"-> "w:x"
        "none": Disable ANSI color. The lower case channel information is removed: "w:x"-> "x"
        "default|color": Use ANSI color codes for known upper and lower case channel info are inserted and lower case channel information is removed.
         (default "default")
  -dc int
        Dumped bytes per line when "-encoding DUMP" (default 32)
  -debug
        Show additional debug information
  -defaultTRICEBitwidth string
        The expected value bit width for TRICE macros. Must be in sync with setting is triceConfig.h (default "32")
  -displayserver
        Send trice lines to displayserver @ ipa:ipp.
        Example: "trice l -port COM38 -ds -ipa 192.168.178.44" sends trice output to a previously started display server in the same network.
  -ds
        Short for '-displayserver'.
  -e string
        Short for -encoding. (default "COBS")
  -encoding string
        The trice transmit data format type, options: '(CHAR|COBS|DUMP|ESC|FLEX)'. Target device encoding must match.
                          CHAR prints the received bytes as characters.
                          COBS expects 0 delimited byte sequences.
                          DUMP prints the received bytes as hex code (see switch -dc too).
                          ESC is a legacy format and will be removed in the future.
                          FLEX is a legacy format and will be removed in the future.
         (default "COBS")
  -i string
        Short for '-idlist'.
         (default "til.json")
  -idList string
        Alternate for '-idlist'.
         (default "til.json")
  -idlist string
        The trice ID list file.
        The specified JSON file is needed to display the ID coded trices during runtime and should be under version control.
         (default "til.json")
  -ipa string
        IP address like '127.0.0.1'.
        You can specify this switch if you intend to use the remote display option to show the output on a different PC in the network.
         (default "localhost")
  -ipp string
        16 bit IP port number.
        You can specify this switch if you want to change the used port number for the remote display functionality.
         (default "61497")
  -logfile string
        Append all output to logfile. Options are: 'off|none|filename|auto':
        "off": no logfile (same as "none")
        "none": no logfile (same as "off")
        "auto": Use as logfile name "2006-01-02_1504-05_trice.log" with actual time.
        "filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
        All trice output of the appropriate subcommands is appended per default into the logfile trice additionally to the normal output.
        Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
         (default "off")
  -p string
        short for -port (default "J-LINK")
  -password string
        The decrypt passphrase. If you change this value you need to compile the target with the appropriate key (see -showKeys).
        Encryption is recommended if you deliver firmware to customers and want protect the trice log output. This does work right now only with flex and flexL format.
  -pick value
        Channel(s) to display. This is a multi-flag switch. It can be used several times with a colon separated list of channel descriptors only to display.
        Example: "-pick err:wrn -pick default" results in suppressing all messages despite of as error, warning and default tagged messages. Not usable in conjunction with "-ban".
  -port string
        receiver device: 'ST-LINK'|'J-LINK'|serial name.
        The serial name is like 'COM12' for Windows or a Linux name like '/dev/tty/usb12'.
        Using a virtual serial COM port on the PC over a FTDI USB adapter is a most likely variant.
         (default "J-LINK")
  -prefix string
        Line prefix, options: any string or 'off|none' or 'source:' followed by 0-12 spaces, 'source:' will be replaced by source value e.g., 'COM17:'. (default "source: ")
  -pw string
        Short for -password.
  -s    Short for '-showInputBytes'.
  -showID string
        Format string for displaying first trice ID at start of each line. Example: "debug:%7d ". Default is "". If several trices form a log line only the first trice ID ist displayed.
  -showInputBytes
        Show incoming bytes, what can be helpful during setup.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -showKey
        Show encryption key. Use this switch for creating your own password keys. If applied together with "-password MySecret" it shows the encryption key.
        Simply copy this key than into the line "#define ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret" inside triceConfig.h.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -suffix string
        Append suffix to all lines, options: any string.
  -tLocFmt string
        Target location format string at start of each line, if target location existent (configured). Use "" to suppress existing target location. If several trices form a log line only the location of f
irst trice ist displayed. (default "%12s:%4d ")
  -targetEndianess string
        Target endianness trice data stream. Option: "bigEndian". (default "littleEndian")
  -testTable
        Generate testTable output and ignore -prefix, -suffix, -ts, -color. This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -til string
        Short for '-idlist'.
         (default "til.json")
  -ts string
        PC timestamp for logs and logfile name, options: 'off|none|UTCmicro|zero'
        This timestamp switch generates the timestamps on the PC only (reception time), what is good enough for many cases.
        "LOCmicro" means local time with microseconds.
        "UTCmicro" shows timestamps in universal time.
        When set to "off" no PC timestamps displayed.
        If you need target timestamps you need to get the time inside the target and send it as TRICE* parameter.
         (default "LOCmicro")
  -ttsf string
        Target timestamp format string at start of each line, if target timestamps existent (configured). Use "" to suppress existing target timestamps. If several trices form a log line only the timestam
p of first trice ist displayed. (default "time:%9d ")
  -u    Short for '-unsigned'. (default true)
  -unsigned
        Hex, Octal and Bin values are printed as unsigned values. (default true)
  -v    short for verbose
  -verbose
        Gives more informal output if used. Can be helpful during setup.
        For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
example: 'trice l -p COM15 -baud 38400': Display trice log messages from serial port COM15
example: 'trice l': Display flexL data format trice log messages from default source J-LINK over Segger RTT protocol.
example: 'trice l -port ST-LINK -v -s': Shows verbose version information and also the received raw bytes.
sub-command 'r|refresh': For updating ID list from source files but does not change the source files.
        "trice refresh" will parse source tree(s) for TRICE macros, and refresh/generate the JSON list.
        This command should be run on adding source files to the project before the first time "trice update" is called.
        If the new source files contain TRICE macros with IDs these are added to til.json if not already used.
        Already used IDs are reported, so you have the chance to remove them from til.son and then do "trice u" again.
        This way you can make sure to get the new sources unchanged in your list.
        Already used IDs are replaced by new IDs during the next "trice update", so the old IDs in the list will survive.
        If you do not refresh the list after adding source files and perform an "trice update" new generated IDs could be equal to
        IDs used in the added sources with the result that IDs in the added sources could get changed what you may not want.
        Using "trice u -IDMethod random" (default) makes the chance for such conflicts very low.
        The "refresh" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
  -dry-run
        No changes applied but output shows what would happen.
        "trice refresh -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -i string
        Short for '-idlist'.
         (default "til.json")
  -idList string
        Alternate for '-idlist'.
         (default "til.json")
  -idlist string
        The trice ID list file.
        The specified JSON file is needed to display the ID coded trices during runtime and should be under version control.
         (default "til.json")
  -s value
        Short for src.
  -src value
        Source dir or file, It has one parameter. Not usable in the form "-src *.c".
        This is a multi-flag switch. It can be used several times for directories and also for files.
        Example: "trice refresh -dry-run -v -src ./test/ -src pkg/src/trice.h" will scan all C|C++ header and
        source code files inside directory ./test and scan also file trice.h inside pkg/src directory.
        Without the "-dry-run" switch it would create|extend a list file til.json in the current directory.
         (default "./")
  -til string
        Short for '-idlist'.
         (default "til.json")
  -v    short for verbose
  -verbose
        Gives more informal output if used. Can be helpful during setup.
        For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
example: 'trice refresh': Update ID list from source tree.
sub-command 'renew': It is like refresh, but til.json is cleared first, so all 'old' trices are removed. Use with care.
  -dry-run
        No changes applied but output shows what would happen.
        "trice renew -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -i string
        Short for '-idlist'.
         (default "til.json")
  -idList string
        Alternate for '-idlist'.
         (default "til.json")
  -idlist string
        The trice ID list file.
        The specified JSON file is needed to display the ID coded trices during runtime and should be under version control.
         (default "til.json")
  -s value
        Short for src.
  -src value
        Source dir or file, It has one parameter. Not usable in the form "-src *.c".
        This is a multi-flag switch. It can be used several times for directories and also for files.
        Example: "trice renew -dry-run -v -src ./test/ -src pkg/src/trice.h" will scan all C|C++ header and
        source code files inside directory ./test and scan also file trice.h inside pkg/src directory.
        Without the "-dry-run" switch it would create|extend a list file til.json in the current directory.
         (default "./")
  -til string
        Short for '-idlist'.
         (default "til.json")
  -v    short for verbose
  -verbose
        Gives more informal output if used. Can be helpful during setup.
        For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
example: 'trice renew': Rebuild ID list from source tree, discard old IDs.
sub-command 's|scan': Shows available serial ports)
example: 'trice s': Show COM ports.
sub-command 'sd|shutdown': Ends display server at IPA:IPP, works also on a remote machine.
  -ipa string
        IP address like '127.0.0.1'.
        You can specify this switch if you intend to use the remote display option to show the output on a different PC in the network.
         (default "localhost")
  -ipp string
        16 bit IP port number.
        You can specify this switch if you want to change the used port number for the remote display functionality.
         (default "61497")
example: 'trice sd': Shut down remote display server.
sub-command 'ver|version': For displaying version information.
        "trice v" will print the version information. If trice is not versioned the build time will be displayed instead.
  -logfile string
        Append all output to logfile. Options are: 'off|none|filename|auto':
        "off": no logfile (same as "none")
        "none": no logfile (same as "off")
        "auto": Use as logfile name "2006-01-02_1504-05_trice.log" with actual time.
        "filename": Any other string than "auto", "none" or "off" is treated as a filename. If the file exists, logs are appended.
        All trice output of the appropriate subcommands is appended per default into the logfile trice additionally to the normal output.
        Change the filename with "-logfile myName.txt" or switch logging off with "-logfile none".
         (default "off")
  -v    short for verbose
  -verbose
        Gives more informal output if used. Can be helpful during setup.
        For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
sub-command 'u|update': For updating ID list and source files.
        "trice update" will parse source tree(s) for new or changed TRICE macros, modify them appropriate and update/generate the JSON list.
        The "update" sub-command has no mandatory switches. Omitted optional switches are used with their default parameters.
  -IDMax value
        Upper end of ID range for normal trices. (default 65535)
  -IDMethod string
        Search method for new ID's in range- Options are 'upward', 'downward' & 'random'. (default "random")
  -IDMin value
        Lower end of ID range for normal trices. (default 32768)
  -addParamCount
        Extend TRICE macro names with the parameter count _n to enable compile time checks.
  -dry-run
        No changes applied but output shows what would happen.
        "trice update -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -i string
        Short for '-idlist'.
         (default "til.json")
  -idList string
        Alternate for '-idlist'.
         (default "til.json")
  -idlist string
        The trice ID list file.
        The specified JSON file is needed to display the ID coded trices during runtime and should be under version control.
         (default "til.json")
  -s value
        Short for src.
  -sharedIDs
        ID policy:
        true: TriceFmt's without TriceID get equal TriceID if an equal TriceFmt exists already.
        false: TriceFmt's without TriceID get a different TriceID if an equal TriceFmt exists already.
  -src value
        Source dir or file, It has one parameter. Not usable in the form "-src *.c".
        This is a multi-flag switch. It can be used several times for directories and also for files.
        Example: "trice update -dry-run -v -src ./test/ -src pkg/src/trice.h" will scan all C|C++ header and
        source code files inside directory ./test and scan also file trice.h inside pkg/src directory.
        Without the "-dry-run" switch it would create|extend a list file til.json in the current directory.
         (default "./")
  -til string
        Short for '-idlist'.
         (default "til.json")
  -v    short for verbose
  -verbose
        Gives more informal output if used. Can be helpful during setup.
        For example "trice u -dry-run -v" is the same as "trice u -dry-run" but with more descriptive output.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
example: 'trice update -src ../A -src ../../B': Parse ../A and ../../B with all subdirectories for TRICE IDs to update and adjusts til.json
sub-command 'zeroSourceTreeIds': Set all Id(n) inside source tree dir to Id(0).
        Avoid using this sub-command normally. The switch "-src" is mandatory and no multi-flag here.
        This sub-command is mainly for testing. For several source directories you need several runs.
  -dry-run
        No changes applied but output shows what would happen.
        "trice zeroSourceTreeIds -dry-run" will change nothing but show changes it would perform without the "-dry-run" switch.
        This is a bool switch. It has no parameters. Its default value is false. If the switch is applied its value is true.
  -src string
        Zero all Id(n) inside source tree dir, required.
example: 'trice zeroSourceTreeIds -src ../A': Sets all TRICE IDs to 0 in ../A. Use with care!

```

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

####  9.2.4. <a name='SetallIDsinadirectorytreeto0'></a>Set all IDs in a directory tree to 0

```bash
trice zeroSourceTreeIds -src ./ 
```

![./ref/ZeroIDsExample.PNG](./ref/ZeroIDsExample.PNG)

* Normally nobody uses that. But if you intend to integrate some existing sources into a project using [ID management](./TriceIDManagement.md) options, this could be a need.
* Calling `trice u` afterwards will assign new IDs, but calling `trice u -shared IDs` will assign the same IDs again.

##  10. <a name='Additionalhints'></a>Additional hints

###  10.1. <a name='LimitationTRICEinTRICEnotpossible'></a>Limitation TRICE in TRICE not possible!

- No-Good Example:
```C
int f0( void ){ TRICE( "msg:f0\n"); return 0; }
void f1( void ){ TRICE( "No; %d", f0() ); }
```
- This will compile normally but corrupt TRICE output.

The reason is: When f1() gets active, the "No" *Trice* header is created, than the f0() *Trice* is executed and afterwards the "No" *Trice* tail is written. This works well during compile time but causes a mismatch during runtime.

- Good Workaround:
```C
int f0( void ){ TRICE( "msg:f0\n"); return 0; }
void f1( void ){ int x = f0(); TRICE( "Yes: %d", x ); }
```

### Dynamic strings/buffers only as variable inside `TRICE` macros

- No-Good Example:
```C
void f0( void ){ TRICE_S( "msg:%s\n", "Hello" ); }
```

- Good Workaround:
```C
void f0( void ){ char* s = "Hello"; TRICE_S( "msg:%s\n", s ); }
```

The reason lays in the way the *Trices* are processed.

###  10.2. <a name='Logfileviewing'></a>Logfile viewing

Logfiles, **trice** tool generated with sub-command switch `-color off`, are normal ASCII files. If they are with color codes, these are ANSI escape sequences.

* One easy view option is `less -R trice.log`. The Linux command `less` is also available inside the windows git bash.
* Under Windows one could also download and use [ansifilter](https://sourceforge.net/projects/ansifilter/) for logfile viewing. A monospaced font is recommended.
* See also [Color issues under Windows](./TriceColor.md#color-issues-under-windows)

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

##  12. <a name='HostsideTriceOn-Off'></a>Host side *Trice* On-Off

* The PC **trice** tool offers command line switches to `-pick` or `-ban` for *trice* channels and will be extended with display switches.
* A **trice** tool logLevel switch is usable too (Issue [#236](https://github.com/rokath/trice/issues/236)).

##  13. <a name='Usingadifferentencoding'></a>Using a different encoding

It is possible to exchange the code behind the `TRICE` macros with a different encoding and to add an appropriate decoder to the **trice** tool.
The ID assignment is adjustable with `-IDMin` and `-IDMax`.

<!--
//##  14. <a name='Sub-commandcheckNotimplemented'></a>Sub-command `check` (Not implemented!)

* `trice check` will check the JSON list and emit all TRICE statements inside the list once with a dataset.

//###  14.1. <a name='checkswitch-datasetNotimplemented'></a>`check` switch '-dataset' (Not implemented!)

* This is a `string` switch. It has one parameter. Its default value is `position`. That means each parameter has a different value. This is useful for testing.
* The `negative` value is uses a dataset with negative values for testing.
* Running `trice check` should show your message, indicating everything is fine so far.
-->
