//! \file trice.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////

#include "trice.h"
#include "tcobs.h"
#include "cobs.h"
#include "xtea.h"

// function prototypes:

static size_t triceDataLen( uint8_t const* p );

#if defined( TRICE_UARTA )
static void triceNonBlockingWriteUartA( void const * buf, size_t nByte );
#endif

#if defined( TRICE_UARTB )
static void triceNonBlockingWriteUartB( void const * buf, size_t nByte );
#endif

#if TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1
static void SEGGER_Write_RTT0_NoCheck32( const uint32_t* pData, unsigned NumW );
#endif

// local definines:

#define TRICE_TYPE_X0 0 //!< TRICE_TYPE_X0 ist a unspecified trice (reserved)
#define TRICE_TYPE_S0 1 //!< TRICE_TYPE_S0 ist a trice without stamp.
#define TRICE_TYPE_S2 2 //!< TRICE_TYPE_S2 ist a trice with 16-bit stamp.
#define TRICE_TYPE_S4 3 //!< TRICE_TYPE_S4 ist a trice with 32-bit stamp.

// global variables:

//! triceErrorCount is incremented, when data inside the internal trice buffer are corrupted.
//! That could happen, when the buffer wrapped before data are sent. The user app should check this diagnostic value.
unsigned triceErrorCount = 0;

//! triceCommand is the command receive buffer.
char triceCommandBuffer[TRICE_COMMAND_SIZE_MAX+1]; // with terminating 0

//! triceCommandFlag ist set, when a command was received completely.
int triceCommandFlag = 0; // updated

#if TRICE_CYCLE_COUNTER == 1

//! TriceCycle is increased and transmitted with each trice message, if enabled.
uint8_t  TriceCycle = 0xc0;

#endif

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
    uint16_t nc = TRICE_TTOHS(*(uint16_t*)p); // lint !e826
    size_t n = nc>>8;
    if( n < 128 ){
        return n;
    }
    return nc & 0x7fff;
}

//! TriceIDAndBuffer evaluates a trice message and returns the ID for routing.
//! \param pAddr is where the trice message starts.
//! \param pWordCount is filled with the word count the trice data occupy from pAddr.
//! \param ppStart is filled with the trice netto data start. That is maybe a 2 bytes offset from pAddr.
//! \param pLength is filled with the netto trice length (without padding bytes), 0 on error.
//! \retval is the triceID, a positive value on success or error information.
int TriceIDAndBuffer( uint32_t const * const pAddr, int* pWordCount, uint8_t** ppStart, size_t* pLength ){
    uint16_t TID = TRICE_TTOHS( *(uint16_t*)pAddr ); // type and id
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
            pStart += 2; // see Id(n) macro definition
            offset = 2;
            len = 6 + triceDataLen(pStart + 4); // tyId ts16
            break;
        case TRICE_TYPE_S4: // S4 = 32-bit stamp
            offset = 0;
            len = 8 + triceDataLen(pStart + 6); // tyId ts32
            break;
        default:
            // fallthrugh
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
    unsigned TID = TRICE_TTOHS( *pTID ); // type and id
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

//! TriceDeferredEncode expects at buf trice date with netto length len.
//! \param enc is the destination.
//! \param buf is the source.
//! \param len is the source len.
//! \retval is the encoded len with 0-delimiter byte.
size_t TriceDeferredEncode( uint8_t* enc, uint8_t const* buf, size_t len ){
    size_t encLen;
    #ifdef XTEA_ENCRYPT_KEY
    len = (len + 7) & ~7; // todo: only multiple of 8 encryptable (needs some adaptions, later!)
    XTEAEncrypt( (uint32_t*)(enc + TRICE_DATA_OFFSET), len>>2 );
    #endif
    #if TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS
    encLen = (size_t)TCOBSEncode(enc, buf, len);
    enc[encLen++] = 0; // Add zero as package delimiter.
    #elif TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_COBS
    encLen = (size_t)COBSEncode(enc, buf, len);
    enc[encLen++] = 0; // Add zero as package delimiter.
    #elif TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_NONE
    memmove( enc, buf, len );
    encLen = len;
    #else
    #error unknown TRICE_DEFERRED_OUT_FRAMING
    #endif
    return encLen;
}

#if TRICE_DIRECT_OUTPUT_WITH_ROUTING == 1

//! TriceDirectEncode expects at buf trice date with netto length len.
//! \param enc is the destination.
//! \param buf is the source.
//! \param len is the source len.
//! \retval is the encoded len with 0-delimiter byte.
static size_t triceDirectEncode( uint8_t* enc, uint8_t const* buf, size_t len ){
    size_t encLen;
    #ifdef XTEA_ENCRYPT_KEY
    len = (len + 7) & ~7; // only multiple of 8 encryptable
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

#endif // #if TRICE_DIRECT_OUTPUT_WITH_ROUTING == 1

#if (TRICE_DIAGNOSTICS ==1) && defined(SEGGER_RTT)

unsigned RTT0_writeSpaceMin = BUFFER_SIZE_UP; //! RTT0_writeSpaceMin is usable for diagnostics.
unsigned RTT0_bytesInBufferMax = 0;        //! RTT0_bytesInBufferMax is usable for diagnostics.

static void triceSeggerRTTDiagnostics( void ){
    unsigned writeSpace = SEGGER_RTT_GetAvailWriteSpace (0);
    unsigned bytesInBuffer = SEGGER_RTT_GetBytesInBuffer(0);
    RTT0_writeSpaceMin    = RTT0_writeSpaceMin    < writeSpace    ? RTT0_writeSpaceMin    : writeSpace;
    RTT0_bytesInBufferMax = RTT0_bytesInBufferMax > bytesInBuffer ? RTT0_bytesInBufferMax : bytesInBuffer;
}

void TriceLogSeggerDiagnostics( void ){
    if( (RTT0_writeSpaceMin > 4) && (RTT0_bytesInBufferMax < BUFFER_SIZE_UP - 4) ){
        trice( iD( 1278), "diag:RTT0_bytesInBufferMax=%u, RTT0_writeSpaceMin\n", RTT0_bytesInBufferMax, RTT0_writeSpaceMin );
    }else{
        trice( iD( 1249), "err:RTT0_bytesInBufferMax=%u, RTT0_writeSpaceMin\n", RTT0_bytesInBufferMax, RTT0_writeSpaceMin );
    }
}

#endif // #if (TRICE_DIAGNOSTICS ==1) && defined(SEGGER_RTT)

#if (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) || (TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1)

void TriceWriteDeviceRtt0( uint8_t *buf, size_t len ){
    SEGGER_RTT_WriteNoLock(0, buf, len );

    #if TRICE_DIAGNOSTICS == 1
    triceSeggerRTTDiagnostics();
    #endif
}

#endif // #if TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1

// TriceNonBlockingWrite routes trice data to output channels.
void TriceNonBlockingWrite( int triceID, uint8_t const * pBuf, size_t len ){
    
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
    #if defined( TRICE_AUXILIARY )
        #if defined(TRICE_AUXILIARY_MIN_ID) && defined(TRICE_AUXILIARY_MAX_ID)
        if( (TRICE_AUXILIARY_MIN_ID < triceID) && (triceID < TRICE_AUXILIARY_MAX_ID) )
        #endif
        { TriceWriteDeviceAuxiliary( pBuf, len ); }
    #endif
    #ifdef TRICE_CGO
        TriceWriteDeviceCgo( pBuf, len );
    #endif
    #if (TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1) // && !defined(TRICE_DOUBLE_BUFFER) && !defined(TRICE_RING_BUFFER) // only when RTT0 is alone
        #if defined(TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE_MIN_ID) && defined(TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE_MAX_ID)
        if( (TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE_MIN_ID < triceID) && (triceID < TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE_MAX_ID) )
        #endif
        TriceWriteDeviceRtt0( enc, encLen );
    #endif
    //  #ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO
    //      #if defined(TRICE_MODBUS_MIN_ID) && defined(TRICE_MODBUS_MAX_ID)
    //      if( (TRICE_MODBUS_MIN_ID < triceID) && (triceID < TRICE_MODBUS_MAX_ID) )
    //      #endif
    //      TriceWriteDeviceModbus( enc, encLen );
    //  #endif
}

#if TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1
//! SEGGER_Write_RTT0_NoCheck32 was derived from SEGGER_RTT.c version 7.60g function _WriteNoCheck for speed reasons. If using a different version please review the code first.
static void SEGGER_Write_RTT0_NoCheck32( const uint32_t* pData, unsigned NumW ) {
    unsigned NumWordsAtOnce;
    unsigned WrOff;
    unsigned RemW;
    // Get "to-host" ring buffer.
    static SEGGER_RTT_BUFFER_UP * const pRingUp0 = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[0] + SEGGER_RTT_UNCACHED_OFF);  // Access uncached to make sure we see changes made by the J-Link side and all of our changes go into HW directly
    WrOff = pRingUp0->WrOff;
    RemW = (pRingUp0->SizeOfBuffer - WrOff)>>2;
    volatile uint32_t* pDstW = (uint32_t*)((pRingUp0->pBuffer + WrOff) + SEGGER_RTT_UNCACHED_OFF); //lint !e826
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
        pDstW = (uint32_t*)(pRingUp0->pBuffer + SEGGER_RTT_UNCACHED_OFF); //lint !e826
        NumWordsAtOnce = NumW - RemW;
        while (NumWordsAtOnce--) {
            *pDstW++ = *pData++;
        };
        RTT__DMB();                     // Force data write to be complete before writing the <WrOff>, in case CPU is allowed to change the order of memory accesses
        pRingUp0->WrOff = (NumW - RemW)<<2;
    }
    #if TRICE_DIAGNOSTICS == 1
    triceSeggerRTTDiagnostics();
    #endif
}
#endif // #if TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1

#if TRICE_32BIT_DIRECT_XTEA_AND_COBS
//! TriceEncryptAndCobsFraming32 does an in-buffer encryption and COBS encoding of a single trice message.
//! \param triceStart is the start of the trice message. In front of it is TRICE_DATA_OFFSET bytes space for in-buffer encoding.
//! The result data are starting TRICE_DATA_OFFSET bytes before triceStart.
//! Up to 4 bytes behind the trice message are used as scratch area, what makes the code faster. Be careful when used in deferred output.
//! \param wordCount is the amount of trice message 32-bit values. When this value is odd, it is internally incremented by 1, so that 4 more (garbage) bytes are encrypted.
//! This is ok, because the trice message internally carries its length and the additional data are ignored then.
//! \retval wordCount is the word count stored at dest. The resulting message gets a 0-delimiter byte and 1-3 padding zeroes.
unsigned TriceEncryptAndCobsFraming32( uint32_t * const triceStart, unsigned wordCount ){
    unsigned wcEven = ((wordCount + 1) & ~1); // only multiple of 8 can be encrypted 
    XTEAEncrypt( triceStart, wcEven ); // in-buffer encryption
    uint8_t* enc = ((uint8_t*)triceStart) - TRICE_DATA_OFFSET;
    unsigned encLen = COBSEncode(enc, triceStart, wcEven<<2);
    do{
        enc[encLen++] = 0; // add 0-delimiter and optional padding zeroes
    }while( (encLen & 3) != 0 ); 
    return encLen>>2;
}
#endif // #if TRICE_SEGGER_RTT_32BIT_DIRECT_XTEA_AND_COBS

#if TRICE_DIRECT_OUTPUT_WITH_ROUTING == 1

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


// triceNonBlockingDirectWrite routes trice data to output channels.
static void triceNonBlockingDirectWrite( int triceID, uint8_t const * pBuf, size_t len ){

    #if defined( TRICE_DIRECT_AUXILIARY )
        #if defined(TRICE_DIRECT_AUXILIARY_MIN_ID) && defined(TRICE_DIRECT_AUXILIARY_MAX_ID)
        if( (TRICE_DIRECT_AUXILIARY_MIN_ID < triceID) && (triceID < TRICE_DIRECT_AUXILIARY_MAX_ID) )
        #endif
        { TriceDirectWriteDeviceAuxiliary( pBuf, len ); }
    #endif

    //  #ifdef TRICE_CGO
    //      TriceWriteDeviceCgo( pBuf, len );
    //  #endif
}

#endif // #if TRICE_DIRECT_OUTPUT_WITH_ROUTING == 1

#if TRICE_DIRECT_OUTPUT == 1

//! TriceDirectWrite copies a single trice from triceStart to output.
//! This is the time critical part, executed inside TRICE_LEAVE.
//! The trice data start at triceStart and last wordCount values including 1-3 padding bytes at the end.
//! In front of triceStart is TRICE_DATA_OFFSET bytes space usable for in-buffer encoding.
void TriceDirectWrite( uint32_t * triceStart, unsigned wordCount ){

    #if TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1
        #if TRICE_32BIT_DIRECT_XTEA_AND_COBS == 1
            wordCount = TriceEncryptAndCobsFraming32( triceStart, wordCount );
            triceStart -= TRICE_DATA_OFFSET>>2;
        #endif // #if TRICE_SEGGER_RTT_32BIT_DIRECT_XTEA_AND_COBS
        SEGGER_Write_RTT0_NoCheck32( triceStart, wordCount );
    #endif

    #if TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1// normal SEGGER RTT without framing
        #if TRICE_32BIT_DIRECT_XTEA_AND_COBS == 1
            wordCount = TriceEncryptAndCobsFraming32( triceStart, wordCount );
            triceStart -= TRICE_DATA_OFFSET>>2;
        #endif // #if TRICE_SEGGER_RTT_32BIT_DIRECT_XTEA_AND_COBS
        size_t len8 = wordCount<<2; // len8 is the trice len without TRICE_OFFSET but with padding bytes.
        TriceWriteDeviceRtt0( triceStart, len8 );
    #endif

    #if TRICE_DIRECT_OUTPUT_WITH_ROUTING == 1
    int triceID;
    size_t len = triceIDAndLen( triceStart, &triceID );
    uint8_t* triceStart8 = (uint8_t*)triceStart;
    uint8_t* enc = triceStart8 - TRICE_DATA_OFFSET;
    size_t encLen = triceDirectEncode( enc, triceStart8, len );
    triceNonBlockingDirectWrite( triceID, enc, encLen );
    #endif // #if TRICE_DIRECT_OUTPUT_WITH_ROUTING == 1
}

#endif // #if TRICE_DIRECT_OUTPUT == 1

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
static void triceNonBlockingWriteUartA( void const * buf, size_t nByte ){
    triceOutBufferUartA = buf;
    triceOutIndexUartA = 0;
    triceOutCountUartA = nByte;
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

#endif // #if defined( TRICE_UARTA )

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
static void triceNonBlockingWriteUartB( void const * buf, size_t nByte ){
    triceOutBufferUartB = buf;
    triceOutIndexUartB = 0;
    triceOutCountUartB = nByte;
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

#endif // #if defined( TRICE_UARTB )

//! TriceOutDepth returns the amount of bytes not written yet from the slowest device.
unsigned TriceOutDepth( void ){
    unsigned d = 0, depth = 0;
    #ifdef SEGGER_RTT
        // When no RTT host is connected, the RTT buffer runs full.
        // If a RTT host is connected, it is assumed to be the fastest device.
    #endif
    #if defined( TRICE_UARTA ) 
        d = TriceOutDepthUartA(); //lint !e838
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
