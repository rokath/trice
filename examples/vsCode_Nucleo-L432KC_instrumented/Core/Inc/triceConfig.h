/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern volatile uint32_t * const DWT_CONTROL;
extern volatile uint32_t * const DWT_CYCCNT ;
extern volatile uint32_t * const DEMCR;
extern volatile uint32_t * const LAR; // lock access register

#define TriceStamp16 (*DWT_CYCCNT) // 64 MHz wraps after >1ms 
#define TriceStamp32 ((*DWT_CYCCNT)>>6) // 64 MHz -> 1 µs
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

#define USE_SEGGER_RTT_LOCK_UNLOCK_MACROS 1
#define TRICE_ENTER_CRITICAL_SECTION { SEGGER_RTT_LOCK() { 
#define TRICE_LEAVE_CRITICAL_SECTION } SEGGER_RTT_UNLOCK() } 

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
