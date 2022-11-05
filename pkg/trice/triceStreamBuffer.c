
#include "trice.h"

#if TRICE_MODE == TRICE_STREAM_BUFFER

#if TRICE_STREAM_BUFFER_SIZE < 2*TRICE_SINGLE_MAX_SIZE
#error
#endif

static uint32_t triceStreamBufferHeap[TRICE_STREAM_BUFFER_SIZE>>2] = {0}; //!< triceStreamBufferHeap is a kind of heap for trice messages.
       uint32_t* TriceBufferWritePosition = triceStreamBufferHeap; //!< TriceBufferWritePosition is the active write position.
static uint32_t* triceBufferWriteLimit  =  &triceStreamBufferHeap[TRICE_STREAM_BUFFER_SIZE>>2]; //!< triceBufferWriteLimit is the triceBuffer written limit. 

//! TRICE_FIFO_ELEMENTS is the amount of positions the triceFifo has.
//! Must be a power of 2.
#define TRICE_FIFO_ELEMENTS 512

// TRICE_FIFO_MAX_DEPTH is the max possible count of values the triceFifo can hold.
#define TRICE_FIFO_MAX_DEPTH (TRICE_FIFO_ELEMENTS-1)

//! triceFifo holds up to TRICE_FIFO_MAX_DEPTH uint32_t* values.
static uint32_t* triceFifo[TRICE_FIFO_ELEMENTS];

//! triceFifoWriteIndex indexes the next write position.
static unsigned triceFifoWriteIndex = 0;

//! triceFifoReadIndex indexes the next read position.
static unsigned triceFifoReadIndex = 0;

//! triceFifoDepth returns the count of elements stored inside triceFifo.
static int triceFifoDepth( void ){
    return triceFifoWriteIndex - triceFifoReadIndex;
}

//! triceFifoPush stores v in triceFifo.
//! There is no depth check.
void TriceFifoPush( uint32_t* v ){
    triceFifo[triceFifoWriteIndex++] = v;
    triceFifoWriteIndex &= TRICE_FIFO_MAX_DEPTH;
}

//! triceFifoPop reads next value from triceFifo.
//! There is no depth check.
static uint32_t* triceFifoPop( void ){
    uint32_t* v = triceFifo[triceFifoReadIndex++];
    triceFifoReadIndex &= TRICE_FIFO_MAX_DEPTH;
    return v;
}

//! TriceStreamBufferSpace returns the space until buffer end.
uint32_t* TriceNextStreamBuffer( void ){
    if( TriceBufferWritePosition > triceBufferWriteLimit ){
        for(;;); // buffer overflow
    }
    if( triceBufferWriteLimit - TriceBufferWritePosition > TRICE_SINGLE_MAX_SIZE ){
        return TriceBufferWritePosition; // enough space at buffer end
    }else{
        return triceStreamBufferHeap; // buffer wrap
    }
}

//! triceLen returns the trice byte count ready for transfer including padding bytes.
//! The trice data start at tb + TRICE_DATA_OFFSET.
//! The returned depth is without the TRICE_DATA_OFFSET offset.
static size_t triceDepth( uint32_t const* tBuf ){
    uint32_t* limit = triceFifoPop(); 
    size_t depth = (limit - tBuf)<<2;
    return depth - TRICE_DATA_OFFSET;
}

//! TriceTransfer, if possible, initiates a write.
//! It is the responsibility of the app to call this function.
void TriceTransfer( void ){
    if( 0 == TriceOutDepth() ){ // transmission done, so a new is possible
        if( triceFifoDepth() >= 2 ){ // data in triceFifo
            uint32_t* tBuf = triceFifoPop(); 
            size_t tLen = triceDepth(tBuf); // tlen is always a multiple of 4
            //if( tLen ){ tLen is always > 0 here
                TriceOut( tBuf, tLen );
            //}
        } // else: nothing to transfer
    } // else: transmission not done yet
}

void TriceLogBufferInfo( void ){
    TRICE32( Id(10909), "att: Trice stream buffer size:%4u ", TRICE_STREAM_BUFFER_SIZE );
}

#if defined( TRICE_UART ) // direct out to UART
void TriceWrite( uint8_t const * buf, unsigned len ){
    triceNonBlockingWrite( buf, len ); 
}
#endif

#endif // #if TRICE_MODE == TRICE_STREAM_BUFFER
