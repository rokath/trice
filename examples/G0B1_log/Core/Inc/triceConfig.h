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
#define TriceStamp16 ((uint16_t)HAL_GetTick()) // Elapsed milliseconds modulo 65.536 seconds.
#define TriceStamp32 HAL_GetTick()             // Elapsed milliseconds; wraps after 49.7 days.

#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 16384
#define TRICE_SINGLE_MAX_SIZE 256
#define TRICE_FULL_CHECK

// Producer tasks and interrupts only append compact binary records. The idle
// diagnostics task later formats those records locally and writes plain text
// through USART2. No binary Trice transfer channel is enabled in this mode.
// In Windows, run "trice s" to detect the COM port number (for example 5).
// In cmd.exe, run "mode COM5: BAUD=115200 PARITY=N DATA=8 STOP=1 XON=OFF OCTS=OFF ODSR=OFF" to setup COM port.
// `type COM5` is suitable for ASCII only: it may split a multi-byte UTF-8 code
// point between reads. Use a serial terminal configured for UTF-8 to see emoji.
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_LOCAL_LOG 1
#define TRICE_LOCAL_LOG_USE_PRINTF_HOOK 1
#define TRICE_LOCAL_LOG_USE_PREFIX_HOOK 1
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

// Match the familiar host presentation: recognized lower-case tags disappear,
// while their message bodies receive record-local terminal colors.
#define TRICE_LOCAL_LOG_USE_ANSI_COLORS 1
#define TRICE_LOCAL_LOG_STRIP_LOWER_CASE_TAGS 1

// The local prefix hook formats millisecond stamps like `trice log -ts ms` but
// omits the "time:" label. Keeping these strings here makes the presentation a
// target setting; the callback in main.c only supplies the calculated fields.
// STAMP16 receives seconds and milliseconds; STAMP32 receives hours, minutes,
// seconds, and milliseconds, in that order.
#if TRICE_LOCAL_LOG_USE_ANSI_COLORS == 1
#define TRICE_LOCAL_LOG_STAMP_COLOR "\x1b[0;7;34;103m"
#define TRICE_LOCAL_LOG_STAMP_RESET "\x1b[0m"
#else
#define TRICE_LOCAL_LOG_STAMP_COLOR ""
#define TRICE_LOCAL_LOG_STAMP_RESET ""
#endif
#define TRICE_LOCAL_LOG_STAMP0_FORMAT "            "
#define TRICE_LOCAL_LOG_STAMP16_FORMAT "      %2u,%03u"
#define TRICE_LOCAL_LOG_STAMP32_FORMAT "%2lu:%02lu:%02lu,%03lu"

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
