//! \file triceRingBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

//lint -e715 Info 715: Symbol 'pFmt' (line 854, file ..\..\..\src\trice.h) not referenced
//lint -e528 Warning 528: Symbol 'Trice0(unsigned short, const char *)'  not referenced
//lint -e438 Warning 438: Last value assigned to variable 'singleTricesRingCount' not used.

#if TRICE_BUFFER == TRICE_RING_BUFFER

#ifndef TRICE_RING_BUFFER_DATA_OFFSET
#define TRICE_RING_BUFFER_DATA_OFFSET 16
#endif

static int TriceSingleDeferredOut(uint32_t* addr);

#ifdef TRICE_RINGBUFFER_OVERFLOW_WATCH

#define TRICE_RINGBUFFER_LOWER_MARGIN 8 //!< 32-bit units just for debugging > 0
#define TRICE_RINGBUFFER_UPPER_MARGIN 8 //!< 32-bit units just for debugging > 0
#define TRICE_RINGBUFFER_MARGIN_FILL_VALUE 0xfee4deb

#else

#define TRICE_RINGBUFFER_LOWER_MARGIN 0 //!< 32-bit units just for debugging > 0
#define TRICE_RINGBUFFER_UPPER_MARGIN 0 //!< 32-bit units just for debugging > 0
#define TRICE_RINGBUFFER_FILL_VALUE   0

#endif

//! TriceRingBuffer is a kind of heap for trice messages. It needs to be initialized with 0.
uint32_t TriceRingBufferMargined[TRICE_RINGBUFFER_LOWER_MARGIN + (TRICE_DEFERRED_BUFFER_SIZE>>2) + TRICE_RINGBUFFER_UPPER_MARGIN] = {0};

uint32_t* const TriceRingBufferStart = TriceRingBufferMargined + TRICE_RINGBUFFER_LOWER_MARGIN;

//! triceBufferWriteLimit is the first address behind TriceRingBuffer. 
//! With encryption it can happen that 4 bytes following triceRingBufferLimit are used as scratch pad.
//! We use the value of TRICE_XTEA_DEFERRED_ENCRYPT (0 or 1) here to respect that
//! See also comment inside TriceSingleDeferredOut.
uint32_t* const triceRingBufferLimit = TriceRingBufferStart + (TRICE_DEFERRED_BUFFER_SIZE>>2) - TRICE_XTEA_DEFERRED_ENCRYPT;

//! SingleTricesRingCount holds the readable trices count inside TriceRingBuffer.
unsigned SingleTricesRingCount = 0;

//! TriceBufferWritePosition is used by the TRICE_PUT macros.
uint32_t* TriceBufferWritePosition = TriceRingBufferStart; 

//ARM5 #pragma push
//ARM5 #pragma diag_suppress=170 //warning:  #170-D: pointer points outside of underlying object
//! TriceRingBufferReadPosition points to a valid trice message when singleTricesRingCount > 0.
//! This is first the TRICE_RING_BUFFER_DATA_OFFSET byte space followed by the trice data.
//! Initally this value is set to TriceRingBufferStart minus TRICE_RING_BUFFER_DATA_OFFSET byte space
//! to get a correct value for the very first call of triceNextRingBufferRead
uint32_t* TriceRingBufferReadPosition = TriceRingBufferStart - (TRICE_RING_BUFFER_DATA_OFFSET>>2); //lint !e428 Warning 428: negative subscript (-4) in operator 'ptr-int'
//ARM5 #pragma  pop

#if TRICE_DIAGNOSTICS == 1

//! SingleTricesRingCountMax holds the max count of trices occurred inside the ring buffer.
//! This value is only informal, because the length of the trice messages is not known.
// unsigned SingleTricesRingCountMax = 0;

//! TriceRingBufferDepthMax holds the max occurred ring buffer depth.
unsigned TriceRingBufferDepthMax = 0;

#endif // #if TRICE_DIAGNOSTICS == 1

#ifdef TRICE_PROTECT

//! TriceEnoughSpace checks, if enough bytes available for the next trice.
//! \retval 0, when not enough space
//! \retval 1, when enough space
int TriceEnoughSpace( void ){
    // depth32 is the used buffer space in 32-bit words. After reset TriceBufferWritePosition and TriceRingBufferReadPosition are equal and depth32 is 0.
    // After some trice data writing, TriceBufferWritePosition is > TriceRingBufferReadPosition and depth32 has a positive value.
    // When trice data read out takes place, the TriceRingBufferReadPosition is incremented. That makes the depth32 value smaller again and
    // gives write space. During the trice write operation TriceBufferWritePosition gets incremented by max TRICE_SINGLE_MAX_SIZE bytes.
    // If afterwards the Trice BufferWritePosition is less TRICE_SINGLE_MAX_SIZE away from the buffer end, it is reset to the buffer start.
    // That means, there are max TRICE_SINGLE_MAX_SIZE-4 bytes wasted in the worst case. So the needed safety space is 2*TRICE_SINGLE_MAX_SIZE-4.
    // The TriceRingBufferReadPosition jumps in the same way as the TriceBufferWritePosition. It wraps according the same rules.
    const int neededSafetySpace32 = (TRICE_SINGLE_MAX_SIZE>>1)-1; // (TRICE_SINGLE_MAX_SIZE>>2) + (TRICE_SINGLE_MAX_SIZE>>2) - 1
    int depth32 = TriceBufferWritePosition - TriceRingBufferReadPosition; //lint !e845 Info 845: The left argument to operator '<<' is certain to be 0 
    if( depth32 < 0 ){ // After a TriceBufferWritePosition reset the difference is negative and needs correction to get the correct value.
        depth32 += (TRICE_DEFERRED_BUFFER_SIZE>>2);
    }
    // This fn is called before an intended trice data write ande therefore additional at least neededSafetySpace32 32-bit words need to fit in the buffer.
    if( depth32 + neededSafetySpace32 <= (TRICE_DEFERRED_BUFFER_SIZE>>2) ){
        return 1;
    }else{
        #if TRICE_DIAGNOSTICS == 1
            TriceDeferredOverflowCount++;
        #endif
        return 0;
    } 
}

#endif // #ifdef TRICE_PROTECT

#if TRICE_DIAGNOSTICS == 1
uint16_t TriceRingBufferDepth = 0;
#endif

//! triceNextRingBufferRead returns a single trice data buffer address. The trice data are starting at byte offset TRICE_RING_BUFFER_DATA_OFFSET from this address.
//! Implicit assumed is, that the pre-condition "SingleTricesRingCount > 0" is fullfilled.
//! \param lastWordCount is the u32 count of the last read trice including padding bytes.
//! The value lastWordCount is needed to increment TriceRingBufferReadPosition accordingly.
//! \retval is the address of the next trice data buffer.
static uint32_t* triceNextRingBufferRead( int lastWordCount ){
    TriceRingBufferReadPosition += (TRICE_RING_BUFFER_DATA_OFFSET>>2) + lastWordCount;
    if( (TriceRingBufferReadPosition + (TRICE_BUFFER_SIZE>>2)) > triceRingBufferLimit ){
        TriceRingBufferReadPosition = TriceRingBufferStart;
    }

    #if TRICE_DIAGNOSTICS == 1
        int depth = (TriceBufferWritePosition - TriceRingBufferReadPosition)<<2; //lint !e845 Info 845: The left argument to operator '<<' is certain to be 0 
        if( depth < 0 ){
            depth += TRICE_DEFERRED_BUFFER_SIZE;
        }
        TriceRingBufferDepth = (uint16_t)depth;
        TriceRingBufferDepthMax = (depth > TriceRingBufferDepthMax) ? depth : TriceRingBufferDepthMax; //lint !e574 !e737 Warning 574: Signed-unsigned mix with relational, Info 737: Loss of sign in promotion from int to unsigned int
    #endif // #if TRICE_DIAGNOSTICS == 1
    
    return TriceRingBufferReadPosition; //lint !e674 Warning 674: Returning address of auto through variable 'TriceRingBufferReadPosition'
}

//! TriceTransfer needs to be called cyclically to read out the Ring Buffer.
void TriceTransfer( void ){
    if( SingleTricesRingCount == 0 ){ // no data
        return;
    }
    if( TriceOutDepth() ){ // last transmission not finished
        return;
    }
    #if TRICE_DIAGNOSTICS == 1
    // SingleTricesRingCountMax = (SingleTricesRingCount > SingleTricesRingCountMax) ? SingleTricesRingCount : SingleTricesRingCountMax;
    #endif
    SingleTricesRingCount--;
    static int lastWordCount = 0;
    uint32_t* addr = triceNextRingBufferRead( lastWordCount );
    lastWordCount = TriceSingleDeferredOut(addr);
}

//! TriceIDAndBuffer evaluates a trice message and returns the ID for routing.
//! \param pData is where the trice message starts.
//! \param pWordCount is filled with the word count the trice data occupy from pData.
//! \param ppStart is filled with the trice netto data start. That is maybe a 2 bytes offset from pData.
//! \param pLength is filled with the netto trice length (without padding bytes), 0 on error.
//! \retval is the triceID, a positive value on success or error information.
static int TriceIDAndBuffer( const uint32_t * const pData, int* pWordCount, uint8_t** ppStart, size_t* pLength ){
    uint16_t TID = TRICE_TTOHS( *(uint16_t*)pData ); // type and id
    int triceID = 0x3FFF & TID;
    int triceType = TID >> 14; // 2 bits
    unsigned offset;
    size_t len;
    uint8_t* pStart = (uint8_t*)pData;
    switch( triceType ){
        case TRICE_TYPE_S0: // S0 = no stamp
            offset = 0;
            len = 4 + triceDataLen(pStart + 2); // tyId
            break;
        case TRICE_TYPE_S2: // S2 = 16-bit stamp
            len = 6 + triceDataLen(pStart + 6); // tyId ts16
            offset = 2;
            #if TRICE_XTEA_DEFERRED_ENCRYPT
                // move trice to start at a uint32_t alingment border
                memmove(pStart, pStart+2, len ); // https://stackoverflow.com/questions/1201319/what-is-the-difference-between-memmove-and-memcpy
            #else // #if TRICE_XTEA_DEFERRED_ENCRYPT
                // Like for UART transfer no uint32_t alignment is needed.
                pStart += 2; // see Id(n) macro definition        
            #endif // #else // #if TRICE_XTEA_DEFERRED_ENCRYPT
            break;
        case TRICE_TYPE_S4: // S4 = 32-bit stamp
            offset = 0;
            len = 8 + triceDataLen(pStart + 6); // tyId ts32
            break;
        default: // impossible case (triceType has only 2 bits)
            // fallthrugh
        case TRICE_TYPE_X0:
            TriceErrorCount++;
            *ppStart = pStart;
            *pLength = 0;
            return -__LINE__; // extended trices not supported (yet)
    }
    // S16 case example:            triceSize  len   t-0-3   t-o
    // 80id 80id 1616 00cc                8     6      3      6
    // 80id 80id 1616 01cc dd            12     7      7     10
    // 80id 80id 1616 02cc dd dd         12     8      7     10
    // 80id 80id 1616 03cc dd dd dd      12     9      7     10
    // 80id 80id 1616 04cc dd dd dd dd   12    10      7     10
    *pWordCount = (len + offset + 3) >> 2;
    *ppStart = pStart;
    *pLength = len;
    return triceID;
}

//! TriceSingleDeferredOut expects a single trice at addr with byte offset TRICE_RING_BUFFER_DATA_OFFSET and returns the wordCount of this trice which includes 1-3 padding bytes.
//! This function is specific to the ring buffer, because the wordCount value needs to be reconstructed.
//! \param addr points to TRICE_RING_BUFFER_DATA_OFFSET bytes usable space, followed by the begin of a single trice.
//! \retval The returned value tells how many words where used by the transmitted trice and is usable for the memory management. See RingBuffer for example.
//! The returned value is typically (TRICE_RING_BUFFER_DATA_OFFSET/4) plus 1 (4 bytes) to 3 (9-12 bytes) but could go up to ((TRICE_RING_BUFFER_DATA_OFFSET/4)+(TRICE_BUFFER_SIZE/4)).
//! Return values <= 0 signal an error.
static int TriceSingleDeferredOut(uint32_t* addr){
    uint32_t* pData = addr + (TRICE_RING_BUFFER_DATA_OFFSET>>2);
    uint8_t* pEnc = (uint8_t*)addr;
    
    int wordCount;
    uint8_t* pStart;
    size_t Length; // This is the trice netto length (without padding bytes).
    int triceID = TriceIDAndBuffer( pData, &wordCount, &pStart, &Length );
    
    // Behind the trice brutto length (with padding bytes), 4 bytes can be used as scratch pad when XTEA is active. 
    // This is ok, when behind triceRingBufferLimit are at least 4 bytes unused space.
    // After TriceIDAndBuffer pStart has a 2 bytes offset, what is an alignmet issue for encryption.
    // That gets corrected inside TriceDeferredEncode.
    // todo: Put this correction into TriceIDAndBuffer to keep tcode cleaner.
    size_t encLen = TriceEncode( TRICE_XTEA_DEFERRED_ENCRYPT, TRICE_DEFERRED_OUT_FRAMING, pEnc, pStart, Length);

    TriceNonBlockingDeferredWrite8( triceID, pEnc, encLen );
    return wordCount;
}

#ifdef TRICE_RINGBUFFER_OVERFLOW_WATCH

void TriceInitRingBufferMargins( void ){
    for( int i = 0; i < TRICE_RINGBUFFER_LOWER_MARGIN; i++ ){
        TriceRingBufferMargined[i] = TRICE_RINGBUFFER_MARGIN_FILL_VALUE;
    }
    for( int i = 0; i < TRICE_RINGBUFFER_UPPER_MARGIN; i++ ){
         *(triceRingBufferLimit + i) = TRICE_RINGBUFFER_MARGIN_FILL_VALUE;
    }    
}

void WatchRingBufferMargins( void ){
    for( int i = 0; i < TRICE_RINGBUFFER_LOWER_MARGIN; i++ ){
        if( TriceRingBufferMargined[i] != TRICE_RINGBUFFER_MARGIN_FILL_VALUE ){
            for(;;);
        }
    }

    for( int i = 0; i < TRICE_RINGBUFFER_UPPER_MARGIN; i++ ){
        if( *(triceRingBufferLimit + i) != TRICE_RINGBUFFER_MARGIN_FILL_VALUE ){
            for(;;);
        }
    }
}

#endif // #ifdef TRICE_RINGBUFFER_OVERFLOW_WATCH

#endif // #if TRICE_BUFFER == TRICE_RING_BUFFER
