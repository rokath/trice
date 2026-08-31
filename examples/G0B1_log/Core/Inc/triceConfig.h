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
#define TRICE_DEFERRED_BUFFER_SIZE 16384
#define TRICE_SINGLE_MAX_SIZE 256
#define TRICE_FULL_CHECK

// Producer tasks and interrupts only append compact binary records. The idle
// diagnostics task later formats those records locally and writes plain text
// through USART2. No binary Trice transfer channel is enabled in this mode.
#define TRICE_DIRECT_OUTPUT 0
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_LOCAL_LOG 1
#define TRICE_LOCAL_LOG_USE_PRINTF_HOOK 1
#define TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER 0
#define TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS 1
#define TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS 1
#define TRICE_LOCAL_LOG_USE_64_BIT_VALUES 1
#define TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS 1
#define TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG 1
#define TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS 1
#define TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES 1
#define TRICE_LOCAL_LOG_USE_BUFFER_TRICES 1

// Local logging consumes ordinary binary log records. The shared TriceCheck
// corpus also contains command/RPC and selector-0 transport tests for other
// workflows; keep only those unrelated families disabled in this target.
#define TRICE_LEGACY_RPC_SUPPORT 0
#define TRICE_TX_ABC_SUPPORT 0
#define TRICE_TX_X0_COUNTED_BUFFER_SUPPORT 0

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
