/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_AUXILIARY32 1
#define TRICE_CGO 1
#define TRICE_CYCLE_COUNTER 0 // Needs to be 0, because `trice log` is called on each buffer separately.

//////////////////////////////////////////////////////////////////////////////
// user print
//
#include "nanoprintf.h"

extern uint8_t* cgoTriceBuffer;
extern unsigned cgoTriceBufferDepth;

//! prints is a user print example with string, float and integer values.
//! We have to perform a normal print into a buffer, which then is passed to triceS.
//! This is slow but we can integrate user code without changing it.
#define print(fmt, ...) \
    do { \
        int len = npf_snprintf((char*)cgoTriceBuffer+sizeof(uint16_t), 256, fmt, ##__VA_ARGS__); \
        uint16_t * plen = (uint16_t*)cgoTriceBuffer; \
        *plen = (uint16_t)len; \
        cgoTriceBufferDepth = len ? sizeof(uint16_t) + len : 0; /* no output for len==0 */ \
    } while(0)

//
//////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
