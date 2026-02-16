/*! \file trice64.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE64_H_
#define TRICE64_H_

#if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 64

// #define TRICE_0  TRICE64_0  //!< Default parameter bit width for 0  parameter count TRICE is 64.
#define TRICE_1 TRICE64_1   //!< Default parameter bit width for 1  parameter count TRICE is 64.
#define TRICE_2 TRICE64_2   //!< Default parameter bit width for 2  parameter count TRICE is 64.
#define TRICE_3 TRICE64_3   //!< Default parameter bit width for 3  parameter count TRICE is 64.
#define TRICE_4 TRICE64_4   //!< Default parameter bit width for 4  parameter count TRICE is 64.
#define TRICE_5 TRICE64_5   //!< Default parameter bit width for 5  parameter count TRICE is 64.
#define TRICE_6 TRICE64_6   //!< Default parameter bit width for 6  parameter count TRICE is 64.
#define TRICE_7 TRICE64_7   //!< Default parameter bit width for 7  parameter count TRICE is 64.
#define TRICE_8 TRICE64_8   //!< Default parameter bit width for 8  parameter count TRICE is 64.
#define TRICE_9 TRICE64_9   //!< Default parameter bit width for 9  parameter count TRICE is 64.
#define TRICE_10 TRICE64_10 //!< Default parameter bit width for 10 parameter count TRICE is 64.
#define TRICE_11 TRICE64_11 //!< Default parameter bit width for 11 parameter count TRICE is 64.
#define TRICE_12 TRICE64_12 //!< Default parameter bit width for 12 parameter count TRICE is 64.

// #define trice_0  trice64_0  //!< Default parameter bit width for 1  parameter count trice is 64.
#define trice_1 trice64_1   //!< Default parameter bit width for 1  parameter count trice is 64.
#define trice_2 trice64_2   //!< Default parameter bit width for 2  parameter count trice is 64.
#define trice_3 trice64_3   //!< Default parameter bit width for 3  parameter count trice is 64.
#define trice_4 trice64_4   //!< Default parameter bit width for 4  parameter count trice is 64.
#define trice_5 trice64_5   //!< Default parameter bit width for 5  parameter count trice is 64.
#define trice_6 trice64_6   //!< Default parameter bit width for 6  parameter count trice is 64.
#define trice_7 trice64_7   //!< Default parameter bit width for 7  parameter count trice is 64.
#define trice_8 trice64_8   //!< Default parameter bit width for 8  parameter count trice is 64.
#define trice_9 trice64_9   //!< Default parameter bit width for 9  parameter count trice is 64.
#define trice_10 trice64_10 //!< Default parameter bit width for 10 parameter count trice is 64.
#define trice_11 trice64_11 //!< Default parameter bit width for 11 parameter count trice is 64.
#define trice_12 trice64_12 //!< Default parameter bit width for 12 parameter count trice is 64.

// #define Trice_0  Trice64_0  //!< Default parameter bit width for 0  parameter count Trice is 64.
#define Trice_1 Trice64_1   //!< Default parameter bit width for 1  parameter count Trice is 64.
#define Trice_2 Trice64_2   //!< Default parameter bit width for 2  parameter count Trice is 64.
#define Trice_3 Trice64_3   //!< Default parameter bit width for 3  parameter count Trice is 64.
#define Trice_4 Trice64_4   //!< Default parameter bit width for 4  parameter count Trice is 64.
#define Trice_5 Trice64_5   //!< Default parameter bit width for 5  parameter count Trice is 64.
#define Trice_6 Trice64_6   //!< Default parameter bit width for 6  parameter count Trice is 64.
#define Trice_7 Trice64_7   //!< Default parameter bit width for 7  parameter count Trice is 64.
#define Trice_8 Trice64_8   //!< Default parameter bit width for 8  parameter count Trice is 64.
#define Trice_9 Trice64_9   //!< Default parameter bit width for 9  parameter count Trice is 64.
#define Trice_10 Trice64_10 //!< Default parameter bit width for 10 parameter count Trice is 64.
#define Trice_11 Trice64_11 //!< Default parameter bit width for 11 parameter count Trice is 64.
#define Trice_12 Trice64_12 //!< Default parameter bit width for 12 parameter count Trice is 64.

// #define TRice_0  TRice64_0  //!< Default parameter bit width for 0  parameter count TRice is 64.
#define TRice_1 TRice64_1   //!< Default parameter bit width for 1  parameter count TRice is 64.
#define TRice_2 TRice64_2   //!< Default parameter bit width for 2  parameter count TRice is 64.
#define TRice_3 TRice64_3   //!< Default parameter bit width for 3  parameter count TRice is 64.
#define TRice_4 TRice64_4   //!< Default parameter bit width for 4  parameter count TRice is 64.
#define TRice_5 TRice64_5   //!< Default parameter bit width for 5  parameter count TRice is 64.
#define TRice_6 TRice64_6   //!< Default parameter bit width for 6  parameter count TRice is 64.
#define TRice_7 TRice64_7   //!< Default parameter bit width for 7  parameter count TRice is 64.
#define TRice_8 TRice64_8   //!< Default parameter bit width for 8  parameter count TRice is 64.
#define TRice_9 TRice64_9   //!< Default parameter bit width for 9  parameter count TRice is 64.
#define TRice_10 TRice64_10 //!< Default parameter bit width for 10 parameter count TRice is 64.
#define TRice_11 TRice64_11 //!< Default parameter bit width for 11 parameter count TRice is 64.
#define TRice_12 TRice64_12 //!< Default parameter bit width for 12 parameter count TRice is 64.

#endif // #if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 64

#include "triceVariadic.h"

#define TRICE64(tid, fmt, ...) TRICE_CONCAT2(TRICE64_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

//! TRICE64_B expects inside pFmt only one format specifier, which is used n times by using pFmt n times.
//!  It is usable for showing n 64-bit values.
#define TRICE64_B(id, pFmt, buf, n)    \
	do {                               \
		TRICE_N(id, pFmt, buf, 8 * (n)); \
	} while (0)

//! TRICE64_F expects inside pFmt just a string which is assumed to be a remote function name.
//!  The trice tool displays the pFmt string followed by n times (64-bit value i).
//!  The idea behind is to generate an id - function pointer referece list from the generated til.json file to
//!  compile it into a remote device and execute the inside pFmt named function remotely.
//!  Look for "TRICE64_F example" inside triceCheck.c.
#define TRICE64_F TRICE64_B

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
#define TRICE64_0(tid, pFmt) \
	TRICE_ENTER tid;         \
	TRICE_CNTC(0);           \
	TRICE_LEAVE

//! TRICE64_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 64 bit value
#define TRICE64_1(tid, pFmt, v0) \
	TRICE_ENTER tid;             \
	TRICE_CNTC(8);               \
	TRICE_PUT64_1(v0)            \
	TRICE_LEAVE

//! TRICE64_2 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 64 bit values
#define TRICE64_2(id, pFmt, v0, v1) \
	TRICE_ENTER id;                 \
	TRICE_CNTC(16);                 \
	TRICE_PUT64_2(v0, v1);          \
	TRICE_LEAVE

//! TRICE64_3 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 64 bit values
#define TRICE64_3(id, pFmt, v0, v1, v2) \
	TRICE_ENTER id;                     \
	TRICE_CNTC(24);                     \
	TRICE_PUT64_3(v0, v1, v2);          \
	TRICE_LEAVE

//! TRICE64_4 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 64 bit values
#define TRICE64_4(id, pFmt, v0, v1, v2, v3) \
	TRICE_ENTER id;                         \
	TRICE_CNTC(32);                         \
	TRICE_PUT64_4(v0, v1, v2, v3);          \
	TRICE_LEAVE

//! TRICE64_5 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 64 bit values
#define TRICE64_5(id, pFmt, v0, v1, v2, v3, v4) \
	TRICE_ENTER id;                             \
	TRICE_CNTC(40);                             \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);          \
	TRICE_LEAVE

//! TRICE64_6 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 64 bit values
#define TRICE64_6(id, pFmt, v0, v1, v2, v3, v4, v5) \
	TRICE_ENTER id;                                 \
	TRICE_CNTC(48);                                 \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);          \
	TRICE_LEAVE

//! TRICE64_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 64 bit values
#define TRICE64_7(id, pFmt, v0, v1, v2, v3, v4, v5, v6) \
	TRICE_ENTER id;                                     \
	TRICE_CNTC(56);                                     \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);          \
	TRICE_LEAVE

//! TRICE64_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 64 bit values
#define TRICE64_8(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_ENTER id;                                         \
	TRICE_CNTC(64);                                         \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);          \
	TRICE_LEAVE

//! TRICE64_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 64 bit values
#define TRICE64_9(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER id;                                             \
	TRICE_CNTC(72);                                             \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);          \
	TRICE_LEAVE

//! TRICE64_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 64 bit values
#define TRICE64_10(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER id;                                                  \
	TRICE_CNTC(80);                                                  \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);          \
	TRICE_LEAVE

//! TRICE64_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 64 bit values
#define TRICE64_11(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER id;                                                       \
	TRICE_CNTC(88);                                                       \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);          \
	TRICE_LEAVE

//! TRICE64_12 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 64 bit values
#define TRICE64_12(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER id;                                                            \
	TRICE_CNTC(96);                                                            \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)           \
	TRICE_LEAVE

#endif // #ifndef TRICE64_H_