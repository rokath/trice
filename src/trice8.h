/*! \file Trice8.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 8

// #define TRICE_0  TRICE8_0  //!< Default parameter bit width for 0  parameter count TRICE is 8.
#define TRICE_1 TRICE8_1   //!< Default parameter bit width for 1  parameter count TRICE is 8.
#define TRICE_2 TRICE8_2   //!< Default parameter bit width for 2  parameter count TRICE is 8.
#define TRICE_3 TRICE8_3   //!< Default parameter bit width for 3  parameter count TRICE is 8.
#define TRICE_4 TRICE8_4   //!< Default parameter bit width for 4  parameter count TRICE is 8.
#define TRICE_5 TRICE8_5   //!< Default parameter bit width for 5  parameter count TRICE is 8.
#define TRICE_6 TRICE8_6   //!< Default parameter bit width for 6  parameter count TRICE is 8.
#define TRICE_7 TRICE8_7   //!< Default parameter bit width for 7  parameter count TRICE is 8.
#define TRICE_8 TRICE8_8   //!< Default parameter bit width for 8  parameter count TRICE is 8.
#define TRICE_9 TRICE8_9   //!< Default parameter bit width for 9  parameter count TRICE is 8.
#define TRICE_10 TRICE8_10 //!< Default parameter bit width for 10 parameter count TRICE is 8.
#define TRICE_11 TRICE8_11 //!< Default parameter bit width for 11 parameter count TRICE is 8.
#define TRICE_12 TRICE8_12 //!< Default parameter bit width for 12 parameter count TRICE is 8.

// #define trice_0  trice8_0  //!< Default parameter bit width for 0  parameter count trice is 8.
#define trice_1 trice8_1   //!< Default parameter bit width for 1  parameter count trice is 8.
#define trice_2 trice8_2   //!< Default parameter bit width for 2  parameter count trice is 8.
#define trice_3 trice8_3   //!< Default parameter bit width for 3  parameter count trice is 8.
#define trice_4 trice8_4   //!< Default parameter bit width for 4  parameter count trice is 8.
#define trice_5 trice8_5   //!< Default parameter bit width for 5  parameter count trice is 8.
#define trice_6 trice8_6   //!< Default parameter bit width for 6  parameter count trice is 8.
#define trice_7 trice8_7   //!< Default parameter bit width for 7  parameter count trice is 8.
#define trice_8 trice8_8   //!< Default parameter bit width for 8  parameter count trice is 8.
#define trice_9 trice8_9   //!< Default parameter bit width for 9  parameter count trice is 8.
#define trice_10 trice8_10 //!< Default parameter bit width for 10 parameter count trice is 8.
#define trice_11 trice8_11 //!< Default parameter bit width for 11 parameter count trice is 8.
#define trice_12 trice8_12 //!< Default parameter bit width for 12 parameter count trice is 8.

// #define Trice_0  Trice8_0  //!< Default parameter bit width for 1  parameter count Trice is 8.
#define Trice_1 Trice8_1   //!< Default parameter bit width for 1  parameter count Trice is 8.
#define Trice_2 Trice8_2   //!< Default parameter bit width for 2  parameter count Trice is 8.
#define Trice_3 Trice8_3   //!< Default parameter bit width for 3  parameter count Trice is 8.
#define Trice_4 Trice8_4   //!< Default parameter bit width for 4  parameter count Trice is 8.
#define Trice_5 Trice8_5   //!< Default parameter bit width for 5  parameter count Trice is 8.
#define Trice_6 Trice8_6   //!< Default parameter bit width for 6  parameter count Trice is 8.
#define Trice_7 Trice8_7   //!< Default parameter bit width for 7  parameter count Trice is 8.
#define Trice_8 Trice8_8   //!< Default parameter bit width for 8  parameter count Trice is 8.
#define Trice_9 Trice8_9   //!< Default parameter bit width for 9  parameter count Trice is 8.
#define Trice_10 Trice8_10 //!< Default parameter bit width for 10 parameter count Trice is 8.
#define Trice_11 Trice8_11 //!< Default parameter bit width for 11 parameter count Trice is 8.
#define Trice_12 Trice8_12 //!< Default parameter bit width for 12 parameter count Trice is 8.

// #define TRice_0  TRice8_0  //!< Default parameter bit width for 1  parameter count TRice is 8.
#define TRice_1 TRice8_1   //!< Default parameter bit width for 1  parameter count TRice is 8.
#define TRice_2 TRice8_2   //!< Default parameter bit width for 2  parameter count TRice is 8.
#define TRice_3 TRice8_3   //!< Default parameter bit width for 3  parameter count TRice is 8.
#define TRice_4 TRice8_4   //!< Default parameter bit width for 4  parameter count TRice is 8.
#define TRice_5 TRice8_5   //!< Default parameter bit width for 5  parameter count TRice is 8.
#define TRice_6 TRice8_6   //!< Default parameter bit width for 6  parameter count TRice is 8.
#define TRice_7 TRice8_7   //!< Default parameter bit width for 7  parameter count TRice is 8.
#define TRice_8 TRice8_8   //!< Default parameter bit width for 8  parameter count TRice is 8.
#define TRice_9 TRice8_9   //!< Default parameter bit width for 9  parameter count TRice is 8.
#define TRice_10 TRice8_10 //!< Default parameter bit width for 10 parameter count TRice is 8.
#define TRice_11 TRice8_11 //!< Default parameter bit width for 11 parameter count TRice is 8.
#define TRice_12 TRice8_12 //!< Default parameter bit width for 12 parameter count TRice is 8.

#endif // #if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 8

#include "triceVariadic.h"

//! TRICE8 inlining code for up to 12 8-bit parameters.
#define TRICE8(tid, fmt, ...) TRICE_CONCAT2(TRICE8_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

//! TRICE8_B expects inside pFmt only one format specifier, which is used n times by using pFmt n times.
//!  It is usable for showing n 8-bit values.
#define TRICE8_B TRICE_N

//! TRICE8_F expects inside pFmt just a string which is assumed to be a remote function name.
//! \li The trice tool displays the pFmt string followed by n times (value i).
//! \li The idea behind is to generate an id - function pointer referece list from the generated til.json file to compile it into a remote device and execute the inside pFmt named function remotely.
//! \li Look for "TRICE8_F example" inside triceCheck.c.
//!  Look for "TRICE8_F example" inside triceCheck.c.
#define TRICE8_F TRICE8_B

#define TRICE_BYTE0(v) ((uint8_t)(v))                       //!< TRICE_BYTE0(v) is the 8-bit value v moved to the LL position in a 32-bit value 0xHH_HL_LH_LL.
#define TRICE_BYTE1(v) (0x0000FF00 & ((uint32_t)(v) << 8))  //!< TRICE_BYTE1(v) is the 8-bit value v moved to the LH position in a 32-bit value 0xHH_HL_LH_LL.
#define TRICE_BYTE2(v) (0x00FF0000 & ((uint32_t)(v) << 16)) //!< TRICE_BYTE2(v) is the 8-bit value v moved to the HL position in a 32-bit value 0xHH_HL_LH_LL.
#define TRICE_BYTE3(v) ((uint32_t)(v) << 24)                //!< TRICE_BYTE3(v) is the 8-bit value v moved to the HH position in a 32-bit value 0xHH_HL_LH_LL.

//! TRICE_PUT8_1 transfers a single 8-bit parameter value to the Trice buffer.
#define TRICE_PUT8_1(v0) TRICE_PUT(TRICE_BYTE0(v0));

#define TRICE_PUT8_2(v0, v1) TRICE_PUT(TRICE_BYTE1(v1) | TRICE_BYTE0(v0));

#define TRICE_PUT8_3(v0, v1, v2) TRICE_PUT(TRICE_BYTE2(v2) | TRICE_BYTE1(v1) | TRICE_BYTE0(v0));

#define TRICE_PUT8_4(v0, v1, v2, v3) TRICE_PUT(TRICE_BYTE3(v3) | TRICE_BYTE2(v2) | TRICE_BYTE1(v1) | TRICE_BYTE0(v0));

#define TRICE_PUT8_5(v0, v1, v2, v3, v4)                                              \
	TRICE_PUT(TRICE_BYTE3(v3) | TRICE_BYTE2(v2) | TRICE_BYTE1(v1) | TRICE_BYTE0(v0)); \
	TRICE_PUT(TRICE_BYTE0(v4));

#define TRICE_PUT8_6(v0, v1, v2, v3, v4, v5)                                          \
	TRICE_PUT(TRICE_BYTE3(v3) | TRICE_BYTE2(v2) | TRICE_BYTE1(v1) | TRICE_BYTE0(v0)); \
	TRICE_PUT(TRICE_BYTE1(v5) | TRICE_BYTE0(v4));

#define TRICE_PUT8_7(v0, v1, v2, v3, v4, v5, v6)                                      \
	TRICE_PUT(TRICE_BYTE3(v3) | TRICE_BYTE2(v2) | TRICE_BYTE1(v1) | TRICE_BYTE0(v0)); \
	TRICE_PUT(TRICE_BYTE2(v6) | TRICE_BYTE1(v5) | TRICE_BYTE0(v4));

#define TRICE_PUT8_8(v0, v1, v2, v3, v4, v5, v6, v7)                                  \
	TRICE_PUT(TRICE_BYTE3(v3) | TRICE_BYTE2(v2) | TRICE_BYTE1(v1) | TRICE_BYTE0(v0)); \
	TRICE_PUT(TRICE_BYTE3(v7) | TRICE_BYTE2(v6) | TRICE_BYTE1(v5) | TRICE_BYTE0(v4));

#define TRICE_PUT8_9(v0, v1, v2, v3, v4, v5, v6, v7, v8)                              \
	TRICE_PUT(TRICE_BYTE3(v3) | TRICE_BYTE2(v2) | TRICE_BYTE1(v1) | TRICE_BYTE0(v0)); \
	TRICE_PUT(TRICE_BYTE3(v7) | TRICE_BYTE2(v6) | TRICE_BYTE1(v5) | TRICE_BYTE0(v4)); \
	TRICE_PUT(TRICE_BYTE0(v8));

#define TRICE_PUT8_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)                         \
	TRICE_PUT(TRICE_BYTE3(v3) | TRICE_BYTE2(v2) | TRICE_BYTE1(v1) | TRICE_BYTE0(v0)); \
	TRICE_PUT(TRICE_BYTE3(v7) | TRICE_BYTE2(v6) | TRICE_BYTE1(v5) | TRICE_BYTE0(v4)); \
	TRICE_PUT(TRICE_BYTE1(v9) | TRICE_BYTE0(v8));

#define TRICE_PUT8_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)                    \
	TRICE_PUT(TRICE_BYTE3(v3) | TRICE_BYTE2(v2) | TRICE_BYTE1(v1) | TRICE_BYTE0(v0)); \
	TRICE_PUT(TRICE_BYTE3(v7) | TRICE_BYTE2(v6) | TRICE_BYTE1(v5) | TRICE_BYTE0(v4)); \
	TRICE_PUT(TRICE_BYTE2(v10) | TRICE_BYTE1(v9) | TRICE_BYTE0(v8));

#define TRICE_PUT8_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)               \
	TRICE_PUT(TRICE_BYTE3(v3) | TRICE_BYTE2(v2) | TRICE_BYTE1(v1) | TRICE_BYTE0(v0)); \
	TRICE_PUT(TRICE_BYTE3(v7) | TRICE_BYTE2(v6) | TRICE_BYTE1(v5) | TRICE_BYTE0(v4)); \
	TRICE_PUT(TRICE_BYTE3(v11) | TRICE_BYTE2(v10) | TRICE_BYTE1(v9) | TRICE_BYTE0(v8));

// The following `TRICE` macro definitions are using the C preprocess to concatenate the code behind the sub-macros.
// The sequence `tid` in this context is executable code `id(n)`, `Id(n)` or `ID(n)` defined in trice.h.
// This works only with tid=`id(n)`, `Id(n)`, `ID(n)`.

//! TRICE8_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define TRICE8_0(tid, pFmt) \
	TRICE_ENTER tid;        \
	TRICE_CNTC(0);          \
	TRICE_LEAVE

//! TRICE8_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define TRICE8_1(tid, pFmt, v0) \
	TRICE_ENTER tid;            \
	TRICE_CNTC(1);              \
	TRICE_PUT8_1(v0)            \
	TRICE_LEAVE

//! TRICE8_2 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 8 bit bit values
#define TRICE8_2(tid, pFmt, v0, v1) \
	TRICE_ENTER tid;                \
	TRICE_CNTC(2);                  \
	TRICE_PUT8_2(v0, v1);           \
	TRICE_LEAVE

//! TRICE8_3 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 8 bit bit values
#define TRICE8_3(tid, pFmt, v0, v1, v2) \
	TRICE_ENTER tid;                    \
	TRICE_CNTC(3);                      \
	TRICE_PUT8_3(v0, v1, v2);           \
	TRICE_LEAVE

//! TRICE8_4 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 8 bit bit values
#define TRICE8_4(tid, pFmt, v0, v1, v2, v3) \
	TRICE_ENTER tid;                        \
	TRICE_CNTC(4);                          \
	TRICE_PUT8_4(v0, v1, v2, v3);           \
	TRICE_LEAVE

//! TRICE8_5 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 8 bit bit values
#define TRICE8_5(tid, pFmt, v0, v1, v2, v3, v4) \
	TRICE_ENTER tid;                            \
	TRICE_CNTC(5);                              \
	TRICE_PUT8_5(v0, v1, v2, v3, v4);           \
	TRICE_LEAVE

//! TRICE8_6 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 8 bit bit values
#define TRICE8_6(tid, pFmt, v0, v1, v2, v3, v4, v5) \
	TRICE_ENTER tid;                                \
	TRICE_CNTC(6);                                  \
	TRICE_PUT8_6(v0, v1, v2, v3, v4, v5);           \
	TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 8 bit bit values
#define TRICE8_7(tid, pFmt, v0, v1, v2, v3, v4, v5, v6) \
	TRICE_ENTER tid;                                    \
	TRICE_CNTC(7);                                      \
	TRICE_PUT8_7(v0, v1, v2, v3, v4, v5, v6);           \
	TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_8(tid, pFmt, v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_ENTER tid;                                        \
	TRICE_CNTC(8);                                          \
	TRICE_PUT8_8(v0, v1, v2, v3, v4, v5, v6, v7);           \
	TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_9(tid, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER tid;                                            \
	TRICE_CNTC(9);                                              \
	TRICE_PUT8_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);           \
	TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_10(tid, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER tid;                                                 \
	TRICE_CNTC(10);                                                  \
	TRICE_PUT8_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);           \
	TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_11(tid, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER tid;                                                      \
	TRICE_CNTC(11);                                                       \
	TRICE_PUT8_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);           \
	TRICE_LEAVE

//! TRICE8_12 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 8 bit bit values
#define TRICE8_12(tid, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER tid;                                                           \
	TRICE_CNTC(12);                                                            \
	TRICE_PUT8_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)            \
	TRICE_LEAVE
