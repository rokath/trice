//! \file trice.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////
#include "trice.h"
#include "tcobs.h"
#include "xtea.h"

//! triceCommand is the command receive buffer.
char triceCommand[TRICE_COMMAND_SIZE_MAX+1]; // with terminating 0

//! triceCommandFlag ist set, when a command was received completely.
int triceCommandFlag = 0; // updated

void TriceWrite( uint8_t const * buf, unsigned len );

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

//! triceErrorCount is incremented, when data inside the internal trice buffer are corrupted.
//! That could happen, when the buffer wrapped before data are sent.
unsigned triceErrorCount = 0;

//! nextTrice expects at *buf 32-bit aligned trice messages and returns the next one in pStart and pLen.
//! *buf is filled with the advanced buf and *pSize gets the reduced value.
//! \retval is 0 on success or negative on error.
static int nextTrice( uint8_t** buf, size_t* pSize, uint8_t** pStart, size_t* pLen ){
    uint16_t* pNC = (uint16_t*)*buf; //lint !e826 
    int triceType = *pNC >> 13; // todo: this will not work with extended trices only 1 byte long
    unsigned offset = 0;
    size_t size = *pSize;
    size_t triceSize;
    size_t len; 
    *pStart = *buf;
    switch( triceType ){
        default:
        case 3: // NOS = no stamp
            len = 4 + triceDataLen(*pStart + 2); // tyId
            break;
        case 5: // S16 = 16-bit stamp
            *pStart += 2; // see Id(n) macro definition
            offset = 2;
            len = 6 + triceDataLen(*pStart + 4); // tyId ts16
            break;
        case 7: // S32 = 32-bit stamp
            len = 8 + triceDataLen(*pStart + 6); // tyId ts32
            break;
        case 1: // S64 = 64-bit stamp
            len = 12 + triceDataLen(*pStart + 10); // tyId ts64
            //len = size; // todo: Change that when needed.
            //// Extended trices without length information cannot be separated here.
            //// But it is possible to store them with length information and to remove it here.
            break;
        case 0:
        case 2:
        case 4:
        case 6:
            break;
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
    return 0;
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
    // diagnostics
    tLen += TRICE_DATA_OFFSET; 
    triceDepthMax = tLen < triceDepthMax ? triceDepthMax : tLen;
    // do it
    while(len){
        uint8_t* triceStart;
        size_t triceLen;
        int r = nextTrice( &buf, &len, &triceStart, &triceLen );
        if( r < 0 ){ // on data error
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
    #if  TRICE_TRANSFER_MODE == TRICE_PACK_MULTI_MODE
    encLen = triceEncode( enc, enc + TRICE_DATA_OFFSET, encLen);
    #endif
    TriceWrite( enc, encLen ); //lint !e534
}

#if defined( TRICE_UART ) && !defined( TRICE_HALF_BUFFER_SIZE ) // direct out to UART
//! triceBlockingPutChar returns after c was successfully written.
static void triceBlockingPutChar( uint8_t c ){
    while( !triceTxDataRegisterEmpty() );
    triceTransmitData8( c );
} 

//! TriceBlockingWrite returns after buf was completely written.
void TriceBlockingWrite( uint8_t const * buf, unsigned len ){
    for( unsigned i = 0; i < len; i++ ){ 
        triceBlockingPutChar( buf[i] ); }
}
#endif // #if defined( TRICE_UART ) && !defined( TRICE_HALF_BUFFER_SIZE )

#if defined( TRICE_UART ) && TRICE_DEFERRED_OUT // buffered out to UART
static uint8_t const * triceOutBuffer;
static size_t triceOutCount = 0;
static unsigned triceOutIndex = 0;

//! triceNonBlockingWrite
size_t triceNonBlockingWrite( void const * buf, size_t nByte ){
    triceOutBuffer = buf;
    triceOutIndex = 0;
    triceOutCount = nByte;
    return nByte;
}

//! TriceOutDepth returns the amount of bytes not written yet.
unsigned TriceOutDepth( void ){
    return triceOutCount - triceOutIndex;
}

//! TriceNextUint8 returns the next trice byte for transmission.
uint8_t TriceNextUint8( void ){
    return triceOutBuffer[triceOutIndex++];
}

//! triceServeTransmit must be called cyclically to proceed ongoing write out.
//! A good place is UART ISR.
void triceServeTransmit(void) {
    triceTransmitData8(TriceNextUint8());
    if (0 == TriceOutDepth()) { // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}

// triceTriggerTransmit must be called cyclically to initialize write out.
void triceTriggerTransmit(void){
    if( TriceOutDepth() && triceTxDataRegisterEmpty() ){
        triceEnableTxEmptyInterrupt(); // next bytes
    }
}
#endif // #if defined( TRICE_UART ) && defined( TRICE_HALF_BUFFER_SIZE )
