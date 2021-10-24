/*! \file trice.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include <stdint.h>
#include <string.h> // strlen
#include "trice.h"

#define TRICE_DATA_OFFSET 8 //! TRICE_DATA_OFFSET is the space in front of trice data for in-buffer COBS encoding.
uint16_t TriceDepthMax = 0; //!< TriceDepthMax is a diagnostics value usable to optimize buffer size.
uint8_t  TriceCycle = 0xc0; //!< trice cycle counter

// https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing/blob/master/cobs.c
/* Stuffs "length" bytes of data at the location pointed to by
 * "input", writing the output to the location pointed to by
 * "output". Returns the number of bytes written to "output".
 *
 * Remove the "restrict" qualifiers if compiling with a
 * pre-C99 C dialect.
 */
//           size_t cobs_encode(const uint8_t * /*restrict*/ input, size_t length, uint8_t * /*restrict*/ output)
TRICE_INLINE unsigned COBSEncode( uint8_t* restrict output, const uint8_t * restrict input, unsigned length){
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

// The trice data stream is little endian, as most embedded MCUs use litte endian format.
// For big endian embedded MCUs the trice tool has an -targetEndianess switch to avoid transcoding in the target processor.
// The buffer tail contains wholes on big endian MCUs for TRICE8_1, TRICE8_2, TRICE8_3, TRICE8_5, TRICE8_6, TRICE8_7, TRICE16_1, TRICE16_3 and needs adjustment then.
#if TRICE_HARDWARE_ENDIANNESS == TRICE_LITTLE_ENDIANNESS
//! TriceSingleToCOBS converts an in t stored trice into a cobs sequence into c with appended 0 as delimiter.
//! For performance reasons t is manipulated afterwards.
unsigned TriceSingleToCOBS( uint8_t* co, uint8_t * tr ){
    unsigned tlen, clen;
    uint32_t* Tr =(uint32_t*)tr;
    uint32_t head = *Tr;
    tlen = sizeof(uint32_t) * (uint8_t)(head>>8) + sizeof(head);
    clen = COBSEncode(co, tr, tlen);
    co[clen] = 0; // add 0-delimiter
    return clen + 1;
}
#else // #if TRICE_HARDWARE_ENDIANNESS == TRICE_LITTLE_ENDIANNESS
    #error "todo: TRICE_HARDWARE_ENDIANNESS == TRICE_BIG_ENDIANNESS"
#endif // #else // #if TRICE_HARDWARE_ENDIANNESS == TRICE_LITTLE_ENDIANNESS


#ifdef TRICE_DOUBLE_BUFFER_SIZE
static uint32_t triceBuffer[2][(TRICE_DOUBLE_BUFFER_SIZE+3)>>3] = {0}; //!< triceBuffer is double buffer for better write speed.
static int swap = 0; //!< swap is the index of the active write buffer. !swap is the active read buffer.
uint32_t* wTb = &triceBuffer[0][0]; //!< wTb is the active write position.
static uint32_t* rTb = &triceBuffer[1][0]; //!< rTb is the active read position. 

//! triceRead returns a pointer to next complete trice message or it returns NULL if no data to process.
//! If in a first try the read buffer is empty, a buffer swap is done:
//! \li Switch next write to this buffer here, because it is empty.
//! \li Switch next read to the other buffer, may be there is stuff to read.
//! If both buffers empty, each triceRead call results in a buffer swap, what is ok.
//! There is no wTp overflow check! The read buffer must be read out fast enough to be swapped before the write buffer can overflow.
static uint8_t* triceRead( void ){
    uint32_t head;
    int triceU32cnt;
    uint8_t* p;
    uint16_t triceDepth = sizeof(uint32_t) * (wTb - &triceBuffer[swap][0]);  // diagnostics
    TriceDepthMax = triceDepth < TriceDepthMax ? TriceDepthMax : triceDepth; // diagnostics
    if( 0 == *rTb ){ // This buffer is empty
        TRICE_ENTER_CRITICAL_SECTION
        *wTb = 0; // write end marker
        swap = !swap; // From section 6.5.3.3/5 of the ISO C99 standard: The result of the logical negation operator ! is 0 if the value of its operand compares unequal to 0, 1 if the value of its operand compares equal to 0.
        wTb = &triceBuffer[swap][TRICE_DATA_OFFSET];
        rTb = &triceBuffer[!swap][TRICE_DATA_OFFSET];
        TRICE_LEAVE_CRITICAL_SECTION
        if( 0 == *rTb ){ // This buffer is empty
            return (void*)0;
        }
    }
    p = (uint8_t*)rTb;
    head = *rTb;
    triceU32cnt = ((0x0000FF00 & head) >> 8 ) + 1;
    rTb += triceU32cnt; // step to next entry (little endian)
    return p;
}

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
    TriceDepthMax = triceDepth < TriceDepthMax ? TriceDepthMax : triceDepth; // diagnostics
    return triceDepth - TRICE_DATA_OFFSET;
}

//! TriceMultiReadAndRTTWrite swaps the double buffer and initiates a write if possible.
void TriceMultiReadAndWrite( void ){
    if( 0 == TriceWriteDepth() ){ // transmission done
        uint32_t* tb = TriceBufferSwap(); 
        size_t tlen = TriceTransferDepth(tb);
        if( tlen ){
            uint8_t* co = (uint8_t*)tb;
            uint8_t* tr = co + TRICE_DATA_OFFSET;
            size_t clen = COBSEncode(co, tr, tlen);
            co[clen] = 0;
            TRICE_WRITE( co, clen+1 );
        }
    } // else: transmission not done yet
}

void TriceSingleReadAndWrite( void ){
    uint8_t *co, *t;
    uint16_t clen; // uint8_t clen, tlen;
    if( TriceWriteDepth() ){
        return; // transmission not done yet
    }
    t = triceRead();
    if( NULL == t ){
        return; // no trice data to transmit
    }
    #ifdef ENCRYPT
    triceServeFifoEncryptedToBytesBuffer(); // To do: rework obsolete code
    #endif
    co = t-TRICE_DATA_OFFSET; 
    clen = TriceSingleToCOBS( co, t );
    TRICE_WRITE( co, clen );
}

#endif // #ifdef TRICE_DOUBLE_BUFFER_SIZE

#ifdef TRICE_UART
static uint8_t const * triceWriteOutBuffer;
static int triceWriteOutCount = 0;
static int triceWriteOutIndex = 0;

//! TriceNonBlockingWrite
int TriceNonBlockingWrite( void const * buf, int nByte ){
    triceWriteOutIndex = 0;
    triceWriteOutBuffer = buf;
    triceWriteOutCount = nByte;
    return nByte;
}

int TriceWriteDepth( void ){
    return triceWriteOutCount - triceWriteOutIndex;
}

uint8_t TriceNextUint8( void ){
    return triceWriteOutBuffer[triceWriteOutIndex++];
}

#endif

#ifdef ENCRYPT
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







//  //! triceCOBSREncode does the same as the cobsr_encode function but a bit faster.
//  //! by leaving out some checks, assuming max 254 source bytes and uses a simpler signature.
//  //! See https://github.com/ctrl-labs/cobs-c for more details.
//  //! \param dst is the result buffer. It must be at least 1 byte longer than len.
//  //! \param src is the source buffer.
//  //! \param len is count of valid data inside the the source buffer. Assumption: 0 < len < 255.
//  //! \retval is the count of valid data inside the the result buffer. It is len or len+1.
//  static uint8_t triceCOBSREncode(uint8_t *dst, const uint8_t * src, uint8_t len){
//      const uint8_t* limit = src + len; // end of source 
//      uint8_t*       code  = dst;       // next code position
//      uint8_t*       data  = dst + 1;   // next data position
//      uint8_t        sl    = 1;         // search length
//      uint8_t        by;                // byte
//      do{ // Iterate over the source bytes
//          by = *src++;
//          if( by == 0 ) { // We found a zero byte
//              *code = sl;
//              code = data++;
//              sl = 1;
//          } else { // Copy the non-zero byte to the destination buffer
//              *data++ = by;
//              sl++;
//          }
//      }while( src < limit );
//      // We've reached the end of the source data (or possibly run out of output buffer)
//      // Finalise the remaining output. In particular, write the code (length) byte.
//      //
//      // For COBS/R, the final code (length) byte is special: if the final data byte is
//      // greater than or equal to what would normally be the final code (length) byte,
//      // then replace the final code byte with the final data byte, and remove the final
//      // data byte from the end of the sequence. This saves one byte in the output.
//      //
//      // Update the pointer to calculate the final output length.
//      if( by < sl ){ // Encoding same as plain COBS 
//          *code = sl;
//      } else { // Special COBS/R encoding: length code is final byte, and final byte is removed from data sequence.
//          *code = by;
//          return sl; // data--;
//      }
//      return len+1; // data - dst; // Calculate the output length, from the value of code
//  }

//  #if TRICE_HARDWARE_ENDIANNESS == TRICE_TRANSFER_ENDIANNESS
//  #define TRICE_HTONS(n) ((uint16_t)(n))
//  #define TRICE_HTON(n)  ((uint32_t)(n))
//  #else
//  #define TRICE_HTONS(n) ( (((uint16_t)(n))>>8) | (((uint16_t)(n))<<8) )
//  #define TRICE_HH(n)                     ((uint32_t)(n)>>24)
//  #define TRICE_HL(n) ((uint32_t)(uint8_t)((uint32_t)(n)>>16))
//  #define TRICE_LH(n) ((uint32_t)         ((uint16_t)(n)>> 8))
//  #define TRICE_LL(n) ((uint32_t)         (( uint8_t)(n)    ))
//  #define TRICE_HTON(n) ((TRICE_LL(n)<<24)|(TRICE_LH(n)<<16)|(TRICE_HL(n)<<8)|TRICE_HH(n) )
//  #endif
