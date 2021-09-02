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

How the packages are to interpret is a question of software configuration. When a decoded COBS/R package is to interpret, the known package length is used to choose the right interpreter. For example all multiple of 8 length packages are possibly XTEA encrypted. Also a fixed-size ID is usable. It is also possible to have several `trice` messages inside a packet. That makes sense to reach a multiple of 8-byte message length good for encryption.


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

- When an COBS/R package was successfully transmitted and decoded its interpretation is a matter of the general configuration.

| ID coding                              | package length    | ID bits |   ID range  |   ID map area   |  remark
| :------------------------------------- | -------------:    | ------: | ----------: |         -:      |  :-
| ``                                     |       0           |    0    |         0   |                 |  reserved
| `xxxxxxxx`                             |       1           |         |             |                 |  256 reserved for other protocol
| `1IIIIIII vvvvvvvv`                    |       2           |    7    | 0 ...   127 |     0 ...   127 |  128 Id's for 8-bit payload
| `01IIIIII IIIIIIII`                    |       2           |   14    | 0 ... 16383 |     0 ... 16383 |  2^14 Id's for no payload
| `00xxxxxx xxxxxxxx`                    |       2           |         |             |                 |  reserved


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
