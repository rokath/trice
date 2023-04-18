//! \file triceStaticBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_MODE == TRICE_STATIC_BUFFER

#if TRICE_DEFERRED_BUFFER_SIZE < 2*(TRICE_SINGLE_MAX_SIZE+TRICE_DATA_OFFSET)
#error
#endif

static uint32_t triceDeferredBufferHeap[TRICE_DEFERRED_BUFFER_SIZE>>2] = {0}; //!< triceDeferredBufferHeap is a kind of heap for trice messages.
       uint32_t* TriceDeferredBufferWritePosition = triceDeferredBufferHeap; //!< TriceBufferWritePosition is the active write position.
       uint32_t* TriceDeferredBufferReadPosition = triceDeferredBufferHeap; //!< TriceBufferReadPosition is the active read position.
static uint32_t* const triceDeferredBufferWriteLimit  =  &triceDeferredBufferHeap[TRICE_DEFERRED_BUFFER_SIZE>>2]; //!< triceBufferWriteLimit is the triceBuffer written limit. 
int singleTricesDeferredCount = 0; // The readable trices count inside triceDeferredBufferHeap.

//! TriceNextDeferredBuffer returns a usable address with at least wordCount+1 space.
uint32_t* TriceNextDeferredBuffer( uint32_t wordCount ){
    //  if( would overwrite data ){
    //      for(;;); // trice deferred buffer overflow
    //  }
    if( !(triceDeferredBufferWriteLimit - TriceDeferredBufferWritePosition > wordCount) ){
        if( TriceDeferredBufferWritePosition < triceDeferredBufferWriteLimit ){ // at least 1 word free
            *TriceDeferredBufferWritePosition = 0; // signal end
        }
        TriceDeferredBufferWritePosition = triceDeferredBufferHeap; // not enough space at buffer end for 1 + wordCount
    }
    return TriceDeferredBufferWritePosition; 
}


void TriceServeDeferredBuffer( void ){
    if( singleTricesDeferredCount == 0 ){
        return;
    }
    singleTricesDeferredCount--;
    uint32_t wordCount;
    if( TriceDeferredBufferReadPosition < triceDeferredBufferWriteLimit ){ // at least 1 word readable
        wordCount = *TriceDeferredBufferReadPosition++;
    }
    if( wordCount == 0 ){ // end marker
        TriceDeferredBufferReadPosition = triceDeferredBufferHeap;
        wordCount = *TriceDeferredBufferReadPosition++;
    }
    while( wordCount-->0 ){
        // *dest++ = *TriceDeferredBufferReadPosition++;
    }
    // ...
}




//! triceLen returns the trice byte count ready for transfer including padding bytes.
//! The trice data start at tb + TRICE_DATA_OFFSET.
//! The returned depth is without the TRICE_DATA_OFFSET offset.
static size_t triceDepth( uint32_t const* tBuf ){
    return 0; // todo
}

size_t triceDeferredBufferDepthMax = 0; //!< triceDeferredBufferDepth is used for diagnostics.

static size_t deferredBufferDepth( uint32_t* tBuf ){
    int depth;
    if( tBuf < TriceDeferredBufferWritePosition ){
        depth = TriceDeferredBufferWritePosition - tBuf;
    }else{
        int top  = TriceDeferredBufferWritePosition - triceDeferredBufferHeap;
        int bottom = triceDeferredBufferWriteLimit - tBuf;
        depth = top + bottom;
    }
    return (size_t)(depth<<2);
}

void TriceLogBufferInfo( void ){
    TRICE32( id( 6429), "att: Trice static buffer size:%5u ", TRICE_DATA_OFFSET + TRICE_SINGLE_MAX_SIZE );
}

#endif // #if TRICE_MODE == TRICE_STATIC_BUFFER
