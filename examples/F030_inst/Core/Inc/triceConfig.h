/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_CLEAN 1 // If found, this is an automatically modified value, when executing `trice insert` or `trice clean`

extern uint32_t ms32; //! ms32 is a 32-bit millisecond counter, counting circular in steps of 1 every ms.
#include "stm32f0xx_ll_system.h"
#define TriceStamp16 (SysTick->VAL) // Counts from 31999 -> 0 in each ms.
#define TriceStamp32 (SysTick->VAL) // ms32

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 4096

// trice l -p jlink -args "-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0" -pf none -d16 -showID "deb:%5d"
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

// trice l -p com5 -ts16 "time:     #%6d"
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2

#include "cmsis_gcc.h"
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

void TriceHeadLine(char* name);
void SomeExampleTrices(int burstCount);

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
