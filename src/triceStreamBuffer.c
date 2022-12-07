//! \file triceStreamBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_MODE == TRICE_STREAM_BUFFER

#if TRICE_STREAM_BUFFER_SIZE < 2*(TRICE_SINGLE_MAX_SIZE+TRICE_DATA_OFFSET)
#error
#endif

static uint32_t triceStreamBufferHeap[TRICE_STREAM_BUFFER_SIZE>>2] = {0}; //!< triceStreamBufferHeap is a kind of heap for trice messages.
       uint32_t* TriceBufferWritePosition = triceStreamBufferHeap; //!< TriceBufferWritePosition is the active write position.
static uint32_t* triceBufferWriteLimit  =  &triceStreamBufferHeap[TRICE_STREAM_BUFFER_SIZE>>2]; //!< triceBufferWriteLimit is the triceBuffer written limit. 

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
    return TRICE_FIFO_MAX_DEPTH & (triceFifoWriteIndex - triceFifoReadIndex);
}

//! triceFifoPush stores v in triceFifo.
//! There is no depth check.
void TriceFifoPush( void* v ){
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
        for(;;); // trice stream buffer overflow
    }
    if( (int)triceBufferWriteLimit - (int)TriceBufferWritePosition > TRICE_SINGLE_MAX_SIZE ){
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

size_t triceFifoDepthMax = 0; //!< triceFifoDepthMax is used for diagnostics.
size_t triceStreamBufferDepthMax = 0; //!< triceStreamBufferDepth is used for diagnostics.

static size_t streamBufferDepth( uint32_t* tBuf ){
    int depth;
    if( tBuf < TriceBufferWritePosition ){
        depth = (int)TriceBufferWritePosition - (int)tBuf;
    }else{
        int top  = (int)TriceBufferWritePosition - (int)triceStreamBufferHeap;
        int bottom = (int)triceBufferWriteLimit - (int)tBuf;
        depth = top + bottom;
    }
    return (size_t)depth;
}

//! TriceTransfer, if possible, initiates a write.
//! It is the responsibility of the app to call this function.
void TriceTransfer( void ){
    if( 0 == TriceOutDepth() ){ // transmission done, so a new is possible
        size_t depth = triceFifoDepth();
        triceFifoDepthMax = depth < triceFifoDepthMax ? triceFifoDepthMax : depth;
        if( depth >= 2 ){ // data in triceFifo
            uint32_t* tBuf = triceFifoPop(); 
            size_t tLen = triceDepth(tBuf); // calls internally triceFifoPop(), tlen is always a multiple of 4
            size_t triceStreamBufferDepth = streamBufferDepth(tBuf);
            TriceOut( tBuf, tLen );
            triceStreamBufferDepthMax = triceStreamBufferDepth < triceStreamBufferDepthMax ? triceStreamBufferDepthMax : triceStreamBufferDepth; // diagnostics
        } // else: nothing to transfer
    } // else: transmission not done yet
}

void TriceLogBufferInfo( void ){
    TRICE32( Id( 1135), "att: Trice stream buffer size:%5u ", TRICE_STREAM_BUFFER_SIZE );
}

#endif // #if TRICE_MODE == TRICE_STREAM_BUFFER
