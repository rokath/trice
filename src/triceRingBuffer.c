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


#ifdef XTEA_ENCRYPT_KEY

//! triceBufferWriteLimit is the first address behind TriceRingBuffer. 
//! With encryption it can happen that 4 bytes following triceRingBufferLimit are user as scratch pad.
//! See comment inside TriceSingleDeferredOut.
uint32_t* const triceRingBufferLimit = &TriceRingBuffer[TRICE_DEFERRED_BUFFER_SIZE>>2] - 1;

#else // #ifdef XTEA_ENCRYPT_KEY

//! triceBufferWriteLimit is the first address behind TriceRingBuffer. 
uint32_t* const triceRingBufferLimit = &TriceRingBuffer[TRICE_DEFERRED_BUFFER_SIZE>>2];

#endif // #else // #ifdef XTEA_ENCRYPT_KEY

//! singleTricesRingCount holds the readable trices count inside TriceRingBuffer.
int singleTricesRingCount = 0; 

//ARM5 #pragma push
//ARM5 #pragma diag_suppress=170 //warning:  #170-D: pointer points outside of underlying object
//! TriceRingBufferReadPosition points to a valid trice message when singleTricesRingCount > 0.
//! This is first the TRICE_DATA_OFFSET byte space followedy the trice data.
//! Initally this value is set to TriceRingBuffer minus TRICE_DATA_OFFSET byte space
//! to ga correct value for the very first call of triceNextRingBufferRead
uint32_t* TriceRingBufferReadPosition = TriceRingBuffer - (TRICE_DATA_OFFSET>>2); //lint !e428 Warning 428: negative subscript (-4) in operator 'ptr-int'
//ARM5 #pragma  pop

#if TRICE_DIAGNOSTICS == 1

//! singleTricesRingCountMax holds the max count of trices occurred inside the ring buffer.
static unsigned singleTricesRingCountMax = 0;

//! triceSingleMaxWordCount is a diagnostics value usable to optimize buffer size.
unsigned triceSingleMaxWordCount = 0;

//! triceRingBufferDepthMax holds the max occurred ring buffer depth.
static unsigned triceRingBufferDepthMax = 0;

//! TriceLogDiagnosticValues shows the max used half buffer space. 
void TriceLogDiagnosticValues( void ){
    #ifdef SEGGER_RTT
    TriceLogSeggerDiagnostics();
    #endif

    unsigned triceSingleDepthMax = TRICE_DATA_OFFSET + (triceSingleMaxWordCount<<2);
    if( triceSingleDepthMax <= TRICE_BUFFER_SIZE ){
        TRice16( iD( 6931), "diag:triceSingleDepthMax =%4u of %d, ", triceSingleDepthMax, TRICE_BUFFER_SIZE );
    }else{
        TRice16( iD( 3497), "err:triceSingleDepthMax =%4u of %d (overflow!), ", triceSingleDepthMax, TRICE_BUFFER_SIZE );
    }
    trice16( iD( 3284), "diag:singleTricesRingCountMax = %u, ", singleTricesRingCountMax );
    if( triceRingBufferDepthMax <= TRICE_DEFERRED_BUFFER_SIZE ){
        trice16( iD( 1608), "diag:triceRingBufferDepthMax =%4u of%5d\n", triceRingBufferDepthMax, TRICE_DEFERRED_BUFFER_SIZE );
    }else{
        trice16( iD( 1087), "err:triceRingBufferDepthMax =%4u of%5d (overflow!)\n", triceRingBufferDepthMax, TRICE_DEFERRED_BUFFER_SIZE );
    }

}

#endif

//! triceNextRingBufferRead returns a single trice data buffer address. The trice are data starting at byte offset TRICE_DATA_OFFSET.
//! Implicit assumed is singleTricesRingCount > 0.
//! \param lastWordCount is the uint32 count of the last read trice including padding bytes.
//! The value lastWordCount is needed to increment TriceRingBufferReadPosition accordingly.
//! \retval is the address of the next trice data buffer.
static uint32_t* triceNextRingBufferRead( int lastWordCount ){
    TriceRingBufferReadPosition += (TRICE_DATA_OFFSET>>2) + lastWordCount;
    if( (TriceRingBufferReadPosition + (TRICE_BUFFER_SIZE>>2)) > triceRingBufferLimit ){
        TriceRingBufferReadPosition = TriceRingBuffer;
    }
    #if TRICE_DIAGNOSTICS == 1
    int depth = (TriceBufferWritePosition - TriceRingBufferReadPosition)<<2;
    if( depth < 0 ){
        depth += TRICE_DEFERRED_BUFFER_SIZE;
    }
    triceRingBufferDepthMax = (depth > triceRingBufferDepthMax) ? depth : triceRingBufferDepthMax;
    #endif
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
    #if TRICE_DIAGNOSTICS == 1
    singleTricesRingCountMax = (singleTricesRingCount > singleTricesRingCountMax) ? singleTricesRingCount : singleTricesRingCountMax;
    #endif
    singleTricesRingCount--;
    static int lastWordCount = 0;
    uint32_t* addr = triceNextRingBufferRead( lastWordCount );
    lastWordCount = TriceSingleDeferredOut(addr);
}

//! TriceSingleDeferredOut expects a single trice at addr with byte offset TRICE_DATA_OFFSET and returns the wordCount of this trice which includes 1-3 padding bytes.
//! This function is specific to the ring buffer, because the wordCount value needs to be reconstructed.
//! \param addr points to TRICE_DATA_OFFSET bytes usable space followed by the begin of a single trice.
//! \retval The returned value tells how many words where used by the transmitted trice and is usable for the memory management. See RingBuffer for example.
//! The returned value is typically (TRICE_DATA_OFFSET/4) plus 1 (4 bytes) to 3 (9-12 bytes) but could go up to ((TRICE_DATA_OFFSET/4)+(TRICE_BUFFER_SIZE/4)).
//! Return values <= 0 signal an error.
static int TriceSingleDeferredOut(uint32_t* addr){
    uint32_t* pData = addr + (TRICE_DATA_OFFSET>>2);
    uint8_t* pEnc = (uint8_t*)addr;
    
    int wordCount;
    uint8_t* pStart;
    size_t Length; // This is the trice netto length (without padding bytes).
    int triceID = TriceIDAndBuffer( pData, &wordCount, &pStart, &Length );
    
    // Behind the trice brutto length (with padding bytes), 4 bytes can be used as scratch pad when XTEA is active. 
    // This i ok, when behind triceRingBufferLimit are at least 4 bytes unused space.
    // After TriceIDAndBuffer pStart has a 2 bytes offset, what is an alignmet issue for encryption.
    // That gets corrected inside TriceDeferredEncode.
    // todo: Put this correction into TriceIDAndBuffer to keep tcode cleaner.
    size_t encLen = TriceDeferredEncode( pEnc, pStart, Length);
    
    TriceNonBlockingDeferredWrite( triceID, pEnc, encLen );
    return wordCount;
}

#endif // #if TRICE_BUFFER == TRICE_RING_BUFFER
