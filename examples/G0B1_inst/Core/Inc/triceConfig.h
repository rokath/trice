// SPDX-License-Identifier: MIT

//! \file triceConfig.h

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_LEGACY_RPC_SUPPORT 1

// Keep the original example as the default; only an explicit build selects fast mode.
#ifndef CONFIGURATION
#define CONFIGURATION 0
#endif

// TRICE_SINGLE_MAX_SIZE is used to truncate long runtime-generated strings, to detect the need for a ring buffer wrap, or to protect against overflow.
// Limit custom assert messages to a safe size (>104) to avoid truncation
#define TRICE_SINGLE_MAX_SIZE 256

// hardware specific trice lib settings
#include "main.h"
#if CONFIGURATION == 0
#define TriceStamp16 TIM17->CNT    // 0...999 us
#define TriceStamp32 HAL_GetTick() // 0...2^32-1 ms (wraps after 49.7 days)
// #define TriceStamp32 1750507965 // Output of 'date +"%s"' at Sa 21 Jun 2025 14:12:45 CEST

#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 2000

// Windows: trice log -p jlink -args "-Device STM32G0B1RE" -pf none -prefix off -hs off -d16 -ts16 us -ts32 ms -showID "deb:%5d" -i ../../demoTIL.json -li ../../demoLI.json -typeX0 "att:% x\n"
// Unix:   ./RTTLogUnix.sh or manually:
// 		Terminal 1: rm -f ./temp/trice.bin && JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin
//      Terminal 2: touch ./temp/trice.bin && trice log -p FILE -args ./temp/trice.bin -pf none -prefix off -hs off -d16 -ts16 us -ts32 ms -i ../../demoTIL.json -li ../../demoLI.json -typeX0 "att:% x\n"
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER_SIZE_UP 2048 // "TRICE_DIRECT_BUFFER_SIZE"

// Windows: trice log -p com4         -prefix off -hs off -pw MySecret -pf COBS -ts16 us -ts32 ms -i ../../demoTIL.json -li ../../demoLI.json -typeX0 "att:% x\n"
// Unix:    trice log -p /dev/ttyACM0 -prefix off -hs off -pw MySecret -pf COBS -ts16 us -ts32 ms -i ../../demoTIL.json -li ../../demoLI.json -typeX0 "att:% x\n"
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

#elif CONFIGURATION == 98
// TIM2 runs at the 64 MHz CPU clock. These are raw clock counts, not microseconds.
#define TriceStamp16 ((uint16_t)TIM2->CNT)
#define TriceStamp32 TIM2->CNT

// One task produces a bounded burst and transfers it afterwards. No other task
// or interrupt may write records in this profile, so no per-record locks/checks
// are needed. The complete burst and its reports fit in one buffer half.
//  tlog -p com5 -i ../../demoTIL.json -li ../../demoLI.json -ts16delta "wr:%4d" -ts32delta "wr:%4d"
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 4096
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }
#else
#error "G0B1_inst supports CONFIGURATION=0 (normal) or CONFIGURATION=98 (fast)."
#endif

#if CONFIGURATION == 98 && !TRICE_OFF
// Initialize the private measurement timer before tasks start, then measure once
// from the sole logging task. HAL's TIM17 and FreeRTOS's SysTick remain available.
void G0B1FastInit(void);
void G0B1FastMeasure(void);
#endif

void TriceHeadLine(char* name);
void LogTriceConfiguration(void);
void SomeExampleTrices(int burstCount);

#define TRICE_FULL_CHECK

#define tell trice
#define who triceS

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
