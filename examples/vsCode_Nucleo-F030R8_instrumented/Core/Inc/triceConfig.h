/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_ll_system.h"

//! ms32 is a 32-bit millisecond counter, counting circular in steps of 1 every ms.
extern uint32_t ms32;

#define TriceStamp16 (SysTick->VAL) // Counts from 31999 -> 0 in each ms.
#define TriceStamp32  ms32
#define TRICE_UARTA   USART2

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE 1

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_UARTA 1

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
