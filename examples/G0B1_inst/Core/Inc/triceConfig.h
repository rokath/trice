/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// hardware specific trice lib settings
//#include "main.h" 
//#define TriceStamp16  TIM17->CNT 
//#define TriceStamp32  HAL_GetTick()

#define TRICE_BUFFER TRICE_RING_BUFFER

//  #define TRICE_DIRECT_OUTPUT 1
//  #define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_COBS
//  #define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2

#include "cmsis_gcc.h"
//#define USE_SEGGER_RTT_LOCK_UNLOCK_MACROS 0
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }
//#define TRICE_ENTER_CRITICAL_SECTION { SEGGER_RTT_LOCK() { 
//#define TRICE_LEAVE_CRITICAL_SECTION } SEGGER_RTT_UNLOCK() } 

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
