<div id="top">

# 🅃🅁ace 🄸d's 🄲 🄴mbedded

#### ![TriceGirlS.png](./docs/ref/TriceGirl-167x222.png) Hi, I am Trice.

<!-- [github.io/trice/](https://rokath.github.io/trice/) -->

<!-- ![GitHub Workflow Status](https://img.shields.io/github/workflow/status/rokath/trice/goreleaser) -->
![GitHub issues](https://img.shields.io/github/issues/rokath/trice)
![GitHub All Releases](https://img.shields.io/github/downloads/rokath/trice/total)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/rokath/trice)
![GitHub watchers](https://img.shields.io/github/watchers/rokath/trice?label=watch)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/rokath/trice)
![GitHub commits since latest release](https://img.shields.io/github/commits-since/rokath/trice/latest)
[![Go Report Card](https://goreportcard.com/badge/github.com/rokath/trice)](https://goreportcard.com/report/github.com/rokath/trice)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com)
[![Coverage Status](https://coveralls.io/repos/github/rokath/trice/badge.svg?branch=master)](https://coveralls.io/github/rokath/trice?branch=master)

<!-- [![Sponsor rokath](https://github.com/sponsors/rokath/button)](https://github.com/sponsors/rokath) -->
<!--[![test](https://github.com/shogo82148/actions-goveralls/workflows/_test/badge.svg?branch=main)](https://coveralls.io/github/rokath/trice)-->

Log in (a) trice ([S>G](https://www.screentogif.com/)) ![ ](./docs/ref/life0.gif)

even inside **↯ interrupts** in less than 1 µs❗

## About

- Replace `printf` or `log` in **C**-code for getting:
  - **[Speed](./docs/TriceUserManual.md/#trice-speed)**, to be usable also **inside interrupts**,
  - **[Space](./docs/TriceUserManual.md/#trice-memory-needs)**, to reduce needed FLASH memory size,
  - **[Features](./docs/TriceUserManual.md/#trice-features-(overview))** delighting the developers heart.
- <u>Main idea:</u> Logging strings **not** into an embedded device to display them later on a PC but keep [usage comfortable and simple](./docs/TriceUserManual.md/#port-it).
- *Trice* consists of 2 parts:
  1. **C** code `trice` macros, similar usable like `printf`, generating tiny & super-fast embedded device real-time trace/log code.
  2. Tool **trice** for managing and visualization.
      - Written in [Go](https://golang.org/) and therefore usable on all platforms Go supports.
      - You can also use your own environment to receive the *Trice* packages, exchange the carried IDs with the format string and print out.
- [Trice User Manual](./docs/TriceUserManual.md)
- Trice Status: **_Trice is ready to be used!_**

## Possible Use Cases

- Using *Trice* not only for **printf debugging** but also as **logging** technique is possible and gives the advantage to have very short messages (no strings) for transmission, but keep in mind that the file [til.json](./_test/testdata/til.json) is the key to read all output if your devices in the field for 10 or more years.
  - Optionally add [til.json](./_test/testdata/til.json) as a (compressed) resource to your target image. One possibility is using [SRecord](http://srecord.sourceforge.net/download.html). Or simply provide a download link.
- You can see *Trice* also as a kind of **data compression** what could be interesting for [IoT](https://en.wikipedia.org/wiki/Internet_of_things) things, especially [NB-IoT](https://en.wikipedia.org/wiki/Narrowband_IoT), where you have very low data rates.
- Storing *Trice* messages in [FLASH memory](https://en.wikipedia.org/wiki/Flash_memory) for later log analysis saves memory because a typical `TRICE` occupies only 4 bytes independently of the format string length plus optional values.
- Also, it is possible to **encrypt** the *Trice* transfer packets to get a reasonable protection for many cases.
  - This way you can deliver firmware images with encrypted *Trice* output, only readable with the appropriate key and [til.json](./_test/testdata/til.json).
  - [XTEA](https://en.m.wikipedia.org/wiki/XTEA) is implemented as one option.
- You can even translate the [til.json](./_test/testdata/til.json) file in **different languages**, so changing a language is just changing the [til.json](./_test/testdata/til.json) file without touching the target binary.
- With *Trice* it is easy to do **timing analysis** on distributed embedded systems. Host and target timestamps are supported.

## How it approximately works (UART example)

This slightly simplified [view](https://github.com/jgraph/drawio) is explained [here](./docs/TriceUserManual.md/#how-it-works---the-main-idea)

![trice](./docs/ref/triceCOBSBlockDiagram.svg)

## Data Transfer

- Implemented:
  - [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter) connectable to virtual UART over USB
  - [RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) over [J-Link](./third_party/segger.com/ReadMe.md) and [RTT over ST-Link](./third_party/goST/ReadMe.md)
  - TCP4 
  - UDP4 (currently as PR #529)
- A small separate microcontroller is always usable as interfaces bridge from [GPIO](https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/), [I²C](https://en.wikipedia.org/wiki/I%C2%B2C), [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface), [CAN](https://en.wikipedia.org/wiki/CAN_bus), [LIN](https://en.wikipedia.org/wiki/Local_Interconnect_Network), ... to UART.
- With a Chip from FTDI, check for example [Adafruit FT232H Breakout](https://learn.adafruit.com/adafruit-ft232h-breakout), GPIO, I2C and SPI are easy accessable.
## Display server option

Start `trice ds` inside a console, option: [third_party/alacritty](./third_party/alacritty), locally or on a remote PC and connect with several **trice** tool instances like with `trice log -p COM15 -ds` for example.

## Documentation

- [https://interrupt.memfault.com/blog/trice](https://interrupt.memfault.com/blog/trice) (a bit outdated)
- [Trice User Manual](./docs/TriceUserManual.md) (covers also the interrupt blog actualized information)
- Check [issues](https://github.com/rokath/trice/issues) and [discussions](https://github.com/rokath/trice/discussions) including the closed items.
- Read the target source code, especially [triceDefaultConfig.h](./src/triceDefaultConfig.h)
- View the [CLI](https://en.wikipedia.org/wiki/Command-line_interface) options by executing `trice help -all` in a terminal or reading [tricehelpall_test.go](./internal/args/tricehelpall_test.go)
- Look into and optionally modify [./internal/emitter/lineTransformerANSI.go](./internal/emitter/lineTransformerANSI.go) (Needs `go install ./cmd/trice/...` afterwards.)

## Debugging using VS-Code and Clang for a Trice-instrumented Project in Direct-Out Mode over SEGGER-RTT

(See chapter [Development Environment Setup](./docs/TriceUserManual.md#development-environment-setup) for more details.)

<img src="./docs/ref/Animation.gif" width="1200">

## Trice Cache

Starting with Trice version 0.70.0 it is possible to use additionally the Trice `-cache` CLI switch for the commands `trice insert` and `trice clean`. This switch will have only effect when the user creates in his home directory the **`.trice/cache`** folder. ([Trice Cache Spec](./docs/TriceUserManual.md#trice-cache-for-compilation-speed))

### When to use it

When you use `trice i` as a pre-compile step and `trice c` as a post-compile step to have the IDs not in the project source code when you work on it, therefore only during compilation time, and wish to speed-up the whole thing.

### How it works

The Trice cache keeps copies of all to `trice i` or `trice c` passed files after processing them, to avoid repeatedly ID inserting and cleaning. **The copies are used to get the same results as with `trice i` or `trice c` for all files not edited inbetween.** Edited files are processed normally and the cache is updated afterwards. Because the file copies are done without changing the file modification time, **a build system does not process unchanged files again even the IDs have been temporarily removed from the files**.

### Hint

> **<span style="color:red"> Special care is needed, when the build system modifies source files as well!</span>**

For example an auto-formatter should get active **before** the `tice insert` command.

## Wich mode to use?

- For the development direct mode with SEGGER_RTT is recommended.
- Most use cases are coverable in deferred mode with TRICE_BUFFER == TRICE_RING_BUFFER (less RAM needs) in TRICE_MULTI_PACK_MODE (less transfer data).

## Project Status

Trice is full usable and there are no known bugs (see [issues](https://github.com/rokath/trice/issues)).

## Future 

The documentation could get improved, for example by adding a quick start help. Additional features like remote procedure calls could be described and a separate tlog, maybe written in C, would allow logging on any platforms - not only **Go** supported ones.

## Structured Logging

Trice will (hopefully soon) suppport structured logging. Triggered by [#531](https://github.com/rokath/trice/discussions/531) there is now a [specifcation draft](https://github.com/rokath/trice/blob/master/docs/StructuredLoggingWithTrice.md). Please provide some critical feedback before the implementaion starts.

## Support?

* Yes please - Trice takes a lot of my spare time and I wish to keep it MIT licensed in the future as well.
* If you make profit using Trice in your products, donations would convince my family to proceed with Trice and also secure future improvements. 

<!-- * Or :star: it. ☺ --> 

[Become a Sponsor with your Github Account](https://github.com/sponsors/rokath/)

OR

<a href="https://www.buymeacoffee.com/rokath" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="30" width="120"></a>

OR

<p>
  <a href="https://www.paypal.me/rolfkarlthomas">
      <img src="https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif" alt="paypal" height="40" width="120">
  </a>
</p>


Cloning the repo:

```b
git clone https://github.com/rokath/trice.git
```

## Similar projects

- ARM ITM/SWO (hardware-native)
- ARM Keil Event Recorder (hardware-native)
- [baical.net](http://baical.net/up7.html) (C)
- [call stack logger function instrumentation](https://dev.to/taugustyn/call-stack-logger-function-instrumentation-as-a-way-to-trace-programs-flow-of-execution-419a) (a way to trace programs flow of execution)
- [Debugging with Dynamic Printf Breakpoints](https://mcuoneclipse.com/2022/02/09/debugging-with-dynamic-printf-breakpoints/) (Eclipse IDE option)
- [defmt (Rust)](https://github.com/knurling-rs/defmt)
- [Diagnostic Log and Trace](https://github.com/COVESA/dlt-daemon) (AUTOSAR)
- [Embedded Logger (elog)](https://github.com/martinribelotta/elog) (Embedded logger with minimal footprint and memory usage)
- [Logging with symbols - The Embedonomicon](https://docs.rust-embedded.org/embedonomicon/logging.html)
- [McuLog (McuOnEclipse)](https://mcuoneclipse.com/2020/06/01/mculog-logging-framework-for-small-embedded-microcontroller-systems/)
- [MCUViewer](https://github.com/klonyyy/MCUViewer)
- [Memfault Compact Log Library](https://docs.memfault.com/docs/mcu/compact-logs/#host-decoding) (part of firmware SDK)
<!-- - [Metal Serial Library](https://github.com/metal-ci/_test/tree/master/doc/metal.serial.md) -->
- [Minimal Structured Logging for Autonomous Vehikles](https://youtu.be/FyJI4Z6jD4w) (C++, closed source talk)
- [NanoLog (C++11)](https://github.com/PlatformLab/NanoLog) (Linux C++)
- [Percepio Tracealyzer (with TraceRecorder)](https://percepio.com/tracealyzer/) (Visual Trace Diagnostics)
- [Pigweed Trace Tokenized](https://pigweed.dev/pw_trace_tokenized/)
- [Postform](https://github.com/Javier-varez/Postform) (Postponed formatting experiments with string interning in C++. Inspired by the defmt Rust crate )
- [qpspy](https://www.state-machine.com/qtools/qpspy.html) (C/C++)
- [Real-time binary data logging/tracing toolkit](https://github.com/RTEdbg/RTEdbg)
- [SEGGER System View](https://www.segger.com/products/development-tools/systemview/technology/what-is-systemview/)
- [Serial-Studio](https://github.com/Serial-Studio/Serial-Studio) (Data visualisation)
- [Tonbandgerät](https://github.com/schilkp/Tonbandgeraet) (A small embedded systems tracer with support for bare-metal and FreeRTOS-based targets)
- [Traces](https://github.com/yotamr/traces) (API tracing framework for Linux C/C++ applications
- [uLog (RD Poor)](https://github.com/rdpoor/ulog)
- [Zepyr Dictionary Based Logging](https://docs.zephyrproject.org/3.1.0/services/logging/index.html#dictionary-based-logging)

[ChatGPT Trice Compare (2025-05-26)](https://htmlpreview.github.io/?https://github.com/rokath/trice/blob/master/docs/ChatGPTo4-mini-high_TriceCompare.html)

<!--- B A C K U P

$${\color{red}Trice}$$ <- $${\color{red}TR}$$~~ace~~ $${\color{red}I}$$~~d's~~ $${\color{red}C}$$   $${\color{red}E}$$~~mbedded~~ 

- [call stack logger function instrumentation](https://sii.pl/blog/call-stack-logger-function-instrumentation-as-a-way-to-trace-programs-flow-of-execution/) (a way to trace programs flow of execution)

## `TRICE` macros for C & C++ code

- Real fast: **12 CPU clocks per (short) *trice* possible!!!**
  - With a 48MHz clock this is 250ns. Light travels about 80 meters in that time.
- TRICE in your code **reduces the needed FLASH memory** because the instrumentation code is very small (can be less 200 bytes FLASH and about 100 bytes RAM) and no printf library code nor log strings are inside the embedded device anymore.


## ATTENTION 4

- In release v0.41.0 now the `TRICE` macro works additionally. To use it, simply use it like `printf`:
  - No need for parameter count and bit width.
    - The internal used parameter bit width is 32 bit, but you can use also `TRICE8`, `TRICE16`, `TRICE32`, `TRICE64`, 
    - 0 to 12 parameters possible (extendable).
  - No strings supported ("%s"). Use `TRICE_S` than.
  - Many usage options inside `test/testdata/triceCheck.c` visible.
- Needs better tests and updated documentation.

## ATTENTION 3

In release v0.39.0 now encryption works again. To implement it well and open for future, the additional COBS package descriptor is now 4 bytes long. That means the **trice** tool version 0.39.0 does not work with older target code. Please update your target code or stay with an older release.
Probably the COBS encoding will not change in the next time anymore.

## ATTENTION 2

In release v0.38.0 now target timestamps possible. To implement it well and open for future, an additional COBS package descriptor byte was added. That means the **trice** tool version 0.38.0 does not work with older target code. Please update your target code or stay with an older release.

## ATTENTION

The **TRICE** technique changed heavily between release 0.33.0 and 0.34.0. The `flex` and `esc` encodings are replaced by a [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) encoding which will be the default now. The stuff works already well but is not in its final state and is not documented vet. It lacks also automated tests. The internal speed goes to its limit (~6 clocks per *trice* on M0+ possible) by using a double buffer instead of a fifo. Also porting is easier now. The documentation is outdated but gets updated soon. But first the tests. If you have a project with `flex` or `esc` encoding, please update the target code or stay with version 0.33.0.

 Because of the very short execution time of a *trice* you could add to the scheduler:

```c
    Trice16i( "tim:@tick %5u ", clock );
    Trice8i( "sig:task %u -> %u\n", previousTaskID, nexTaskID );
```

 The execution of this code block produces totally 8 log bytes to visualize the output on PC, what looks similar to this for 3 task switches:

![alt](./docs/ref/taskSwitchTimesExample.PNG)

First are the PC reception timestamps and after the port info are the used *trice* ids just for easy location inside the source code. See the diferences between the (blue) ticks in this 3 lines. These are 28 or 36 processor clocks only. The code producing this is:

![alt](./docs/ref/taskSwitchTimesExampleCode.PNG)

  The same is possible for **interrupt timing analysis**.

- Mixed case TRICE macros are [short](./docs/TriceEncodings.md#flex-short-sub-encoding) *Trices* and the letter i at the end says **i**nside critical section. (FLEX encoding)
- `Trice16( "tim: myFunc %d\n", sysTick );` before and after a function call lets you easy measure the function execution time.
- As graphical visualization you could use a tool similar to [https://github.com/sqshq/sampler](https://github.com/sqshq/sampler).

<!---
## Target timestamps?

- TRICE has intentionally no target timestamps for performance reasons. Also it is not foreseeable which time base is needed in which application. On the PC you can display the *reception timestamps*.
- Because several *trice* statements can form a single log line, a generally added timestamp would cause difficulties with that. This could be handled but adds complexity not worth the effort.
- But you can add own **timestamps as parameters** for exact embedded time measurements. Having several devices with *trice* timestamps, **network timing measurement** is possible.

### Target timestamp examples

- Lets say you have a 16 bit systick, called `SYSTICKVAL16` and 16 bit timestamps are fine for you:
  - Simply add `trice16( "time:@%5u:", SYSTICKVAL16 );` everywhere you need exact time.
  - Or use `trice16( "time:@%5u:My values are %d, %d, %d\n", SYSTICKVAL16, my0, my1, my2 );`
- Same with a 32 bit systick, called `SYSTICKVAL32`:
  - Simply add `trice32( "time:@%9u:", SYSTICKVAL32 );` everywhere you need exact time.
  - Or use `trice32( "time:@%9u:My values are %d, %d, %d\n", SYSTICKVAL32, my0, my1, my2 );`

This is a slightly simplified [view](https://github.com/jgraph/drawio):

![trice](./docs/ref/trice4BlockDiagram.svg)

- When the program flow passes the line `Trice16( Id(12345), "MSG: %d Kelvin\n", k );` the ID *12345* and the 16 bit temperature value are transferred as one combined 32 bit value into the triceFifo, what goes really fast. Different encodings are possible. The program flow is nearly undisturbed, so **TRICE macros are usable also inside interrupts or in the scheduler**.
- For visualization a background service is needed. In the simplest case it is just an UART triggered interrupt for triceFIFO reading. Or you can use [RTT](./docs/TriceOverRTT.md).
- So the whole target instrumentation are the *trice* macros, the *trice* fifo and the UART  ISR.
- During runtime the PC **trice** tool receives the *trice* as a 4 byte package `0x30 0x39 0x00 0x0e` from the UART port.
- The `0x30 0x39` is the ID 12345 and a map lookup delivers the format string *"MSG: %d Kelvin\n"* and also the format information *"TRICE16_1"*. Now the **trice** tool is able to execute `printf("MSG: %d Kelvin\n", 0x000e);` and the full log information is displayed in the MSG color.
- Only the parameter count and size affect encoding size but not the format string length.

## `trice` PC tool

- Manages `TRICE` macro IDs inside a C or C++ source tree and extracts the strings in an ID-string list during target device compile time.
- Displays `TRICE` macros like printf() output in real-time during target device runtime. The received IDs and parameters are printed out.
- Can receive trices on several PCs and display them on a remote display server.
- Written in [Go](https://github.com/golang/go), simply usage, no installer, needs to be in $PATH.

## Structured Logging?

Right now only event logging is implemented.

According to the design aim **"Keep embedded device code small and fast"** there is no structuring code inside the target device, **but** you can add channel information to the *trice* log strings:

```c
trice32( Id(12345), "Verbose: bla bla")
```

These can be understood as tags too. But only one tag per *trice* right now.
Look into [lineTransformerANSI.go](./internal/emitter/lineTransformerANSI.go) for options or extensions.

Also you can at compile time disable *trice* code generation on file level with `#define TRICE_OFF 1` before including `trice.h`.

Because [one *trice* consists typically only of 4 to 8 bytes](./docs/TriceEncodings.md#flex-encoding) there is usually no need to dynamically switch trices on and off inside the embedded device. This can be done on the display side inside the **trice** tool with the command line switches `-ban` or `-pick`. For example `-pick err,wrn` disables all output despite error and warning messages.
Switching trices on and off inside the target increases the overhead and demands some kind of command interface.
If needed, always an `if` is usable.

The **trice** tool can also perform further tasks like JSON encoding with additional log information and transferring this information to some webserver in the future.

## Search counters

![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/trace)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/instrumentation)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/embedded)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/logging)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/real-time)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/debugging)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/monitoring)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/terminal)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/cli)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/diagnostics)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/tool)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/data-recording)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/rtos)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/multi-language-support)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/compression)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/timing-analysis)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/time-measurement)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/golang)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/printf)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/encryption)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/serial)
![GitHub search hit counter](https://img.shields.io/github/search/rokath/trice/C)


<iframe src="https://github.com/sponsors/rokath/button" title="Sponsor rokath" height="32" width="114" style="border: 0; border-radius: 6px;"></iframe>


<iframe src="https://github.com/sponsors/rokath/card" title="Sponsor rokath" height="225" width="600" style="border: 0;"></iframe>

-->

<p align="right">(<a href="#top">back to top</a>)</p>
