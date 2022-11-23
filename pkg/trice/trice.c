//! \file trice.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////
#include "trice.h"
#include "tcobs.h"
#include "xtea.h"

// triceCommands is to make 32-bit alignment sure
static uint32_t triceCommands[(TRICE_COMMAND_SIZE_MAX+3)>>2]; // with terminating 0

//! triceCommand is the command receive buffer.
uint8_t* const triceCommandBuffer = (uint8_t*)&triceCommands; // with terminating 0

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
//! \li ______v___________________v__________v__ (32-bit alignment positions)
//! \li *da = 111iiiiiI TT        TT      NC ... | ID(n): After writing 11iiiiiiI write the 32-bit TTTT value in 2 16-bit write operations.
//! \li *da = 101iiiiiI 10iiiiiiI TT      NC ... | Id(n): Write 10iiiiiiI as doubled value in one 32-bit operation into the trice buffer. The first 16-bit will be removed just before sending to the out channel. 
//! \li *da = 011iiiiiI                   NC ... | id(n): Just write 01iiiiiiI as 16-nit operation.
//! \li *da = 001iiiiiI TT        TTTT TT NC ... | iD(n): like ID but with 64-bit stamp instead of 32-bit stamp
//! \li *da = ss0......extended trices are not used yet
//! \li This way, after writing the 16-bit NC value the payload starts always at a 32-bit boundary.
static size_t triceDataLen( uint8_t const* p ){
    uint16_t nc = *(uint16_t*)p; //lint !e826
    size_t n = nc>>8;
    if( n < 128 ){
        return n;
    }
    return nc & 0x7fff;
}

const int TriceTypeS0 = 3; //!< TriceTypeS0 ist a trice without stamp.
const int TriceTypeS2 = 5; //!< TriceTypeS2 ist a trice with 16-bit stamp.
const int TriceTypeS4 = 7; //!< TriceTypeS4 ist a trice with 32-bit stamp.
const int TriceTypeS8 = 1; //!< TriceTypeS8 ist a trice with 64-bit stamp.
const int TriceTypeX0 = 0; //!< TriceTypeX0 ist a unspecified trice extension.
const int TriceTypeX1 = 2; //!< TriceTypeX0 ist a unspecified trice extension.
const int TriceTypeX2 = 4; //!< TriceTypeX0 ist a unspecified trice extension.
const int TriceTypeX3 = 6; //!< TriceTypeX0 ist a unspecified trice extension.

//! triceErrorCount is incremented, when data inside the internal trice buffer are corrupted.
//! That could happen, when the buffer wrapped before data are sent.
unsigned triceErrorCount = 0;

//! nextTrice expects at *buf 32-bit aligned trice messages and returns the next one in pStart and pLen.
//! *buf is filled with the advanced buf and *pSize gets the reduced value.
//! \retval is the trice ID on success or negative on error.
static int nextTrice( uint8_t** buf, size_t* pSize, uint8_t** pStart, size_t* pLen ){
    uint16_t* pNC = (uint16_t*)*buf; //lint !e826, get NC address
    int NC = *pNC; // get NC
    int triceID = 0x1FFF & NC;
    int triceType = NC >> 13; // todo: this will not work with extended trices only 1 byte long
    unsigned offset = 0;
    size_t size = *pSize;
    size_t triceSize;
    size_t len;
    *pStart = *buf;
    switch( triceType ){
        default:
        case TriceTypeS0: // S0 = no stamp
            len = 4 + triceDataLen(*pStart + 2); // tyId
            break;
        case TriceTypeS2: // S2 = 16-bit stamp
            *pStart += 2; // see Id(n) macro definition
            offset = 2;
            len = 6 + triceDataLen(*pStart + 4); // tyId ts16
            break;
        case TriceTypeS4: // S4 = 32-bit stamp
            len = 8 + triceDataLen(*pStart + 6); // tyId ts32
            break;
        case TriceTypeS8: // S8 = 64-bit stamp
            len = 12 + triceDataLen(*pStart + 10); // tyId ts64
            //len = size; // todo: Change that when needed.
            //// Extended trices without length information cannot be separated here.
            //// But it is possible to store them with length information and to remove it here.
            break;
        case TriceTypeX0:
        case TriceTypeX1:
        case TriceTypeX2:
        case TriceTypeX3:
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
    #ifdef TRICE_ENCRYPT
    len = (len + 7) & ~7; // only multiple of 8 encryptable
    TriceEncrypt( enc + TRICE_DATA_OFFSET, len>>2 );
    #endif
    encLen = (size_t)TCOBSEncode(enc, buf, len);
    enc[encLen++] = 0; // Add zero as package delimiter.
    return encLen;
}

//! TriceWriteDevice sends data to enumerated output device.
void TriceWriteDevice( TriceWriteDevice_t device, uint8_t *buf, size_t len ){
    switch( device ){
#ifdef TRICE_UARTA
        case UartA:
            #if TRICE_MODE == TRICE_STACK_BUFFER
                TriceBlockingWriteUartA( buf, len ); // direct out to UART
            #else
                triceNonBlockingWriteUartA( buf, len ); // // buffered out to UARTA
            #endif // #if TRICE_MODE == TRICE_STACK_BUFFER
            break;
#endif
#ifdef TRICE_UARTB
        case UartB:
            #if TRICE_MODE == TRICE_STACK_BUFFER
                TriceBlockingWriteUartB( buf, len ); // direct out to UART
            #else
                triceNonBlockingWriteUartB( buf, len ); // // buffered out to UARTB
            #endif // #if TRICE_MODE == TRICE_STACK_BUFFER
            break;
#endif
#ifdef TRICE_RTT0
        case Rtt0:
            SEGGER_RTT_Write(0, buf, len );
            break;
#endif
#ifdef TRICE_CGO
        case Cgo:
        {
            void triceNonBlockingWriteCgoBuffer( uint8_t const * buf, unsigned len );
            triceNonBlockingWriteCgoBuffer( buf, len ); 
        }
            break;
#endif
        default:
            break;
    }
}

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
    #ifdef TRICE_UARTA
    if( (TRICE_UARTA_MIN_ID < triceID) && (triceID < TRICE_UARTA_MAX_ID) ){
        TriceWriteDevice( UartA, enc, encLen ); //lint !e534
    }
    #endif
    #ifdef TRICE_UARTB
    if( (TRICE_UARTB_MIN_ID < triceID) && (triceID < TRICE_UARTB_MAX_ID) ){
        TriceWriteDevice( UartB, enc, encLen ); //lint !e534
    }
    #endif
    #ifdef TRICE_RTT0
        TriceWriteDevice( Rtt0, enc, encLen ); //lint !e534
    #endif
    #ifdef TRICE_CGO
        TriceWriteDevice( Cgo, enc, encLen );
    #endif
}

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

#if defined( TRICE_UARTA ) && TRICE_DEFERRED_OUT // buffered out to UART
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

#if defined( TRICE_UARTB ) && TRICE_DEFERRED_OUT // buffered out to UART
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
    // unsigned depthRtt0 = 0; -> assuming RTT is fast enough
    #ifdef TRICE_UARTA
    unsigned depthUartA = TriceOutDepthUartA();
    #else
    unsigned depthUartA = 0;
    #endif
    #ifdef TRICE_UARTB
    unsigned depthUartB = TriceOutDepthUartB();
    #else
    unsigned depthUartB = 0;
    #endif
    unsigned depth = depthUartA > depthUartB ? depthUartA : depthUartB;
    return depth;
}
