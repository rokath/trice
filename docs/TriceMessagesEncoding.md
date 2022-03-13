# *Trice* message encoding

> _(Read only you are interested in)_

<!-- vscode-markdown-toc -->
* 1. [COBS encoding and user protocols](#COBSencodinganduserprotocols)
* 2. [[COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) encoding for re-sync after data disruption](#COBShttps:en.wikipedia.orgwikiConsistent_Overhead_Byte_Stuffingencodingforre-syncafterdatadisruption)
* 3. [32-bit transfer chunks](#bittransferchunks)
	* 3.1. [Example: zero-delimited 7-bytes COBS package](#Example:zero-delimited7-bytesCOBSpackage)
	* 3.2. [Example: zero-delimited 8-bytes COBS package:](#Example:zero-delimited8-bytesCOBSpackage:)
* 4. [Package mode prefix](#Packagemodeprefix)
	* 4.1. [Example: 12 byte trice message buffer prefixed with mode 0 (no time stamps):](#Example:12bytetricemessagebufferprefixedwithmode0notimestamps:)
	* 4.2. [Example: 16 byte trice message buffer prefixed with mode 1 (time stamps)](#Example:16bytetricemessagebufferprefixedwithmode1timestamps)
	* 4.3. [Example: 12-bytes user packet - will be ignored by the **trice** tool:](#Example:12-bytesuserpacket-willbeignoredbythetricetool:)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  1. <a name='COBSencodinganduserprotocols'></a>COBS encoding and user protocols

* The [**C**onsistent **O**verhead **B**yte **S**tuffing](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) technique is a very powerful and simple way for re-syncing.
* Just in case, wait for the next package delimiter.
* Several *Trice* message can occur within one single **COBS** package.
* Each **COBS** package starts with a 32-bit descriptor:

| Descriptor                  | Meaning                                                      |
|-----------------------------|--------------------------------------------------------------|
| `0x00000000`                | *Trice* message(s) without prefix                            |
| `0x00000001`                | *Trice* message(s) with 32-bit target timestamp              |
| `0x00000002`                | *Trice* message(s) with 32-bit target location               |
| `0x00000003`                | *Trice* message(s) with 64-bit target timestamp and location |
| `0x00000004`...`0x000000FF` | Reserved  for *Trice* encodings                              |
| `0x00000100`...`0xFFFFFFFF` | User protocol data, the **trice** tool ignores them          |

* This allows intermixing of several data streams with *Trice* data.
* After the 4 COBS package descriptor bytes start several full *Trice* messages.
* In dependence of the COBS package descriptor each *Trice* message is prefixed with 0, 32 or 64 bit additional information: target code location and target timestamp.
* The detailed *Trice* encoding is derivable from [trice.h](../pkg/src/trice.h) and not repeated here to avoid unnecessary errors.

##  2. <a name='COBShttps:en.wikipedia.orgwikiConsistent_Overhead_Byte_Stuffingencodingforre-syncafterdatadisruption'></a>[COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) encoding for re-sync after data disruption

* After a data transmission disruption, reliable re-sync should be possible.
* The [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) techique gives this possibility in an easy way: simply wait for the next packet delimiter byte (usually `0`).
* Therefore all *Trice* transfers are COBS encoded.

##  3. <a name='bittransferchunks'></a>32-bit transfer chunks

* A *Trice* data stream comes always in a multiple-of-4 length for effective transfer.
  * After COBS encoding the length is the same or 1.04 times longer.
  * Therefore after [COBS](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) encoding the packages are delimited by 1 to 4 zeroes.
  * After the first zero delimiter, 0 to 3 padding zeroes are ignored as len-0 packages.

###  3.1. <a name='Example:zero-delimited7-bytesCOBSpackage'></a>Example: zero-delimited 7-bytes COBS package

|cobs|cobs|cobs|cobs|cobs|cobs|cobs|delimiter|
| -  | -  | -  | -  | -  | -  | -  | -       |
| x  | x  | x  | x  | x  | x  | x  | 0       |
| 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7       |

###  3.2. <a name='Example:zero-delimited8-bytesCOBSpackage:'></a>Example: zero-delimited 8-bytes COBS package:

|cobs|cobs|cobs|cobs|cobs|cobs|cobs|cobs|delimiter|padding|padding|padding|
| -  | -  | -  | -  | -  | -  | -  | -  | -       | -     | -     | -     |
| x  | x  | x  | x  | x  | x  | x  | x  | 0       | 0     | 0     | 0     |
| 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8       | 9     | 10    | 11    |

##  4. <a name='Packagemodeprefix'></a>Package mode prefix

* *Trice* messages are always multiple-of-4-bytes messages.
* A *Trice* buffer can contain several *trice* messages.
  * Each single *trice* carries its own length information.
* Just before buffer encoding takes part, a 32-bit buffer mode value is prefixed:
  * buffer mode 0: *Trice* messages are without embedded device timestamps.
  * buffer mode 1: *Trice* messages are prefixed with 32-bit embedded device timestamps.
  * buffer mode 2: *Trice* messages are prefixed with 32-bit embedded device source location information.
  * buffer mode 3: *Trice* messages are prefixed with 64-bit embedded device source location information and timestamp. 
  * buffer mode 4-15: Reserved. The **trice** tool ignores such package.
  * buffer mode 16-0xFFFFFFFF: user mode values. The **trice** tool ignores such package. This way any user protocols transferable over the same line.

###  4.1. <a name='Example:12bytetricemessagebufferprefixedwithmode0notimestamps:'></a>Example: 12 byte trice message buffer prefixed with mode 0 (no time stamps):

* The 12 *trice* bytes could be 1, 2 or 3 *trice* messages consisting of 3, 2 or 1 32-Bit values.

|mode|mode|mode|mode|data|data|data|data|data|data|data|data|data|data|data|data|
| -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  |
| 0  | 0  | 0  | 0  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  |
| 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 |

###  4.2. <a name='Example:16bytetricemessagebufferprefixedwithmode1timestamps'></a>Example: 16 byte trice message buffer prefixed with mode 1 (time stamps)

* The 16 *trice* bytes could be 1 or 2 *trice* messages consisting of 3 or 1 32-bit values, each prefixed by a target timestamp.

|mode|mode|mode|mode|data|data|data|data|data|data|data|data|data|data|data|data|data|data|data|data|
| -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  |
| 1  | 0  | 0  | 0  | t  | t  | t  | t  | x  | x  | x  | x  |t\|x|t\|x|t\|x|t\|x| x  | x  | x  | x  |
| 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 |

###  4.3. <a name='Example:12-bytesuserpacket-willbeignoredbythetricetool:'></a>Example: 12-bytes user packet - will be ignored by the **trice** tool:

|mode|mode|mode|mode|data|data|data|data|data|data|data|data|data|data|data|data|
| -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  | -  |
| 0  | 1  | 0  | 0  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  | x  |
| 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 |

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
## Ex(tendable) encoding (ideas)

### Symbols

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

Framing is done by COBS encoding, maybe with zeroes reduction. Several *Trices* can be in one COBS package.

### Main stream logs

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

### *Trice* format

* Optional data start after optional timestamp.
* N is not u32 count, it is data byte count (without header, without timestamp).
* N > 127 tells `N C` is replaced by `1nnnnnnn nnnnnnnn`, alllowing 32767 bytes.
  * C is incremented with each *Trice* but not transmitted when:
    * N > 127
    * extended *Trice* without C
* Inside double buffer each trice starts at a u32 boundary.
* The COBS encoding skips the wholes, making a compact buffer.

### Extended *Trices* as future option

If for special cases, the main stream encoding is not sufficient, the user can add its own encoding.

* `00...` sub-options `TRICEX0`, `TRICEX1`, `TRICEX2`, `TRICEX3`
  * `-ex0 pos -ex1 pos -ex2 pos -ex3 pos`  Select position in extendable table for TRICEXn, 4 codings selectable in one shot.
  * The table is creatable and extendable on demand.
  * For each line an appropriate target and host code needs to be done.
  * Then the target configuration must match the CLI switches.
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
    * `-ex0 13` means TRICEX0 = `0000iiii C T T  D D D D`. Usage `TRICEX0( "result %f\n", aFloat(val));`
    * `-ex2 6`  means TRICEX2 = `0010dddd dddddddd`. Usage TRICEX2( "point %x,%d\n", a, b)
  * *Trice* extensions without cycle counter are counted as well.
  * Each TRICEXn has its own ID space.

### Unknown user data

* Unknown user data are possible as part of the *Trice* extensions.
  * Without the `-ex0` switch, `0000...` packages are ignored as unknown user data.
  * Without the `-ex1` switch, `0001...` packages are ignored as unknwno user data.
  * Without the `-ex2` switch, `0010...` packages are ignored as unknown user data.
  * Without the `-ex3` switch, `0011...` packages are ignored as unknown user data.
* So, if *Trice* extensions not used, all `00...` packages are ignored as unknown user data.
* Unknown user data have a unknown length. Therefore they cannot share a COBS packet with *Trices*.
* Unknown user data packets do not affect the cycle counter. The can have their own cycle counter.
