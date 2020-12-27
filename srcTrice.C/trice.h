/*! \file trice.h
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_H_
#define TRICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_NOCODE_ENCODING 10
#define TRICE_FAST_BARE_SYNC_ENCODING 20
#define TRICE_LESS_BARE_SYNC_ENCODING 30
#define TRICE_FAST_BARE_WRAP_ENCODING 40
#define TRICE_LESS_BARE_WRAP_ENCODING 50
#define TRICE_FAST_BARE_L_SYNC_ENCODING 60
#define TRICE_LESS_BARE_L_SYNC_ENCODING 70
#define TRICE_FAST_BARE_L_WRAP_ENCODING 80
#define TRICE_LESS_BARE_L_WRAP_ENCODING 90
#define TRICE_ESC_ENCODING 100
#define TRICE_ESCL_ENCODING 200
#define TRICE_PACK32_ENCODING 300

#include "triceConfig.h"

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
    defined(__BIG_ENDIAN__) || \
    defined(__ARMEB__) || \
    defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
// It's a big-endian target architecture
#define HTONS(n) ((uint16_t)(n))
#define HTON(n)  ((uint32_t)(n))
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
    defined(__LITTLE_ENDIAN__) || \
    defined(__ARMEL__) || \
    defined(__THUMBEL__) || \
    defined(__AARCH64EL__) || \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
// It's a little-endian target architecture
#define HTONS(n) ( (((uint16_t)(n))>>8) | (((uint16_t)(n))<<8) )
#define HTON(n)  ( HTONS((((uint32_t)(n))>>16)) |  HTONS((((uint32_t)(n))<<16)) ) 
#else
#error "I don't know what architecture this is!"
#endif

#include "intern/triceConfigCompiler.h"
#include "intern/triceFifo.h"
#include "intern/triceSeggerRTT.h"

#include "intern/triceNoCode.h"
#include "intern/triceEsc.h"
#include "intern/triceFastBare.h"
#include "intern/tricePack32.h"
//#include "intern/triceLessFlashBareL.h"

#include "intern/triceBareFifoToBytesBuffer.h"

#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
