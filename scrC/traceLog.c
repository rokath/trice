/*! \file TraceAll.c
\brief tracelogs to transfer buffers
\details The tracelogs are dumped as 32bit values into a 32 bit fifo.
That is the time critical part. 
\li a tracelog is a 16 bit ID with a 16 bit data value
\li tracelogs with more data are split into several 32bit basic tracelog values with following IDs = 0.
\li So a basic tracelog consists always of 4 byte.
\li for tracelog transmission each basic tracelog gets a header of additional 4 bytes.
\li The header contains a startbyte, client and server address and a crc8 exOr checksum.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include "traceLog.h"

#ifdef TRACELOG_OFF

/*! This function should be called inside the transmit done device interrupt.
Also it should be called cyclically to trigger transmission start.
*/
void traceLogTxHandler( void ){}

#else // #ifdef TRACELOG_OFF

#include <stdint.h>
#include "traceLog.h"

//! tracelog fifo instance
ALIGN4 uint32_t tlFifo[ TL_FIFO_SIZE>>2 ] ALIGN4_END;

uint32_t rdIndexTlFifo = 0; //!< tracelog fifo read index
uint32_t wrIndexTlFifo = 0; //!< tracelog fifo write index

//! partial prefilled tracelog message buffer 
ALIGN4 static traceLogMsg_t traceLogMsg ALIGN4_END = {
    { TL_START_BYTE,  TL_LOCAL_ADDR,  TL_DISPL_ADDR, 0 }, // crc8
    { 0, 0 } // 16bit ID, 16bit data
};

static uint8_t const * const limit = (uint8_t*)(&traceLogMsg + 1); //!< tracelog message buffer limit
static uint8_t       *       pRead = (uint8_t*)(&traceLogMsg + 1); //!< tracelog message buffer read pointer

/*! get next tracelog byte for transmission from tracelog message buffer, no depth check here
\retval data byte
*/
TL_INLINE uint8_t traceLogMsgNextByte( void ){
    return *pRead++;
}

/*! return count of bytes ready for transmission
\retval count
*/
static size_t traceLogMsgDepth( void ){
    size_t count = limit - pRead;
    if( count ){
        return count;
    } else {
        if( tlFifoDepth() ){
            tlFifoPop( (uint32_t*)(&(traceLogMsg.ld)) );
            pRead = (uint8_t*)&traceLogMsg;
            traceLogMsg.hd.crc8  = TL_START_BYTE ^ TL_LOCAL_ADDR ^ TL_DISPL_ADDR
                                 ^ traceLogMsg.ld.load[0]
                                 ^ traceLogMsg.ld.load[1]
                                 ^ traceLogMsg.ld.load[2]
                                 ^ traceLogMsg.ld.load[3];
            return 8;
        }
    }
    return 0;
}

/*! This function should be called inside the transmit done device interrupt.
Also it should be called cyclically to trigger transmission start.
*/
void traceLogTxHandler( void ){
    if( traceLogTxDataRegisterEmpty() ){ 
        if( traceLogMsgDepth() ){
            uint8_t x = traceLogMsgNextByte();
            traceLogTransmitData8( x );
            traceLogEableTxEmptyInterrupt(); 
        }else{
            traceLogDisableTxEmptyInterrupt();
        }
    }
}

#endif // #else // #ifdef TRACELOG_OFF
