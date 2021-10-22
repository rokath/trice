/*! \file trice.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include <stdint.h>
#include <string.h> // strlen
#include "trice.h"

uint16_t TriceDepthMax = 0; //!< TriceDepthMax is a diagnostics value.
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
#define PREPARE_MULTI
//! TriceSingleToCOBS converts an in t stored trice into a cobs sequence into c with appended 0 as delimiter.
//! For performance reasons t is manipulated afterwards.
unsigned TriceSingleToCOBS( uint8_t* co, uint8_t * tr ){
    unsigned tlen, clen;
#ifdef PREPARE_MULTI
    // keep length and cycle endianness independent
    uint32_t* Tr =(uint32_t*)tr;
    uint32_t head = *Tr;
    tlen = sizeof(uint32_t) * (uint8_t)(head>>8) + sizeof(head);
    clen = COBSEncode(co, tr, tlen);
#else
    #if TRICE_CYCLE_COUNTER == 1 // with cycle counter
        tlen = tr[1] + 3; // little endian, add id size and cycle size
        tr[1] = tr[0]; // write cycle to 2nd position (little endian assumed!)
        clen = COBSEncode(co, &tr[1], tlen);
    #else // no cycle counter used
        tlen = tr[1] + 2; // little endian, add id size
        clen = COBSEncode(co, &tr[2], tlen);
    #endif
#endif
    co[clen] = 0; // add 0-delimiter
    return clen + 1;
}

#else // #if TRICE_HARDWARE_ENDIANNESS == TRICE_LITTLE_ENDIANNESS
    #error "todo: TRICE_HARDWARE_ENDIANNESS == TRICE_BIG_ENDIANNESS"
#endif // #else // #if TRICE_HARDWARE_ENDIANNESS == TRICE_LITTLE_ENDIANNESS

#if 100 <= TRICE_MODE && TRICE_MODE < 200
#ifndef NULL
#define NULL (void*)0
#endif
static uint32_t triceBuffer[2][(TRICE_BUFFER_SIZE+3)>>3] = {0}; //!< triceBuffer is double buffer for better write speed.
#define TRICE_ACTIVE 0 //!< TRICE_ACTIVE is the init value for swap.
static int swap = TRICE_ACTIVE; //!< swap is the active write buffer. !swap is the active read buffer.
uint32_t* wTb = &triceBuffer[TRICE_ACTIVE][0]; //!< wTb is the active write position.

static uint32_t* rTb = &triceBuffer[!TRICE_ACTIVE][0]; //!< rTb is the active read position.

//! triceRead returns a pointer to next complete trice message or it returns NULL if no data to process.
//! If in a first try the read buffer is empty, a buffer swap is done:
//! \li Switch next write to this buffer here, because it is empty.
//! \li Switch next read to the other buffer, may be there is stuff to read.
//! If both buffers empty, each triceRead call results in a buffer swap, what is ok.
//! There is no wTp overflow check! The read buffer must be read out fast enough to be swapped before the write buffer can overflow.
static uint8_t* triceRead( void ){
    uint8_t* p;
    uint16_t triceDepth = sizeof(uint32_t) * (wTb - &triceBuffer[swap][0]);  // diagnostics
    TriceDepthMax = triceDepth < TriceDepthMax ? TriceDepthMax : triceDepth; // diagnostics
    if( 0 == *rTb ){ // This buffer is empty
        TRICE_ENTER_CRITICAL_SECTION
        *wTb = 0; // write end marker
        swap = !swap;
        wTb = &triceBuffer[swap][0];
        rTb = &triceBuffer[!swap][0];
        TRICE_LEAVE_CRITICAL_SECTION
        if( 0 == *rTb ){ // This buffer is empty
            return NULL;
        }
    }
    p = (uint8_t*)rTb;
    rTb += 1 + p[1]; // step to next entry (little endian)
    return p;
}
#endif

#if TRICE_MODE == 200
static uint32_t triceBuffer[2][(TRICE_BUFFER_SIZE+3)>>3] = {0}; //!< triceBuffer is double buffer for better write speed.
#define TRICE_ACTIVE 0 //!< TRICE_ACTIVE is the init value for swap.
static int swap = TRICE_ACTIVE; //!< swap is the active write buffer. !swap is the active read buffer.
#define TRICE_DATA_OFFSET 8 //! Free bytes in front of stored trice bytes, must be a multiple of 4
uint32_t* wTb = &triceBuffer[TRICE_ACTIVE][TRICE_DATA_OFFSET>>2]; //!< wTb is the active write position.
static uint32_t* rTb; //!< rTb is the active read position.
#endif

uint8_t co2[TRICE_BUFFER_SIZE]; // to do

#if defined(RTT_WRITE) && defined(TRICE_MAX_SIZE)
void TriceSingleReadAndRTTWrite( void ){
    uint8_t* t;
    uint16_t clen; // uint8_t clen, tlen;
    //if( triceU8FifoDepth() ){
    //    return; // transmission not done yet
    //}
    t = triceRead();
    if( NULL == t ){
        return; // no trice data to transmit
    }
    #ifdef ENCRYPT
    triceServeFifoEncryptedToBytesBuffer(); // To do: rework obsolete code
    #endif
    clen = TriceSingleToCOBS( co2, t );
    RTT_WRITE( co2, clen ); 
}
#endif

#if TRICE_MODE == 200 // try out idea
//  //! TriceNonBlockingWrite
//  size_t TriceNonBlockingWrite( const void* buf, size_t nByte ){
//      // to do
//      return nByte;
//  }
//  
//  #define TRICE_WRITE( buf, len ) do{ \
//      size_t cnt = (size_t)(len); \
//      TriceTransferBusy = 1; do{ \
//      cnt = TriceNonBlockingWrite( buf, cnt ); }while(cnt); \
//      } while(0)


//! TriceTransferBusy is set before TriceNonBlockingWrite is called and needs to be cleared somehow, when last byte was written.
int TriceTransferBusy = 0;

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
size_t TriceTransferDepth( uint32_t* tb ){
    size_t triceDepth = (rTb - tb)<<2;
    TriceDepthMax = triceDepth < TriceDepthMax ? TriceDepthMax : triceDepth; // diagnostics
    return triceDepth - TRICE_DATA_OFFSET;
}

//! TriceMultiReadAndRTTWrite swaps the double buffer and initiates a write if possible.
void TriceMultiReadAndRTTWrite( void ){
    if( TriceTransferBusy ){
        return;
    }
    {
        uint32_t* tb = TriceBufferSwap(); 
        size_t tlen = TriceTransferDepth(tb);
        if( tlen ){
            uint8_t* co = (uint8_t*)tb;
            uint8_t* tr = co + TRICE_DATA_OFFSET;
            size_t clen = COBSEncode(co, tr, tlen);
            co[clen] = 0;
            TRICE_WRITE( co, clen+1 );
        }
    }
}
#endif

#ifdef TRICE_FIFO_BYTE_SIZE

void TriceSingleReadAndTranslate( void ){
    uint8_t* t;
    uint16_t clen; // uint8_t clen, tlen;
    if( triceU8FifoDepth() ){
        return; // transmission not done yet
    }
    t = triceRead();
    if( NULL == t ){
        return; // no trice data to transmit
    }
    #ifdef ENCRYPT
    triceServeFifoEncryptedToBytesBuffer(); // To do: rework obsolete code
    #endif
    clen = TriceSingleToCOBS( triceU8Fifo, t );
    TRICE_ENTER_CRITICAL_SECTION
    triceU8FifoWriteIndex = clen;
    triceU8FifoReadIndex = 0;
    TRICE_LEAVE_CRITICAL_SECTION
}




//! trice fifo instance, here are the trices buffered.
ALIGN4 uint32_t
triceU32Fifo[ TRICE_FIFO_BYTE_SIZE>>2 ]
ALIGN4_END;

uint8_t* const triceU8Fifo = (uint8_t*)triceU32Fifo;

//int triceU32FifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
//int triceU32FifoReadIndex = 0; //!< trice fifo read index for 32 bit values

unsigned triceU8FifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
unsigned triceU8FifoReadIndex = 0; //!< trice fifo read index

int triceFifoMaxDepth = 0; //!< diagnostics
/*
//! triceU32FifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int32 access for push and pop.
//! \return count of buffered bytes
int triceU32FifoDepth(void) {
    int triceCount = (triceU32FifoWriteIndex - triceU32FifoReadIndex) & TRICE_U32_FIFO_MASK;
    int depth = triceCount*sizeof(uint32_t);
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}
*/

//! triceU8UsageFifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int8 access for push and pop.
//! \return count of buffered bytes
int triceU8FifoDepth(void) {
    int depth = (triceU8FifoWriteIndex - triceU8FifoReadIndex) & TRICE_U8_FIFO_MASK;
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}

//  //! triceU32WriteU8ReadFifoDepth determines bytes count inside trice fifo.
//  //! Assumption: Only int32 for push and only int8 for pop.
//  //! \return count of buffered bytes
//  int triceU32WriteU8ReadFifoDepth(void) {
//      int depth = ((triceU32FifoWriteIndex<<2) - triceU8FifoReadIndex) & TRICE_U8_FIFO_MASK;
//      triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
//      return depth;
//  }

#endif // #ifdef TRICE_FIFO_BYTE_SIZE


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
