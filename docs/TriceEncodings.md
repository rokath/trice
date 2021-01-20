# Trice encodings

## General

Trice bytes can be encodend in different flawors and it is easy to develop a different encoding.

## Quick start recommendation

- Use **pack2L** encoding if your target processor is a little endian mashine, otherwise use **pack2**
- The `trice` tool assumes **pack2L** per default, so no need for commandline switch `-enc pack2L`.

```c
#define TRICE_ENCODING TRICE_PACK2_ENCODING
```

## Overview

Inside the target project specific triceConfig.h is selectable:

- Encoding with more memory needs but a bit faster. (to do)
  - The trice macros are expanded in Assembler code.
  - If in the code only a few TRICE macrose this is the best choice.
- Encoding with less memory needs but a bit slower.
  - Each TRICE macro is a function call.
  - Good choice if in the code many TRICE macros.
  - Unused TRICE macro definitions could be deleted manually from the target code to reduce memory needs.
- Encoding in little or big endian.
  - The encoding should match the target processor endiannes.

  ```c
  #define TRICE_TRANSFER_ENDIANESS TRICE_LITTLE_ENDIANESS
  ```

- Additionally an encoding can be wrapped with transport information. This setting is done by calling cyclically this function:

```c
triceServeFifoWrappedToBytesBuffer();
```

 As example checkout wrapped bare encoding.

- Also it is possible to use encryption, which is shown as example for the wrapped bare encoding.

Currently these encodings are supported:

### `pack2` & `pacl2L` encoding

This is the recommended encoding.

- with cycle counter
- 20-bit IDs
- runtime strings up to 65535 chars
- target source: trice/srcTrice.C/intern/tricePack2Encoder.h 
- trice tool source: trice/internal/decoder/pack2Decoder.go
- trice tool test file: trice/internal/decoder/pack2Decoder_test.go

### `pack` & `packL` encoding

This is the pack2 & pack2L predecessor and kept for reference. 

- no cycle counter
- 16-bit IDs
- runtime strings up to 65535 chars
- target source: trice/srcTrice.C/intern/tricePackEncoder.h 
- trice tool source: trice/internal/decoder/packDecoder.go
- trice tool test file: trice/internal/decoder/packDecoder_test.go


### `bare` and `bareL` encoding

This was the first minimal implementation. Could be interstuing for 8-bit and 16-bit processors.

- no cycle counter
- 16-bit IDs
- no runtime strings
- target source: trice/srcTrice.C/intern/triceBareEncoder.h 
- trice tool source: trice/internal/decoder/bareDecoder.go
- trice tool test file: trice/internal/decoder/bareDecoder_test.go

### `wrap` and `wrapL` encoding

This is also `bare` & `bareL` encoding but with additional control bytes.

### `esc` encoding

This is a try-out escape sequence encoding implementation and kept for reference.

- no endianness choice
- no cycle counter
- 16-bit IDs
- runtime strings up to 255 chars
- target source: trice/srcTrice.C/intern/triceEscEncoder.h 
- trice tool source: trice/internal/decoder/escDecoder.go
- trice tool test file: trice/internal/decoder/escDecoder_test.go


### own encoding

To implement a different encoding:
- Copy trice/srcTrice.C/intern/trice*Any*Encoder.h, to trice/srcTrice.C/intern/trice*Own*Encoder.h.
- Adapt trice/srcTrice.C/intern/trice*Own*Encoder.h and integrate it in trice.h accordingly.
- Create a test project, copy and adapt triceConfig.h in the desired way.
- Copy trice/internal/decoder/*any*Decoder.go to trice/internal/decoder/**own**Decoder.go and adapt it.
- Integrate *own*Decoder.go accordingly.
- Write tests!

## Encoding `pack2` & `pack2L` (with cycle counter, 20-bit IDs, runtime strings up to 65535 chars)

The encoding is similar to `pack` & `packL` encoding with these differences:

`IIIICCCC` is replaced by `IIIIICNN` and  an optional following long count `LLLLcccc`

- `IIIII` = 20-bit ID
- `C` = 4-bit byte count
  - 0...12 = short count (no following long count)
  - 0xd = indicates a following long count
  - 0xe = reserved
  - 0xf = reserved
- `NN` = 8-bit cycle counter
- `LLLL` = 16-bit long count
- `cccc` = bit-inversed LLLL as check sum

## Encoding `pack` & `packL` (no cycle counter, 16-bit IDs, runtime strings up to 65535 chars)

All values up to 32 bit are combined 32 bit units in big (=network) or little endian order.
64-bit values are in the same byte order.

- `IIII` = 16-bit ID
- `CCCC` = byte count without counting padding bytes
- 0-3 padding 0-bytes fill the last 32-bit unit


```b
byte 3 2 1 0  | macro
--------------|-----------------------------------------------------------------
   0x89abcdef | TRICE16_1( Id(0x89ab), "inf:[ SYNCTRICE 0x89ab%04x ]", 0xcdef );
     IIIICCCC | TRICE0( Id(I), "..." );
     IIIICCCC | TRICE_S( Id(I), "...%s...", "" );
```

```b
byte 3 2 1 0    3 2 1 0      | macro
-----------------------------|-----------------------------------------------------
     IIIICCCC   000000b0     | TRICE8_1( Id(I), "...", b0 );             // cnt = 1
     IIIICCCC   0000b0b1     | TRICE8_2( Id(I), "...", b0, b1 );         // cnt = 2
     IIIICCCC   00b0b1b2     | TRICE8_3( Id(I), "...", b0, b1, b2 );     // cnt = 2
     IIIICCCC   b0b1b2b3     | TRICE8_4( Id(I), "...", b0, b1, b2, b3 ); // cnt = 3
     IIIICCCC   0000w0w0     | TRICE16_1( Id(I), "...", w0 );            // cnt = 2
     IIIICCCC   w0w0w1w1     | TRICE16_2( Id(I), "...", w0, w1 );        // cnt = 4
     IIIICCCC   d0d0d0d0     | TRICE32_1( Id(I), "...", d0 );            // cnt = 4
     IIIICCCC   000000aa     | TRICE_S( Id(I), "...%s...", "a" );        // cnt = 1
     IIIICCCC   0000aabb     | TRICE_S( Id(I), "...%s...", "ab" );       // cnt = 2
     IIIICCCC   00aabbcc     | TRICE_S( Id(I), "...%s...", "abc" );      // cnt = 3
     IIIICCCC   aabbccdd     | TRICE_S( Id(I), "...%s...", "abcd" );     // cnt = 4
```

```b
byte  3 2 1 0   3 2 1 0   3 2 1 0  | macro
-----------------------------------|---------------------------------------------------------------------
     IIIICCCC  b0b1b2b3  000000b4  | TRICE8_5( Id(I), "...", b0, b1, b2, b3, b4 );             // cnt = 5
     IIIICCCC  b0b1b2b3  0000b4b5  | TRICE8_6( Id(I), "...", b0, b1, b2, b3, b4, b5 );         // cnt = 6
     IIIICCCC  b0b1b2b3  00b4b5b6  | TRICE8_7( Id(I), "...", b0, b1, b2, b3, b4, b5, b6 );     // cnt = 7
     IIIICCCC  b0b1b2b3  b4b5b6b7  | TRICE8_8( Id(I), "...", b0, b1, b2, b3, b4, b5, b6, b7 ); // cnt = 8
     IIIICCCC  w0w0w1w1  0000w2w2  | TRICE16_3( Id(I), "...", w0, w1, w2 );                    // cnt = 6
     IIIICCCC  w0w0w1w1  w2w2w3w3  | TRICE16_4( Id(I), "...", w0, w1, w2, w3 );                // cnt = 8
     IIIICCCC  d0d0d0d0  d1d1d1d1  | TRICE32_2( Id(I), "...", d0, d1 );                        // cnt = 8
     IIIICCCC  l0l0l0l0  l0l0l0l0  | TRICE64_1( Id(I), "...", l0 );                            // cnt = 8
     IIIICCCC  aabbccdd  000000ee  | TRICE_S( Id(I), "...%s...", "abcde" );                    // cnt = 5
     IIIICCCC  aabbccdd  0000eeff  | TRICE_S( Id(I), "...%s...", "abcdef" );                   // cnt = 6
     IIIICCCC  aabbccdd  00eeffgg  | TRICE_S( Id(I), "...%s...", "abcdefg" );                  // cnt = 7
     IIIICCCC  aabbccdd  eeffgghh  | TRICE_S( Id(I), "...%s...", "abcdefgh" );                 // cnt = 8
```

and so on...

A [sync package](#sync-packages) can be inserted anytime between 2 trice but not inside a trice.

## Encoding `bare` & `bareL`

- Each trice is coded in one to eight 4-byte trice atoms.
- A trice atom consists of a 2 byte id and 2 bytes data.
- When a trice consists of several trice atoms, only the last one carries the trice id. The others have a trice id 0.

- `IIII` = 16-bit ID

```b
byte  3 2 1 0  | macro
---------------|--------------------------------
     IIII0000  | TRICE0( Id(I), "..." );
     IIII00b0  | TRICE8_1( Id(I), "...", b0 );
     IIIIb0b1  | TRICE8_2( Id(I), "...", b1 );
     IIIIw0w0  | TRICE16_1( Id(I), "...", w0 );
```

```b
byte  3 2 1 0    3 2 1 0  | macro
--------------------------|-----------------------------------------------
     0000b0b1   IIII00b2  | TRICE8_3( Id(I), "...", b0, b1, b2 );
     0000b0b1   IIIIb2b3  | TRICE8_4( Id(I), "...", b0, b1, b2, b3 );
     0000w0w0   IIIIw1w1  | TRICE16_2( Id(I), "...", w0, w1 );
     0000d0d0   IIIId0d0  | TRICE32_1( Id(I), "...", d0 );
```

```b
byte  3 2 1 0    3 2 1 0    3 2 1 0  | macro
-----------------------------------  |------------
     0000b0b1   0000b2b3   IIII00b4  | TRICE8_5( Id(I), "...", b0, b1, b2, b3, b4 );
     0000b0b1   0000b2b3   IIIIb4b5  | TRICE8_6( Id(I), "...", b0, b1, b2, b3, b4, b5 );
     0000w0w0   0000w1w1   IIIIw2w2  | TRICE16_3( Id(I), "...", w0, w1, w2);
```

```b
byte 3 2 1 0    3 2 1 0    3 2 1 0    3 2 1 0  | macro
-----------------------------------------------|------------
    0000b0b1   0000b2b3   0000b4b5   IIII00b6  | TRICE8_7( Id(I), "...", b0, b1, b2, b3, b4, b5, b6 );
    0000b0b1   0000b2b3   0000b4b5   IIIIb6b7  | TRICE8_8( Id(I), "...", b0, b1, b2, b3, b4, b5, b6, b7 );
    0000w0w0   0000w1w1   0000w2w2   IIIIw3w3  | TRICE16_3( Id(I), "...", w0, w1, w2, w3);
    0000d0d0   0000d0d0   0000d1d1   IIIId1d1  | TRICE32_2( Id(I), "...", d0, d1 );
    0000l0l0   0000l0l0   0000l0l0   IIIIl0l0  | TRICE64_1( Id(I), "...", l0 );
```

and so on...

The bare transmit format is exactly the same as the bare internal storage format with these differences:

During runtime normally only the 16-bit ID 12345 (together with the parameters like hour, min, sec) is copied to a buffer. Execution time for a TRICE16_1 (as example) on a 48 MHz ARM can be about 16 systicks resulting in 250 nanoseconds duration, so you can use `trice` also inside interrupts or the RTOS scheduler to analyze task timings. The needed buffer space is one 32 bit word per normal trice (for up to 2 data bytes). 

If the wrap format is desired as output the buffered 4 byte trice is transmitted as an 8 byte packet allowing start byte, sender and receiver addresses and CRC8 check to be used later in parallel with different software protocols.

The bare output format contains exactly the bare bytes but is enriched with 4 byte [sync packages](#sync-packages) mixed in at 4 byte offsets to achieve syncing. The sync package interval is adjustable.

## Encoding `wrap` & `wrapL`

This is the same as bare, but each trice atom is prefixed with a 4 byte wrap information:

- 0xEB = start byte
- 0x80 = source address
- 0x81 = destination address
- crc8 = 8 bit checksum over start byte, source and destination address, and the 4 bare bytes.

## Encoding `esc`

The `esc` encoding uses an escape character for syncing after some data loss. It is extendable.

- All numbers are transmitted in network order (big endian).
- All values are in left-right order - first value comes first.

An `esc` trice transfer packet consists of an 4-byte header followed by an optional payload.

|     Start Byte     |    Second Byte    |  Third Byte  | Fourth Byte
|--------------------|-------------------|--------------|--------------
|  Escape char `EC`  | Length Code `LC`  | triceID `IH` | triceID `IL`

### Start byte `EC`

```c
#define TRICE_ESC  0xEC //!< Escape char is control char to start a package.
```

The ESCaped encoding allowes a syncing to the next trice message on the behalf of the escape character `0xec`. It is
always followed by a length code in the range `0xdf ... 0xe8`. All other bytes are reserved for future usage despite
the `0xde` byte.

```c
#define TRICE_DEL  0xDE //!< Delete char, if follower of TRICE_ESC, deletes the meaning os TRICE_ESC making it an ordinary TRICE_ESC char.
```

This is inserted as not counted value into the bytes stream after an `0xec` to signal that this is an ordinary `0xec`
byte inside the data stream. As byte `0xec` is not used so often is is defined as ESC character:

### Length Code `LC`

The LC is a 1-byte logarithmic length code. This is a copy
from [trice.h lines 44-58](https://github.com/rokath/trice/blob/master/srcTrice.C/trice.h) and shows the length code
meaning:

```c
#define TRICE_P0   0xDF //!< No param char = If follower of TRICE_ESC only a 16 bit ID is inside the payload.
#define TRICE_P1   0xE0 //!< 1 byte param char = If follower of TRICE_ESC a 16 bit ID and 1 byte are inside the payload.
#define TRICE_P2   0xE1 //!< 2 byte param char = If follower of TRICE_ESC a 16 bit ID and 2 byte are inside the payload.
#define TRICE_P4   0xE2 //!< 4 byte param char = If follower of TRICE_ESC a 16 bit ID and 4 byte are inside the payload.
#define TRICE_P8   0xE3 //!< 8 byte param char = If follower of TRICE_ESC a 16 bit ID and 8 byte are inside the payload.
#define TRICE_P16  0xE4 //!< 16 byte param char = If follower of TRICE_ESC a 16 bit ID and 8 byte are inside the payload.
//                 0xE5 // dynamically used for runtime strings with size 17-32
//                 0xE6 // dynamically used for runtime strings with size 33-64
//                 0xE7 // dynamically used for runtime strings with size 63-128
//                 0xE8 // dynamically used for runtime strings with size 127-256
//                 0xE9 // reserved
//                 0xEA // reserved
//                 0xEB // reserved

```

### TriceID `IH` and `IL`

- The third and fourth byte are the 16 bit trice ID: IH & IL.
- The trice ID encodes one of the allowed trice macros, and a format string.
- The format string has some format specifiers accordingly to the trice macro.
- In the case of `TRICE_S` the format string contains one and only one `%s`.

### Payload

A number of bytes according LC is optionally following the header. If within the data to be transmitted an 0xEC occurs
it stays on its place and is followed by a not counted 0xDE byte to signal that this is no start byte.

- Generic description

```b
Code                  |Meaning                   |pad|Remark
----------------------|--------------------------|---|-----------------------------------------------------------------------------
EC LC IH IL ...       |payload = 2^(LC-E0) bytes |   |LC is a length code
EC 00 ...             |reserved                  |   |All packages starting with EC 00 until starting with EC DD are reserved.
EC .. ...             |reserved                  |   |All packages starting with EC 00 until starting with EC DD are reserved.
EC DD ...             |reserved                  |   |All packages starting with EC 00 until starting with EC DD are reserved.
EC DE = EC            |real EC character         |   |If inside the payload occures EC an uncounted DE is injected afterwards.
EC DF IH IL           |16 bit ID no payload      |  0|TRICE0, special case: 2^-1 = 0 byte payload
EC E0 IH IL B0        |16 bit ID   1 byte payload|  0|TRICE8_1, TRICE_S(""): 2^0 = 1 byte payload
EC E1 IH IL B0 B1     |16 bit ID   2 byte payload|  0|TRICE8_2, TRICE16_1, TRICE_S("0")
EC E2 IH IL B0 .. B3  |16 bit ID   4 byte payload|  1|TRICE8_3, TRICE8_4, TRICE16_2, TRICE32_1, TRICE_S("01"), TRICE_S("012")
EC E3 IH IL B0 .. B7  |16 bit ID   8 byte payload|  3|TRICE8_5,...TRICE8_8,TRICE16_3,TRICE16_4,TRICE32_2,TRICE64_1,TRICE_S("0...7")
EC E4 IH IL B0 .. B15 |16 bit ID  16 byte payload|  7|TRICE32_3, TRICE32_4, TRICE64_2, TRICE_S("0...e")
EC E5 IH IL B0 .. B31 |16 bit ID  32 byte payload| 15|TRICE_S("0123456789abcdef"), ... TRICE_S(strlen(31))
EC E6 IH IL B0 .. B63 |16 bit ID  64 byte payload| 31|TRICE_S(strlen(32)), ...,TRICE_S(strlen(63))
EC E7 IH IL B0 .. B127|16 bit ID 128 byte payload| 63|TRICE_S(strlen(64)), ...,TRICE_S(strlen(127))
EC E8 IH IL B0 .. B255|16 bit ID 256 byte payload|127|TRICE_S(strlen(128)), ...,TRICE_S(strlen(255))
EC E9 ...             |reserved                  |   |All packages starting with EC E9 until starting with EC FF are reserved.
EC .. ...             |reserved                  |   |All packages starting with EC E9 until starting with EC FF are reserved.
EC FF ...             |reserved                  |   |All packages starting with EC E9 until starting with EC FF are reserved.
```

- Examples See function `TestEsc` and `TestEscDynStrings` in
  file [decoder_test.go](https://github.com/rokath/trice/blob/master/internal/decoder/decoder_test.go).

## Sync packages

- The frequency is adjustable and could be every 100ms or 40 bytes.
- The PC `trice` tool removes them silently.
- A sync package is `0x89 0xab 0xcd 0ef`.
- The PC `trice` tool does not use

```c
//! TRICE_SYNC is an optional trice sync message for syncing, when bare transmission is used.
//! The value 35243 (0x89ab) is a reserved pattern used as ID with value DA 0xcdef.
//! The hex notation protects against unwanted automatic changes.
//! The byte sequence of the sync message is 0x89 0xab 0xcd 0xef.
//! It cannot occure in the trice stream in another way due to ID generaion policy.
//! Sync package is IDDA=89abcdef
//!
//! To avoid wrong syncing these ID's are excluded: xx89, abcd, cdef, efxx (514 pieces)
//!
//! Possible:    IH IL DH DL IH IL DH DL IH IL DH DL (1 right)
//!              xx xx xx xx xx 89 ab cd ef xx xx xx -> avoid IL=89, IH=ef
//!
//! Possible:    IH IL DH DL IH IL DH DL IH IL DH DL (2 right)
//!              xx xx xx xx xx xx 89 ab cd ef xx xx -> avoid ID=cdef
//!
//! Possible:    IH IL DH DL IH IL DH DL IH IL DH DL (3 right)
//!              xx xx xx xx xx xx xx 89 ab cd ef xx -> avoid ID=abcd
//!
//! Sync packet: IH IL DH DL IH IL DH DL IH IL DH DL
//!              xx xx xx xx 89 ab cd ef xx xx xx xx -> use ID=89ab with DA=cdef as sync packet
//!
//!  Possible:   IH IL DH DL IH IL DH DL IH IL DH DL (1 left)
//!              xx xx xx 89 ab cd ef xx xx xx xx xx -> avoid ID=abcd
//!
//!  Possible:   IH IL DH DL IH IL DH DL IH IL DH DL (2 left)
//!              xx xx 89 ab cd ef xx xx xx xx xx xx -> avoid ID=cdef
//!
//!  Possible:   IH IL DH DL IH IL DH DL IH IL DH DL (3 left)
//!              xx 89 ab cd ef xx xx xx xx xx xx xx ->  avoid IL=nn89, IH=ef
//!
//! If an ID=89ab with DA!=cdef is detected -> out of sync!
//! If an IH=ef is detected -> out of sync, all 256 IDs starting with 0xef are excluded
//! If an IL=89 is detected -> out of sync, all 256 IDs ending with 0x89 are excluded
//! If an ID=abcd is detected -> out of sync, ID 0xabcd is excluded
//! If an ID=cdef is detected -> out of sync, ID 0xcdef is excluded
//! ID 0x89ab is reserved for this trice sync package.
//! The trice sync message payload must be 0xcdef.
//! You must not change any of the above demands. Otherwise the syncing will not work.
//! The Id(0x89ab) is here as hex value, so it is ignored by ID management.
//! The trice sync string makes the trice sync info invisible just in case,
//! but the trice tool will filter them out anyway. The trice tool automatic id generation
//! follows these rules.
//#define TRICE_SYNC do{ TRICE16_1( Id(0x89ab), "%x\b\b\b\b", 0xcdef ); }while(0)
```
