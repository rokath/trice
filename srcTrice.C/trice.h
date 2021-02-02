/*! \file trice.h
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_H_
#define TRICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_NO_CODE_ENCODING 10
#define TRICE_BARE_ENCODING    20
#define TRICE_ESC_ENCODING     30
#define TRICE_PACK_ENCODING    40

#define TRICE_SPEED_OVER_SPACE 0x55aa
#define TRICE_SPACE_OVER_SPEED 0xaa55

#define TRICE_LITTLE_ENDIANNESS 0x00112233
#define TRICE_BIG_ENDIANNESS    0x33221100

#include "triceConfig.h"
#include "intern/triceConfigCompiler.h"
#include "intern/triceFifo.h"
#include "intern/triceSeggerRTT.h"
#ifdef ENCRYPT
#include "intern/triceXteaCrypto.h"
#endif

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

#if TRICE_NOCODE_ENCODING == TRICE_ENCODING
#include "intern/triceNoCode.h"
#endif

#if TRICE_ESC_ENCODING == TRICE_ENCODING
#include "intern/triceEscEncoder.h"
#include "intern/triceFifoToBytesBuffer.h"
#endif


#if TRICE_BARE_ENCODING == TRICE_ENCODING
#include "intern/triceBareEncoder.h"
#include "intern/triceFifoToBytesBuffer.h"
#endif

#if TRICE_PACK_ENCODING == TRICE_ENCODING
#include "intern/tricePackEncoder.h"
#endif

#if TRICE_PACK2_ENCODING == TRICE_ENCODING
#include "intern/tricePack2Encoder.h"
#endif

#ifndef TRICE_SYNC // some encoder define a sync trice
#define TRICE_SYNC do{ } while(0)// otherwise empty definition for compability
#endif

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

#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
