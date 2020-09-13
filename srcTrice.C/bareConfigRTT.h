/*! \file bareConfigRTT.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef BARE_CONFIG_RTT_H_
#define BARE_CONFIG_RTT_H_

#include "bareConfigCompiler.h"

#ifdef __cplusplus
extern "C" {
#endif

// predefine SEGGER constants before including "SEGGER_RTT.h" to overwrite default values
#ifndef TRICE_RTT_CHANNEL
#define TRICE_RTT_CHANNEL 0
#endif
/*
#define TRICE_RTT_BUFFER_SIZE 1024 //!< must be a multiple of 4
#if 0 == TRICE_RTT_CHANNEL
#define BUFFER0_SIZE_UP   TRICE_RTT_BUFFER_SIZE
#else
#define BUFFER0_SIZE_UP   4
#endif
#if 1 == TRICE_RTT_CHANNEL
#define BUFFER1_SIZE_UP   TRICE_RTT_BUFFER_SIZE
#else
#define BUFFER1_SIZE_UP   4
#endif
#if 2 == TRICE_RTT_CHANNEL
#define BUFFER2_SIZE_UP   TRICE_RTT_BUFFER_SIZE
#else
#define BUFFER2_SIZE_UP   4
#endif
#define BUFFER0_SIZE_DOWN   16
#define BUFFER1_SIZE_DOWN    4
#define BUFFER2_SIZE_DOWN    4
#define SEGGER_RTT_MAX_NUM_UP_BUFFERS                   (3)    // Number of up-buffers (T->H) available on this target
#define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS                 (3)    // Number of down-buffers (H->T) available on this target
*/
#include "SEGGER_RTT.h"

//! put one trice into RTT0 buffer
//! \param v trice
//! trice time critical part
TRICE_INLINE void tricePush( uint32_t v ){
    SEGGER_RTT_Write(TRICE_RTT_CHANNEL, &v, sizeof(v));
}

#ifdef __cplusplus
}
#endif

#endif /* BARE_CONFIG_RTT_H_ */
