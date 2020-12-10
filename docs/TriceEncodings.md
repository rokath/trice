# Trice encodings

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