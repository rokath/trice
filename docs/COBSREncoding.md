# COBS/R encoding design draft

## Table of Contents

- [COBS/R encoding design draft](#cobsr-encoding-design-draft)
  - [Table of Contents](#table-of-contents)
  - [Preface](#preface)
  - [COBS/R encoding for 0\-byte packages](#cobsr-encoding-for-0-byte-packages)
  - [COBS/R encoding for 1\-byte packages](#cobsr-encoding-for-1-byte-packages)
  - [COBS/R encoding for 2\-byte packages](#cobsr-encoding-for-2-byte-packages)
  - [COBS/R encoding for n\-byte packages](#cobsr-encoding-for-n-byte-packages)
  - [Decoded COBS/R package interpreter](#decoded-cobsr-package-interpreter)
    - [Encoding table legend](#encoding-table-legend)
    - [Encoding table](#encoding-table)

(Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc.go))

## Preface

- Packages are [COBS/R](https://pythonhosted.org/cobs/cobsr-intro.html) encoded.
- Separator byte is `00`. That means the packages are without containing `00` bytes and separated by a `00` byte.
- This allows the transfer of n-byte packages without the need to decide the meaning of the payload, means, how many bits are ID and how many bits are value is simply a configuration question.
- The COBS/R encoding usually has the same length as the unencoded data and sometimes has one byte more but an additional 00 is needed for secure package separation.
- This way the ID bit count is adjustable to the real communication needs because a data disturbance is easily detectable by just waiting for the next 0.
- One important point is the possibility to embed additional protocols in the data stream.

## COBS/R encoding for 0-byte packages

- This is simply an empty package. Just the `00` package separator byte is transmitted.
- It is normally used as padding byte to reach a multiple of 8 bytes package length.

## COBS/R encoding for 1-byte packages

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

## COBS/R encoding for 2-byte packages

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

- Two byte packages are fast COBS/R codable by simply using the subset >= 0300 and using it directly but replacing a possible 2nd 00 with the first byte and putting 01 on the first position.

## COBS/R encoding for n-byte packages

- This looks similar to 1-byte and 2-byte encoding and is not shown here.
- Some super fast code for 3- and 4-byte packet encoding is also possible.
- All *trice* packages are much shorter than 255 bytes so the COBS/R encoding is cheap.

## Decoded COBS/R package interpreter

- After receiving and decoding a COBS/R package, the receiver can decide according to the package length and its starting bits what to do with it:
  - Package lengths 2, 3, 4, 6, 10, 18, 34, 66 starting with four 0-bits are trice logs.
    - Treat as received *trice* message
  - Multiple of 8 bytes packages are intended for XTEA encryption.
    - The decrypted packet is treated again as a COBS/R encoded byte stream and handled recursive the same way.
    - This way several COBS/R encoded data packages can be joint in one package for encryption.
      - Empty COBS/R packages are `00` bytes and used to reach a multiple of 8-bytes COBS/R sequence.
  - All other packages are useable for other protocols (marked as reserved).
    - Ignore, route forward or call user handler.
    - 1-byte COBS/R packages are not recommended for numerous data. Because of the delimiter byte, are only ~50% bandwidth usable.

### Encoding table legend

| Legend | Meaning                                                           |
| :-     | :---------------------------------------------------------------- |
| ...n   | totally n times                                                   |
| I\|iiii| 4 Id-bits (half byte)                                             |
| V\|vvvv| 4 value bits                                                      |
| X\|xxxx| 4 arbitrary bits (any half byte )                                 |
| Y\|yyyy| 4 arbitrary bits, but at least one must be 1 (any half byte != 0) |

### Encoding table

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
