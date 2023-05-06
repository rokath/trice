/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#if 0



//! TriceDirectEncode expects at buf trice date with netto length len.
//! \param enc is the destination.
//! \param buf is the source.
//! \param len is the source len.
//! \retval is the encoded len with 0-delimiter byte.
size_t TriceDirectEncode( uint8_t* enc, uint8_t const* buf, size_t len ){
    size_t encLen;
    #ifdef XTEA_ENCRYPT_KEY
    len = (len + 7) & ~7; // todo: only multiple of 8 encryptable (needs some adaptions, later!)
    XTEAEncrypt( (uint32_t*)(enc + TRICE_DATA_OFFSET), len>>2 );
    #endif
    #if TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_TCOBS
    encLen = (size_t)TCOBSEncode(enc, buf, len);
    enc[encLen++] = 0; // Add zero as package delimiter.
    #elif TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_COBS
    encLen = (size_t)COBSEncode(enc, buf, len);
    enc[encLen++] = 0; // Add zero as package delimiter.
    #elif TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE
    memmove( enc, buf, len );
    encLen = len;
    #else
    #error unknown TRICE_DIRECT_OUT_FRAMING
    #endif
    return encLen;
}


#ifdef TRICE_CGO

//! TriceWriteDeviceCgo 
void TriceWriteDeviceCgo( uint8_t *buf, size_t len ){
    void triceNonBlockingWriteCgoBuffer( uint8_t const * buf, unsigned len );
    triceNonBlockingWriteCgoBuffer( buf, len ); // defined in cgoTrice.c
}

#endif


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
        #if TRICE_TRANSFER_MODE == TRICE_SAFE_SINGLE_MODE
        encLen += TriceEncode( enc+encLen, triceStart, triceLen );
        #endif
    }
    #if TRICE_RTT0 == 1
    TriceWriteDeviceRtt0( enc, encLen );
    #endif
}

//! triceIDAndLen expects at buf a trice message and returns the ID for routing.
//! \param pBuf is where the trice message starts.
//! \param triceID is filled positive ID value on success or negative on error.
//! \retval is the netto trice length (without padding bytes), 0 on error.
static size_t triceIDAndLen( uint32_t* pBuf, int* triceID ){
    uint16_t* pTID = (uint16_t*)*pBuf; // get TID address
    uint16_t TID = TRICE_TTOHS( *pTID ); // type and id
    *triceID = 0x3FFF & TID;
    int triceType = TID >> 14;
    unsigned offset = 0;
    size_t len;
    uint8_t* pStart = (uint8_t*)pBuf;
    switch( triceType ){
        case TRICE_TYPE_S0: // S0 = no stamp
            len = 4 + triceDataLen(pStart + 2); // tyId
            break;
        case TRICE_TYPE_S2: // S2 = 16-bit stamp
            *pStart += 2; // see Id(n) macro definition
            offset = 2;
            len = 6 + triceDataLen(pStart + 4); // tyId ts16
            break;
        case TRICE_TYPE_S4: // S4 = 32-bit stamp
            len = 8 + triceDataLen(pStart + 6); // tyId ts32
            break;
        default:
            //lint -fallthrugh
        case TRICE_TYPE_X0:
            triceErrorCount++;
            *triceID = -__LINE__; // extended trices not supported (yet)
            return 0;
    }
    size_t triceSize = (len + offset + 3) & ~3;
    // S16 case example:            triceSize  len   t-0-3   t-o
    // 80id 80id 1616 00cc                8     6      3      6
    // 80id 80id 1616 01cc dd            12     7      7     10
    // 80id 80id 1616 02cc dd dd         12     8      7     10
    // 80id 80id 1616 03cc dd dd dd      12     9      7     10
    // 80id 80id 1616 04cc dd dd dd dd   12    10      7     10
    if( !( triceSize - (offset + 3) <= len && len <= triceSize - offset )){ // todo: simplify expression, if possible
        // corrupt data
        triceErrorCount++;
        *triceID = -__LINE__; //
        return 0;
    }    
    return triceSize;
}

#ifdef TRICE_SEGGER_RTT_DIAGNOSTICS
unsigned RTT0_writeSpaceMin = BUFFER_SIZE_UP; //! RTT0_writeSpaceMin is usable for diagnostics.
unsigned RTT0_bytesInBufferMax = 0;        //! RTT0_bytesInBufferMax is usable for diagnostics.
#endif

void TriceWriteDeviceRtt0( uint8_t *buf, size_t len ){
    
    // diagnostics
    #ifdef TRICE_SEGGER_RTT_DIAGNOSTICS
    unsigned writeSpace = SEGGER_RTT_GetAvailWriteSpace (0);
    unsigned bytesInBuffer = SEGGER_RTT_GetBytesInBuffer(0);
    RTT0_writeSpaceMin    = RTT0_writeSpaceMin    < writeSpace    ? RTT0_writeSpaceMin    : writeSpace;
    RTT0_bytesInBufferMax = RTT0_bytesInBufferMax > bytesInBuffer ? RTT0_bytesInBufferMax : bytesInBuffer;
    #endif
    
    // action
    SEGGER_RTT_WriteNoLock(0, buf, len );
}
//#endif // #if TRICE_RTT0

#if TRICE_RTT0 == 1
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
        #if TRICE_TRANSFER_MODE == TRICE_SAFE_SINGLE_MODE
        encLen += TriceEncode( enc+encLen, triceStart, triceLen );
        #endif
    }
    TriceWriteDeviceRtt0( enc, encLen ); //lint !e534
}
#endif // #if TRICE_RTT0 == 1

#endif
