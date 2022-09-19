
#include "trice.h"

static uint32_t triceStreamBufferHeap[TRICE_STREAM_BUFFER_SIZE>>2] = {0}; //!< triceStreamBufferHeap is a kind of heap for trice messages.
       uint32_t* TriceBufferWritePosition = triceStreamBufferHeap; //!< TriceBufferWritePosition is the active write position.
static uint32_t* triceBufferWriteLimit  =  &triceStreamBufferHeap[TRICE_STREAM_BUFFER_SIZE>>2]; //!< triceBufferWriteLimit is the triceBuffer written limit. 


//! TriceStreamBufferSpace returns the space until buffer end.
uint32_t* TriceNextStreamBuffer( void ){
    if( TriceBufferWritePosition > triceBufferWriteLimit ){
        for(;;); // buffer overflow
    }
    if( triceBufferWriteLimit - TriceBufferWritePosition > TRICE_SINGLE_MAX_SIZE ){
        return TriceBufferWritePosition;
    }else{
        return triceStreamBufferHeap;
    }
}

void TriceAddressPush( uint32_t* ta ){
}

uint32_t* TriceAddressPop( void ){
    return triceStreamBufferHeap;
}

//! triceDepth returns the total trice byte count ready for transfer.
//! The trice data start at tb + TRICE_DATA_OFFSET.
//! The returned depth is without the TRICE_DATA_OFFSET offset.
static size_t triceDepth( uint32_t const* tb ){
    size_t depth = (triceBufferWriteLimit - tb)<<2; //lint !e701 // 32-bit write width 
    return depth - TRICE_DATA_OFFSET;
}


//! TriceTransfer, if possible, initiates a write.
//! It is the resposibility of the app to call this function.
void TriceTransfer( void ){
    if( 0 == TriceOutDepth() ){ // transmission done, so a new is possible
        uint32_t* tb = TriceAddressPop(); 
        size_t tLen = triceDepth(tb); // tlen is always a multiple of 4
        if( tLen ){
            TriceOut( tb, tLen );
        }
    } // else: transmission not done yet
}
