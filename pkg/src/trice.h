/*! \file trice.h
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_H_
#define TRICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_NO_CODE_ENCODING 10
#define TRICE_ESC_ENCODING     30
#define TRICE_FLEX_ENCODING    50

#define TRICE_SPEED_OVER_SPACE 0x55aa
#define TRICE_SPACE_OVER_SPEED 0xaa55

#define TRICE_LITTLE_ENDIANNESS 0x00112233
#define TRICE_BIG_ENDIANNESS    0x33221100

#include "triceConfig.h"
#include "intern/triceConfigCompiler.h"
#include "intern/triceSeggerRTT.h"
#ifdef ENCRYPT
void encrypt(uint8_t *p);
void decrypt(uint8_t *p);

//! little endian! change byte order for big endian machines
#define XTEA_KEY(b00, b01, b02, b03, \
                  b10, b11, b12, b13, \
                  b20, b21, b22, b23, \
                  b30, b31, b32, b33) { \
    0x##b00##b01##b02##b03, \
    0x##b10##b11##b12##b13, \
    0x##b20##b21##b22##b23, \
    0x##b30##b31##b32##b33 }

void InitXteaTable(void);
    
#endif // #ifdef ENCRYPT

#define TRICE_U8_JOIN(  first, second ) ((uint16_t)((((uint8_t )(first))<< 8)|((uint8_t )(second)))) //!< helper macro
#define TRICE_U16_JOIN( first, second ) (          ((((uint32_t)(first))<<16)|((uint16_t)(second)))) //!< helper macro


#if TRICE_HARDWARE_ENDIANNNESS == TRICE_TRANSFER_ENDIANNESS
#define TRICE_HTONS(n) ((uint16_t)(n))
#define TRICE_HTON(n)  ((uint32_t)(n))
#else
#define TRICE_HTONS(n) ( (((uint16_t)(n))>>8) | (((uint16_t)(n))<<8) )
#define TRICE_HH(n)                     ((uint32_t)(n)>>24)
#define TRICE_HL(n) ((uint32_t)(uint8_t)((uint32_t)(n)>>16))
#define TRICE_LH(n) ((uint32_t)         ((uint16_t)(n)>> 8))
#define TRICE_LL(n) ((uint32_t)         (( uint8_t)(n)    ))
#define TRICE_HTON(n) ((TRICE_LL(n)<<24)|(TRICE_LH(n)<<16)|(TRICE_HL(n)<<8)|TRICE_HH(n) )
#endif
#define TRICE_HTON_U32PUSH(v) TRICE_U32PUSH( TRICE_HTON(v) )



//! TRICE_FIFO_BYTE_SIZE must be a power of 2, one trice needs typically 4 or 8 bytes, max 32 bytes.
//! The fifo has to hold trice bursts until they are transmitted.
//! It is transmitted with lower priority in the background for example with the UART tx interrupt.
#ifndef TRICE_FIFO_BYTE_SIZE
#define TRICE_FIFO_BYTE_SIZE (2048)
#endif

#define TRICE_U32_FIFO_MASK (((TRICE_FIFO_BYTE_SIZE)>>2)-1) //!< max possible int32 count in fifo
#define TRICE_U8_FIFO_MASK ((TRICE_FIFO_BYTE_SIZE)-1) //!< max possible bytes count in fifo

extern uint32_t triceU32Fifo[ TRICE_FIFO_BYTE_SIZE>>2 ];
extern uint8_t* const triceU8Fifo;

extern int triceU32FifoWriteIndex;
extern int triceU32FifoReadIndex;

extern int triceU8FifoWriteIndex;
extern int triceU8FifoReadIndex;

extern int triceFifoMaxDepth;

//! triceU32Push puts one trice into trice fifo.
//! This is a trice time critical part.
//! \param v trice id with 2 byte data
TRICE_INLINE void triceU32Push(uint32_t v) {
    triceU32Fifo[triceU32FifoWriteIndex++] = v;
    triceU32FifoWriteIndex &= TRICE_U32_FIFO_MASK;
}

//! triceU8Push puts one byte into trice fifo.
//! This is a trice time critical part.
//! \param v trice id with 2 byte data
TRICE_INLINE void triceU8Push(uint8_t v) {
    triceU8Fifo[triceU8FifoWriteIndex++] = v;
    triceU8FifoWriteIndex &= TRICE_U8_FIFO_MASK;
}

//! triceU32Pop gets one trice from trice fifo.
//! \return trice id with 2 byte data in one uint32_t.
TRICE_INLINE uint32_t triceU32Pop(void) {
    uint32_t v = triceU32Fifo[triceU32FifoReadIndex++];
    triceU32FifoReadIndex &= TRICE_U32_FIFO_MASK;
    return v;
}

//! triceU8Pop gets one trice from trice fifo.
//! \return trice id with 2 byte data in one uint32_t.
TRICE_INLINE uint8_t triceU8Pop(void) {
    uint8_t v = triceU8Fifo[triceU8FifoReadIndex++];
    triceU8FifoReadIndex &= TRICE_U8_FIFO_MASK;
    return v;
}

int triceU32FifoDepth(void);
int triceU8FifoDepth(void);
int triceU32WriteU8ReadFifoDepth(void);


#if TRICE_NOCODE_ENCODING == TRICE_ENCODING
#include "intern/triceNoCode.h"
#endif

#if TRICE_ESC_ENCODING == TRICE_ENCODING
#include "intern/triceEscEncoder.h"
#endif

#if TRICE_FLEX_ENCODING == TRICE_ENCODING
#include "intern/triceFlexEncoder.h"
#endif

#ifndef TRICE_SYNC // some encoder define a sync trice
#define TRICE_SYNC do{ } while(0)// otherwise empty definition for compability
#endif


/*
#ifndef trice0
#define trice_sync TRICE_SYNC
#define trice0     TRICE0
#define trice8_1   TRICE8_1
#define trice8_2   TRICE8_2
#define trice8_3   TRICE8_3
#define trice8_4   TRICE8_4
#define trice8_5   TRICE8_5
#define trice8_6   TRICE8_6
#define trice8_7   TRICE8_7
#define trice8_8   TRICE8_8
#define trice16_1  TRICE16_1
#define trice16_2  TRICE16_2
#define trice16_3  TRICE16_3
#define trice16_4  TRICE16_4
#define trice32_1  TRICE32_1
#define trice32_2  TRICE32_2
#define trice32_3  TRICE32_3
#define trice32_4  TRICE32_4
#define trice64_1  TRICE64_1
#define trice64_2  TRICE64_2
#endif // #ifndef trice0
*/




#ifndef trice0i
void trice0i( uint32_t id, char* pFmt );
#endif

#ifndef trice0
void trice0( uint32_t id, char* pFmt );
#endif

#ifndef trice8_1i
void trice8_1i( uint32_t id, char* pFmt, int8_t d0 );
#endif

#ifndef trice8_1
void trice8_1( uint32_t id, char* pFmt, int8_t d0 );
#endif

#ifndef trice8_2i
void trice8_2i( uint32_t id, char* pFmt, int8_t d0, int8_t d1 );
#endif

#ifndef trice8_2
void trice8_2( uint32_t id, char* pFmt, int8_t d0, int8_t d1 );
#endif

#ifndef trice8_3i
void trice8_3i( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2 );
#endif

#ifndef trice8_3
void trice8_3( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2 );
#endif

#ifndef trice8_4i
void trice8_4i( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3 );
#endif

#ifndef trice8_4
void trice8_4( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3 );
#endif

#ifndef trice8_5i
void trice8_5i( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4 );
#endif

#ifndef trice8_5
void trice8_5( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4 );
#endif


#ifndef trice8_6i
void trice8_6i( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5 );
#endif

#ifndef trice8_6
void trice8_6( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5 );
#endif

#ifndef trice8_7i
void trice8_7i( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6 );
#endif

#ifndef trice8_7
void trice8_7( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6 );
#endif

#ifndef trice8_8i
void trice8_8i( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6, int8_t d7 );
#endif

#ifndef trice8_8
void trice8_8( uint32_t id, char* pFmt, int8_t d0, int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6, int8_t d7 );
#endif


#ifndef trice16_1i
void trice16_1i( uint32_t id, char* pFmt, int16_t d0 );
#endif

#ifndef trice16_1
void trice16_1( uint32_t id, char* pFmt, int16_t d0 );
#endif

#ifndef trice16_2i
void trice16_2i( uint32_t id, char* pFmt, int16_t d0, int16_t d1 );
#endif

#ifndef trice16_2
void trice16_2( uint32_t id, char* pFmt, int16_t d0, int16_t d1 );
#endif

#ifndef trice16_3i
void trice16_3i( uint32_t id, char* pFmt, int16_t d0, int16_t d1, int16_t d2 );
#endif

#ifndef trice16_3
void trice16_3( uint32_t id, char* pFmt, int16_t d0, int16_t d1, int16_t d2 );
#endif

#ifndef trice16_4i
void trice16_4i( uint32_t id, char* pFmt, int16_t d0, int16_t d1, int16_t d2, int16_t d3 );
#endif

#ifndef trice16_4
void trice16_4( uint32_t id, char* pFmt, int16_t d0, int16_t d1, int16_t d2, int16_t d3 );
#endif

#ifndef trice32_1i
void trice32_1i( uint32_t id, char* pFmt, int32_t d0 );
#endif

#ifndef trice32_1
void trice32_1( uint32_t id, char* pFmt, int32_t d0 );
#endif

#ifndef trice32_2i
void trice32_2i( uint32_t id, char* pFmt, int32_t d0, int32_t d1 );
#endif

#ifndef trice32_2
void trice32_2( uint32_t id, char* pFmt, int32_t d0, int32_t d1 );
#endif

#ifndef trice32_3i
void trice32_3i( uint32_t id, char* pFmt, int32_t d0, int32_t d1, int32_t d2 );
#endif

#ifndef trice32_3
void trice32_3( uint32_t id, char* pFmt, int32_t d0, int32_t d1, int32_t d2 );
#endif

#ifndef trice32_4i
void trice32_4i( uint32_t id, char* pFmt, int32_t d0, int32_t d1, int32_t d2, int32_t d3 );
#endif

#ifndef trice32_4
void trice32_4( uint32_t id, char* pFmt, int32_t d0, int32_t d1, int32_t d2, int32_t d3 );
#endif

#ifndef trice64_1i
void trice64_1i( uint32_t id, char* pFmt, int64_t d0 );
#endif

#ifndef trice64_1
void trice64_1( uint32_t id, char* pFmt, int64_t d0 );
#endif

#ifndef trice64_2i
void trice64_2i( uint32_t id, char* pFmt, int64_t d0, int64_t d1 );
#endif

#ifndef trice64_2
void trice64_2( uint32_t id, char* pFmt, int64_t d0, int64_t d1 );
#endif

#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
