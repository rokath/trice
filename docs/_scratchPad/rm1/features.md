# Trice Features

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. No Dynamic Memory Management needed](#no-dynamic-memory-management-needed)
* [2. Open source](#open-source)
* [3. Easy-to-use](#easy-to-use)
* [4. Small size - using Trice frees FLASH memory](#small-size---using-trice-frees-flash-memory)
* [5. Execution speed](#execution-speed)
* [6. Robustness](#robustness)
* [7. Minimal Transfer Bytes Amount](#minimal-transfer-bytes-amount)
* [8. More comfort than printf-like functions but small differences](#more-comfort-than-printf-like-functions-but-small-differences)
* [9. Tags, Color and Log Levels](#tags-color-and-log-levels)
* [10. Compile Time Enable/Disable Trice Macros on File or Project Level](#compile-time-enabledisable-trice-macros-on-file-or-project-level)
* [11. Target and host timestamps](#target-and-host-timestamps)
* [12. Target source code location](#target-source-code-location)
* [13. Several target devices in one log output](#several-target-devices-in-one-log-output)
* [14. Any byte-capable 1-wire connection usable](#any-byte-capable-1-wire-connection-usable)
* [15. Scalability](#scalability)
* [16. Portability and Modularity](#portability-and-modularity)
* [17. Optional Trice messages encryption](#optional-trice-messages-encryption)
* [18. Trice Protection](#trice-protection)
* [19. Trice Diagnostics](#trice-diagnostics)
* [20. Trice Cache](#trice-cache)
* [21. Avoiding False-Positive Editor Warnings](#avoiding-false-positive-editor-warnings)
* [22. Trice Generator](#trice-generator)
* [23. Versions and Variants Trice Stability](#versions-and-variants-trice-stability)
* [24. Legacy Project Code Integration](#legacy-project-code-integration)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## 1. <a id="no-dynamic-memory-management-needed"></a>No Dynamic Memory Management needed

Trice's target logging path needs no heap allocator: it does not call `malloc`, `calloc`, `realloc` or `free`. Buffer sizes are selected in the target configuration. Depending on the chosen mode, storage includes static buffers and automatic local buffers on the stack; a stack buffer is not a static object. RAM requirements therefore depend on the configuration, record sizes and buffering strategy; see [Trice memory needs](memory_usage.md).

This statement covers Trice's own target logging code, including its CE adapters. It does not cover the Go host tool, an RTOS, a transport callback, an application's local-formatting sink, or a function invoked by a CE expression. For example, a user-supplied `read_context()` that allocates memory still allocates when called through `-ce`. Choose non-allocating application functions when the complete log call must avoid the heap.

## 2. <a id="open-source"></a>Open source

Target code and PC tool are open source. The MIT license gives full usage freedom. Users are invited to support the further Trice development.

## 3. <a id="easy-to-use"></a>Easy-to-use

Making it facile for a user to use Trice was the driving point just to have

* one Trice tool
* one additional [target code](../../src/) source folder
* a project specific simple to use [triceConfig.h](../../examples/F030_inst/Core/Inc/triceConfig.h)
* and to get away with the one macro `trice` for most situations.

Trice understands itself as a silent helper in the background to give the developer more focus on its real task. If, for example, `trice log` is running and you re-flash the target, there is ***no need to restart*** the Trice tool just to load changed decoding tables. When [til.json](../../demoTIL.json) is updated in a pre-build step, the Trice tool automatically reloads it during logging. The location table selected with `-li` is also reloaded if it was present when logging started.

For example, keep `trice log -i til.json -li li.json` running while you rebuild with `trice bind` or `trice insert`. New IDs, changed structured field names, and updated source locations become available to subsequent records in text, JSON, and KV output. Both ordinary file writes and replacement by a newly generated file are supported. Each save is read after a short settling interval (normally about 100 ms), including saves made shortly after one another. Active visualization rules check new or changed records against their existing rules; a rule already disabled because of an incompatible format stays disabled until the logger is restarted.

If a file is temporarily missing, empty, or invalid JSON while being saved, the logger keeps that table's last valid contents, reports a warning on stderr, and retries automatically. The two files reload independently; updating them is not a single atomic transaction. A valid JSON object replaces the respective in-memory table completely, including removal of keys no longer in the file. `{}` intentionally clears a table. Successful reloads are silent unless `-v` is enabled; reload diagnostics never enter the application logfile or JSON/KV output.

Use `-li off` to disable location information. If no location file existed at startup, create it before starting the logger to enable its automatic reload. File watching requires an available local directory; if setup fails, the logger warns which path cannot be watched and continues using the loaded data. Restart it after resolving that problem. Reloading dictionaries does not repair a disconnected RTT server or other transport connection, and location information still needs to match the firmware actually running on the target.

The Trice tool comes with many command line switches (`trice help -all`) for tailoring various needs, but mostly these are not needed. <small>The generated file [ref/trice-help-all.txt](../ref/trice-help-all.txt) contains this information as well.</small>

Normal Trice tool usage is:
* [./build.sh](../../examples/L432_inst/build.sh) containing `trice insert -cache`, `make` and `trice clean -cache`
* **`make log`** containing `trice l -p COMn` for logging with default baud rate.

In this example, the user code gets **not** polluted with Trice IDs - they exists only during the compilation step and the Trice cache makes this invisible for the user and the build system.

## 4. <a id="small-size---using-trice-frees-flash-memory"></a>Small size - using Trice frees FLASH memory

Compared to a printf-library code which occupies [1](https://github.com/mludvig/mini-printf) to over [20](https://github.com/mpaland/printf#a-printf--sprintf-implementation-for-embedded-systems) KB FLASH memory, the Trice code is normally [smaller](memory_usage.md) but provides full support.

## 5. <a id="execution-speed"></a>Execution speed

Can it get faster than [6 clocks only](performance.md)? Only 3 runtime Assembler instructions per Trice needed in the minimum case! Optional target timestamp, critical sections, cycle counter, diagnostics and overflow protection can consume a few more processor clocks, if enabled, but a Trice is still incomparable fast.

## 6. <a id="robustness"></a>Robustness

When a Trice data stream is interrupted, the optional [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) or [TCOBS](https://github.com/rokath/tcobs) encoding allows an immediate re-sync with the next COBS/TCOBS package delimiter byte and a default Trice **cycle counter** gives a high chance to detect lost Trice messages. <small>See also [Versions and Variants Trice Stability](#versions-and-variants-trice-stability).</small>

## 7. <a id="minimal-transfer-bytes-amount"></a>Minimal Transfer Bytes Amount

A Trice message is 4 bytes long (2 ID bytes and 2 count bytes) plus optional time stamps and/or values. In conjunction with the compressing [TCOBS](https://github.com/rokath/tcobs) framing the Trice data stream is as small as possible. Use the `-debug` switch to see the compressed and framed packages alongside the decompressed ones together with the decoded messages.

To see the encoding for each single message `#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE` inside the project specific _triceConfig.h_.

Without `-debug` CLI switch:

```bash
ms@MacBook-Pro G0B1_inst % trice log -p /dev/tty.usbmodem0007722641261 -prefix off -li off -hs off -ts off
...
This is a message without values and without stamp.
...
```

With `-debug` CLI switch:

```bash
ms@MacBook-Pro G0B1_inst % trice log -p /dev/tty.usbmodem0007722641261 -prefix off -li off -hs off -ts off -debug
...
TCOBSv1: c1 74 e2 23 00 
->TRICE: c1 74 e2 00 
This is a message without values and without stamp.
...
```

The TCOBS encoding cannot compress in the example above, because the data are too small, but here is a significant compression result shown:

```bash
ms@MacBook-Pro G0B1_inst % trice log -p /dev/tty.usbmodem0007722641261 -prefix off -hs off -debug
...
TCOBSv1: b8 76 7b 18 84 fe e1 fd e1 fc e1 fb e1 fa e1 00 
->TRICE: b8 76 7b 18 ff ff ff ff fe ff ff ff fd ff ff ff fc ff ff ff fb ff ff ff fa ff ff ff 
_test/testdata/triceCheck.c   805              value=-1, -2, -3, -4, -5, -6
...
```

* The `TRICE_SINGLE_PACK_MODE` inserts after each Trice a package delimiter `0`. 
* The `TRICE_MULTI_PACK_MODE` inserts after a group of Trice messages a package delimiter `0`, what minimizes the transmit data amount.

When encryption is active, a compression makes no sense, but the `TRICE_MULTI_PACK_MODE` can help to reduce the total amount of padding bytes, because each encrypted package must have a multiple of 8 as length.

```bash
ms@MacBook-Pro G0B1_inst % trice log -p /dev/tty.usbmodem0007722641261 -prefix off -hs off -pw MySecret -pf cobs -debug    
...
cobs: 21 84 b7 60 8b 21 89 1e e3 07 6d dc d9 2d 6f 59 04 8e 50 8f 24 1c a2 63 2e 3d 4a 57 ef 39 63 01 cb 00 
->TRICE: 84 b7 60 8b 21 89 1e e3 07 6d dc d9 2d 6f 59 04 8e 50 8f 24 1c a2 63 2e 3d 4a 57 ef 39 63 01 cb 
-> DEC:  cc b6 63 01 71 02 ff fe cd 76 72 03 ff fe fd ce f6 64 81 00 00 73 04 ff fe fd fc 00 00 00 00 00 
_test/testdata/triceCheck.c   827        0_355 value=-1, -2
_test/testdata/triceCheck.c   828              value=-1, -2, -3
_test/testdata/triceCheck.c   829    0,033_124 value=-1, -2, -3, -4
cobs: 19 50 70 79 d7 75 6f d7 99 dc d8 ec 06 e1 66 e7 a7 c1 0d 96 85 df 19 25 55 00 
->TRICE: 50 70 79 d7 75 6f d7 99 dc d8 ec 06 e1 66 e7 a7 c1 0d 96 85 df 19 25 55 
-> DEC:  cf b6 64 01 74 05 ff fe fd fc fb d0 76 75 06 ff fe fd fc fb fa 00 00 00 
_test/testdata/triceCheck.c   830        0_356 value=-1, -2, -3, -4, -5
_test/testdata/triceCheck.c   831              value=-1, -2, -3, -4, -5, -6
...
```

## 8. <a id="more-comfort-than-printf-like-functions-but-small-differences"></a>More comfort than printf-like functions but small differences

Trice is usable also inside interrupts and [extended format specifier possibilities](printf_comparison.md#extended-format-specifier-possibilities) give options like binary or bool output. Transmitting runtime generated strings could be a need, so a `triceS` macro exists supporting the `%s` format specifier for strings up to 32737 bytes long. It is possible to log float/double numbers using `%f` and its relatives, but the numbers need to be covered with the fast converter function `aFloat(x)` or `aDouble(y)`. Also UTF-8 encoded strings are implicitly supported, if you use UTF-8 for the source code. See chapter [Trice Similarities and differences to printf usage](printf_comparison.md) for more details.

![./ref/UTF-8Example.PNG](../ref/UTF-8Example.PNG)

## 9. <a id="tags-color-and-log-levels"></a>Tags, Color and Log Levels

You can label each Trice with a tag specifier to [colorize](tags_color_and_weights.md) the output. This is free of any runtime costs because the tags are part of the Trice log format strings, which are not compiled into the target. The Trice tool will strip full lowercase tag descriptors from the format string after setting the appropriate color, making it possible to give each message its color.

Loggers use log levels and offer a setting like "log all above **INFO**" for example. The Trice tags can cover that but can do better: Inside package _emitter.ColorChannels_ in a single file [./internal/emitter/lineTransformerANSI.go](../../internal/emitter/lineTransformerANSI.go) all common log levels defined as Trice tags alongside with user tags. The user can adjust this. The Trice tool has the `-pick` and `-ban` switches to control the display in detail. Also a `-logLevel` switch is usable to determine a display threshold as tag position inside ColorChannels.

If an inside-target log selection is needed (routing), the Trice tool can assign each log tag a separate ID range and a target side ID based log selector can control which IDs are transmitted over which output channel. See chapter [Trice ID management](id_management.md) or type `trice help -insert` and look for `-IDRange`.

![./ref/COLOR_output.PNG](../ref/COLOR_output.PNG)

## 10. <a id="compile-time-enabledisable-trice-macros-on-file-or-project-level"></a>Compile Time Enable/Disable Trice Macros on File or Project Level

After debugging code in a file, there is [no need to remove or comment out Trice macros](switching_trice_on_and_off.md). Write a `#define TRICE_OFF 1` just before the `#include "trice.h"` line and all Trice macros in this file are ignored completely by the compiler, but not by the Trice tool. In case of reconstructing the [**T**rice **ID** **L**ist](../../demoTIL.json), these no code generating macros are regarded.

```C
#define TRICE_OFF 1 // Disable trice code generation for this file object.
#include "trice.h"
```

When you wish to build a firmware without any Trice code, it is sufficient to add

```make
C_DEFS += -DTRICE_OFF=1 // Define TRICE_OFF=1 for the whole project.
```

or similar to your Makefile.

## 11. <a id="target-and-host-timestamps"></a>Target and host timestamps

For each Trice you can have (time) stamps or not:

* `trice( "...", ...);` or `TRICE( id(0), ( "...", ...)`: no stamp:
* `Trice( "...", ...);` or `TRICE( Id(0), ( "...", ...)`: 16-bit stamp:
* `TRice( "...", ...);` or `TRICE( ID(0), ( "...", ...)`: 32-bit stamp:

The optional 16- or 32-bit value then carries the system clock, a millisecond counter, or another event counter configured in the project specific [triceConfig.h](../../examples/G0B1_inst/Core/Inc/triceConfig.h). The Trice tool will automatically recognize and display the stamps in a mode you can control. If several Trice macros form a single line, the Trice tool only displays the target timestamp of the first Trice macro.

Embedded devices often lack a real-time clock and some scenarios can last for weeks. Therefore the Trice tool precedes each Trice line with a PC timestamp, if not disabled. This is the Trice reception time on the PC, which can be some milliseconds later than the target Trice event.

## 12. <a id="target-source-code-location"></a>Target source code location

Some developers like to see the `filename.c` and `line` in front of each log line for quick source location. During `trice i` a file `li.json` is generated containing the location information. If `trice log` finds this file, filename and line number are displayed in front of each log line, otherwise not.

Because software is a matter of change it could happen you get obsolete information this way. Therefore the Trice tool log option `-showID` exists to display the Trice ID in front of each log line what gives a more reliable way for event localization in some cases. Also you can get it for free, because no target code is needed for that.

## 13. <a id="several-target-devices-in-one-log-output"></a>Several target devices in one log output

Several Trice tool instances can run in parallel on one or different PCs. Each Trice tool instance receives *Trices* from one embedded device. Instead of displaying the log lines, the Trice tool instances can transmit them over TCP/IP (`trice l -p COMx -ds`) to a Trice tool instance acting as display server (`trice ds`). The display server can fold these log lines in one output. For each embedded device a separate Trice line prefix and suffix is definable. This allows comparable time measurements in distributed systems.

## 14. <a id="any-byte-capable-1-wire-connection-usable"></a>Any byte-capable 1-wire connection usable

The usual Trice output device is an UART but also [SEGGER-RTT](rtt.md) is supported over J-Link or ST-Link devices. Many microcontroller boards can act as Trice bridge to a serial port from any port ([Trice without UART](without_uart.md)).

## 15. <a id="scalability"></a>Scalability

The various [Trice ID management](id_management.md) options allow the organization also of bigger software systems. 16383 possible different IDs should match also large projects. Just in case: 16-bit for the ID is a not too hard changeable value.

## 16. <a id="portability-and-modularity"></a>Portability and Modularity

The Trice tool is written in the open source language [*Go*](https://go.dev/) and is therefore usable on many platforms. That means the automatic code patching and ID handling side with `trice insert`.

All C-compilers should be usable to compile the target Trice code and there is no hardware dependency despite the byte transmission. MCUs with 8-bit to 64-bit, little or big endian are supported.

Any user program able to read a [JSON](../../demoTIL.json) file, can receive the [documented](binary_encoding.md) Trice message format, look-up the ID and perform a printf-like action to translate into log strings. The Trice tool with its `log` switch is a working example.

Using no framing, [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) or [TCOBS](https://github.com/rokath/tcobs) packages starting with a [package descriptor](binary_encoding.md#package-format) allows alongside user protocols. The other way around is also implementable: In a user protocol embedded `Trice` messages.

The Trice tool is expandable with several decoders. So it is possible to implement a minimal Trice encoding, if bandwidth matters heavily and control that with switches.

When less RAM usage is more important the target double buffer is replaceable with a ring buffer. So the user will be able to decide at compile time about that. A ring buffer mode is selectable inside [triceConfig.h](../../examples/F030_inst/Core/Inc/triceConfig.h) avoiding any buffer by paying a time toll.

The Trice tool supports [many command line switches](../ref/trice-help-all.txt).

## 17. <a id="optional-trice-messages-encryption"></a>Optional Trice messages encryption

The encryption opportunity makes it possible to test thoroughly a binary with log output and releasing it without the need to change any bit but to make the log output unreadable for a not authorized person. Implemented is the lightweight [XTEA](https://en.wikipedia.org/wiki/XTEA) as option, what will be sufficient for many cases. It should be no big deal to add a different algorithm.

## 18. <a id="trice-protection"></a>Trice Protection

When using Trice, data are written into buffers. A buffer overflow is impossible with the default configuration `#define TRICE_PROTECT 1` by simply ignoring possible overflow causing Trice statements. Those cases are not detectable by the cycle counter evaluation because non-existing Trice data on the embedded system cannot cause cycle errors. Therefore overflow error counters exists, which the user can watch. In [./examples/exampleData/triceLogDiagData.c](../../examples/exampleData/triceLogDiagData.c) an option is shown. Of course this buffer overflow protection costs valuable execution time. If you prefer speed over protection, simply write into your project specific _triceConfig.h_ `#define TRICE_PROTECT 0`.

## 19. <a id="trice-diagnostics"></a>Trice Diagnostics

A trice statement produces 4 bytes buffer data plus optional values data. When for example `TRice16("Voltage=%u\n", x);` is called inside the ms system-tick interrupt every 5th time, 10 bytes data are generated each 5 millisecond. This needs a transfer baudrate of at least 20.000 bit/s. A UART running at 115.200 baud can easily handle that.
Anyway after 100 ms, a 200 Bytes buffer is filled and the question arises what is the optimal Trice buffer size. A calculation is error prone, so measuring is better. So configure the buffer sizes bigger than estimated and watch the max depth of their usage. In [./examples/exampleData/triceLogDiagData.c](../../examples/exampleData/triceLogDiagData.c) an option is shown. After you optimized your buffer sizes, you can deactivate the Trice diagnostics in your project specific _triceConfig.h_ with `#define TRICE_DIAGNOSTICS 0`.

## 20. <a id="trice-cache"></a>Trice Cache

One may think, automatically cleaning the IDs in the target code with `trice c` after building and re-inserting them just for the compilation needs file modifications all the time and a permanent rebuild of all files containing Trices will slow down the re-build process. That is true, but by using the Trice cache this is avoidable.
Simply one-time create a `.trice/cache` folder in your home directory and use `trice insert -cache` and `trice clean -cache` in your [build.sh](../../examples/L432_inst/build.sh) script.
Find more details in chapter [Trice Cache for Compilation Speed](insert_clean_cache.md).

## 21. <a id="avoiding-false-positive-editor-warnings"></a>Avoiding False-Positive Editor Warnings

When the user writes

```C
trice("msg: Hello! 👋🙂\n");
```

after `trice insert` this gets

```C
trice(iD(123), "msg: Hello! 👋🙂\n");
```

and the compiler builds and then with `trice clean`, this gets again

```C
trice("msg: Hello! 👋🙂\n");
```

Sophisticated editors may detect the missing ID and warn by underlining the trice command:

![x](../ref/triceHello.png)

To avoid this you can add the following line to your project specific _triceConfig.h_ file:

```C
#define TRICE_CLEAN 1
```

The Trice tool, will change the value to 0 and change it back to 1, when performing the ID insertion and cleaning, when this line occurs inside the _triceConfig.h_ file. This way these false-positive editor warnings are avoidable:

![x](../ref/triceHelloOKnoID.png)
![x](../ref/triceHelloOKwithID.png)

It is recommended to use the Trice cache in conjunction with this to avoid a permanent re-translation of files including Trice code.

TRICE_CLEAN==1 changes all Trice macros into empty ones. It is used only to silence sophisticated editors. In the cleaned state, when the IDs are removed from the files, the editor could underline the Trice macros indicating a false positive.

Do not use TRICE_CLEAN for disabling Trice macros. The *triceConfig.h* line `#define TRICE_CLEAN 0` changes to `1` with every `trice clean` and to `0` with every `trice insert`. This line is optional and must not be in a different file. If you want to disable Trice macros use TRICE_OFF.

## 22. <a id="trice-generator"></a>Trice Generator

The Trice tool is able to generate colors or code to support various tasks. One interesting option is the **A**synchronous **B**roadcast **C**ommand support, allowing ABC usage in a network of embedded devices.

Read chapter [Trice ABC - Asynchronous Broadcast Commands](abc_asynchronous_broadcast_commands.md) or type:

```bash
trice help -generate
```

## 23. <a id="versions-and-variants-trice-stability"></a>Versions and Variants Trice Stability

When developing firmware, we get often different versions and variants in the developing process. When, for example, getting an older device back, it could be, we do not know the flashed firmware version at all. Because the Trice tool adds only IDs and their Trices to the project specific _til.json_ file, the complete development history remains in that file. So connecting an old device to the Trice tool will deliver correct output. Of course the location information will be outdated. Use the _li.json_ archived with the corresponding firmware build for accurate source positions. Without that file, disable locations with `trice log -li off`; `-showID "%6d"` displays the IDs for lookup in the matching sources.

## 24. <a id="legacy-project-code-integration"></a>Legacy Project Code Integration

When it comes to instrument legacy project with Trice or to integrate legacy project files into a Trice instrumented project different approaches are possible:

1. Use for user specific log statements a different output channel. No special care has to be taken. This is maybe acceptable in some cases.
2. Replace user specific log statements with Trice statements using a text processor and adapt the float, double or runtime strings handling manually. This is acceptable for small code amounts and when it is no problem to edit the legacy sources.
3. Get the legacy output packages before transmitting them, add a 2-byte count in little-endian (0-16383) in front and frame them the same way the trice packages get framed (for example with COBS). This will set the 2 most significant bits to 00 and the Trice tool, can get informed via CLI switch to treat those packages accordingly. The user code containing specific logs will work unchanged together with Trice code over the same output channel.
4. Take advantage of the new support for dynamic trice and triceS macro aliases ([Legacy User Code Option Trice Aliases Adaptation](legacy_user_code.md#legacy-user-code-option-trice-aliases-adaptation)).

<p align="right">(<a href="#trice-features">back to top</a>)</p>

