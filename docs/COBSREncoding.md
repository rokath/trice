# COBS/R encoding design draft

## Table of Contents

- [COBS/R encoding design draft](#cobsr-encoding-design-draft)
  - [Table of Contents](#table-of-contents)
  - [Preface](#preface)
  - [COBS/R encoding examples](#cobsr-encoding-examples)
    - [COBS/R encoding for 0-byte packages](#cobsr-encoding-for-0-byte-packages)
    - [COBS/R encoding for 1-byte packages](#cobsr-encoding-for-1-byte-packages)
    - [COBS/R encoding for 2-byte packages](#cobsr-encoding-for-2-byte-packages)
    - [COBS/R encoding for n-byte packages](#cobsr-encoding-for-n-byte-packages)
  - [Interpreter for decoded COBS/R package](#interpreter-for-decoded-cobsr-package)
    - [Encoding table 0 legend](#encoding-table-0-legend)
    - [Encoding table 0 (without cycle counter)](#encoding-table-0-without-cycle-counter)
    - [Encoding table 1 legend](#encoding-table-1-legend)
    - [Encoding table 1 (with 4-bit cycle counter)](#encoding-table-1-with-4-bit-cycle-counter)
    - [Encoding table 2 (with 8-bit cycle counter)](#encoding-table-2-with-8-bit-cycle-counter)
  - [Fast TRICE data storing](#fast-trice-data-storing)

(Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc.go))

## Preface

- Packages are [COBS/R](https://pythonhosted.org/cobs/cobsr-intro.html) encoded.
- Selected separator byte is `00`. That means the COBS/R encoded packages contain no `00` bytes and separated by a `00` byte.
- After a transfer interruption a very easy resync mechanism is usable: simply wait for the next `00` byte.
- The COBS/R encoding usually has the same length as the unencoded data and sometimes has one byte more but an additional `00` is needed for package separation.
- This way the ID bit count is adjustable to the real communication needs.
- One important point is the possibility to embed additional protocols in the data stream.

## COBS/R encoding examples

### COBS/R encoding for 0-byte packages

- This is simply an empty package. Just the `00` package separator byte is transmitted.
- It is normally used as padding byte to reach a multiple of 8 bytes package length when putting several COBS/R packages into one encryption packet.

### COBS/R encoding for 1-byte packages

- One byte COBS/R packages are a 1:1 transformation despite for the values `00` and `01`.

| raw  | COBS/R (all followed by a not shown 00)  | remark
| :--  | :-----                                   | ---------------------------
| `00` |  `01 01`                                 | starting byte 00 prolongs code
| `01` |  `02 01`                                 | starting byte 01 prolongs code
| `02` |  `02`                                    |
| `03` |  `03`                                    |
| `...`|  `...`                                   |
| `fc` |  `fc`                                    |
| `fd` |  `fd`                                    |
| `fe` |  `fe`                                    |
| `ff` |  `ff`                                    |

One byte packages are fast COBS/R codable by simply incrementing the 2 values `00` and `01` and appending a `01`.

### COBS/R encoding for 2-byte packages

- Two bytes COBS/R packages are often a 1:1 transformation despite some cases as seen in the following table.

| raw  | COBS/R (all followed by a not shown 00)     | remark
| :--  | :-----                                      | ---------------------------
| `00 00` |  `01 01 01`                              | starting bytes 00, 01 and 02 prolong code usually
| `00 01` |  `02 01 01`                              |
| `00 02` |  `02 02 01`                              |
| `00 03` |  `02 03 01`                              |
| `...`   |  `...`                                   |
| `00 fc` |  `02 fc 01`                              |
| `00 fd` |  `02 fd 01`                              |
| `00 fe` |  `02 fe 01`                              |
| `00 ff` |  `02 ff 01`                              |
| `...`   |  `...`                                   |
| `01 00` |  `01 02 01`                              |
| `01 01` |  `03 01 01`                              |
| `01 02` |  `03 02 01`                              |
| `01 03` |  `03 03 01`                              |
| `...`   |  `...`                                   |
| `01 fc` |  `03 fc 01`                              |
| `01 fd` |  `03 fd 01`                              |
| `01 fe` |  `03 fe 01`                              |
| `01 ff` |  `03 ff 01`                              |
| `...`   |  `...`                                   |
| `02 00` |  `01 02   `                              | special case
| `02 01` |  `03 01 02`                              |
| `02 02` |  `03 02 02`                              |
| `02 03` |  `03 03 02`                              |
| `...`   |  `...`                                   |
| `02 fc` |  `03 fc 02`                              |
| `02 fd` |  `03 fd 02`                              |
| `02 fe` |  `03 fe 02`                              |
| `02 ff` |  `03 ff 02`                              |
| `...`   |  `...`                                   |
| `03 00` |  `01 03`                                 |
| `03 01` |  `03 01`                                 |
| `03 02` |  `03 02`                                 |
| `03 03` |  `03 03`                                 |
| `...`   |  `...`                                   |
| `03 fc` |  `03 fc`                                 |
| `03 fd` |  `03 fd`                                 |
| `03 fe` |  `03 fe`                                 |
| `03 ff` |  `03 ff`                                 |
| `...`   |  `...`                                   |
| `fc 00` |  `01 fc`                                 |
| `fc 01` |  `fc 01`                                 |
| `fc 02` |  `fc 02`                                 |
| `fc 03` |  `fc 03`                                 |
| `...`   |  `...`                                   |
| `fc fc` |  `fc fc`                                 |
| `fc fd` |  `fc fd`                                 |
| `fc fe` |  `fc fe`                                 |
| `fc ff` |  `fc ff`                                 |
| `...`   |  `...`                                   |
| `fd 00` |  `01 fd`                                 |
| `fd 01` |  `fd 01`                                 |
| `fd 02` |  `fd 02`                                 |
| `fd 03` |  `fd 03`                                 |
| `...`   |  `...`                                   |
| `fd fc` |  `fd fc`                                 |
| `fd fd` |  `fd fd`                                 |
| `fd fe` |  `fd fe`                                 |
| `fd ff` |  `fd ff`                                 |
| `...`   |  `...`                                   |
| `fe 00` |  `01 fe`                                 |
| `fe 01` |  `fe 01`                                 |
| `fe 02` |  `fe 02`                                 |
| `fe 03` |  `fe 03`                                 |
| `...`   |  `...`                                   |
| `fe fc` |  `fe fc`                                 |
| `fe fd` |  `fe fd`                                 |
| `fe fe` |  `fe fe`                                 |
| `fe ff` |  `fe ff`                                 |
| `...`   |  `...`                                   |
| `ff 00` |  `01 ff`                                 |
| `ff 01` |  `ff 01`                                 |
| `ff 02` |  `ff 02`                                 |
| `ff 03` |  `ff 03`                                 |
| `...`   |  `...`                                   |
| `ff fc` |  `ff fc`                                 |
| `ff fd` |  `ff fd`                                 |
| `ff fe` |  `ff fe`                                 |
| `ff ff` |  `ff ff`                                 |

- Two byte packages are fast COBS/R codable by simply using an Id subset having no first byte 0, 1, 2 and no 0 in the 2nd byte higher nibble:
- Using `II IC`, where C is a 4 bit cycle counter assumed to sometimes 0 :
  - Id = 0xIII0 = range 4096 
  - Id = 0x00n = range 16 is forbidden
  - Id = 0x01n = range 16 is forbidden // only for trice0
  - Id = 0x02n = range 16 is forbidden // only for trice0
  - Id = 0xnn0 = range 256 is forbidden, but not if cycle counter moves only between 1 and 15.
  - -> 3792 different Ids allowed

### COBS/R encoding for n-byte packages

- This looks similar to 1-byte and 2-byte encoding and is not shown here.
- Some super fast code for 3- and 4-byte packet encoding is also possible.
- All *trice* packages are much shorter than 255 bytes so the COBS/R encoding is cheap.

## Interpreter for decoded COBS/R package

[!IMPORTANT]
- After receiving and decoding a COBS/R package, the receiver can decide according to the package length and its starting bits what to do with it:
  - Package lengths 2, 3, 4, 6, 10, 18, 34, 66 starting with four 0-bits are trice logs.
    - Treat as received *trice* message.
  - Multiple of 8 bytes packages are used for XTEA encryption.
    - The decrypted packet is treated again as a COBS/R encoded byte stream and handled recursively the same way.
    - This way several COBS/R encoded data packages can be joint in one package for encryption.
      - Empty COBS/R packages are `00` bytes and used to reach the next multiple of 8-bytes COBS/R sequence.
  - All other packages are useable for other protocols (marked as reserved).
    - Ignore, route forward or call user handler.
    - 1-byte COBS/R packages are not recommended for numerous data. Because of the delimiter byte, are only ~50% bandwidth usable.

### Encoding table 0 legend

| Legend | Meaning                                                           |
| :-     | :---------------------------------------------------------------- |
| ...n   | totally n times                                                   |
| I\|iiii| 4 Id-bits (half byte)                                             |
| V\|vvvv| 4 value bits                                                      |
| X\|xxxx| 4 arbitrary bits (any half byte )                                 |
| Y\|yyyy| 4 arbitrary bits, but at least one must be 1 (any half byte != 0) |

### Encoding table 0 (without cycle counter)

|half bytes      | same as bits                     | bytes|ID bits| ID range    |ID map| remark                                                                      |
| -              | -------------------------------- |:----:| :---: | :------:    |  :-: |     :-                                                                      |
|` `             | ` `                              |    0 |       |             |      | COBS/R padding byte                                                         |
|`0I II`         |`0000iiii iiiiiiii`               |    2 |   12  | 0\-4095     |  0   | `TRICE0`                                                                    |
|`0I II VV`      |`0000iiii iiiiiiii vvvvvvvv`      |    3 |   12  | 4096\- 8191 |  1   | `TRICE8_1`                                                                  |
|`0I II VV VV`   |`0000iiii iiiiiiii vvvvvvvv...2`  |    4 |   12  | 8192\-12287 |  2   | `TRICE8_2`, `TRICE16_1`                                                     |
|`0I II VV...4`  |`0000iiii iiiiiiii vvvvvvvv...4`  |    6 |   12  |12288\-16383 |  3   | `TRICE8_3`, `TRICE8_4`, `TRICE16_1`, `TRICE16_2`,  `TRICE32_1`              |
|`0I II VV...8`  |`0000iiii iiiiiiii vvvvvvvv...8`  |   10 |   12  |16384\-20479 |  4   | `TRICE8_5`...`TRICE8_8`, `TRICE16_3`, `TRICE16_4`, `TRICE32_2`, `TRICE64_1` |
|`0I II VV...16` |`0000iiii iiiiiiii vvvvvvvv...16` |   18 |   12  |20480\-24575 |  5   | `TRICE116_5`...`TRICE16_8`, `TRICE32_3`...`TRICE32_4`, `TRICE64_2`          |
|`0I II VV...32` |`0000iiii iiiiiiii vvvvvvvv...32` |   34 |   12  |24576\-28671 |  6   | `TRICE32_5`...`TRICE32_8`, `TRICE64_3`...`TRICE64_4`                        |
|`0I II VV...64` |`0000iiii iiiiiiii vvvvvvvv...64` |   66 |   12  |28672\-32767 |  7   | `TRICE64_5`...`TRICE64_8`                                                   |
|`YX XX`         |`yyyyxxxx xxxxxxxx`               |    2 |       |             |      | reserved                                                                    |
|`YX XX XX...2^n`|`yyyyxxxx xxxxxxxx xxxxxxxx...2^n`|2+2^n |       |             |      | reserved, n = 0...6                                                         |
|`XX...8*n`      |`xxxxxxxx...8*n`                  |  8*n |       |             |      | encrypted or reserved                                                       |
|`XX...n`        |`xxxxxxxx...n`                    |    n |       |             |      | reserved, n%8 != 0 && n != 2+2^m for m = 0...6                              |

- All packages are as encoded COBS/R sometimes 1 byte longer and always followed by the delimiter `00`byte.
- The ID map number can be deduced from the package length and needs no transmission.
  - So only the 12 lower ID bits are transmitted.

### Encoding table 1 legend

| Legend | Meaning                           |
| :-     | :---------------------------------|
| ...n   | totally n times                   |
| I\|iiii| 4 Id-bits (half byte)             |
| V\|vvvv| 4 value bits                      |
| X\|xxxx| 4 arbitrary bits (any half byte ) |
| C\|cccc| 4 cycle counter bits              |

### Encoding table 1 (with 4-bit cycle counter)

|half bytes      | same as bits                     | bytes|ID bits| ID range    |ID map| remark                                                                      |
| -              | -------------------------------- |:----:| :---: | :------:    |  :-: |     :-                                                                      |
|` `             | ` `                              |    0 |       |             |      | COBS/R padding byte                                                         |
|`II IC`         |`iiiiiiii iiiicccc`               |    2 |   12  | 0\-4095     |  0   | `TRICE0`                                                                    |
|`II IC VV`      |`iiiiiiii iiiicccc vvvvvvvv`      |    3 |   12  | 4096\- 8191 |  1   | `TRICE8_1`                                                                  |
|`II IC VV VV`   |`iiiiiiii iiiicccc vvvvvvvv...2`  |    4 |   12  | 8192\-12287 |  2   | `TRICE8_2`, `TRICE16_1`                                                     |
|`II IC VV...4`  |`iiiiiiii iiiicccc vvvvvvvv...4`  |    6 |   12  |12288\-16383 |  3   | `TRICE8_3`, `TRICE8_4`, `TRICE16_1`, `TRICE16_2`,  `TRICE32_1`              |
|`II IC VV...8`  |`iiiiiiii iiiicccc vvvvvvvv...8`  |   10 |   12  |16384\-20479 |  4   | `TRICE8_5`...`TRICE8_8`, `TRICE16_3`, `TRICE16_4`, `TRICE32_2`, `TRICE64_1` |
|`II IC VV...16` |`iiiiiiii iiiicccc vvvvvvvv...16` |   18 |   12  |20480\-24575 |  5   | `TRICE116_5`...`TRICE16_8`, `TRICE32_3`...`TRICE32_4`, `TRICE64_2`          |
|`II IC VV...32` |`iiiiiiii iiiicccc vvvvvvvv...32` |   34 |   12  |24576\-28671 |  6   | `TRICE32_5`...`TRICE32_8`, `TRICE64_3`...`TRICE64_4`                        |
|`II IC VV...64` |`iiiiiiii iiiicccc vvvvvvvv...64` |   66 |   12  |28672\-32767 |  7   | `TRICE64_5`...`TRICE64_8`                                                   |
|`XX...8*n`      |`xxxxxxxx...8*n`                  |  8*n |       |             |      | encrypted                                                                   |
|`XX...n`        |`xxxxxxxx...n`                    |    n |       |             |      | reserved, n%8 != 0 && n != 0, 2, 3, 4, 6, 10, 18, 34, 66                    |

### Encoding table 2 (with 8-bit cycle counter)

|half bytes      | same as bits                              | bytes|ID bits| ID range  |ID map| remark                                                                      |
| -              | --------------------------------          |:----:| :---: | :------:  |  :-: |     :-                                                                      |
|` `             | ` `                                       |    0 |       |           |      | COBS/R padding byte                                                         |
|`II IC`         |`iiiiiiii iiiiiiii cccccccc`               |    3 |   16  | 1 - 65535 |  0   | `TRICE0`                                                                    |
|`II IC VV`      |`iiiiiiii iiiiiiii cccccccc vvvvvvvv`      |    4 |   16  | 1 - 65535 |  1   | `TRICE8_1`                                                                  |
|`II IC VV VV`   |`iiiiiiii iiiiiiii cccccccc vvvvvvvv...2`  |    5 |   16  | 1 - 65535 |  2   | `TRICE8_2`, `TRICE16_1`                                                     |
|`II IC VV...4`  |`iiiiiiii iiiiiiii cccccccc vvvvvvvv...4`  |    7 |   16  | 1 - 65535 |  3   | `TRICE8_3`, `TRICE8_4`, `TRICE16_1`, `TRICE16_2`,  `TRICE32_1`              |
|`II IC VV...8`  |`iiiiiiii iiiiiiii cccccccc vvvvvvvv...8`  |   17 |   16  | 1 - 65535 |  4   | `TRICE8_5`...`TRICE8_8`, `TRICE16_3`, `TRICE16_4`, `TRICE32_2`, `TRICE64_1` |
|`II IC VV...16` |`iiiiiiii iiiiiiii cccccccc vvvvvvvv...16` |   19 |   16  | 1 - 65535 |  5   | `TRICE16_5`...`TRICE16_8`, `TRICE32_3`...`TRICE32_4`, `TRICE64_2`           |
|`II IC VV...32` |`iiiiiiii iiiiiiii cccccccc vvvvvvvv...32` |   35 |   16  | 1 - 65535 |  6   | `TRICE32_5`...`TRICE32_8`, `TRICE64_3`...`TRICE64_4`                        |
|`II IC VV...64` |`iiiiiiii iiiiiiii cccccccc vvvvvvvv...64` |   67 |   16  | 1 - 65535 |  7   | `TRICE64_5`...`TRICE64_8`                                                   |
|`XX...8*n`      |`xxxxxxxx...8*n`                           |  8*n |       |           |      | encrypted                                                                   |
|`XX...n`        |`xxxxxxxx...n`                             |    n |       |           |      | reserved, n%8 != 0 && n != 0, 2, 3, 4, 6, 10, 18, 34, 66                    |

## Fast TRICE data storing

