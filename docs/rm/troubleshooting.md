# Trice Troubleshooting

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Initial Data Transfer Setup Hints](#initial-data-transfer-setup-hints)
* [2. Short Trouble Shooting Hints](#short-trouble-shooting-hints)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## 1. <a id="initial-data-transfer-setup-hints"></a>Initial Data Transfer Setup Hints

If you do not succeed initially, you can try this:

*triceConfig.h*:

```C
#define TriceStamp32 0x44434241 // a fixed value

#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_NONE   // default
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

* Command line with expected output (`-s` is):

```bash
trice log -s -port com1 -v -ts32="att:%08x fix" # enter this (adapted)
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
Nothing shown with `trice -s`  | Check that format strings end with `\n` and/or use `-addNL`

<p align="right">(<a href="#trice-troubleshooting">back to top</a>)</p>

