// SPDX-License-Identifier: MIT

//! \file triceConfig.h

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//! TRICE_CLEAN, if found inside triceConfig.h, is modified by the Trice tool to silent editor warnings in the cleaned state.
#define TRICE_CLEAN 0 // Do not define this at an other place! But you can delete this here.

// hardware specific trice lib settings
#include "main.h"
#define TriceStamp16 TIM17->CNT    // 0...999 us
#define TriceStamp32 HAL_GetTick() // 0...2^32-1 ms (wraps after 49.7 days)
// #define TriceStamp32 1750507965 // Output of 'date +"%s"' at Sa 21 Jun 2025 14:12:45 CEST

#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 2000

// Producer tasks and interrupts only append compact binary records. The idle
// diagnostics task later formats those records locally and writes plain text
// through USART2. No binary Trice transfer channel is enabled in this mode.
#define TRICE_DIRECT_OUTPUT 0
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_LOCAL_LOG 1
#define TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER 0

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

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
