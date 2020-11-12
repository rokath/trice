# Byte level

## Main packet strucure
- All numbers are transmitted in network order (big endian).
- All values are in left-right order.
- One byte not used fo often is defined as ESC character, for example EC = 0xd3.
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
| Code                       | Meaning                    | max padding |
|----------------------------|----------------------------|-------------|
| EC LC IH IL .......... CR  | payload = 2^(LC-E0) bytes  |             |
|----------------------------|----------------------------|-------------|
| EC 00 ...                  | reserved                   |             | 
| EC ...                     | reserved                   |             | 
| EC DD ...                  | reserved                   |             | 
| EC DE = EC                 | real EC character          |             | 
| EC DE = EC                 | real EC character          |             | 
| EC DF IH IL            CR  | 16 bit ID no payload       |             | 
| EC E0 IH IL B0         CR  | 16 bit ID   1 byte payload |        0    | 
| EC E1 IH IL B0 B2      CR  | 16 bit ID   2 byte payload |        0    | 
| EC E2 IH IL B0 .. B3   CR  | 16 bit ID   4 byte payload |        1    | 
| EC E3 IH IL B0 .. B7   CR  | 16 bit ID   8 byte payload |        3    | 
| EC E4 IH IL B0 .. B15  CR  | 16 bit ID  16 byte payload |        7    | 
| EC E5 IH IL B0 .. B31  CR  | 16 bit ID  32 byte payload |       15    | 
| EC E6 IH IL B0 .. B63  CR  | 16 bit ID  64 byte payload |       31    | 
| EC E7 IH IL B0 .. B95  CR  | 16 bit ID  96 byte payload |       31    | 
| EC E8 IH IL B0 .. B127 CR  | 16 bit ID 128 byte payload |       31    | 
| EC E9 IH IL B0 .. B159 CR  | 16 bit ID 160 byte payload |       31    | 
| EC EA IH IL B0 .. B191 CR  | 16 bit ID 192 byte payload |       31    | 
| EC EB IH IL B0 .. B255 CR  | 16 bit ID 256 byte payload |       63    | 
| EC EC ...                  | reserved                   |             | 
| EC ...                     | reserved                   |             | 
| EC FF ...                  | reserved                   |             | 

- The interpretation is only dependent on the ID code.
- LC codes 0...DD & EC...FF reserved.


