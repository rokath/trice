/*! \file trice.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include <stdint.h>
#include <string.h> // strlen
#include "trice.h"

#if ((TRICE_ENCODING == TRICE_FLEX_ENCODING) || (TRICE_ENCODING == TRICE_COBSR_ENCODING))
uint8_t triceCycle = 0;
#endif

//! trice fifo instance, here are the trices buffered.
ALIGN4 uint32_t
triceU32Fifo[ TRICE_FIFO_BYTE_SIZE>>2 ]
ALIGN4_END;

uint8_t* const triceU8Fifo = (uint8_t*)triceU32Fifo;

int triceU32FifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
int triceU32FifoReadIndex = 0; //!< trice fifo read index for 32 bit values

int triceU8FifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
int triceU8FifoReadIndex = 0; //!< trice fifo read index

int triceFifoMaxDepth = 0; //!< diagnostics

//! triceU32FifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int32 access for push and pop.
//! \return count of buffered bytes
int triceU32FifoDepth(void) {
    int triceCount = (triceU32FifoWriteIndex - triceU32FifoReadIndex) & TRICE_U32_FIFO_MASK;
    int depth = triceCount*sizeof(uint32_t);
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}

//! triceU8UsageFifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int8 access for push and pop.
//! \return count of buffered bytes
int triceU8FifoDepth(void) {
    int depth = (triceU8FifoWriteIndex - triceU8FifoReadIndex) & TRICE_U8_FIFO_MASK;
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}

//! triceU32WriteU8ReadFifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int32 for push and only int8 for pop.
//! \return count of buffered bytes
int triceU32WriteU8ReadFifoDepth(void) {
    int depth = ((triceU32FifoWriteIndex<<2) - triceU8FifoReadIndex) & TRICE_U8_FIFO_MASK;
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}

#ifndef trice0i
//! trice0i does trace id unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
void trice0i( uint32_t id, char* pFmt ){
    TRICE0i( id, pFmt );
}
#endif

#ifndef trice0
//! trice0 does trace id protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
void trice0( uint32_t id, char* pFmt ){
    TRICE0( id, pFmt );
}
#endif

#ifndef trice8_1i
//! trice8_1i does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
void trice8_1i( uint32_t id, char* pFmt, int8_t d0 ){
    TRICE8_1i( id, pFmt, d0 );
}
#endif

#ifndef trice8_1
//! trice8_1 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
void trice8_1( uint32_t id, char* pFmt, int8_t d0 ){
    TRICE8_1( id, pFmt, d0 );
}
#endif

#ifndef trice8_2i
//! trice8_2i does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
void trice8_2i( uint32_t id, char* pFmt, int8_t d0, int8_t d1 ){
    TRICE8_2i( id, pFmt, d0, d1 );
}
#endif

#ifndef trice8_2
//! trice8_2 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
void trice8_2( uint32_t id, char* pFmt, int8_t d0, int8_t d1 ){
    TRICE8_2( id, pFmt, d0, d1 );
}
#endif

#ifndef trice8_3i
//! trice8_3i does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
void trice8_3i( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2 ){
    TRICE8_3i( id, pFmt, d0, d1, d2 );
}
#endif

#ifndef trice8_3
//! trice8_3 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
void trice8_3( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2 ){
    TRICE8_3( id, pFmt, d0, d1, d2 );
}
#endif

#ifndef trice8_4i
//! trice8_4i does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
void trice8_4i( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3 ){
    TRICE8_4i( id, pFmt, d0, d1, d2, d3 );
}
#endif

#ifndef trice8_4
//! trice8_4 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
void trice8_4( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3 ){
    TRICE8_4( id, pFmt, d0, d1, d2, d3 );
}
#endif

#ifndef trice8_5i
//! trice8_6i does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
void trice8_5i( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4 ){
    TRICE8_5i( id, pFmt, d0, d1, d2, d3, d4 );
}
#endif

#ifndef trice8_5
//! trice8_6 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
void trice8_5( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4 ){
    TRICE8_5( id, pFmt, d0, d1, d2, d3, d4 );
}
#endif


#ifndef trice8_6i
//! trice8_6i does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
void trice8_6i( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5 ){
    TRICE8_6i( id, pFmt, d0, d1, d2, d3, d4, d5 );
}
#endif

#ifndef trice8_6
//! trice8_6 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
void trice8_6( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5 ){
    TRICE8_6( id, pFmt, d0, d1, d2, d3, d4, d5 );
}
#endif

#ifndef trice8_7i
//! trice8_7i does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
void trice8_7i( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6 ){
    TRICE8_7i( id, pFmt, d0, d1, d2, d3, d4, d5, d6 );
}
#endif

#ifndef trice8_7
//! trice8_7 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
void trice8_7( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6 ){
    TRICE8_7( id, pFmt, d0, d1, d2, d3, d4, d5, d6 );
}
#endif

#ifndef trice8_8i
//! trice8_8i does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
void trice8_8i( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6, int8_t d7 ){
    TRICE8_8i( id, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 );
}
#endif

#ifndef trice8_8
//! trice8_8 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
void trice8_8( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6, int8_t d7 ){
    TRICE8_8( id, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 );
}
#endif


#ifndef trice16_1i
//! trice16_1i does trace id and 16-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
void trice16_1i( uint32_t id, char* pFmt, int16_t d0 ){
    TRICE16_1i( id, pFmt, d0 );
}
#endif

#ifndef trice16_1
//! trice16_1 does trace id and 16-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
void trice16_1( uint32_t id, char* pFmt, int16_t d0 ){
    TRICE16_1( id, pFmt, d0 );
}
#endif

#ifndef trice16_2i
//! trice16_2i does trace id and 16-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
void trice16_2i( uint32_t id, char* pFmt, int16_t d0, int16_t d1 ){
    TRICE16_2i( id, pFmt, d0, d1 );
}
#endif

#ifndef trice16_2
//! trice16_2 does trace id and 16-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
void trice16_2( uint32_t id, char* pFmt, int16_t d0, int16_t d1 ){
    TRICE16_2( id, pFmt, d0, d1 );
}
#endif

#ifndef trice16_3i
//! trice16_3i does trace id and 16-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
void trice16_3i( uint32_t id, char* pFmt, int16_t d0, int16_t d1, int16_t d2 ){
    TRICE16_3i( id, pFmt, d0, d1, d2 );
}
#endif

#ifndef trice16_3
//! trice16_3 does trace id and 16-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
void trice16_3( uint32_t id, char* pFmt, int16_t d0, int16_t d1, int16_t d2 ){
    TRICE16_3( id, pFmt, d0, d1, d2 );
}
#endif

#ifndef trice16_4i
//! trice16_4i does trace id and 16-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
void trice16_4i( uint32_t id, char* pFmt, int16_t d0, int16_t d1, int16_t d2, int16_t d3 ){
    TRICE16_4i( id, pFmt, d0, d1, d2, d3 );
}
#endif

#ifndef trice16_4
//! trice16_4 does trace id and 16-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
void trice16_4( uint32_t id, char* pFmt, int16_t d0, int16_t d1, int16_t d2, int16_t d3 ){
    TRICE16_4( id, pFmt, d0, d1, d2, d3 );
}
#endif

#ifndef trice32_1i
//! trice32_1i does trace id and 32-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
void trice32_1i( uint32_t id, char* pFmt, int32_t d0 ){
    TRICE32_1i( id, pFmt, d0 );
}
#endif

#ifndef trice32_1
//! trice32_1 does trace id and 32-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
void trice32_1( uint32_t id, char* pFmt, int32_t d0 ){
    TRICE32_1( id, pFmt, d0 );
}
#endif

#ifndef trice32_2i
//! trice32_2i does trace id and 32-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
void trice32_2i( uint32_t id, char* pFmt, int32_t d0, int32_t d1 ){
    TRICE32_2i( id, pFmt, d0, d1 );
}
#endif

#ifndef trice32_2
//! trice32_2 does trace id and 32-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
void trice32_2( uint32_t id, char* pFmt, int32_t d0, int32_t d1 ){
    TRICE32_2( id, pFmt, d0, d1 );
}
#endif

#ifndef trice32_3i
//! trice32_3i does trace id and 32-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
void trice32_3i( uint32_t id, char* pFmt, int32_t d0, int32_t d1, int32_t d2 ){
    TRICE32_3i( id, pFmt, d0, d1, d2 );
}
#endif

#ifndef trice32_3
//! trice32_3 does trace id and 32-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
void trice32_3( uint32_t id, char* pFmt, int32_t d0, int32_t d1, int32_t d2 ){
    TRICE32_3( id, pFmt, d0, d1, d2 );
}
#endif

#ifndef trice32_4i
//! trice32_4i does trace id and 32-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
void trice32_4i( uint32_t id, char* pFmt, int32_t d0, int32_t d1, int32_t d2, int32_t d3 ){
    TRICE32_4i( id, pFmt, d0, d1, d2, d3 );
}
#endif

#ifndef trice32_4
//! trice32_4 does trace id and 32-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
void trice32_4( uint32_t id, char* pFmt, int32_t d0, int32_t d1, int32_t d2, int32_t d3 ){
    TRICE32_4( id, pFmt, d0, d1, d2, d3 );
}
#endif

#ifndef trice64_1i
//! trice64_1i does trace id and 64-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
void trice64_1i( uint32_t id, char* pFmt, int64_t d0 ){
    TRICE64_1i( id, pFmt, d0 );
}
#endif

#ifndef trice64_1
//! trice64_1 does trace id and 64-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
void trice64_1( uint32_t id, char* pFmt, int64_t d0 ){
    TRICE64_1( id, pFmt, d0 );
}
#endif

#ifndef trice64_2i
//! trice64_2i does trace id and 64-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
void trice64_2i( uint32_t id, char* pFmt, int64_t d0, int64_t d1 ){
    TRICE64_2i( id, pFmt, d0, d1 );
}
#endif

#ifndef trice64_2
//! trice64_2 does trace id and 64-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
void trice64_2( uint32_t id, char* pFmt, int64_t d0, int64_t d1 ){
    TRICE64_2( id, pFmt, d0, d1 );
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
