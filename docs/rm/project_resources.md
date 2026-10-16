# Project Background and Resources

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

- [1. Download and install Trice](#download-and-install-trice)
- [2. Alternative projects and related approaches](#alternative-projects-and-related-approaches)
  - [2.1. Logging and deferred formatting](#logging-and-deferred-formatting)
  - [2.2. Tracing and visualization](#tracing-and-visualization)
  - [2.3. Other techniques and background reading](#other-techniques-and-background-reading)
- [3. Community and contributions](#community-and-contributions)
- [4. Support and sponsoring](#support-and-sponsoring)
- [5. Project history](#project-history)
- [6. Ideas for future development](#ideas-for-future-development)
  - [6.1. Further Context Enrichment Variants](#further-context-enrichment-variants)
  - [6.2. Improving the Trice Tool Internal Parser (not planned right now)](#improving-the-trice-tool-internal-parser-not-planned-right-now)
    - [6.2.1. Trice Internal Log Code Short Description](#trice-internal-log-code-short-description)
  - [6.3. Using Trice on Servers](#using-trice-on-servers)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

## 1. <a id="download-and-install-trice"></a>Download and install Trice

Choose a release or a source checkout, then use its matching target sources, host tools, and documentation. An older release binary may not contain features documented by a newer checkout. See [firmware and host-tool compatibility](id_management.md#compatibility-with-firmware-and-host-tool-versions).

- [GitHub Releases](https://github.com/rokath/trice/releases) provides published host-tool archives and source archives. Choose the operating system and architecture of your host computer; extract both `trice` and `tlog` and add their directory to `PATH`.
- For a checkout build, run the existing script from the repository root with Go and Bash installed:

  ```sh
  ./scripts/buildTriceTool.sh
  trice --version
  tlog --version
  ```

  The script builds `trice` and `tlog` and prints the installation paths. Use the Go version required by [go.mod](../../go.mod); see [building the host tools](tool_build_release.md#build-trice-tool-from-go-sources) for details.
- For v2, use a release archive or build a checkout of its tag. Remote `go install github.com/rokath/trice/cmd/trice@v2.0.0` is not supported while the module path has no `/v2` suffix. A local checkout build does not require that suffix.
- The macOS Homebrew tap provides stable releases: `brew install rokath/tap/trice`. For a beta, use its tagged release archive or build from its source checkout. Confirm the installed version rather than assuming it includes the current development checkout.

For the first hardware-free experiment, follow the [User Manual](../TriceUserManual.md#get-lib-and-tools). Its PC examples additionally require a native C compiler. Shell examples use Bash; Windows users can run them in Git Bash with a Windows host compiler. For target integration, choose a [firmware quickstart](quickstarts.md).

## 2. <a id="alternative-projects-and-related-approaches"></a>Alternative projects and related approaches

Trice combines printf-like source calls, ID-based host decoding, and configurable binary transport. Other projects emphasize different languages, runtime environments, tracing models, or integrations. These links are starting points for comparison, not a claim that all projects provide the same features or interchangeable formats.

### 2.1. <a id="logging-and-deferred-formatting"></a>Logging and deferred formatting

- [defmt](https://github.com/knurling-rs/defmt): deferred formatting for embedded Rust.
- [Zephyr dictionary-based logging](https://docs.zephyrproject.org/latest/services/logging/index.html#dictionary-based-logging).
- [Memfault Compact Logs](https://docs.memfault.com/docs/mcu/compact-logs).
- [Postform](https://github.com/Javier-varez/Postform).
- [Embedded Logger (elog)](https://github.com/martinribelotta/elog).
- [McuLog](https://mcuoneclipse.com/2020/06/01/mculog-logging-framework-for-small-embedded-microcontroller-systems/).
- [uLog](https://github.com/rdpoor/ulog).
- [NanoLog](https://github.com/PlatformLab/NanoLog).
- [Diagnostic Log and Trace (DLT)](https://github.com/COVESA/dlt-daemon).

### 2.2. <a id="tracing-and-visualization"></a>Tracing and visualization

- [Baical UP7](https://baical.net/up7.html).
- [Pigweed tokenized tracing](https://pigweed.dev/pw_trace_tokenized/).
- [SEGGER SystemView](https://www.segger.com/products/development-tools/systemview/technology/what-is-systemview/).
- [Percepio Tracealyzer](https://percepio.com/tracealyzer/).
- [QP/Spy](https://www.state-machine.com/qtools/qpspy.html).
- [MCUViewer](https://github.com/klonyyy/MCUViewer).
- [RTEdbg](https://github.com/RTEdbg/RTEdbg).
- [Tonbandgerät](https://github.com/schilkp/Tonbandgeraet).
- [Traces](https://github.com/yotamr/traces).

### 2.3. <a id="other-techniques-and-background-reading"></a>Other techniques and background reading

- Plain `printf` over UART, or a project-specific binary logger.
- Hardware trace through Arm ITM/SWO, and [Arm Keil Event Recorder](https://arm-software.github.io/CMSIS-View/latest/evr.html).
- [SEGGER RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/) as a transport; Trice can also [use RTT](rtt.md).
- [Logging with symbols in the Embedonomicon](https://docs.rust-embedded.org/embedonomicon/logging.html).
- [Call-stack logging through function instrumentation](https://dev.to/taugustyn/call-stack-logger-function-instrumentation-as-a-way-to-trace-programs-flow-of-execution-419a).
- [Dynamic printf breakpoints](https://mcuoneclipse.com/2022/02/09/debugging-with-dynamic-printf-breakpoints/).
- [Minimal Structured Logging for Autonomous Vehicles](https://youtu.be/FyJI4Z6jD4w), a talk about another approach.

## 3. <a id="community-and-contributions"></a>Community and contributions

Use [GitHub Discussions](https://github.com/rokath/trice/discussions) for questions, ideas, and integration experience. Search [open and closed issues](https://github.com/rokath/trice/issues?q=is%3Aissue) for known problems and previous resolutions. For a reproducible bug, include the tool version, relevant configuration, command, expected result, and actual output.

Examples, documentation improvements, platform recipes, fixes, and reproducible performance measurements are welcome. [CONTRIBUTING](../../CONTRIBUTING.md) describes the development workflow, [CHANGELOG](../../CHANGELOG.md) records release changes, and the [MIT License](../../LICENSE.md) governs the project. [AUTHORS](../../AUTHORS.md) lists contributors.

## 4. <a id="support-and-sponsoring"></a>Support and sponsoring

If Trice helps your work, you can support it by sharing an example, improving the documentation, reporting a reproducible problem, contributing a fix, or starring the repository. Financial support for continued maintenance is also welcome:

- [GitHub Sponsors](https://github.com/sponsors/rokath/).
- [Buy Me a Coffee](https://buymeacoffee.com/rokath).
- [PayPal](https://www.paypal.com/paypalme/rolfkarlthomas).

These are the support routes configured by the project in [FUNDING.yml](../../.github/FUNDING.yml). Sponsoring is optional; using Trice does not require a donation.

## 5. <a id="project-history"></a>Project history

Developing firmware means to deal also with interrupts and often with timing. How do you check, if an interrupt occurred? OK, increment a counter and display it in a background loop with some printf-like function. What about time measurement? Set a digital output to 1 and 0 and connect a measurement device. Once, developing software for a real-time image processing device, I had no clue where in detail the processing time exploded when the image quality got bad. A spare analog output with a video interrupt synced oscilloscope gave me the needed information, after I changed the analog output on several points in my algorithm. But, hey guys, I want to deal with my programming tasks and do not like all this hassle connecting wires and steer into instruments.

A `printf` is so cool on a PC, developing software there. But an embedded device often cannot use it for performance reasons. My very first attempt was writing the format string `.const` offset together with its values in a FIFO during a log statement and to do the `printf` it in the background. But that is compiler specific. OK the full string address is better but needs buffer space. [Zephyr](https://docs.zephyrproject.org/latest/) for example does something like that calling it "deferred logging".

Then, one day I had the idea to compute short checksums for the format strings in a pre-compile step and to use them as ID in a list together with the format strings. That was a step forward but needed to write a supporting PC program. I did that in C++ in the assumption to get it better done that way. Finally, it worked, but I hated my PC code, as I dislike C++ now because of all its nuts and bolts to handle, accompanied by missing libraries on the next PC. The tool usability was also unhandy and therefore error prone and the need became clear for a full automated solution. Also, what is, if 2 different format strings accidentally generate the same short checksum? There was a way around, but an ID based message filtering will never be possible that way.

The need became clear for controllable IDs and management options. And there was [Go](https://go.dev) now, an as-fast-as-**C** language, easy to learn, promising high programming efficiency and portability. It would be interesting to try it out on a real PC project.

Trying to add tags in form of partial Trice macro names was blowing up the header code amount and was a too rigid design. Which are the right tags? One lucky day I came to the conclusion to handle tags just as format string parts like `"debug:Here we are!\n"` and getting rid of them in the target code this way also giving the user [freedom](../../internal/emitter/lineTransformerANSI.go) to invent any tags.

Another point in the design was the question how to re-sync after data stream interruption, because that happens often during firmware development. Several encodings were tried out and a proprietary escape sequence format and an alternative flexible data format with more ID bits were working reliably but with [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) things got satisfactory. A side result of that trials is the Trice tool option to add different decoders if needed. Now the default Trice message framing is [TCOBSv1](https://github.com/rokath/tcobs) which includes short message compression and this way allows very low transmit bandwidths and/or saves storage, when binary Trice data are stored in Flash memory.

There was a learning **not** to reduce the transmit byte count to an absolute minimum, but to focus more on Trice macro [speed](performance.md) and universality. That led to a double buffer on the target side as an alternative to the ring buffer solution. The actual binary [encoding](binary_encoding.md), allowing alongside user protocols, is the result of the optional target timestamps and location info some users asked for, keeping the target code as light as possible. Float and double number support was implementable for free because this work is done mainly on the host side.

Trice grew, and as it got usable I decided to make it Open Source to say "Thank You" to the community this way.

Learning that Trice is also a [baby girl name](https://www.babynamespedia.com/meaning/Trice), our daughter Ida designed the little girl with the pen symbolizing the Trice macro for recording and the eyeglasses standing for the PC tool Trice visualizing the logs.

![./ref/TriceGirlS.png](../ref/TriceGirlS.png)

## 6. <a id="ideas-for-future-development"></a>Ideas for future development

These are ideas and historical proposals, not release commitments.

### 6.1. <a id="further-context-enrichment-variants"></a>Further Context Enrichment Variants

[Context Enrichment](context_enrichment.md) supports direct Bind log sites and reversible source extensions with `insert/clean`. Bind rejects selected CE sites in wrapper macros or counter-rebase regions. Use an ordinary function, put direct calls on separate source lines, or use `insert/clean` as described under [bind-limits](bind.md#bind-limits).




### 6.2. <a id="improving-the-trice-tool-internal-parser-not-planned-right-now"></a>Improving the Trice Tool Internal Parser (not planned right now)

#### 6.2.1. <a id="trice-internal-log-code-short-description"></a>Trice Internal Log Code Short Description

##### Trice v1.0 Code

> **Hint:** To follow this explanation with the debugger, you can open in VSCode the trice folder, klick the Run & Debug Button or press CTRL-SHIFT-D, select `trice l -p DUMP` and set a breakpoint at `func main()` in [./cmd/trice/main.go](../../cmd/trice/main.go) or directly in `translator.Translate` [./internal/translator/translator.go](../../internal/translator/translator.go).
* In file [./internal/args/handler.go](../../internal/args/handler.go) function `logLoop` calls `receiver.NewReadWriteCloser` and passes the created `rwc` object to `translator.Translate`.
* There `rwc` is used to create an appropriate decode object `dec` passed to `decodeAndComposeLoop`, which uses `func (p *trexDec) Read(b []byte) (n int, err error)` then, doing the byte interpretation.
  * Finally `n += p.sprintTrice(b[n:]) // use param info` is called doing the conversion.
* Read returns a **single** Trice conversion result or a **single** error message in b[:n] or 0 and is called again and again.
* The returned Trice conversion result is the Trice format string with inserted values, but no timestamp, color or location information. The target timestamp, for this single Trice is hold in `decoder.TargetTimestamp`. It is used only when a new log line begins, what is the normal case. If a Trice format string ends not with a newline, the following Trice gets part of the same log line and therefore its target timestamp is discarded. Also additional data like the location information only displayed for the first Trice in a log line containing several Trices. Because the color is inherent to the Trice tag and needs no display space it is attached to the following Trices in a log line as well.
* The after `Read` following `emitter.BanOrPickFilter` could remove the Read result.
* If something is to write, the location information is generated if a new line starts and passed to the
 with `sw := emitter.New(w)` created object `sw.WriteString` method which internally keeps a slice of type `[]string` collecting all parts of an output line.
* The line `p.Line = append(p.Line, ts, p.prefix, sx)` adds host timestamp `ts`, the build `p.prefix` and the "printed Trice" (`sx` is here just the location information) to the line slice.
* In the next step the stored target timestamp `decoder.TargetTimestamp` is printed into a string and added to the Trice line.
* Optionally the Trice ID follows, if desired.
* The in a string printed Trice follows now and if the `sw.WriteString` method detects a `\n` at its end, the configured line suffix (usually `""`) follows and `p.completeLine()` is called then. It passes the line (slice of strings) to `p.lw.WriteLine(p.Line)`, which adds color, prints to the output device and clears the sw.Line slice for the next line.

##### Disadvantages of Trice v1.0 Implementation

* The Reader can only return a **single** Trice, because its byte interface cannot distinguish between Trices anymore.
* The field order (prefix, host stamp, location information, target stamp optional ID, format string, suffix) is hardcoded.
* Trices with several newlines inside the format string cannot deal with tags after an (internal) line break (newline)
* Binary parser needs to hold internal location after one Trice was decoded.
* Only one target timestamp value in a global variable.
* Line writing is not straight forward understandable.

##### Aims for a better implementation

* Read should parse the binary data only and return a Trice struct slice.
* Cycle errors?
* Each Trice struct gets printed in a string.
* The printed string then is split into several strings, which all get the same stamp information or space fields.
* Finally we have a slice of such structs: hs, ts, loc, idString, format string.
* The format string has no newline inside anymore, but has one at the end (usually) or not.
* The struct slice is cyclically passed to a line writer, which writes one line if it can find a format string ending with a newline.

### 6.3. <a id="using-trice-on-servers"></a>Using Trice on Servers

A server can ingest and analyze Trice streams from devices. Using Trice as the server application's own logger is a separate use case and needs evidence of a practical benefit. Claims about speed, energy use, storage, and additional compression require measurements with equivalent retained information.

<!--
* The internet traffic causes many megabytes logfiles, which need storage and are also often transferred by themselves.
* Of course it is possibe to compress them to save space and traffic.
* But if a server generates binary Trice log data directly:
  * The log generation is much faster and demands less energy.
  * An additional compression is not needed, because the Trice internal [TCOBS](https://github.com/rokath/tcobs) already does it in a reasonable way.
  * Less log data are to be transferred.
* To read the (binary) log data, the matching Trice-Id-List *til.json* is needed. Because this has a size of only a few kilobytes, the server can transmit it on request.
* The ~16000 usable IDs may be not enough for big systems. Options:
  1. Change the TREX binary format to
    * use 32- or 48- or 64-bit IDs. The many zeroes are efficiently compressed internally with TCOBS.
    * use the full 16-bit IDs for ~65000 IDs
  2. Use up to 2^16 or 2^32 different *til.json* files and transmit their index in the optional Trice stamp field. This requests minimal code adaptations.
-->

