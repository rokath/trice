//! \file triceStaticBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_MODE == TRICE_STATIC_BUFFER

#if TRICE_STREAM_BUFFER_SIZE < 2*(TRICE_SINGLE_MAX_SIZE+TRICE_DATA_OFFSET)
#error
#endif

static uint32_t triceStreamBufferHeap[TRICE_STREAM_BUFFER_SIZE>>2] = {0}; //!< triceStreamBufferHeap is a kind of heap for trice messages.
       uint32_t* TriceBufferWritePosition = triceStreamBufferHeap; //!< TriceBufferWritePosition is the active write position.
static uint32_t* triceBufferWriteLimit  =  &triceStreamBufferHeap[TRICE_STREAM_BUFFER_SIZE>>2]; //!< triceBufferWriteLimit is the triceBuffer written limit. 

//! TriceNextStreamBuffer returns a usable address with at least TRICE_SINGLE_MAX_SIZE space.
uint32_t* TriceNextStreamBuffer( void ){
    if( TriceBufferWritePosition > triceBufferWriteLimit ){
        for(;;); // trice stream buffer overflow
    }
    if( triceBufferWriteLimit - TriceBufferWritePosition > (TRICE_SINGLE_MAX_SIZE>>2) ){
        return TriceBufferWritePosition; // enough space at buffer end
    }else{
        return triceStreamBufferHeap; // buffer wrap
    }
}

//! triceLen returns the trice byte count ready for transfer including padding bytes.
//! The trice data start at tb + TRICE_DATA_OFFSET.
//! The returned depth is without the TRICE_DATA_OFFSET offset.
static size_t triceDepth( uint32_t const* tBuf ){
    return 0; // todo
}

size_t triceStreamBufferDepthMax = 0; //!< triceStreamBufferDepth is used for diagnostics.

static size_t streamBufferDepth( uint32_t* tBuf ){
    int depth;
    if( tBuf < TriceBufferWritePosition ){
        depth = TriceBufferWritePosition - tBuf;
    }else{
        int top  = TriceBufferWritePosition - triceStreamBufferHeap;
        int bottom = triceBufferWriteLimit - tBuf;
        depth = top + bottom;
    }
    return (size_t)(depth<<2);
}

void TriceLogBufferInfo( void ){
    TRICE32( id( 6429), "att: Trice static buffer size:%5u ", 0 );
}

#endif // #if TRICE_MODE == TRICE_STATIC_BUFFER
