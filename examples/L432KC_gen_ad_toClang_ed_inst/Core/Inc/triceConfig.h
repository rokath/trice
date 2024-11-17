/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//! TRICE_CLEAN, if found inside triceConfig.h, is modified by the Trice tool to silent editor warnings in the cleaned state.
#define TRICE_CLEAN 1 // Do not define this at an other place! But you can delete this here.

#include <stdint.h>

extern volatile uint32_t * const DWT_CONTROL;
extern volatile uint32_t * const DWT_CYCCNT ;
extern volatile uint32_t * const DEMCR;
extern volatile uint32_t * const LAR; // lock access register

void TriceHeadLine(char* name);
void SomeExampleTrices(int burstCount);

#define TRICE_FULL_CHECK

#if !defined(TRICE_PROJECT_CONFIG) || TRICE_PROJECT_CONFIG == 0 /////////////////////
// An example configuration with direct RTT output and parallel deferred UART output.
/////////////////////////////////////////////////////////////////////////////////////

#define TriceStamp16 (*DWT_CYCCNT)      // @64MHz wraps after a bit more than 1ms (MCU clocks) 
#define TriceStamp32 ((*DWT_CYCCNT)>>6) // @64MHz -> 1 µs, wraps after 2^32 µs ~= 1.2 hours

#define TRICE_BUFFER TRICE_RING_BUFFER

// trice l -p JLINK -args="-Device STM32L432KC -if SWD -Speed 4000 -RTTChannel 0" -pf cobs -ts ms -d16
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_COBS
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

// trice log -p com7 -pw MySecret -pf COBS
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2

#define USE_SEGGER_RTT_LOCK_UNLOCK_MACROS 1
#define TRICE_ENTER_CRITICAL_SECTION { SEGGER_RTT_LOCK() { 
#define TRICE_LEAVE_CRITICAL_SECTION } SEGGER_RTT_UNLOCK() } 

#elif TRICE_PROJECT_CONFIG == 1 /////////////////////////////////////////////////////
// An example configuration with direct RTT output.
// trice l -p JLINK -args="-Device STM32L432KC" -pf none -ts ms -d16
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_BUFFER TRICE_STACK_BUFFER

#elif TRICE_PROJECT_CONFIG == 2 /////////////////////////////////////////////////////
// An example configuration with deferred UART output using the ring buffer.
// trice log -p com7
#include "cmsis_gcc.h"
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

#elif TRICE_PROJECT_CONFIG == 4 /////////////////////////////////////////////////////
// An example configuration with deferred UART output using the double buffer and
// without protection, diagnostiocs, critical sections to get max log speed.
// trice log -p com7
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0

#elif TRICE_PROJECT_CONFIG == 4 /////////////////////////////////////////////////////
// An minimum configuration example
#define TRICE_DEFERRED_OUTPUT 1
#define TriceStamp16 uwTick  // 1ms, wraps after 2^16 ms ~= a bit more than 1min 
#define TriceStamp32 uwTick  // 1ms, wraps after 2^32 ms ~= 41 days 
#define TRICE_BUFFER TRICE_RING_BUFFER
/////////////////////////////////////////////////////////////////////////////////////
#else
#error unknown TRICE_PROJECT_CONFIG value
#endif

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
