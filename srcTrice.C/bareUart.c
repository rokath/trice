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



//! TRICE_SYNC is an optional trice sync message for syncing, when bare transmission is used.
//! The value 35243 (0x89ab) is a reserved pattern used as ID with value DA 0xcdef.
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
#define TRICE_SYNC do{ TRICE16_1( Id(0x89ab), "%x\b\b\b\b", 0xcdef ); }while(0)





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
