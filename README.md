# ![TriceGirlS.png](./docs/README.media/TriceGirl-167x222.png) **Trice** -> **TR**ace **I**ds **C** **E**mbedded  

Tiny & fast trace code for embedded device real-time PC logging (trace ID visualization) over any port.

## Info shields

![GitHub Workflow Status](https://img.shields.io/github/workflow/status/rokath/trice/goreleaser)
![GitHub issues](https://img.shields.io/github/issues/rokath/trice)
![GitHub All Releases](https://img.shields.io/github/downloads/rokath/trice/total)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/rokath/trice)
![GitHub watchers](https://img.shields.io/github/watchers/rokath/trice?label=watch)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/rokath/trice)
![GitHub commits since latest release](https://img.shields.io/github/commits-since/rokath/trice/latest)

## Link shields

[![Go Report Card](https://goreportcard.com/badge/github.com/rokath/trice)](https://goreportcard.com/report/github.com/rokath/trice)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com)
[![test](https://github.com/shogo82148/actions-goveralls/workflows/test/badge.svg?branch=main)](https://coveralls.io/github/rokath/trice)
[![Coverage Status](https://coveralls.io/repos/github/rokath/trice/badge.svg?branch=master)](https://coveralls.io/github/rokath/trice?branch=master)

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

- [github.io/trice/](https://rokath.github.io/trice/)

## About

- Printf-like trace macros `TRICE` and PC `trice` tool (written in [Go](https://en.wikipedia.org/wiki/Go_(programming_language))) for automatic ID managing & logging.
- Communication without string transfer, just with IDs. Prerequisite: byte transmission to PC, low bandwidth is ok:
  - method does'nt matter: \
  [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter), \
  [I²C](https://en.wikipedia.org/wiki/I%C2%B2C), \
  [SPI](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface), \
  [GPIO](https://circuitcellar.com/cc-blog/a-trace-tool-for-embedded-systems/), \
  [RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/), \
  [CAN](https://en.wikipedia.org/wiki/CAN_bus), \
  [LIN](https://en.wikipedia.org/wiki/Local_Interconnect_Network), ...
- "log in (a) trice" ([S>G](https://www.screentogif.com/)) ![ ](./docs/README.media/life0.gif)
- Main idea: Logging strings **not** into an embedded device to display them later on a PC but keep usage comfortable and simple.

## `TRICE` macros for C & C++ code

- Real fast (**12 CPU clocks per trice possible!!!**)
- TRICE in your code **reduces the needed FLASH memory** because the instrumentation code is very small (can be less 200 bytes FLASH and about 100 bytes RAM) and no printf library code nor log strings are inside the embedded device anymore.

## Possible Use Cases

- Using trice not only for **dynamic debugging** but also as **logging** technique
    is possible and gives the advantage to have very short messages (no strings) for transmission,
    but keep in mind that the file [til.json](https://github.com/rokath/trice/blob/master/til.json) is the key to read all output if your devices in the field for 10 or more years.
- You can consider TRICE also as a kind of **data compression** what could be interesting for IoT things, especially NB-IoT, where you have very low data rates.
- Storing trices in FLASH for later log analysis saves memory because a `TRICE` occupies only 4 bytes.
- Also it is possible to **encrypt** the trice transfer packets to get a reasonable protection for many cases.
  - This way you can deliver firmware images with encrypted TRICE output only readable with the appropriate key and til.json.
  - XTEA is implemented as one option.
- You can even translate the til.json in **different languages**, so changing a language is just changing the til.json file.
- Using trice with an **RTOS** gives the option for detailed **task timing analysis**. Because of the very short execution time of a trice you could add

```c
    Trice8_2i( "sig:task %d -> %d: ", previousTaskID, nexTaskID );
    Trice16_1i( "tim:tick=%d\n", clock );
```

 to the scheduler and vizualize the output on PC. The same is possible for **interrupt timing analysis**.

- Mixed case trice macros are short trices and the letter i at the end says **i**nside critical section. (FLEX encoding)
- `Trice16_1( "tim: myFunc %d\n", sysTick );` before and after a function call lets you easy measure the function execution time.
- As graphical vizualisation you could use a tool similar to [https://github.com/sqshq/sampler](https://github.com/sqshq/sampler).
- TRICE has intentionally no target timestamps for performance reasons. On the PC you can display the *reception timestampts*. But you can add own **timestamps as parameters** for exact embedded time measurements. Having several devices with trice timestamps, **network timing measurement** is possible.

## How it approximately works

For example change the source code line

```c
printf( "MSG: %d Kelvin\n", k );
```

into

```c
TRICE16( "MSG: %d Kelvin\n", k );
```

`trice update` (run it automatically in the tool chain) changes it to  

```c
TRICE16( Id(12345), "MSG: %d Kelvin\n", k );
```

or (if `-addParamCount` is used)

```c
TRICE16_1( Id(12345), "MSG: %d Kelvin\n", k );
```

and adds the *ID 12345* together with *"MSG: %d Kelvin\n"* into a **t**rice **I**D **l**ist, a JSON referece file named [til.json](https://github.com/rokath/trice/blob/master/til.json).

- The *12345* is a randomly generated 16 bit ID not used so far.
- With the `16` in TRICE**16** you adjust the parameter size to 16 bit what allows more runtime efficient code compared to `32` or `64`.
- The appended **_1** sets the expected parameter count to 1, allowing a compile time parameter count check.
- During compilation the `TRICE16_1` macro is translated to only a *12345* reference and the variable *k*. The format string never sees the target.

This is a slightly simplified view:

![trice](./docs/README.media/trice4BlockDiagram.svg)

- When the programflow passes the line `TRICE16_1( Id(12345), "MSG: %d Kelvin\n", k );` the 16 bit ID *12345* and the 16 bit temperature value are transfered as one combined 32 bit value into the triceFifo, what goes really fast. Different encodings are possible. The program flow is nearly undisturbed, so **TRICE macros are usable also inside interrupts or in the scheduler**.
- For visualization a background service is needed. In the simplest case it is just an UART triggered interrupt for triceFIFO reading.
- During runtime the PC trice tool receives the trice as a 4 byte package `0x30 0x39 0x00 0x0F` from the UART port.
- The `0x30 0x39` is the ID 12345 and a map lookup delivers the format string *"MSG: %d Kelvin\n"* and also the format information *"TRICE16_1"*. Now the trice tool is able to execute `printf("MSG: %d Kelvin\n", 0x000e);` and the full log information is displayed.
- Only the parameter count and size affect encoding size but not the format string length.

## `trice` PC tool

- Manages `TRICE` macro IDs inside a C or C++ source tree and extracts the strings in an ID-string list during target device compile time.
- Displays `TRICE` macros like printf() output in realtime during target device runtime. The received IDs and parameters are printed out.
- Can receive trices on several PCs and display them on a remote display server.
- Written in [Go](https://github.com/golang/go), simply usage, no installer, needs to be in $PATH.

## How to start

- Get [trice](https://github.com/rokath/trice) or download latest release assets for your system: Source code and compressed binaries.
- A port to Darwin should be easy possible.  

### Either use pre-compiled `trice` binary

- Place the extracted `trice` binary somewhere in your $PATH.

### Or build `trice` from Go sources

- Install [Go](https://golang.org/).
- Open a console inside the `trice` directory.
- Check and install:

```b
go vet ./...
go test ./...
go install ./...
```

Afterwards you should find an executable `trice` inside $GOPATH/bin/

### Running

```b
trice help
```

### Quick target setup

- It is sufficient for most cases just to use the `TRICE32` macro with max 4 parameters as a replacement for `printf` and to use the default settings.
- Compare the **not** instrumented test project [MDK-ARM_LL_generatedDemo_STM32F030R8-NUCLEO-64](https://github.com/rokath/trice/tree/master/test/MDK-ARM_LL_generatedDemo_STM32F030R8-NUCLEO-64) with one of the instrumented test projects in [test](https://github.com/rokath/trice/tree/master/test/) to see what to to.
  - Recommendation: FLEX encoding
- **Or** follow these steps for instrumentation information even your target processor is not an ARM (any bit width will do):
  - Install the free [STCubeMX](https://www.st.com/en/development-tools/stm32cubemx.html).
  - Choose from [test examples](https://github.com/rokath/trice/tree/master/test) the for you best fitting project `MyExample`.
  - Open the `MyExample.ioc` file with [STCubeMX](https://www.waveshare.com/wiki/STM32CubeMX_Tutorial_Series:_Overview) and generate without changing any setting.
  - Make an empty directory `MyProject` inside the `test` folder and copy the `MyExample.ioc` there and rename it to `MyProject.ioc`.
  - Open `MyProject.ioc` with [STCubeMX](https://www.waveshare.com/wiki/STM32CubeMX_Tutorial_Series:_Overview), change in projects settings `MyExample` to `MyProject` and generate.
  - Now compare the directories `MyExample` and `MyProject` to see the trice instrumentation as differences.

## Documentation

No need to read all this stuff - is is just for help and reference.

- [Common.md](https://github.com/rokath/trice/tree/master/docs/Common.md)
- [TriceEncodings.md](https://github.com/rokath/trice/tree/master/docs/TriceEncodings.md)
- [ID management](https://github.com/rokath/trice/tree/master/docs/IDManagement.md)
- [OneWireOption](https://github.com/rokath/trice/tree/master/docs/OneWireOption.md)
- [SeggerRTT](https://github.com/rokath/trice/tree/master/docs/SeggerRTT.md)
