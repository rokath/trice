# Future Development

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Further Context Enrichment Variants](#further-context-enrichment-variants)
* [2. Improving the Trice Tool Internal Parser (not planned right now)](#improving-the-trice-tool-internal-parser-not-planned-right-now)
  * [2.1. Trice Internal Log Code Short Description](#trice-internal-log-code-short-description)
    * [2.1.1. Trice v1.0 Code](#trice-v10-code)
    * [2.1.2. Disadvantages of Trice v1.0 Implementation](#disadvantages-of-trice-v10-implementation)
    * [2.1.3. Aims for a better implementation](#aims-for-a-better-implementation)
* [3. Using Trice on Servers](#using-trice-on-servers)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>



<p align="right">(<a href="#future-development">back to top</a>)</p>

## 1. <a id="further-context-enrichment-variants"></a>Further Context Enrichment Variants

[Context Enrichment](trice_context_enrichment.md) supports direct Bind log sites and reversible source extensions with `insert/clean`. Bind rejects selected CE sites in wrapper macros or counter-rebase regions. Use an ordinary function, put direct calls on separate source lines, or use `insert/clean` as described under [bind-limits](trice_bind.md#bind-limits).




<p align="right">(<a href="#future-development">back to top</a>)</p>

## 2. <a id="improving-the-trice-tool-internal-parser-not-planned-right-now"></a>Improving the Trice Tool Internal Parser (not planned right now)

### 2.1. <a id="trice-internal-log-code-short-description"></a>Trice Internal Log Code Short Description

#### 2.1.1. <a id="trice-v10-code"></a>Trice v1.0 Code

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

#### 2.1.2. <a id="disadvantages-of-trice-v10-implementation"></a>Disadvantages of Trice v1.0 Implementation

* The Reader can only return a **single** Trice, because its byte interface cannot distinguish between Trices anymore.
* The field order (prefix, host stamp, location information, target stamp optional ID, format string, suffix) is hardcoded.
* Trices with several newlines inside the format string cannot deal with tags after an (internal) line break (newline) 
* Binary parser needs to hold internal location after one Trice was decoded.
* Only one target timestamp value in a global variable.
* Line writing is not straight forward understandable.

#### 2.1.3. <a id="aims-for-a-better-implementation"></a>Aims for a better implementation

* Read should parse the binary data only and return a Trice struct slice.
* Cycle errors?
* Each Trice struct gets printed in a string.
* The printed string then is split into several strings, which all get the same stamp information or space fields.
* Finally we have a slice of such structs: hs, ts, loc, idString, format string. 
* The format string has no newline inside anymore, but has one at the end (usually) or not.
* The struct slice is cyclically passed to a line writer, which writes one line if it can find a format string ending with a newline.

## 3. <a id="using-trice-on-servers"></a>Using Trice on Servers

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

<p align="right">(<a href="#future-development">back to top</a>)</p>

