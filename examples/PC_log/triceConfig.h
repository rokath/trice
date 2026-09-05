// SPDX-License-Identifier: MIT

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#include <stdint.h>

// Match G0B1_inst: 16-bit stamps are microsecond offsets inside a millisecond,
// while 32-bit stamps carry elapsed milliseconds.
uint16_t TriceDemoMicrosecondOffset(void);
uint32_t TriceDemoMilliseconds(void);
#define TriceStamp16 TriceDemoMicrosecondOffset()
#define TriceStamp32 TriceDemoMilliseconds()

// Store short binary records quickly in the producer context. TriceLog reads
// them later without first copying their payload out of the ring buffer.
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 16384
#define TRICE_SINGLE_MAX_SIZE 256
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_LOCAL_LOG 1
#define TRICE_FULL_CHECK
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

// Match `trice log -ts16 us -ts32 ms` without its stripped "time:" tag.
// STAMP16 receives the 0...999 microseconds after the millisecond boundary.
// STAMP32 receives hours, minutes, seconds, and milliseconds, in that order.
#if TRICE_LOCAL_LOG_USE_ANSI_COLORS == 1
#define TRICE_LOCAL_LOG_STAMP_COLOR "\x1b[0;7;34;103m"
#define TRICE_LOCAL_LOG_STAMP_RESET "\x1b[0m"
#else
#define TRICE_LOCAL_LOG_STAMP_COLOR ""
#define TRICE_LOCAL_LOG_STAMP_RESET ""
#endif
#define TRICE_LOCAL_LOG_STAMP0_FORMAT "            "
#define TRICE_LOCAL_LOG_STAMP16_FORMAT "       0_%03u"
#define TRICE_LOCAL_LOG_STAMP32_FORMAT "%2lu:%02lu:%02lu,%03lu"

// Local logging consumes ordinary binary log records. The shared TriceCheck
// corpus also contains command/RPC and selector-0 transport tests for other
// workflows; keep only those unrelated families disabled in this target.
#define TRICE_LEGACY_RPC_SUPPORT 0
#define TRICE_TX_ABC_SUPPORT 0
#define TRICE_TX_X0_COUNTED_BUFFER_SUPPORT 0

// The example uses the application-provided snprintf hook and needs no binary
// UART, RTT, auxiliary, framing, or encryption output.
#define TRICE_DIRECT_OUTPUT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_CONFIG_WARNINGS 0

#endif // TRICE_CONFIG_H_
