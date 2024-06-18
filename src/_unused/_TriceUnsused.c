#if 0

//! TriceSingleEncode expects a single trice at addr and returns the wordCount amount of this trice which includes 1-3 padding bytes.
//! The returned value tells how many words where used by the transmitted trice and is usable for the memory management. See RingBuffer for example.
//! The returned value is typically (TRICE_DATA_OFFSET/4) plus 1 (4 bytes) to 3 (9-12 bytes) but could go up theoretically up to 8182, when really long buffers are transferred with trice.
//! Return values <= 0 signal an error.
//! \param addr is where the trice data start. Before addr are TRICE_DATA_OFFSET bytes usable for in-buffer encoding.
//! \param pBuf is filled with the address where the encoded trice data start.
//! \param pLen is filled with the byte count of the encoded data including a terminating 0. 
static int TriceSingleEncode(uint32_t* addr, int* pTriceID, uint8_t** pEnc, size_t* pLen){
    int len = triceIDAndLen( addr, pTriceID );
    uint8_t* buf = (uint8_t*)addr;
    *pEnc = buf - TRICE_DATA_OFFSET; // encoded data starting address
    *pLen = TriceEncode( *pEnc, buf, len );
    return (len+3)>>2;
}

//! singleTriceDirectOut encodes a single trice and writes it to the output.
//! \param tb is start of uint32_t* trice buffer. The space TRICE_DATA_OFFSET at
//! the tb start is for in-buffer encoding of the trice data.
//! \param tLen is length of trice data. tlen is always a multiple of 4 because
//! of 32-bit alignment and padding bytes.
// TODO: rewrite this working function for more efficiency
/*
- The fastest would be to direct call SEGGER Write without (COBS) encoding.
  - For that the exact length is needed (without padding bytes)
  - In case of SEGGER buffer wrap then the re-sync may have issues.
- Simple COBS encoding is also fast and gives reliability.
- TCOBS is not recommended here.
- Encryption is possible but could cause timing issues especially when trices occur inside interrupts.
- The user should be able to configure that.
- Also:
  - Use SEGGER_RTT_LOCK() for TRICE_ENTER_CRITICAL_SECTION 
  - Use SEGGER_RTT_UNLOCK() for TRICE_LEAVE_CRITICAL_SECTION 
  - Write special void SEGGER_RTT0_Write32_No_Lock( const uint32_t* pBuffer, unsigned NumWords) in a file SEGGER_RTText.c
  - start with void _Write32NoCheck(SEGGER_RTT_BUFFER_UP* pRing, const uint32_t* pData, unsigned NumWords)
    - At init ensure Dst is aligned, otherwise write padding zeroes what is ok for COBS
*/
static void singleTriceDirectOut( uint32_t* tb, size_t tLen ){
    uint8_t* enc = (uint8_t*)tb; // encoded data starting address
    size_t encLen = 0;
    uint8_t* buf = enc + TRICE_DATA_OFFSET; // start of 32-bit aligned trices
    size_t len = tLen; // (byte count)
    int triceID;
    // diagnostics
    tLen += TRICE_DATA_OFFSET; 
    triceDepthMax = tLen < triceDepthMax ? triceDepthMax : tLen;

    // do it
    while(len){
        uint8_t* triceStart;
        size_t triceLen;
        triceID = TriceNext( &buf, &len, &triceStart, &triceLen );
        if( triceID <= 0 ){ // on data error
            break;   // ignore following data
        }
        #if TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE
        encLen += TriceEncode( enc+encLen, triceStart, triceLen );
        #endif
    }
    #if TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1
    TriceWriteDeviceRtt0( enc, encLen );
    #endif
}

#if TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1
//! TriceOutRtt0 encodes trices and writes them in one step to the output.
//! \param tb is start of uint32_t* trice buffer. The space TRICE_DATA_OFFSET at
//! the tb start is for in-buffer encoding of the trice data.
//! \param tLen is length of trice data. tlen is always a multiple of 4 because
//! of 32-bit alignment and padding bytes.
void TriceOutRtt0( uint32_t* tb, size_t tLen ){
    uint8_t* enc = (uint8_t*)tb; // encoded data starting address
    size_t encLen = 0;
    uint8_t* buf = enc + TRICE_DATA_OFFSET; // start of 32-bit aligned trices
    size_t len = tLen; // (byte count)
    int triceID;
    // diagnostics
    tLen += TRICE_DATA_OFFSET; 
    triceDepthMax = tLen < triceDepthMax ? triceDepthMax : tLen;
    // do it
    while(len){
        uint8_t* triceStart;
        size_t triceLen;
        triceID = TriceNext( &buf, &len, &triceStart, &triceLen );
        if( triceID <= 0 ){ // on data error
            break;   // ignore following data
        }
        #if TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE
        encLen += TriceEncode( enc+encLen, triceStart, triceLen );
        #endif
    }
    TriceWriteDeviceRtt0( enc, encLen ); //lint !e534
}
#endif // #if TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1

#endif
