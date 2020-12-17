# Trice encodings

There are different possibilities for encoding trices. The trice encoding inside the triceFifo can differ from the trice transmit format.

internal fifo buffer storage format:

- bare code format
  
  During runtime normally only the 16-bit ID 12345 (together with the parameters like hour, min, sec) is copied to a buffer. Execution time for a TRICE16_1 (as example) on a 48 MHz ARM can be about 16 systicks resulting in 250 nanoseconds duration, so you can use `trice` also inside interrupts or the RTOS scheduler to analyze task timings. The needed buffer space is one 32 bit word per normal trice (for up to 2 data bytes). A direct out transfer is possible but not recommended for serial output because of possible issues to re-sync in case of data loss. Just in case the internal bare fifo overflows, the data are still in sync.

  If the wrap format is desired as output the buffered 4 byte trice is transmitted as an 8 byte packet allowing start byte, sender and receiver addresses and CRC8 check to be used later in parallel with different software protocols.

  The bare output format contains exactly the bare bytes but is enriched with sync packages to achieve syncing. The sync package interval is adjustable.

- (direct) esc(ape) code format

  During untime the esc code format is generated immediately during the TRICE macro execution. This results in a slightly longer TRICE macro execution but allows the direct background transfer to the output device (UART or RTT memory) because re-sync is easy. One advantage of the esc format compared to bare is the more efficient coding of dynamic strings if you have lots of them.


Targest can send trices in different encodings

## Encoding `esc`

- Byte order is network order (bigendian)

## Encoding `bare`

- Byte order is network order (bigendian)
- Each trice is coded in one to eight 4-byte trice atoms.
- A trice atom consists of a 2 byte id and 2 bytes data.
- When a trice consists of several trice atoms only the last one carries the trice id. The others have a trice id 0.

## Encoding `wrap`

- This is the same as bare, but each trice atom is prefixed with a 4 byte wrap information:
  - 0xEB = start byte
  - 0x60 = source address
  - 0x60 = destination address
  - crc8 = 8 bit checksum over start byte, source and destination address and the 4 bare bytes.


## ESC encoding (to do: improve this)

The ESCaped encoding allowes a syncing to the next trice message on the behalf of the escape character `0xec`. It is always followed by a length code in the range `0xdf ... 0xeb`. All other bytes are reserved for future usesage despite the `0xde` byte. This is inserted as not counted value into the bytestream after an `0xec` to signal that this is an ordinary `0xec` byte inside the data stream. 

- All numbers are transmitted in network order (big endian).
- All values are in left-right order - first value comes first.
- As byte `0xec` is not used so often is is defined as ESC character: 
  - This is a copy from [trice.h lines 44-58](https://github.com/rokath/trice/blob/master/srcTrice.C/trice.h):

```c
#define TRICE_ESC  0xEC //!< Escape char is control char to start a package.
#define TRICE_DEL  0xDE //!< Delete char, if follower of TRICE_ESC, deletes the meaning os TRICE_ESC making it an ordinary TRICE_ESC char.
#define TRICE_P0   0xDF //!< No param char = If follower of TRICE_ESC only a 16 bit ID is inside the payload.
#define TRICE_P1   0xE0 //!< 1 byte param char = If follower of TRICE_ESC a 16 bit ID and 1 byte are inside the payload.
#define TRICE_P2   0xE1 //!< 2 byte param char = If follower of TRICE_ESC a 16 bit ID and 2 byte are inside the payload.
#define TRICE_P4   0xE2 //!< 4 byte param char = If follower of TRICE_ESC a 16 bit ID and 4 byte are inside the payload.
#define TRICE_P8   0xE3 //!< 8 byte param char = If follower of TRICE_ESC a 16 bit ID and 8 byte are inside the payload.
#define TRICE_P16  0xE4 //!< 16 byte param char = If follower of TRICE_ESC a 16 bit ID and 8 byte are inside the payload.
// #define TRICE_P32  0xE5
// #define TRICE_P64  0xE6
// #define TRICE_P128 0xE7
// #define TRICE_P256 0xE8
// #define TRICE_P512 0xE9
// #define TRICE_P1K  0xEA
// #define TRICE_P2K  0xEB
```

- EC ist the starting byte of a packet and is always followed by some additional bytes.
  - 2 bytes forming a 16 bit ID: IH & IL
  - Normally a 1-byte logarithmic length code LC. 
  - A number of bytes according LC
  - An ExOR CRC byte at the end, called CR
- If within the data to be transmitted EC occurs it stays on its place and is followed by an ~EC byte (probably 0x13 or 0xd4 or so).
  - This is called escaped EC.
- The additional byte inside an escaped EC is not counted and also not part of the CRC.
- If by chance the CRC is EC it is also escaped.
- The receiver waits for a non-escaped EC, gets the ID and LC, the bytes and the CRC at the end.
- LC: only lower nibble used
  - 0 = no payload
  - 1 =  1 byte payload
  - 2 =  2 byte payload
  - 3 =  4 byte payload (max  1 byte padding)
  - 4 =  8 byte payload (max  3 bytes padding)
  - 5 = 16 byte payload (max  7 bytes padding)
  - 6 = 32 byte payload (max 15 bytes padding)
  - 7 = 64 byte payload (max 31 bytes padding)
  - n = 2^(n-1) bytes payload
  - f = 2^14 = 16 KB payload (max 8191 bytes padding)
  - the higher nibble is reserved
- Examples
  - Shortest packet 5 bytes: ED IH IL 00 CR -> TRICE0( Id(6666), "msg:Hello World!\n" )
  - 13 bytes: ED IH IL 04 01 02 03 04 05 00 00 00 CR  (3 bytes padding) -> TRICE8_5( Id( 6667), "debug:%d,%d,%d,%d,%d", 1, 2, 3, 4, 5)
  - 21 bytes: ED IH IL 05 30 31 32 33 34 35 36 37 38 39 00 00 00 00 00 00 CR  (6 bytes padding) -> TRICES_1( Id( 6668), "att:%s\n", "0123456789" )
  - 13 bytes: ED IH IL 04 88 77 66 55 44 33 22 11 CR -> TRICE32_2( Id( 6669), "cmd:Add %d %d\n", 0x88776655, 0x44332211 )
  
| Code                   | Meaning                    | max padding | Remark
|------------------------|----------------------------|-------------|---------------------------------------------------------------------------------------------------------------
| EC LC IH IL ...        | payload = 2^(LC-E0) bytes  |             | LC is a length code
| EC 00 ...              | reserved                   |             | All packages starting with EC 00 until starting with EC DD are reserved.
| EC .. ...              |                            |             | All packages starting with EC 00 until starting with EC DD are reserved.
| EC DD ...              | reserved                   |             | All packages starting with EC 00 until starting with EC DD are reserved.
| EC DE = EC             | real EC character          |             | If inside the payload occures EC an uncounted DE is injected afterwards.
| EC DF IH IL            | 16 bit ID no payload       |             | TRICE0, special case: 2^-1 = 0 byte payload
| EC E0 IH IL B0         | 16 bit ID   1 byte payload |        0    | TRICE8_1, TRICE_S(""): 2^0 = 1 byte payload
| EC E1 IH IL B0 B1      | 16 bit ID   2 byte payload |        0    | TRICE8_2, TRICE16_1, TRICE_S("0")
| EC E2 IH IL B0 .. B3   | 16 bit ID   4 byte payload |        1    | TRICE8_3, TRICE8_4, TRICE16_2, TRICE32_1, TRICE_S("01"), TRICE_S("012")
| EC E3 IH IL B0 .. B7   | 16 bit ID   8 byte payload |        3    | TRICE8_5,... TRICE8_8, TRICE16_3, TRICE16_4, TRICE32_2, TRICE64_1, TRICE_S("0123"), ..., TRICE_S("01234567")
| EC E4 IH IL B0 .. B15  | 16 bit ID  16 byte payload |        7    | TRICE32_3, TRICE32_4, TRICE64_2, TRICE_S("012345678"), ..., TRICE_S("0123456789abcde")
| EC E5 IH IL B0 .. B31  | 16 bit ID  32 byte payload |       15    | TRICE_S("0123456789abcdef"), ... TRICE_S(strlen(31))
| EC E6 IH IL B0 .. B63  | 16 bit ID  64 byte payload |       31    | TRICE_S(strlen(32)), ...,TRICE_S(strlen(63))
| EC E7 IH IL B0 .. B95  | 16 bit ID  96 byte payload |       31    | TRICE_S(strlen(64)), ...,TRICE_S(strlen(95))
| EC E8 IH IL B0 .. B127 | 16 bit ID 128 byte payload |       31    | TRICE_S(strlen(96)), ...,TRICE_S(strlen(127))
| EC E9 IH IL B0 .. B159 | 16 bit ID 160 byte payload |       31    | TRICE_S(strlen(128)), ...,TRICE_S(strlen(159))
| EC EA IH IL B0 .. B191 | 16 bit ID 192 byte payload |       31    | TRICE_S(strlen(169)), ...,TRICE_S(strlen(191))
| EC EB IH IL B0 .. B255 | 16 bit ID 256 byte payload |       63    | TRICE_S(strlen(192)), ...,TRICE_S(strlen(255))
| EC EC ...              | reserved                   |             | All packages starting with EC EC until starting with EC FF are reserved.
| EC .. ...              | reserved                   |             | All packages starting with EC EC until starting with EC FF are reserved.
| EC FF ...              | reserved                   |             | All packages starting with EC EC until starting with EC FF are reserved.

- The interpretation is only dependent on the ID code.
- LC codes 0...DD & EC...FF reserved.

Macro                               | trice transfer sequence      | string bytes | padding bytes
------------------------------------|------------------------------|--------------|-----------
TRICE_S( Id(I), pFmt, "" )          | EC E0 IH IL 00               |       1      |      0
TRICE_S( Id(I), pFmt, "0" )         | EC E1 IH IL 30 00            |       2      |      0
TRICE_S( Id(I), pFmt, "01" )        | EC E2 IH IL 30 31 00 00      |       3      |      1
TRICE_S( Id(I), pFmt, "012" )       | EC E2 IH IL 30 31 32 00      |       4      |      0
TRICE_S( Id(I), pFmt, "0123" )      | EC E3 IH IL 30 31 32 33 00   |       5      |      3
...                                 | EC En IH IL    ...      00   |      ...     |      ...
TRICE_S( Id(I), pFmt, "..." )       | EC EB IH IL    ...      00   |      ...     |      ...
