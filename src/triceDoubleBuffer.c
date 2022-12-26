//! \file triceDoubleBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_MODE == TRICE_DOUBLE_BUFFER

#if TRICE_HALF_BUFFER_SIZE < TRICE_SINGLE_MAX_SIZE + TRICE_DATA_OFFSET
#error
#endif

static uint32_t triceBuffer[2][TRICE_HALF_BUFFER_SIZE>>2] = {0}; //!< triceBuffer is a double buffer for better write speed.
static int triceSwap = 0; //!< triceSwap is the index of the active write buffer. !triceSwap is the active read buffer index.
    uint32_t* TriceBufferWritePosition = &triceBuffer[0][TRICE_DATA_OFFSET>>2]; //!< TriceBufferWritePosition is the active write position.
static uint32_t* triceBufferWriteLimit = &triceBuffer[1][TRICE_DATA_OFFSET>>2]; //!< triceBufferWriteLimit is the triceBuffer written limit. 


//! triceBufferSwap swaps the trice double buffer and returns the read buffer address.
static uint32_t* triceBufferSwap( void ){
    TRICE_ENTER_CRITICAL_SECTION
    triceBufferWriteLimit = TriceBufferWritePosition; // keep end position
    triceSwap = !triceSwap; // exchange the 2 buffers
    TriceBufferWritePosition = &triceBuffer[triceSwap][TRICE_DATA_OFFSET>>2]; // set write position for next TRICE
    TRICE_LEAVE_CRITICAL_SECTION
    return &triceBuffer[!triceSwap][0]; //lint !e514
}

//! triceDepth returns the total trice byte count ready for transfer.
//! The trice data start at tb + TRICE_DATA_OFFSET.
//! The returned depth is without the TRICE_DATA_OFFSET offset.
static size_t triceDepth( uint32_t const* tb ){
    size_t depth = (triceBufferWriteLimit - tb)<<2; //lint !e701 // 32-bit write width 
    return depth - TRICE_DATA_OFFSET;
}

//! TriceTransfer, if possible, swaps the double buffer and initiates a write.
//! It is the resposibility of the app to call this function once every 10-100 milliseconds.
void TriceTransfer( void ){
    if( 0 == TriceOutDepth() ){ // transmission done, so a swap is possible
        uint32_t* tb = triceBufferSwap(); 
        size_t tLen = triceDepth(tb); // tlen is always a multiple of 4
        if( tLen ){
            TriceOut( tb, tLen );
        }
    } // else: transmission not done yet
}

//! TriceDepth returns current trice buffer depth.
size_t TriceDepth( void ){
    size_t currentDepth = (size_t)(4*(TriceBufferWritePosition - &triceBuffer[triceSwap][0]));
    return currentDepth;
}

//! TriceDepthMax returns the max trice buffer depth until now.
size_t TriceDepthMax( void ){
    size_t currentDepth = TriceDepth(); 
    return currentDepth > triceDepthMax ? currentDepth : triceDepthMax;
}

void TriceLogBufferInfo( void ){
    TRICE32( id( 2572), "att: Trice 2x half buffer size:%4u ", TRICE_HALF_BUFFER_SIZE );
}

#endif // #if TRICE_MODE == TRICE_DOUBLE_BUFFER
