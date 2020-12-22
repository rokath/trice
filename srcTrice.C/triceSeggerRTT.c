/*! \file bareRTT.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include <stdint.h>
#include "trice.h"
#include "SEGGER_RTT.h"

#define TRICE_RTT_CHANNEL 0

//! put one trice into RTT0 buffer
//! \param v trice
//! trice time critical part
void triceU32PushSeggerRTT(uint32_t v) {
    uint8_t b[4];
    b[0] = (uint8_t)(v >> 24);
    b[1] = (uint8_t)(v >> 16);
    b[2] = (uint8_t)(v >> 8);
    b[3] = (uint8_t)(v >> 0);
    SEGGER_RTT_Write(TRICE_RTT_CHANNEL, &b, sizeof(v));
}


//! put one byte into RTT0 buffer
//! \param v byte
//! trice time critical part
void triceU8PushSeggerRTT(uint8_t v) {
    SEGGER_RTT_Write(TRICE_RTT_CHANNEL, &v, sizeof(v));
}
