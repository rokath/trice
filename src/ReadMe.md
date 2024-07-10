# Target Setup

* See ./doc/TriceUserGuide.md -> [Embedded system code setup](https://github.com/rokath/trice/blob/master/docs/TriceUserGuide.md#24-port-it)

## Issue Tracking

### L432 FreeRTOS, `TRICE_RING_BUFFER` with RTT but without reading RTT @115200

```bash
Jul  9 10:24:58.607462  com3:          triceCheck.c   167
COBS: 08 98 cb 32 32 32 32 3b 01 00 
COBS:          32 32 32 32 5b 01 00 
inconsistent COBS buffer!

Jul  9 10:24:57.006345  com3:          triceCheck.c  1534  842,150_450 1b43 triceSingleDepthMax = 168 of 168 (TRICE_BUFFER_SIZE)
COBS: 07 41 5a 31 04 98 07 01 01 00 
COBS:       5a 6c 04 d3 31 01 01 00 
inconsistent COBS buffer!

Jul  9 11:00:54.395380  com3:          triceCheck.c   280  842,150_450 1afd TRICE32_2 line 280,2
COBS: 0b 59 cd 32 32 32 32 3c 10 19 01 01 01 01 01 01 02 02 01 01 01 01 01 01 01 00 
COBS:    59 cd 32 32 32 32 f7 10 19 01 01 01 01 01 01 02 02 01 01 01 01 01 01 01 00 
inconsistent COBS buffer!

Jul  9 11:02:03.653064  com3:          triceCheck.c  1515              0f89 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJK
COBS: 35 89 4f 09 30 30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 78 79 7a 41 42 43 44 45 46 47 48 49 4a 4b 4c 00 
COBS:       4f c4 30 30 31 32 33 34 35 36 37 38 39 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 78 79 7a 41 42 43 44 45 46 47 48 49 4a 4b 4c 00 
inconsistent COBS buffer!

Jul  9 11:03:33.892756  com3:          triceCheck.c   182  842,150_450 04ce TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
COBS: 29 8b d6 32 32 32 32 65 20 b0 ef 9f df 8f cf 7f bf 50 10 60 20 70 30 80 c0 50 10 60 20 70 30 80 c0 50 10 60 20 70 30 80 c0 00 
COBS: 29 8b d6 32 32 32 32 1f 20 b0 ef 9f df 8f cf 7f bf 50 10 60                            70 30 80 c0 50 10 60 20 70 30 80 c0 00 
inconsistent COBS buffer!

Jul  9 11:21:09.519959  com3:          triceCheck.c   298  842,150_450 1c8d Runtime generated strings
COBS: 0a 15 c8 32 32 32 32 6f 04 0c 01 01 01 00 
COBS:                         04       01 01 00 
inconsistent COBS buffer!

Jul  9 11:22:30.968562  com3:          triceCheck.c   266  842,150_450 1e71 TRICE STOP time message
COBS: 08 83 c7 32 32 32 32 88 01 00 
COBS:       c7 32 32 32 32 43 01 00 
inconsistent COBS buffer!

Jul  9 11:23:43.414792  com3:          triceCheck.c   137  842,150_450 1620 12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
COBS: 1f b8 8a 16 16 b6 18 ff ff fe ff fd ff fc ff fb ff fa ff f9 ff f8 ff f7 ff f6 ff f5 ff f4 ff 00 
COBS:             16 71 18 ff ff fe ff fd ff fc ff fb ff fa ff f9 ff f8 ff f7 ff f6 ff f5 ff f4 ff 00 
inconsistent COBS buffer!
```

### L432 FreeRTOS, `TRICE_RING_BUFFER` without RTT @115200

```bash
Jul  9 11:57:15.313061  com3:          triceCheck.c   167              1544 value=88
COBS: 08 98 cb 32 32 32 32 98 01 00
COBS:                32 32 2e 01 00 
inconsistent COBS buffer!

Jul  9 12:00:44.139512  com3:          triceCheck.c   277  842,150_450 0f9e TRICE64 1, 2
COBS: 0b db d1 32 32 32 32 b5 04 16 01 01 01 00
COBS:          32 32 32 32 6b 04 16 01 01 01 00 
inconsistent COBS buffer!


Jul  9 12:05:16.403643  com3:          triceCheck.c   181  842,150_450 1670 TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
COBS: 29 ce c4 32 32 32 32 a2 20 fa fd ff ff ff ff ff ff 50 10 60 20 70 30 80 c0 50 10 60 20 70 30 80 c0 50 10 60 20 70 30 80 c0 00 
COBS:                      5b 20 fa fd ff ff ff ff ff ff 50 10 60 20 70 30 80 c0 50 10 60 20 70 30 80 c0 50 10 60 20 70 30 80 c0 00 
inconsistent COBS buffer!


Jul  9 12:06:41.752570  com3:          triceCheck.c   188  842,150_450 0574 With TRICE_S:abcde 12345
COBS: 14 7a d4 32 32 32 32 c9 0b 61 62 63 64 65 20 31 32 33 34 35 00
COBS: 14 7a d4 32 32 32 32 7f 0b 61 62 63 64 65 20 31       
inconsistent COBS buffer!
```

