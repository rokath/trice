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

//! TriceBufferWritePosition is used by TRICE_PUT macros.
uint32_t* TriceBufferLastWritePosition;

//! triceBufferWriteLimit is the triceBuffer written limit. 
static uint32_t* triceBufferWriteLimit = &triceBuffer[1][TRICE_DATA_OFFSET>>2];

#if TRICE_DIAGNOSTICS == 1

//! triceSingleMaxWordCount is a diagnostics value usable to optimize buffer size.
unsigned triceSingleMaxWordCount = 0;

//! triceHalfBufferDepthMax is a diagnostics value usable to optimize buffer size.
static unsigned triceHalfBufferDepthMax = 0; 

//! TriceLogDiagnosticValues shows the max used half buffer space. 
void TriceLogDiagnosticValues( void ){
    #ifdef SEGGER_RTT
    TriceLogSeggerDiagnostics();
    #endif
    
    unsigned triceSingleDepthMax = TRICE_DATA_OFFSET + (triceSingleMaxWordCount<<2);

    if( triceSingleDepthMax <= TRICE_BUFFER_SIZE ){
        TRice16( iD( 3751), "diag:triceSingleDepthMax =%4u of%4d, ", triceSingleDepthMax, TRICE_BUFFER_SIZE );
    }else{
        TRice16( iD( 5199), "err:triceSingleDepthMax =%4u of%4d (overflow!), ", triceSingleDepthMax, TRICE_BUFFER_SIZE );
    }

    if( triceHalfBufferDepthMax <= TRICE_DEFERRED_BUFFER_SIZE/2 ){
        TRice16( iD( 3925), "diag:triceHalfBufferDepthMax =%4u of%5d\n", triceHalfBufferDepthMax, TRICE_DEFERRED_BUFFER_SIZE/2 );
    }else{
        TRice16( iD( 1099), "err:triceHalfBufferDepthMax =%4u of%5d (overflow!)\n", triceHalfBufferDepthMax, TRICE_DEFERRED_BUFFER_SIZE/2 );
    }
}

#endif

//! triceBufferSwap swaps the trice double buffer and returns the read buffer address.
static uint32_t* triceBufferSwap( void ){
    TRICE_ENTER_CRITICAL_SECTION
    triceBufferWriteLimit = TriceBufferWritePosition; // keep end position
    triceSwap = !triceSwap; // exchange the 2 buffers
    TriceBufferWritePosition = &triceBuffer[triceSwap][TRICE_DATA_OFFSET>>2]; // set write position for next TRICE
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

//! TriceOut encodes trices and writes them in one step to the output.
//! This function is called only, when the slowest deferred output device has finished its last buffer.
//! That 
//! \param tb is start of uint32_t* trice buffer. The space TRICE_DATA_OFFSET at
//! the tb start is for in-buffer encoding of the trice data.
//! \param tLen is length of trice data. tlen is always a multiple of 4 because
//! of 32-bit alignment and padding bytes.
static void TriceOut( uint32_t* tb, size_t tLen ){
    uint8_t* enc = (uint8_t*)tb; // encoded data starting address
    size_t encLen = 0;
    uint8_t* buf = enc + TRICE_DATA_OFFSET; // start of 32-bit aligned trices
    size_t len = tLen; // (byte count)
    int triceID;
    #if TRICE_DIAGNOSTICS == 1
    tLen += TRICE_DATA_OFFSET; 
    triceHalfBufferDepthMax = tLen < triceHalfBufferDepthMax ? triceHalfBufferDepthMax : tLen;
    #endif
    // do it
    while(len){
        uint8_t* triceStart;
        size_t triceLen; // This is the trice netto length (without padding bytes).
        triceID = TriceNext( &buf, &len, &triceStart, &triceLen );
        if( triceID <= 0 ){ // on data error
            break;   // ignore following data
        }
        #if TRICE_TRANSFER_MODE == TRICE_SAFE_SINGLE_MODE
        // Behind the trice brutto length (with padding bytes), 4 bytes can be used as scratch pad when XTEA is active. 
        encLen += TriceDeferredEncode( enc+encLen, triceStart, triceLen );
        #endif
        #if  TRICE_TRANSFER_MODE == TRICE_PACK_MULTI_MODE
        memmove(enc + TRICE_DATA_OFFSET + encLen, triceStart, triceLen );
        encLen += triceLen;
        #endif
    }
    #if TRICE_TRANSFER_MODE == TRICE_PACK_MULTI_MODE
    // Behind the trice brutto length (with padding bytes), 4 bytes can be used as scratch pad when XTEA is active. 
    encLen = TriceDeferredEncode( enc, enc + TRICE_DATA_OFFSET, encLen);
    #endif

    // Reaching here means all trice data in the current double buffer are encoded
    // into a single continuous buffer having 0-delimiters between them or not but at the ent is a 0-delimiter.
    //
    // output
    TriceNonBlockingDeferredWrite( triceID, enc, encLen ); //lint !e771 Info 771: Symbol 'triceID' conceivably not initialized. Comment: tLen is always > 0.
}

#endif // #if TRICE_BUFFER == TRICE_DOUBLE_BUFFER
