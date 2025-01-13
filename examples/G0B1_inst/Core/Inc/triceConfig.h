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

// Windows RTT logging: in a user shell like git bash:
// trice log -p JLINK -args="-Device STM32G0B1RE" -prefix off -hs off -d16 -ts ms -i ../../demoTIL.json -li ../../demoLI.json -v
//
// Linux or Darwin:
// Flash, start debugger, run to main() and then execute ./RTTLogUnix.sh or do manually:
    // Terminal 1: rm ./temp/trice.bin && JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin
    // Terminal 2: touch ./temp/trice.bin && trice log -p FILE -args ./temp/trice.bin -prefix off -hs off -d16 -ts ms -i ../../demoTIL.json -li ../../demoLI.json -pf none
// Continue to run in debugger
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

// Flash, start debugger, run to main() and then execute ./RTTLog*.sh or do manually:
// Windows serial port logging in a user shell like git bash 
    // trice log -p com3         -prefix off -hs off -pw MySecret -pf COBS -i ../../demoTIL.json -li ../../demoLI.json
// Unix serial port logging tn a terminal window::
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
