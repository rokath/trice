# Troubleshooting and Limits

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Initial Data Transfer Setup Hints](#initial-data-transfer-setup-hints)
* [2. Short Trouble Shooting Hints](#short-trouble-shooting-hints)
* [3. Cycle Counter](#cycle-counter)
* [4. Permanent Limitations](#permanent-limitations)
  * [4.1. Limitation TRICE in TRICE not possible](#limitation-trice-in-trice-not-possible)
* [5. Source syntax and workflow limits](#source-syntax-and-workflow-limits)
* [6. Argument limits](#argument-limits)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

Start with the [UM's first checks](../TriceUserManual.md#if-something-does-not-work). For generated-header errors use [Bind diagnostics](bind.md#diagnostics-and-troubleshooting). This chapter covers deeper transfer checks and shared call limitations.

## 1. <a id="initial-data-transfer-setup-hints"></a>Initial Data Transfer Setup Hints

For a raw-byte diagnostic, the following uses a deliberately fixed numeric ID. Collect this explicit-ID specimen into a matching TIL with `trice add`; it is not the normal Bind application pattern. Do not add it to an already bound source file.

*triceConfig.h*:

```C
#define TriceStamp32 0x44434241 // a fixed value

#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_NONE   // disable framing for this diagnostic
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_NONE // no framing to interpret the byte stream manually

```

*main.c*:

```C
int main( void) {
    // system init...
    TriceInit();
    TRice(iD(170), "Fun %x!\n", 0xadded ); // with "fixed" iD(170), 32-bit stamp, and with `\n`
    TriceTransfer(); // call cyclically for deferred mode
    // system run ...
}
```

* Command line with expected output (`-s` shows the Trice ID):

```bash
trice log -s -port com1 -pf none -v -ts32="att:%08x fix" # enter this (adapted)
#       /-------------------------------------- ID low byte (170)
#       |  /----------------------------------- ID high byte (6 bits=0) with 2 most significant bits set (32-bit stamp follows)
#       |  |       /--------------------------- 32-bit (time) stamp
#       |  |       |      /-------------------- initial cycle counter: 192
#       |  |       |      |  /----------------- payload size
#       |  |       |      |  |       /--------- payload (0x00added) 
#       |  |       |      |  |       |      / - 0-delimiter or next Trice
#       |  |       |      |  |       |      |
#       v  v  vvvvvvvvvvv v  v  vvvvvvvvvvv v
# Input(aa c0 41 42 43 44 c0 04 ed dd 0a 00 ... ) # expected byte stream
# ...
#              main.c    84 44434241 fix   170 Fun added!
# ...
```

* If you receive something different, you have to debug your system.
* To interpret the bytes see Trice Binary encoding chapter.
  * `33 ff` ID as 16-bit little endian
    * `33` low part of ID 0x3333
    * `ff` high part if ID 0x3333 - the 6 least significant bits ored with 0b11000000 to signal a 32-bit timestamp
  * `41 42 43 44` 32-bit timestamp, usually as little endian
  * `c0` cycle counter, initial value is 192
  * `04` parameter size
  * `22 22 22 22` 4 parameter bytes

## 2. <a id="short-trouble-shooting-hints"></a>Short Trouble Shooting Hints

Problem                        | Hint
--------------------------------|------------------------------------------------------------------------------------------------
Missing `objcopy` in macOS     | `brew install binutils`
Small GUI Editor for macOS     | `brew install cotedit` Usage: `cot` (not as root)
Small In-Terminal Editor Linux | https://cte.e10labs.com/, tilde, micro, joe, https://craigbarnes.gitlab.io/dte/ (also as root)
Nothing shown with `trice log -s`  | Check that format strings end with `\n` and/or use `-addNL`

## 3. <a id="cycle-counter"></a>Cycle Counter

* The trice tool expects the first cycle counter to start with 0xC0 (=192). If the target is already running and you connect the trice tool then, the first message is marked with "CYCLE: ? not equal expected value 192 - adjusting. Now 1 CycleEvents".
* If the target is resetted asynchronous, the trice tool receives a cycle counter 192. Most probably the last cycle counter was not 191, so this triggers also a message  with "CYCLE: 192 not equal expected value ?- adjusting. Now n CycleEvents".
* In the Trice tool is some heuristics to suppress such obvious false positives.

## 4. <a id="permanent-limitations"></a>Permanent Limitations

### 4.1. <a id="limitation-trice-in-trice-not-possible"></a>Limitation TRICE in TRICE not possible

* No-Good Example:

```C
int f0( void ){ TRICE( "msg:f0\n"); return 0; }
void f1( void ){ TRICE( "No; %d", f0() ); }
```

* This will compile normally but corrupt TRICE output.

`f1()` creates the "No" record header, then evaluates `f0()`, which emits another record before the "No" tail is written. The interleaved records corrupt the output despite successful compilation.

* Workaround:

```C
int f0( void ){ TRICE( "msg:f0\n"); return 0; }
void f1( void ){ int x = f0(); TRICE( "Yes: %d", x ); }
```

## 5. <a id="source-syntax-and-workflow-limits"></a>Source syntax and workflow limits

Bind, Insert and CE have different rules for several calls on one line and calls inside wrappers. Use the [support tables](context_enrichment.md#supported-log-sites-and-alternatives) rather than assuming one rule for all workflows. [Bind limits](bind.md#bind-limits) also cover compiler requirements and generated includes.

Trice scans source text rather than running the C preprocessor. Write a complete supported literal format at the log site; do not rely on token pasting to build it. For example, `TRICE("msg:" ## "Hello\\n")` is not a supported way to specify a format. Historical Insert parser examples are retained in the [legacy reference](insert_clean_cache.md#legacy-parser-examples).

## 6. <a id="argument-limits"></a>Argument limits

Scalar calls have at most twelve values of one selected width. For larger data use [buffer macros](printf_comparison.md#runtime-generated-buffer-transfer-with-triceb). Floats need `aFloat()`; doubles need `aDouble()` and a 64-bit macro. Runtime strings use `triceS` or `triceN`. Full examples and supported formats are in [Writing Trice Messages](printf_comparison.md).

