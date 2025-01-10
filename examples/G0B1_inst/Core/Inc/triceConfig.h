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

// hardware specific trice lib settings
#include "main.h" 
#define TriceStamp16 TIM17->CNT     // 0...999 us
#define TriceStamp32 HAL_GetTick()  // 0...2^32-1 ms (wraps after 49.7 days)

#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 2000

// trice log -p JLINK -args="-Device STM32G0B1RE"                                             -prefix off -hs off -d16 -ts ms  -i ../../demoTIL.json -li ../../demoLI.json -v
// trice log -p JLINK -args="-Device STM32G0B1RE -RTTAddress      0x20001238        -if SWD"  -prefix off -hs off -d16 -ts ms  -i ../../demoTIL.json -li ../../demoLI.json -v
// trice log -p JLINK -args="-Device STM32G0B1RE -RTTSearchRanges 0x20000000 0x2000 -if SWD"  -prefix off -hs off -d16 -ts ms  -i ../../demoTIL.json -li ../../demoLI.json -v
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

// trice log -p com3         -prefix off -hs off -pw MySecret -pf COBS -i ../../demoTIL.json -li ../../demoLI.json
// trice log -p /dev/ttyACM0 -prefix off -hs off -pw MySecret -pf COBS -i ../../demoTIL.json -li ../../demoLI.json
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2

#include "cmsis_gcc.h"
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

void TriceHeadLine(char* name);
void LogTriceConfiguration(void);
void SomeExampleTrices(int burstCount);

#define TRICE_FULL_CHECK

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
