/*! \file trice.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"

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
static int triceNonBlockingWrite( void const * buf, int nByte );
#define TRICE_WRITE( buf, len ) do{ triceNonBlockingWrite( buf, len ); }while(0)
#endif

//! triceBufferSwap swaps the trice double buffer and returns the read buffer address.
static uint32_t* triceBufferSwap( void ){
    TRICE_ENTER_CRITICAL_SECTION
    triceBufferWriteLimit = TriceBufferWritePosition; // keep end position
    triceSwap = !triceSwap; // exchange the 2 buffers
    TriceBufferWritePosition = &triceBuffer[triceSwap][TRICE_DATA_OFFSET>>2]; // set write position for next TRICE
    TRICE_LEAVE_CRITICAL_SECTION
    return &triceBuffer[!triceSwap][0];
}

//! triceDepth returns the total trice byte count ready for transfer.
//! The trice data start at tb + TRICE_DATA_OFFSET.
//! The returned depth is without the TRICE_DATA_OFFSET offset.
static size_t triceDepth( uint32_t* tb ){
    size_t depth = (triceBufferWriteLimit - tb)<<2; // 32-bit write width
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
    size_t currentDepth = 4*(TriceBufferWritePosition - &triceBuffer[triceSwap][0]); 
    return currentDepth > triceDepthMax ? currentDepth : triceDepthMax;
}

#else // #ifdef TRICE_HALF_BUFFER_SIZE

//! TriceDepthMax returns the max trice buffer depth until now.
size_t TriceDepthMax( void ){
    return triceDepthMax;
}

#endif // #else #ifdef TRICE_HALF_BUFFER_SIZE

//! triceDataLen returns encoded len.
//! \param p points to nc
// *da = 11iiiiiiI 11iiiiiiI TT TT NC ...
// *da = 10iiiiiiI TT NC ...
// *da = 01iiiiiiI 11iiiiiiI NC ...
// *da = 00...
static size_t triceDataLen( uint8_t* p ){
    uint16_t nc = *(uint16_t*)p;
    size_t n = nc>>8;
    if( n < 128 ){
        return n;
    }
    return nc & 0x7fff;
}

unsigned triceErrorCount = 0;

//! nextTrice expects at buf 32-bit aligned trice messages and returns the next one in pStart and pLen.
//! *buf is filled with the advanced buf and *len gets the reduced len.
//! \retval is 0 on success or negative on error.
static int nextTrice( uint8_t** buf, size_t* pSize, uint8_t** pStart, size_t* pLen ){
    uint16_t* pNC = (uint16_t*)*buf;
    int triceType = *pNC >> 14;
    int offset = 0;
    size_t size = *pSize;
    size_t triceSize;
    size_t len; 
    *pStart = *buf;
    switch( triceType ){
        case 0: // EX
            len = size; // todo: Change that when needed.
            // Extended trices without length information cannot be separated here.
            // But it is possible to store them with length information and to remove it here.
            break;
        case 1: // T0
            len = 4 + triceDataLen(*pStart + 2); // tyId
            break;
        case 2: // T2
            *pStart += 2; // see Id(n) macro definition
            offset = 2;
            len = 6 + triceDataLen(*pStart + 4); // tyId ts16
            break;
        case 3: // T4
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
    if( !( triceSize - offset - 3 <= len && len <= triceSize - offset )){ // corrupt data
        triceErrorCount++;
        return -__LINE__;
    }    
    size -= triceSize;
    *buf += triceSize;
    *pSize = size;
    *pLen = len;
    return 0;
}

static size_t triceEncode( uint8_t* enc, uint8_t* buf, size_t len ){
    size_t encLen;
    #ifdef TRICE_ENCRYPT
    len = (len + 7) & ~7; // only multiple of 8 encryptable
    TriceEncrypt( enc + TRICE_DATA_OFFSET, len>>2 );
    #endif
    encLen = TriceCOBSEncode(enc, buf, len);
    enc[encLen++] = 0; // Add zero as package delimiter.
    return encLen;
}

//! TriceOutMultiSafeMode separately encodes multiple trice, each in one package, and writes them in one step to the output.
//! \param tb is start of uint32_t* trice buffer. The space TRICE_DATA_OFFSET at
//! the tb start is for in-buffer encoding of the trice data.
//! \param tLen is length of trice data. tlen is always a multiple of 4 because
//! of 32-bit alignment and padding bytes.
void TriceOutMultiSafeMode( uint32_t* tb, size_t tLen ){
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
        encLen += triceEncode( enc+encLen, triceStart, triceLen );
    }
    TRICE_WRITE( enc, encLen );
}

//! TriceOutMultiSafeMode separately encodes multiple trice, each in one package, and writes them in one step to the output.
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
    TRICE_WRITE( enc, encLen );
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
static int triceOutCount = 0;
static int triceOutIndex = 0;

//! triceNonBlockingWrite
static int triceNonBlockingWrite( void const * buf, int nByte ){
    triceOutBuffer = buf;
    triceOutIndex = 0;
    triceOutCount = nByte;
    return nByte;
}

//! TriceOutDepth returns the amount of bytes not written yet.
int TriceOutDepth( void ){
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

//! TriceCOBSEncode stuffs "length" bytes of data at the location pointed to by "input"
//! and writes the output to the location pointed to by "output".
//! Returns the number of bytes written to "output".
//! Remove the "restrict" qualifiers if compiling with a pre-C99 C dialect.
//! (copied and adapted from https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing/blob/master/cobs.c)
unsigned TriceCOBSEncode( uint8_t* restrict output, const uint8_t * restrict input, unsigned length){
    unsigned read_index = 0;
    unsigned write_index = 1;
    unsigned code_index = 0;
    uint8_t code = 1;
    while(read_index < length)
    {
        if(input[read_index] == 0)
        {
            output[code_index] = code;
            code = 1;
            code_index = write_index++;
            read_index++;
        }
        else
        {
            output[write_index++] = input[read_index++];
            code++;
            if(code == 0xFF)
            {
                output[code_index] = code;
                code = 1;
                code_index = write_index++;
            }
        }
    }
    output[code_index] = code;
    return write_index;
}

#ifdef TRICE_ENCRYPT // needs a re-design
//! golang XTEA works with 64 rounds
static const unsigned int numRounds = 64;

//! 128 bit static key
static const uint32_t k[4] = TRICE_ENCRYPT; 

//! internal constant
static const uint32_t delta = 0x9E3779B9;

//! precomputed values for faster execution
static uint32_t table[64];

//! TriceInitXteaTable precalculates the table.
//! It is possible to put this table completely into FLASH by precomputing it during compile time.
void TriceInitXteaTable( void ){
    uint32_t sum = 0;
    // Two rounds of XTEA applied per loop
    for( int i = 0; i < numRounds; ) {
        table[i] = sum + k[sum&3];
        i++;
        sum += delta;
        table[i] = sum + k[(sum>>11)&3];
        i++;
    }
}

// encipher converts 64 bits.
//! Code taken and adapted from xtea\block.go
//!\param v 64 bits of data in v[0] and v[1] are encoded in place
static void encipher( uint32_t v[2] ) {
    uint32_t v0 = v[0], v1 = v[1];
    for( int i=0; i < numRounds; ) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ table[i];
        i++;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ table[i];
        i++;
    }
    v[0] = v0; v[1] = v1;
}

#ifdef TRICE_DECRYPT
//! decipher reverses encipher action.
//! Code taken and adapted from xtea\block.go
//!\param v 64 bits of data in v[0] and v[1] are decoded in place
static void decipher( uint32_t v[2] ) {
    uint32_t v0 = v[0], v1 = v[1];
    for( int i=numRounds; i > 0; ) {
        i--;
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ table[i];
        i--;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ table[i];
    }
    v[0] = v0; v[1] = v1;
}


//! TriceDecrypt re-converts from xtea cipher.
//! \param p pointer to 8 byte buffer
void TriceDecrypt( uint32_t* p, unsigned count ){
    for( int i = 0; i < count; i +=2 ){
        decipher( &p[i]); // byte swapping is done inside trice tool according to endianness.
    }
}
#endif // #ifdef TRICE_DECRYPT

//! TriceEncrypt converts to xtea cipher.
//! \param p pointer to 8 byte buffer
void TriceEncrypt( uint32_t* p, unsigned count ){
    for( int i = 0; i < count; i +=2 ){
        encipher( &p[i] ); // byte triceSwap is done inside trice tool
    }
}

#endif // #ifdef TRICE_ENCRYPT
