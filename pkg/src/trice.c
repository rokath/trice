/*! \file trice.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"

unsigned TriceDepthMax = 0; //!< TriceDepthMax is a diagnostics value usable to optimize buffer size.

#if TRICE_CYCLE_COUNTER == 1
uint8_t  TriceCycle = 0xc0; //!< TriceCycle is increased and transmitted with each trice message, if enabled.
#endif

#ifdef TRICE_HALF_BUFFER_SIZE
static uint32_t triceBuffer[2][(TRICE_HALF_BUFFER_SIZE+TRICE_DATA_OFFSET)>>2] = {0}; //!< triceBuffer is double buffer for better write speed.
static int swap = 0; //!< swap is the index of the active write buffer. !swap is the active read buffer index.
uint32_t* wTb = &triceBuffer[0][TRICE_DATA_OFFSET>>2]; //!< wTb is the active write position.
static uint32_t* rTb = &triceBuffer[1][0]; //!< rTb is the active read position. 

//! TriceBufferSwap swaps the trice double buffer and returns the transfer buffer address.
uint32_t* TriceBufferSwap( void ){
    TRICE_ENTER_CRITICAL_SECTION
    *wTb = 0; // write end marker
    rTb = wTb; // keep end position
    swap = !swap;
    wTb = &triceBuffer[swap][TRICE_DATA_OFFSET>>2]; // set write position for next TRICE
    TRICE_LEAVE_CRITICAL_SECTION
    return &triceBuffer[!swap][0];
}

//! TriceTransferDepth returns the total trice byte count ready for transfer.
//! The trice data start at tp + TRICE_DATA_OFFSET.
//! The returned depth is without the TRICE_DATA_OFFSET offset.
static size_t TriceTransferDepth( uint32_t* tb ){
    size_t triceDepth = (rTb - tb)<<2;
    return triceDepth - TRICE_DATA_OFFSET;
}

//! TriceTransfer, if possible, swaps the double buffer and initiates a write.
void TriceTransfer( void ){
    if( 0 == TriceWriteOutDepth() ){ // transmission done
        uint32_t* tb = TriceBufferSwap(); 
        size_t tlen = TriceTransferDepth(tb);
        if( tlen ){
            uint8_t* co = (uint8_t*)tb;
            uint8_t* tr = co + TRICE_DATA_OFFSET;
            size_t clen = TriceCOBSEncode(co, tr, tlen);
            co[clen++] = 0;
            TRICE_WRITE( co, clen );
        }
    } // else: transmission not done yet
}
#endif // #ifdef TRICE_HALF_BUFFER_SIZE

#if defined( TRICE_UART ) && TRICE_MODE == 0
//! TriceBlockingPutChar returns after c was successfully written.
void TriceBlockingPutChar( uint8_t c ){
    while( !triceTxDataRegisterEmpty() );
    triceTransmitData8( c );
} 

//! TriceBlockingWrite returns after buf was complketely written.
void TriceBlockingWrite( uint8_t const * buf, int len ){
    for( unsigned i = 0; i < len; i++ ){ 
        TriceBlockingPutChar( buf[i] ); }
}
#endif // #if defined( TRICE_UART ) && TRICE_MODE == 0

#if defined( TRICE_UART ) && TRICE_MODE != 0
static uint8_t const * triceWriteOutBuffer;
static int triceWriteOutCount = 0;
static int triceWriteOutIndex = 0;

//! TriceNonBlockingWrite
int TriceNonBlockingWrite( void const * buf, int nByte ){
    triceWriteOutBuffer = buf;
    triceWriteOutIndex = 0;
    triceWriteOutCount = nByte;
    return nByte;
}

//! TriceWriteOutDepth returns the amount of bytes not written yet.
int TriceWriteOutDepth( void ){
    return triceWriteOutCount - triceWriteOutIndex;
}

//! TriceNextUint8 returns the next trice byte for transmission.
uint8_t TriceNextUint8( void ){
    return triceWriteOutBuffer[triceWriteOutIndex++];
}

//! triceServeTransmit as triceServeU8FifoTransmit must be called cyclically to proceed ongoing write out.
//! A good place is UART ISR.
void triceServeTransmit(void) {
    triceTransmitData8(TriceNextUint8());
    if (0 == TriceWriteOutDepth()) { // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}

// triceTriggerTransmit as triceTriggerU8FifoTransmit must be called cyclically to initialize write out.
void triceTriggerTransmit(void){
    if( TriceWriteOutDepth() && triceTxDataRegisterEmpty() ){
        triceEnableTxEmptyInterrupt(); // next bytes
    }
}
#endif // #if defined( TRICE_UART ) && TRICE_MODE != 0

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
    TriceDepthMax = length + TRICE_DATA_OFFSET < TriceDepthMax ? TriceDepthMax : length + TRICE_DATA_OFFSET; // diagnostics
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

#if 0 // def ENCRYPT // needs a re-design
//! golang XTEA works with 64 rounds
static const unsigned int numRounds = 64;

//! 128 bit static key
static const uint32_t k[4] = ENCRYPT; 

//! internal constant
static const uint32_t delta = 0x9E3779B9;

//! precomputed values for faster execution
static uint32_t table[64];

//! Precalculate the table
//! It is possible to put this table completely into FLASH by precomputing it during compile time
void InitXteaTable( void ){
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

//!code taken and adapted from xtea\block.go
//!\param v take 64 bits of data in v[0] and v[1]
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

#ifdef DECRYPT
//!code taken and adapted from xtea\block.go
//!\param v take 64 bits of data in v[0] and v[1]
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


//! re-convert from xtea cipher
//! \param p pointer to 8 byte buffer
void decrypt( uint8_t* p ){
    decipher( (uint32_t*)p ); // byte swap is done inside trice tool
}
#endif // #ifdef DECRYPT

//! convert to xtea cipher
//! \param p pointer to 8 byte buffer
void encrypt( uint8_t* p ){
    encipher( (uint32_t*)p ); // byte swap is done inside trice tool
}

uint8_t triceBytesBuffer[8]; //!< bytes transmit buffer
int const triceBytesBufferIndexLimit = sizeof(triceBytesBuffer);
int triceBytesBufferIndex = triceBytesBufferIndexLimit;

void triceServeFifoEncryptedToBytesBuffer(void) {
    if (triceBytesBufferIndexLimit == triceBytesBufferIndex) { // bytes buffer empty and tx finished
        // next trice
        int n = triceU32FifoDepth();
        if ( n >= 8 ) { // a trice to transmit
            *(uint32_t*)&triceBytesBuffer[0] = triceU32Pop();
            *(uint32_t*)&triceBytesBuffer[4] = triceU32Pop();
            encrypt( triceBytesBuffer );
            triceBytesBufferIndex = 0;
        }else if ( 4 == n ) {
            TRICE_SYNC; // avoid delay of a single last trice
        }
    }
}


#endif // #ifdef ENCRYPT

