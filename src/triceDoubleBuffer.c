//! \file triceDoubleBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_BUFFER == TRICE_DOUBLE_BUFFER

static void TriceOut( uint32_t* tb, size_t tLen );

//! triceBuffer is a double buffer for better write speed.
static uint32_t triceBuffer[2][TRICE_DEFERRED_BUFFER_SIZE/8] = {0}; 

//! triceSwap is the index of the active write buffer. !triceSwap is the active read buffer index.
static int triceSwap = 0;

//! TriceBufferWritePosition is the active write position.
uint32_t* TriceBufferWritePosition = &triceBuffer[0][TRICE_DATA_OFFSET>>2];

//! TriceBufferLastWritePosition is used by TRICE_PUT macros.
uint32_t* TriceBufferLastWritePosition;

//! triceBufferWriteLimit is the triceBuffer written limit. 
static uint32_t* triceBufferWriteLimit = &triceBuffer[1][TRICE_DATA_OFFSET>>2];

#if TRICE_DIAGNOSTICS == 1

uint16_t TriceHalfBufferDepth = 0;

//! TriceHalfBufferDepthMax is a diagnostics value usable to optimize buffer size.
unsigned TriceHalfBufferDepthMax = 0; 

#endif

//! triceBufferSwap swaps the trice double buffer and returns the read buffer address.
static uint32_t* triceBufferSwap( void ){
    TRICE_ENTER_CRITICAL_SECTION
    triceBufferWriteLimit = TriceBufferWritePosition; // keep end position
    triceSwap = !triceSwap; // exchange the 2 buffers
    // Set write position for next TRICE.
    // The TRICE_DATA_OFFSET value is used to have some recoding space during the transfer operation.
    TriceBufferWritePosition = &triceBuffer[triceSwap][TRICE_DATA_OFFSET>>2];
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

#ifdef TRICE_PROTECTED

//! TriceEnoughSpace checks, if at least TRICE_SINGLE_MAX_SIZE bytes available for the next trice.
//! \retval 0, when not enough space
//! \retval 1, when enough space
int TriceEnoughSpace( void ){
    // currentLimit32 points to the first 32-bit address outside the current write buffer.
    // TRICE_DEFERRED_BUFFER_SIZE is the total buffer size, so TRICE_DEFERRED_BUFFER_SIZE/2 is the half buffer size.
    // TRICE_DEFERRED_BUFFER_SIZE>>3 is the count of 32-bit value positions in the half buffer.
    uint32_t* currentLimit32 = &triceBuffer[triceSwap][TRICE_DEFERRED_BUFFER_SIZE>>3];
    // space32 is the writable 32-bit value count in the current write buffer.
    int space32 = currentLimit32 - TriceBufferWritePosition; 
    // there need to be at least TRICE_SINGLE_MAX_SIZE bytes space in the current write buffer.
    if( space32 >= (TRICE_SINGLE_MAX_SIZE>>2) ){
        return 1;
    }else{
        #if TRICE_DIAGNOSTICS == 1
            TriceOverflowCount++;
        #endif
        return 0;
    } 
}

#endif // #ifdef TRICE_PROTECTED

//! TriceTransfer, if possible, swaps the double buffer and initiates a write.
//! It is the resposibility of the app to call this function once every 10-100 milliseconds.
void TriceTransfer( void ){
    if( 0 == TriceOutDepth() ){ // transmission done for slowest output channel, so a swap is possible
        uint32_t* tb = triceBufferSwap(); 
        size_t tLen = triceDepth(tb); // tlen is always a multiple of 4
        if( tLen ){
            TriceOut( tb, tLen );
        }
    } // else: transmission not done yet
}

//! TriceNext expects at *buf 32-bit aligned trice messages and returns the next one in pStart and pLen.
//! \todo: use this function only when MULTI
//! \param buf points to 32-bit aligned trice messages and is filled with the advanced buf.
//! \param pSize contains the total size of all following trices including padding bytes to 32-bit alignments and gets the by next trice reduced value.
//! \param pStart points to the netto start of the next trice.
//! \param pLen contains the nettlo length of the first trice.
//! \retval is the trice ID on success or negative on error.
//! The trices inside the double buffer start 32-bit aligned with maybe 1-3 padding bytes inbetween.
//! \li |    ... |    trice   | trice | ... |  trice |
//! \li before:  ^- buf
//! \li before:  <-------------- pSize -------------->
//! \li after:                 ^- buf
//! \li after:                 <-------- pSize ------>
//! \li after:     ^- pStart    (maybe 2 bytes > before buf)
//! \li after:     <- pLen ->   (maybe 1-3 bytes shorter)
static int TriceNext( uint8_t** buf, size_t* pSize, uint8_t** pStart, size_t* pLen ){
    uint16_t* pTID = (uint16_t*)*buf; //lint !e826, get TID address
    unsigned TID = TRICE_TTOHS( *pTID ); // type and id
    int triceID = 0x3FFF & TID;
    int triceType = TID >> 14;
    unsigned offset;
    size_t size = *pSize;
    size_t triceSize;
    size_t len;
    *pStart = *buf;
    switch( triceType ){
        case TRICE_TYPE_S0: // S0 = no stamp
            offset = 0;
            len = 4 + triceDataLen(*pStart + 2); // tyId
            break;
        case TRICE_TYPE_S2: // S2 = 16-bit stamp
            *pStart += 2; // see Id(n) macro definition
            offset = 2;
            len = 6 + triceDataLen(*pStart + 4); // tyId ts16
            break;
        case TRICE_TYPE_S4: // S4 = 32-bit stamp
            offset = 0;
            len = 8 + triceDataLen(*pStart + 6); // tyId ts32
            break;
        default: // impossible
            // fallthrugh
        case TRICE_TYPE_X0:
            TriceErrorCount++;
            *pStart = 0;
            *pLen = 0;
            return -__LINE__; // extended trices not supported (yet)
    }
    triceSize = (len + offset + 3) & ~3;
    // S16 case example:            triceSize  len   t-0-3   t-o
    // 80id 80id 1616 00cc                8     6      3      6
    // 80id 80id 1616 01cc dd            12     7      7     10
    // 80id 80id 1616 02cc dd dd         12     8      7     10
    // 80id 80id 1616 03cc dd dd dd      12     9      7     10
    // 80id 80id 1616 04cc dd dd dd dd   12    10      7     10
    if( !( triceSize - (offset + 3) <= len && len <= triceSize - offset )){ // corrupt data
        TriceErrorCount++;
        return -__LINE__;
    }    
    size -= triceSize;
    *buf += triceSize;
    *pSize = size;
    *pLen = len;
    return triceID;
}

//! TriceOut encodes trices and writes them in one step to the output.
//! This function is called only, when the slowest deferred output device has finished its last buffer.
//! At the half buffer start tb are TRICE_DATA_OFFSET bytes space followed by a number of trice messages which all contain
//! 0-3 padding bytes and therefore have a length of a multiple of 4. There is no additional space between these trice messages.
//! When XTEA enabled, only (TRICE_TRANSFER_MODE == TRICE_PACK_MULTI_MODE) is allowed, because the 4 bytes behind a trice messages
//! are changed, when the trice length is not a multiple of 8, but only of 4. (XTEA can encrypt only multiple of 8 lenth packages.)
//! \param tb is start of uint32_t* trice buffer. The space TRICE_DATA_OFFSET at the tb start is for in-buffer encoding of the trice data.
//! \param tLen is total length of several trice data. It is always a multiple of 4 because of 32-bit alignment and padding bytes.
static void TriceOut( uint32_t* tb, size_t tLen ){
    uint8_t* enc = (uint8_t*)tb; // encoded data starting address
    size_t encLen = 0;
    uint8_t* buf = enc + TRICE_DATA_OFFSET; // start of 32-bit aligned trices
    //size_t len = tLen; // (byte count)
    int triceID = 0; // This assignment is only needed to silence compiler complains about being uninitialized.
    #if TRICE_DIAGNOSTICS == 1
        TriceHalfBufferDepth = tLen + TRICE_DATA_OFFSET;
        TriceHalfBufferDepthMax = TriceHalfBufferDepth < TriceHalfBufferDepthMax ? TriceHalfBufferDepthMax : TriceHalfBufferDepth;
    #endif
    // do it
    while(tLen){
        uint8_t* triceStart;
        size_t triceLen; // This is the trice netto length (without padding bytes).
        triceID = TriceNext( &buf, &tLen, &triceStart, &triceLen );
        if( triceID <= 0 ){ // on data error
            break;   // ignore following data
        }
        #if TRICE_TRANSFER_MODE == TRICE_SAFE_SINGLE_MODE
            #ifdef XTEA_ENCRYPT_KEY
                // Behind the trice brutto length (with padding bytes), 4 bytes could be used as scratch pad when XTEA is active.
                // Therefore, when XTEA is used, the single trice must be moved first by 4 bytes in lower address direction if its length is not a multiple of 4.
                #error not implemented (use "#define TRICE_TRANSFER_MODE TRICE_PACK_MULTI_MODE" or ring buffer )
            #endif
        encLen += TriceDeferredEncode( enc+encLen, triceStart, triceLen );
        #endif
        #if  TRICE_TRANSFER_MODE == TRICE_PACK_MULTI_MODE
        // This action removes all padding bytes of the trices, compacting their sequence this way
        memmove(enc + TRICE_DATA_OFFSET + encLen, triceStart, triceLen );
        encLen += triceLen;
        #endif
    }
    #if TRICE_TRANSFER_MODE == TRICE_PACK_MULTI_MODE
    // At this point the compacted trice messages start TRICE_DATA_OFFSET bytes after tb (now enc) and the encLen is their total netto length.
    // Behind this up to 7 bytes can be used as scratch pad when XTEA is active. That is ok, because the half buffer should not get totally filled.
    encLen = TriceDeferredEncode( enc, enc + TRICE_DATA_OFFSET, encLen);
    #endif

    // Reaching here means all trice data in the current half buffer are encoded
    // into a single continuous buffer having 0-delimiters between them or not but at the ent is a 0-delimiter.
    //
    // output
    TriceNonBlockingDeferredWrite( triceID, enc, encLen ); //lint !e771 Info 771: Symbol 'triceID' conceivably not initialized. Comment: tLen is always > 0.
}

#endif // #if TRICE_BUFFER == TRICE_DOUBLE_BUFFER
