
<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>

<!-- vscode-markdown-toc -->
* 1. [Preface](#Preface)
* 2. [Trice Decoding](#TriceDecoding)
	* 2.1. [*Trice* ID list `til.json`](#TriceIDlisttil.json)
	* 2.2. [*Trice* location information file `li.json`](#Tricelocationinformationfileli.json)
* 3. [TREX (*TR*ice *EX*tendable) encoding](#TREXTRiceEXtendableencoding)
	* 3.1. [Framing](#Framing)
	* 3.2. [Endianness](#Endianness)
	* 3.3. [`TRICE` (Time)Stamps](#TRICETimeStamps)
	* 3.4. [Binary Encoding](#BinaryEncoding)
		* 3.4.1. [Symbols](#Symbols)
		* 3.4.2. [*Trice* format](#Triceformat)
		* 3.4.3. [Framing with TCOBS encoding](#FramingwithTCOBSencoding)
* 4. [ID Management](#IDManagement)
* 5. [Changelog](#Changelog)

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

* The [TREX (*Trice* extendable) encoding](#TREXTriceextendableencoding) format is planned to be stable.
* Because the **trice** tool supports several encodings, old projects should be usable with newer **trice** tool versions by applying the `-encoding TLE` switch.

<!--
The with name "COBS" branded [*Trice* v0.48.0 encoding](./TriceMessagesEncoding.md) is not optimal concerning the generated data amount:
* See discussion [#253 Save trice COBS encoded data on target and view it later on PC](https://github.com/rokath/trice/discussions/253).
* The location information is transmitted as 16 bit file ID plus 16 bit line number. It is possible to generate during `trice update` an additional file `li.json` containing the location information for each *Trice* ID avoiding the additional 4 bytes this way. But this could cause assignment issues, when the same *Trice* ID is used at different locations (see [https://github.com/rokath/trice/discussions/264](https://github.com/rokath/trice/discussions/264)). But it is possible to drop the option `trice u -sharedIDs`.
* The 32-bit "COBS" package descriptor is overkill for allowing user data and dropped in TREX.
* The additional padding bytes to achieve 32 bit sizes are not needed and dropped in TREX. The user could add them by himself if really needed.
* The 4 timestamp bytes in front of each *Trice* demand the "COBS" package descriptor. The timestamp should go inside the *Trice* message and be optionally smaller. That is done in TREX.


<p align="right">(<a href="#top">back to top</a>)</p>

-##  2. <a name='Compatibility'></a>Compatibility

* There is no guaranty for compatibility with legacy target trice versions but the aim is to provide it.


* The *Trice* v0.48.0 user syntax will remain mainly unchanged. The letter case of the ID codes the target timestamp size. (see below)
* The as "COBS" branded legacy [v0.48.0 *Trice* encoding](.TriceMessageEncoding.md) will stay unchanged as an option for compatibility. But it will not be the default encoding anymore. To use newer **trice** tool versions with legacy projects the CLI switch `-encoding TLE` needs to be used.
* The option `-sharedIDs` will be further available but depreciated to avoid location assignment issues.
* Legacy projects which used the option `-sharedIDs` will still work even with a `li.json` file. A several times used ID will get an assignment of one of the locations.
* The issue [#242 Add target context option](https://github.com/rokath/trice/issues/242) could get the label "wontfix". When a task ID is needed, it could be also a data value in such cases.
* The same user source files usable with the legacy *Trice* "COBS" encoding and the proposed additional [TREX](#TREXTriceextendableencoding) encoding. They will have 16 bit stamps instead of 32-bits if you keep the sub macros`Id(n)`.
* Exchange individually to `ID(n)` to get 32-bit timestamps back.
* Exchange individually to `id(n)` to avoid timestamps.

* User data are in separate TCOBS packages encoded. When *Trices* are accumulated in a double half buffer, their separation in TCOBS packages is possible until the first extended *Trice*. Because of the generally unknown extended *Trice* length from this point, all following *Trices* in this half buffer need to go in one TCOBS package (including optional padding bytes) what is ok. The only disadvantage with this is, that in case of a data disruption at this place, several *Trice* messages can get lost.

Possible better implementation: See issue [#290](https://github.com/rokath/trice/issues/290)

COBS: See [#337](https://github.com/rokath/trice/issues/337)
-->

<p align="right">(<a href="#top">back to top</a>)</p>

##  2. <a name='TriceDecoding'></a>Trice Decoding

The binary *trice* data carry IDs which are used to display the log strings. These IDs are pointing in two reference files.

###  2.1. <a name='TriceIDlisttil.json'></a>*Trice* ID list `til.json`

* This file integrates all firmware variants and versions and is the key to display the message strings. With the latest version of this file all previous deployed firmware images are usable without the need to know the actual firmware version. <!-- For the implementation of the optional *Trice* extensions (see below), a `til.json` format extension is needed because several files are unhandy. Both `til.json` formats will be accepted in the future. -->


###  2.2. <a name='Tricelocationinformationfileli.json'></a>*Trice* location information file `li.json`

<!-- With [TREX](#TREXTriceextendableencoding) encoding the location information needs no transmission anymore but goes not into the `til.json` file. In the field the location information is normally useless and probably outdated. The software developer is the one, mostly interested in the location information.  When the firmware left the developer table, only the file `til.json` is of interest.--> 
* If the generated `li.json` is available, the **trice** tool automatically displays file name and line number. But that is accurate only with the exact matching firmware version. That usually is the case right after compiling and of most interest at the developers table.
* The **trice** tool will silently not display location information, if the `li.json` file is not found. For in-field logging, the option `-showID string` could be used. This allows later an easy location of the relevant source code.
* An other option is to record the binary trice messages and to play them later with the trice tool using the correct `li.json`.
 <!-- Also the planned `-binaryLogfile` option is possible. See [issue #267 Add `-binaryLogfile` option](https://github.com/rokath/trice/issues/267). It allows a replay of the logs and the developer can provide the right version of the `li.json` file.-->

<p align="right">(<a href="#top">back to top</a>)</p>

##  3. <a name='TREXTRiceEXtendableencoding'></a>TREX (*TR*ice *EX*tendable) encoding

* The extendable encoding leaves options for user specific data mixable with trice data. This allows filtering of the framed data by just checking a bit pattern.


<p align="right">(<a href="#top">back to top</a>)</p>

###  3.1. <a name='Framing'></a>Framing

* Framing is important for data disruption cases and is done with [TCOBS](./TCOBSSpecification.md) (has included data reduction) but the user can force to use [COBS](https://github.com/rokath/COBS), what makes it easier to write an own decoder in some cases. 
  * Change the setting `TRICE_FRAMING` inside `triceConfig.h` and use the **trice** tools `-framing` switch for that.
* For robustness each *Trice* gets its own (T)COBS package per default. That is changeable.



###  3.2. <a name='Endianness'></a>Endianness

* For efficiency binary trice data are stored and transmitted in MCU endianness and the **trice** tool expects binary data in little endian format as most MCUs are little endian.
* On big endian MCUs the compiler switch `TRICE_MCU_IS_BIG_ENDIAN` needs to be defined and the **trice** tool has a CLI switch "triceEndianness" which needs to be set to "bigEndian" then.
* If trice transmit data are needed to be not in MCU order for some reason, the macro `TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN` is needed. This increases the critical trice storage time and target code amount.


###  3.3. <a name='TRICETimeStamps'></a>`TRICE` (Time)Stamps

* Each `TRICE` message can carry stamp bits, which are free usable like for time, addressing or filtering.
* By selecting the ID letter case `id(0)`, `Id(0)` or `ID(0)` the user can decide for each single `TRICE` macro about the stamp size.

| notation       | meaning |
| -              | - |
| `TRICE( id(0)` | no stamp at all |
| `TRICE( Id(0)` | calls internally `uint16_t TriceStamp16( void )` for trice message stamping |
| `TRICE( ID(0)` | calls internally `uint32_t TriceStamp32( void )` for trice message stamping |

It is up to the user to provide the functions `TriceStamp16()` and/or `TriceStamp32()`.

* [x] New *Trice* macros are writable without the ID, so when `trice u` is executed, a CLI switch controls the ID type selection:
  * The update switch `-stamp 32` defaults new ID´s to `ID`.
  * The update switch `-stamp 16` defaults new ID´s to `Id`.
  * The update switch `-stamp 0`  defaults new ID´s to `id`.
  * The update switch `-stamp to32` converts all `id` and `Id` to `ID`.
  * The update switch `-stamp to16` converts all `id` and `ID` to `Id`.
  * The update switch `-stamp to0`  converts all `ID` and `Id` to `id`.
* [x] `trice log`:
  * The log switch `-ttsf` is the same as `-ttsf32`.
  * There is a new log switch `ttsf16` for the 16 bit timestamps.
  * The `trice` tool alignes *Trice* messages with different timestamp sizes.

###  3.4. <a name='BinaryEncoding'></a>Binary Encoding

####  3.4.1. <a name='Symbols'></a>Symbols

* The (time)stamp
* `i` = ID bit
* `I` = `iiiiiiii` = ID byte
* `n` = number bit
* `s` = selector bit
* `N` = `snnnnnnnn` = number byte
* `c` = cycle counter bit
* `C` = s==0 ? `cccccccc` : `nnnnnnnn` = cycle counter byte or number byte extension
* `t` = (time)stamp bit
* `T` = `tttttttt` = (time)stamp byte
* `d` = data bit
* `D` = `dddddddd` = data byte
* `...` = 0 to 32767 data bytes
* `"..."` = format string


* All decoded 0-, 1-, 2- and 3-byte frames is considered as user data and ignored by the trice tool.
* In decoded >= 4-byte frames the first 2 bytes are the 14-bit ID with 2 selector bits.
* The `0` selector is usable for any user encoding. The **trice** tool ignores such packages.

<!--
* [x] A 32-bit wide *Trice* storage access is implemented for speed.

 
* [x] All main stream logs share the same 13 bit ID space[^1] allowing 1-8191 IDs in 8 variants parallel usable:

[^1]: Legacy projects can use the `-IDbits` switch.

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
-->

| 16-bit groups            | Selector (2 msb)| *Trice* code                 | Comment                              |
| :-                       | :-:             | -                            | -                                    |
| `00xxxxxxX ...`          | 0               |                              | reserved for extensions or user data |
| `01iiiiiiI NC  ...`      | 1               | `TRICE( id(n), "...", ...);` | *Trice* format without     stamp     |
| `10iiiiiiI TT NC ...`    | 2               | `TRICE( Id(n), "...", ...);` | *Trice* format with 16-bit stamp     |
| `11iiiiiiI TT TT NC ...` | 3               | `TRICE( ID(n), "...", ...);` | *Trice* format with 32-bit stamp     |

<!--
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

  * [x] **iD(n)**

  ```c
  #define iD(n) { uint64_t ts = TRICE_READ_TICK64; TRICE_PUT16(  0xC000|(n)); TRICE_PUT1616(ts); TRICE_PUT1616(ts>>32); }
  ```


* [ ] Example:
  * Code:

  ```c
  //! TRICE8_1 writes trice data as fast as possible in a buffer.
  //! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
  //! \param v0 a 8 bit bit value
  #define TRICE8_1( id, pFmt, v0 ) \
    TRICE_ENTER id; CNTC(1); \
    TRICE_PUT( (uint8_t)(v0) ); /* little endian*/ \
    TRICE_LEAVE
  ```

  * Transfer package (decoded little endian)
 
With 16-bit stamp:

  ```b
  e3 // lower byte of 14-bit ID byte
  ae // 2 msb = 10 = typeT2 plus 6 bit upper ID byte
  ec // lower byte 16-bit timestamp
  02 // upper byte 16-bit timestamp
  64 // cycle counter
  01 // parameter count
  ff // parameter value
  ```



  ```b
  e3 // lower byte of 14-bit ID byte
  ae // 3 msb = 101 = type S2, plus 5 bit upper ID byte
  ec // lower byte 16-bit stamp
  02 // upper byte 16-bit stamp
  64 // cycle counter
  01 // parameter count
  ff // parameter value
  ```

Without stamp:

  ```b
  e3 // lower byte of 14-bit ID byte
  6e // 3 msb = 011 = type S0, plus 5 bit upper ID byte
  64 // cycle counter
  01 // parameter count
  ff // parameter value
  ```
-->


####  3.4.2. <a name='Triceformat'></a>*Trice* format

* Parameter data bytes start after the optional timestamp.
* N is the parameter data bytes count. Padding bytes are not counted.
* Usually N is < 127 but for buffer or string transfer N can get up to 32767 (15 bits).
* When N > 127 (s==1) `NC` is replaced by `1nnnnnnn nnnnnnnn`. C is incremented with each *Trice* but not transmitted when:
  * N > 127
  * extended *Trice* without C

####  3.4.3. <a name='FramingwithTCOBSencoding'></a>Framing with TCOBS encoding

* For maximum storage speed each **trice** message starts at a 32-bit boundary and has 1-3 padding bytes.
* In direct mode only a single message needs handling.
* In deferred mode after half buffer swap any count of trice messages is in the buffer.
* There are different policies possible:
  1. **TRICE_FAST_MULTI_MODE**: Compact buffer by removing all padding bytes, encode it as a single package, append one 0-delimiter and transmit. This allows to reduce the transmitted data amount by paying the price of possibly more data lost in case of an error. Also the data interpretation can perform less checks.
  2. **TRICE_SAFE_SINGLE_MODE**: Encode each buffer separate, append a 0-delimiter for each and pack them all together before transmitting. This increases the transmit data slightly but minimizes the amount of lost data in case of a data disruption.

<!-- ###  6.3. <a name='ExtendedTricesasfutureoption'></a>Extended *Trices* as future option

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
-->
<!-- ###  6.4. <a name='Unknownuserdata'></a>Unknown user data

* Unknown user data are possible as part of the *Trice* extensions.
  * Without the `-ex0` switch, `0000...` packages are ignored as unknown user data.
  * Without the `-ex1` switch, `0001...` packages are ignored as unknown user data.
  * Without the `-ex2` switch, `0010...` packages are ignored as unknown user data.
  * Without the `-ex3` switch, `0011...` packages are ignored as unknown user data.
* So, if *Trice* extensions not used, all `00...` packages are ignored as unknown user data.
* Unknown user data have an unknown length. Therefore they cannot share a COBS packet with *Trices*.
* Unknown user data packets do not affect the cycle counter. The can have their own cycle counter.
-->

##  4. <a name='IDManagement'></a>ID Management

* The IDs inside the source code are a "dealbreaker" as [bora](https://community.memfault.com/u/bora) mentioned in his [comment](https://interrupt.memfault.com/blog/trice). In fact it is not acceptable for library code used in several projects. An improved approach could look like this:


```c
TRICE( id(0), "...", ...); // a trice without stamp
TRICE( Id(0), "...", ...); // a trice with a 16-bit stamp
TRICE( ID(0), "...", ...); // a trice with a 32-bit stamp
```
<!--

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

* When editing, the user needs to write only `TRICE( "...", ...);` and the trice tool inserts a S0, S2, S4 or S8 automatically according to the used `-stamp` switch parameter.
* After repository check-out and before compiling, following substitutions are done using `trice -u`:
  * `TRICE( S0, "...", ...);` → `TRICE( id(0), "...", ...);` → `TRICE( id(12345), "...", ...);`
  * `TRICE( S2, "...", ...);` → `TRICE( Id(0), "...", ...);` → `TRICE( Id(12345), "...", ...);`
  * `TRICE( S4, "...", ...);` → `TRICE( ID(0), "...", ...);` → `TRICE( ID(12345), "...", ...);`
  * `TRICE( S8, "...", ...);` → `TRICE( iD(0), "...", ...);` → `TRICE( ID(12345), "...", ...);`
* After compiling and before repository check-in, following substitutions are done using `trice -z`:
  * `TRICE( id(12345), "...", ...);` → `TRICE( id(0), "...", ...);` → `TRICE( S0, "...", ...);`
  * `TRICE( Id(12345), "...", ...);` → `TRICE( Id(0), "...", ...);` → `TRICE( S2, "...", ...);`
  * `TRICE( ID(12345), "...", ...);` → `TRICE( ID(0), "...", ...);` → `TRICE( S4, "...", ...);`
  * `TRICE( iD(12345), "...", ...);` → `TRICE( iD(0), "...", ...);` → `TRICE( S8, "...", ...);`
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
-->

##  5. <a name='Changelog'></a>Changelog

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
| 2022-NOV-28 | 0.11.3  | +[#337](https://github.com/rokath/trice/issues/337) in [Framing](#Framing)|

