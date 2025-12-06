/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// TRICE_SINGLE_MAX_SIZE is used to truncate long runtime-generated strings, to detect the need for a ring buffer wrap, or to protect against overflow.
// Limit custom assert messages to a safe size (>104) to avoid truncation
#define TRICE_SINGLE_MAX_SIZE 256

//! TRICE_CLEAN, if found inside triceConfig.h, is modified by the Trice tool to silent editor warnings in the cleaned state.
#define TRICE_CLEAN 1 // Do not define this at an other place! But you can delete this here.

// hardware specific trice lib settings
#include "main.h"
#define TriceStamp16 TIM17->CNT    // 0...999 us
#define TriceStamp32 HAL_GetTick() // 0...2^32-1 ms (wraps after 49.7 days)
// #define TriceStamp32 1750507965 // Output of 'date +"%s"' at Sa 21 Jun 2025 14:12:45 CEST

#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 2000

// Windows: trice log -p jlink -args "-Device STM32G0B1RE" -pf none -prefix off -hs off -d16 -showID "deb:%5d" -i ../../demoTIL.json -li ../../demoLI.json
// Unix:   ./RTTLogUnix.sh or manually:
// 		Terminal 1: rm -f ./temp/trice.bin && JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin
//      Terminal 2: touch ./temp/trice.bin && trice log -p FILE -args ./temp/trice.bin -pf none -prefix off -hs off -d16 -ts ms -i ../../demoTIL.json -li ../../demoLI.json
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER_SIZE_UP 2048 // "TRICE_DIRECT_BUFFER_SIZE"

// Windows: trice log -p com3         -prefix off -hs off -pw MySecret -pf COBS -i ../../demoTIL.json -li ../../demoLI.json
// Unix:    trice log -p /dev/ttyACM0 -prefix off -hs off -pw MySecret -pf COBS -i ../../demoTIL.json -li ../../demoLI.json
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2
// #define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#include "cmsis_gcc.h"
#define TRICE_ENTER_CRITICAL_SECTION             \
	{                                            \
		uint32_t primaskstate = __get_PRIMASK(); \
		__disable_irq();                         \
		{
#define TRICE_LEAVE_CRITICAL_SECTION \
	}                                \
	__set_PRIMASK(primaskstate);     \
	}

void TriceHeadLine(char* name);
void LogTriceConfiguration(void);
void SomeExampleTrices(int burstCount);

#define TRICE_FULL_CHECK

#define tell trice
#define who triceS
// #define who(iD(13002), id,...) triceS(id, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
