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
ALIGN4 uint32_t triceFifo[ TRICE_FIFO_BYTE_SIZE>>2 ] ALIGN4_END;

uint32_t triceFifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible

uint32_t triceFifoReadIndex = 0; //!< trice fifo read index

uint32_t triceFifoMaxDepthTrices = 0; //!< diagnostics

uint8_t triceBytesBuffer[8]; //!< bytes transmit buffer
int triceBytesBufferIndex = sizeof(triceBytesBuffer)+1; // 9

//! TODO: endianess with compiler macros.
static void triceLoadTriceToBuffer( uint8_t* p, uint32_t t ){
    p[0] = (uint8_t)(t>>8); // IDHi big endian
    p[1] = (uint8_t)(t);    // IDLo big endian
    p[3] = (uint8_t)(t>>24);// DaHi litte endian -> should be changed to by[2] (needs many changes in trice.h but does not influence trice go code)
    p[2] = (uint8_t)(t>>16);// DaLo litte endian -> should be changed to by[3] (needs many changes in trice.h but does not influence trice go code)
}

//! triceServeOut() must be called cyclically to proceed ongoing write out.
//! A possibe place is main loop.
void triceServeOut( void ){
    if( triceBytesBufferIndex < sizeof(triceBytesBuffer)+1 ){ // bytes buffer not empty or tx not finished
        return; // nothing to do
    }
    // next trice
    int n = triceFifoDepth();
    if( 0 == n ){
        return; // nothing to transmit
    }else{
        uint32_t trice = triceTricePop();
        triceLoadTriceToBuffer( triceBytesBuffer, trice ); // 1st trice
        if( 1 == n ){ // only one trice to transmit
            trice = 0xcdef89ab; // sync trice as 2nd trice
        }else{  // at least 2 trices to transmit
            trice = triceTricePop();
        }
        triceLoadTriceToBuffer( triceBytesBuffer+sizeof(trice), trice ); // 2nd trice
        triceBytesBufferIndex = 0;
        // next byte
        triceTransmitData8( triceBytesBuffer[triceBytesBufferIndex++] );
        triceEnableTxEmptyInterrupt();
    }
}

#endif // #if TRICE_CODE
