![./README.media/TriceCheckOutput.gif](./README.media/TriceCheckOutput.gif)

# Trace with `TRICE` and get `printf` comfort inside interrupts and everywhere

<!-- vscode-markdown-toc -->
* 1. [Description](#Description)
* 2. [Abstract](#Abstract)
* 3. [How it works - the main idea](#Howitworks-themainidea)
* 4. [A brief history of *Trice*](#AbriefhistoryofTrice)
* 5. [*Trice* features](#Tricefeatures)
	* 5.1. [Small size](#Smallsize)
	* 5.2. [Execution speed](#Executionspeed)
	* 5.3. [Open source](#Opensource)
	* 5.4. [Similarity and differences to `printf` usage](#Similarityanddifferencestoprintfusage)
		* 5.4.1. [ 16-bit *Trice* **Id**s](#16-bitTriceIds)
		* 5.4.2. [Possible *trice* value bit width](#Possibletricevaluebitwidth)
		* 5.4.3. [Possible *trice* value count](#Possibletricevaluecount)
		* 5.4.4. [`float` and `double` values](#floatanddoublevalues)
		* 5.4.5. [Runtime generated strings transfer](#Runtimegeneratedstringstransfer)
		* 5.4.6. [Extended format specifier possibilities](#Extendedformatspecifierpossibilities)
	* 5.5. [Simplicity and Convinience](#SimplicityandConvinience)
	* 5.6. [Target configuration and options](#Targetconfigurationandoptions)
	* 5.7. [Embedded device timestamps](#Embeddeddevicetimestamps)
	* 5.8. [Runtime filterable colored channels](#Runtimefilterablecoloredchannels)
	* 5.9. [Compile time enable/disable `TRICE` on file level](#CompiletimeenabledisableTRICEonfilelevel)
	* 5.10. [Encryption option](#Encryptionoption)
	* 5.11. [Integrate several target devices in one log file](#Integrateseveraltargetdevicesinonelogfile)
	* 5.12. [Any byte capable 1-wire connection usable](#Anybytecapable1-wireconnectionusable)
	* 5.13. [Less FLASH memory needed compared to `printf` usage](#LessFLASHmemoryneededcomparedtoprintfusage)
	* 5.14. [COBS encoding and user protocols](#COBSencodinganduserprotocols)
* 6. [Future](#Future)
* 7. [Conclusion](#Conclusion)
* 8. [References and further reading](#Referencesandfurtherreading)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  1. <a name='Description'></a>Description

*Trice* is a software tracer-logger and consists of two parts:

* [x] **C** language macro `TRICE`, generating tiny code for getting real-time `printf` comfort at "speed-of-light" for any micro-controller.
  * [x] Example: `TRICE( Id(12345), "msg:%u words and %d numbers prining π as float %f\n", 7, 3, aFloat(3.14159) );`
* [x] Supporting PC tool **trice**, executable on all [Go](https://golang.org) platforms:
  * [ ] Android
  * [x] Linux
  * [ ] MacOS
  * [x] Windows
  * [ ] Interface options for log collectors are possible.


![./README.media/life0.gif](./README.media/life0.gif)

##  2. <a name='Abstract'></a>Abstract

If you develop software for an embedded system, you need some kind of system feedback. Debuggers are awesome tools, but when it comes to analyze dynamic behavior in the field, they are not usable.

Logging then, usually done with `printf` like functions, gets quick a result after having a `putchar()` implemented. This turns out to be an expensive way in terms of processor clocks and needed FLASH memory, when you regard the library code and all the strings needing FLASH memory space. For small micro-controllers that´s it.

Bigger micro-controllers are coming with embedded trace hardware. To use it, an expensive tool is needed. Useful for analyzing complex systems, but for in-field related issues at least unhandy.

<!-- Unhappy with this situation, the developer starts thinking of using digital pins or starts emitting some proprietary LED blinking codes or byte sequences, difficult to interpret. -->

The *trice* technique tries to fill this gap, trying to be minimal invasive for the target and as comfortable as possible. It is the result of a long-year dissatisfaction and several attempts to find a loophole to make embedded programming more fun and this way more effective.

##  3. <a name='Howitworks-themainidea'></a>How it works - the main idea

* Executing a `printf` like function means:
  * Copy format string from FLASH memory into RAM.
  * Parse the format string for format specifiers.
  * Parse variadic parameter list.
  * Convert parameters according the format specifiers into character sequences.
    * This includes several divisions - costly function calls.
  * Concatenate the parts to an output string and deliver it to the output, what often means copying again.
  * Never ever call a `printf` like function in time critical code, like an interrupt.
* `TRICE`, instead, just copies an ID together with the values to a buffer and is done. (A direct to the output option exists.)
* This can happen in 6-8 processor clocks. When running on a 64 MHz clock, light can travel about 30 meters in that time.
* To achieve that, a pre-compile step is needed, executing a `trice update` command.
  * The trice tool parses the the source tree for macros like `TRICE( "Hello World" );` and patches them to `TRICE( Id(nnnnn), "Hello World" );`, where `nnnnn` is a 16-bit identifier associated to the format string `"Hello World"`.
  * During compilation the `TRICE` macro is translated just to the `nnnnn` ID and the optional parameter values. The format string is ignored by the compiler.
  * At runtime just the ID with its values is delivered to the PC. There the **trice** tool receives it, gets the right format string from the reference list and performs the printing task:

---
  ![./README.media/triceCOBSBlockDiagram.svg](./README.media/triceCOBSBlockDiagram.svg)

* The **trice** tool tries to help as much as possible, to let the developer focus on its programming task. The once generated ID is not changed anymore without need. If for example the format string gets changed into `"Hello World!"`, a new ID is generated automatically and the reference list gets extended.
* Obsolete IDs are kept inside the reference list for compatibility with older firmware versions.
* It could be possible, when merging code, an **Id** is used twice for different format strings. In that case, the **Id** inside the reference list wins and the additional source gets patched with a new **Id**.
* The reference list should be kept under source code control.

##  4. <a name='AbriefhistoryofTrice'></a>A brief history of *Trice*

Developing firmware means to deal also with interrupts and often with timing. How do you check, if an interrupt occurred? Ok, increment a counter and display it in a background loop with some   `printf`. What about time measurement? Set a digital output to 1 and 0 and connect a measurement device. Once, developing software for a real-time image processing device, I had no clue where in detail the processing time exploded when the image quality got bad. A spare analog output with an oscilloscope synced with the video interrupt gave me the needed information after I set the analog output on several points in my algorithm. But, hey guys, I want to deal with my programming tasks and do not like all this hassle connecting wires and steer into instruments.

A `printf` is so cool on a PC, developing software there. But an embedded device often cannot use it for performance reasons. My very first attempt was writing the format string `.const` offset together with its values in a FIFO during a log statement and to do the `printf` it in the background. But that is compiler specific. Ok the full string address is better but needs more buffer space. [Zephyr](https://docs.zephyrproject.org/latest/reference/logging/index.html) for example does something like that calling it "deferred logging".

Than, one day I had the idea to compute format string checksums in a pre-compile step and to use them as ID in a list together with the format strings. That was a step forward but I had to write a supporting PC program. I did that in C++ in the assumption to get it better done that way. Finally it worked quite well but I hated my PC code, as I hate C++ now. And no good solution: Complicated to use! Also, what is, if 2 different format strings accidentally generate to the same short checksum? OK, I found a way, but nothing to be proud off. Also special IDs for message filtering are not possible.

The need became clear for ID management options. And there was [Go](https://golang.org) now, a fast language like **C**, portable, promising high programming efficiency and execution speed. I was keen to try it out on a real PC project. *Trice* grew, and as it got usable I decided to make it Open Source to say "Thank You" to the community this way.

Thinking of other people using *Trice* and some external ideas brought me to add features while keeping the target code as light as possible.

Learning that *Trice*  is also a [baby girl name](https://www.babynamespedia.com/meaning/Trice), my daughter designed the little girl with the pen symbolizing the `TRICE` macro for recording and the eyeglasses standing for the PC tool **trice**.

![./README.media/TriceGirlS.png](./README.media/TriceGirlS.png)

##  5. <a name='Tricefeatures'></a>*Trice* features
 
<!-- Even with the many options *Trice* is extrem lightwight, fast, flexible and easy to use.-->

* The various [ID management features](./IDManagement.md) allow the organization even also of bigger projects.
* Additional [format specifier support](./FormatSpecifier.md) gives options like binary or boolean output.
* The encryption opportunity makes it possible to test thoroughly a binary with log output and releasing it without the need to change any bit but to make the log output unreadable for a not authorized person.
* Easy re-syncing: On important point in the design was the question how to re-sync after some *trice* data stream interruption, because that happens often during firmware development. An escape sequence format and a flexible data format with more ID bits where working reliable but with [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) things got satisfying.
* Allowing parallel user protocols as well is a result using COBS packages starting with a package descriptor.
* Modularity: There was a learning **not** to reduce the transmit byte count to an absolute minimum, but to focus more on `TRICE` macro speed. Still it is possible to implement a minimal space encoding, if required, and control that with switches.
* Easy-to-use: Making it facile for a user to use *Trice* was the driving point just to have a `trice.c`, `trice.h` and a project specific simple to use `triceConfig.h` file on one side and to get away just with one macro `TRICE` for most situations.
* Transmitting runtime generated strings could be a need, so a `TRICE_S` macro exists supporting the `%s` format specifier for strings up to 1000 bytes long.
* It is possible to log float/double numbers using `%f` and the like, but the numbers need to be covered with `aFloat(x)` or `aDouble(y)`.
* Some asked for target timestamps - enable them with the time base you want. This adds a 32-bit value to the *trice* sequence.
* Target source code location options `file.c nnn` are usable. This adds a 32-bit value to the *trice* sequence.
* PC side ID display can be switched on: This costs no additional target code and allows later a more reliable `TRICE` macro location than target source code location, when source code evolved.

* Channels
* 
ID display
<!--
Features:
- super fast: a `TRICE` macro is executable in less than 10 clocks
- target and host timestamps
- runtime filterable colored channels
- encryption option
- enable/disable `TRICE` on file level 
- integrate several target devices in one log file
- transmit over TCP/IP to a remote server 
- any byte capable connection usable
- compared to `printf` usage, less needed FLASH memory
-->


###  5.1. <a name='Smallsize'></a>Small size

[small](https://github.com/rokath/trice/blob/master/docs/Space.md)

###  5.2. <a name='Executionspeed'></a>Execution speed 

![./README.media/MEASURE_executionClocks.PNG](./README.media/MEASURE_executionClocks.PNG)
* During `TRICE` macro runtime, 32-bit pushing into a double buffer half occurs:
  * a target timestamp, if enabled
  * a mandatory 2-byte ID value with 1-byte data size and optionally 1-byte cycle counter
  * parameter values, if existing
* The white-on-blue numbers in the image above are the processor clock counting backwards 6 steps on each `TRICE` macro execution in this example.
* The disassembly shows, that only 3 Assembler instructions per *trice* needed in this special case.

---

![./README.media/MEASURE_executionCode.PNG](./README.media/MEASURE_executionCode.PNG)

* Additionally disable interrupts and restore interrupt state and cycle counter increment can consume a few processor clocks.

###  5.3. <a name='Opensource'></a>Open source

* Target code and PC tool are open source.
* The MIT license gives full usage freedom.
* Any program can decode the *trice* messages. The **trice** tool with its `log` switch is a working example written in the open source language [*Go*](https://go.dev/).

###  5.4. <a name='Similarityanddifferencestoprintfusage'></a>Similarity and differences to `printf` usage

To get rid of the `printf` like functions disadvantages, a small toll needs to be paid. That is the **Id** occurring in the source code, its management and special handling for non-integer like values.

####  5.4.1. <a name='16-bitTriceIds'></a> 16-bit *Trice* **Id**s 

| Write              | After `trice update`          | Remark                                               |
|--------------------|-------------------------------|------------------------------------------------------|
| `TRICE( "Hi!\n");` | `TRICE( Id(12345), "Hi!\n"),` | Run `trice update` automatically in a prebuilt step. |

* The **Id** `123435` is a generated number specific to `"Hi!\n"`.
  * It is a so far unused number, according to rules.
* If you write `TRICE( "Hi!\n");` again on a 2nd location, it gets the same or a different **Id** - as you decide. Default is *different*.
* Per default new IDs determined randomly to keep the chance low, that several developers grab the same ID.
* It is possible to divide the ID space - each developer can gets it region.
* In a future **trice** tool it is possible to give each *trice* channel (see below) an **Id** range making it possible to implement *trice* channel specific runtime on/off.
* New IDs can be also incremental or decremental instead of random.
* `TRICE` macros commented out, are visible for the `trice update` command and therefore regarded.
* IDs get changed automatically to solve conflicts only.
* To make sure, a single ID will not be changed, change it to a hexadecimal syntax.
* The ID reference list keeps all obsolete IDs with their format strings allowing compatibility to former firmware versions.
* One can delete the ID reference list. It will be reconstructed automatically from the source tree with the next `trice update` command, but history is lost then.
* Optionally add the (compressed) ID reference list as resource into the target FLASH memory to be sure not to loose it in the next 20 years.  

####  5.4.2. <a name='Possibletricevaluebitwidth'></a>Possible *trice* value bit width

* The default parameter width behind the `TRICE` macro is 32 bit. It is changeable.
* Also one can use the macros `TRICE8`, `TRICE16`, `TRICE32`, `TRICE64` to define the bit width explicit.
  * This leads for the smaller bit-widths to a data packing and less needed space and bandwidth if this is critical.
####  5.4.3. <a name='Possibletricevaluecount'></a>Possible *trice* value count

* Up to 12 values are supported. For example one can write:
  * `TRICE( "%p | %04x %04x %04x %04x %04x %04x %04x %04x %04x | %f\n", p, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], aFloat(x));`
  * This is extendable to a total payload of 1008 bytes for each *trice*.
* Each macro can be prolonged with the used parameter count, for example `TRICE8_3` or `TRICE_2` to improve compile time checks.

####  5.4.4. <a name='floatanddoublevalues'></a>`float` and `double` values

* *float* values need to be covered by the `aFloat()` function and need a value bit width of 32, to secure correct data transfer.
* *double* values need to be covered by the `aDouble()` function and need need a value bit width of 64, to secure correct data transfer.

####  5.4.5. <a name='Runtimegeneratedstringstransfer'></a>Runtime generated strings transfer

* The `%s` format specifier is not directly supported by the `TRICE` macro.
* Strings, known at compile time should be a part of a format string.
* Strings created at runtime, need a special `TRICE_S` macro, which accepts exactly one type `%s` format specifier. They are allowed to a size of 1000 bytes each, if the configured *trice* buffer size matches.

####  5.4.6. <a name='Extendedformatspecifierpossibilities'></a>Extended format specifier possibilities

* Because the format string is interpreted by the **trice** tool written in [Go](https://en.wikipedia.org/wiki/Go_(programming_language)), the **Go** capabilities partial usable. For example `%b` allows output of bit patterns.

###  5.5. <a name='SimplicityandConvinience'></a>Simplicity and Convinience

* No compiler dependency, any not too old C-compiler should be usable.
* No hardware dependency, 8-bit to 64-bit & endianness is supported.
* Code instrumentation:
  * Add `trice.c` to the embedded project and `#include "trice.h"` to your source files.
  * Write for example `TRICE( "%d mV\n, voltage );` in a function.
  * Create an empty file named `til.json` in project root.
  * `trice update` there, will update the source to `TRICE( Id(12345), "%d mV\n, voltage );` and extend `til.json`. 
  * Use `trice log -port COMx -baud y` for logging.
* The logging does not need a restart, after changing the source again, it automatically updates the internal data.
* The **trice** tool comes with many command line switches for tailoring various needs, but usually these are not needed.

###  5.6. <a name='Targetconfigurationandoptions'></a>Target configuration and options

* Each project gets its own `triceConfig.h` file.
* Choose the *trice* mode here:
  * Direct mode: Straight output inside `TRICE` macro at the cost of the time it takes.
  * Indirect mode: Background output outside `TRICE` macro at the cost of RAM buffer needed.
* Set Options:
  * Target timestamps and their time base
  * Cycle counter
  * Allow `TRICE` usage inside interrupts
  * Buffer size

###  5.7. <a name='Embeddeddevicetimestamps'></a>Embedded device timestamps

* If enabled, any 32-bit target value can be used as target timestamp, so it is in the hand of the developer if the system clock, a 10µs or a millisecond second counter is used.
* Delivered target timestamps are displayed by the **trice** tool automatically, changeable or suppressible by command line switch.
* If several `TRICE` macros form a single line, the **trice** tool only displays the target timestamp of the first `TRICE` macro.

<!-- ###  5.7. <a name='PCtimestamps'></a>PC timestamps

* Embedded devices often lack a real-time clock and some scenarios can last for weeks.
* Therefore the **trice** tool precedes each *trice* line with a PC timestamp, if not disabled. This is the *trice* reception time on the PC, what can be some milliseconds later than the *trice* event.
* With a normal setting the *trice* double buffer switches every 100ms. This is no visible display delay.

###  5.8. <a name='Runtimefilterablecoloredchannels'></a>Runtime filterable colored channels

* Each *trice* format string can optionally start with a `pattern:` like in `TRICE( "msg:Hi!\n" );`.
* The **trice** tool, if knowing `msg:` as pattern, prepends the appropriate color code. It removes the sequence `pattern:`, if it is completely lower case.
* The user can define any pattern with any color code to create colored output with the **trice** tool.
* There is no channel enable switch inside the target code. It would need a back channel and add overhead.
  * A future extension could make that an option.
* The **trice** tool offers the 2 command line switches `-pick` and `-ban` to control channel visualization during runtime.  

![./README.media/COLOR_output.PNG](./README.media/COLOR_output.PNG)
-->
###  5.9. <a name='CompiletimeenabledisableTRICEonfilelevel'></a>Compile time enable/disable `TRICE` on file level 

* After debugging code in a file, there is no need to remove or comment out `TRICE` macros.
* `#define TRICE_OFF` just before `#include "trice.h"` and all `TRICE` macros in this file are ignored completely by the compiler.
* In case of re-constructing the **Id** reference list these no code generating macros are regarded.

<!--
###  5.10. <a name='Encryptionoption'></a>Encryption option

* *Trice* output can be encrypted, allowing to deliver devices with access to diagnostics only with a password.

###  5.11. <a name='Integrateseveraltargetdevicesinonelogfile'></a>Integrate several target devices in one log file

* Several **trice** tool instances can run parallel on one or different PCs.
* Each **trice** tool instance receives *trices* from one embedded device.
* Instead of displaying the log lines, the **trice** tools can transmit them over TCP/IP to a **trice** tool acting as display server.
* The display server can fold these log lines in one output.
* For each embedded device a separate *trice* line prefix and suffix is definable.
-->
###  5.12. <a name='Anybytecapable1-wireconnectionusable'></a>Any byte capable 1-wire connection usable

* The usual trice output device is an UART but also SEGGER-RTT is supported over J-Link or ST-Link devices.
* It is easy to use a micro controller as *trice* bridge to a serial port from any port.

###  5.13. <a name='LessFLASHmemoryneededcomparedtoprintfusage'></a>Less FLASH memory needed compared to `printf` usage

* Using *trice* frees FLASH memory from standard library `printf` code and also from all format strings.
* Of course the `TRICE` code needs also FLASH memory but only a fair amount and few bytes per macro. 

###  5.14. <a name='COBSencodinganduserprotocols'></a>COBS encoding and user protocols

* One issue on data transfer is always how to re-sync after a data disruption. 
* The [**C**onsistent **O**verhead **B**yte **S**tuffing](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) technique is a very powerful and simple way for re-syncing.
* Just in case, wait for the next package delimiter.
* Several *trice* message can occur within one single **COBS** package.
* Each **COBS** package starts with a 32-bit descriptor:

| Descriptor     | Meaning                                             |
|----------------|-----------------------------------------------------|
| 0x00000000     | Several *trice* messages without target timestamp   |
| 0x00000001     | Several *trice* messages with target timestamp      |
| 2...15         | Reserved
| 16...0xffffffff| User protocol data, the **trice** tool ignores them |

* This allows intermixing of several data streams with *trice* data.

##  6. <a name='Future'></a>Future

* The *trice* technique is new and still under development.
* Plans exist: 
  * Additional tests and bug fixing
  * Line number and source file support
  * A **trice** PC tool configuration file
  * Interfacing [Grafana](https://grafana.com/) or similar tools
  * A target code option using a FIFO instead of double buffering. This will slow down the *trices* a bit, but needs less RAM.
  * A target code option calling a trice function behind the `Trice` macro. This will safe FLASH memory in large projects but slows down the *trices* a bit.

##  7. <a name='Conclusion'></a>Conclusion

* Getting started with *trice* will take a few hours, but probably pay off during the further development.

##  8. <a name='Referencesandfurtherreading'></a>References and further reading

* [*Trice* on Github with examples](https://github.com/rokath/trice)
* [*Go* home](https://go.dev/)
