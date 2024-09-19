//! \file trice16.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////
#include "trice.h"

// lint -e529  Warning 529: Symbol '_SEGGER_RTT__LockState' not subsequently referenced
// lint -e701  Info 701: Shift left of signed quantity (int)

#if TRICE_16_BIT_SUPPORT == 1 && TRICE_OFF == 0

// no-stamp 16-bit values functions
#ifndef ENABLE_trice16fn_0
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 0 * 2) + 3) & ~3)
#define ENABLE_trice16fn_0 0 //!< Disables no-stamp 16-bit 0 values functions.
#else
#define ENABLE_trice16fn_0 1 //!< Enables  no-stamp 16-bit 0 values functions.
#endif
#endif

#ifndef ENABLE_trice16fn_1
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 1 * 2) + 3) & ~3)
#define ENABLE_trice16fn_1 0 //!< Disables no-stamp 16-bit 1 values functions.
#else
#define ENABLE_trice16fn_1 1 //!< Enables  no-stamp 16-bit 1 values functions.
#endif
#endif

#ifndef ENABLE_trice16fn_2
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 2 * 2) + 3) & ~3)
#define ENABLE_trice16fn_2 0 //!< Disables no-stamp 16-bit 2 values functions.
#else
#define ENABLE_trice16fn_2 1 //!< Enables  no-stamp 16-bit 2 values functions.
#endif
#endif

#ifndef ENABLE_trice16fn_3
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 3 * 2) + 3) & ~3)
#define ENABLE_trice16fn_3 0 //!< Disables no-stamp 16-bit 3 values functions.
#else
#define ENABLE_trice16fn_3 1 //!< Enables  no-stamp 16-bit 3 values functions.
#endif
#endif

#ifndef ENABLE_trice16fn_4
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 4 * 2) + 3) & ~3)
#define ENABLE_trice16fn_4 0 //!< Disables no-stamp 16-bit 4 values functions.
#else
#define ENABLE_trice16fn_4 1 //!< Enables  no-stamp 16-bit 4 values functions.
#endif
#endif

#ifndef ENABLE_trice16fn_5
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 5 * 2) + 3) & ~3)
#define ENABLE_trice16fn_5 0 //!< Disables no-stamp 16-bit 5 values functions.
#else
#define ENABLE_trice16fn_5 1 //!< Enables  no-stamp 16-bit 5 values functions.
#endif
#endif

#ifndef ENABLE_trice16fn_6
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 6 * 2) + 3) & ~3)
#define ENABLE_trice16fn_6 0 //!< Disables no-stamp 16-bit 6 values functions.
#else
#define ENABLE_trice16fn_6 1 //!< Enables  no-stamp 16-bit 6 values functions.
#endif
#endif

#ifndef ENABLE_trice16fn_7
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 7 * 2) + 3) & ~3)
#define ENABLE_trice16fn_7 0 //!< Disables no-stamp 16-bit 7 values functions.
#else
#define ENABLE_trice16fn_7 1 //!< Enables  no-stamp 16-bit 7 values functions.
#endif
#endif

#ifndef ENABLE_trice16fn_8
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 8 * 2) + 3) & ~3)
#define ENABLE_trice16fn_8 0 //!< Disables no-stamp 16-bit 8 values functions.
#else
#define ENABLE_trice16fn_8 1 //!< Enables  no-stamp 16-bit 8 values functions.
#endif
#endif

#ifndef ENABLE_trice16fn_9
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 9 * 2) + 3) & ~3)
#define ENABLE_trice16fn_9 0 //!< Disables no-stamp 16-bit 9 values functions.
#else
#define ENABLE_trice16fn_9 1 //!< Enables  no-stamp 16-bit 9 values functions.
#endif
#endif

#ifndef ENABLE_trice16fn_10
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 10 * 2) + 3) & ~3)
#define ENABLE_trice16fn_10 0 //!< Disables no-stamp 16-bit 10 values functions.
#else
#define ENABLE_trice16fn_10 1 //!< Enables  no-stamp 16-bit 10 values functions.
#endif
#endif

#ifndef ENABLE_trice16fn_11
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 11 * 2) + 3) & ~3)
#define ENABLE_trice16fn_11 0 //!< Disables no-stamp 16-bit 11 values functions.
#else
#define ENABLE_trice16fn_11 1 //!< Enables  no-stamp 16-bit 11 values functions.
#endif
#endif

#ifndef ENABLE_trice16fn_12
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 12 * 2) + 3) & ~3)
#define ENABLE_trice16fn_12 0 //!< Disables no-stamp 16-bit 12 values functions.
#else
#define ENABLE_trice16fn_12 1 //!< Enables  no-stamp 16-bit 12 values functions.
#endif
#endif

// with 16-bit-stamp 16-bit values functions (16-bit-stamps need 4 bytes for allocation reasons)
#ifndef ENABLE_Trice16fn_0
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 0 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_0 0 //!< Disables 16-bit-stamp 16-bit 0 values functions.
#else
#define ENABLE_Trice16fn_0 1 //!< Enables  16-bit-stamp 16-bit 0 values functions.
#endif
#endif

#ifndef ENABLE_Trice16fn_1
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 1 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_1 0 //!< Disables 16-bit-stamp 16-bit 1 values functions.
#else
#define ENABLE_Trice16fn_1 1 //!< Enables  16-bit-stamp 16-bit 1 values functions.
#endif
#endif

#ifndef ENABLE_Trice16fn_2
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 2 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_2 0 //!< Disables 16-bit-stamp 16-bit 2 values functions.
#else
#define ENABLE_Trice16fn_2 1 //!< Enables  16-bit-stamp 16-bit 2 values functions.
#endif
#endif

#ifndef ENABLE_Trice16fn_3
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 3 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_3 0 //!< Disables 16-bit-stamp 16-bit 3 values functions.
#else
#define ENABLE_Trice16fn_3 1 //!< Enables  16-bit-stamp 16-bit 3 values functions.
#endif
#endif

#ifndef ENABLE_Trice16fn_4
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 4 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_4 0 //!< Disables 16-bit-stamp 16-bit 4 values functions.
#else
#define ENABLE_Trice16fn_4 1 //!< Enables  16-bit-stamp 16-bit 4 values functions.
#endif
#endif

#ifndef ENABLE_Trice16fn_5
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 5 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_5 0 //!< Disables 16-bit-stamp 16-bit 5 values functions.
#else
#define ENABLE_Trice16fn_5 1 //!< Enables  16-bit-stamp 16-bit 5 values functions.
#endif
#endif

#ifndef ENABLE_Trice16fn_6
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 6 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_6 0 //!< Disables 16-bit-stamp 16-bit 6 values functions.
#else
#define ENABLE_Trice16fn_6 1 //!< Enables  16-bit-stamp 16-bit 6 values functions.
#endif
#endif

#ifndef ENABLE_Trice16fn_7
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 7 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_7 0 //!< Disables 16-bit-stamp 16-bit 7 values functions.
#else
#define ENABLE_Trice16fn_7 1 //!< Enables  16-bit-stamp 16-bit 7 values functions.
#endif
#endif

#ifndef ENABLE_Trice16fn_8
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 8 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_8 0 //!< Disables 16-bit-stamp 16-bit 8 values functions.
#else
#define ENABLE_Trice16fn_8 1 //!< Enables  16-bit-stamp 16-bit 8 values functions.
#endif
#endif

#ifndef ENABLE_Trice16fn_9
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 9 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_9 0 //!< Disables 16-bit-stamp 16-bit 9 values functions.
#else
#define ENABLE_Trice16fn_9 1 //!< Enables  16-bit-stamp 16-bit 9 values functions.
#endif
#endif

#ifndef ENABLE_Trice16fn_10
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 10 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_10 0 //!< Disables 16-bit-stamp 16-bit 10 values functions.
#else
#define ENABLE_Trice16fn_10 1 //!< Enables  16-bit-stamp 16-bit 10 values functions.
#endif
#endif

#ifndef ENABLE_Trice16fn_11
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 11 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_11 0 //!< Disables 16-bit-stamp 16-bit 11 values functions.
#else
#define ENABLE_Trice16fn_11 1 //!< Enables  16-bit-stamp 16-bit 11 values functions.
#endif
#endif

#ifndef ENABLE_Trice16fn_12
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 12 * 2) + 3) & ~3)
#define ENABLE_Trice16fn_12 0 //!< Disables 16-bit-stamp 16-bit 12 values functions.
#else
#define ENABLE_Trice16fn_12 1 //!< Enables  16-bit-stamp 16-bit 12 values functions.
#endif
#endif

// with 32-bit-stamp 16-bit values functions
#ifndef ENABLE_TRice16fn_0
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 0 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_0 0 //!< Disables 32-bit-stamp 16-bit 0 values functions.
#else
#define ENABLE_TRice16fn_0 1 //!< Enables  32-bit-stamp 16-bit 0 values functions.
#endif
#endif

#ifndef ENABLE_TRice16fn_1
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 1 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_1 0 //!< Disables 32-bit-stamp 16-bit 1 values functions.
#else
#define ENABLE_TRice16fn_1 1 //!< Enables  32-bit-stamp 16-bit 1 values functions.
#endif
#endif

#ifndef ENABLE_TRice16fn_2
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 2 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_2 0 //!< Disables 32-bit-stamp 16-bit 2 values functions.
#else
#define ENABLE_TRice16fn_2 1 //!< Enables  32-bit-stamp 16-bit 2 values functions.
#endif
#endif

#ifndef ENABLE_TRice16fn_3
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 3 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_3 0 //!< Disables 32-bit-stamp 16-bit 3 values functions.
#else
#define ENABLE_TRice16fn_3 1 //!< Enables  32-bit-stamp 16-bit 3 values functions.
#endif
#endif

#ifndef ENABLE_TRice16fn_4
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 4 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_4 0 //!< Disables 32-bit-stamp 16-bit 4 values functions.
#else
#define ENABLE_TRice16fn_4 1 //!< Enables  32-bit-stamp 16-bit 4 values functions.
#endif
#endif

#ifndef ENABLE_TRice16fn_5
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 5 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_5 0 //!< Disables 32-bit-stamp 16-bit 5 values functions.
#else
#define ENABLE_TRice16fn_5 1 //!< Enables  32-bit-stamp 16-bit 5 values functions.
#endif
#endif

#ifndef ENABLE_TRice16fn_6
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 6 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_6 0 //!< Disables 32-bit-stamp 16-bit 6 values functions.
#else
#define ENABLE_TRice16fn_6 1 //!< Enables  32-bit-stamp 16-bit 6 values functions.
#endif
#endif

#ifndef ENABLE_TRice16fn_7
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 7 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_7 0 //!< Disables 32-bit-stamp 16-bit 7 values functions.
#else
#define ENABLE_TRice16fn_7 1 //!< Enables  32-bit-stamp 16-bit 7 values functions.
#endif
#endif

#ifndef ENABLE_TRice16fn_8
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 8 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_8 0 //!< Disables 32-bit-stamp 16-bit 8 values functions.
#else
#define ENABLE_TRice16fn_8 1 //!< Enables  32-bit-stamp 16-bit 8 values functions.
#endif
#endif

#ifndef ENABLE_TRice16fn_9
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 9 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_9 0 //!< Disables 32-bit-stamp 16-bit 9 values functions.
#else
#define ENABLE_TRice16fn_9 1 //!< Enables  32-bit-stamp 16-bit 9 values functions.
#endif
#endif

#ifndef ENABLE_TRice16fn_10
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 10 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_10 0 //!< Disables 32-bit-stamp 16-bit 10 values functions.
#else
#define ENABLE_TRice16fn_10 1 //!< Enables  32-bit-stamp 16-bit 10 values functions.
#endif
#endif

#ifndef ENABLE_TRice16fn_11
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 11 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_11 0 //!< Disables 32-bit-stamp 16-bit 11 values functions.
#else
#define ENABLE_TRice16fn_11 1 //!< Enables  32-bit-stamp 16-bit 11 values functions.
#endif
#endif

#ifndef ENABLE_TRice16fn_12
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 12 * 2) + 3) & ~3)
#define ENABLE_TRice16fn_12 0 //!< Disables 32-bit-stamp 16-bit 12 values functions.
#else
#define ENABLE_TRice16fn_12 1 //!< Enables  32-bit-stamp 16-bit 12 values functions.
#endif
#endif

#if ENABLE_trice16fn_0
void trice16fn_0(uint16_t tid) {
	trice16m_0(tid);
}
#endif

#if ENABLE_trice16fn_1
void trice16fn_1(uint16_t tid, uint16_t v0) {
	trice16m_1(tid, v0);
}
#endif

#if ENABLE_trice16fn_2
void trice16fn_2(uint16_t tid, uint16_t v0, uint16_t v1) {
	trice16m_2(tid, v0, v1);
}
#endif

#if ENABLE_trice16fn_3
void trice16fn_3(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2) {
	trice16m_3(tid, v0, v1, v2);
}
#endif

#if ENABLE_trice16fn_4
void trice16fn_4(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3) {
	trice16m_4(tid, v0, v1, v2, v3);
}
#endif

#if ENABLE_trice16fn_5
void trice16fn_5(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4) {
	trice16m_5(tid, v0, v1, v2, v3, v4);
}
#endif

#if ENABLE_trice16fn_6
void trice16fn_6(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5) {
	trice16m_6(tid, v0, v1, v2, v3, v4, v5);
}
#endif

#if ENABLE_trice16fn_7
void trice16fn_7(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6) {
	trice16m_7(tid, v0, v1, v2, v3, v4, v5, v6);
}
#endif

#if ENABLE_trice16fn_8
void trice16fn_8(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7) {
	trice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7);
}
#endif

#if ENABLE_trice16fn_9
void trice16fn_9(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8) {
	trice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif

#if ENABLE_trice16fn_10
void trice16fn_10(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9) {
	trice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
#endif

#if ENABLE_trice16fn_11
void trice16fn_11(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10) {
	trice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}
#endif

#if ENABLE_trice16fn_12
void trice16fn_12(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10, uint16_t v11) {
	trice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}
#endif

#if ENABLE_Trice16fn_0
void Trice16fn_0(uint16_t tid) {
	Trice16m_0(tid);
}
#endif

#if ENABLE_Trice16fn_1
void Trice16fn_1(uint16_t tid, uint16_t v0) {
	Trice16m_1(tid, v0);
}
#endif

#if ENABLE_Trice16fn_2
void Trice16fn_2(uint16_t tid, uint16_t v0, uint16_t v1) {
	Trice16m_2(tid, v0, v1);
}
#endif

#if ENABLE_Trice16fn_3
void Trice16fn_3(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2) {
	Trice16m_3(tid, v0, v1, v2);
}
#endif

#if ENABLE_Trice16fn_4
void Trice16fn_4(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3) {
	Trice16m_4(tid, v0, v1, v2, v3);
}
#endif

#if ENABLE_Trice16fn_5
void Trice16fn_5(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4) {
	Trice16m_5(tid, v0, v1, v2, v3, v4);
}
#endif

#if ENABLE_Trice16fn_6
void Trice16fn_6(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5) {
	Trice16m_6(tid, v0, v1, v2, v3, v4, v5);
}
#endif

#if ENABLE_Trice16fn_7
void Trice16fn_7(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6) {
	Trice16m_7(tid, v0, v1, v2, v3, v4, v5, v6);
}
#endif

#if ENABLE_Trice16fn_8
void Trice16fn_8(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7) {
	Trice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7);
}
#endif

#if ENABLE_Trice16fn_9
void Trice16fn_9(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8) {
	Trice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif

#if ENABLE_Trice16fn_10
void Trice16fn_10(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9) {
	Trice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
#endif

#if ENABLE_Trice16fn_11
void Trice16fn_11(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10) {
	Trice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}
#endif

#if ENABLE_Trice16fn_12
void Trice16fn_12(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10, uint16_t v11) {
	Trice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}
#endif

#if ENABLE_TRice16fn_0
void TRice16fn_0(uint16_t tid) {
	TRice16m_0(tid);
}
#endif

#if ENABLE_TRice16fn_1
void TRice16fn_1(uint16_t tid, uint16_t v0) {
	TRice16m_1(tid, v0);
}
#endif

#if ENABLE_TRice16fn_2
void TRice16fn_2(uint16_t tid, uint16_t v0, uint16_t v1) {
	TRice16m_2(tid, v0, v1);
}
#endif

#if ENABLE_TRice16fn_3
void TRice16fn_3(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2) {
	TRice16m_3(tid, v0, v1, v2);
}
#endif

#if ENABLE_TRice16fn_4
void TRice16fn_4(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3) {
	TRice16m_4(tid, v0, v1, v2, v3);
}
#endif

#if ENABLE_TRice16fn_5
void TRice16fn_5(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4) {
	TRice16m_5(tid, v0, v1, v2, v3, v4);
}
#endif

#if ENABLE_TRice16fn_6
void TRice16fn_6(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5) {
	TRice16m_6(tid, v0, v1, v2, v3, v4, v5);
}
#endif

#if ENABLE_TRice16fn_7
void TRice16fn_7(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6) {
	TRice16m_7(tid, v0, v1, v2, v3, v4, v5, v6);
}
#endif

#if ENABLE_TRice16fn_8
void TRice16fn_8(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7) {
	TRice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7);
}
#endif

#if ENABLE_TRice16fn_9
void TRice16fn_9(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8) {
	TRice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif

#if ENABLE_TRice16fn_10
void TRice16fn_10(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9) {
	TRice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
#endif

#if ENABLE_TRice16fn_11
void TRice16fn_11(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10) {
	TRice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}
#endif

#if ENABLE_TRice16fn_12
void TRice16fn_12(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10, uint16_t v11) {
	TRice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}
#endif

#endif // #if TRICE_16_BIT_SUPPORT == 1 && TRICE_OFF == 0
