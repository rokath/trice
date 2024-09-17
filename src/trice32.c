//! \file trice32.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////
#include "trice.h"

// lint -e529  Warning 529: Symbol '_SEGGER_RTT__LockState' not subsequently referenced
// lint -e701  Info 701: Shift left of signed quantity (int)

#if TRICE_32_BIT_SUPPORT == 1 && TRICE_OFF == 0

// no-stamp 32-bit-values functions
#ifndef ENABLE_trice32fn_0
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 0 * 4) + 3) & ~3)
#define ENABLE_trice32fn_0 0 //!< Disables no-stamp 32-bit 0 values functions.
#else
#define ENABLE_trice32fn_0 1 //!< Enables  no-stamp 32-bit 0 values functions.
#endif
#endif

#ifndef ENABLE_trice32fn_1
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 1 * 4) + 3) & ~3)
#define ENABLE_trice32fn_1 0 //!< Disables no-stamp 32-bit 1 values functions.
#else
#define ENABLE_trice32fn_1 1 //!< Enables  no-stamp 32-bit 1 values functions.
#endif
#endif

#ifndef ENABLE_trice32fn_2
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 2 * 4) + 3) & ~3)
#define ENABLE_trice32fn_2 0 //!< Disables no-stamp 32-bit 2 values functions.
#else
#define ENABLE_trice32fn_2 1 //!< Enables  no-stamp 32-bit 2 values functions.
#endif
#endif

#ifndef ENABLE_trice32fn_3
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 3 * 4) + 3) & ~3)
#define ENABLE_trice32fn_3 0 //!< Disables no-stamp 32-bit 3 values functions.
#else
#define ENABLE_trice32fn_3 1 //!< Enables  no-stamp 32-bit 3 values functions.
#endif
#endif

#ifndef ENABLE_trice32fn_4
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 4 * 4) + 3) & ~3)
#define ENABLE_trice32fn_4 0 //!< Disables no-stamp 32-bit 4 values functions.
#else
#define ENABLE_trice32fn_4 1 //!< Enables  no-stamp 32-bit 4 values functions.
#endif
#endif

#ifndef ENABLE_trice32fn_5
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 5 * 4) + 3) & ~3)
#define ENABLE_trice32fn_5 0 //!< Disables no-stamp 32-bit 5 values functions.
#else
#define ENABLE_trice32fn_5 1 //!< Enables  no-stamp 32-bit 5 values functions.
#endif
#endif

#ifndef ENABLE_trice32fn_6
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 6 * 4) + 3) & ~3)
#define ENABLE_trice32fn_6 0 //!< Disables no-stamp 32-bit 6 values functions.
#else
#define ENABLE_trice32fn_6 1 //!< Enables  no-stamp 32-bit 6 values functions.
#endif
#endif

#ifndef ENABLE_trice32fn_7
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 7 * 4) + 3) & ~3)
#define ENABLE_trice32fn_7 0 //!< Disables no-stamp 32-bit 7 values functions.
#else
#define ENABLE_trice32fn_7 1 //!< Enables  no-stamp 32-bit 7 values functions.
#endif
#endif

#ifndef ENABLE_trice32fn_8
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 8 * 4) + 3) & ~3)
#define ENABLE_trice32fn_8 0 //!< Disables no-stamp 32-bit 8 values functions.
#else
#define ENABLE_trice32fn_8 1 //!< Enables  no-stamp 32-bit 8 values functions.
#endif
#endif

#ifndef ENABLE_trice32fn_9
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 9 * 4) + 3) & ~3)
#define ENABLE_trice32fn_9 0 //!< Disables no-stamp 32-bit 9 values functions.
#else
#define ENABLE_trice32fn_9 1 //!< Enables  no-stamp 32-bit 9 values functions.
#endif
#endif

#ifndef ENABLE_trice32fn_10
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 10 * 4) + 3) & ~3)
#define ENABLE_trice32fn_10 0 //!< Disables no-stamp 32-bit 10 values functions.
#else
#define ENABLE_trice32fn_10 1 //!< Enables  no-stamp 32-bit 10 values functions.
#endif
#endif

#ifndef ENABLE_trice32fn_11
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 11 * 4) + 3) & ~3)
#define ENABLE_trice32fn_11 0 //!< Disables no-stamp 32-bit 11 values functions.
#else
#define ENABLE_trice32fn_11 1 //!< Enables  no-stamp 32-bit 11 values functions.
#endif
#endif

#ifndef ENABLE_trice32fn_12
#if TRICE_SINGLE_MAX_SIZE < (((0 + 4 + 12 * 4) + 3) & ~3)
#define ENABLE_trice32fn_12 0 //!< Disables no-stamp 32-bit 12 values functions.
#else
#define ENABLE_trice32fn_12 1 //!< Enables  no-stamp 32-bit 12 values functions.
#endif
#endif

// 16-bit-stamp 32-bit-values functions (16-bit-stamps need 4 bytes for allocation reasons)
#ifndef ENABLE_Trice32fn_0
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 0 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_0 0 //!< Disables 16-bit-stamp 32-bit 0 values functions.
#else
#define ENABLE_Trice32fn_0 1 //!< Enables  16-bit-stamp 32-bit 0 values functions.
#endif
#endif

#ifndef ENABLE_Trice32fn_1
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 1 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_1 0 //!< Disables 16-bit-stamp 32-bit 1 values functions.
#else
#define ENABLE_Trice32fn_1 1 //!< Enables  16-bit-stamp 32-bit 1 values functions.
#endif
#endif

#ifndef ENABLE_Trice32fn_2
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 2 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_2 0 //!< Disables 16-bit-stamp 32-bit 2 values functions.
#else
#define ENABLE_Trice32fn_2 1 //!< Enables  16-bit-stamp 32-bit 2 values functions.
#endif
#endif

#ifndef ENABLE_Trice32fn_3
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 3 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_3 0 //!< Disables 16-bit-stamp 32-bit 3 values functions.
#else
#define ENABLE_Trice32fn_3 1 //!< Enables  16-bit-stamp 32-bit 3 values functions.
#endif
#endif

#ifndef ENABLE_Trice32fn_4
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 4 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_4 0 //!< Disables 16-bit-stamp 32-bit 4 values functions.
#else
#define ENABLE_Trice32fn_4 1 //!< Enables  16-bit-stamp 32-bit 4 values functions.
#endif
#endif

#ifndef ENABLE_Trice32fn_5
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 5 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_5 0 //!< Disables 16-bit-stamp 32-bit 5 values functions.
#else
#define ENABLE_Trice32fn_5 1 //!< Enables  16-bit-stamp 32-bit 5 values functions.
#endif
#endif

#ifndef ENABLE_Trice32fn_6
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 6 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_6 0 //!< Disables 16-bit-stamp 32-bit 6 values functions.
#else
#define ENABLE_Trice32fn_6 1 //!< Enables  16-bit-stamp 32-bit 6 values functions.
#endif
#endif

#ifndef ENABLE_Trice32fn_7
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 7 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_7 0 //!< Disables 16-bit-stamp 32-bit 7 values functions.
#else
#define ENABLE_Trice32fn_7 1 //!< Enables  16-bit-stamp 32-bit 7 values functions.
#endif
#endif

#ifndef ENABLE_Trice32fn_8
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 8 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_8 0 //!< Disables 16-bit-stamp 32-bit 8 values functions.
#else
#define ENABLE_Trice32fn_8 1 //!< Enables  16-bit-stamp 32-bit 8 values functions.
#endif
#endif

#ifndef ENABLE_Trice32fn_9
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 9 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_9 0 //!< Disables 16-bit-stamp 32-bit 9 values functions.
#else
#define ENABLE_Trice32fn_9 1 //!< Enables  16-bit-stamp 32-bit 9 values functions.
#endif
#endif

#ifndef ENABLE_Trice32fn_10
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 10 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_10 0 //!< Disables 16-bit-stamp 32-bit 10 values functions.
#else
#define ENABLE_Trice32fn_10 1 //!< Enables  16-bit-stamp 32-bit 10 values functions.
#endif
#endif

#ifndef ENABLE_Trice32fn_11
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 11 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_11 0 //!< Disables 16-bit-stamp 32-bit 11 values functions.
#else
#define ENABLE_Trice32fn_11 1 //!< Enables  16-bit-stamp 32-bit 11 values functions.
#endif
#endif

#ifndef ENABLE_Trice32fn_12
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 12 * 4) + 3) & ~3)
#define ENABLE_Trice32fn_12 0 //!< Disables 16-bit-stamp 32-bit 12 values functions.
#else
#define ENABLE_Trice32fn_12 1 //!< Enables  16-bit-stamp 32-bit 12 values functions.
#endif
#endif

// with 32-bit-stamp 32-bit-values functions
#ifndef ENABLE_TRice32fn_0
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 0 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_0 0 //!< Disables 32-bit-stamp 32-bit 0 values functions.
#else
#define ENABLE_TRice32fn_0 1 //!< Enables  32-bit-stamp 32-bit 0 values functions.
#endif
#endif

#ifndef ENABLE_TRice32fn_1
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 1 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_1 0 //!< Disables 32-bit-stamp 32-bit 1 values functions.
#else
#define ENABLE_TRice32fn_1 1 //!< Enables  32-bit-stamp 32-bit 1 values functions.
#endif
#endif

#ifndef ENABLE_TRice32fn_2
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 2 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_2 0 //!< Disables 32-bit-stamp 32-bit 2 values functions.
#else
#define ENABLE_TRice32fn_2 1 //!< Enables  32-bit-stamp 32-bit 2 values functions.
#endif
#endif

#ifndef ENABLE_TRice32fn_3
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 3 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_3 0 //!< Disables 32-bit-stamp 32-bit 3 values functions.
#else
#define ENABLE_TRice32fn_3 1 //!< Enables  32-bit-stamp 32-bit 3 values functions.
#endif
#endif

#ifndef ENABLE_TRice32fn_4
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 4 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_4 0 //!< Disables 32-bit-stamp 32-bit 4 values functions.
#else
#define ENABLE_TRice32fn_4 1 //!< Enables  32-bit-stamp 32-bit 4 values functions.
#endif
#endif

#ifndef ENABLE_TRice32fn_5
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 5 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_5 0 //!< Disables 32-bit-stamp 32-bit 5 values functions.
#else
#define ENABLE_TRice32fn_5 1 //!< Enables  32-bit-stamp 32-bit 5 values functions.
#endif
#endif

#ifndef ENABLE_TRice32fn_6
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 6 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_6 0 //!< Disables 32-bit-stamp 32-bit 6 values functions.
#else
#define ENABLE_TRice32fn_6 1 //!< Enables  32-bit-stamp 32-bit 6 values functions.
#endif
#endif

#ifndef ENABLE_TRice32fn_7
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 7 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_7 0 //!< Disables 32-bit-stamp 32-bit 7 values functions.
#else
#define ENABLE_TRice32fn_7 1 //!< Enables  32-bit-stamp 32-bit 7 values functions.
#endif
#endif

#ifndef ENABLE_TRice32fn_8
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 8 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_8 0 //!< Disables 32-bit-stamp 32-bit 8 values functions.
#else
#define ENABLE_TRice32fn_8 1 //!< Enables  32-bit-stamp 32-bit 8 values functions.
#endif
#endif

#ifndef ENABLE_TRice32fn_9
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 9 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_9 0 //!< Disables 32-bit-stamp 32-bit 9 values functions.
#else
#define ENABLE_TRice32fn_9 1 //!< Enables  32-bit-stamp 32-bit 9 values functions.
#endif
#endif

#ifndef ENABLE_TRice32fn_10
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 10 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_10 0 //!< Disables 32-bit-stamp 32-bit 10 values functions.
#else
#define ENABLE_TRice32fn_10 1 //!< Enables  32-bit-stamp 32-bit 10 values functions.
#endif
#endif

#ifndef ENABLE_TRice32fn_11
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 11 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_11 0 //!< Disables 32-bit-stamp 32-bit 11 values functions.
#else
#define ENABLE_TRice32fn_11 1 //!< Enables  32-bit-stamp 32-bit 11 values functions.
#endif
#endif

#ifndef ENABLE_TRice32fn_12
#if TRICE_SINGLE_MAX_SIZE < (((4 + 4 + 12 * 4) + 3) & ~3)
#define ENABLE_TRice32fn_12 0 //!< Disables 32-bit-stamp 32-bit 12 values functions.
#else
#define ENABLE_TRice32fn_12 1 //!< Enables  32-bit-stamp 32-bit 12 values functions.
#endif
#endif

#if ENABLE_trice32fn_0
void trice32fn_0(uint16_t tid) {
	trice32m_0(tid);
}
#endif

#if ENABLE_trice32fn_1
void trice32fn_1(uint16_t tid, uint32_t v0) {
	trice32m_1(tid, v0);
}
#endif

#if ENABLE_trice32fn_2
void trice32fn_2(uint16_t tid, uint32_t v0, uint32_t v1) {
	trice32m_2(tid, v0, v1);
}
#endif

#if ENABLE_trice32fn_3
void trice32fn_3(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2) {
	trice32m_3(tid, v0, v1, v2);
}
#endif

#if ENABLE_trice32fn_4
void trice32fn_4(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) {
	trice32m_4(tid, v0, v1, v2, v3);
}
#endif

#if ENABLE_trice32fn_5
void trice32fn_5(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4) {
	trice32m_5(tid, v0, v1, v2, v3, v4);
}
#endif

#if ENABLE_trice32fn_6
void trice32fn_6(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5) {
	trice32m_6(tid, v0, v1, v2, v3, v4, v5);
}
#endif

#if ENABLE_trice32fn_7
void trice32fn_7(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	trice32m_7(tid, v0, v1, v2, v3, v4, v5, v6);
}
#endif

#if ENABLE_trice32fn_8
void trice32fn_8(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7) {
	trice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7);
}
#endif

#if ENABLE_trice32fn_9
void trice32fn_9(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8) {
	trice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif

#if ENABLE_trice32fn_10
void trice32fn_10(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9) {
	trice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
#endif

#if ENABLE_trice32fn_11
void trice32fn_11(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10) {
	trice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}
#endif

#if ENABLE_trice32fn_12
void trice32fn_12(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10, uint32_t v11) {
	trice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}
#endif

#if ENABLE_Trice32fn_0
void Trice32fn_0(uint16_t tid) {
	Trice32m_0(tid);
}
#endif

#if ENABLE_Trice32fn_1
void Trice32fn_1(uint16_t tid, uint32_t v0) {
	Trice32m_1(tid, v0);
}
#endif

#if ENABLE_Trice32fn_2
void Trice32fn_2(uint16_t tid, uint32_t v0, uint32_t v1) {
	Trice32m_2(tid, v0, v1);
}
#endif

#if ENABLE_Trice32fn_3
void Trice32fn_3(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2) {
	Trice32m_3(tid, v0, v1, v2);
}
#endif

#if ENABLE_Trice32fn_4
void Trice32fn_4(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) {
	Trice32m_4(tid, v0, v1, v2, v3);
}
#endif

#if ENABLE_Trice32fn_5
void Trice32fn_5(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4) {
	Trice32m_5(tid, v0, v1, v2, v3, v4);
}
#endif

#if ENABLE_Trice32fn_6
void Trice32fn_6(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5) {
	Trice32m_6(tid, v0, v1, v2, v3, v4, v5);
}
#endif

#if ENABLE_Trice32fn_7
void Trice32fn_7(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	Trice32m_7(tid, v0, v1, v2, v3, v4, v5, v6);
}
#endif

#if ENABLE_Trice32fn_8
void Trice32fn_8(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7) {
	Trice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7);
}
#endif

#if ENABLE_Trice32fn_9
void Trice32fn_9(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8) {
	Trice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif

#if ENABLE_Trice32fn_10
void Trice32fn_10(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9) {
	Trice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
#endif

#if ENABLE_Trice32fn_11
void Trice32fn_11(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10) {
	Trice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}
#endif

#if ENABLE_Trice32fn_12
void Trice32fn_12(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10, uint32_t v11) {
	Trice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}
#endif

#if ENABLE_TRice32fn_0
void TRice32fn_0(uint16_t tid) {
	TRice32m_0(tid);
}
#endif

#if ENABLE_TRice32fn_1
void TRice32fn_1(uint16_t tid, uint32_t v0) {
	TRice32m_1(tid, v0);
}
#endif

#if ENABLE_TRice32fn_2
void TRice32fn_2(uint16_t tid, uint32_t v0, uint32_t v1) {
	TRice32m_2(tid, v0, v1);
}
#endif

#if ENABLE_TRice32fn_3
void TRice32fn_3(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2) {
	TRice32m_3(tid, v0, v1, v2);
}
#endif

#if ENABLE_TRice32fn_4
void TRice32fn_4(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) {
	TRice32m_4(tid, v0, v1, v2, v3);
}
#endif

#if ENABLE_TRice32fn_5
void TRice32fn_5(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4) {
	TRice32m_5(tid, v0, v1, v2, v3, v4);
}
#endif

#if ENABLE_TRice32fn_6
void TRice32fn_6(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5) {
	TRice32m_6(tid, v0, v1, v2, v3, v4, v5);
}
#endif

#if ENABLE_TRice32fn_7
void TRice32fn_7(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6) {
	TRice32m_7(tid, v0, v1, v2, v3, v4, v5, v6);
}
#endif

#if ENABLE_TRice32fn_8
void TRice32fn_8(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7) {
	TRice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7);
}
#endif

#if ENABLE_TRice32fn_9
void TRice32fn_9(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8) {
	TRice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8);
}
#endif

#if ENABLE_TRice32fn_10
void TRice32fn_10(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9) {
	TRice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
#endif

#if ENABLE_TRice32fn_11
void TRice32fn_11(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10) {
	TRice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);
}
#endif

#if ENABLE_TRice32fn_12
void TRice32fn_12(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10, uint32_t v11) {
	TRice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}
#endif

#endif // #if TRICE_32_BIT_SUPPORT == 1 && TRICE_OFF == 0
