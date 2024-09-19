//! \file trice8.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////
#include "trice.h"

// lint -e529  Warning 529: Symbol '_SEGGER_RTT__LockState' not subsequently referenced
// lint -e701  Info 701: Shift left of signed quantity (int)

#if TRICE_8_BIT_SUPPORT == 1 && TRICE_OFF == 0

// without stamp 8-bit values functions
#ifndef ENABLE_trice8fn_0
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 0 * 1) + 3) & ~3)
#define ENABLE_trice8fn_0 0 //!< Disables no-stamp 8-bit 0 values functions.
#else
#define ENABLE_trice8fn_0 1 //!< Enables  no-stamp 8-bit 0 values functions.
#endif
#endif

#ifndef ENABLE_trice8fn_1
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 1 * 1) + 3) & ~3)
#define ENABLE_trice8fn_1 0 //!< Disables no-stamp 8-bit 1 values functions.
#else
#define ENABLE_trice8fn_1 1 //!< Enables  no-stamp 8-bit 1 values functions.
#endif
#endif

#ifndef ENABLE_trice8fn_2
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 2 * 1) + 3) & ~3)
#define ENABLE_trice8fn_2 0 //!< Disables no-stamp 8-bit 2 values functions.
#else
#define ENABLE_trice8fn_2 1 //!< Enables  no-stamp 8-bit 2 values functions.
#endif
#endif

#ifndef ENABLE_trice8fn_3
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 3 * 1) + 3) & ~3)
#define ENABLE_trice8fn_3 0 //!< Disables no-stamp 8-bit 3 values functions.
#else
#define ENABLE_trice8fn_3 1 //!< Enables  no-stamp 8-bit 3 values functions.
#endif
#endif

#ifndef ENABLE_trice8fn_4
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 4 * 1) + 3) & ~3)
#define ENABLE_trice8fn_4 0 //!< Disables no-stamp 8-bit 4 values functions.
#else
#define ENABLE_trice8fn_4 1 //!< Enables  no-stamp 8-bit 4 values functions.
#endif
#endif

#ifndef ENABLE_trice8fn_5
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 5 * 1) + 3) & ~3)
#define ENABLE_trice8fn_5 0 //!< Disables no-stamp 8-bit 5 values functions.
#else
#define ENABLE_trice8fn_5 1 //!< Enables  no-stamp 8-bit 5 values functions.
#endif
#endif

#ifndef ENABLE_trice8fn_6
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 6 * 1) + 3) & ~3)
#define ENABLE_trice8fn_6 0 //!< Disables no-stamp 8-bit 6 values functions.
#else
#define ENABLE_trice8fn_6 1 //!< Enables  no-stamp 8-bit 6 values functions.
#endif
#endif

#ifndef ENABLE_trice8fn_7
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 7 * 1) + 3) & ~3)
#define ENABLE_trice8fn_7 0 //!< Disables no-stamp 8-bit 7 values functions.
#else
#define ENABLE_trice8fn_7 1 //!< Enables  no-stamp 8-bit 7 values functions.
#endif
#endif

#ifndef ENABLE_trice8fn_8
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 8 * 1) + 3) & ~3)
#define ENABLE_trice8fn_8 0 //!< Disables no-stamp 8-bit 8 values functions.
#else
#define ENABLE_trice8fn_8 1 //!< Enables  no-stamp 8-bit 8 values functions.
#endif
#endif

#ifndef ENABLE_trice8fn_9
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 9 * 1) + 3) & ~3)
#define ENABLE_trice8fn_9 0 //!< Disables no-stamp 8-bit 9 values functions.
#else
#define ENABLE_trice8fn_9 1 //!< Enables  no-stamp 8-bit 9 values functions.
#endif
#endif

#ifndef ENABLE_trice8fn_10
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 10 * 1) + 3) & ~3)
#define ENABLE_trice8fn_10 0 //!< Disables no-stamp 8-bit 10 values functions.
#else
#define ENABLE_trice8fn_10 1 //!< Enables  no-stamp 8-bit 10 values functions.
#endif
#endif

#ifndef ENABLE_trice8fn_11
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 11 * 1) + 3) & ~3)
#define ENABLE_trice8fn_11 0 //!< Disables no-stamp 8-bit 11 values functions.
#else
#define ENABLE_trice8fn_11 1 //!< Enables  no-stamp 8-bit 11 values functions.
#endif
#endif

#ifndef ENABLE_trice8fn_12
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 12 * 1) + 3) & ~3)
#define ENABLE_trice8fn_12 0 //!< Disables no-stamp 8-bit 12 values functions.
#else
#define ENABLE_trice8fn_12 1 //!< Enables  no-stamp 8-bit 12 values functions.
#endif
#endif

// with 16-bit-stamp 8-bit values functions (16-bit timestamps need 4 bytes for allocation reasons)
#ifndef ENABLE_Trice8fn_0
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 0 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_0 0 //!< Disables 16-bit-stamp 8-bit 0 values functions.
#else
#define ENABLE_Trice8fn_0 1 //!< Enables  16-bit-stamp 8-bit 0 values functions.
#endif
#endif

#ifndef ENABLE_Trice8fn_1
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 1 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_1 0 //!< Disables 16-bit-stamp 8-bit 1 values functions.
#else
#define ENABLE_Trice8fn_1 1 //!< Enables  16-bit-stamp 8-bit 1 values functions.
#endif
#endif

#ifndef ENABLE_Trice8fn_2
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 2 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_2 0 //!< Disables 16-bit-stamp 8-bit 2 values functions.
#else
#define ENABLE_Trice8fn_2 1 //!< Enables  16-bit-stamp 8-bit 2 values functions.
#endif
#endif

#ifndef ENABLE_Trice8fn_3
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 3 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_3 0 //!< Disables 16-bit-stamp 8-bit 3 values functions.
#else
#define ENABLE_Trice8fn_3 1 //!< Enables  16-bit-stamp 8-bit 3 values functions.
#endif
#endif

#ifndef ENABLE_Trice8fn_4
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 4 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_4 0 //!< Disables 16-bit-stamp 8-bit 4 values functions.
#else
#define ENABLE_Trice8fn_4 1 //!< Enables  16-bit-stamp 8-bit 4 values functions.
#endif
#endif

#ifndef ENABLE_Trice8fn_5
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 5 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_5 0 //!< Disables 16-bit-stamp 8-bit 5 values functions.
#else
#define ENABLE_Trice8fn_5 1 //!< Enables  16-bit-stamp 8-bit 5 values functions.
#endif
#endif

#ifndef ENABLE_Trice8fn_6
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 6 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_6 0 //!< Disables 16-bit-stamp 8-bit 6 values functions.
#else
#define ENABLE_Trice8fn_6 1 //!< Enables  16-bit-stamp 8-bit 6 values functions.
#endif
#endif

#ifndef ENABLE_Trice8fn_7
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 7 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_7 0 //!< Disables 16-bit-stamp 8-bit 7 values functions.
#else
#define ENABLE_Trice8fn_7 1 //!< Enables  16-bit-stamp 8-bit 7 values functions.
#endif
#endif

#ifndef ENABLE_Trice8fn_8
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 8 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_8 0 //!< Disables 16-bit-stamp 8-bit 8 values functions.
#else
#define ENABLE_Trice8fn_8 1 //!< Enables  16-bit-stamp 8-bit 8 values functions.
#endif
#endif

#ifndef ENABLE_Trice8fn_9
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 9 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_9 0 //!< Disables 16-bit-stamp 8-bit 9 values functions.
#else
#define ENABLE_Trice8fn_9 1 //!< Enables  16-bit-stamp 8-bit 9 values functions.
#endif
#endif

#ifndef ENABLE_Trice8fn_10
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 10 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_10 0 //!< Disables 16-bit-stamp 8-bit 10 values functions.
#else
#define ENABLE_Trice8fn_10 1 //!< Enables  16-bit-stamp 8-bit 10 values functions.
#endif
#endif

#ifndef ENABLE_Trice8fn_11
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 11 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_11 0 //!< Disables 16-bit-stamp 8-bit 11 values functions.
#else
#define ENABLE_Trice8fn_11 1 //!< Enables  16-bit-stamp 8-bit 11 values functions.
#endif
#endif

#ifndef ENABLE_Trice8fn_12
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 12 * 1) + 3) & ~3)
#define ENABLE_Trice8fn_12 0 //!< Disables 16-bit-stamp 8-bit 12 values functions.
#else
#define ENABLE_Trice8fn_12 1 //!< Enables  16-bit-stamp 8-bit 12 values functions.
#endif
#endif

// with 32-bit-stamp 8-bit values functions
#ifndef ENABLE_TRice8fn_0
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 0 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_0 0 //!< Disables 32-bit-stamp 8-bit 0 values functions.
#else
#define ENABLE_TRice8fn_0 1 //!< Enables  32-bit-stamp 8-bit 0 values functions.
#endif
#endif

#ifndef ENABLE_TRice8fn_1
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 1 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_1 0 //!< Disables 32-bit-stamp 8-bit 1 values functions.
#else
#define ENABLE_TRice8fn_1 1 //!< Enables  32-bit-stamp 8-bit 1 values functions.
#endif
#endif

#ifndef ENABLE_TRice8fn_2
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 2 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_2 0 //!< Disables 32-bit-stamp 8-bit 2 values functions.
#else
#define ENABLE_TRice8fn_2 1 //!< Enables  32-bit-stamp 8-bit 2 values functions.
#endif
#endif

#ifndef ENABLE_TRice8fn_3
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 3 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_3 0 //!< Disables 32-bit-stamp 8-bit 3 values functions.
#else
#define ENABLE_TRice8fn_3 1 //!< Enables  32-bit-stamp 8-bit 3 values functions.
#endif
#endif

#ifndef ENABLE_TRice8fn_4
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 4 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_4 0 //!< Disables 32-bit-stamp 8-bit 4 values functions.
#else
#define ENABLE_TRice8fn_4 1 //!< Enables  32-bit-stamp 8-bit 4 values functions.
#endif
#endif

#ifndef ENABLE_TRice8fn_5
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 5 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_5 0 //!< Disables 32-bit-stamp 8-bit 5 values functions.
#else
#define ENABLE_TRice8fn_5 1 //!< Enables  32-bit-stamp 8-bit 5 values functions.
#endif
#endif

#ifndef ENABLE_TRice8fn_6
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 6 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_6 0 //!< Disables 32-bit-stamp 8-bit 6 values functions.
#else
#define ENABLE_TRice8fn_6 1 //!< Enables  32-bit-stamp 8-bit 6 values functions.
#endif
#endif

#ifndef ENABLE_TRice8fn_7
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 7 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_7 0 //!< Disables 32-bit-stamp 8-bit 7 values functions.
#else
#define ENABLE_TRice8fn_7 1 //!< Enables  32-bit-stamp 8-bit 7 values functions.
#endif
#endif

#ifndef ENABLE_TRice8fn_8
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 8 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_8 0 //!< Disables 32-bit-stamp 8-bit 8 values functions.
#else
#define ENABLE_TRice8fn_8 1 //!< Enables  32-bit-stamp 8-bit 8 values functions.
#endif
#endif

#ifndef ENABLE_TRice8fn_9
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 9 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_9 0 //!< Disables 32-bit-stamp 8-bit 9 values functions.
#else
#define ENABLE_TRice8fn_9 1 //!< Enables  32-bit-stamp 8-bit 9 values functions.
#endif
#endif

#ifndef ENABLE_TRice8fn_10
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 10 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_10 0 //!< Disables 32-bit-stamp 8-bit 10 values functions.
#else
#define ENABLE_TRice8fn_10 1 //!< Enables  32-bit-stamp 8-bit 10 values functions.
#endif
#endif

#ifndef ENABLE_TRice8fn_11
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 11 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_11 0 //!< Disables 32-bit-stamp 8-bit 11 values functions.
#else
#define ENABLE_TRice8fn_11 1 //!< Enables  32-bit-stamp 8-bit 11 values functions.
#endif
#endif

#ifndef ENABLE_TRice8fn_12
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 12 * 1) + 3) & ~3)
#define ENABLE_TRice8fn_12 0 //!< Disables 32-bit-stamp 8-bit 12 values functions.
#else
#define ENABLE_TRice8fn_12 1 //!< Enables  32-bit-stamp 8-bit 12 values functions.
#endif
#endif

#if ENABLE_trice8fn_0
void trice8fn_0(uint16_t tid) {
	trice8m_0(tid);
}
#endif

#if ENABLE_trice8fn_1
void trice8fn_1(uint16_t tid, uint8_t v0) {
	trice8m_1(tid, v0);
}
#endif

#if ENABLE_trice8fn_2
void trice8fn_2(uint16_t tid, uint8_t v0, uint8_t v1) {
	trice8m_2(tid, v0, v1);
}
#endif

#if ENABLE_trice8fn_3
void trice8fn_3(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2) {
	trice8m_3(tid, v0, v1, v2);
}
#endif

#if ENABLE_trice8fn_4
void trice8fn_4(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3) {
	trice8m_4(tid, v0, v1, v2, v3);
}
#endif

#if ENABLE_trice8fn_5
void trice8fn_5(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4) {
	trice8m_5(tid, v0, v1, v2, v3, v4);
}
#endif

#if ENABLE_trice8fn_6
void trice8fn_6(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5) {
	trice8m_6(tid, v0, v1, v2, v3, v4, v5);
}
#endif

#if ENABLE_trice8fn_7
void trice8fn_7(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6) {
	trice8m_7(tid, v0, v1, v2, v3, v4, v5, v6);
}
#endif

#if ENABLE_trice8fn_8
void trice8fn_8(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7) {
	trice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7);
}
#endif

#if ENABLE_trice8fn_9
void trice8fn_9(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8) {
	trice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif

#if ENABLE_trice8fn_10
void trice8fn_10(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9) {
	trice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
#endif

#if ENABLE_trice8fn_11
void trice8fn_11(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10) {
	trice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}
#endif

#if ENABLE_trice8fn_12
void trice8fn_12(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10, uint8_t v11) {
	trice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}
#endif

#if ENABLE_Trice8fn_0
void Trice8fn_0(uint16_t tid) {
	Trice8m_0(tid);
}
#endif

#if ENABLE_Trice8fn_1
void Trice8fn_1(uint16_t tid, uint8_t v0) {
	Trice8m_1(tid, v0);
}
#endif

#if ENABLE_Trice8fn_2
void Trice8fn_2(uint16_t tid, uint8_t v0, uint8_t v1) {
	Trice8m_2(tid, v0, v1);
}
#endif

#if ENABLE_Trice8fn_3
void Trice8fn_3(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2) {
	Trice8m_3(tid, v0, v1, v2);
}
#endif

#if ENABLE_Trice8fn_4
void Trice8fn_4(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3) {
	Trice8m_4(tid, v0, v1, v2, v3);
}
#endif

#if ENABLE_Trice8fn_5
void Trice8fn_5(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4) {
	Trice8m_5(tid, v0, v1, v2, v3, v4);
}
#endif

#if ENABLE_Trice8fn_6
void Trice8fn_6(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5) {
	Trice8m_6(tid, v0, v1, v2, v3, v4, v5);
}
#endif

#if ENABLE_Trice8fn_7
void Trice8fn_7(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6) {
	Trice8m_7(tid, v0, v1, v2, v3, v4, v5, v6);
}
#endif

#if ENABLE_Trice8fn_8
void Trice8fn_8(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7) {
	Trice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7);
}
#endif

#if ENABLE_Trice8fn_9
void Trice8fn_9(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8) {
	Trice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif

#if ENABLE_Trice8fn_10
void Trice8fn_10(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9) {
	Trice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
#endif

#if ENABLE_Trice8fn_11
void Trice8fn_11(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10) {
	Trice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}
#endif

#if ENABLE_Trice8fn_12
void Trice8fn_12(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10, uint8_t v11) {
	Trice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}
#endif

#if ENABLE_TRice8fn_0
void TRice8fn_0(uint16_t tid) {
	TRice8m_0(tid);
}
#endif

#if ENABLE_TRice8fn_1
void TRice8fn_1(uint16_t tid, uint8_t v0) {
	TRice8m_1(tid, v0);
}
#endif

#if ENABLE_TRice8fn_2
void TRice8fn_2(uint16_t tid, uint8_t v0, uint8_t v1) {
	TRice8m_2(tid, v0, v1);
}
#endif

#if ENABLE_TRice8fn_3
void TRice8fn_3(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2) {
	TRice8m_3(tid, v0, v1, v2);
}
#endif

#if ENABLE_TRice8fn_4
void TRice8fn_4(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3) {
	TRice8m_4(tid, v0, v1, v2, v3);
}
#endif

#if ENABLE_TRice8fn_5
void TRice8fn_5(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4) {
	TRice8m_5(tid, v0, v1, v2, v3, v4);
}
#endif

#if ENABLE_TRice8fn_6
void TRice8fn_6(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5) {
	TRice8m_6(tid, v0, v1, v2, v3, v4, v5);
}
#endif

#if ENABLE_TRice8fn_7
void TRice8fn_7(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6) {
	TRice8m_7(tid, v0, v1, v2, v3, v4, v5, v6);
}
#endif

#if ENABLE_TRice8fn_8
void TRice8fn_8(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7) {
	TRice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7);
}
#endif

#if ENABLE_TRice8fn_9
void TRice8fn_9(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8) {
	TRice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif

#if ENABLE_TRice8fn_10
void TRice8fn_10(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9) {
	TRice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
#endif

#if ENABLE_TRice8fn_11
void TRice8fn_11(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10) {
	TRice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}
#endif

#if ENABLE_TRice8fn_12
void TRice8fn_12(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10, uint8_t v11) {
	TRice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}
#endif

#endif // #if TRICE_8_BIT_SUPPORT == 1 && TRICE_OFF == 0
