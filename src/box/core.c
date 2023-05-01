//! \file trice.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_TCOBS
#include "tcobs.h"
#endif
#if TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_COBS
#include "cobs.h"
#endif
#ifdef XTEA_ENCRYPT_KEY
#include "xtea.h"
#endif

// function prototypes:

static size_t triceDataLen( uint8_t const* p );
static int triceIDAndBuffer( uint32_t const * const pAddr, int* pWordCount, uint8_t** ppStart, size_t* pLength );
static size_t triceNonBlockingWriteUartA( void const * buf, size_t nByte );
static size_t triceNonBlockingWriteUartB( void const * buf, size_t nByte );
static void SEGGER_Write_RTT0_NoCheck32( const uint32_t* pData, unsigned NumW );

// local definines:

#define TRICE_TYPE_X0 0 //!< TRICE_TYPE_X0 ist a unspecified trice (reserved)
#define TRICE_TYPE_S0 1 //!< TRICE_TYPE_S0 ist a trice without stamp.
#define TRICE_TYPE_S2 2 //!< TRICE_TYPE_S2 ist a trice with 16-bit stamp.
#define TRICE_TYPE_S4 3 //!< TRICE_TYPE_S4 ist a trice with 32-bit stamp.

// local variables:

// triceCommands is to make 32-bit alignment sure
static uint32_t triceCommands[(TRICE_COMMAND_SIZE_MAX+3)>>2]; // with terminating 0

//! triceCommandLength contains the count of valid bytes inside triceCommand including the terminating 0.
static int triceCommandLength = 0;

// global variables:

//! triceCommand is the command receive buffer.
char* const triceCommandBuffer = (char*)&triceCommands; // with terminating 0

//! triceCommandFlag ist set, when a command was received completely.
int triceCommandFlag = 0; // updated

#if TRICE_CYCLE_COUNTER == 1
uint8_t  TriceCycle = 0xc0; //!< TriceCycle is increased and transmitted with each trice message, if enabled.
#endif

//! triceErrorCount is incremented, when data inside the internal trice buffer are corrupted.
//! That could happen, when the buffer wrapped before data are sent.
static unsigned triceErrorCount = 0;

//! triceDataLen returns encoded len.
//! \param p points to nc
//! To avoid alignment issues, the optional payload (...) needs to start at a 32-bit boundary.
//! The provided buffer starts also at a 32-bit boundary.
//! To ensure, the first 16-bit value is ssiiiiiiI we do the following:
//! \li       v__________________ v___________ v__ (32-bit alignment positions)
//! \li *da = 11iiiiiiI TT        TT        NC ... | ID(n): After writing 11iiiiiiI write the 32-bit TTTT value in 2 16-bit write operations.
//! \li *da = 10iiiiiiI 10iiiiiiI TT        NC ... | Id(n): Write 10iiiiiiI as doubled value in one 32-bit operation into the trice buffer. The first 16-bit will be removed just before sending to the out channel. 
//! \li *da =                     01iiiiiiI NC ... | id(n): Just write 01iiiiiiI as 16-bit operation.
//! \li *da = ss0......extended trices are not used yet
//! \li This way, after writing the 16-bit NC value the payload starts always at a 32-bit boundary.
static size_t triceDataLen( uint8_t const* p ){
    uint16_t nc = TRICE_TTOHS(*(uint16_t*)p); //lint !e826
    size_t n = nc>>8;
    if( n < 128 ){
        return n;
    }
    return nc & 0x7fff;
}

//! triceIDAndBuffer evaluates a trice message and returns the ID for routing.
//! \param pAddr is where the trice message starts.
//! \param pWordCount is filled with the word count the trice data occupy from pAddr.
//! \param ppStart is filled with the trice netto data start. That is maybe a 2 bytes offset from pAddr.
//! \param pLength is filled with the netto trice length (without padding bytes), 0 on error.
//! \retval is the triceID, a positive value on success or error information.
static int triceIDAndBuffer( uint32_t const * const pAddr, int* pWordCount, uint8_t** ppStart, size_t* pLength ){
    uint16_t* pTID = (uint16_t*)*pAddr; // get TID address
    uint16_t TID = TRICE_TTOHS( *pTID ); // type and id
    int triceID = 0x3FFF & TID;
    int triceType = TID >> 14;
    unsigned offset;
    size_t len;
	  uint8_t* pStart = (uint8_t*)pAddr;
    switch( triceType ){
        case TRICE_TYPE_S0: // S0 = no stamp
						offset = 0;
            len = 4 + triceDataLen(pStart + 2); // tyId
            break;
        case TRICE_TYPE_S2: // S2 = 16-bit stamp
            *pStart += 2; // see Id(n) macro definition
            offset = 2;
            len = 6 + triceDataLen(pStart + 4); // tyId ts16
            break;
        case TRICE_TYPE_S4: // S4 = 32-bit stamp
						offset = 0;
            len = 8 + triceDataLen(pStart + 6); // tyId ts32
            break;
        default:
					  //lint -fallthrugh
        case TRICE_TYPE_X0:
					  triceErrorCount++;
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

//todo: use this function only when MULTI
//! TriceNext expects at *buf 32-bit aligned trice messages and returns the next one in pStart and pLen.
//! *buf is filled with the advanced buf and *pSize gets the reduced value.
//! \retval is the trice ID on success or negative on error.
int TriceNext( uint8_t** buf, size_t* pSize, uint8_t** pStart, size_t* pLen ){
    uint16_t* pTID = (uint16_t*)*buf; //lint !e826, get TID address
    int TID = TRICE_TTOHS( *pTID ); // type and id
    int triceID = 0x3FFF & TID;
    int triceType = TID >> 14;
    unsigned offset = 0;
    size_t size = *pSize;
    size_t triceSize;
    size_t len;
    *pStart = *buf;
    switch( triceType ){
        default:
        case TRICE_TYPE_S0: // S0 = no stamp
            len = 4 + triceDataLen(*pStart + 2); // tyId
            break;
        case TRICE_TYPE_S2: // S2 = 16-bit stamp
            *pStart += 2; // see Id(n) macro definition
            offset = 2;
            len = 6 + triceDataLen(*pStart + 4); // tyId ts16
            break;
        case TRICE_TYPE_S4: // S4 = 32-bit stamp
            len = 8 + triceDataLen(*pStart + 6); // tyId ts32
            break;
        case TRICE_TYPE_X0:
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
        triceErrorCount++;
        return -__LINE__;
    }    
    size -= triceSize;
    *buf += triceSize;
    *pSize = size;
    *pLen = len;
    return triceID;
}

//! TriceEncode expects at buf trice date with netto length len.
//! \param enc is the destination.
//! \param buf is the source.
//! \param len is the source len.
//! \retval is the encoded len with 0-delimiter byte.
size_t TriceEncode( uint8_t* enc, uint8_t const* buf, size_t len ){
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

// TriceNonBlockingWrite routes trice data to output channels.
void TriceNonBlockingWrite( int triceID, uint8_t* pBuf, size_t len ){
    
    #if defined( TRICE_UARTA )
        #if defined(TRICE_UARTA_MIN_ID) && defined(TRICE_UARTA_MAX_ID)
        if( (TRICE_UARTA_MIN_ID < triceID) && (triceID < TRICE_UARTA_MAX_ID) )
        #endif
        { triceNonBlockingWriteUartA( pBuf, len ); }
    #endif
    #if defined( TRICE_UARTB )
        #if defined(TRICE_UARTB_MIN_ID) && defined(TRICE_UARTB_MAX_ID)
        if( (TRICE_UARTB_MIN_ID < triceID) && (triceID < TRICE_UARTB_MAX_ID) )
        #endif
        { triceNonBlockingWriteUartB( pBuf, len ); }
    #endif
    //  #if defined(TRICE_RTT0) // && !defined(TRICE_DOUBLE_BUFFER) && !defined(TRICE_STREAM_BUFFER) // only when RTT0 is alone
    //      #if defined(TRICE_RTT0_MIN_ID) && defined(TRICE_RTT0_MAX_ID)
    //      if( (TRICE_RTT0_MIN_ID < triceID) && (triceID < TRICE_RTT0_MAX_ID) )
    //      #endif
    //      TriceWriteDeviceRtt0( enc, encLen ); //lint !e534
    //  #endif
    #ifdef TRICE_CGO
        TriceWriteDeviceCgo( pBuf, len );
    #endif
    //  #ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO
    //      #if defined(TRICE_MODBUS_MIN_ID) && defined(TRICE_MODBUS_MAX_ID)
    //      if( (TRICE_MODBUS_MIN_ID < triceID) && (triceID < TRICE_MODBUS_MAX_ID) )
    //      #endif
    //      TriceWriteDeviceModbus( enc, encLen );
    //  #endif
    #ifdef TRICE_AUXILIARY
        #if defined(TRICE_AUXILIARY_MIN_ID) && defined(TRICE_AUXILIARY_MAX_ID)
        if( (TRICE_AUXILIARY_MIN_ID < triceID) && (triceID < TRICE_AUXILIARY_MAX_ID) )
        #endif
        { TriceWriteDeviceAuxiliary( pBuf, len ); }
    #endif
}

#if TRICE_SEGGER_RTT_32BIT_WRITE
//! SEGGER_Write_RTT0_NoCheck32 was derived from SEGGER_RTT.c version 7.60g function _WriteNoCheck for speed reasons. If using a different version please review the code first.
static void SEGGER_Write_RTT0_NoCheck32( const uint32_t* pData, unsigned NumW ) {
    unsigned NumWordsAtOnce;
    unsigned WrOff;
    unsigned RemW;
    // Get "to-host" ring buffer.
    static SEGGER_RTT_BUFFER_UP * const pRingUp0 = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[0] + SEGGER_RTT_UNCACHED_OFF);  // Access uncached to make sure we see changes made by the J-Link side and all of our changes go into HW directly
    WrOff = pRingUp0->WrOff;
    RemW = (pRingUp0->SizeOfBuffer - WrOff)>>2;
    volatile uint32_t* pDstW = (uint32_t*)((pRingUp0->pBuffer + WrOff) + SEGGER_RTT_UNCACHED_OFF);
    if (RemW > NumW) { // All data fits before wrap around
        WrOff += NumW<<2;
        while (NumW--) {
            *pDstW++ = *pData++;
        };
        RTT__DMB();                     // Force data write to be complete before writing the <WrOff>, in case CPU is allowed to change the order of memory accesses
        pRingUp0->WrOff = WrOff;
    } else { // We reach the end of the buffer, so need to wrap around
        NumWordsAtOnce = RemW;
        while (NumWordsAtOnce--) {
            *pDstW++ = *pData++;
        };
        pDstW = (uint32_t*)(pRingUp0->pBuffer + SEGGER_RTT_UNCACHED_OFF);
        NumWordsAtOnce = NumW - RemW;
        while (NumWordsAtOnce--) {
            *pDstW++ = *pData++;
        };
        RTT__DMB();                     // Force data write to be complete before writing the <WrOff>, in case CPU is allowed to change the order of memory accesses
        pRingUp0->WrOff = (NumW - RemW)<<2;
    }
}
#endif // #if TRICE_SEGGER_RTT_32BIT_WRITE



#ifdef TRICE_CGO
void TriceWriteDeviceCgo( uint8_t *buf, size_t len ){
    void triceNonBlockingWriteCgoBuffer( uint8_t const * buf, unsigned len );
    triceNonBlockingWriteCgoBuffer( buf, len ); // defined in cgoTrice.c
}
#endif









// | TRICE_DATA_OFFSET | data | ...
//! TriceDirectWrite copies a single trice from triceSingleBuffer to output.
//! This is the time critical part, executed inside TRICE_LEAVE.
//! The trice data start at triceStart and last wordCount values including 1-3 padding bytes at the end.
//! In front of triceStart is TRICE_DATA_OFFSET bytes space usable for in-buffer encoding.
void TriceDirectWrite( uint32_t* const triceStart, int wordCount ){

    #if defined(TRICE_RTT0) && TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_NONE
        #if TRICE_SEGGER_RTT_32BIT_WRITE
            SEGGER_Write_RTT0_NoCheck32( triceStart, wordCount );
        #else // normal SEGGER RTT
            size_t len = wordCount<<2; // len is the trice len without TRICE_OFFSET but with padding bytes.
            SEGGER_RTT_WriteNoLock(0, triceStart, len ); // no encoding!!!!!!!!!!!
        #endif
    #else // encode data
	//todo:        singleTriceDirectOut( triceSingleBuffer, len ); // with encoding
    #endif

}


#if defined( TRICE_UARTA ) // deferred out to UART

//! triceOutBufferUartA points into the double or ring buffer to the next (encoded) trice package.
static uint8_t const * triceOutBufferUartA;

//! triceOutCountUartA is the not yet transmitted byte count after a triceNonBlockingWriteUartA() call.
static size_t triceOutCountUartA = 0;

//! triceOutIndexUartA is the triceOutBufferUartA offset to the next to transmit byte.
static unsigned triceOutIndexUartA = 0;

//! triceNonBlockingWriteUartA registers a buffer for TRICE_UARTA transmission.
//! \param buf is byte buffer start.
//! \param nByte is the number of bytes to transfer
static size_t triceNonBlockingWriteUartA( void const * buf, size_t nByte ){
    triceOutBufferUartA = buf;
    triceOutIndexUartA = 0;
    triceOutCountUartA = nByte;
    return nByte;
}

//! TriceOutDepthUartA returns the amount of bytes not written yet to UARTB.
unsigned TriceOutDepthUartA( void ){
    unsigned depth = triceOutCountUartA - triceOutIndexUartA;
    return depth;
}

//! TriceNextUint8UartA returns the next trice byte for transmission to TRICE_UARTA.
uint8_t TriceNextUint8UartA( void ){
    return triceOutBufferUartA[triceOutIndexUartA++];
}

//! triceServeTransmitUartA must be called cyclically to proceed ongoing write out.
//! A good place is UARTA ISR.
void triceServeTransmitUartA(void) {
    triceTransmitData8UartA(TriceNextUint8UartA());
    if (0 == TriceOutDepthUartA()) { // no more bytes
        triceDisableTxEmptyInterruptUartA();
    }
}

// triceTriggerTransmitUartA must be called cyclically to initialize write out.
void triceTriggerTransmitUartA(void){
    if( TriceOutDepthUartA() && triceTxDataRegisterEmptyUartA() ){
        triceEnableTxEmptyInterruptUartA(); // next bytes
    }
}
#endif // #if defined( TRICE_UARTA ) && defined( TRICE_HALF_BUFFER_SIZE )

#if defined( TRICE_UARTB ) // deferred out to UART

//! triceOutBufferUartB holds the uart out buffer address.
static uint8_t const * triceOutBufferUartB;

//! triceOutCountUartB holds th uarts out buffer size.
static size_t triceOutCountUartB = 0;

//! triceOutIndexUartB is the next to transmit byte index.
static unsigned triceOutIndexUartB = 0;

//! triceNonBlockingWriteUartB registers a buffer for TRICE_UARTB transmission.
//! \param buf is byte buffer start.
//! \param nByte is the number of bytes to transfer
static size_t triceNonBlockingWriteUartB( void const * buf, size_t nByte ){
    triceOutBufferUartB = buf;
    triceOutIndexUartB = 0;
    triceOutCountUartB = nByte;
    return nByte;
}

//! TriceOutDepthUartB returns the amount of bytes not written yet to UARTB.
unsigned TriceOutDepthUartB( void ){
    // unsigned depthRtt0 = 0; -> assuming RTT is fast enough
    unsigned depth = triceOutCountUartB - triceOutIndexUartB;
    return depth;
}

//! TriceNextUint8UartB returns the next trice byte for transmission to TRICE_UARTA.
uint8_t TriceNextUint8UartB( void ){
    return triceOutBufferUartB[triceOutIndexUartB++];
}

//! triceServeTransmitUartB must be called cyclically to proceed ongoing write out.
//! A good place is UARTA ISR.
void triceServeTransmitUartB(void) {
    triceTransmitData8UartB(TriceNextUint8UartB());
    if (0 == TriceOutDepthUartB()) { // no more bytes
        triceDisableTxEmptyInterruptUartB();
    }
}

// triceTriggerTransmitUartB must be called cyclically to initialize write out.
void triceTriggerTransmitUartB(void){
    if( TriceOutDepthUartB() && triceTxDataRegisterEmptyUartB() ){
        triceEnableTxEmptyInterruptUartB(); // next bytes
    }
}
#endif // #if defined( TRICE_UARTA ) && defined( TRICE_HALF_BUFFER_SIZE )

//! TriceOutDepth returns the amount of bytes not written yet from the slowest device.
unsigned TriceOutDepth( void ){
    unsigned d = 0, depth = 0;
    #ifdef TRICE_RTT0
        // When no RTT host is connected, the RTT buffer runs full.
        // If a RTT host is connected, it is assumed to be the fastest device.
        // d = BUFFER_SIZE_UP - SEGGER_RTT_GetAvailWriteSpace(TRICE_RTT0);
        // depth = d > depth ? d : depth;
    #endif
    #if defined( TRICE_UARTA ) 
        d = TriceOutDepthUartA();
        depth = d > depth ? d : depth;
    #endif
    #if defined( TRICE_UARTB )
        d = TriceOutDepthUartB();
        depth = d > depth ? d : depth;
    #endif
    #ifdef TRICE_CGO
        d = TriceOutDepthCGO();
        depth = d > depth ? d : depth;
    #endif
    depth = d > depth ? d : depth;
    return depth;
}

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
        #if TRICE_TRANSFER_MODE == TRICE_SAFE_SINGLE_MODE
        encLen += TriceEncode( enc+encLen, triceStart, triceLen );
        #endif
    }
    #ifdef TRICE_RTT0
    TriceWriteDeviceRtt0( enc, encLen ); //lint !e534
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
//#endif // #ifdef TRICE_RTT0

#ifdef TRICE_RTT0
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
#endif // #ifdef TRICE_RTT0




#endif

