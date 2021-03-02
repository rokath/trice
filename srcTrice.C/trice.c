/*! \file trice.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include <stdint.h>
#include <string.h> // strlen
//#include "triceConfig.h"
//#include "intern/triceConfigCompiler.h"
//#include "intern/triceFifo.h"
#include "trice.h"

#define TRICE_FILENAME TRICE0( Id(65002), "rd_:triceFifo.c" );

#if TRICE_ENCODING == TRICE_PACK2_ENCODING || TRICE_ENCODING == TRICE_FLEX_ENCODING 
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
//! trice0i does trace id protected (outside critical section).
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


//! syncLevelLimit is the max amount of trices bulk transmitted without a sync trice injected.
//! Assuming triceServeBareFifoToBytesBuffer() is called every ms this is also the max ms abount without a sync trice.
//! Bigger numbers decrease the bandwidth needs but increase the possible loss of trices in case of
//! re-sync need. A number of 10-100 is recommended: every 10-100 ms or 10-100 trices a sync trice is transmitted.
//! It increases the bandwidth need by 10-1%.
int const syncLevelLimit = 100;

#ifndef TRICE_WRAP_START_BYTE
#define TRICE_WRAP_START_BYTE 0
#endif

#ifndef TRICE_WRAP_LOCAL_ADDR
#define TRICE_WRAP_LOCAL_ADDR 0
#endif

#ifndef TRICE_WRAP_DEST_ADDR
#define TRICE_WRAP_DEST_ADDR 0
#endif

uint8_t triceBytesBuffer[8] = { TRICE_WRAP_START_BYTE, TRICE_WRAP_LOCAL_ADDR, TRICE_WRAP_DEST_ADDR, 0, 0,0,0,0 }; //!< bytes transmit buffer (prefilled for wrap
int const triceBytesBufferIndexLimit = 8; // sizeof(triceBytesBuffer[8]);
int triceBytesBufferIndex = triceBytesBufferIndexLimit;

TRICE_INLINE void triceTransfer(uint32_t t0, uint32_t t1) {
    *(uint32_t*)&triceBytesBuffer[0] = t0;
    *(uint32_t*)&triceBytesBuffer[4] = t1;
#ifdef ENCRYPT
    encrypt( triceBytesBuffer );
#endif
}

int triceBytesByfferDepth( void ){
    return triceBytesBufferIndexLimit - triceBytesBufferIndex;
}

void triceServeFifoSyncedToBytesBuffer(void) {
    // 89 ab cd ef <- on serial port
    // ih il dh dl
    uint32_t const syncTrice = TRICE_HTON(0x89abcdef);
    static int syncLevel = syncLevelLimit; // start with a sync trice
    if (triceBytesBufferIndexLimit == triceBytesBufferIndex) { // bytes buffer empty and tx finished
        // next trice
        int n = triceU32FifoDepth();
        if (syncLevel < syncLevelLimit) { // no need for a sync trice
            if (0 == n) { // no trices to transmit
                syncLevel++;
                return;
            } else if (4 == n) { // one trice to transmit
                triceTransfer(triceU32Pop(), syncTrice);
                syncLevel = 0;
            } else { // at least 2 trices to transmit
                uint32_t t0 = triceU32Pop();
                uint32_t t1 = triceU32Pop();
                triceTransfer(t0, t1);
                syncLevel += 2;
            }
        } else { // need for a sync trice
            if (4 <= n) { // at least one trice, so transmit it and one sync trice
                triceTransfer(triceU32Pop(), syncTrice);
            } else { // nothing to transmit so transmit 2 sync trices
                triceTransfer(syncTrice, syncTrice);
            }
            syncLevel = 0;
        }
        triceBytesBufferIndex = 0;
    }
}

void triceServeFifoWrappedToBytesBuffer(void) {
    if (triceBytesBufferIndexLimit == triceBytesBufferIndex) { // bytes buffer empty and tx finished
        // next trice
        int n = triceU32FifoDepth();
        if ( n >= 4 ) { // a trice to transmit
            uint32_t x = triceU32Pop();
#ifdef ENCRYPT // need to re-init buffer
            triceBytesBuffer[0] = TRICE_WRAP_START_BYTE;
            triceBytesBuffer[1] = TRICE_WRAP_LOCAL_ADDR;
            triceBytesBuffer[2] = TRICE_WRAP_DEST_ADDR;
#endif
            triceBytesBuffer[3] = (uint8_t)( TRICE_WRAP_START_BYTE ^ TRICE_WRAP_LOCAL_ADDR ^ TRICE_WRAP_DEST_ADDR ^ x ^ (x>>8) ^ (x>>16) ^ (x>>24) ); // crc8
            
            *(uint32_t*)&triceBytesBuffer[4] = x; //triceLoadInNetworkOrder(&triceBytesBuffer[4], x);
#ifdef ENCRYPT
            encrypt( triceBytesBuffer );
#endif
            triceBytesBufferIndex = 0;
        }
    }
}

