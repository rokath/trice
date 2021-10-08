/*! \file trice.h
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_H_
#define TRICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_ESC_ENCODING     30
#define TRICE_FLEX_ENCODING    50
#define TRICE_COBSR_ENCODING   70

#define TRICE_LITTLE_ENDIANNESS 0x00112233
#define TRICE_BIG_ENDIANNESS    0x33221100

#include "triceConfig.h"
#include "intern/triceConfigCompiler.h"

#if (TRICE_ENCODING == TRICE_COBSR_ENCODING)
uint8_t triceCOBSREncode(uint8_t *dst, const uint8_t * src, uint8_t len);
extern uint16_t TriceDepthMax;
void ServeTriceTranslation( void );
#endif

#ifdef TRICE_RTT_CHANNEL
#include "SEGGER_RTT.h"

//! put one trice into RTT0 buffer
//! \param v trice
//! trice time critical part
TRICE_INLINE void triceU32PushSeggerRTT(uint32_t v) {
    SEGGER_RTT_Write(TRICE_RTT_CHANNEL, &v, sizeof(v));
}

//! put one byte into RTT0 buffer
//! \param v byte
//! trice time critical part
TRICE_INLINE void triceU8PushSeggerRTT(uint8_t v) {
    SEGGER_RTT_Write(TRICE_RTT_CHANNEL, &v, sizeof(v));
}

#else // #ifdef TRICE_RTT_CHANNEL

#define triceU8PushSeggerRTT(v)
#define triceU32PushSeggerRTT(v)

#endif // #else // #ifdef TRICE_RTT_CHANNEL

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

#define TRICE8_COUNT(_1,_2,_3,_4,_5,_6,_7,_8, NAME,...) NAME
#define TRICE8(id,frmt, ...) TRICE8_COUNT(__VA_ARGS__,TRICE8_8,TRICE8_7,TRICE8_6,TRICE8_5,TRICE8_4,TRICE8_3,TRICE8_2,TRICE8_1)(id,frmt, __VA_ARGS__)

#define TRICE16_COUNT(_1,_2,_3,_4, NAME,...) NAME
#define TRICE16(id,frmt, ...) TRICE16_COUNT(__VA_ARGS__,TRICE16_4,TRICE16_3,TRICE16_2,TRICE16_1)(id,frmt, __VA_ARGS__)

#define TRICE32_COUNT(_1,_2,_3,_4, NAME,...) NAME
#define TRICE32(id,frmt, ...) TRICE32_COUNT(__VA_ARGS__,TRICE32_4,TRICE32_3,TRICE32_2,TRICE32_1)(id,frmt, __VA_ARGS__)

#define TRICE64_COUNT(_1,_2,_3,_4, NAME,...) NAME
#define TRICE64(id,frmt, ...) TRICE64_COUNT(__VA_ARGS__,TRICE64_4,TRICE64_3,TRICE64_2,TRICE64_1)(id,frmt, __VA_ARGS__)

#define TRICE8_COUNTi(_1i,_2i,_3i,_4i,_5i,_6i,_7i,_8i, NAME,...) NAME
#define TRICE8i(id,frmt, ...) TRICE8_COUNTi(__VA_ARGS__,TRICE8_8i,TRICE8_7i,TRICE8_6i,TRICE8_5i,TRICE8_4i,TRICE8_3i,TRICE8_2i,TRICE8_1i)(id,frmt, __VA_ARGS__)

#define TRICE16_COUNTi(_1i,_2i,_3i,_4i, NAME,...) NAME
#define TRICE16i(id,frmt, ...) TRICE16_COUNTi(__VA_ARGS__,TRICE16_4i,TRICE16_3i,TRICE16_2i,TRICE16_1i)(id,frmt, __VA_ARGS__)

#define TRICE32_COUNTi(_1i,_2i,_3i,_4i, NAME,...) NAME
#define TRICE32i(id,frmt, ...) TRICE32_COUNTi(__VA_ARGS__,TRICE32_4i,TRICE32_3i,TRICE32_2i,TRICE32_1i)(id,frmt, __VA_ARGS__)

#define TRICE64_COUNTi(_1i,_2i,_3i,_4i, NAME,...) NAME
#define TRICE64i(id,frmt, ...) TRICE64_COUNTi(__VA_ARGS__,TRICE64_4i,TRICE64_3i,TRICE64_2i,TRICE64_1i)(id,frmt, __VA_ARGS__)

#define Trice8_COUNT(_1,_2, NAME,...) NAME
#define Trice8(id,frmt, ...) Trice8_COUNT(__VA_ARGS__, Trice8_2,Trice8_1)(id,frmt, __VA_ARGS__)

#define Trice16_COUNT(_1, NAME,...) NAME
#define Trice16(id,frmt, ...) Trice16_COUNT(__VA_ARGS__,Trice16_1)(id,frmt, __VA_ARGS__)

#define Trice8_COUNTi(_1i,_2i, NAME,...) NAME
#define Trice8i(id,frmt, ...) Trice8_COUNTi(__VA_ARGS__, Trice8_2i,Trice8_1i)(id,frmt, __VA_ARGS__)

#define Trice16_COUNTi(_1i, NAME,...) NAME
#define Trice16i(id,frmt, ...) Trice16_COUNTi(__VA_ARGS__,Trice16_1i)(id,frmt, __VA_ARGS__)

#define trice8_COUNT(_1,_2,_3,_4,_5,_6,_7,_8, NAME,...) NAME
#define trice8(id,frmt, ...) trice8_COUNT(__VA_ARGS__,trice8_8,trice8_7,trice8_6,trice8_5,trice8_4,trice8_3,trice8_2,trice8_1)(id,frmt, __VA_ARGS__)

#define trice16_COUNT(_1,_2,_3,_4, NAME,...) NAME
#define trice16(id,frmt, ...) trice16_COUNT(__VA_ARGS__,trice16_4,trice16_3,trice16_2,trice16_1)(id,frmt, __VA_ARGS__)

#define trice32_COUNT(_1,_2,_3,_4, NAME,...) NAME
#define trice32(id,frmt, ...) trice32_COUNT(__VA_ARGS__,trice32_4,trice32_3,trice32_2,trice32_1)(id,frmt, __VA_ARGS__)

#define trice64_COUNT(_1,_2,_3,_4, NAME,...) NAME
#define trice64(id,frmt, ...) trice64_COUNT(__VA_ARGS__,trice64_4,trice64_3,trice64_2,trice64_1)(id,frmt, __VA_ARGS__)

#define trice8_COUNTi(_1i,_2i,_3i,_4i,_5i,_6i,_7i,_8i, NAME,...) NAME
#define trice8i(id,frmt, ...) trice8_COUNTi(__VA_ARGS__,trice8_8i,trice8_7i,trice8_6i,trice8_5i,trice8_4i,trice8_3i,trice8_2i,trice8_1i)(id,frmt, __VA_ARGS__)

#define trice16_COUNTi(_1i,_2i,_3i,_4i, NAME,...) NAME
#define trice16i(id,frmt, ...) trice16_COUNTi(__VA_ARGS__,trice16_4i,trice16_3i,trice16_2i,trice16_1i)(id,frmt, __VA_ARGS__)

#define trice32_COUNTi(_1i,_2i,_3i,_4i, NAME,...) NAME
#define trice32i(id,frmt, ...) trice32_COUNTi(__VA_ARGS__,trice32_4i,trice32_3i,trice32_2i,trice32_1i)(id,frmt, __VA_ARGS__)

#define trice64_COUNTi(_1i,_2i,_3i,_4i, NAME,...) NAME
#define trice64i(id,frmt, ...) trice64_COUNTi(__VA_ARGS__,trice64_4i,trice64_3i,trice64_2i,trice64_1i)(id,frmt, __VA_ARGS__)

#define TRICE_U8_JOIN(  first, second ) ((uint16_t)((((uint8_t )(first))<< 8)|((uint8_t )(second)))) //!< helper macro
#define TRICE_U16_JOIN( first, second ) (          ((((uint32_t)(first))<<16)|((uint16_t)(second)))) //!< helper macro

#if TRICE_HARDWARE_ENDIANNESS == TRICE_TRANSFER_ENDIANNESS
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

#ifdef TRICE_OFF
#include "intern/triceNoCode.h"
#elif  TRICE_ESC_ENCODING == TRICE_ENCODING
#include "intern/triceEscEncoder.h"
#elif TRICE_FLEX_ENCODING == TRICE_ENCODING
#include "intern/triceFlexEncoder.h"
#elif TRICE_COBSR_ENCODING == TRICE_ENCODING
#include "intern/triceCobsrEncoder.h"
#else
#error "wrong configuration"
#endif

#ifndef TRICE_SYNC // some encoder define a sync trice
#define TRICE_SYNC do{ } while(0)// otherwise empty definition for compability
#endif

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
