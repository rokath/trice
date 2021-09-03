# COBS/R encoding

- Packages are [COBS/R](https://pythonhosted.org/cobs/cobsr-intro.html) encoded.
- Separator byte is `00`. That means the packages are without containing `00` bytes and separated by a `00` byte.
- This allows the transfer of n-byte packages without the need to decide the meaning of the payload, means, how many bits are ID and how many bits are value is simply a configuration question.
- The COBS/R encoding usually has the same length as the unencoded data and sometimes has one byte more but an additional 00 is needed for secure package separation.
- This way the ID bit count is adjustable to the real communication needs because a data disturbance is easily detectable by just waiting for the next 0.
- One important point is the possibility to embed additional protocols in the data stream.

## COBS/R encoding for 0-byte packages

- This is simply an empty package. Just the `00` package separator byte is transmitted.
- It is normally not used but could get a meaning in some applications.

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

## Decoded COBS/R package interpreter

- After receiving and decoding a COBS/R package, the receiver can decide according to the package length and its starting bits what to do with it:
  - Package lengths 2, 3, 4, 6, 10, 18, 34, 66 starting with four 0-bits are trice logs.
    - Treat as received *trice* message
  - Multiple of 8 bytes packages are intended for XTEA encryption.
    - The decoded packet is treated again as a COBS/R encoded byte stream and handled recursive the same way.
    - This way several COBS/R encoded data packages can be joint in one package for encryption. Padding `00` bytes are used to reach a multiple of 8 package length.  
  - All other packages are useable for user protocols (marked as reserved).
    - Ignore, Route forward or call user handler.
    - 1-byte COBS/R packages are not recommended for numerous data, because of the delimiter byte are only ~50% bandwidth usable.

### Encoding table legend

| Legend | Meaning                                                           |
| :-     | :---------------------------------------------------------------- |
| ...n   | totally n times                                                   |
| I\|iiii| 4 Id-bits (half byte)                                             |
| V\|vvvv| 4 value bits                                                      |
| X\|xxxx| 4 arbitrary bits (any half byte )                                 |
| Y\|yyyy| 4 arbitrary bits, but at least one must be 1 (any half byte != 0) |

### Encoding table

|half bytes     |as bits                          | bytes|ID bits| ID range|ID map| remark
|:-             |:--------------------------------|-----:| ----: |--------:|    -:| :-
|` `            | ` `                             |    0 |       |         |      | used as padding byte in a COBS/R sequence
|`0I II`        |`0000iiii iiiiiiii`              |    2 |   12  | 0\-4095 |  0   | 2^12  Id's for no payload
|`YX XX`        |`yyyyxxxx xxxxxxxx`              |    2 |       |         |      | reserved
|`0I II VV`     |`0000iiii iiiiiiii vvvvvvvv`     |    3 |   12  | 0\-4095 |  1   | 2^12  Id's for a 1-byte payload
|`YX XX XX`     |`yyyyxxxx xxxxxxxx xxxxxxxx`     |    3 |       |         |      | reserved
|`0I II VV VV`  |`0000iiii iiiiiiii vvvvvvvv...2` |    4 |   12  | 0\-4095 |  2   | 2^12  Id's for a 2-bytes payload
|`YX XX XX XX`  |`yyyyxxxx xxxxxxxx xxxxxxxx...2` |    4 |       |         |      | reserved
|`0I II VV...4` |`0000iiii iiiiiiii vvvvvvvv...4` |    6 |   12  | 0\-4095 |  3   | 2^12  Id's for a 4-bytes payload
|`YX XX XX...4` |`yyyyxxxx xxxxxxxx xxxxxxxx...4` |    6 |       |         |      | reserved
|`0I II VV...8` |`0000iiii iiiiiiii vvvvvvvv...8` |   10 |   12  | 0\-4095 |  4   | 2^12  Id's for a 8-bytes payload
|`YX XX XX...8` |`yyyyxxxx xxxxxxxx xxxxxxxx...8` |   10 |       |         |      | reserved
|`0I II VV...16`|`0000iiii iiiiiiii vvvvvvvv...16`|   18 |   12  | 0\-4095 |  5   | 2^12  Id's for a 16 bytes payload
|`YX XX XX...16`|`yyyyxxxx xxxxxxxx xxxxxxxx...16`|   18 |       |         |      | reserved
|`0I II VV...32`|`0000iiii iiiiiiii vvvvvvvv...32`|   34 |   12  | 0\-4095 |  6   | 2^12  Id's for a 32 bytes payload
|`YX XX XX...32`|`yyyyxxxx xxxxxxxx xxxxxxxx...32`|   34 |       |         |      | reserved
|`0I II VV...64`|`0000iiii iiiiiiii vvvvvvvv...64`|   66 |   12  | 0\-4095 |  7   | 2^12  Id's for a 16 bytes payload
|`YX XX XX...64`|`yyyyxxxx xxxxxxxx xxxxxxxx...64`|   66 |       |         |      | reserved
|`XX...8*n`     |`xxxxxxxx...8*n`                 |  8*n |       |         |      | encrypted or reserved
|`XX...n`       |`xxxxxxxx...n`                   |    n |       |         |      | reserved n%8 !=0

- The ID map number can be deduced from the package length and needs no transmission.

| ID coding                                                                                   | package length    | ID bits |   ID range  |   ID map area   |  remark
| :-------------------------------------                                                      | -------------:    | ------: | ----------: |         -:      |  :-
| ``                                                                                          |       0           |         |             |                 | reserved
| `xxxxxxxx`                                                                                  |       1           |         |             |                 | reserved
| `1000IIII IIIIIIII`                                                                         |       2           |   15    | 0 ... 32767 |                 | 2^15  Id's for no payload
| `0xxxxxxx xxxxxxxx`                                                                         |       2           |         |             |                 | reserved
| `1IIIIIII IIIIIIII vvvvvvvv`                                                                |       3           |   15    | 0 ... 32767 |                 | 2^15  Id's for a 1-byte payload
| `0xxxxxxx xxxxxxxx xxxxxxxx`                                                                |       3           |         |             |                 | reserved
| `1IIIIIII IIIIIIII vvvvvvvv vvvvvvvv`                                                       |       4           |   15    | 0 ... 32767 |                 | 2^15  Id's for a 2-bytes payload
| `0xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx`                                                       |       4           |         |             |                 | reserved
| `xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx`                                              |       5           |         |             |                 | reserved
| `1IIIIIII IIIIIIII vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv`                                     |       6           |   15    | 0 ... 32767 |                 | 2^15  Id's for a 4-bytes payload
| `0xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx`                                     |       6           |         |             |                 | reserved
| `xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx`                            |       7           |         |             |                 | reserved
| `eeeeeeee eeeeeeee eeeeeeee eeeeeeee eeeeeeee eeeeeeee eeeeeeee eeeeeeee`                   |       8           |         |             |                 | encrypted message
| `dddddddd dddddddd dddddddd dddddddd dddddddd dddddddd dddddddd dddddddd`                   |       8           |         |             |                 | decrypted message (details tbd.)
| `xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx`          |       9           |         |             |                 | reserved
| `1IIIIIII IIIIIIII vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv` |      10           |   15    | 0 ... 32767 |                 | 2^15  Id's for a 8-bytes payload
| `0xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx` |      10           |         |             |                 | reserved
| `X * xxxxxxxx`                                                                              | 11\-17 & 19\-...  |         |             |                 | reserved
| `1IIIIIII IIIIIIII + 16 * vvvvvvvv`                                                         |      18           |   15    | 0 ... 32767 |                 | 2^15  Id's for a 16 bytes payload

## Data encryption and double COBS/R encoding

- All decoded COBS/R packets with a byte count not equal 8, 16, 32, 64 or 128 are not encrypted.
- Decoded COBS/R packets with a byte count equal 8, 16, 32, 64 or 128 are assumed to be encrypted.
- Despite encrypted or not, these form after a possible decryption, or directly, a separate COBS/R sequence. That means, these packet lengths are double COBS/R encoded.
  - So lets receive 5 COBS/R packages (delimited by a `00`), which after decoding each 8 bytes, all together a 40 bytes stream, which can contain some `00` bytes.
  - Probably some padding `00` bytes at the end.
  - Than this is treated again as COBS/R encoded stream to get the original packages for interpretation.



### Legacy stuff

- Examples for ID - value apportionment (these are only thinkable options):

Hint: The value space itself is usable according to ID, for example a 32 bit value space could be a 16-bit and two 8-bit values.

| package byte count |   optional usage                      | apportionment example
| -----------------: |   --------------------------          | :-
|                  2 |   on|off time measurement             | `Ivvvvvvv vvvvvvvv`
|                  2 |   256 different value events          | `IIIIIIII vvvvvvvv`
|                  8 | **XTEA, 256 different 56 bit values** | `IIIIIIII vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv`
|                 10 |   65535 different 64 bit values       | `IIIIIIII IIIIIIII vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv`
|                  6 |   65535 different 32 bit values       | `IIIIIIII IIIIIIII vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv`
|                  4 |   65535 different 16 bit values       | `IIIIIIII IIIIIIII vvvvvvvv vvvvvvvv`
|                  3 |   65535 different  8 bit values       | `IIIIIIII IIIIIIII vvvvvvvv`
|                  2 |   65535 different no-value messages   | `IIIIIIII IIIIIIII`
|                  1 |   8 different  5 bit values           | `IIIvvvvv`
|                  1 |   256 different no-value messages     | `IIIIIIII`
|                  4 |   32768 different 17-bit messages     | `IIIIIIII IIIIIIIx vvvvvvvv vvvvvvvv`
|                  8 |   XTEA, 2 messages in one packet      | `IIIIIIII IIIIIIII vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv IIIIIIII vvvvvvvv`
|                 16 |   XTEA, 5 messages in one packet      | `IIIIIIII IIIIIIII vvvvvvvv vvvvvvvv vvvvvvvv vvvvvvvv IIIIIIII vvvvvvvv IIIIIIII IIIIIIII vvvvvvvv vvvvvvvv IIIIIIII vvvvvvvv IIIIIIII IIIIIIII`

- To keeps things simple:
- The first byte in a package is an ID byte optionally followed by more ID bytes

| ID coding                    | ID bits |         ID range  | remark
| ---------------------------: | ------: | ----------------: | -
| `0IIIIIII`                   |  7      |     0 ...     127 | 
| `10IIIIII IIIIIIII`          | 14      |   128 ...   16383 | 0 ...   127 unused (reserved)
| `100IIIII IIIIIIII IIIIIIII` | 21      | 16384 ... 2097151 | 0 ... 16383 unused (reserved)

- How many value bits are following an ID and how they are to interpret is coded inside the ID information.
-->



| ID coding                              | package length    | ID bits |   ID range  |   ID map area   |  remark
| :------------------------------------- | -------------:    | ------: | ----------: |         -:      |  :-
| `1IIIIIII vvvvvvvv vvvvvvvv`           |       3           |    7    | 0 ...   127 |   384 ...   511 |  2 value bytes, avoid IDs 0 and 1 for fast COBS/R encoding
| `00IIIIII IIIIIIII vvvvvvvv`           |       3           |   14    | 0 ... 16383 | 16384 ... 32767 |  1 value byte
| `01xxxxxx xxxxxxxx xxxxxxxx`           |       3           |         |             |        -        |  2^22 packets unused (reserved), usable as a special 3-bytes message
| `1IIIIIII vvvvvvvv vvvvvvvv vvvvvvvv`  |       4           |    7    | 0 ...   127 |   512       639 |  3 value bytes, avoid IDs 0 and 1 for fast COBS/R encoding
| `00IIIIII IIIIIIII vvvvvvvv vvvvvvvv`  |       4           |   14    | 0 ... 16383 | 32768 ... 49151 |  2 value bytes
| `01xxxxxx xxxxxxxx xxxxxxxx xxxxxxxx`  |       4           |         |             |        -        |  2^14 packets unused (reserved), usable as a special 4-bytes message
| 5 ... 7 `xxxxxxxx`                     |     5,6,7         |         |             |        -        |  reserved, usable as a special 5-7-bytes message
| `1IIIIIII` + 7 `vvvvvvvv`              |       8           |    7    | 0 ...   127 |   640 ...   767 |  7 value bytes, avoid IDs 0 and 1 for fast COBS/R encoding
| `00IIIIII IIIIIIII` + 6 `vvvvvvvv`     |       8           |   14    | 0 ... 16383 | 49152 ... 65535 |  6 value bytes
| `01xxxxxx` + 7 `xxxxxxx`               |       8           |         |             |        -        |  2^30 packets unused (reserved), usable as a special 4-bytes message
| 9 ... 15 `xxxxxxxx`                    |9,10,11,12,13,14,15|         |             |        -        |  reserved, usable as a special 9-15-bytes message
| `1IIIIIII` + 15 `vvvvvvvv`             |      16           |    7    | 0 ...   127 |   768 ...   895 | 15 value bytes
| `00IIIIII IIIIIIII` + 14 `vvvvvvvv`    |      16           |   14    | 0 ... 16383 | 65536 ... 81919 | 14 value bytes
| `01xxxxxx` + 15 `xxxxxxxx`             |      16           |         |             |        -        |  2^30 packets unused (reserved), usable as a special 4-bytes message
| n `xxxxxxxx`                           |       n           |         |             |        -        |  n > 16, n mod 8 != 0, reserved, usable as a special n-bytes message
| `1IIIIIII` + (n-1) `vvvvvvvv`          |       n           |    7    | 0 ...   127 |   896 ...  1023 |  n > 16, n mod 8 == 0, n-1 value bytes
| `00IIIIII IIIIIIII` + (n-2) `vvvvvvvv` |       n           |   14    | 0 ... 16383 | 81920 ... 98304 |  n > 16, n mod 8 == 0, n-2 value bytes
| `01xxxxxx` + (n-1) `xxxxxxxx`          |       n           |         |             |        -        |  n > 16, n mod 8 == 0, n-2 value bytes, 2^((n-1)*8+6) packets unused (reserved), usable as a special 4-bytes message

If less value bytes are needed padding bytes are used.

`Trice( DESCRIPTOR, Id(0), "fmtString", ... )`

| Legacy notation                    | COBSR notation                       | ID coding
| :-                                 | :-                                   | :-
| Trice0( id(0), "text" );           | Trice0( IDE(0), "text" );            | `00IIIIII IIIIIIII`
|                                    | TriceB( ID7(0), "text", 1 );         | `IIIIIIIv`
| Trice8( id(0), "text", 255 );      | Trice8( ID7(0), "text", 255 );       | `1IIIIIII vvvvvvvv`
| Trice8( id(0), "text", 255, 255 ); | Trice8( ID7(0), "text", 255, 255 );  | `1IIIIIII vvvvvvvv vvvvvvvv`
| Trice16( id(0), "text", 65535 );   | Trice16( ID7(0), "text", 65535 );    | `1IIIIIII vvvvvvvv vvvvvvvv`
| trice8( Id(0), "text" );           | Trice8( IDE(0), "text", 255 );       | `00IIIIII IIIIIIII vvvvvvvv` 
| trice16( Id(0), "text" );          | Trice16( IDE(0), "text", 65535 );    | `00IIIIII IIIIIIII vvvvvvvv vvvvvvvv`

ID7(n) =  7-bit ID
IDE(n) = 14-bit ID
