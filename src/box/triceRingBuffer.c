//! \file triceRingBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_DIRECT_BUFFER == TRICE_RING_BUFFER

//! triceRingBuffer is a kind of heap for trice messages.
static uint32_t triceRingBuffer[TRICE_DEFERRED_BUFFER_SIZE>>2] = {0};

//! TriceBufferWritePosition is used by the TRICE_PUT macros.
uint32_t* TriceBufferWritePosition; 

//! triceBufferWriteLimit is the first address behind triceRingBuffer. 
static uint32_t* const triceRingBufferLimit  =  &triceRingBuffer[TRICE_DEFERRED_BUFFER_SIZE>>2]; 

//! singleTricesRingCount holds the readable trices count inside triceRingBuffer.
int singleTricesRingCount = 0; 

//! TriceRingBufferReadPosition points to a valid trice message when singleTricesRingCount > 0.
uint32_t* TriceRingBufferReadPosition = triceRingBuffer; 

//! TriceNextRingWriteBuffer returns a usable address with TRICE_DIRECT_BUFFER space.
//! TriceBufferWritePosition is the next usable address in the ring buffer. 
//! If there is at least TRICE_DIRECT_BUFFER_SIZE the very same address is returned, otherwise the ring buffer start.
//! For performance there is no full check, so the user needs tor read out faster than it is possible to fill the ring buffer.
uint32_t* TriceNextRingWriteBuffer( uint32_t* TriceBufferWritePosition ){
    if( (triceRingBufferLimit - TriceBufferWritePosition < (TRICE_DIRECT_BUFFER_SIZE>>2)) ){
				TriceBufferWritePosition = triceRingBuffer;
    }
    return TriceBufferWritePosition; 
}

//! triceNextRingBufferRead returns a single trice data buffer address. The trice are data starting at byte offset TRICE_DATA_OFFSET.
//! Implicit assumed is singleTricesRingCount > 0.
//! \param lastWordCount is the uint32 count of the last read trice including padding bytes.
//! The value lastWordCount is needed to increment TriceRingBufferReadPosition accordingly.
//! \retval is the address of the next trice data buffer.
static uint32_t* triceNextRingBufferRead( int lastWordCount ){
    TriceRingBufferReadPosition += lastWordCount;
    if( (triceRingBufferLimit - TriceRingBufferReadPosition < (TRICE_DIRECT_BUFFER_SIZE>>2)) ){
				TriceRingBufferReadPosition = triceRingBuffer;
    }
    return TriceRingBufferReadPosition; 
}

//! TriceTransfer needs to be called cyclically to read out the Ring Buffer.
void TriceTransfer( void ){
    if( singleTricesRingCount == 0 ){ // no data
        return;
    }
		if( TriceOutDepth() ){ // last transmission not finished
			  return;
		}
    singleTricesRingCount--;
		static int lastWordCount = 0;
		uint32_t* addr = triceNextRingBufferRead( lastWordCount );	
		
    lastWordCount = TriceSingleDeferredOut(addr+(TRICE_DATA_OFFSET>>2));
}

#endif // #if TRICE_DIRECT_BUFFER == TRICE_RING_BUFFER
