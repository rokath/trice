/*! \file bare.c
\details The format strings are ignored - they go not into the target binary. See trice tool doc for details.
The trices (macros) are dumped as 32bit values into a 32 bit fifo. That is the time critical part.
\li A basic trice (subtrace) consists always of 4 byte: a 16 bit ID with a 16 bit data value.
\li Trices with more data are split into several 32bit basic trice values with IDs = 0 in front.
\li TRICE0, TRICE8_1, TRICE8_2 and TRICE16_1 have 4 bytes size, all others have 8 - 32 bytes size.
\li For trice transmission each basic trice gets a header of additional 4 bytes.
\li The header contains a startbyte, client and server address and a crc8 exOr checksum.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include "bare.h"

#if TRICE_CODE

//! trice fifo instance, here are the trices buffered.
ALIGN4 uint32_t triceFifo[ TRICE_FIFO_SIZE>>2 ] ALIGN4_END;

uint32_t triceFifoWriteIndexTrices = 0; //!< trice fifo write index, used inside macros, so must be visible

uint32_t triceFifoReadIndexBytes = 0; //!< trice fifo read index

uint32_t triceFifoMaxDepthBytes = 0; //!< diagnostics

#endif // #if TRICE_CODE
