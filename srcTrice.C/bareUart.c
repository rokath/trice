/*! \file bareUART.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include "bare.h"

//! trice fifo instance, here are the trices buffered.
ALIGN4 uint32_t triceFifo[ TRICE_FIFO_BYTE_SIZE>>2 ] ALIGN4_END;

uint32_t triceFifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
uint32_t triceFifoReadIndex = 0; //!< trice fifo read index
uint32_t triceFifoMaxDepthTrices = 0; //!< diagnostics
uint8_t  triceBytesBuffer[8]; //!< bytes transmit buffer
int const triceBytesBufferIndexLimit = 8; // sizeof(triceBytesBuffer[8]);
int triceBytesBufferIndex = triceBytesBufferIndexLimit;

//! tricePop gets one trice from trice fifo.
//! \return trice id with 2 byte data in one uint32_t.
TRICE_INLINE uint32_t tricePop(){
    uint32_t v = triceFifo[triceFifoReadIndex++];
    triceFifoReadIndex &= TRICE_FIFO_MASK;
    return v;
}

//! TODO: endianess with compiler macros.
TRICE_INLINE void triceLoadInNetworkOrder( uint8_t* p, uint32_t t ){
    // ID arrives in upper 16 bit in machine endianess
    // DA arrives in lower 16 bit in machine endianess (8 bit values are in trice ordering)

    // transmit in big endian
    uint8_t idHi = p[0] = (uint8_t)(t>>24); // IDHi big endian
    uint8_t idLo = p[1] = (uint8_t)(t>>16); // IDLo big endian
    
    // transmit in big endian   
    uint8_t daLo = p[2] = (uint8_t)(t>> 8); // DaLo big endian -> should be changed to >>24 (needs many changes in trice.h but does not influence trice go code)
    uint8_t daHi = p[3] = (uint8_t)(t>> 0); // DaHi big endian -> should be changed to >>16 (needs many changes in trice.h but does not influence trice go code)
}

TRICE_INLINE void triceTransfer( uint32_t t0, uint32_t t1 ){
    triceLoadInNetworkOrder( &triceBytesBuffer[0], t0 );
    triceLoadInNetworkOrder( &triceBytesBuffer[4], t1 );
}

//! triceFifoDepth determines trices count inside trice fifo.
//! \return count of buffered trices
TRICE_INLINE unsigned triceFifoDepth( void ){
    unsigned triceDepth = (triceFifoWriteIndex - triceFifoReadIndex) & TRICE_FIFO_MASK;
    triceFifoMaxDepthTrices = triceDepth < triceFifoMaxDepthTrices ? triceFifoMaxDepthTrices : triceDepth; // diagnostics
    return triceDepth;
}

//! triceServeOut must be called cyclically to proceed ongoing write out.
//! It schould be called at least every ms.
//! A possibe place is main loop.
void triceServeOut( void ){
    // 89 ab cd ef <- on serial port
    // ih il dh dl
    uint32_t const syncTrice = 0x89abcdef; // endianess!! ID is in low part
    static int syncLevel = TRICE_BARE_SYNC_LEVEL; // start with a sync trice
    if( triceBytesBufferIndexLimit == triceBytesBufferIndex ){ // bytes buffer empty and tx finished
        // next trice
        int n = triceFifoDepth();
        if( syncLevel < TRICE_BARE_SYNC_LEVEL ){ // no need for a sync trice
            if( 0 == n ) { // no trices to transmit
                syncLevel++; 
                return;
            } else if( 1 == n ){ // one trice to transmit
                triceTransfer( tricePop(), syncTrice );
                syncLevel = 0;
            } else { // at least 2 trices to transmit
                uint32_t t0 = tricePop();
                uint32_t t1 = tricePop();
                triceTransfer( t0, t1 );
                syncLevel+=2;
            }
        } else { // need for a sync trice
            if( 1 <= n ){ // at least one trice, so transmit it and one sync trice
                triceTransfer( tricePop(), syncTrice );
            } else { // nothing to transmit so transmit 2 sync trices
                triceTransfer( syncTrice, syncTrice );
            }
            syncLevel = 0;
        }
        triceBytesBufferIndex = 0;
        // next byte
        triceTransmitData8( triceBytesBuffer[triceBytesBufferIndex++] );
        triceEnableTxEmptyInterrupt();
    }
}

//! triceServeTransmit() must be called cyclically to proceed ongoing write out.
//! A good place: sysTick ISR and UART ISR (both together).
//! TODO: endianess with compiler macros.
void triceServeTransmit( void ){
    if( !triceTxDataRegisterEmpty() ){
        for(;;); // unexpected case
    }
    if( triceBytesBufferIndexLimit == triceBytesBufferIndex ){
        for(;;); // unexpected case
    }
    // next byte
    triceTransmitData8( triceBytesBuffer[triceBytesBufferIndex++] );
    if( sizeof(triceBytesBuffer) == triceBytesBufferIndex  ){ // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}
