/*! \file trice.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"
#define TRICE_FILE Id(56030)

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

//! TriceOut converts trice data and transmits them to the output.
//! \param tb is start of uint32_t* trice buffer. The space TRICE_DATA_OFFSET>>2
//! at the tb start is for in-buffer COBS encoding and the
//! TRICE_COBS_PACKAGE_MODE in front of the trice data.
//! \param tLen is length of trice data. tlen is always a multiple of 4 and counts after TRICE_COBS_PACKAGE_MODE.
void TriceOut( uint32_t* tb, size_t tLen ){
    size_t eLen, cLen;
    uint8_t* co = (uint8_t*)tb; // encoded COBS data starting address
    uint32_t* da = tb + (TRICE_DATA_OFFSET>>2)-1; // start of unencoded COBS package data: descriptor and trice data
    *da = TRICE_COBS_PACKAGE_MODE; // add a 32-bit COBS package mode descriptor in front of trice data. That allowes to inject third-party non-trice COBS packages.
    eLen = tLen + 4; // add COBS package mode descriptor length 
    #ifdef TRICE_ENCRYPT
    eLen = (eLen + 4) & ~7; // only multiple of 8 encryptable
    TriceEncrypt( da, eLen>>2 );
    #endif
    cLen = TriceCOBSEncode(co, (uint8_t*)da, eLen);
    do{                 // Add 1 to 4 zeroes as COBS package delimiter.
        co[cLen++] = 0; // One is ok, but padding to an uint32_t border could make TRICE_WRITE faster.
    }while( cLen & 3 ); // Additional empty packages are ignored on th receiver side.
    TRICE_WRITE( co, cLen );
    tLen += TRICE_DATA_OFFSET; 
    triceDepthMax = tLen < triceDepthMax ? triceDepthMax : tLen; // diagnostics
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
