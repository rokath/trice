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

extern uint32_t ms32; //! ms32 is a 32-bit millisecond counter, counting circular in steps of 1 every ms.
#include "stm32f0xx_ll_system.h"
#define TriceStamp16 (SysTick->VAL) // Counts from 31999 -> 0 in each ms.
#define TriceStamp32 (SysTick->VAL) // ms32

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 2048

// Windows in a user shell like git bash:
// trice log -p jlink -args "-Device STM32F030R8" -pf none -prefix off -hs off -d16 -showID "deb:%5d" -i ../../demoTIL.json -li ../../demoLI.json
//
// Linux or Darwin:
// Flash, start debugger and run to main()
// Terminal 1: rm ./temp/trice.bin && JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin
// Terminal 2: touch ./temp/trice.bin && trice log -p FILE -args ./temp/trice.bin -prefix off -hs off -d16 -i ../../demoTIL.json -li ../../demoLI.json -pf none
// Continue to run in debugger
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

// Windows in a user shell like git bash:
	// trice l -p com4       -ts16 "time:     #%6d" -i ../../demoTIL.json -li ../../demoLI.json
//
// Linux or Darwin:
	// trice l -p /dev/ttyACM0 -ts16 "time:     #%6d" -i ../../demoTIL.json -li ../../demoLI.json
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2

#include "cmsis_gcc.h"
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

void TriceHeadLine(char* name);
void LogTriceConfiguration(void);
void SomeExampleTrices(int burstCount);

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
