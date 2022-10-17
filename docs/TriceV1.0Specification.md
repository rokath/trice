
<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>

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
		* 6.2.2. [Framing with TCOBS encoding](#FramingwithTCOBSencoding)
	* 6.3. [Extended *Trices* as future option](#ExtendedTricesasfutureoption)
	* 6.4. [Unknown user data](#Unknownuserdata)
* 7. [ID Management](#IDManagement)
* 8. [Changelog](#Changelog)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->
<div id="top"></div>

  </ol>
</details>

# *Trice*  Version 1.0 Specification (Draft)

##  1. <a name='Preface'></a>Preface

The with name "COBS" branded [*Trice* v0.48.0 encoding](./TriceMessagesEncoding.md) is not optimal concerning the generated data amount:
* See discussion [#253 Save trice COBS encoded data on target and view it later on PC](https://github.com/rokath/trice/discussions/253).
* The location information is transmitted as 16 bit file ID plus 16 bit line number. It is possible to generate during `trice update` an additional file `li.json` containing the location information for each *Trice* ID avoiding the additional 4 bytes this way. But this could cause assignment issues, when the same *Trice* ID is used at different locations (see [https://github.com/rokath/trice/discussions/264](https://github.com/rokath/trice/discussions/264)). But it is possible to drop the option `trice u -sharedIDs`.
* The 32-bit COBS package descriptor is overkill for allowing user data.
* The additional padding bytes to achieve 32 bit sizes are not needed. The user could add them by himself if really needed.
* The 4 timestamp bytes in front of each *Trice* demand the COBS package descriptor. The timestamp should go inside the *Trice* message and be optionally smaller.

<p align="right">(<a href="#top">back to top</a>)</p>

##  2. <a name='Compatibility'></a>Compatibility

* The *Trice* v0.48.0 user syntax will remain mainly unchanged. The letter case of the ID codes the target timestamp size. (see below)
* The as "COBS" branded legacy [v0.48.0 *Trice* encoding](.TriceMessageEncoding.md) will stay unchanged as an option for compatibility. But it will not be the default encoding anymore. To use newer **trice** tool versions with legacy projects the CLI switch `-encoding TLE` needs to be used.
* The option `-sharedIDs` will be further available but depreciated to avoid location assignment issues.
* Legacy projects which used the option `-sharedIDs` will still work even with a `li.json` file. A several times used ID will get an assignment of one of the locations.
* The issue [#242 Add target context option](https://github.com/rokath/trice/issues/242) could get the label "wontfix". When a task ID is needed, it could be also a data value in such cases.
* The same user source files usable with the legacy *Trice* COBS encoding and the proposed additional [TREX](#TREXTriceextendableencoding) encoding.
* 
<p align="right">(<a href="#top">back to top</a>)</p>

##  3. <a name='Framing'></a>Framing

Framing will be done with [TCOBS](./TCOBSSpecification.md) for data reduction. As a pre-v1.0 version normal COBS will be used. For robustness each *Trice* gets its own TCOBS package. User data are in separate TCOBS packages encoded. When *Trices* are accumulated in a double half buffer, their separation in TCOBS packages is possible until the first extended *Trice*. Because of the generally unknown extended *Trice* length from this point, all following *Trices* in this half buffer need to go in one TCOBS package (including optional padding bytes) what is ok. The only disadvantage with this is, that in case of a data disruption at this place, several *Trice* messages can get lost.

Possible better implementation: See issue [#290](https://github.com/rokath/trice/issues/290)

<p align="right">(<a href="#top">back to top</a>)</p>

##  4. <a name='TriceIDlisttil.json'></a>*Trice* ID list `til.json`

This file integrates all firmware variants and versions as v0.48.0 does. For the implementation of the optional *Trice* extensions (see below), a `til.json` format extension is needed because several files are unhandy. Both `til.json` formats will be accepted in the future.

<p align="right">(<a href="#top">back to top</a>)</p>

##  5. <a name='Tricelocationinformationfileli.json'></a>*Trice* location information file `li.json`

With [TREX](#TREXTriceextendableencoding) encoding the location information needs no transmission anymore but goes not into the `til.json` file. In the field the location information is normally useless and easy outdated. The software developer is the one, mostly interested in the location information. So, if the `li.json` is generated and therefore available, the **trice** tool automatically displays file name and line number. When the firmware left the developer table, only the file `til.json` is of interest. The **trice** tool will silently not display location information, if the `li.json` file is not found. For in-field logging, the usage of option `-showID string` could be used. This allows later an easy location of the relevant source code. Also the planned `-binaryLogfile` option is possible. See [issue #267 Add `-binaryLogfile` option](https://github.com/rokath/trice/issues/267). It allows a replay of the logs and the developer can provide the right version of the `li.json` file.

<p align="right">(<a href="#top">back to top</a>)</p>

##  6. <a name='TREXTriceextendableencoding'></a>TREX (*Trice* extendable) encoding

###  6.1. <a name='Symbols'></a>Symbols

* `i` = ID bit
* `I` = `iiiiiiii` = ID byte
* `n` = number bit
* `s` = selector bit
* `N` = `snnnnnnnn` = number byte
* `c` = cycle counter bit
* `C` = s==0 ? `cccccccc` : `nnnnnnnn` = cycle counter byte or number byte extension
* `t` = timestamp bit
* `T` = `tttttttt` = timestamp byte
* `d` = data bit
* `D` = `dddddddd` = data byte
* `...` = 0 to 32767 data bytes
* `"..."` = format string

###  6.2. <a name='Mainstreamlogs'></a>Main stream logs

* [x] A 32-bit wide *Trice* storage access is implemented for speed. 
* [x] All main stream logs share the same 14 bit ID space allowing 1-16383 IDs in 3 variants parallel usable:

| 16-bit groups                  | *Trice* code                 | Comment                                         |
| :-                             | -                            | -                                               |
| `011iiiiiI NC  ...`            | `TRICE( id(n), "...", ...);` | *Trice* format without     stamp |
| `101iiiiiI TT NC ...`          | `TRICE( Id(n), "...", ...);` | *Trice* format with 16-bit stamp |
| `111iiiiiI TT TT NC ...`       | `TRICE( ID(n), "...", ...);` | *Trice* format with 32-bit stamp |
| `001iiiiiI TT TT TT TT NC ...` | `TRICE( iD(n), "...", ...);` | *Trice* format with 64-bit stamp |
| `000xxxxX ...`                 | `TRICE( ii(n), "...", ...);` | *Trice* extended type 0 format |
| `010xxxxX ...`                 | `TRICE( iI(n), "...", ...);` | *Trice* extended type 1 format |
| `100xxxxX ...`                 | `TRICE( Ii(n), "...", ...);` | *Trice* extended type 2 format |
| `110xxxxX ...`                 | `TRICE( II(n), "...", ...);` | *Trice* extended type 3 format |

* This leaves 13 ID bits allowing numbers 1-8191.
* Technically it is possible to have distinct ID spaces for each ID type but this would give no real advantage and complicate the handling only.
* [x] For straight forward runtime code, the identifiers `id`, `Id` and `ID` are sub macros:
  * [x] **id(n)**
  
  ```c
  #define id(n) TRICE_PUT16(  0x4000|(n));
  ```

  * [x] **Id(n)**

  ```c
  #define Id(n) { uint16_t ts = TRICE_READ_TICK16; TRICE_PUT((0x80008000|((n)<<16)|(n))); TRICE_PUT16(ts); }
  ```

  * The tyId value `10iiiiiiI` is stored in one step twice, to achieve also a 32-bit boundary after storing `NC` later: `10iiiiiiI 10iiiiiiI TT NC`. The first 2 bytes are not transmitted.

  * [x] **ID(n)**

  ```c
  #define ID(n) { uint32_t ts = TRICE_READ_TICK32; TRICE_PUT16(  0xC000|(n)); TRICE_PUT1616(ts); }
  ```

* [ ] Example:
  * Code:

  ```c
  //! TRICE8_1 writes trice data as fast as possible in a buffer.
  //! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
  //! \param v0 a 8 bit bit value
  #define TRICE8_1( id, pFmt, v0 ) \
    TRICE_ENTER id; CNTC(1); \
    TRICE_PUT( (uint8_t)(v) ); /* little endian*/ \
    TRICE_LEAVE
  ```

  * Transfer package (decoded little endian)

  ```b
  e3 // lower byte of 14-bit ID byte
  ae // 2 msb = 10 = typeT2 plus 6 bit upper ID byte
  ec // lower byte 16-bit timestamp
  02 // upper byte 16-bit timestamp
  64 // cycle counter
  01 // parameter count
  ff // parameter value
  ```

* [x] New *Trice* macros are writable without the ID, so when `trice u` is executed a CLI switch controls the ID type selection:
  * The update switch `-timeStamp 32` defaults new ID´s to `ID`.
  * The update switch `-timeStamp 16` defaults new ID´s to `Id`.
  * The update switch `-timeStamp 0`  defaults new ID´s to `id`.
  * The update switch `-timeStamp to32` converts all `id` and `Id` to `ID`.
  * The update switch `-timeStamp to16` converts all `id` and `ID` to `Id`.
  * The update switch `-timeStamp to0`  converts all `ID` and `Id` to `id`.
* [x] `trice log`:
  * The log switch `-ttsf` is the same as `-ttsf32`.
  * There is a new log switch `ttsf16` for the 16 bit timestamps.
  * The `trice` tool alignes *Trice` messages with different timestamp sizes.

####  6.2.1. <a name='Triceformat'></a>*Trice* format

* Parameter data bytes start after the optional timestamp.
* N is the parameter data bytes count. Padding bytes are not counted.
* Usually N is < 127 but for buffer or string transfer N can get up to 32767 (15 bits).
* When N > 127 (s==1) `NC` is replaced by `1nnnnnnn nnnnnnnn`. C is incremented with each *Trice* but not transmitted when:
  * N > 127
  * extended *Trice* without C

####  6.2.2. <a name='FramingwithTCOBSencoding'></a>Framing with TCOBS encoding

* For maximum storage speed each **trice** message starts at a 32-bit boundary and has 1-3 padding bytes.
* In direct mode only a single message needs handling.
* In deferred mode after half buffer swap any count of trice messages is in the buffer.
* There are different policies possible:
  1. **TRICE_FAST_MULTI_MODE**: Compact buffer by removing all padding bytes, encode it as a single package, append one 0-delimiter and transmit. This allows to reduce the transmitted data amount by paying the price of possibly more data lost in case of an error. Also the data interpretation can perform less checks.
  2. **TRICE_SAFE_SINGLE_MODE**: Encode each buffer separate, append a 0-delimiter for each and pack them all together before transmitting. This increases the transmit data slightly but minimizes the amount of lost data in case of a data disruption.

###  6.3. <a name='ExtendedTricesasfutureoption'></a>Extended *Trices* as future option

If for special cases, the main stream encoding is not sufficient, the user can add its own encoding.

See issue #313 for an other option. The user can write `TRICE16( 7, "%d,%d,%d", x, y, z);` For example 
the iD7 macro Packs the 3 16-bit values x, y, z as packed 12-bit, 12-bit, 8-bit into a 32-bit payload.
Additionally only 2 IDs (1 bit) are needed without cycle and count:
`00sssii dddddddd dddddddd dddddddd dddddddd`
* `00...` sub-options `TRICEX0`, `TRICEX1`, `TRICEX2`, `TRICEX3`
  * `-ex0 pos -ex1 pos -ex2 pos -ex3 pos`  Select position in extendable table for TRICEXn, 4 coding's selectable in one shot.
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
* Several different *Trice* extensions are contributive.

###  6.4. <a name='Unknownuserdata'></a>Unknown user data

* Unknown user data are possible as part of the *Trice* extensions.
  * Without the `-ex0` switch, `0000...` packages are ignored as unknown user data.
  * Without the `-ex1` switch, `0001...` packages are ignored as unknown user data.
  * Without the `-ex2` switch, `0010...` packages are ignored as unknown user data.
  * Without the `-ex3` switch, `0011...` packages are ignored as unknown user data.
* So, if *Trice* extensions not used, all `00...` packages are ignored as unknown user data.
* Unknown user data have an unknown length. Therefore they cannot share a COBS packet with *Trices*.
* Unknown user data packets do not affect the cycle counter. The can have their own cycle counter.

##  7. <a name='IDManagement'></a>ID Management

* The IDs inside the source code are a "dealbreaker" as [bora](https://community.memfault.com/u/bora) mentioned in his [comment](https://interrupt.memfault.com/blog/trice). In fact it is not acceptable for library code used in several projects. An improved approach could look like this:

```c
TRICE( NOTS, "...", ...); // a trice without timestamp
TRICE( TS16, "...", ...); // a trice with a 16-bit timestamp
TRICE( TS32, "...", ...); // a trice with a 32-bit timestamp
TRICE( MOD7, "...", ...); // a trice with a user mode 7 encoding
```

```c
TRICE( S0, "...", ...); // a trice without stamp
TRICE( S2, "...", ...); // a trice with a 16-bit stamp
TRICE( S4, "...", ...); // a trice with a 32-bit stamp
TRICE( S8, "...", ...); // a trice with a 64-bit stamp
```

```c
TRICE( X0, "...", ...); // an extended type 0 trice 
TRICE( X1, "...", ...); // an extended type 1 trice 
TRICE( X2, "...", ...); // an extended type 2 trice 
TRICE( X3, "...", ...); // an extended type 3 trice 
```

* When editing, the user needs to write only `TRICE( "...", ...);` and the trice tool inserts a T0, T2 or T4 automatically according to the used `-timeStamp` switch parameter.
* After repository check-out and before compiling, following substitutions are done using `trice -u`:
  * `TRICE( NOTS, "...", ...);` → `TRICE( id(0), "...", ...);` → `TRICE( id(12345), "...", ...);`
  * `TRICE( TS16, "...", ...);` → `TRICE( Id(0), "...", ...);` → `TRICE( Id(12345), "...", ...);`
  * `TRICE( TS32, "...", ...);` → `TRICE( ID(0), "...", ...);` → `TRICE( ID(12345), "...", ...);`
  * `TRICE( MOD7, "...", ...);` → `TRICE( iD7(0), "...", ...);` → `TRICE( ID(12345), "...", ...);`
* After compiling and before repository check-in, following substitutions are done using `trice -z`:
  * `TRICE( id(12345), "...", ...);` → `TRICE( id(0), "...", ...);` → `TRICE( NOTS, "...", ...);`
  * `TRICE( Id(12345), "...", ...);` → `TRICE( Id(0), "...", ...);` → `TRICE( TS16, "...", ...);`
  * `TRICE( ID(12345), "...", ...);` → `TRICE( ID(0), "...", ...);` → `TRICE( TS32, "...", ...);`
  * `TRICE( id7(12345), "...", ...);` → `TRICE( iD7(0), "...", ...);` → `TRICE( MOD7, "...", ...);`
* The project specific `til.json` contains all IDs and during `trice u` the same IDs are used again for the same **trice** statement. For new or modified **trices** new IDs a chosen and `til.json` is extended as usual.
* Identical **trices** should have different IDs for the correctness of the location information. The switch `-sharedIDs` is obsolete and depreciated.
* There is no guaranty each **trice** gets its old ID back, if for example 5 identical **trices** with different IDs exist, but the probability for an exact restore can made high using the previous `li.json` file. Proposed method:
  * When `trice -u` is executed, the previous `li.json` is read into an internal `li_1.map` and `li.json` is reset to be an empty file and that is red into `li.map`.
  * The `til.json` is read into a `lu` as already done, but the reversal `tflu` list format gets an ID slice assigned to each *trice*.
  * **Trices** occurring only once, what are probably the most, contain an ID slice of length 1.
  * If a trice occurs for example 5 times its ID slice has length 5 containing 5 different IDs.
  * When the `trice -u` command finds a **trice** with ID slice length > 1, it looks into `li_1.map` for all possible IDs and compares the location information with the actual location:
    * If no matching file name is found, a new ID is generated.
    * If file name is identical, the ID with the minimum difference to the line number is chosen if not already used.
    * Because all assigned IDs go into the `li.map` this is possible to check.
    * If all IDs in the slice with identical file name are used, a new ID is generated.
    * Of course there are cases possible, where some unwanted ID "shift" happens. But we have to consider, that first we are talking about rare identical **trices** and that such case, if, only happens once with the result, that the `til.json` file adds a bit data garbage. A `til.json` cleaning is always possible, but you loose history then.

##  8. <a name='Changelog'></a>Changelog

| Date        | Version | Comment |
| -           | -       | - |
| 2022-MAR-15 |  0.0.0  | Initial Draft |
| 2022-MAR-15 |  0.1.0  | Minor corrections applied. |
| 2022-MAR-15 |  0.2.0  | Sigil byte encoding clarified. |
| 2022-MAR-15 |  0.3.0  | Forward versus backward COBS encoding discussion inserted. |
| 2022-MAR-15 |  0.4.0  | Forward versus backward COBS encoding reworked. Disruption detection added. |
| 2022-MAR-15 |  0.5.0  | Minor corrections |
| 2022-MAR-16 |  0.6.0  | TCOBS prime number comment added, simplified |
| 2022-MAR-17 |  0.7.0  | TCOBS move into a separate [TCOBS Specification](./TCOBSSpecification.md), Framing more detailed. |
| 2022-MAR-20 |  0.7.1  | Contributive *Trice* extension remark added. |
| 2022-APR-12 |  0.8.0  | TREX mainstream format changed to timestamps immediate after ID. |
| 2022-MAY-20 |  0.8.1  | Formatting, Spelling |
| 2022-JUN-19 |  0.9.0  | Implementation hint added to chapter Framing. |
| 2022-AUG-14 | 0.10.0  | Chapter ID Management added |
| 2022-AUG-19 | 0.11.0  | Chapter Main Stream Logs changed/extended |
| 2022-SEP-15 | 0.11.1  | TS32, TS16, NOTS, MOD7 added |
| 2022-OCT-08 | 0.11.2  | S0...X3 added |
