# *Trice*  Version 1.0 Specification (Draft)

<!-- vscode-markdown-toc -->
* 1. [Preface](#Preface)
* 2. [Compatibility](#Compatibility)
* 3. [Framing](#Framing)
* 4. [*Trice* ID list `til.json`](#TriceIDlisttil.json)
* 5. [*Trice* location information file `li.json`](#Tricelocationinformationfileli.json)
* 6. [TREX (*Trice* extendable) encoding](#TREXTriceextendableencoding)
	* 6.1. [Symbols](#Symbols)
	* 6.2. [Main stream logs](#Mainstreamlogs)
		* 6.2.1. [*Trice* format](#Triceformat)
		* 6.2.2. [COBS encoding](#COBSencoding)
	* 6.3. [Extended *Trices* as future option](#ExtendedTricesasfutureoption)
	* 6.4. [Unknown user data](#Unknownuserdata)
	* 6.5. [TCOBS *Trice* messages optimized COBS](#TCOBSTricemessagesoptimizedCOBS)
	* 6.6. [Encoding](#Encoding)
		* 6.6.1. [Assumptions](#Assumptions)
		* 6.6.2. [Symbols](#Symbols-1)
		* 6.6.3. [Examples](#Examples)
* 7. [Changelog](#Changelog)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  1. <a name='Preface'></a>Preface

The with name "COBS" branded [current (2022-03) *Trice* encoding](./TriceMessagesEncoding.md) is not optimal concerning the generated data amount:
* See discussion [#253 Save trice COBS encoded data on target and view it later on PC](https://github.com/rokath/trice/discussions/253).
* The location information is transmitted as 16 bit file ID plus 16 bit line number. It is possible to generate during `trice update` an additional file `li.json` containing the location information for each *Trice* ID avoiding the additional 4 bytes this way. But this could cause assignment issues, when the same *Trice* ID is used at different locations (see [https://github.com/rokath/trice/discussions/264](https://github.com/rokath/trice/discussions/264)). But it is possible to drop the option `trice u -sharedIDs`.
* The 32-bit COBS package descriptor is overkill for allowing user data.
* The additional padding bytes to achieve 32 bit sizes are not needed. The user could add them by himself if really needed.
* The 4 timestamp bytes in front of each *Trice* demand the COBS package descriptor. The timestamp should go inside the *Trice* message and be optionally smaller.

##  2. <a name='Compatibility'></a>Compatibility

* The *Trice* v0.48.0 user syntax will remain mainly unchanged. The letter case of the ID codes the target timestamp size. (see below)
* The as "COBS" branded legacy[current (2022-03) *Trice* encoding](.TriceMessageEncoding.md) will stay unchanged as an option for compatibility. But it will not be the default encoding anymore. To use newer **trice** tool versions with legacy projects the CLI switch `-encoding LegacyCOBS` needs to be used.
* The option `-sharedIDs` will be further available but depreciated to avoid location assignment issues.
* Legacy projects which used the option `-sharedIDs` will still work even with a `li.json` file. A several times used ID will get an assignment of one of the locations.
* The issue [#242 Add target context option](https://github.com/rokath/trice/issues/242) could get the label "wontfix". When a task ID is needed, it could be also a data value in such cases.
* The same user source files usable with the legacy *Trice* COBS encoding and the proposed additional [TREX](#TREXTriceextendableencoding) encoding. The `#define TRICE_FILE Id(n)` is ignored when TREX is used.

##  3. <a name='Framing'></a>Framing

Framing is will be done by with [TCOBS](#TCOBSTricemessagesoptimizedCOBS) for data reduction. For robustness each *Trice* gets its own TCOBS package. User data are in separate TCOBS packages. See TCOBS proposal below.

##  4. <a name='TriceIDlisttil.json'></a>*Trice* ID list `til.json`

This file integrates all firmware variants and versions as the legacy **trice** tool does. For the implementation of the optional *Trice* extensions (see below), a `til.json` format extension is needed because several files are unhandy. Both `til.json` formats will be accepted in the future.

##  5. <a name='Tricelocationinformationfileli.json'></a>*Trice* location information file `li.json`

With [TREX](#TREXTriceextendableencoding) encoding the location information needs no transmission anymore but goes not into the `til.json` file. In the field the location information is normally useless and easy outdated. The software developer is the one, mostly interested in the location information. So, if the `li.json` is generated and therefore available, the **trice** tool automatically displays file name and line number. When the firmware left the developer table, only the file `til.json` is of interest. The **trice** tool will silently not display location information, if the `li.json` file is not found. For in-field logging, the usage of option `-showID string` could be used. This allows later an easy location of the relevant source code. Also the planned `-binaryLogfile` option is possible. See [issue #267 Add `-binaryLogfile` option](https://github.com/rokath/trice/issues/267). It allows a replay of the logs and the developer can provide the right version of the `li.json` file.

##  6. <a name='TREXTriceextendableencoding'></a>TREX (*Trice* extendable) encoding

###  6.1. <a name='Symbols'></a>Symbols

* `i` = ID bit
* `I` = `iiiiiiii`
* `n` = number bit
* `s` = selector bit
* `N` = `snnnnnnnn`
* `c` = cycle counter bit
* `C` = s==0 ? `cccccccc` : `nnnnnnnn`
* `t` = timestamp bit
* `T` = `tttttttt`
* `d` = data bit
* `D` = `dddddddd`

###  6.2. <a name='Mainstreamlogs'></a>Main stream logs

All main stream logs share the same 14 bit ID space allowing 1-16383 IDs.

* `11iiiiii I N C  T T T T ...` 14 bit ID, *Trice* format with 32-bit timestamp: TRICE( ID(n), "...", ...), ...
* `10iiiiii I N C  T T ...`     14 bit ID, *Trice* format with 16-bit timestamp: TRICE( Id(n), "...", ...), ...
* `01iiiiii I N C  ...`         14 bit ID, *Trice* format without     timestamp: TRICE( id(n), "...", ...), ...
* The update switch `-timeStamp 32` defaults new ID´s to `ID`.
* The update switch `-timeStamp 16` defaults new ID´s to `Id`.
* The update switch `-timeStamp 0`  defaults new ID´s to `id`.
* The update switch `-timeStamp to32` converts all `id` and `Id` to `ID`.
* The update switch `-timeStamp to16` converts all `id` and `ID` to `Id`.
* The update switch `-timeStamp to0`  converts all `ID` and `Id` to `id`.
* The log switch `-ttsf` is the same as `-ttsf32`.
* There is a new log switch `ttsf16` for the 16 bit timestamps. 

####  6.2.1. <a name='Triceformat'></a>*Trice* format

* Optional data bytes start after optional timestamp.
* N is not u32 count anymore, it is data byte count (without header, without timestamp).
* N > 127 (s==1) tells `N C` is replaced by `1nnnnnnn nnnnnnnn`, allowing 32767 bytes.
  * C is incremented with each *Trice* but not transmitted when:
    * N > 127
    * extended *Trice* without C

####  6.2.2. <a name='COBSencoding'></a>COBS encoding

* Inside double buffer each trice starts at a u32 boundary.
* There are 1-3 padding bytes possible after each *Trice*.
* The COBS encoding skips the padding bytes using N.

###  6.3. <a name='ExtendedTricesasfutureoption'></a>Extended *Trices* as future option

If for special cases, the main stream encoding is not sufficient, the user can add its own encoding.

* `00...` sub-options `TRICEX0`, `TRICEX1`, `TRICEX2`, `TRICEX3`
  * `-ex0 pos -ex1 pos -ex2 pos -ex3 pos`  Select position in extendable table for TRICEXn, 4 codings selectable in one shot.
  * The table is creatable and extendable on demand.
  * For each line an appropriate target and host code needs to be done.
  * Then the target configuration must match the CLI switches.
  * Table example:
    |Position | Encoding                          | Remarks                                       |
    | -   | -                                     | -                                             |
    | pos | `00nniiii I D D`                      | 12 bit ID, no timestamp, 1x 16 bit data       |
    | pos | `00nniiii I D D`                      | 12 bit ID, no timestamp, 2x 8 bit data        |
    | pos | `00nniiii I D D D D`                  | 12 bit ID, no timestamp, 2x 16 bit data       |
    | pos | `00nniiii I T T  D D`                 | 12 bit ID, 16 bit timestamp,1x16 bit data     |
    | pos | `00nniiii I T T  D D D D`             | 12 bit ID, 16 bit timestamp, 1x32 bit data    |
    | pos | `00nniidd dddddddd`                   | 2 bit ID & 1x 10 bit data                     |
    |   6 | `00nndddd dddddddd`                   | no ID, 12 bit data as a 5 and a 7 bit value   |
    | pos | `00nndddd`                            | no ID, 4x 1 bit data                          |
    | pos | `00nniiii I`                          | 12 bit ID, no data                            |
    | pos | `00nniiii D D`                        | 4 bit ID, 1x 16 bit data                      |
    | pos | `00nniiii I D D`                      | 12 bit ID, 2x 8 bit data                      |
    | pos | `00nniiii I T T`                      | 12 bit ID, 16 bit timestamp, no data          |
    | pos | `00nniiii D D T`                      | ...                                           |
    |  13 | `00nniiii C T T  D D D D`             | 8 bit cyle counter, 16 bit timestamp, a float |
    | pos | `00nniiii C D D  T T T T`             | ...                                           |
    | pos | `00nniiii I D D  T`                   | ...                                           |
    | pos | `00nniiii tttttttt ttttdddd dddddddd` | 4 bit ID, 12 bit timestamp, 12 bit data       |
    | ... | `00nn...`                             | ...                                           |
  * Examples:
    * `-ex0 13` means TRICEX0 = `0000iiii C T T  D D D D`.
      * Usage: `TRICEX0( "result %f\n", aFloat(v));`
    * `-ex2 6`  means TRICEX2 = `0010dddd dddddddd`.
      * Usage: `TRICEX2( "point %x,%d\n", a, b);`
  * *Trice* extensions without cycle counter are counted as well.
  * Each TRICEXn has its own ID space.

###  6.4. <a name='Unknownuserdata'></a>Unknown user data

* Unknown user data are possible as part of the *Trice* extensions.
  * Without the `-ex0` switch, `0000...` packages are ignored as unknown user data.
  * Without the `-ex1` switch, `0001...` packages are ignored as unknwno user data.
  * Without the `-ex2` switch, `0010...` packages are ignored as unknown user data.
  * Without the `-ex3` switch, `0011...` packages are ignored as unknown user data.
* So, if *Trice* extensions not used, all `00...` packages are ignored as unknown user data.
* Unknown user data have an unknown length. Therefore they cannot share a COBS packet with *Trices*.
* Unknown user data packets do not affect the cycle counter. The can have their own cycle counter.

###  6.5. <a name='TCOBSTricemessagesoptimizedCOBS'></a>TCOBS *Trice* messages optimized COBS

This is inspired by [rlercobs](https://docs.rs/kolben/0.0.3/kolben/rlercobs/index.html) with focus speed over compression.

###  6.6. <a name='Encoding'></a>Encoding

* Encoding could be forward or reverse, what does not matter, because streaming data are not expected.
* Chained sigil bytes are used.
* Each package starts (forward) or ends (reverse) with a sigil byte.

####  6.6.1. <a name='Assumptions'></a>Assumptions

* Most *Trices* consist of 16 or less bytes.
* Some *Trices* or user data are longer.
* Zero is the most common byte (example:`00 00 00 05`) but other bytes could be repeated too.

####  6.6.2. <a name='Symbols-1'></a>Symbols

* `o` = offset bit to next sigil byte
* `n` = number bit
* NOP sigil byte **N**: `00oooooo` = N(1 ... 63), `00000000` is forbidden.
  * This does not represent data in the stream and only serves to keep the chain linked.
  * The remaining 6 bits encode the distance to the next sigil (1 <= n <= 63).
* Zero sigil byte **Zn** = `1nnnoooo` = Z1-Z8 = Zn(1-16), 0000=16.
  * This sigil represents 1 to 8 zeroes in the data stream, and is a replacement to reduce data and keep the chain linked.
  * The remaining 4 bits encode the distance to the next sigil (1 <= n <= 16), 0000=16.
  * Z1 = Zn with nnn = 000: `1000oooo`, Z1(1-16)
  * ...
  * Z8 = Zn with nnn = 111: `1111oooo`, Z8(1-16)
* Repeat sigil byte **Rn**: `01nnoooo` stays for 2-5 repetitions
  * This sigil represents 2 to 5 repetitions of previous byte in the data stream, and is a replacement to reduce data and keep the chain linked.
  * The remaining 4 bits encode the distance to the next sigil (1 <= n <= 16), 0000=16
  * R2 = R with nn = 00: `0100oooo`, R2(1-16)
  * ...
  * R5 = R with nn = 11: `0111oooo`, R5(1-16)

####  6.6.3. <a name='Examples'></a>Examples

* Zero bytes:
  *  1 * 00 = Z1
  * ...
  *  8 * 00 = Z8
  *  9 * 00 = Z8 Z1
  * ...
  * 16 * 00 = Z8 Z8 
  * 17 * 00 = Z8 Z8 Z1
  * ...
  * 23 * 00 = Z8 Z8 Z7
  * 24 * 00 = Z8 R2
  ...
  * 39 * 00 = Z8 R3 Z7
  * 40 * 00 = Z8 R4
  * 41 * 00 = Z8 R4 Z1
  ...
  * 49 * 00 = Z8 R4 Z8 Z1
  * ...

* non zero bytes:
  *  1 * aa = aa
  *  2 * aa = aa aa
  *  3 * aa = aa R2
  * ...
  *  6 * aa = aa R5
  *  7 * aa = aa R5 aa
  *  8 * aa = aa R5 aa aa
  * ...
  * 12 * aa = aa R5 aa R5
  * ...

##  7. <a name='Changelog'></a>Changelog

| Date | Version | Comment |
| - | - | - |
| 2022-MAR-15 | 0.1 | Initial Draft |
- [*Trice*  Version 1.0 Specification (Draft)](#trice--version-10-specification-draft)
  - [1. <a name='Preface'></a>Preface](#1-preface)
  - [2. <a name='Compatibility'></a>Compatibility](#2-compatibility)
  - [3. <a name='Framing'></a>Framing](#3-framing)
  - [4. <a name='TriceIDlisttil.json'></a>*Trice* ID list `til.json`](#4-trice-id-list-tiljson)
  - [5. <a name='Tricelocationinformationfileli.json'></a>*Trice* location information file `li.json`](#5-trice-location-information-file-lijson)
  - [6. <a name='TREXTriceextendableencoding'></a>TREX (*Trice* extendable) encoding](#6-trex-trice-extendable-encoding)
    - [6.1. <a name='Symbols'></a>Symbols](#61-symbols)
    - [6.2. <a name='Mainstreamlogs'></a>Main stream logs](#62-main-stream-logs)
      - [6.2.1. <a name='Triceformat'></a>*Trice* format](#621-trice-format)
      - [6.2.2. <a name='COBSencoding'></a>COBS encoding](#622-cobs-encoding)
    - [6.3. <a name='ExtendedTricesasfutureoption'></a>Extended *Trices* as future option](#63-extended-trices-as-future-option)
    - [6.4. <a name='Unknownuserdata'></a>Unknown user data](#64-unknown-user-data)
    - [6.5. <a name='TCOBSTricemessagesoptimizedCOBS'></a>TCOBS *Trice* messages optimized COBS](#65-tcobs-trice-messages-optimized-cobs)
    - [6.6. <a name='Encoding'></a>Encoding](#66-encoding)
      - [6.6.1. <a name='Assumptions'></a>Assumptions](#661-assumptions)
      - [6.6.2. <a name='Symbols-1'></a>Symbols](#662-symbols)
      - [6.6.3. <a name='Examples'></a>Examples](#663-examples)
  - [7. <a name='Changelog'></a>Changelog](#7-changelog)
<!--
Module kolben::rlercobs
[−][src]

Run Length Encoded Reverse Cobs

This works very similarly to rcobs, however the behavior of sigil bytes are slightly changed.
Sigils

There are now four different kind of sigil bytes, defined by their two msbits:

    0b00 - NOP sigil.
        This does not represent data in the stream, and only serves to keep the reverse chain linked
        The remaining six bits encode the distance back to the previous sigil (1 <= n <= 63)
    0b01 - Zero sigil.
        This sigil represents a zero in the data stream, and has been replaced to preserve framing
        The remaining six bits encode the distance back to the previous sigil (1 <= n <= 63)
    0b10 - Exponential Repeat sigil.
        This sigil is a directive to repeat the previous non-sigil character (or Zero sigil representing a data-zero) 2 ** n times, where 3 <= n <= 10.
        If multiple repeats (exponential or linear) appear in a row, their repeating counts should be added together.
        The remaining three bits encode the distance back to the previous sigil (1 <= n <= 7)
    0b11 - Linear Repeat sigil.
        This sigil is a directive to repeat the previous non-sigil character (or Zero sigil representing a data-zero) n times, where 1 <= n <= 7.
        If multiple repeats (exponential or linear) appear in a row, their repeating counts should be added together.
        The remaining three bits encode the distance back to the previous sigil (1 <= n <= 7)

All sigil types encode the number of bytes back until the next sigil, and all messages must end with a sigil. This allows for decoding by walking the data stream backwards, which is done to preserve encoder simplicity.
-->





<!-- ## Small Size Logs (ideas)


* `01iiiiii I N C  ...` 14 bit ID, Legacy COBS no timestamp inside: TRICE
* `0001iiii I D D`      12 bit ID, no timestamp, 16 bit data: TRICE82, TRICE161
* `0010iiii I D D D D`  12 bit ID, no timestamp, 32 bit data: TRICE84, TRICE162, TRICE8216, TRICE1682, TRICE321

* `11iiiiii I N C  T T T T ...` 14 bit ID, Legacy COBS with timestamp inside: TTRICE
* `1001iiii I T T  D D`     12 bit ID, 16 bit timestamp, 16 bit data: TTRICE82, TTRICE161
* `1010iiii I T T  D D D D` 12 bit ID, 16 bit timestamp, 32 bit data: TTRICE84, TTRICE162, TTRICE8216, TTRICE1682, TTRICE321

* `0011....` sub-option -x n TRICEX
* `1011....` sub-option -y n TRICEY
* examples: s select x or y
  * `s011iidd dddddddd` 2 bit ID & 10 bit data
  * `s011dddd dddddddd` no ID, 12 bit data
  * `s011dddd` no ID, 4 bit data
  * `s011iiii I` 12 bit ID, no data
  * `s011iiii D D` 4 bit ID, 16 bit data
  * `s011iiii I D D` 12 bit ID, 16 bit data
  * `s011iiii I T T` 12 bit ID, 16 bit timestamp, no data
  * `s011iiii D D T`
  * `s011iiii C T T  D D D D`
  * `s011iiii C D D  T T T T`
  * `s011iiii I D D  T`
  * `s011iiii tttttttt ttttdddd dddddddd` 4 bit ID, 12 bit timestamp, 12 bit data
  * ...
* `0000....` user data with defined length or length code
* `1000....` reserved
-->
