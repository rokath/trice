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
uint8_t  triceBytesBuffer[8]; //!< bytes transmit buffer
int const triceBytesBufferDone = -1;
int triceBytesBufferIndex = triceBytesBufferDone;

//! TODO: endianess with compiler macros.
static void triceLoadTriceToBuffer( uint8_t* p, uint32_t t ){
    p[0] = (uint8_t)(t>>8); // IDHi big endian
    p[1] = (uint8_t)(t);    // IDLo big endian
    p[3] = (uint8_t)(t>>24);// DaHi litte endian -> should be changed to by[2] (needs many changes in trice.h but does not influence trice go code)
    p[2] = (uint8_t)(t>>16);// DaLo litte endian -> should be changed to by[3] (needs many changes in trice.h but does not influence trice go code)
}

//! triceServeOut must be called cyclically to proceed ongoing write out.
//! It schould be called at least every ms.
//! A possibe place is main loop.
void triceServeOut( void ){
	  int const syncPeriod = 3; // max every 100 ms or 200 trices a sync trice is needed
	  static int syncNeed = syncPeriod; // start with a sync trice
    if( triceBytesBufferDone == triceBytesBufferIndex ){ // bytes buffer empty and tx finished
        // next trice
        static uint8_t* const b0 = &triceBytesBuffer[0];
        static uint8_t* const b1 = &triceBytesBuffer[4];
        int n = triceFifoDepth();
        if( 1 < n || syncNeed < syncPeriod ){ // no need for sync trice
            triceLoadTriceToBuffer( b0, triceTricePop() ); // 1st trice
            triceLoadTriceToBuffer( b1, triceTricePop() ); // 2nd trice
					  syncNeed++;
        }else if( 1 <= n ) { // only 1 trice, so transmit also 1 sync trices
            triceLoadTriceToBuffer( b0, triceTricePop() ); // 1st trice
            triceLoadTriceToBuffer( b1, 0xcdef89ab ); // 2nd sync trice
					  syncNeed = 0;
        }else { // nothing to transmit, so transmit sync trices
            triceLoadTriceToBuffer( b0, 0xcdef89ab ); // 1st sync trice
            triceLoadTriceToBuffer( b1, 0xcdef89ab ); // 2nd sync trice
					  syncNeed = 0;
        }
        triceBytesBufferIndex = 0;
        // next byte
        triceTransmitData8( triceBytesBuffer[triceBytesBufferIndex++] );
        triceEnableTxEmptyInterrupt();
    }
}

#endif // #if TRICE_CODE
