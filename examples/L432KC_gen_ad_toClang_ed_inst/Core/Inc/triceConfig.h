/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_CLEAN 1

#include <stdint.h>

extern volatile uint32_t * const DWT_CONTROL;
extern volatile uint32_t * const DWT_CYCCNT ;
extern volatile uint32_t * const DEMCR;
extern volatile uint32_t * const LAR; // lock access register

#define TriceStamp16 (*DWT_CYCCNT)      // 64 MHz wraps after >1ms 
#define TriceStamp32 ((*DWT_CYCCNT)>>6) // 64 MHz -> 1 µs, wraps after 2^32 µs ~= 1.2 hours

#define TRICE_BUFFER TRICE_RING_BUFFER

// trice l -p JLINK -args="-Device STM32G0B1RE -if SWD -Speed 4000 -RTTChannel 0" -pf cobs -ts ms -d16
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_COBS
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

// trice log -p com7 -pw MySecret -pf COBS
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2

// #include "cmsis_gcc.h"
// #define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {
// #define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

#define USE_SEGGER_RTT_LOCK_UNLOCK_MACROS 1
#define TRICE_ENTER_CRITICAL_SECTION { SEGGER_RTT_LOCK() { 
#define TRICE_LEAVE_CRITICAL_SECTION } SEGGER_RTT_UNLOCK() } 

void TriceHeadLine(char* name);
void SomeExampleTrices(int burstCount);

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
