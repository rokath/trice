/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// hardware specific trice lib settings
#include "main.h" 
#define TriceStamp16  TIM17->CNT 
#define TriceStamp32  HAL_GetTick()
#define TRICE_UARTA   USART2

// agnostic trice lib settings

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_COBS

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_UARTA 1

#define USE_SEGGER_RTT_LOCK_UNLOCK_MACROS 1
#define TRICE_ENTER_CRITICAL_SECTION { SEGGER_RTT_LOCK() { 
#define TRICE_LEAVE_CRITICAL_SECTION } SEGGER_RTT_UNLOCK() } 

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
