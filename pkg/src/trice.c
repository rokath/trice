/*! \file trice.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"
#include "tcobs.h"
#include "xtea.h"

//! triceCommand is the command receive buffer.
char triceCommand[TRICE_COMMAND_SIZE_MAX+1]; // with terminating 0

//! triceCommandFlag ist set, when a command was received completely.
int triceCommandFlag = 0; // updated

static unsigned triceDepthMax = 0; //!< triceDepthMax is a diagnostics value usable to optimize buffer size.

#if TRICE_CYCLE_COUNTER == 1
uint8_t  TriceCycle = 0xc0; //!< TriceCycle is increased and transmitted with each trice message, if enabled.
#endif

#ifdef TRICE_HALF_BUFFER_SIZE
static uint32_t triceBuffer[2][TRICE_HALF_BUFFER_SIZE>>2] = {0}; //!< triceBuffer is a double buffer for better write speed.
static int triceSwap = 0; //!< triceSwap is the index of the active write buffer. !triceSwap is the active read buffer index.
    uint32_t* TriceBufferWritePosition = &triceBuffer[0][TRICE_DATA_OFFSET>>2]; //!< TriceBufferWritePosition is the active write position.
static uint32_t* triceBufferWriteLimit = &triceBuffer[1][TRICE_DATA_OFFSET>>2]; //!< triceBufferWriteLimit is the triceBuffer written limit. 

#if defined( TRICE_UART ) && defined( TRICE_HALF_BUFFER_SIZE ) // buffered out to UART
static size_t triceNonBlockingWrite( void const * buf, size_t nByte );
#define TRICE_WRITE( buf, len ) do{ triceNonBlockingWrite( buf, len ); }while(0) //lint !e534
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
    if( 0 == TriceOutDepth() ){ // transmission done, so a swap is possible
        uint32_t* tb = triceBufferSwap(); 
        size_t tLen = triceDepth(tb); // tlen is always a multiple of 4
        if( tLen ){
            TriceOut( tb, tLen );
        }
    } // else: transmission not done yet
}

//! TriceDepthMax returns the max trice buffer depth until now.
size_t TriceDepthMax( void ){
    size_t currentDepth = (size_t)(4*(TriceBufferWritePosition - &triceBuffer[triceSwap][0])); 
    return currentDepth > triceDepthMax ? currentDepth : triceDepthMax;
}

#else // #ifdef TRICE_HALF_BUFFER_SIZE

//! TriceDepthMax returns the max trice buffer depth until now.
size_t TriceDepthMax( void ){
    return triceDepthMax;
}

#endif // #else #ifdef TRICE_HALF_BUFFER_SIZE

#if TRICE_MODE == TRICE_STREAM_BUFFER
static uint32_t triceStreamBufferHeap[TRICE_STREAM_BUFFER_SIZE>>2] = {0}; //!< triceStreamBufferHeap is a kind of heap for trice messages.
       uint32_t* TriceBufferWritePosition = triceStreamBufferHeap; //!< TriceBufferWritePosition is the active write position.
static uint32_t* triceBufferWriteLimit  =  &triceStreamBufferHeap[TRICE_STREAM_BUFFER_SIZE>>2]; //!< triceBufferWriteLimit is the triceBuffer written limit. 


//! TriceStreamBufferSpace returns the space until buffer end.
uint32_t* TriceNextStreamBuffer( void ){
    if( TriceBufferWritePosition > triceBufferWriteLimit ){
        for(;;); // buffer overflow
    }
    if( triceBufferWriteLimit - TriceBufferWritePosition > TRICE_SINGLE_MAX_SIZE ){
        return TriceBufferWritePosition;
    }else{
        return triceStreamBufferHeap;
    }
}

void TriceAddressPush( uint32_t* ta ){
}

uint32_t* TriceAddressPop( void ){
    return triceStreamBufferHeap;
}

//! TriceTransfer, if possible, initiates a write.
//! It is the resposibility of the app to call this function.
void TriceTransfer( void ){
    if( 0 == TriceOutDepth() ){ // transmission done, so a new is possible
        uint32_t* tb = TriceAddressPop(); 
        size_t tLen = triceDepth(tb); // tlen is always a multiple of 4
        if( tLen ){
            TriceOut( tb, tLen );
        }
    } // else: transmission not done yet
}


#endif // #if TRICE_MODE == TRICE_STREAM_BUFFER

//! triceDataLen returns encoded len.
//! \param p points to nc
// *da = 11iiiiiiI 11iiiiiiI TT TT NC ...
// *da = 10iiiiiiI TT NC ...
// *da = 01iiiiiiI 11iiiiiiI NC ...
// *da = 00...
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
    int triceType = *pNC >> 14; // todo: this will not work with extended trices only 1 byte long
    unsigned offset = 0;
    size_t size = *pSize;
    size_t triceSize;
    size_t len; 
    *pStart = *buf;
    switch( triceType ){
        default:
        case 0: // EX
            len = size; // todo: Change that when needed.
            // Extended trices without length information cannot be separated here.
            // But it is possible to store them with length information and to remove it here.
            break;
        case 1: // NOTS
            len = 4 + triceDataLen(*pStart + 2); // tyId
            break;
        case 2: // TS16
            *pStart += 2; // see Id(n) macro definition
            offset = 2;
            len = 6 + triceDataLen(*pStart + 4); // tyId ts16
            break;
        case 3: // TS32
            len = 8 + triceDataLen(*pStart + 6); // tyId ts32
            break;
    }
    triceSize = (len + offset + 3) & ~3;
    // T2 case example:             triceSize  len   t-0-3   t-o
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
    TRICE_WRITE( enc, encLen ); //lint !e534
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

#if defined( TRICE_UART ) && defined( TRICE_HALF_BUFFER_SIZE ) // buffered out to UART
static uint8_t const * triceOutBuffer;
static size_t triceOutCount = 0;
static unsigned triceOutIndex = 0;

//! triceNonBlockingWrite
static size_t triceNonBlockingWrite( void const * buf, size_t nByte ){
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
