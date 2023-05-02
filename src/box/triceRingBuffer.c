//! \file triceRingBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_BUFFER == TRICE_RING_BUFFER

static int TriceSingleDeferredOut(uint32_t* addr);

//! TriceRingBuffer is a kind of heap for trice messages.
uint32_t TriceRingBuffer[TRICE_DEFERRED_BUFFER_SIZE>>2] = {0};

//! TriceBufferWritePosition is used by the TRICE_PUT macros.
uint32_t* TriceBufferWritePosition = TriceRingBuffer; 

//! triceBufferWriteLimit is the first address behind TriceRingBuffer. 
uint32_t* const triceRingBufferLimit = &TriceRingBuffer[TRICE_DEFERRED_BUFFER_SIZE>>2]; 

//! singleTricesRingCount holds the readable trices count inside TriceRingBuffer.
int singleTricesRingCount = 0; 

//! TriceRingBufferReadPosition points to a valid trice message when singleTricesRingCount > 0.
uint32_t* TriceRingBufferReadPosition = TriceRingBuffer; 

//! TriceNextRingWriteBuffer returns a usable address with TRICR_BUFFER space.
//! \trtval is the next usable address in the ring buffer. 
//! If there is at least TRICE_DIRECT_BUFFER_SIZE TriceBufferWritePosition remains unchanged, otherwise it is set to the ring buffer start.
//! For performance there is no full check, so the user needs tor read out faster than it is possible to fill the ring buffer.
uint32_t* TriceNextRingWriteBuffer( void ){
    if( (triceRingBufferLimit - TriceBufferWritePosition < (TRICE_DIRECT_BUFFER_SIZE>>2)) ){
        TriceBufferWritePosition = TriceRingBuffer;
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
        TriceRingBufferReadPosition = TriceRingBuffer;
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

//! TriceSingleDeferredOut expects a single trice at addr with byte offset TRICE_DATA_OFFSET and returns the wordCount of this trice which includes 1-3 padding bytes.
//! \param addr points to TRICE_DATA_OFFSET bytes usble space followed by the begin of a single trice.
//! \retval The returned value tells how many words where used by the transmitted trice and is usable for the memory management. See RingBuffer for example.
//! The returned value is typically (TRICE_DATA_OFFSET/4) plus 1 (4 bytes) to 3 (9-12 bytes) but could go up to ((TRICE_DATA_OFFSET/4)+(TRICE_DIRECT_BUFFER_SIZE/4)).
//! Return values <= 0 signal an error.
static int TriceSingleDeferredOut(uint32_t* addr){
    uint32_t* pData = addr + (TRICE_DATA_OFFSET>>2);
    uint8_t* pEnc = (uint8_t*)addr;

    int wordCount;
    uint8_t* pStart;
    size_t Length;
    int triceID = TriceIDAndBuffer( pData, &wordCount, &pStart, &Length );
  
    size_t encLen = TriceEncode( pEnc, pStart, Length);
    TriceNonBlockingWrite( triceID, pEnc, encLen );
    return wordCount;
}

#endif // #if TRICE_BUFFER == TRICE_RING_BUFFER
