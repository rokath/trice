//! \file trice.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////
#include "trice.h"
#if TRICE_FRAMING == TRICE_FRAMING_TCOBS
#include "tcobs.h"
#endif
#if TRICE_FRAMING == TRICE_FRAMING_COBS
#include "cobs.h"
#endif
#ifdef XTEA_ENCRYPT_KEY
#include "xtea.h"
#endif

// triceCommands is to make 32-bit alignment sure
static uint32_t triceCommands[(TRICE_COMMAND_SIZE_MAX+3)>>2]; // with terminating 0

//! triceCommand is the command receive buffer.
char* const triceCommandBuffer = (char*)&triceCommands; // with terminating 0

//! triceCommandLength contains the count of valid bytes inside triceCommand including the terminating 0.
int triceCommandLength = 0;

//! triceCommandFlag ist set, when a command was received completely.
int triceCommandFlag = 0; // updated

unsigned triceDepthMax = 0; //!< triceDepthMax is a diagnostics value usable to optimize buffer size.


#if TRICE_CYCLE_COUNTER == 1
uint8_t  TriceCycle = 0xc0; //!< TriceCycle is increased and transmitted with each trice message, if enabled.
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
    uint16_t nc = TRICE_TTOHS(*(uint16_t*)p); //lint !e826
    size_t n = nc>>8;
    if( n < 128 ){
        return n;
    }
    return nc & 0x7fff;
}

#define TRICE_TYPE_X0 0 //!< TRICE_TYPE_X0 ist a unspecified trice (reserved)
#define TRICE_TYPE_S0 1 //!< TRICE_TYPE_S0 ist a trice without stamp.
#define TRICE_TYPE_S2 2 //!< TRICE_TYPE_S2 ist a trice with 16-bit stamp.
#define TRICE_TYPE_S4 3 //!< TRICE_TYPE_S4 ist a trice with 32-bit stamp.

//! triceErrorCount is incremented, when data inside the internal trice buffer are corrupted.
//! That could happen, when the buffer wrapped before data are sent.
unsigned triceErrorCount = 0;

    uint16_t* pTID;
    int TID; // type and id
    int triceID;
    int triceType;
    size_t size;
    size_t triceSize;
    size_t len;


//! nextTrice expects at *buf 32-bit aligned trice messages and returns the next one in pStart and pLen.
//! *buf is filled with the advanced buf and *pSize gets the reduced value.
//! \retval is the trice ID on success or negative on error.
static int nextTrice( uint8_t** buf, size_t* pSize, uint8_t** pStart, size_t* pLen ){
    pTID = (uint16_t*)*buf; //lint !e826, get TID address
    TID = TRICE_TTOHS( *pTID ); // type and id
    triceID = 0x3FFF & TID;
    triceType = TID >> 14;
    unsigned offset = 0;
    /*size_t*/ size = *pSize;
    //*size_t*/ triceSize;
    //*size_t*/ len;
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

static size_t triceEncode( uint8_t* enc, uint8_t const* buf, size_t len ){
    size_t encLen;
    #ifdef XTEA_ENCRYPT_KEY
    len = (len + 7) & ~7; // only multiple of 8 encryptable
    XTEAEncrypt( (uint32_t*)(enc + TRICE_DATA_OFFSET), len>>2 );
    #endif
    #if TRICE_FRAMING == TRICE_FRAMING_TCOBS
    encLen = (size_t)TCOBSEncode(enc, buf, len);
    enc[encLen++] = 0; // Add zero as package delimiter.
    #elif TRICE_FRAMING == TRICE_FRAMING_COBS
    encLen = (size_t)COBSEncode(enc, buf, len);
    enc[encLen++] = 0; // Add zero as package delimiter.
    #elif TRICE_FRAMING == TRICE_FRAMING_NONE
    memmove( enc, buf, len );
    encLen = len;
    #else
    #error unknown TRICE_FRAMING
    #endif
    return encLen;
}

#ifdef TRICE_RTT0

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
#endif // #ifdef TRICE_RTT0

#if defined( TRICE_UARTA ) && ((TRICE_MODE == TRICE_DOUBLE_BUFFER) || (TRICE_MODE == TRICE_STREAM_BUFFER) )
void TriceWriteDeviceUartA( uint8_t *buf, size_t len ){
    #if TRICE_MODE == TRICE_STACK_BUFFER
        TriceBlockingWriteUartA( buf, len ); // direct out to UART
    #else
        triceNonBlockingWriteUartA( buf, len ); // // buffered out to UARTA
    #endif // #if TRICE_MODE == TRICE_STACK_BUFFER
}
#endif

#if defined( TRICE_UARTB ) && ((TRICE_MODE == TRICE_DOUBLE_BUFFER) || (TRICE_MODE == TRICE_STREAM_BUFFER) )
void TriceWriteDeviceUartB( uint8_t *buf, size_t len ){
    #if TRICE_MODE == TRICE_STACK_BUFFER
        TriceBlockingWriteUartB( buf, len ); // direct out to UART
    #else
        triceNonBlockingWriteUartB( buf, len ); // // buffered out to UARTB
    #endif // #if TRICE_MODE == TRICE_STACK_BUFFER
}
#endif

#ifdef TRICE_CGO
void TriceWriteDeviceCgo( uint8_t *buf, size_t len ){
    void triceNonBlockingWriteCgoBuffer( uint8_t const * buf, unsigned len );
    triceNonBlockingWriteCgoBuffer( buf, len ); // defined in cgoTrice.c
}
#endif

#ifdef TRICE_AUXILIARY
void TriceWriteDeviceAuxiliary( uint8_t *buf, size_t len ){
    // user code here
}
#endif

#ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO
void TriceWriteDeviceModbus( uint8_t *buf, size_t len ){
    TriceNonBlockingWriteModbusBuffer( buf, len ); 
}
#endif

#if TRICE_MODE == TRICE_STATIC_BUFFER
uint32_t triceSingleBuffer[(TRICE_DATA_OFFSET + TRICE_SINGLE_MAX_SIZE)>>2];
uint32_t* const triceSingleBufferStartWritePosition = &triceSingleBuffer[TRICE_DATA_OFFSET>>2];
uint32_t* TriceBufferWritePosition;
#endif

//! TriceOut encodes trices and writes them in one step to the output.
//! \param tb is start of uint32_t* trice buffer. The space TRICE_DATA_OFFSET at
//! the tb start is for in-buffer encoding of the trice data.
//! \param tLen is length of trice data. tlen is always a multiple of 4 because
//! of 32-bit alignment and padding bytes.
void TriceOut( uint32_t* tb, size_t tLen ){
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
        triceID = nextTrice( &buf, &len, &triceStart, &triceLen );
        if( triceID <= 0 ){ // on data error
            break;   // ignore following data
        }
        #if TRICE_TRANSFER_MODE == TRICE_SAFE_SINGLE_MODE
        encLen += triceEncode( enc+encLen, triceStart, triceLen );
        #endif
        #if  TRICE_TRANSFER_MODE == TRICE_PACK_MULTI_MODE
        memmove(enc + TRICE_DATA_OFFSET + encLen, triceStart, triceLen );
        encLen += triceLen;
        #endif
    }
    #if TRICE_TRANSFER_MODE == TRICE_PACK_MULTI_MODE
    encLen = triceEncode( enc, enc + TRICE_DATA_OFFSET, encLen);
    #endif
    ToggleOpticalFeedbackLED();
    
    // output
    
    #if defined( TRICE_UARTA ) && ((TRICE_MODE == TRICE_DOUBLE_BUFFER) || (TRICE_MODE == TRICE_STREAM_BUFFER) )
        #if defined(TRICE_UARTA_MIN_ID) && defined(TRICE_UARTA_MAX_ID)
        if( (TRICE_UARTA_MIN_ID < triceID) && (triceID < TRICE_UARTA_MAX_ID) )
        #endif
        TriceWriteDeviceUartA( enc, encLen );
    #endif
    #if defined( TRICE_UARTB ) && ((TRICE_MODE == TRICE_DOUBLE_BUFFER) || (TRICE_MODE == TRICE_STREAM_BUFFER) )
        #if defined(TRICE_UARTB_MIN_ID) && defined(TRICE_UARTB_MAX_ID)
        if( (TRICE_UARTB_MIN_ID < triceID) && (triceID < TRICE_UARTB_MAX_ID) )
        #endif
        TriceWriteDeviceUartB( enc, encLen );
    #endif
    //  #if defined(TRICE_RTT0) // && !defined(TRICE_DOUBLE_BUFFER) && !defined(TRICE_STREAM_BUFFER) // only when RTT0 is alone
    //      #if defined(TRICE_RTT0_MIN_ID) && defined(TRICE_RTT0_MAX_ID)
    //      if( (TRICE_RTT0_MIN_ID < triceID) && (triceID < TRICE_RTT0_MAX_ID) )
    //      #endif
    //      TriceWriteDeviceRtt0( enc, encLen ); //lint !e534
    //  #endif
    #ifdef TRICE_CGO
        TriceWriteDeviceCgo( enc, encLen );
    #endif
    #ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO
        #if defined(TRICE_MODBUS_MIN_ID) && defined(TRICE_MODBUS_MAX_ID)
        if( (TRICE_MODBUS_MIN_ID < triceID) && (triceID < TRICE_MODBUS_MAX_ID) )
        #endif
        TriceWriteDeviceModbus( enc, encLen );
    #endif
    #ifdef TRICE_AUXILIARY
        #if defined(TRICE_AUXILIARY_MIN_ID) && defined(TRICE_AUXILIARY_MAX_ID)
        if( (TRICE_AUXILIARY_MIN_ID < triceID) && (triceID < TRICE_AUXILIARY_MAX_ID) )
        #endif
        TriceWriteDeviceAuxiliary( enc, encLen );
    #endif
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
void singleTriceDirectOut( uint32_t* tb, size_t tLen ){
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
        triceID = nextTrice( &buf, &len, &triceStart, &triceLen );
        if( triceID <= 0 ){ // on data error
            break;   // ignore following data
        }
        #if TRICE_TRANSFER_MODE == TRICE_SAFE_SINGLE_MODE
        encLen += triceEncode( enc+encLen, triceStart, triceLen );
        #endif
    }
    #ifdef TRICE_RTT0
    TriceWriteDeviceRtt0( enc, encLen ); //lint !e534
    #endif
}

#ifdef TRICE_RTT0
//! TriceOut encodes trices and writes them in one step to the output.
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
        triceID = nextTrice( &buf, &len, &triceStart, &triceLen );
        if( triceID <= 0 ){ // on data error
            break;   // ignore following data
        }
        #if TRICE_TRANSFER_MODE == TRICE_SAFE_SINGLE_MODE
        encLen += triceEncode( enc+encLen, triceStart, triceLen );
        #endif
    }
    TriceWriteDeviceRtt0( enc, encLen ); //lint !e534
}
#endif


#if defined( TRICE_UARTA ) && !defined( TRICE_HALF_BUFFER_SIZE ) // direct out to UART
//! triceBlockingPutChar returns after c was successfully written.
static void triceBlockingPutCharUartA( uint8_t c ){
    while( !triceTxDataRegisterEmptyUartA() );
    triceTransmitData8UartA( c );
} 

//! TriceBlockingWriteUartA returns after buf was completely written.
void TriceBlockingWriteUartA( uint8_t const * buf, unsigned len ){
    for( unsigned i = 0; i < len; i++ ){ 
        triceBlockingPutCharUartA( buf[i] ); }
}
#endif // #if defined( TRICE_UARTA ) && !defined( TRICE_HALF_BUFFER_SIZE )

#if defined( TRICE_UARTA ) && ((TRICE_MODE == TRICE_DOUBLE_BUFFER) || (TRICE_MODE == TRICE_STREAM_BUFFER) ) // buffered out to UART
static uint8_t const * triceOutBufferUartA;
static size_t triceOutCountUartA = 0;
static unsigned triceOutIndexUartA = 0;

//! triceNonBlockingWriteUartA
size_t triceNonBlockingWriteUartA( void const * buf, size_t nByte ){
    triceOutBufferUartA = buf;
    triceOutIndexUartA = 0;
    triceOutCountUartA = nByte;
    return nByte;
}

//! TriceOutDepth returns the amount of bytes not written yet from the slowes device.
unsigned TriceOutDepthUartA( void ){
    // unsigned depthRtt0 = 0; -> assuming RTT is fast enough
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


#if defined( TRICE_UARTB ) && !defined( TRICE_HALF_BUFFER_SIZE ) // direct out to UART
//! triceBlockingPutChar returns after c was successfully written.
static void triceBlockingPutCharUartB( uint8_t c ){
    while( !triceTxDataRegisterEmptyUartB() );
    triceTransmitData8UartB( c );
} 

//! TriceBlockingWriteUartB returns after buf was completely written.
void TriceBlockingWriteUartB( uint8_t const * buf, unsigned len ){
    for( unsigned i = 0; i < len; i++ ){ 
        triceBlockingPutCharUartB( buf[i] ); }
}
#endif // #if defined( TRICE_UARTA ) && !defined( TRICE_HALF_BUFFER_SIZE )

#if defined( TRICE_UARTB ) && ((TRICE_MODE == TRICE_DOUBLE_BUFFER) || (TRICE_MODE == TRICE_STREAM_BUFFER) ) // buffered out to UART
static uint8_t const * triceOutBufferUartB;
static size_t triceOutCountUartB = 0;
static unsigned triceOutIndexUartB = 0;

//! triceNonBlockingWriteUartB
size_t triceNonBlockingWriteUartB( void const * buf, size_t nByte ){
    triceOutBufferUartB = buf;
    triceOutIndexUartB = 0;
    triceOutCountUartB = nByte;
    return nByte;
}

//! TriceOutDepth returns the amount of bytes not written yet from the slowes device.
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
    #if defined( TRICE_UARTA ) && ((TRICE_MODE == TRICE_DOUBLE_BUFFER) || (TRICE_MODE == TRICE_STREAM_BUFFER) )
        d = TriceOutDepthUartA();
        depth = d > depth ? d : depth;
    #endif
    #if defined( TRICE_UARTB ) && ((TRICE_MODE == TRICE_DOUBLE_BUFFER) || (TRICE_MODE == TRICE_STREAM_BUFFER) )
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


