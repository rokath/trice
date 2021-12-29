# Trace & Log with `TRICE` and get `printf` comfort inside interrupts and everywhere

![./ref/TriceCheckOutput.gif](./ref/TriceCheckOutput.gif)

<!-- vscode-markdown-toc -->
* 1. [Description](#Description)
* 2. [Abstract](#Abstract)
* 3. [A brief history of *Trice*](#AbriefhistoryofTrice)
* 4. [How it works - the main idea](#Howitworks-themainidea)
* 5. [*Trice* features](#Tricefeatures)
	* 5.1. [Open source](#Opensource)
	* 5.2. [Easy-to-use](#Easy-to-use)
	* 5.3. [Small size - using *Trice* <u>frees</u> FLASH memory](#Smallsize-usingTriceufreesuFLASHmemory)
	* 5.4. [Execution speed](#Executionspeed)
	* 5.5. [Robustness](#Robustness)
	* 5.6. [More comfort than printf-like functions but small differences](#Morecomfortthanprintf-likefunctionsbutsmalldifferences)
	* 5.7. [Labeled channels, color and log levels](#Labeledchannelscolorandloglevels)
	* 5.8. [Compile time enable/disable `TRICE` macros on file level](#CompiletimeenabledisableTRICEmacrosonfilelevel)
	* 5.9. [Target and host timestamps](#Targetandhosttimestamps)
	* 5.10. [Target source code location](#Targetsourcecodelocation)
	* 5.11. [ Several target devices in one log output](#Severaltargetdevicesinonelogoutput)
	* 5.12. [Any byte capable 1-wire connection usable](#Anybytecapable1-wireconnectionusable)
	* 5.13. [Scalability](#Scalability)
	* 5.14. [Portability and Modularity](#PortabilityandModularity)
	* 5.15. [Optional *Trice* messages encryption](#OptionalTricemessagesencryption)
* 6. [ Bottom line](#Bottomline)
* 7. [References and further reading](#Referencesandfurtherreading)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  1. <a name='Description'></a>Description

*Trice* is a unusual software tracer-logger and consists of these parts to use:

- [x] [trice.c](../pkg/src/trice.c) containing the [less that 1KB](./TriceSpace.md) runtime code using [triceConfig.h](../test/MDK-ARM_STM32G071RB/Core/Inc/triceConfig.h) as setup.
- [x] [trice.h](../pkg/src/trice.h) containing a **C** language macro `TRICE`, generating [tiny code](./TriceSpeed.md) for getting real-time `printf` comfort at "speed-of-light" for any micro-controller.
  * [x] Example: `float x = 3.14159265/4; TRICE( Id(12345), "info:π/4 is %f with the bit pattern %032b\n", aFloat(x), aFloat(x) );`
- [x] PC tool **trice**, executable on all [Go](https://golang.org) platforms:
  * [ ] Android
  * [x] Linux
  * [ ] MacOS
  * [x] Windows
  * [ ] Interface options for log collectors are possible.


![./ref/life0.gif](./ref/life0.gif)

##  2. <a name='Abstract'></a>Abstract

If you develop software for an embedded system, you need some kind of system feedback. Debuggers are awesome tools, but when it comes to analyze dynamic behavior in the field, they are not usable.

Logging then, usually done with printf-like functions, gets quick a result after having i.e. `putchar()` implemented. This turns out to be an expensive way in terms of processor clocks and needed FLASH memory, when you regard the library code and all the strings needing FLASH memory space. For small micro-controllers that´s it.

Bigger micro-controllers are coming with embedded trace hardware. To use it, an expensive tool is needed. Useful for analyzing complex systems, but for in-field related issues at least unhandy.

<!-- Unhappy with this situation, the developer starts thinking of using digital pins or starts emitting some proprietary LED blinking codes or byte sequences, difficult to interpret. -->

The *Trice* technique tries to fill this gap, being be minimal invasive for the target and as comfortable as possible. It is the result of a long-year dissatisfaction and several attempts to find a loophole to make embedded programming more fun and this way more effective.

##  3. <a name='AbriefhistoryofTrice'></a>A brief history of *Trice*

Developing firmware means to deal also with interrupts and often with timing. How do you check, if an interrupt occurred? Ok, increment a counter and display it in a background loop with some printf-like function. What about time measurement? Set a digital output to 1 and 0 and connect a measurement device. Once, developing software for a real-time image processing device, I had no clue where in detail the processing time exploded when the image quality got bad. A spare analog output with an oscilloscope synced with the video interrupt gave me the needed information, after I set the analog output on several points in my algorithm. But, hey guys, I want to deal with my programming tasks and do not like all this hassle connecting wires and steer into instruments.

A `printf` is so cool on a PC, developing software there. But an embedded device often cannot use it for performance reasons. My very first attempt was writing the format string `.const` offset together with its values in a FIFO during a log statement and to do the `printf` it in the background. But that is compiler specific. Ok the full string address is better but needs more buffer space. [Zephyr](https://docs.zephyrproject.org/latest/reference/logging/index.html) for example does something like that calling it "deferred logging".

Than, one day I had the idea to compute short checksums for the format strings in a pre-compile step and to use them as ID in a list together with the format strings. That was a step forward but needed to write a supporting PC program. I did that in C++ in the assumption to get it better done that way. Finally it worked but I hated my PC code, as I dislike C++ now because of all its nuts and bolts to handle, accompanied by missing libraries on the next PC. The tool usability was also unhandy and therefore error prone and the need became clear for a full automatized solution. Also, what is, if 2 different format strings accidentally generate the same short checksum? There was a way around, but an ID based message filtering will never be possible that way.

The need became clear for controllable IDs and management options. And there was [Go](https://golang.org) now, an as-fast-as-**C** language, easy to learn, promising high programming efficiency and portability. It would be interesting to try it out on a real PC project.

Trying to add channels in form of partial *TRICE* macro names was blowing up the header code amount and was a too rigid design. Which are the right channels? One lucky day I came to the conclusion to handle channels just as format string parts like `"debug:Here we are!\n"` and getting rid of them in the target code this way also giving the user [full freedom](../internal/emitter/lineTransformerANSI.go) to invent any channels.

An other point in the design was the question how to re-sync after data stream interruption, because that happens often during firmware development. A tryout proprietary escape sequence format and an alternative flexible data format with more ID bits where working reliable but with [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) things got satisfying. A side result of that trials is the **trice** tool option to use different decoders if needed.

There was a learning **not** to reduce the transmit byte count to an absolute minimum, but to focus more on `TRICE` macro speed and universality. That led to a double buffer on the target side discarding the previous FIFO solution and the package descriptor allowing alongside user protocols.

*Trice* grew, and as it got usable I decided to make it Open Source to say "Thank You" to the community this way.

Thinking of other people using *Trice* and some external ideas brought me to add features like float and double number support, target timestamps and source code location while keeping the target code as light as possible.

Learning that *Trice*  is also a [baby girl name](https://www.babynamespedia.com/meaning/Trice), my daughter Ida designed the little girl with the pen symbolizing the `TRICE` macro for recording and the eyeglasses standing for the PC tool **trice** visualizing the logs.

![./ref/TriceGirlS.png](./ref/TriceGirlS.png)

##  4. <a name='Howitworks-themainidea'></a>How it works - the main idea

*Trice* performs **no** [costly](./TriceVsPrintfSimilaritiesAndDifferences.md#printf-like-functions) printf-like functions on the target at all. The `TRICE` macro, instead, just copies an ID together with the optional values to a buffer and is done. In the minimum case this can happen in [6-8](./TriceSpeed.md) processor clocks even with target timestamps included. When running on a 64 MHz clock, **light can travel about 30 meters in that time**.

To achieve that, an automatable pre-compile step is needed, executing a `trice update` command on the PC. The **trice** tool parses then the source tree for macros like `TRICE( "msg: %d Kelvin\n", k );` and patches them to `TRICE( Id(12345), "msg: %d Kelvin\n", k );`, where `12345` is a generated 16-bit identifier copied into a [**T**rice **I**D **L**ist](../til.json). During compilation than, the `TRICE` macro is translated to the `12345` ID only, and the optional parameter values. The format string is ignored by the compiler.

The target code is [project specific](../test/MDK-ARM_STM32G071RB/Core/Inc/triceConfig.h) configurable.  In **immediate mode** the the stack is used as *Trice* buffer and the TRICE macro execution includes the quick [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) encoding and the data transfer. This more straightforward and slower architecture can be interesting for many cases because it is anyway much faster than printf-like functions calls. In **deferred mode** a service swaps the *Trice* double buffer periodically, the COBS encoding takes part and with the filled buffer the background transfer is triggered. Out buffer and half *Trice* buffer share the same memory for efficiency.

During runtime the PC **trice** tool receives all what happened in the last ~100ms as a COBS package from the UART port. The `0x30 0x39` is the ID 12345 and a map lookup delivers the format string *"msg: %d Kelvin\n"* and also the bit width information. Now the **trice** tool can write target timestamp, set msg color and execute `printf("%d Kelvin\n", 0x0000000e);`

---
  ![./ref/triceCOBSBlockDiagram.svg](./ref/triceCOBSBlockDiagram.svg)

The **trice** tool is a background helper giving the developer focus on its programming task. The once generated ID is not changed anymore without need. If for example the format string gets changed into `"msg: %d Kelvin!\n"`, a new ID is inserted automatically and the reference list gets extended. Obsolete IDs are kept inside the [**T**rice **I**D **L**ist](../til.json) for compatibility with older firmware versions. It could be possible, when merging code, an **ID** is used twice for different format strings. In that case, the **ID** inside the reference list wins and the additional source gets patched with a new **ID**. This maybe unwanted patching is avoidable with proper [ID management](./TriceIDManagement.md). The reference list should be kept under source code control.

##  5. <a name='Tricefeatures'></a>*Trice* features

###  5.1. <a name='Opensource'></a>Open source

Target code and PC tool are open source. The MIT license gives full usage freedom. Users are invited to support the further *Trice* development.

###  5.2. <a name='Easy-to-use'></a>Easy-to-use

Making it [facile](./TriceUsageGuide.md) for a user to use *Trice* was the driving point just to have one **trice** tool and an additional source file with a project specific simple to use `triceConfig.h` and to get away with the one macro `TRICE` for most situations. *Trice* understands itself as a silent helper in the background to give the developer more focus on its real task. If, for example, `trice log` is running and you re-flash the target, there is no need to restart the **trice** tool. When [til.json](../til.json) was updated in an pre-build step, the **trice** tool automatically reloads the new data.

The **trice** tool comes with many command line switches (`trice help -all`) for tailoring various needs, but mostly these are not needed. Usually only type `trice l -p COMn` for logging with a 115200 bit/s baud rate.

###  5.3. <a name='Smallsize-usingTriceufreesuFLASHmemory'></a>Small size - using *Trice* <u>frees</u> FLASH memory

Compared to a printf-library code which occupies [1](https://github.com/mludvig/mini-printf) to over [20](https://github.com/mpaland/printf#a-printf--sprintf-implementation-for-embedded-systems) KB FLASH memory, the *Trice* code is really [small](./TriceSpace.md) - less 1 KB will do already but provide full support.

###  5.4. <a name='Executionspeed'></a>Execution speed

Can it get faster that [that](./TriceSpeed.md)? Only 3 runtime Assembler instructions per *Trice* needed in the minimum case! Additionally target timestamp and location, disable interrupts and restore interrupt state and cycle counter increment can consume a few more processor clocks, if enabled, but a *Trice* is still incomparable fast.

###  5.5. <a name='Robustness'></a>Robustness

When a *Trice* data stream is interrupted, the [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) encoding allows an immediate re-sync with the next COBS package delimiter byte and a default *Trice* cycle counter gives a high chance to detect lost *Trice* messages.

###  5.6. <a name='Morecomfortthanprintf-likefunctionsbutsmalldifferences'></a>More comfort than printf-like functions but small differences

*Trice* is usable also inside interrupts and additional [format specifier support](./TriceVsPrintfSimilaritiesAndDifferences.md#Extended-format-specifier-possibilities) gives options like binary or bool output. Transmitting runtime generated strings could be a need, so a `TRICE_S` macro exists supporting the `%s` format specifier for strings up to 1000 bytes long. It is possible to log float/double numbers using `%f` and the like, but the numbers need to be covered with the function `aFloat(x)` or `aDouble(y)`.

###  5.7. <a name='Labeledchannelscolorandloglevels'></a>Labeled channels, color and log levels

You can label each *Trice* with a channel specifier to [colorize](./TriceColor.md) the output. This is free of any runtime costs because the channel specifiers are part of the log format strings, which never get to the target.

Many logger use so called log levels and offer a log level setting like "log all above **INFO**" for example. The *Trice* channels can cover that but they can do even better: First you can [define](../internal/emitter/lineTransformerANSI.go) any "log levels" you need and use them as *Trice* channels. Further you can assign each of these "log levels" an ID range and than use a level mechanism on the target side to decide which IDs are transmitted. I see no need to implement that right now, because the runtime and bandwidth costs are so small for each *Trice* and the **trice** tool is much better in selecting what to display (`-pick` and `-ban` switches) and no back channel is needed. BTW: Full lowercase channel strings the **trice** tool will strip from the format string after setting the appropriate color.

###  5.8. <a name='CompiletimeenabledisableTRICEmacrosonfilelevel'></a>Compile time enable/disable `TRICE` macros on file level 

After debugging code in a file, there is [no need to remove or comment out `TRICE` macros](./TriceOnOff.md). Write a `#define TRICE_OFF` just before the `#include "trice.h"` line and all `TRICE` macros in this file are ignored completely by the compiler, but not by the **trice** tool. In case of re-constructing the [**T**rice **ID** **L**ist](../til.json) these no code generating macros are regarded.

###  5.9. <a name='Targetandhosttimestamps'></a>Target and host timestamps 

Enable target timestamps with a variable you want inside [triceConfig.h](../test/MDK-ARM_STM32G071RB/Core/Inc/triceConfig.h). This adds a 32-bit value to each *Trice* sequence, which carries than the system clock, a millisecond second or an other event counter. The **trice** tool will automatically recognize and display them in a default mode you can control. If several `TRICE` macros form a single line, the **trice** tool only displays the target timestamp of the first `TRICE` macro.

Embedded devices often lack a real-time clock and some scenarios can last for weeks. Therefore the **trice** tool precedes each *Trice* line with a PC timestamp, if not disabled. This is the *Trice* reception time on the PC, what can be some milliseconds later than the target *Trice* event.

###  5.10. <a name='Targetsourcecodelocation'></a>Target source code location 

Some developers like to see the `filename.c` and `line` in front of each log line for quick source location. Enable that inside [triceConfig.h](../test/MDK-ARM_STM32G071RB/Core/Inc/triceConfig.h). This adds a 32-bit value to the *Trice* sequence containing a 16-bit file ID and a 16-bit line number. The file ID is generated automatically by inserting `#define TRICE_FILE Id(nnnnn)` in each source.c file containing a `#include "trice.h"` line. 

Because software is a matter of change it could happen you get obsolete information this way. Therefore the **trice** tool log option `-showID` exists to display the *Trice* ID in front of each log line what gives a more reliable way for event localization in some cases. Also you can get it for free, because no target code is needed for that. 

###  5.11. <a name='Severaltargetdevicesinonelogoutput'></a> Several target devices in one log output

Several **trice** tool instances can run parallel on one or different PCs. Each **trice** tool instance receives *Trices* from one embedded device. Instead of displaying the log lines, the **trice** tool instances can transmit them over TCP/IP (`trice l -p COMx -ds`) to a **trice** tool instance acting as display server (`trice ds`). The display server can fold these log lines in one output. For each embedded device a separate *Trice* line prefix and suffix is definable. This allows comparable time measurements in distributed systems. BTW: The *Trice* message integration could be done also directly with the COBS packages.

###  5.12. <a name='Anybytecapable1-wireconnectionusable'></a>Any byte capable 1-wire connection usable

The usual *Trice* output device is an [UART](./https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter) but also [SEGGER-RTT](./TriceOverRTT.md) is supported over J-Link or ST-Link devices. Many micro controller boards can act as *Trice* bridge to a serial port from any port ([example](./TriceOverOneWire.md)).

###  5.13. <a name='Scalability'></a>Scalability

The various [*Trice* ID management features](./TriceIDManagement.md) allow the organization also of bigger software systems. More than 65000 possible different IDs should match also large projects. Just in case: 16-bit for the ID is a not to hard changeable value.

###  5.14. <a name='PortabilityandModularity'></a>Portability and Modularity

The **trice** tool is written in the open source language [*Go*](https://go.dev/) and is therefore usable on many platforms. That means the automatic code patching and ID handling side with `trice update`.

All C-compilers should be usable to compile the target *Trice* code and there is no hardware dependency despite the byte transmission. MCUs with 8-bit to 64-bit, little or big endian are supported.

Any user program able to read a [JSON](../til.json) file, can receive the [documented](./TriceMessagesEncoding.md) *Trice* message format, look-up the ID and perform a printf-like action to translate into log strings. The **trice** tool with its `log` switch is a working example.

Using [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) packages starting with a [package descriptor](./TriceMessagesEncoding.md#package-mode-prefix) allows alongside user protocols. The other way around is also implementable: In a user protocol embedded `Trice` messages.

The **trice** tool is expandable with several decoders. So it is possible to implement a minimal *Trice* encoding, if bandwidth matters heavily and control that with switches.

When less RAM usage is more important the target double buffer is replaceable with a FIFO. So the user will be able to decide at compile time about that. Right now already a immediate mode is selectable inside [triceConfig.h](../test/MDK-ARM_STM32G071RB/Core/Inc/triceConfig.h) avoiding any buffer by paying a time toll.

The **trice** tool supports [many command line switches](./TriceCommandLineExamples.md).

###  5.15. <a name='OptionalTricemessagesencryption'></a>Optional *Trice* messages encryption

The encryption opportunity makes it possible to test thoroughly a binary with log output and releasing it without the need to change any bit but to make the log output unreadable for a not authorized person. Implemented is the lightweight [XTEA](https://de.wikipedia.org/wiki/Extended_Tiny_Encryption_Algorithm) as option, what will do for many cases. It should be no big deal to add a different algorithm.

##  6. <a name='Bottomline'></a> Bottom line

The *Trice* technique is new and still under development. Additional tests and bug fixing is necessary. A **trice** tool [configuration file](./TriceConfiguration.md#host-configuration-file) and interfacing [Grafana](https://grafana.com/) or similar tools would be possible extensions. Getting started with *Trice* will take a few hours, but probably pay off during the further development.

##  7. <a name='Referencesandfurtherreading'></a>A few maybe interesting links

* [https://mcuoneclipse.com/2016/10/17/tutorial-using-single-wire-output-swo-with-arm-cortex-m-and-eclipse/](https://mcuoneclipse.com/2016/10/17/tutorial-using-single-wire-output-swo-with-arm-cortex-m-and-eclipse/)
* [https://mcuoneclipse.com/2016/11/05/tutorial-getting-etm-instruction-trace-with-nxp-kinetis-arm-cortex-m4f/](https://mcuoneclipse.com/2016/11/05/tutorial-getting-etm-instruction-trace-with-nxp-kinetis-arm-cortex-m4f/)
* [https://interrupt.memfault.com/blog/a-deep-dive-into-arm-cortex-m-debug-interfaces](https://interrupt.memfault.com/blog/a-deep-dive-into-arm-cortex-m-debug-interfaces)
* [https://interrupt.memfault.com/blog/instruction-tracing-mtb-m33](https://interrupt.memfault.com/blog/instruction-tracing-mtb-m33)
* [python script is used to parse ITM trace packets from the SWO pin on the STM32 using OpenOCD](https://github.com/robertlong13/SWO-Parser)
* [NanoLog - extremely performant nanosecond scale logging system](https://github.com/PlatformLab/NanoLog)
* [baical - high-performance tools set for sending and receiving traces](http://baical.net/index.html)
* [MCUXpresso IDE Instruction Trace](https://www.nxp.com/docs/en/quick-reference-guide/MCUXpresso_IDE_Instruction_Trace.pdf)
* [Arm CoreSight technology which introduces powerful new debug and trace capabilities](https://www2.keil.com/coresight)
* [*Trice* on Github with examples](https://github.com/rokath/trice)
* [*Go* home](https://go.dev/)
