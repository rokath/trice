/*! \file triceSeggerRTT.c
\author Thomas.Hoehenleitner [at] seerose.net
\details This file decouples SEGGER RTT usage from trice code.
*******************************************************************************/
#include <stdint.h>
#include "trice.h"
#include "SEGGER_RTT.h"

#define TRICE_RTT_CHANNEL 0

//! put one trice into RTT0 buffer
//! \param v trice
//! trice time critical part
void triceU32PushSeggerRTT(uint32_t v) {
    SEGGER_RTT_Write(TRICE_RTT_CHANNEL, &v, sizeof(v));
}


//! put one byte into RTT0 buffer
//! \param v byte
//! trice time critical part
void triceU8PushSeggerRTT(uint8_t v) {
    SEGGER_RTT_Write(TRICE_RTT_CHANNEL, &v, sizeof(v));
}
