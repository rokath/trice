# TCOBSv1 Specification

<details>
  <summary>Table of Contents</summary>
  <ol>

<!-- vscode-markdown-toc -->
* 1. [TCOBS Encoding Principle](#TCOBSEncodingPrinciple)
	* 1.1. [Symbols](#Symbols)
		* 1.1.1. [NOP Sigil Byte `N`](#NOPSigilByteN)
		* 1.1.2. [Zero Sigil Byte `Z1`, `Z2`, `Z3`](#ZeroSigilByteZ1Z2Z3)
		* 1.1.3. [Full Sigil Byte `F2`, `F3`, `F4`](#FullSigilByteF2F3F4)
		* 1.1.4. [Repeat Sigil Byte `R2`, `R3`, `R4`](#RepeatSigilByteR2R3R4)
	* 1.2. [TCOBS Encoding](#TCOBSEncoding)
		* 1.2.1. [Simple Encoding Algorithm](#SimpleEncodingAlgorithm)
		* 1.2.2. [Sigil Bytes Chaining](#SigilBytesChaining)
* 2. [TCOBS Software Interface](#TCOBSSoftwareInterface)
	* 2.1. [C Interface and Code](#CInterfaceandCode)
	* 2.2. [Go interface and Code](#GointerfaceandCode)
* 3. [Appendix: Extended Encoding Possibilities](#Appendix:ExtendedEncodingPossibilities)
	* 3.1. [Example: RLE for longer rows of equal bytes (not implemented)](#Example:RLEforlongerrowsofequalbytesnotimplemented)
	* 3.2. [Other Example: Any proposal?](#OtherExample:Anyproposal)
* 4. [Changelog](#Changelog)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

<div id="top"></div>

  </ol>
</details>

##  1. <a name='TCOBSEncodingPrinciple'></a>TCOBS Encoding Principle

###  1.1. <a name='Symbols'></a>Symbols

* `o` = offset bit to next sigil byte

* `101ooooo` NOP  sigil byte **N**: `ooooo` = 0-31
* `001ooooo` Zero sigil byte **Z1**: `ooooo` = 0-31
* `010ooooo` Zero sigil byte **Z2**: `ooooo` = 0-31
* `011ooooo` Zero sigil byte **Z3**: `ooooo` = 0-31
* `110ooooo` Full sigil byte **F2**: `ooooo` = 0-31
* `111ooooo` Full sigil byte **F3**: `ooooo` = 0-31
* `100ooooo` Full sigil byte **F4**: `ooooo` = 0-31
* `00001ooo` Repeat sigil byte **R2**:  `ooo` = 0-7
* `00010ooo` Repeat sigil byte **R3**:  `ooo` = 0-7
* `00011ooo` Repeat sigil byte **R4**:  `ooo` = 0-7
* `00000ooo` reserved bytes: `ooo` = 1-7
* `00000000` forbidden byte  

####  1.1.1. <a name='NOPSigilByteN'></a>NOP Sigil Byte `N`

This does not represent data in the stream and only serves to keep the chain linked. The remaining 5 bits encode the distance to the next sigil (0 <= n <=31).
* N_0 = `101000001`
* ...
* N_31 = `10111111`

####  1.1.2. <a name='ZeroSigilByteZ1Z2Z3'></a>Zero Sigil Byte `Z1`, `Z2`, `Z3`

* This sigil represents 1 to 3 zeroes in the data stream, and is a `00` to `00 00 00` replacement to eliminate zeroes, reduce data and keep the chain linked.
* The remaining 5 bits encode the distance to the next sigil (0 <= n <= 31).
* Z1 = `001ooooo`
  * Z1_0 = `00100000`
  * ...
  * Z1_31 = `00111111`
* ...
* Z3 = `011ooooo`
  * Z3_0 = `01100000`
  * ...
  * Z3_31 = `01111111`

####  1.1.3. <a name='FullSigilByteF2F3F4'></a>Full Sigil Byte `F2`, `F3`, `F4`

* This sigil represents 2 to 4 0xFF in the data stream, and is a `FF FF` to `FF FF FF FF` replacement to reduce data and keep the chain linked.
* The remaining 5 bits encode the distance to the next sigil (0 <= n <= 31).
* F2 = `110ooooo`
  * F2_0 = `11000000`
  * ...
  * F2_31 = `11011111`
* ...
* F4 = `100ooooo`
  * F4_0 = `10000000`
  * ...
  * F4_31 = `10011111`

####  1.1.4. <a name='RepeatSigilByteR2R3R4'></a>Repeat Sigil Byte `R2`, `R3`, `R4`

* This sigil represents 2 to 4 repetitions of previous byte in the data stream, and is a replacement to reduce data and keep the chain linked.
* The remaining 3 bits encode the distance to the next sigil (0 <= n <= 7).
* R2 = `00010ooo`
  * R2_0 = `00010000`
  * ...
  * R2_7 = `00010111`
* ...
* R4 = `00011ooo`
  * R4_0 = `00011000`
  * ...
  * R4_7 = `00011111`

###  1.2. <a name='TCOBSEncoding'></a>TCOBS Encoding

The encoding can be done in a straight forward code on the senders side touching each byte only once.

####  1.2.1. <a name='SimpleEncodingAlgorithm'></a>Simple Encoding Algorithm

* `aa` represents any non-zero and non-FF byte
* `aa aa ...` represents any non-zero and non-FF **equal** bytes

* [x] Easy to implement (fast).
* [x] Longer sequences are possible by repetition.

**EXAMPLES:**

| Unencoded Data             | Encoded Data | Comment     |
| :-                         | :-           | -           |
| `00`                       | `Z1`         |             |
| `00 00`                    | `Z2`         |             |
| `00 00 00`                 | `Z3`         |             |
| `00 00 00  00`             | `Z3 Z1`      | repetition  |
| `00 00 00  00 00`          | `Z3 Z2`      | repetition  |
| `00 00 00  00 00 00`       | `Z3 Z3`      | repetition  |
| `00 00 00  00 00 00  00`   | `Z3 Z3 Z1`   | repetition  |
| ...                        | ...          | repetition  |
| `aa`                       | `aa`         |             |
| `aa aa`                    | `aa aa`      |             |
| `aa aa`                    | `aa N31 aa`  | offset reached 31, so a NOP sigil byte is added |
| `aa aa aa`                 | `aa R2`      |             |
| `aa aa aa aa`              | `aa R3`      |             |
| `aa aa aa aa`              | `aa Nn R3`   | n=8...31, offset exceeds 7, so a NOP sigil byte is inserted |
| `aa aa aa aa aa`           | `aa R4`      |             |
| `aa aa aa aa aa  aa`       | `aa R4 aa`   | repetition  |
| `aa aa aa aa aa  aa aa`    | `aa R4 aa aa`| repetition  |
| ...                        | ...          | repetition  |
| `FF`                       | `FF`         |             |
| `FF`                       | `FF N31`     | offset reached 31, so a NOP sigil byte is added |
| `FF FF`                    | `F2`         |             |
| `FF FF FF`                 | `F3`         |             |
| `FF FF FF FF`              | `F4`         |             |
| `FF FF FF FF  FF`          | `F4 FF`      | repetition  |
| `FF FF FF FF  FF FF`       | `F4 F2`      | repetition  |
| `FF FF FF FF  FF FF FF`    | `F4 F3`      | repetition  |
| `FF FF FF FF  FF FF FF FF` | `F4 F4`      | repetition  |
| ...                        | ...          | repetition  |

* If several encodings possible, the encoder has than the choice.
  * Example: `00 00 00 00` could be encoded `A0 20` (Z3 Z1) or `40 40` (Z2 Z2)
* NOP sigil bytes are logically ignored. They simply serve as link chain elements.

####  1.2.2. <a name='SigilBytesChaining'></a>Sigil Bytes Chaining

* The encoding starts at first buffer address.
* The encoded buffer ends with a sigil byte.
* The decoding then, starts at the sigil byte which is the last of the encoded data.
* The first sigil byte (at the end) carries as offset the byte count between it to the next sigil byte (before it) or to the buffer start.
* If 2 sigil bytes neighbors the offset is 0.
* Encoded examples: (Sn =sigil byte with offset n, by = data byte)

  ```c
  S0 // only one sigil byte like F4 (representing FF FF FF FF)
  by S1 // one byte and a sigil byte like AA R4 representing AA AA AA AA AA
  by by S2 S0 // like AA BB R2 Z2 representing AA BB BB BB 00 00
  by by by by by by by by by S9 S0 S0 by by by S3 // and so on ...
  ```

* Any next sigil byte carries as offset the byte count to the sigil byte before in buffer start direction.

<p align="right">(<a href="#top">back to top</a>)</p>

##  2. <a name='TCOBSSoftwareInterface'></a>TCOBS Software Interface

###  2.1. <a name='CInterfaceandCode'></a>C Interface and Code

* [../tcobs.h](../tcobs.h)
* [..tcobsEncode.c](../tcobsEncode.c)
* [..tcobsDecode.c](../tcobsDecode.c)

###  2.2. <a name='GointerfaceandCode'></a>Go interface and Code

```Go
// TCOBSCEncode a slice of bytes to a null-terminated frame
func TCOBSCEncode(p []byte) []byte)
```

* [../tcobsCEncode.go](../tcobsCEncode.go)

```Go
// TCOBSDecode a null-terminated frame to a slice of bytes
func TCOBSDecode(p []byte) []byte)
```

* [../tcobsDecode.go](../tcobsDecode.go)

<p align="right">(<a href="#top">back to top</a>)</p>

##  3. <a name='Appendix:ExtendedEncodingPossibilities'></a>Appendix: Extended Encoding Possibilities 

* The reserved bytes `00000ooo` with `ooo` = 1-7 are usable in any manner.

###  3.1. <a name='Example:RLEforlongerrowsofequalbytesnotimplemented'></a>Example: RLE for longer rows of equal bytes (not implemented)

It is possible to improve compression by the following means. This complicates the encoder and makes no sense for messages like [*Trice*](https://github.com/rokath/trice) produces. But if user data with long equal byte rows are expected, it can make sense to implement it, when computing power matters and a standard zipping code is too slow.

<!--
| unencoded data             | encoded data | comment     |
| -                          | -            | -           |
| 7 \* `aa`                  | `aa R4 R2`   | addition    |
| 13 \* `aa`                 | `aa R4 R4 R4`| addition    |
| ...                        | ...          | addition    |
-->

* The reserved values `00000ooo` with `ooo` = `001`...`111` are usable for further extended compressing.
* These sigil bytes then have implicit the offset 0. They are only allowed as "right" neighbor of an other sigil byte.
* R = Repetition sigils repeat the data bytes according to their count value, if no M sigil (see below) is right of them.
* Several repetition sigils are added. Examle:
  * `aa R4 R3` = (1 + 4 + 3) \* `aa` = 8 \* `aa`
* M = Multiply sigils multiply their count with the count of the sigil left of them.
* A multiplication between M sigils is possible unlimited times.
* If left of a M sigil a R, Z or F sigil occurs it is also multiplied, but the multiplication chain ends then.
* Examples:
  * `Z2 R3 R4 M8` = ( 2 + 3 + (4 \* 8)) \* `00` = 37 \* `00` 
  * `aa R4 R2 M3 M3` = ( 1 + 4 + (2 \* 3 \*3 ) \* `aa` = 23 \* `aa`
  * `F2 M3 R4 M3 M8 R5` = ( (2 \*3 ) + (4 \* 3 \* 8) + 5 ) ) \* `00` = 107 \* `00`
* The encoder has the choice how to encode. The decoder follows an clear algorithm. 

| Sigil|code |Use count until 21 repetitions| Comment                    |
| -    | -   |  :-:                         | -                          |
|      |`001`|  0                           | reserved                   |
|      |`010`|  0                           | reserved                   |
| RA   |`011`|  6                           | 10 data byte repetitions   |
| M3   |`100`|  3                           | multiply left count with 3 |
| M4   |`101`| 10                           | multiply left count with 4 |
| M5   |`110`|  6                           | multiply left count with 5 |
| M8   |`111`| 10                           | multiply left count with 8 |

These 5 sigils allow a minimum encoded byte count for equal bytes in a row of up over 20.

| Decoded    | TCOBS encoded  | Decoded    | TCOBS encoded   | Decoded    | TCOBS encoded  |
|  -         | -              |  -         | -               |  -         | -              |
|  1 \* `00` | `Z1`           |  1 \* `FF` | `FF`            |  1 \* `aa` | `aa`           |
|  2 \* `00` | `Z2`           |  2 \* `FF` | `F2`            |  2 \* `aa` | `aa aa`        |
|  3 \* `00` | `Z3`           |  3 \* `FF` | `F3`            |  3 \* `aa` | `aa R2`        |
|  4 \* `00` | `Z3 Z1`        |  4 \* `FF` | `F4`            |  4 \* `aa` | `aa R3`        |
|  5 \* `00` | `Z3 Z2`        |  5 \* `FF` | `F4 FF`         |  5 \* `aa` | `aa R4`        |
|  6 \* `00` | `Z3 Z3`        |  6 \* `FF` | `F4 F2`         |  6 \* `aa` | `aa R3 R2`     |
|  7 \* `00` | `Z3 R4`        |  7 \* `FF` | `F4 F4`         |  7 \* `aa` | `aa R4 R2`     |
|  8 \* `00` | `Z2 M4`        |  8 \* `FF` | `F2 M4`         |  8 \* `aa` | `aa R4 R3`     |
|  9 \* `00` | `Z3 M3`        |  9 \* `FF` | `F3 M3`         |  9 \* `aa` | `aa R2 M4`     |
| 10 \* `00` | `Z2 M5`        | 10 \* `FF` | `F2 M5`         | 10 \* `aa` | `aa R3 M3`     |
| 11 \* `00` | `Z1 RA`        | 11 \* `FF` | `F1 RA`         | 11 \* `aa` | `aa RA`        |
| 12 \* `00` | `Z3 M4`        | 12 \* `FF` | `F3 M4`         | 12 \* `aa` | `aa RA aa`     |
| 13 \* `00` | `Z3 RA`        | 13 \* `FF` | `F3 RA`         | 13 \* `aa` | `aa R3 M4`     |
| 14 \* `00` | `Z2 M7`        | 14 \* `FF` | `F2 M7`         | 14 \* `aa` | `aa R3 RA`     |
| 15 \* `00` | `Z3 M5`        | 15 \* `FF` | `F3 M5`         | 15 \* `aa` | `aa R2 M7`     |
| 16 \* `00` | `Z2 M8`        | 16 \* `FF` | `F2 M8`         | 16 \* `aa` | `aa R3 M5`     |
| 17 \* `00` | `Z2 M8 Z1`     | 17 \* `FF` | `F2 M8 FF`      | 17 \* `aa` | `aa R4 M4`     |
| 18 \* `00` | `Z2 M8 R2`     | 18 \* `FF` | `F2 M8 R2`      | 18 \* `aa` | `aa R4 M4 aa`  |
| 19 \* `00` | `Z2 M8 R3`     | 19 \* `FF` | `F2 M8 R3`      | 19 \* `aa` | `aa R4 M4 R2`  |
| 20 \* `00` | `Z2 M8 R4`     | 20 \* `FF` | `F2 M8 R4`      | 20 \* `aa` | `aa R4 M4 R3`  |
| 21 \* `00` | `Z4 M5 Z1`     | 21 \* `FF` | `F3 M5 FF`      | 21 \* `aa` | `aa R4 M5`     |

As said, these extended possibilities are currently **not implemented** and shown just for discussion. A decoder, able to interpret such extension, will decode simple encoding as well.

###  3.2. <a name='OtherExample:Anyproposal'></a>Other Example: Any proposal?

F4 is maybe not use that often and could be used in a completely different way. But this would lead to a different method.

<p align="right">(<a href="#top">back to top</a>)</p>

##  4. <a name='Changelog'></a>Changelog

| Date | Version | Comment |
| - | - | - |
| 2022-MAR-17 | 0.0.0 | Moved from (Trice1.0Specification](https://github.com/rokath/trice/blob/master/docs/TCOBSSpecification.md) and reworked |
| 2022-MAR-17 | 0.1.1 | Clarification |
| 2022-MAR-18 | 0.2.0 | Correction & Simplifocation |
| 2022-MAR-18 | 0.3.0 | Software Interface added |
| 2022-MAR-18 | 0.3.1 | wip TCOBS Encoding |
| 2022-MAR-19 | 0.4.0 | TCOBS Encoding as C-Code in separate file TCOBS.C |
| 2022-MAR-20 | 0.4.1 | Sigil chaining better explained.|
| 2022-MAR-20 | 0.4.2 | Sigil corrected. Now the offset is the byte count between two sigil bytes.|
| 2022-MAR-21 | 0.5.0 | R5 removed |
| 2022-MAR-22 | 0.5.1 | Simple encoding example table extended. |
| 2022-MAR-23 | 0.5.2 | Sigil bytes offset correction, [tcobs.h](../tcobs.h) Link corrected. [tcobs.c](../tcobs.c) Link added|
| 2022-MAR-24 | 0.6.0 | Comment added to preface after talk with Sergii |
| 2022-MAR-24 | 0.6.1 | Smaller corrections |
| 2022-MAR-28 | 0.7.0 | Multiply sigil byte idea added |
| 2022-MAR-28 | 0.7.1 | Multiply sigil byte idea more specified |
| 2022-MAR-29 | 0.8.0 | Document slightly restructured, some comments added |
| 2022-APR-01 | 0.8.1 | Document slightly restructured |
| 2022-APR-02 | 0.8.2 | Preface reworked |
| 2022-MAY-08 | 0.8.3 | Correction: in the worst case 1 additional byte per ~32~ 31 bytes |
| 2022-MAY-22 | 0.8.4 | F4 remark added. Correction: *Trice* \-> message in chapter 2 and 3. |
| 2022-JUL-24 | 0.8.5 | Smaller wording improvements. |
| 2022-JUL-30 | 0.9.0 | Common (v1 & v2) parts removed. |
| 2022-AUG-06 | 0.9.1 | Assumptions moved to TCOBS ReadMe.md |
| 2022-AUG-08 | 0.9.2 | Link corrected |
