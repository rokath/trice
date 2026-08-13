// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file trice64.h
//! \brief trice 64 declarations and macros.

#ifndef TRICE64_H_
#define TRICE64_H_

#if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 64

// #define TRICE_0  TRICE64_0  //!< Default parameter bit width for 0  parameter count TRICE is 64.
#define TRICE_INSERT_TRICE_1 TRICE_INSERT_TRICE64_1   //!< Default parameter bit width for 1  parameter count TRICE is 64.
#define TRICE_INSERT_TRICE_2 TRICE_INSERT_TRICE64_2   //!< Default parameter bit width for 2  parameter count TRICE is 64.
#define TRICE_INSERT_TRICE_3 TRICE_INSERT_TRICE64_3   //!< Default parameter bit width for 3  parameter count TRICE is 64.
#define TRICE_INSERT_TRICE_4 TRICE_INSERT_TRICE64_4   //!< Default parameter bit width for 4  parameter count TRICE is 64.
#define TRICE_INSERT_TRICE_5 TRICE_INSERT_TRICE64_5   //!< Default parameter bit width for 5  parameter count TRICE is 64.
#define TRICE_INSERT_TRICE_6 TRICE_INSERT_TRICE64_6   //!< Default parameter bit width for 6  parameter count TRICE is 64.
#define TRICE_INSERT_TRICE_7 TRICE_INSERT_TRICE64_7   //!< Default parameter bit width for 7  parameter count TRICE is 64.
#define TRICE_INSERT_TRICE_8 TRICE_INSERT_TRICE64_8   //!< Default parameter bit width for 8  parameter count TRICE is 64.
#define TRICE_INSERT_TRICE_9 TRICE_INSERT_TRICE64_9   //!< Default parameter bit width for 9  parameter count TRICE is 64.
#define TRICE_INSERT_TRICE_10 TRICE_INSERT_TRICE64_10 //!< Default parameter bit width for 10 parameter count TRICE is 64.
#define TRICE_INSERT_TRICE_11 TRICE_INSERT_TRICE64_11 //!< Default parameter bit width for 11 parameter count TRICE is 64.
#define TRICE_INSERT_TRICE_12 TRICE_INSERT_TRICE64_12 //!< Default parameter bit width for 12 parameter count TRICE is 64.

// #define trice_0  trice64_0  //!< Default parameter bit width for 1  parameter count trice is 64.
#define TRICE_INSERT_trice_1 TRICE_INSERT_trice64_1   //!< Default parameter bit width for 1  parameter count trice is 64.
#define TRICE_INSERT_trice_2 TRICE_INSERT_trice64_2   //!< Default parameter bit width for 2  parameter count trice is 64.
#define TRICE_INSERT_trice_3 TRICE_INSERT_trice64_3   //!< Default parameter bit width for 3  parameter count trice is 64.
#define TRICE_INSERT_trice_4 TRICE_INSERT_trice64_4   //!< Default parameter bit width for 4  parameter count trice is 64.
#define TRICE_INSERT_trice_5 TRICE_INSERT_trice64_5   //!< Default parameter bit width for 5  parameter count trice is 64.
#define TRICE_INSERT_trice_6 TRICE_INSERT_trice64_6   //!< Default parameter bit width for 6  parameter count trice is 64.
#define TRICE_INSERT_trice_7 TRICE_INSERT_trice64_7   //!< Default parameter bit width for 7  parameter count trice is 64.
#define TRICE_INSERT_trice_8 TRICE_INSERT_trice64_8   //!< Default parameter bit width for 8  parameter count trice is 64.
#define TRICE_INSERT_trice_9 TRICE_INSERT_trice64_9   //!< Default parameter bit width for 9  parameter count trice is 64.
#define TRICE_INSERT_trice_10 TRICE_INSERT_trice64_10 //!< Default parameter bit width for 10 parameter count trice is 64.
#define TRICE_INSERT_trice_11 TRICE_INSERT_trice64_11 //!< Default parameter bit width for 11 parameter count trice is 64.
#define TRICE_INSERT_trice_12 TRICE_INSERT_trice64_12 //!< Default parameter bit width for 12 parameter count trice is 64.

// #define Trice_0  Trice64_0  //!< Default parameter bit width for 0  parameter count Trice is 64.
#define TRICE_INSERT_Trice_1 TRICE_INSERT_Trice64_1   //!< Default parameter bit width for 1  parameter count Trice is 64.
#define TRICE_INSERT_Trice_2 TRICE_INSERT_Trice64_2   //!< Default parameter bit width for 2  parameter count Trice is 64.
#define TRICE_INSERT_Trice_3 TRICE_INSERT_Trice64_3   //!< Default parameter bit width for 3  parameter count Trice is 64.
#define TRICE_INSERT_Trice_4 TRICE_INSERT_Trice64_4   //!< Default parameter bit width for 4  parameter count Trice is 64.
#define TRICE_INSERT_Trice_5 TRICE_INSERT_Trice64_5   //!< Default parameter bit width for 5  parameter count Trice is 64.
#define TRICE_INSERT_Trice_6 TRICE_INSERT_Trice64_6   //!< Default parameter bit width for 6  parameter count Trice is 64.
#define TRICE_INSERT_Trice_7 TRICE_INSERT_Trice64_7   //!< Default parameter bit width for 7  parameter count Trice is 64.
#define TRICE_INSERT_Trice_8 TRICE_INSERT_Trice64_8   //!< Default parameter bit width for 8  parameter count Trice is 64.
#define TRICE_INSERT_Trice_9 TRICE_INSERT_Trice64_9   //!< Default parameter bit width for 9  parameter count Trice is 64.
#define TRICE_INSERT_Trice_10 TRICE_INSERT_Trice64_10 //!< Default parameter bit width for 10 parameter count Trice is 64.
#define TRICE_INSERT_Trice_11 TRICE_INSERT_Trice64_11 //!< Default parameter bit width for 11 parameter count Trice is 64.
#define TRICE_INSERT_Trice_12 TRICE_INSERT_Trice64_12 //!< Default parameter bit width for 12 parameter count Trice is 64.

// #define TRice_0  TRice64_0  //!< Default parameter bit width for 0  parameter count TRice is 64.
#define TRICE_INSERT_TRice_1 TRICE_INSERT_TRice64_1   //!< Default parameter bit width for 1  parameter count TRice is 64.
#define TRICE_INSERT_TRice_2 TRICE_INSERT_TRice64_2   //!< Default parameter bit width for 2  parameter count TRice is 64.
#define TRICE_INSERT_TRice_3 TRICE_INSERT_TRice64_3   //!< Default parameter bit width for 3  parameter count TRice is 64.
#define TRICE_INSERT_TRice_4 TRICE_INSERT_TRice64_4   //!< Default parameter bit width for 4  parameter count TRice is 64.
#define TRICE_INSERT_TRice_5 TRICE_INSERT_TRice64_5   //!< Default parameter bit width for 5  parameter count TRice is 64.
#define TRICE_INSERT_TRice_6 TRICE_INSERT_TRice64_6   //!< Default parameter bit width for 6  parameter count TRice is 64.
#define TRICE_INSERT_TRice_7 TRICE_INSERT_TRice64_7   //!< Default parameter bit width for 7  parameter count TRice is 64.
#define TRICE_INSERT_TRice_8 TRICE_INSERT_TRice64_8   //!< Default parameter bit width for 8  parameter count TRice is 64.
#define TRICE_INSERT_TRice_9 TRICE_INSERT_TRice64_9   //!< Default parameter bit width for 9  parameter count TRice is 64.
#define TRICE_INSERT_TRice_10 TRICE_INSERT_TRice64_10 //!< Default parameter bit width for 10 parameter count TRice is 64.
#define TRICE_INSERT_TRice_11 TRICE_INSERT_TRice64_11 //!< Default parameter bit width for 11 parameter count TRice is 64.
#define TRICE_INSERT_TRice_12 TRICE_INSERT_TRice64_12 //!< Default parameter bit width for 12 parameter count TRice is 64.

#endif // #if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 64

#include "triceVariadic.h"

#define TRICE_INSERT_TRICE64(tid, ...) TRICE_CONCAT2(TRICE_INSERT_TRICE64_, TRICE_COUNT_VALUE_ARGUMENTS(__VA_ARGS__))(tid, __VA_ARGS__)

//! TRICE64_B expects inside pFmt only one format specifier, which is used n times by using pFmt n times.
//!  It is usable for showing n 64-bit values.
#define TRICE_INSERT_TRICE64_B(id, pFmt, buf, n)      \
	do {                                              \
		TRICE_INSERT_TRICE_N(id, pFmt, buf, 8 * (n)); \
	} while (0)

#if TRICE_LEGACY_RPC_SUPPORT == 1
//! TRICE64_F expects inside pFmt just a string which is assumed to be a remote function name.
//!  The trice tool displays the pFmt string followed by n times (64-bit value i).
//!  The idea behind is to generate an id - function pointer referece list from the generated til.json file to
//!  compile it into a remote device and execute the inside pFmt named function remotely.
//!  Look for "TRICE64_F example" inside triceCheck.c.
#define TRICE_INSERT_TRICE64_F TRICE_INSERT_TRICE64_B
#endif // #if TRICE_LEGACY_RPC_SUPPORT == 1

#define TRICE_PUT64_1(v0) TRICE_PUT64(v0);

#define TRICE_PUT64_2(v0, v1) \
	TRICE_PUT64(v0);          \
	TRICE_PUT64(v1);

#define TRICE_PUT64_3(v0, v1, v2) \
	TRICE_PUT64(v0);              \
	TRICE_PUT64(v1);              \
	TRICE_PUT64(v2);

#define TRICE_PUT64_4(v0, v1, v2, v3) \
	TRICE_PUT64(v0);                  \
	TRICE_PUT64(v1);                  \
	TRICE_PUT64(v2);                  \
	TRICE_PUT64(v3);

#define TRICE_PUT64_5(v0, v1, v2, v3, v4) \
	TRICE_PUT64(v0);                      \
	TRICE_PUT64(v1);                      \
	TRICE_PUT64(v2);                      \
	TRICE_PUT64(v3);                      \
	TRICE_PUT64(v4);

#define TRICE_PUT64_6(v0, v1, v2, v3, v4, v5) \
	TRICE_PUT64(v0);                          \
	TRICE_PUT64(v1);                          \
	TRICE_PUT64(v2);                          \
	TRICE_PUT64(v3);                          \
	TRICE_PUT64(v4);                          \
	TRICE_PUT64(v5);

#define TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6) \
	TRICE_PUT64(v0);                              \
	TRICE_PUT64(v1);                              \
	TRICE_PUT64(v2);                              \
	TRICE_PUT64(v3);                              \
	TRICE_PUT64(v4);                              \
	TRICE_PUT64(v5);                              \
	TRICE_PUT64(v6);

#define TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_PUT64(v0);                                  \
	TRICE_PUT64(v1);                                  \
	TRICE_PUT64(v2);                                  \
	TRICE_PUT64(v3);                                  \
	TRICE_PUT64(v4);                                  \
	TRICE_PUT64(v5);                                  \
	TRICE_PUT64(v6);                                  \
	TRICE_PUT64(v7);

#define TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_PUT64(v0);                                      \
	TRICE_PUT64(v1);                                      \
	TRICE_PUT64(v2);                                      \
	TRICE_PUT64(v3);                                      \
	TRICE_PUT64(v4);                                      \
	TRICE_PUT64(v5);                                      \
	TRICE_PUT64(v6);                                      \
	TRICE_PUT64(v7);                                      \
	TRICE_PUT64(v8);

#define TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_PUT64(v0);                                           \
	TRICE_PUT64(v1);                                           \
	TRICE_PUT64(v2);                                           \
	TRICE_PUT64(v3);                                           \
	TRICE_PUT64(v4);                                           \
	TRICE_PUT64(v5);                                           \
	TRICE_PUT64(v6);                                           \
	TRICE_PUT64(v7);                                           \
	TRICE_PUT64(v8);                                           \
	TRICE_PUT64(v9);

#define TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_PUT64(v0);                                                \
	TRICE_PUT64(v1);                                                \
	TRICE_PUT64(v2);                                                \
	TRICE_PUT64(v3);                                                \
	TRICE_PUT64(v4);                                                \
	TRICE_PUT64(v5);                                                \
	TRICE_PUT64(v6);                                                \
	TRICE_PUT64(v7);                                                \
	TRICE_PUT64(v8);                                                \
	TRICE_PUT64(v9);                                                \
	TRICE_PUT64(v10);

#define TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_PUT64(v0);                                                     \
	TRICE_PUT64(v1);                                                     \
	TRICE_PUT64(v2);                                                     \
	TRICE_PUT64(v3);                                                     \
	TRICE_PUT64(v4);                                                     \
	TRICE_PUT64(v5);                                                     \
	TRICE_PUT64(v6);                                                     \
	TRICE_PUT64(v7);                                                     \
	TRICE_PUT64(v8);                                                     \
	TRICE_PUT64(v9);                                                     \
	TRICE_PUT64(v10);                                                    \
	TRICE_PUT64(v11);

// The following `TRICE` macro definitions uses the C preprocess to concatenate the code behind the sub-macros.
// The sequence `tid` in this context is executable code `id(n)`, `Id(n)` or `ID(n)` defined in trice.h.
// This works only with tid=`id(n)`, `Id(n)`, `ID(n)`.

//! TRICE64_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define TRICE_INSERT_TRICE64_0(tid, pFmt) \
	TRICE_ENTER tid;                      \
	TRICE_CNTC(0);                        \
	TRICE_LEAVE

//! TRICE64_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 64 bit value
#define TRICE_INSERT_TRICE64_1(tid, pFmt, v0) \
	TRICE_ENTER tid;                          \
	TRICE_CNTC(8);                            \
	TRICE_PUT64_1(v0)                         \
	TRICE_LEAVE

//! TRICE64_2 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 64 bit values
#define TRICE_INSERT_TRICE64_2(id, pFmt, v0, v1) \
	TRICE_ENTER id;                              \
	TRICE_CNTC(16);                              \
	TRICE_PUT64_2(v0, v1);                       \
	TRICE_LEAVE

//! TRICE64_3 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 64 bit values
#define TRICE_INSERT_TRICE64_3(id, pFmt, v0, v1, v2) \
	TRICE_ENTER id;                                  \
	TRICE_CNTC(24);                                  \
	TRICE_PUT64_3(v0, v1, v2);                       \
	TRICE_LEAVE

//! TRICE64_4 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 64 bit values
#define TRICE_INSERT_TRICE64_4(id, pFmt, v0, v1, v2, v3) \
	TRICE_ENTER id;                                      \
	TRICE_CNTC(32);                                      \
	TRICE_PUT64_4(v0, v1, v2, v3);                       \
	TRICE_LEAVE

//! TRICE64_5 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 64 bit values
#define TRICE_INSERT_TRICE64_5(id, pFmt, v0, v1, v2, v3, v4) \
	TRICE_ENTER id;                                          \
	TRICE_CNTC(40);                                          \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                       \
	TRICE_LEAVE

//! TRICE64_6 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 64 bit values
#define TRICE_INSERT_TRICE64_6(id, pFmt, v0, v1, v2, v3, v4, v5) \
	TRICE_ENTER id;                                              \
	TRICE_CNTC(48);                                              \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                       \
	TRICE_LEAVE

//! TRICE64_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 64 bit values
#define TRICE_INSERT_TRICE64_7(id, pFmt, v0, v1, v2, v3, v4, v5, v6) \
	TRICE_ENTER id;                                                  \
	TRICE_CNTC(56);                                                  \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);                       \
	TRICE_LEAVE

//! TRICE64_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 64 bit values
#define TRICE_INSERT_TRICE64_8(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_ENTER id;                                                      \
	TRICE_CNTC(64);                                                      \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);                       \
	TRICE_LEAVE

//! TRICE64_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 64 bit values
#define TRICE_INSERT_TRICE64_9(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER id;                                                          \
	TRICE_CNTC(72);                                                          \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                       \
	TRICE_LEAVE

//! TRICE64_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 64 bit values
#define TRICE_INSERT_TRICE64_10(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER id;                                                               \
	TRICE_CNTC(80);                                                               \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);                       \
	TRICE_LEAVE

//! TRICE64_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 64 bit values
#define TRICE_INSERT_TRICE64_11(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER id;                                                                    \
	TRICE_CNTC(88);                                                                    \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);                       \
	TRICE_LEAVE

//! TRICE64_12 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 64 bit values
#define TRICE_INSERT_TRICE64_12(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER id;                                                                         \
	TRICE_CNTC(96);                                                                         \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                        \
	TRICE_LEAVE

#endif // #ifndef TRICE64_H_
