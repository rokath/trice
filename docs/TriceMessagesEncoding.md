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
* The detailed *Trice* encoding is derivable from [trice.h]("../pkg/src/trice.h) and not repeated here to avoid unnecessary errors.

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

## Small Size Logs (ideas)
<!--
* `0000iiii D D`
* `0001iiii D D T`
* `0010iiii C T T  D D D D`
* `0011iiii C D D  T T T T`
* `0100iiii I D D`
* `0101iiii I D D  T`
-->
* `0000iiii tttttttt ttttdddd dddddddd` 4 bit ID, 12 bit timestamp & 12 bit data TRICE12
* `0100iiii I T T  D D D D` 12 bit ID, 16 bit timestamp & 32 bit data: TRICE84, TRICE162, TRICE816, TRICE168, TRICE321
* `1iiiiiii I N C  T T T T ...` Legacy COBS with timestamp inside
