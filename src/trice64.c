//! \file trice64.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////
#include "trice.h"

// lint -e529  Warning 529: Symbol '_SEGGER_RTT__LockState' not subsequently referenced
// lint -e701  Info 701: Shift left of signed quantity (int)
// lint -e712  Info 712: Loss of precision (assignment) (unsigned long long to unsigned int)

#if TRICE_64_BIT_SUPPORT == 1 && TRICE_OFF == 0

// no-stamp 64-bit-values functions
#ifndef ENABLE_trice64fn_0
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 0 * 8) + 3) & ~3)
#define ENABLE_trice64fn_0 0 //!< Disables no-stamp 64-bit 0 values functions.
#else
#define ENABLE_trice64fn_0 1 //!< Enables  no-stamp 64-bit 0 values functions.
#endif
#endif

#ifndef ENABLE_trice64fn_1
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 1 * 8) + 3) & ~3)
#define ENABLE_trice64fn_1 0 //!< Disables no-stamp 64-bit 1 values functions.
#else
#define ENABLE_trice64fn_1 1 //!< Enables  no-stamp 64-bit 1 values functions.
#endif
#endif

#ifndef ENABLE_trice64fn_2
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 2 * 8) + 3) & ~3)
#define ENABLE_trice64fn_2 0 //!< Disables no-stamp 64-bit 2 values functions.
#else
#define ENABLE_trice64fn_2 1 //!< Enables  no-stamp 64-bit 2 values functions.
#endif
#endif

#ifndef ENABLE_trice64fn_3
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 3 * 8) + 3) & ~3)
#define ENABLE_trice64fn_3 0 //!< Disables no-stamp 64-bit 3 values functions.
#else
#define ENABLE_trice64fn_3 1 //!< Enables  no-stamp 64-bit 3 values functions.
#endif
#endif

#ifndef ENABLE_trice64fn_4
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 4 * 8) + 3) & ~3)
#define ENABLE_trice64fn_4 0 //!< Disables no-stamp 64-bit 4 values functions.
#else
#define ENABLE_trice64fn_4 1 //!< Enables  no-stamp 64-bit 4 values functions.
#endif
#endif

#ifndef ENABLE_trice64fn_5
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 5 * 8) + 3) & ~3)
#define ENABLE_trice64fn_5 0 //!< Disables no-stamp 64-bit 5 values functions.
#else
#define ENABLE_trice64fn_5 1 //!< Enables  no-stamp 64-bit 5 values functions.
#endif
#endif

#ifndef ENABLE_trice64fn_6
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 6 * 8) + 3) & ~3)
#define ENABLE_trice64fn_6 0 //!< Disables no-stamp 64-bit 6 values functions.
#else
#define ENABLE_trice64fn_6 1 //!< Enables  no-stamp 64-bit 6 values functions.
#endif
#endif

#ifndef ENABLE_trice64fn_7
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 7 * 8) + 3) & ~3)
#define ENABLE_trice64fn_7 0 //!< Disables no-stamp 64-bit 7 values functions.
#else
#define ENABLE_trice64fn_7 1 //!< Enables  no-stamp 64-bit 7 values functions.
#endif
#endif

#ifndef ENABLE_trice64fn_8
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 8 * 8) + 3) & ~3)
#define ENABLE_trice64fn_8 0 //!< Disables no-stamp 64-bit 8 values functions.
#else
#define ENABLE_trice64fn_8 1 //!< Enables  no-stamp 64-bit 8 values functions.
#endif
#endif

#ifndef ENABLE_trice64fn_9
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 9 * 8) + 3) & ~3)
#define ENABLE_trice64fn_9 0 //!< Disables no-stamp 64-bit 9 values functions.
#else
#define ENABLE_trice64fn_9 1 //!< Enables  no-stamp 64-bit 9 values functions.
#endif
#endif

#ifndef ENABLE_trice64fn_10
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 10 * 8) + 3) & ~3)
#define ENABLE_trice64fn_10 0 //!< Disables no-stamp 64-bit 10 values functions.
#else
#define ENABLE_trice64fn_10 1 //!< Enables  no-stamp 64-bit 10 values functions.
#endif
#endif

#ifndef ENABLE_trice64fn_11
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 11 * 8) + 3) & ~3)
#define ENABLE_trice64fn_11 0 //!< Disables no-stamp 64-bit 11 values functions.
#else
#define ENABLE_trice64fn_11 1 //!< Enables  no-stamp 64-bit 11 values functions.
#endif
#endif

#ifndef ENABLE_trice64fn_12
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 12 * 8) + 3) & ~3)
#define ENABLE_trice64fn_12 0 //!< Disables no-stamp 64-bit 12 values functions.
#else
#define ENABLE_trice64fn_12 1 //!< Enables  no-stamp 64-bit 12 values functions.
#endif
#endif

// with 16-bit-stamp 64-bit-values functions (16-bit-stamps need 4 bytes for allocation reasons)
#ifndef ENABLE_Trice64fn_0
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 0 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_0 0 //!< Disables 16-bit stamp 64-bit 0 values functions.
#else
#define ENABLE_Trice64fn_0 1 //!< Enables  16-bit stamp 64-bit 0 values functions.
#endif
#endif

#ifndef ENABLE_Trice64fn_1
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 1 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_1 0 //!< Disables 16-bit stamp 64-bit 1 values functions.
#else
#define ENABLE_Trice64fn_1 1 //!< Enables  16-bit stamp 64-bit 1 values functions.
#endif
#endif

#ifndef ENABLE_Trice64fn_2
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 2 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_2 0 //!< Disables 16-bit stamp 64-bit 2 values functions.
#else
#define ENABLE_Trice64fn_2 1 //!< Enables  16-bit stamp 64-bit 2 values functions.
#endif
#endif

#ifndef ENABLE_Trice64fn_3
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 3 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_3 0 //!< Disables 16-bit stamp 64-bit 3 values functions.
#else
#define ENABLE_Trice64fn_3 1 //!< Enables  16-bit stamp 64-bit 3 values functions.
#endif
#endif

#ifndef ENABLE_Trice64fn_4
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 4 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_4 0 //!< Disables 16-bit stamp 64-bit 4 values functions.
#else
#define ENABLE_Trice64fn_4 1 //!< Enables  16-bit stamp 64-bit 4 values functions.
#endif
#endif

#ifndef ENABLE_Trice64fn_5
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 5 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_5 0 //!< Disables 16-bit stamp 64-bit 5 values functions.
#else
#define ENABLE_Trice64fn_5 1 //!< Enables  16-bit stamp 64-bit 5 values functions.
#endif
#endif

#ifndef ENABLE_Trice64fn_6
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 6 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_6 0 //!< Disables 16-bit stamp 64-bit 6 values functions.
#else
#define ENABLE_Trice64fn_6 1 //!< Enables  16-bit stamp 64-bit 6 values functions.
#endif
#endif

#ifndef ENABLE_Trice64fn_7
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 7 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_7 0 //!< Disables 16-bit stamp 64-bit 7 values functions.
#else
#define ENABLE_Trice64fn_7 1 //!< Enables  16-bit stamp 64-bit 7 values functions.
#endif
#endif

#ifndef ENABLE_Trice64fn_8
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 8 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_8 0 //!< Disables 16-bit stamp 64-bit 8 values functions.
#else
#define ENABLE_Trice64fn_8 1 //!< Enables  16-bit stamp 64-bit 8 values functions.
#endif
#endif

#ifndef ENABLE_Trice64fn_9
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 9 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_9 0 //!< Disables 16-bit stamp 64-bit 9 values functions.
#else
#define ENABLE_Trice64fn_9 1 //!< Enables  16-bit stamp 64-bit 9 values functions.
#endif
#endif

#ifndef ENABLE_Trice64fn_10
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 10 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_10 0 //!< Disables 16-bit stamp 64-bit 10 values functions.
#else
#define ENABLE_Trice64fn_10 1 //!< Enables  16-bit stamp 64-bit 10 values functions.
#endif
#endif

#ifndef ENABLE_Trice64fn_11
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 11 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_11 0 //!< Disables 16-bit stamp 64-bit 11 values functions.
#else
#define ENABLE_Trice64fn_11 1 //!< Enables  16-bit stamp 64-bit 11 values functions.
#endif
#endif

#ifndef ENABLE_Trice64fn_12
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 12 * 8) + 3) & ~3)
#define ENABLE_Trice64fn_12 0 //!< Disables 16-bit stamp 64-bit 12 values functions.
#else
#define ENABLE_Trice64fn_12 1 //!< Enables  16-bit stamp 64-bit 12 values functions.
#endif
#endif

// with 32-bit-stamp 64-bit-values functions
#ifndef ENABLE_TRice64fn_0
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 0 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_0 0 //!< Disables 32-bit-stamp 64-bit 0 values functions.
#else
#define ENABLE_TRice64fn_0 1 //!< Enables  32-bit-stamp 64-bit 0 values functions.
#endif
#endif

#ifndef ENABLE_TRice64fn_1
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 1 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_1 0 //!< Disables 32-bit-stamp 64-bit 1 values functions.
#else
#define ENABLE_TRice64fn_1 1 //!< Enables  32-bit-stamp 64-bit 1 values functions.
#endif
#endif

#ifndef ENABLE_TRice64fn_2
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 2 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_2 0 //!< Disables 32-bit-stamp 64-bit 2 values functions.
#else
#define ENABLE_TRice64fn_2 1 //!< Enables  32-bit-stamp 64-bit 2 values functions.
#endif
#endif

#ifndef ENABLE_TRice64fn_3
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 3 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_3 0 //!< Disables 32-bit-stamp 64-bit 3 values functions.
#else
#define ENABLE_TRice64fn_3 1 //!< Enables  32-bit-stamp 64-bit 3 values functions.
#endif
#endif

#ifndef ENABLE_TRice64fn_4
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 4 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_4 0 //!< Disables 32-bit-stamp 64-bit 4 values functions.
#else
#define ENABLE_TRice64fn_4 1 //!< Enables  32-bit-stamp 64-bit 4 values functions.
#endif
#endif

#ifndef ENABLE_TRice64fn_5
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 5 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_5 0 //!< Disables 32-bit-stamp 64-bit 5 values functions.
#else
#define ENABLE_TRice64fn_5 1 //!< Enables  32-bit-stamp 64-bit 5 values functions.
#endif
#endif

#ifndef ENABLE_TRice64fn_6
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 6 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_6 0 //!< Disables 32-bit-stamp 64-bit 6 values functions.
#else
#define ENABLE_TRice64fn_6 1 //!< Enables  32-bit-stamp 64-bit 6 values functions.
#endif
#endif

#ifndef ENABLE_TRice64fn_7
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 7 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_7 0 //!< Disables 32-bit-stamp 64-bit 7 values functions.
#else
#define ENABLE_TRice64fn_7 1 //!< Enables  32-bit-stamp 64-bit 7 values functions.
#endif
#endif

#ifndef ENABLE_TRice64fn_8
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 8 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_8 0 //!< Disables 32-bit-stamp 64-bit 8 values functions.
#else
#define ENABLE_TRice64fn_8 1 //!< Enables  32-bit-stamp 64-bit 8 values functions.
#endif
#endif

#ifndef ENABLE_TRice64fn_9
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 9 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_9 0 //!< Disables 32-bit-stamp 64-bit 9 values functions.
#else
#define ENABLE_TRice64fn_9 1 //!< Enables  32-bit-stamp 64-bit 9 values functions.
#endif
#endif

#ifndef ENABLE_TRice64fn_10
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 10 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_10 0 //!< Disables 32-bit-stamp 64-bit 10 values functions.
#else
#define ENABLE_TRice64fn_10 1 //!< Enables  32-bit-stamp 64-bit 10 values functions.
#endif
#endif

#ifndef ENABLE_TRice64fn_11
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 11 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_11 0 //!< Disables 32-bit-stamp 64-bit 11 values functions.
#else
#define ENABLE_TRice64fn_11 1 //!< Enables  32-bit-stamp 64-bit 11 values functions.
#endif
#endif

#ifndef ENABLE_TRice64fn_12
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 12 * 8) + 3) & ~3)
#define ENABLE_TRice64fn_12 0 //!< Disables 32-bit-stamp 64-bit 12 values functions.
#else
#define ENABLE_TRice64fn_12 1 //!< Enables  32-bit-stamp 64-bit 12 values functions.
#endif
#endif

#if ENABLE_trice64fn_0
void trice64fn_0(uint16_t tid) {
	trice64m_0(tid);
}
#endif

#if ENABLE_trice64fn_1
void trice64fn_1(uint16_t tid, uint64_t v0) {
	trice64m_1(tid, v0);
}
#endif

#if ENABLE_trice64fn_2
void trice64fn_2(uint16_t tid, uint64_t v0, uint64_t v1) {
	trice64m_2(tid, v0, v1);
}
#endif

#if ENABLE_trice64fn_3
void trice64fn_3(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2) {
	trice64m_3(tid, v0, v1, v2);
}
#endif

#if ENABLE_trice64fn_4
void trice64fn_4(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3) {
	trice64m_4(tid, v0, v1, v2, v3);
}
#endif

#if ENABLE_trice64fn_5
void trice64fn_5(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4) {
	trice64m_5(tid, v0, v1, v2, v3, v4);
}
#endif

#if ENABLE_trice64fn_6
void trice64fn_6(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5) {
	trice64m_6(tid, v0, v1, v2, v3, v4, v5);
}
#endif

#if ENABLE_trice64fn_7
void trice64fn_7(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6) {
	trice64m_7(tid, v0, v1, v2, v3, v4, v5, v6);
}
#endif

#if ENABLE_trice64fn_8
void trice64fn_8(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7) {
	trice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7);
}
#endif

#if ENABLE_trice64fn_9
void trice64fn_9(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8) {
	trice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif

#if ENABLE_trice64fn_10
void trice64fn_10(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9) {
	trice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
#endif

#if ENABLE_trice64fn_11
void trice64fn_11(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10) {
	trice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}
#endif

#if ENABLE_trice64fn_12
void trice64fn_12(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10, uint64_t v11) {
	trice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}
#endif

#if ENABLE_Trice64fn_0
void Trice64fn_0(uint16_t tid) {
	Trice64m_0(tid);
}
#endif

#if ENABLE_Trice64fn_1
void Trice64fn_1(uint16_t tid, uint64_t v0) {
	Trice64m_1(tid, v0);
}
#endif

#if ENABLE_Trice64fn_2
void Trice64fn_2(uint16_t tid, uint64_t v0, uint64_t v1) {
	Trice64m_2(tid, v0, v1);
}
#endif

#if ENABLE_Trice64fn_3
void Trice64fn_3(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2) {
	Trice64m_3(tid, v0, v1, v2);
}
#endif

#if ENABLE_Trice64fn_4
void Trice64fn_4(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3) {
	Trice64m_4(tid, v0, v1, v2, v3);
}
#endif

#if ENABLE_Trice64fn_5
void Trice64fn_5(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4) {
	Trice64m_5(tid, v0, v1, v2, v3, v4);
}
#endif

#if ENABLE_Trice64fn_6
void Trice64fn_6(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5) {
	Trice64m_6(tid, v0, v1, v2, v3, v4, v5);
}
#endif

#if ENABLE_Trice64fn_7
void Trice64fn_7(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6) {
	Trice64m_7(tid, v0, v1, v2, v3, v4, v5, v6);
}
#endif

#if ENABLE_Trice64fn_8
void Trice64fn_8(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7) {
	Trice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7);
}
#endif

#if ENABLE_Trice64fn_9
void Trice64fn_9(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8) {
	Trice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif

#if ENABLE_Trice64fn_10
void Trice64fn_10(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9) {
	Trice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
#endif

#if ENABLE_Trice64fn_11
void Trice64fn_11(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10) {
	Trice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}
#endif

#if ENABLE_Trice64fn_12
void Trice64fn_12(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10, uint64_t v11) {
	Trice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}
#endif

#if ENABLE_TRice64fn_0
void TRice64fn_0(uint16_t tid) {
	TRice64m_0(tid);
}
#endif

#if ENABLE_TRice64fn_1
void TRice64fn_1(uint16_t tid, uint64_t v0) {
	TRice64m_1(tid, v0);
}
#endif

#if ENABLE_TRice64fn_2
void TRice64fn_2(uint16_t tid, uint64_t v0, uint64_t v1) {
	TRice64m_2(tid, v0, v1);
}
#endif

#if ENABLE_TRice64fn_3
void TRice64fn_3(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2) {
	TRice64m_3(tid, v0, v1, v2);
}
#endif

#if ENABLE_TRice64fn_4
void TRice64fn_4(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3) {
	TRice64m_4(tid, v0, v1, v2, v3);
}
#endif

#if ENABLE_TRice64fn_5
void TRice64fn_5(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4) {
	TRice64m_5(tid, v0, v1, v2, v3, v4);
}
#endif

#if ENABLE_TRice64fn_6
void TRice64fn_6(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5) {
	TRice64m_6(tid, v0, v1, v2, v3, v4, v5);
}
#endif

#if ENABLE_TRice64fn_7
void TRice64fn_7(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6) {
	TRice64m_7(tid, v0, v1, v2, v3, v4, v5, v6);
}
#endif

#if ENABLE_TRice64fn_8
void TRice64fn_8(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7) {
	TRice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7);
}
#endif

#if ENABLE_TRice64fn_9
void TRice64fn_9(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8) {
	TRice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif

#if ENABLE_TRice64fn_10
void TRice64fn_10(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9) {
	TRice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
#endif

#if ENABLE_TRice64fn_11
void TRice64fn_11(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10) {
	TRice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}
#endif

#if ENABLE_TRice64fn_12
void TRice64fn_12(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10, uint64_t v11) {
	TRice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}
#endif

#endif // #if TRICE_64_BIT_SUPPORT == 1 && TRICE_OFF == 0
