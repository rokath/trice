/*! \file internalTrice16.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 16

// #define TRICE_0  TRICE16_0  //!< Default parameter bit width for 0  parameter count TRICE is 16.
#define TRICE_1 TRICE16_1   //!< Default parameter bit width for 1  parameter count TRICE is 16.
#define TRICE_2 TRICE16_2   //!< Default parameter bit width for 2  parameter count TRICE is 16.
#define TRICE_3 TRICE16_3   //!< Default parameter bit width for 3  parameter count TRICE is 16.
#define TRICE_4 TRICE16_4   //!< Default parameter bit width for 4  parameter count TRICE is 16.
#define TRICE_5 TRICE16_5   //!< Default parameter bit width for 5  parameter count TRICE is 16.
#define TRICE_6 TRICE16_6   //!< Default parameter bit width for 6  parameter count TRICE is 16.
#define TRICE_7 TRICE16_7   //!< Default parameter bit width for 7  parameter count TRICE is 16.
#define TRICE_8 TRICE16_8   //!< Default parameter bit width for 8  parameter count TRICE is 16.
#define TRICE_9 TRICE16_9   //!< Default parameter bit width for 9  parameter count TRICE is 16.
#define TRICE_10 TRICE16_10 //!< Default parameter bit width for 10 parameter count TRICE is 16.
#define TRICE_11 TRICE16_11 //!< Default parameter bit width for 11 parameter count TRICE is 16.
#define TRICE_12 TRICE16_12 //!< Default parameter bit width for 12 parameter count TRICE is 16.

// #define trice_0  trice16_0  //!< Default parameter bit width for 0  parameter count trice is 16.
#define trice_1 trice16_1   //!< Default parameter bit width for 1  parameter count trice is 16.
#define trice_2 trice16_2   //!< Default parameter bit width for 2  parameter count trice is 16.
#define trice_3 trice16_3   //!< Default parameter bit width for 3  parameter count trice is 16.
#define trice_4 trice16_4   //!< Default parameter bit width for 4  parameter count trice is 16.
#define trice_5 trice16_5   //!< Default parameter bit width for 5  parameter count trice is 16.
#define trice_6 trice16_6   //!< Default parameter bit width for 6  parameter count trice is 16.
#define trice_7 trice16_7   //!< Default parameter bit width for 7  parameter count trice is 16.
#define trice_8 trice16_8   //!< Default parameter bit width for 8  parameter count trice is 16.
#define trice_9 trice16_9   //!< Default parameter bit width for 9  parameter count trice is 16.
#define trice_10 trice16_10 //!< Default parameter bit width for 10 parameter count trice is 16.
#define trice_11 trice16_11 //!< Default parameter bit width for 11 parameter count trice is 16.
#define trice_12 trice16_12 //!< Default parameter bit width for 12 parameter count trice is 16.

// #define Trice_0  Trice16_0  //!< Default parameter bit width for 0  parameter count Trice is 16.
#define Trice_1 Trice16_1   //!< Default parameter bit width for 1  parameter count Trice is 16.
#define Trice_2 Trice16_2   //!< Default parameter bit width for 2  parameter count Trice is 16.
#define Trice_3 Trice16_3   //!< Default parameter bit width for 3  parameter count Trice is 16.
#define Trice_4 Trice16_4   //!< Default parameter bit width for 4  parameter count Trice is 16.
#define Trice_5 Trice16_5   //!< Default parameter bit width for 5  parameter count Trice is 16.
#define Trice_6 Trice16_6   //!< Default parameter bit width for 6  parameter count Trice is 16.
#define Trice_7 Trice16_7   //!< Default parameter bit width for 7  parameter count Trice is 16.
#define Trice_8 Trice16_8   //!< Default parameter bit width for 8  parameter count Trice is 16.
#define Trice_9 Trice16_9   //!< Default parameter bit width for 9  parameter count Trice is 16.
#define Trice_10 Trice16_10 //!< Default parameter bit width for 10 parameter count Trice is 16.
#define Trice_11 Trice16_11 //!< Default parameter bit width for 11 parameter count Trice is 16.
#define Trice_12 Trice16_12 //!< Default parameter bit width for 12 parameter count Trice is 16.

// #define TRice_0  TRice16_0  //!< Default parameter bit width for 0  parameter count TRice is 16.
#define TRice_1 TRice16_1   //!< Default parameter bit width for 1  parameter count TRice is 16.
#define TRice_2 TRice16_2   //!< Default parameter bit width for 2  parameter count TRice is 16.
#define TRice_3 TRice16_3   //!< Default parameter bit width for 3  parameter count TRice is 16.
#define TRice_4 TRice16_4   //!< Default parameter bit width for 4  parameter count TRice is 16.
#define TRice_5 TRice16_5   //!< Default parameter bit width for 5  parameter count TRice is 16.
#define TRice_6 TRice16_6   //!< Default parameter bit width for 6  parameter count TRice is 16.
#define TRice_7 TRice16_7   //!< Default parameter bit width for 7  parameter count TRice is 16.
#define TRice_8 TRice16_8   //!< Default parameter bit width for 8  parameter count TRice is 16.
#define TRice_9 TRice16_9   //!< Default parameter bit width for 9  parameter count TRice is 16.
#define TRice_10 TRice16_10 //!< Default parameter bit width for 10 parameter count TRice is 16.
#define TRice_11 TRice16_11 //!< Default parameter bit width for 11 parameter count TRice is 16.
#define TRice_12 TRice16_12 //!< Default parameter bit width for 12 parameter count TRice is 16.

#endif // #if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 16

#include "triceVariadic.h"

#define TRICE_SHORT0(v) TRICE_HTOTS((uint16_t)(v))                   //!< TRICE_SHORT0(v) is the 16-bit value v moved to the LLLL position in a 32-bit value 0xHHHH_LLLL.
#define TRICE_SHORT1(v) ((uint32_t)TRICE_HTOTS((uint16_t)(v)) << 16) //!< TRICE_SHORT1(v) is the 16-bit value v moved to the HHHH position in a 32-bit value 0xHHHH_LLLL.

#define TRICE16(tid, fmt, ...) TRICE_CONCAT2(TRICE16_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

//! TRICE16_B expects inside pFmt only one format specifier, which is used n times by using pFmt n times.
//!  It is usable for showing n 16-bit values.
#define TRICE16_B(id, pFmt, buf, n)    \
	do {                               \
		TRICE_N(id, pFmt, buf, 2 * n); \
	} while (0)

//! TRICE16_F expects inside pFmt just a string which is assumed to be a remote function name.
//!  The trice tool displays the pFmt string followed by n times (16-bit value i).
//!  The idea behind is to generate an id - function pointer referece list from the generated til.json file to
//!  compile it into a remote device and execute the inside pFmt named function remotely.
//!  Look for "TRICE16_F example" inside triceCheck.c.
#define TRICE16_F TRICE16_B

#define TRICE_PUT16_1(v0) TRICE_PUT(TRICE_SHORT0(v0));

#define TRICE_PUT16_2(v0, v1) TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1));

#define TRICE_PUT16_3(v0, v1, v2)                   \
	TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1)); \
	TRICE_PUT(TRICE_SHORT0(v2));

#define TRICE_PUT16_4(v0, v1, v2, v3)               \
	TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1)); \
	TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3));

#define TRICE_PUT16_5(v0, v1, v2, v3, v4)           \
	TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1)); \
	TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3)); \
	TRICE_PUT(TRICE_SHORT0(v4));

#define TRICE_PUT16_6(v0, v1, v2, v3, v4, v5)       \
	TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1)); \
	TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3)); \
	TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5));

#define TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6)   \
	TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1)); \
	TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3)); \
	TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5)); \
	TRICE_PUT(TRICE_SHORT0(v6));

#define TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1));   \
	TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3));   \
	TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5));   \
	TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7));

#define TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1));       \
	TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3));       \
	TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5));       \
	TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7));       \
	TRICE_PUT(TRICE_SHORT0(v8));

#define TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1));            \
	TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3));            \
	TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5));            \
	TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7));            \
	TRICE_PUT(TRICE_SHORT0(v8) | TRICE_SHORT1(v9));

#define TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1));                 \
	TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3));                 \
	TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5));                 \
	TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7));                 \
	TRICE_PUT(TRICE_SHORT0(v8) | TRICE_SHORT1(v9));                 \
	TRICE_PUT(TRICE_SHORT0(v10));

#define TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1));                      \
	TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3));                      \
	TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5));                      \
	TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7));                      \
	TRICE_PUT(TRICE_SHORT0(v8) | TRICE_SHORT1(v9));                      \
	TRICE_PUT(TRICE_SHORT0(v10) | TRICE_SHORT1(v11));

// The following `TRICE` macro definitions uses the C preprocess to concatenate the code behind the sub-macros.
// The sequence `tid` in this context is executable code `id(n)`, `Id(n)` or `ID(n)` defined in trice.h.
// This works only with tid=`id(n)`, `Id(n)`, `ID(n)`.

//! TRICE16_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define TRICE16_0(tid, pFmt) \
	TRICE_ENTER tid;         \
	TRICE_CNTC(0);           \
	TRICE_LEAVE

//! TRICE16_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define TRICE16_1(tid, pFmt, v0) \
	TRICE_ENTER tid;             \
	TRICE_CNTC(2);               \
	TRICE_PUT16_1(v0)            \
	TRICE_LEAVE

//! TRICE16_2 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 16 bit values
#define TRICE16_2(id, pFmt, v0, v1) \
	TRICE_ENTER id;                 \
	TRICE_CNTC(4);                  \
	TRICE_PUT16_2(v0, v1);          \
	TRICE_LEAVE

//! TRICE16_3 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 16 bit values
#define TRICE16_3(id, pFmt, v0, v1, v2) \
	TRICE_ENTER id;                     \
	TRICE_CNTC(6);                      \
	TRICE_PUT16_3(v0, v1, v2);          \
	TRICE_LEAVE

//! TRICE16_4 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 16 bit values
#define TRICE16_4(id, pFmt, v0, v1, v2, v3) \
	TRICE_ENTER id;                         \
	TRICE_CNTC(8);                          \
	TRICE_PUT16_4(v0, v1, v2, v3);          \
	TRICE_LEAVE

//! TRICE16_5 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 16 bit values
#define TRICE16_5(id, pFmt, v0, v1, v2, v3, v4) \
	TRICE_ENTER id;                             \
	TRICE_CNTC(10);                             \
	TRICE_PUT16_5(v0, v1, v2, v3, v4);          \
	TRICE_LEAVE

//! TRICE16_6 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 16 bit values
#define TRICE16_6(id, pFmt, v0, v1, v2, v3, v4, v5) \
	TRICE_ENTER id;                                 \
	TRICE_CNTC(12);                                 \
	TRICE_PUT16_6(v0, v1, v2, v3, v4, v5);          \
	TRICE_LEAVE

//! TRICE16_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 16 bit values
#define TRICE16_7(id, pFmt, v0, v1, v2, v3, v4, v5, v6) \
	TRICE_ENTER id;                                     \
	TRICE_CNTC(14);                                     \
	TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6);          \
	TRICE_LEAVE

//! TRICE16_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 16 bit values
#define TRICE16_8(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_ENTER id;                                         \
	TRICE_CNTC(16);                                         \
	TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7);          \
	TRICE_LEAVE

//! TRICE16_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 16 bit values
#define TRICE16_9(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER id;                                             \
	TRICE_CNTC(18);                                             \
	TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);          \
	TRICE_LEAVE

//! TRICE16_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 16 bit values
#define TRICE16_10(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER id;                                                  \
	TRICE_CNTC(20);                                                  \
	TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);          \
	TRICE_LEAVE

//! TRICE16_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 16 bit values
#define TRICE16_11(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER id;                                                       \
	TRICE_CNTC(22);                                                       \
	TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);          \
	TRICE_LEAVE

//! TRICE16_12 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 16 bit values
#define TRICE16_12(id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER id;                                                            \
	TRICE_CNTC(24);                                                            \
	TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)           \
	TRICE_LEAVE
