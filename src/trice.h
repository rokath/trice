// SPDX-License-Identifier: MIT

//! \file trice.h
//! \brief trice declarations and macros.


#ifndef TRICE_H_
#define TRICE_H_

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#undef ID //!< avoid name clashes in case ID was used by an other library
#undef Id //!< avoid name clashes in case Id was used by an other library
#undef id //!< avoid name clashes in case id was used by an other library
#undef iD //!< avoid name clashes in case iD was used by an other library

#define TRICE_UNUSED(x) (void)(x); //!< https://stackoverflow.com/questions/3599160/how-can-i-suppress-unused-parameter-warnings-in-c

// lint -e529  Warning 529: Symbol '_SEGGER_RTT__LockState' not subsequently referenced
// lint -e629  Warning 629: static class for function '' is non standard
// lint -emacro( 701, TRICE* ) Info 701: Shift left of signed quantity (int)
// lint -emacro( 123, TRICE* )
// lint -emacro( 571, TRICE* )
// lint -emacro( 572, TRICE* )
// lint -emacro( 578, TRICE* )
// lint -emacro( 648, TRICE* )
// lint -emacro( 665, TRICE* )
// lint -emacro( 666, TRICE* )
// lint -emacro( 670, TRICE* )
// lint -emacro( 694, TRICE* )
// lint -emacro( 717, TRICE* )
// lint -emacro( 718, TRICE* )
// lint -emacro( 732, TRICE* )
// lint -emacro( 734, TRICE* )
// lint -emacro( 736, TRICE* )
// lint -emacro( 740, TRICE* )
// lint -emacro( 746, TRICE* )
// lint -emacro( 747, TRICE* )
// lint -emacro( 774, TRICE* )
// lint -emacro( 778, TRICE* )
// lint -emacro( 826, TRICE* )
// lint -emacro( 830, TRICE* )
// lint -emacro( 831, TRICE* )
// lint -emacro( 835, TRICE* )
// lint -emacro( 545, SCOPY, DCOPY )
// lint -ecall( 666, aFloat, aDouble )
// lint -efunc( 666, aFloat, aDouble )
// lint -esym( 666, aFloat, aDouble )
// lint -emacro( 732, SCOPY )
// lint -emacro( 717, DCOPY, SCOPY )
// lint -emacro( 732, DCOPY )

// helper macros (the numbers are 16-bit random values)

//! TRICE_SINGLE_PACK_MODE is the recommended TRICE_DEFERRED_TRANSFER_MODE. It packs each trice in a separate TCOBS package with a following 0-delimiter byte.
//! Single trices need a bit more transfer data. In case of a data disruption, only a single trice messages can get lost.
#define TRICE_SINGLE_PACK_MODE 5706U

//! TRICE_MULTI_PACK_MODE packs all trices of a buffer in a single TCOBS package and a following 0-delimiter byte.
//! Grouped trices need a bit less transfer data. In case of a data disruption, multiple trice messages can get lost.
//! Olny relevant when TRICE_DOUBLE_BUFFER is selected
#define TRICE_MULTI_PACK_MODE 2482U

//! With TRICE_BUFFER == TRICE_STACK_BUFFER  the internal macro TRICE_PUT writes to the stack.
//! This is direct logging. This reduces memory needs if only one stack is used.
#define TRICE_STACK_BUFFER 2063U

//! With TRICE_BUFFER == TRICE_STATIC_BUFFER the internal macro TRICE_PUT writes to a static buffer.
//! This reduces memory needs if many stacks are used.
#define TRICE_STATIC_BUFFER 1404U

//! With TRICE_BUFFER == TRICE_DOUBLE_BUFFER the internal macro TRICE_PUT writes to a double buffer half.
//! This is deferred logging using more space but the TRICE macros are executed faster.
#define TRICE_DOUBLE_BUFFER 368U

//! With TRICE_BUFFER == TRICE_RING_BUFFER the internal macro TRICE_PUT writes to a ring buffer segment.
//! This is deferred logging using less space but the TRICE macros are executed a bit slower.
#define TRICE_RING_BUFFER 9049U

//! TRICE_FRAMING_TCOBS is recommended for trice transfer over UART.
#define TRICE_FRAMING_TCOBS 7584U

//! TRICE_FRAMING_COBS is recommended for encrypted trices.
#define TRICE_FRAMING_COBS 4234U

//! TRICE_FRAMING_NONE is recommended for RTT in direct mode. One trice costs about 100 clocks and is completely done.
#define TRICE_FRAMING_NONE 787U

#include "triceConfig.h"        // Project specific settings are overwriting the default settings.
#include "triceDefaultConfig.h" // default settings

#ifndef TRICE_WEAK // user can define TRICE_WEAK for special cases
#ifdef WEAK
#define TRICE_WEAK WEAK // use existing weak
#else                   // #ifdef WEAK
#if defined(__GNUC__) || defined(__clang__)
#define TRICE_WEAK __attribute__((weak))
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION) || defined(__IAR_SYSTEMS_ICC__)
#define TRICE_WEAK __weak
#else              // for example MSVC (Visual C++)
#define TRICE_WEAK // fallback
#endif
#endif // #else // #ifdef WEAK
#endif // #ifndef TRICE_WEAK

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// These converter functions need to be visible in the TRICE_OFF == 1 case to avoid compiler warnings then.
//

// pre C99
//  // aFloat returns passed float value x as bit pattern in a uint32_t type.
//  TRICE_INLINE uint32_t aFloat( float x ){
//      union {
//          float f;
//          uint32_t u;
//      } t;
//      t.f = x;
//      return t.u;
//  }

//  // aFloat returns passed float value x as bit pattern in a uint32_t type.
//  // See also \#571
//  TRICE_INLINE uint32_t aFloat(float f) {
//  	union {
//  		float from_aFloat;
//  		uint32_t to_aFloat;
//  	} pun = {.from_aFloat = f};
//  	return pun.to_aFloat;
//  }

// aFloat returns passed float value x as bit pattern in a uint32_t type.
// See also \#571
TRICE_INLINE uint32_t aFloat(float f) {
	union {
		float from;
		uint32_t to;
	} pun;
	pun.from = f;
	return pun.to;
}

#if (TRICE_64_BIT_SUPPORT == 1)
// aDouble returns passed double value x as bit pattern in a uint64_t type.
TRICE_INLINE uint64_t aDouble(double x) {
	union {
		double d;
		uint64_t u;
	} t_aDouble;
	t_aDouble.d = x;
	return t_aDouble.u;
}
#endif // #if (TRICE_64_BIT_SUPPORT == 1)

// Just in case you are receiving Trice messages containing uint32_t values to be interpreted as float:
//
//  // asFloat returns passed uint32_t value x bit pattern as float type.
//  TRICE_INLINE float asFloat(uint32_t x) {
//  	union {
//  		uint32_t from;
//  		float to;
//  	} pun = {.from = x};
//  	return pun.to;
//  }

//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Do not generate trice code
// - when defining TRICE_CLEAN to 1 inside "triceConfig.h" or
// - when defining TRICE_OFF to 1 before including "trice.h".
// It is possible to `#define TRICE_OFF 1` inside the project settings to disable all Trice code.
#if ((defined(TRICE_OFF) && TRICE_OFF == 1)) || ((defined(TRICE_CLEAN) && TRICE_CLEAN == 1))

// When the user defines TRICE_CLEAN to 0 or 1 inside triceConfig.h, this value is set to 0 with "trice insert" and to 1 with "trice clean".
// This gives the option to silence editor warnings in the "trice clean" state.
// To avoid a re-build on files including trice.h, the Trice cache will be helpful. See issue #488.
#include "triceOff.h"

#else // #if ((defined(TRICE_OFF) && TRICE_OFF == 1)) || ((defined(TRICE_CLEAN) && TRICE_CLEAN == 1))
/***************************************************************************************************/
/**/
/**/

#include "triceOn.h"

#if (TRICE_DIRECT_SEGGER_RTT_8BIT_WRITE == 1) || (TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE == 1) || (TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE == 1)

#define SEGGER_RTT

#endif

#if defined(SEGGER_RTT) || (USE_SEGGER_RTT_LOCK_UNLOCK_MACROS == 1)

#include "SEGGER_RTT.h"

#endif

// global defines

#define TRICE_TYPE_X0 0 //!< TRICE_TYPE_X0 is an unspecified trice (reserved)
#define TRICE_TYPE_S0 1 //!< TRICE_TYPE_S0 is a trice without stamp.
#define TRICE_TYPE_S2 2 //!< TRICE_TYPE_S2 is a trice with 16-bit stamp.
#define TRICE_TYPE_S4 3 //!< TRICE_TYPE_S4 is a trice with 32-bit stamp.

// global variables:

extern uint32_t* triceSingleBufferStartWritePosition;
extern unsigned TricesCountRingBuffer;
extern char triceCommandBuffer[];
extern int triceCommandFlag;
extern uint_fast32_t TriceCycle;
extern unsigned RTT0_writeDepthMax;
extern unsigned TriceErrorCount;

#if (TRICE_BUFFER == TRICE_RING_BUFFER)

extern uint32_t* const TriceRingBufferStart;
extern uint32_t* const TriceRingBufferLimit;

extern uint32_t* const TriceRingBufferProtectLimit;

#if (TRICE_DIAGNOSTICS == 1)

extern int TriceRingBufferDepthMax;

#endif

#if TRICE_RING_BUFFER_OVERFLOW_WATCH == 1

void TriceInitRingBufferMargins(void);
void WatchRingBufferMargins(void);

#endif

#endif // #if (TRICE_BUFFER == TRICE_RING_BUFFER)

#if (TRICE_DIAGNOSTICS == 1)

extern int TriceDataOffsetDepthMax;
extern unsigned TriceSingleMaxWordCount;
extern unsigned TriceDynStringBufTruncateCount;
extern unsigned TriceHalfBufferDepthMax;

#if TRICE_PROTECT == 1

extern unsigned TriceDirectOverflowCount;
extern unsigned TriceDeferredOverflowCount;

#endif // #if TRICE_PROTECT == 1

#define TRICE_DYN_STRG_BUF_TRUNCATE_COUNT_INCREMENT() \
	do {                                              \
		TriceDynStringBufTruncateCount++;             \
	} while (0)

#else // #if (TRICE_DIAGNOSTICS == 1)

#define TRICE_DYN_STRG_BUF_TRUNCATE_COUNT_INCREMENT()

#endif // #else // #if (TRICE_DIAGNOSTICS == 1)

#if (TRICE_BUFFER == TRICE_RING_BUFFER) || (TRICE_BUFFER == TRICE_DOUBLE_BUFFER)

extern uint32_t* TriceBufferWritePosition;

#endif

//! TRICE_BUFFER_SIZE is
//! \li the additional needed stack space when TRICE_BUFFER == TRICE_STACK_BUFFER
//! \li the statically allocated buffer size when TRICE_BUFFER == TRICE_STATIC_BUFFER
//! \li the value before Ringbuffer wraps, when TRICE_BUFFER == TRICE_RING_BUFFER
//!
//! The trice buffer needs 4 additional scratch bytes, when the longest possible
//! trice gets formally the padding space cleared.
#define TRICE_BUFFER_SIZE (TRICE_DATA_OFFSET + TRICE_SINGLE_MAX_SIZE + 4)

#if TRICE_CYCLE_COUNTER == 1

#define TRICE_CYCLE (TriceCycle++ & 0xFF) //!< TRICE_CYCLE is the trice cycle counter as 8 bit count 0-255.
#define TRICE_CYCLE_INCREMENT (TriceCycle++)

#else // #if TRICE_CYCLE_COUNTER == 1

#define TRICE_CYCLE ((uint_fast32_t) 0xC0) //!< TRICE_CYCLE is no trice cycle counter, just a static value.

#endif // #else // #if TRICE_CYCLE_COUNTER == 1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "Declare" endian dependent macros, needed in trice8.h, trice16.h, trice32.h, trice64.h:
//
#define TRICE_HTOTS(x) TRICE_HTOTS(x)
#define TRICE_HTOTL(x) TRICE_HTOTL(x)
#define TRICE_TTOHS(x) TRICE_TTOHS(x)
#define TRICE_PUT16_1616(x, ts) TRICE_PUT16_1616(x, ts)
#define TRICE_PUT64(x) TRICE_PUT64(x)
#define TRICE_SWAPINT16(x) TRICE_SWAPINT16(x)
#define TRICE_SWAPINT32(x) TRICE_SWAPINT32(x)
#define idLH idLH
#define IdLH IdLH
#define IDLH IDLH
// #define tsL tsL
// #define tsH tsH
// #define tsHH tsHH
// #define tsHL tsHL
// #define tsLH tsLH
// #define tsLL tsLL
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __cplusplus // see \#571
#ifdef __GNUC__ // see \#594
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#endif // __GNUC__
#endif
//! TRICE_PUT16 copies 16-bit value x into the Trice buffer.
#define TRICE_PUT16(x)                                             \
	do {                                                             \
		uint16_t* p_TRICE_PUT16 = (uint16_t*)TriceBufferWritePosition; \
		*p_TRICE_PUT16++ = TRICE_HTOTS(x);                             \
		TriceBufferWritePosition = (uint32_t*)p_TRICE_PUT16;           \
	} while (0)
#include "trice8.h"
#include "trice16.h"
#include "trice32.h"
#if (TRICE_64_BIT_SUPPORT == 1)
#include "trice64.h"
#endif // (TRICE_64_BIT_SUPPORT == 1)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Endian dependent macros and code:
//
#undef TRICE_HTOTS
#undef TRICE_HTOTL
#undef TRICE_TTOHS
#undef TRICE_PUT16_1616
#undef TRICE_PUT64
#undef TRICE_SWAPINT16
#undef TRICE_SWAPINT32
#undef idLH
#undef IdLH
#undef IDLH
// #undef tsL
// #undef tsH
// #undef tsHH
// #undef tsHL
// #undef tsLH
// #undef tsLL

#if (__STDC_VERSION__ >= 202000) //! C23 standard specification for endianess detection (Note N3022)

// https://github.com/rokath/trice/pull/505

#if __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__
#define TRICE_MCU_IS_BIG_ENDIAN 0
#else
#define TRICE_MCU_IS_BIG_ENDIAN 1
#endif // __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__

#else

//! Try detect endianess by using compilers macros
#if (defined(BYTE_ORDER) && defined(ORDER_LITTLE_ENDIAN) && BYTE_ORDER == ORDER_LITTLE_ENDIAN) ||         \
    (defined(__BYTE_ORDER) && defined(__ORDER_LITTLE_ENDIAN) && __BYTE_ORDER == __ORDER_LITTLE_ENDIAN) || \
    (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define TRICE_MCU_IS_BIG_ENDIAN 0

#elif (defined(BYTE_ORDER) && defined(ORDER_BIG_ENDIAN) && BYTE_ORDER == ORDER_BIG_ENDIAN) ||       \
    (defined(__BYTE_ORDER) && defined(__ORDER_BIG_ENDIAN) && __BYTE_ORDER == __ORDER_BIG_ENDIAN) || \
    (defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define TRICE_MCU_IS_BIG_ENDIAN 1

#else

#ifndef TRICE_MCU_IS_BIG_ENDIAN
#error Bytes order not supported or not detected, set TRICE_MCU_IS_BIG_ENDIAN to 0 or 1 in your triceConfig.h file.
#endif // #ifndef TRICE_MCU_IS_BIG_ENDIAN

#endif // __BYTE_ORDER__

#endif // __STDC_VERSION__

#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == TRICE_MCU_IS_BIG_ENDIAN

// TRICE_REVERSE == 0 uses no byte swapping inside the Trice macros resulting in less code and faster execution.
#include "triceMcuOrder.h"
#include "trice8McuOrder.h"
#include "trice16McuOrder.h"
#include "trice32McuOrder.h"
#if (TRICE_64_BIT_SUPPORT == 1)
#include "trice64McuOrder.h"
#endif // (TRICE_64_BIT_SUPPORT == 1)

#else // #if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == TRICE_MCU_IS_BIG_ENDIAN

// TRICE_REVERSE == 1 causes byte swapping inside the Trice macros resulting in more code and slower execution.
#include "triceMcuReverse.h"
#include "trice8McuReverse.h"
#include "trice16McuReverse.h"
#include "trice32McuReverse.h"
#if (TRICE_64_BIT_SUPPORT == 1)
#include "trice64McuReverse.h"
#endif // (TRICE_64_BIT_SUPPORT == 1)

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == TRICE_MCU_IS_BIG_ENDIAN

//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if TRICE_DIAGNOSTICS == 1

#define TRICE_DIAGNOSTICS_SINGLE_BUFFER_KEEP_START \
	triceSingleBufferStartWritePosition = TriceBufferWritePosition;

#define TRICE_DIAGNOSTICS_SINGLE_BUFFER                                                                        \
	do {                                                                                                       \
		unsigned wordCount = TriceBufferWritePosition - triceSingleBufferStartWritePosition;                   \
		TriceSingleMaxWordCount = (wordCount < TriceSingleMaxWordCount) ? TriceSingleMaxWordCount : wordCount; \
	} while (0);

#define TRICE_DIAGNOSTICS_SINGLE_BUFFER_USING_WORD_COUNT                                                       \
	do {                                                                                                       \
		TriceSingleMaxWordCount = (wordCount < TriceSingleMaxWordCount) ? TriceSingleMaxWordCount : wordCount; \
	} while (0);

#else // #if TRICE_DIAGNOSTICS == 1

#define TRICE_DIAGNOSTICS_SINGLE_BUFFER_KEEP_START
#define TRICE_DIAGNOSTICS_SINGLE_BUFFER
#define TRICE_DIAGNOSTICS_SINGLE_BUFFER_USING_WORD_COUNT

#endif // #else // #if TRICE_DIAGNOSTICS == 1

// clang-format off

#ifndef TRICE_ENTER

	#if TRICE_BUFFER == TRICE_STACK_BUFFER

		//! TRICE_ENTER is the start of TRICE macro.
		#define TRICE_ENTER                                                                                           \
			TRICE_ENTER_CRITICAL_SECTION {                                                                            \
				{                                                                                                     \
					uint32_t triceSingleBuffer[TRICE_BUFFER_SIZE >> 2]; /* no = {0} here for speed */                 \
					triceSingleBufferStartWritePosition = &triceSingleBuffer[TRICE_DATA_OFFSET >> 2]; \
					uint32_t* TriceBufferWritePosition = triceSingleBufferStartWritePosition;

	#endif // #if TRICE_BUFFER == TRICE_STACK_BUFFER

	#if TRICE_BUFFER == TRICE_STATIC_BUFFER

		//! TRICE_ENTER is the start of TRICE macro.
		#define TRICE_ENTER                \
			TRICE_ENTER_CRITICAL_SECTION { \
				{                          \
					uint32_t* TriceBufferWritePosition = triceSingleBufferStartWritePosition;

	#endif // #if TRICE_BUFFER == TRICE_STATIC_BUFFER

	#if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUTPUT == 1)

		#if TRICE_PROTECT == 1

			//! TRICE_ENTER is the start of TRICE macro.
			#define TRICE_ENTER                \
				TRICE_ENTER_CRITICAL_SECTION { \
					if (TriceEnoughSpace()) {  \
						triceSingleBufferStartWritePosition = TriceBufferWritePosition;

		#else //  #if TRICE_PROTECT == 1

			//! TRICE_ENTER is the start of TRICE macro.
			#define TRICE_ENTER                \
				TRICE_ENTER_CRITICAL_SECTION { \
					{                          \
						triceSingleBufferStartWritePosition = TriceBufferWritePosition;

		#endif // #else //  #if TRICE_PROTECT == 1

	#endif // #if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUTPUT == 1)

	#if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUTPUT == 0)

		#if TRICE_PROTECT == 1

			//! TRICE_ENTER is the start of TRICE macro.
			#define TRICE_ENTER                \
				TRICE_ENTER_CRITICAL_SECTION { \
					if (TriceEnoughSpace()) {  \
						TRICE_DIAGNOSTICS_SINGLE_BUFFER_KEEP_START

		#else //  #if TRICE_PROTECT == 1

			//! TRICE_ENTER is the start of TRICE macro.
			#define TRICE_ENTER                \
				TRICE_ENTER_CRITICAL_SECTION { \
					{                          \
						TRICE_DIAGNOSTICS_SINGLE_BUFFER_KEEP_START

		#endif // #else //  #if TRICE_PROTECT == 1

	#endif // #if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUTPUT == 0)

	#if (TRICE_BUFFER == TRICE_RING_BUFFER) && (TRICE_DIRECT_OUTPUT == 1)

		#if TRICE_PROTECT == 1

			//! TRICE_ENTER is the start of TRICE macro.
			#define TRICE_ENTER                                                                                                                                                     \
				TRICE_ENTER_CRITICAL_SECTION {                                                                                                                                      \
				/* The TriceBufferWritePosition stays unchanged, when there is enough space for the next trice at the current write position.*/                                 \
				/* Because the the size of the next trice message is unknown here, the biggest value is assumed, that is TRICE_SINGLE_MAX_SIZE bytes. */                        \
				/* If this space is not given anymore, the `TriceBufferWritePosition` is reset to the start address of the ring buffer. */                                      \
				TriceBufferWritePosition = TriceBufferWritePosition <= TriceRingBufferProtectLimit ? TriceBufferWritePosition : TriceRingBufferStart; \
				if (TriceEnoughSpace()) {                                                                                                                                           \
					triceSingleBufferStartWritePosition = TriceBufferWritePosition;                                                                             \
					TricesCountRingBuffer++; // Because TRICE macros are an atomic instruction normally, this can be done here.

		#else //  #if TRICE_PROTECT == 1

			//! TRICE_ENTER is the start of TRICE macro.
			#define TRICE_ENTER                                                                                                                                                     \
				TRICE_ENTER_CRITICAL_SECTION {                                                                                                                                      \
					{                                                                                                                                                               \
						/* The TriceBufferWritePosition stays unchanged, when there is enough space for the next trice at the current write position.*/                             \
						/* Because the the size of the next trice message is unknown here, the biggest value is assumed, that is TRICE_BUFFER_SIZE bytes. */                        \
						/* If this space is not given anymore, the `TriceBufferWritePosition` is reset to the start address of the ring buffer. */                                  \
						/* This implementation is a bit different than a ring buffer is usually implemented. */                                                                     \
						TriceBufferWritePosition = TriceBufferWritePosition <= TriceRingBufferProtectLimit ? TriceBufferWritePosition : TriceRingBufferStart; \
						triceSingleBufferStartWritePosition = TriceBufferWritePosition;                                                                             \
						TricesCountRingBuffer++; // Because TRICE macros are an atomic instruction normally, this can be done here.

		#endif // #else //  #if TRICE_PROTECT == 1

	#endif // #if TRICE_BUFFER == TRICE_RING_BUFFER && (TRICE_DIRECT_OUTPUT == 1)

	#if (TRICE_BUFFER == TRICE_RING_BUFFER) && (TRICE_DIRECT_OUTPUT == 0)

		#if TRICE_PROTECT == 1

			//! TRICE_ENTER is the start of TRICE macro.
			#define TRICE_ENTER                                                                                                                                                     \
				TRICE_ENTER_CRITICAL_SECTION {                                                                                                                                      \
					TriceBufferWritePosition = TriceBufferWritePosition <= TriceRingBufferProtectLimit ? TriceBufferWritePosition : TriceRingBufferStart; \
					if (TriceEnoughSpace()) {                                                                                                                                       \
						TRICE_DIAGNOSTICS_SINGLE_BUFFER_KEEP_START                                                                                                                  \
						TricesCountRingBuffer++; // Because TRICE macros are an atomic instruction normally, this can be done here.

		#else //  #if TRICE_PROTECT == 1

			//! TRICE_ENTER is the start of TRICE macro.
			#define TRICE_ENTER                                                                                                                                                     \
				TRICE_ENTER_CRITICAL_SECTION {                                                                                                                                      \
					{                                                                                                                                                               \
						TriceBufferWritePosition = TriceBufferWritePosition <= TriceRingBufferProtectLimit ? TriceBufferWritePosition : TriceRingBufferStart; \
						TRICE_DIAGNOSTICS_SINGLE_BUFFER_KEEP_START                                                                                                                  \
						TricesCountRingBuffer++; // Because TRICE macros are an atomic instruction normally, this can be done here.

		#endif // #else //  #if TRICE_PROTECT == 1

	#endif // #if TRICE_BUFFER == TRICE_RING_BUFFER && (TRICE_DIRECT_OUTPUT == 0)

#endif // #ifndef TRICE_ENTER

#ifndef TRICE_LEAVE

	#if TRICE_DIRECT_OUTPUT == 1

		//! TRICE_LEAVE is the end of TRICE macro. It is the same for all buffer variants.
		#define TRICE_LEAVE                                                                                          \
			/* wordCount is the amount of steps, the TriceBufferWritePosition went forward for the actual trice.  */ \
			/* The last written uint32_t trice value can contain 1 to 3 padding bytes. */                            \
			unsigned wordCount = (unsigned)(TriceBufferWritePosition - triceSingleBufferStartWritePosition);         \
			TRICE_DIAGNOSTICS_SINGLE_BUFFER_USING_WORD_COUNT                                                         \
			TriceNonBlockingDirectWrite(triceSingleBufferStartWritePosition, wordCount);                             \
			}                                                                                                        \
			}                                                                                                        \
			TRICE_LEAVE_CRITICAL_SECTION

	#else // #if TRICE_DIRECT_OUTPUT == 1

		//! TRICE_LEAVE is the end of TRICE macro. It is the same for all buffer variants.
		#define TRICE_LEAVE                 \
			TRICE_DIAGNOSTICS_SINGLE_BUFFER \
			}                               \
			}                               \
			TRICE_LEAVE_CRITICAL_SECTION

	#endif // #else  //#if TRICE_DIRECT_OUTPUT == 1

#endif // #ifndef TRICE_LEAVE

#ifndef TRICE_PUT

//! TRICE_PUT writes x as 32-bit value into the Trice buffer without changing the endianness.
//! This macro does not use internally TRICE_HTOTL and therefore it is usable without speed loss, when TRICE_REVERSE == 1.
//! In that case the byte order needs to be provided by the written code.
#define TRICE_PUT(x)              \
	do {                                 \
		*TriceBufferWritePosition++ = (uint32_t)(x); \
	} while (0);

#endif

#ifndef TRICE_PUT_BUFFER

	//! TRICE_PUT_BUFFER copies a buffer into the TRICE buffer.
	#define TRICE_PUT_BUFFER(buf, len)                                                               \
		do {                                                                                         \
			memcpy(TriceBufferWritePosition, buf, len);                                              \
			unsigned len4 = ((unsigned)(len) + 3u) & ~3u;                                            \
			memset((uint8_t*)TriceBufferWritePosition + (len), 0, len4 - (len)); /*clear padding space*/ \
			TriceBufferWritePosition += len4 >> 2;                                                   \
		} while (0)

#endif

// clang-format on

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//

// The zero-value dispatcher paths deliberately forward to the compact 32-bit
// helper macros instead of passing the format string through wrapper functions.
//
// Why this is done:
// - In inserted source files the usual spelling is `trice(iD(...), "msg")`,
//   `Trice(iD(...), "msg")` or `TRice(iD(...), "msg")`.
// - If these forms call a real function taking `const char* pFmt`, the caller
//   must still materialize the string literal as an argument.
// - Without LTO or other strong interprocedural optimization, some compilers
//   then keep the string literal in the object file although the callee does
//   not use it semantically.
// - Routing the `_0` dispatcher directly to `*32_0` keeps the code-size
//   advantage of shared helpers while letting the compiler discard the format
//   string at the call site.
//
// Why the 32-bit helpers are used:
// - `trice`, `Trice` and `TRice` only differ in the stamp-producing `tid`
//   expression, not in the payload layout for a zero-value message.
// - The existing `trice32_0`, `Trice32_0` and `TRice32_0` helpers already
//   encode the desired no-value payload compactly.
//
// A deliberate consequence of this design is that the public `trice0`,
// `Trice0` and `TRice0` spellings are no longer emitted as linker-visible
// symbols. That is acceptable here because their compatibility role is served
// by fixed-arity macros below, which work in strict non-GNU C modes as well.
#define TRICE_0 TRICE32_0 //!< Only the format string without parameter values.
#define TRice_0 TRice32_0 //!< Only the format string without parameter values.
#define Trice_0 Trice32_0 //!< Only the format string without parameter values.
#define trice_0 trice32_0 //!< Only the format string without parameter values.

// These explicit zero-value spellings are kept as fixed-arity macros.
//
// Why keep them:
// - In strict C modes (`-std=c11`, `-std=c99`, ...), empty variadic argument
//   lists are notoriously fragile across older or non-GNU-compatible compilers.
// - The explicit `trice0(...)`, `Trice0(...)`, `TRice0(...)` forms therefore
//   remain useful as a portable source-level fallback.
//
// Why these are macros instead of functions:
// - The format string must disappear before the compiler sees a function call.
// - If a real function with signature `(uint16_t tid, const char* pFmt)` were
//   called here, the caller would again have to preserve the string literal as
//   a call argument and the original optimization problem would reappear.
//
// The `pFmt` parameter is intentionally accepted for source compatibility but
// ignored by the expansion.
#define trice0(tid, pFmt) trice32_0(tid, pFmt)
#define Trice0(tid, pFmt) Trice32_0(tid, pFmt)
#define TRice0(tid, pFmt) TRice32_0(tid, pFmt)

#ifndef TRICE_N

//! TRICE_N writes id and buffer of size len.
//! \param tid trice identifier
//! \param pFmt formatstring for trice (ignored here but used by the trice tool), could contain any add on information. The trice tool "sees" the "TRICE_N" and can handle that.
//! \param dynString 0-terminated runtime generated string
//! After the 4 byte trice message header are following 4 bytes coding n (only 2 used) and the buffer
//! transfer format:
//! idH    idL    len    cycle <- trice header
//! n0     n1     n2     n3    <- payload count without paddings bytes (transmittable len)
//! c0     c1     c2     c3    <- buffer
//! ...                        <- buffer
//! cLen-3 cLen-2 cLen-1 cLen  <- buffer ending with maybe 1-3 undetermined padding bytes
//
// todo: for some reason this macro is not working well wit name len instead of len_, probably when injected len as value.
//
#define TRICE_N(tid, pFmt, buf, n)                                                                                                           \
	do {                                                                                                                                     \
		TRICE_UNUSED(pFmt);                                                                                                                  \
		uint32_t limit_TRICE_N = TRICE_SINGLE_MAX_SIZE - 12; /* 12 = head(2) + max timestamp size(4) + count(2) + max 3 zeroes, we take 4 */ \
		uint32_t len_TRICE_N = (uint32_t)(n);                /* n could be a constant */                                                     \
		if (len_TRICE_N > limit_TRICE_N) {                                                                                                   \
			TRICE_DYN_STRG_BUF_TRUNCATE_COUNT_INCREMENT();                                                                                   \
			len_TRICE_N = limit_TRICE_N;                                                                                                     \
		}                                                                                                                                    \
		TRICE_ENTER tid;                                                                                                                     \
		if (len_TRICE_N <= 127) {                                                                                                            \
			TRICE_CNTC(len_TRICE_N);                                                                                                         \
		} else {                                                                                                                             \
			TRICE_LCNT(len_TRICE_N);                                                                                                         \
		}                                                                                                                                    \
		TRICE_PUT_BUFFER(buf, len_TRICE_N);                                                                                                  \
		TRICE_LEAVE                                                                                                                          \
	} while (0)

void triceN(int tid, char const* fmt, const void* buf, uint32_t n);
void TriceN(int tid, char const* fmt, const void* buf, uint32_t n);
void TRiceN(int tid, char const* fmt, const void* buf, uint32_t n);

void trice8B(int tid, char const* fmt, const void* buf, uint32_t n);
void Trice8B(int tid, char const* fmt, const void* buf, uint32_t n);
void TRice8B(int tid, char const* fmt, const void* buf, uint32_t n);

void trice16B(int tid, char const* fmt, const void* buf, uint32_t n);
void Trice16B(int tid, char const* fmt, const void* buf, uint32_t n);
void TRice16B(int tid, char const* fmt, const void* buf, uint32_t n);

void trice32B(int tid, char const* fmt, const void* buf, uint32_t n);
void Trice32B(int tid, char const* fmt, const void* buf, uint32_t n);
void TRice32B(int tid, char const* fmt, const void* buf, uint32_t n);

void trice64B(int tid, char const* fmt, const void* buf, uint32_t n);
void Trice64B(int tid, char const* fmt, const void* buf, uint32_t n);
void TRice64B(int tid, char const* fmt, const void* buf, uint32_t n);

void trice8F(int tid, char const* fmt, const void* buf, uint32_t n);
void Trice8F(int tid, char const* fmt, const void* buf, uint32_t n);
void TRice8F(int tid, char const* fmt, const void* buf, uint32_t n);

void trice16F(int tid, char const* fmt, const void* buf, uint32_t n);
void Trice16F(int tid, char const* fmt, const void* buf, uint32_t n);
void TRice16F(int tid, char const* fmt, const void* buf, uint32_t n);

void trice32F(int tid, char const* fmt, const void* buf, uint32_t n);
void Trice32F(int tid, char const* fmt, const void* buf, uint32_t n);
void TRice32F(int tid, char const* fmt, const void* buf, uint32_t n);

void trice64F(int tid, char const* fmt, const void* buf, uint32_t n);
void Trice64F(int tid, char const* fmt, const void* buf, uint32_t n);
void TRice64F(int tid, char const* fmt, const void* buf, uint32_t n);

#endif // #ifndef TRICE_N

#ifndef TRICE_S

//! TRICE_S writes id and runtimeGeneratedString.
//! \param tid trice identifier
//! \param pFmt format string for trice (ignored here but used by the trice tool)
//! \param runtimeGeneratedString 0-terminated runtime generated string
#define TRICE_S(tid, pFmt, runtimeGeneratedString)                \
	do {                                                          \
		uint32_t ssiz_TRICE_S = strlen(runtimeGeneratedString);   \
		TRICE_N(tid, pFmt, runtimeGeneratedString, ssiz_TRICE_S); \
	} while (0)

// These fixed-arity source-level forms are intentionally macros instead of
// wrapper functions taking `fmt`.
//
// Why this matters:
// - The runtime string itself must stay visible to the compiler because it is
//   transmitted as payload and therefore affects both code and data generation.
// - The format string, however, is only tool metadata and should disappear as
//   early as possible.
// - If `triceS(...)`, `TriceS(...)` or `TRiceS(...)` called a real function
//   taking `const char* fmt`, some compilers without LTO would still keep the
//   format literal in readonly data just because it appeared as a call
//   argument.
// - A pure macro expansion fixes that optimization issue, but it also repeats
//   the full runtime-string encoding sequence at every call site.
// - The compromise used here is therefore:
//   1. the public spelling stays a macro so `fmt` vanishes before code
//      generation
//   2. the macro forwards only `tid` and the runtime string to a compact helper
//      function that does not take `fmt`
//
// The `fmt` parameter is therefore accepted for source compatibility and tool
// processing, but it is intentionally not forwarded through a wrapper symbol
// or helper signature.
void triceSfn(uint16_t tid, const char* runtimeGeneratedString);
void TriceSfn(uint16_t tid, const char* runtimeGeneratedString);
void TRiceSfn(uint16_t tid, const char* runtimeGeneratedString);

#define triceS(tid, fmt, runtimeGeneratedString) triceSfn((uint16_t)(tid), runtimeGeneratedString)
#define TriceS(tid, fmt, runtimeGeneratedString) TriceSfn((uint16_t)(tid), runtimeGeneratedString)
#define TRiceS(tid, fmt, runtimeGeneratedString) TRiceSfn((uint16_t)(tid), runtimeGeneratedString)

#endif // #ifndef TRICE_S

//! ID writes 14-bit id with 11 as 2 most significant bits, followed by a 32-bit stamp.
//! 11iiiiiiI TT | TT (NC) | ...
//! C000 = 1100 0000 0000 0000
#define ID(n)                                                            \
	do {                                                                 \
		uint32_t ts_ID = TriceStamp32;                                   \
		TRICE_PUT16_1616((((uint16_t)0xC000) | ((uint16_t)(n))), ts_ID); \
	} while (0)

//! Id writes 14-bit id with 10 as 2 most significant bits two times, followed by a 16-bit stamp.
//! 10iiiiiiI 10iiiiiiI | TT (NC) | ...
//! 8000 = 1000 0000 0000 0000
#define Id(n)                                                                            \
	do {                                                                                 \
		uint16_t ts_Id = TriceStamp16;                                                   \
		TRICE_PUT(TRICE_HTOTL(0x80008000u | (((uint32_t)(n)) << 16) | ((uint32_t)(n)))); \
		TRICE_PUT16(ts_Id);                                                              \
	} while (0)

//! id writes 14-bit id with 01 as 2 most significant bits, followed by no stamp.
//! 01iiiiiiI (NC) | ...
//! 4000 = 0100 0000 0000 0000
#define id(n) TRICE_PUT16((uint16_t)(0x4000) | (n));

//! iD is just a code parsing helper.
#define iD(n) (n)

//! TRICE_CNTC writes 7-bit byte count and 8-bit cycle counter.
#define TRICE_CNTC(count)                                                 \
	do {                                                                  \
		uint16_t v_TRICE_CNTC = (((uint16_t)(count)) << 8) | TRICE_CYCLE; \
		TRICE_PUT16(v_TRICE_CNTC);                                        \
	} while (0)

#if TRICE_CYCLE_COUNTER == 1

//! TRICE_LCNT writes 1 as most significant bit and 15-bit byte count. It does not write the cycle counter but increments the cycle counter.
#define TRICE_LCNT(count)            \
	TRICE_PUT16((0x8000 | (count))); \
	TRICE_CYCLE_INCREMENT // increment TRICE_CYCLE but do not transmit it

#else

//! TRICE_LCNT writes 1 as most significant bit and 15-bit byte count. It does not write the cycle counter but increments the cycle counter.
#define TRICE_LCNT(count) TRICE_PUT16((0x8000 | (count))); // no TRICE_CYCLE

#endif

//! TRICE0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define TRICE0(tid, pFmt) \
	TRICE_ENTER tid;      \
	TRICE_CNTC(0);        \
	TRICE_LEAVE

// The Assert spellings follow the same hybrid pattern as the string helpers:
// - the public source form stays a macro so `msg` disappears before a real
//   function call is formed
// - compact helper functions keep the generated code centralized
//
// This avoids leaving assertion message literals in object data on compilers
// that do not optimize away unused function parameters aggressively unless LTO
// is enabled.
void triceAssertTrueFn(uint16_t idN, int flag);
void TriceAssertTrueFn(uint16_t idN, int flag);
void TRiceAssertTrueFn(uint16_t idN, int flag);

void triceAssertFalseFn(uint16_t idN, int flag);
void TriceAssertFalseFn(uint16_t idN, int flag);
void TRiceAssertFalseFn(uint16_t idN, int flag);

#define triceAssertTrue(idN, msg, flag) triceAssertTrueFn((uint16_t)(idN), flag)
#define TriceAssertTrue(idN, msg, flag) TriceAssertTrueFn((uint16_t)(idN), flag)
#define TRiceAssertTrue(idN, msg, flag) TRiceAssertTrueFn((uint16_t)(idN), flag)

#define triceAssertFalse(idN, msg, flag) triceAssertFalseFn((uint16_t)(idN), flag)
#define TriceAssertFalse(idN, msg, flag) TriceAssertFalseFn((uint16_t)(idN), flag)
#define TRiceAssertFalse(idN, msg, flag) TRiceAssertFalseFn((uint16_t)(idN), flag)

#define triceAssert(idN, msg, flag) triceAssertTrue(idN, msg, flag)
#define TriceAssert(idN, msg, flag) TriceAssertTrue(idN, msg, flag)
#define TRiceAssert(idN, msg, flag) TRiceAssertTrue(idN, msg, flag)

#define triceAssertOrReturn(idN, msg, flag) do {if (!(flag)) {trice(idN, msg); return;}} while(0)
#define TriceAssertOrReturn(idN, msg, flag) do {if (!(flag)) {Trice(idN, msg); return;}} while(0)
#define TRiceAssertOrReturn(idN, msg, flag) do {if (!(flag)) {TRice(idN, msg); return;}} while(0)

#define triceAssertOrReturnValue(idN, msg, flag, value) do {if (!(flag)) {trice(idN, msg); return (value);}} while(0)
#define TriceAssertOrReturnValue(idN, msg, flag, value) do {if (!(flag)) {Trice(idN, msg); return (value);}} while(0)
#define TRiceAssertOrReturnValue(idN, msg, flag, value) do {if (!(flag)) {TRice(idN, msg); return (value);}} while(0)

typedef void (*Write8AuxiliaryFn_t)(const uint8_t* enc, size_t encLen);
extern Write8AuxiliaryFn_t UserNonBlockingDirectWrite8AuxiliaryFn;
extern Write8AuxiliaryFn_t UserNonBlockingDeferredWrite8AuxiliaryFn;

typedef void (*Write32AuxiliaryFn_t)(const uint32_t* enc, unsigned count);
extern Write32AuxiliaryFn_t UserNonBlockingDirectWrite32AuxiliaryFn;
extern Write32AuxiliaryFn_t UserNonBlockingDeferredWrite32AuxiliaryFn;

/**/
/**/
/***************************************************************************************************/
#endif // #else // #if ((defined(TRICE_OFF) && TRICE_OFF == 1)) || ((defined(TRICE_CLEAN) && TRICE_CLEAN == 1))

#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
