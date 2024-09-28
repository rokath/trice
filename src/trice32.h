/*! \file internalTrice32.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 32

// #define TRICE_0  TRICE32_0  //!< Default parameter bit width for 0  parameter count TRICE is 32.
#define TRICE_1 TRICE32_1   //!< Default parameter bit width for 1  parameter count TRICE is 32.
#define TRICE_2 TRICE32_2   //!< Default parameter bit width for 2  parameter count TRICE is 32.
#define TRICE_3 TRICE32_3   //!< Default parameter bit width for 3  parameter count TRICE is 32.
#define TRICE_4 TRICE32_4   //!< Default parameter bit width for 4  parameter count TRICE is 32.
#define TRICE_5 TRICE32_5   //!< Default parameter bit width for 5  parameter count TRICE is 32.
#define TRICE_6 TRICE32_6   //!< Default parameter bit width for 6  parameter count TRICE is 32.
#define TRICE_7 TRICE32_7   //!< Default parameter bit width for 7  parameter count TRICE is 32.
#define TRICE_8 TRICE32_8   //!< Default parameter bit width for 8  parameter count TRICE is 32.
#define TRICE_9 TRICE32_9   //!< Default parameter bit width for 9  parameter count TRICE is 32.
#define TRICE_10 TRICE32_10 //!< Default parameter bit width for 10 parameter count TRICE is 32.
#define TRICE_11 TRICE32_11 //!< Default parameter bit width for 11 parameter count TRICE is 32.
#define TRICE_12 TRICE32_12 //!< Default parameter bit width for 12 parameter count TRICE is 32.

// #define trice_0  trice32_0  //!< Default parameter bit width for 0  parameter count trice is 32.
#define trice_1 trice32_1   //!< Default parameter bit width for 1  parameter count trice is 32.
#define trice_2 trice32_2   //!< Default parameter bit width for 2  parameter count trice is 32.
#define trice_3 trice32_3   //!< Default parameter bit width for 3  parameter count trice is 32.
#define trice_4 trice32_4   //!< Default parameter bit width for 4  parameter count trice is 32.
#define trice_5 trice32_5   //!< Default parameter bit width for 5  parameter count trice is 32.
#define trice_6 trice32_6   //!< Default parameter bit width for 6  parameter count trice is 32.
#define trice_7 trice32_7   //!< Default parameter bit width for 7  parameter count trice is 32.
#define trice_8 trice32_8   //!< Default parameter bit width for 8  parameter count trice is 32.
#define trice_9 trice32_9   //!< Default parameter bit width for 9  parameter count trice is 32.
#define trice_10 trice32_10 //!< Default parameter bit width for 10 parameter count trice is 32.
#define trice_11 trice32_11 //!< Default parameter bit width for 11 parameter count trice is 32.
#define trice_12 trice32_12 //!< Default parameter bit width for 12 parameter count trice is 32.

// #define Trice_0  Trice32_0  //!< Default parameter bit width for 0  parameter count Trice is 32.
#define Trice_1 Trice32_1   //!< Default parameter bit width for 1  parameter count Trice is 32.
#define Trice_2 Trice32_2   //!< Default parameter bit width for 2  parameter count Trice is 32.
#define Trice_3 Trice32_3   //!< Default parameter bit width for 3  parameter count Trice is 32.
#define Trice_4 Trice32_4   //!< Default parameter bit width for 4  parameter count Trice is 32.
#define Trice_5 Trice32_5   //!< Default parameter bit width for 5  parameter count Trice is 32.
#define Trice_6 Trice32_6   //!< Default parameter bit width for 6  parameter count Trice is 32.
#define Trice_7 Trice32_7   //!< Default parameter bit width for 7  parameter count Trice is 32.
#define Trice_8 Trice32_8   //!< Default parameter bit width for 8  parameter count Trice is 32.
#define Trice_9 Trice32_9   //!< Default parameter bit width for 9  parameter count Trice is 32.
#define Trice_10 Trice32_10 //!< Default parameter bit width for 10 parameter count Trice is 32.
#define Trice_11 Trice32_11 //!< Default parameter bit width for 11 parameter count Trice is 32.
#define Trice_12 Trice32_12 //!< Default parameter bit width for 12 parameter count Trice is 32.

// #define TRice_0  TRice32_0  //!< Default parameter bit width for 0  parameter count TRice is 32.
#define TRice_1 TRice32_1   //!< Default parameter bit width for 1  parameter count TRice is 32.
#define TRice_2 TRice32_2   //!< Default parameter bit width for 2  parameter count TRice is 32.
#define TRice_3 TRice32_3   //!< Default parameter bit width for 3  parameter count TRice is 32.
#define TRice_4 TRice32_4   //!< Default parameter bit width for 4  parameter count TRice is 32.
#define TRice_5 TRice32_5   //!< Default parameter bit width for 5  parameter count TRice is 32.
#define TRice_6 TRice32_6   //!< Default parameter bit width for 6  parameter count TRice is 32.
#define TRice_7 TRice32_7   //!< Default parameter bit width for 7  parameter count TRice is 32.
#define TRice_8 TRice32_8   //!< Default parameter bit width for 8  parameter count TRice is 32.
#define TRice_9 TRice32_9   //!< Default parameter bit width for 9  parameter count TRice is 32.
#define TRice_10 TRice32_10 //!< Default parameter bit width for 10 parameter count TRice is 32.
#define TRice_11 TRice32_11 //!< Default parameter bit width for 11 parameter count TRice is 32.
#define TRice_12 TRice32_12 //!< Default parameter bit width for 12 parameter count TRice is 32.

#endif // #if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 32

#define TRICE32(tid, fmt, ...) TRICE_CONCAT2(TRICE32_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

//! TRICE32_B expects inside pFmt only one format specifier, which is used n times by using pFmt n times.
//!  It is usable for showing n 32-bit values.
#define TRICE32_B(id, pFmt, buf, n)    \
	do {                               \
		TRICE_N(id, pFmt, buf, 4 * n); \
	} while (0)

//! TRICE32_F expects inside pFmt just a string which is assumed to be a remote function name.
//!  The trice tool displays the pFmt string followed by n times (32-bit value i).
//!  The idea behind is to generate an id - function pointer referece list from the generated til.json file to
//!  compile it into a remote device and execute the inside pFmt named function remotely.
//!  Look for "TRICE32_F example" inside triceCheck.c.
#define TRICE32_F TRICE32_B

#define TRICE_PUT32_1(v0) TRICE_PUT(TRICE_HTOTL(v0));

#define TRICE_PUT32_2(v0, v1)   \
	TRICE_PUT(TRICE_HTOTL(v0)); \
	TRICE_PUT(TRICE_HTOTL(v1));

#define TRICE_PUT32_3(v0, v1, v2) \
	TRICE_PUT(TRICE_HTOTL(v0));   \
	TRICE_PUT(TRICE_HTOTL(v1));   \
	TRICE_PUT(TRICE_HTOTL(v2));

#define TRICE_PUT32_4(v0, v1, v2, v3) \
	TRICE_PUT(TRICE_HTOTL(v0));       \
	TRICE_PUT(TRICE_HTOTL(v1));       \
	TRICE_PUT(TRICE_HTOTL(v2));       \
	TRICE_PUT(TRICE_HTOTL(v3));

#define TRICE_PUT32_5(v0, v1, v2, v3, v4) \
	TRICE_PUT(TRICE_HTOTL(v0));           \
	TRICE_PUT(TRICE_HTOTL(v1));           \
	TRICE_PUT(TRICE_HTOTL(v2));           \
	TRICE_PUT(TRICE_HTOTL(v3));           \
	TRICE_PUT(TRICE_HTOTL(v4));

#define TRICE_PUT32_6(v0, v1, v2, v3, v4, v5) \
	TRICE_PUT(TRICE_HTOTL(v0));               \
	TRICE_PUT(TRICE_HTOTL(v1));               \
	TRICE_PUT(TRICE_HTOTL(v2));               \
	TRICE_PUT(TRICE_HTOTL(v3));               \
	TRICE_PUT(TRICE_HTOTL(v4));               \
	TRICE_PUT(TRICE_HTOTL(v5));

#define TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6) \
	TRICE_PUT(TRICE_HTOTL(v0));                   \
	TRICE_PUT(TRICE_HTOTL(v1));                   \
	TRICE_PUT(TRICE_HTOTL(v2));                   \
	TRICE_PUT(TRICE_HTOTL(v3));                   \
	TRICE_PUT(TRICE_HTOTL(v4));                   \
	TRICE_PUT(TRICE_HTOTL(v5));                   \
	TRICE_PUT(TRICE_HTOTL(v6));

#define TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_PUT(TRICE_HTOTL(v0));                       \
	TRICE_PUT(TRICE_HTOTL(v1));                       \
	TRICE_PUT(TRICE_HTOTL(v2));                       \
	TRICE_PUT(TRICE_HTOTL(v3));                       \
	TRICE_PUT(TRICE_HTOTL(v4));                       \
	TRICE_PUT(TRICE_HTOTL(v5));                       \
	TRICE_PUT(TRICE_HTOTL(v6));                       \
	TRICE_PUT(TRICE_HTOTL(v7));

#define TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_PUT(TRICE_HTOTL(v0));                           \
	TRICE_PUT(TRICE_HTOTL(v1));                           \
	TRICE_PUT(TRICE_HTOTL(v2));                           \
	TRICE_PUT(TRICE_HTOTL(v3));                           \
	TRICE_PUT(TRICE_HTOTL(v4));                           \
	TRICE_PUT(TRICE_HTOTL(v5));                           \
	TRICE_PUT(TRICE_HTOTL(v6));                           \
	TRICE_PUT(TRICE_HTOTL(v7));                           \
	TRICE_PUT(TRICE_HTOTL(v8));

#define TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_PUT(TRICE_HTOTL(v0));                                \
	TRICE_PUT(TRICE_HTOTL(v1));                                \
	TRICE_PUT(TRICE_HTOTL(v2));                                \
	TRICE_PUT(TRICE_HTOTL(v3));                                \
	TRICE_PUT(TRICE_HTOTL(v4));                                \
	TRICE_PUT(TRICE_HTOTL(v5));                                \
	TRICE_PUT(TRICE_HTOTL(v6));                                \
	TRICE_PUT(TRICE_HTOTL(v7));                                \
	TRICE_PUT(TRICE_HTOTL(v8));                                \
	TRICE_PUT(TRICE_HTOTL(v9));

#define TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_PUT(TRICE_HTOTL(v0));                                     \
	TRICE_PUT(TRICE_HTOTL(v1));                                     \
	TRICE_PUT(TRICE_HTOTL(v2));                                     \
	TRICE_PUT(TRICE_HTOTL(v3));                                     \
	TRICE_PUT(TRICE_HTOTL(v4));                                     \
	TRICE_PUT(TRICE_HTOTL(v5));                                     \
	TRICE_PUT(TRICE_HTOTL(v6));                                     \
	TRICE_PUT(TRICE_HTOTL(v7));                                     \
	TRICE_PUT(TRICE_HTOTL(v8));                                     \
	TRICE_PUT(TRICE_HTOTL(v9));                                     \
	TRICE_PUT(TRICE_HTOTL(v10));

#define TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_PUT(TRICE_HTOTL(v0));                                          \
	TRICE_PUT(TRICE_HTOTL(v1));                                          \
	TRICE_PUT(TRICE_HTOTL(v2));                                          \
	TRICE_PUT(TRICE_HTOTL(v3));                                          \
	TRICE_PUT(TRICE_HTOTL(v4));                                          \
	TRICE_PUT(TRICE_HTOTL(v5));                                          \
	TRICE_PUT(TRICE_HTOTL(v6));                                          \
	TRICE_PUT(TRICE_HTOTL(v7));                                          \
	TRICE_PUT(TRICE_HTOTL(v8));                                          \
	TRICE_PUT(TRICE_HTOTL(v9));                                          \
	TRICE_PUT(TRICE_HTOTL(v10));                                         \
	TRICE_PUT(TRICE_HTOTL(v11));

// The following `TRICE` macro definitions uses the C preprocess to concatenate the code behind the sub-macros.
// The sequence `tid` in this context is executable code `id(n)`, `Id(n)` or `ID(n)` defined in trice.h.
// This works only with tid=`id(n)`, `Id(n)`, `ID(n)`.

//! TRICE32_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define TRICE32_0(tid, pFmt) \
	TRICE_ENTER tid;         \
	TRICE_CNTC(0);           \
	TRICE_LEAVE

//! TRICE32_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 32 bit bit value
#define TRICE32_1(tid, pFmt, v0) \
	TRICE_ENTER tid;             \
	TRICE_CNTC(4);               \
	TRICE_PUT32_1(v0)            \
	TRICE_LEAVE

//! TRICE32_2 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 32 bit values
#define TRICE32_2(tid, pFmt, v0, v1) \
	TRICE_ENTER tid;                 \
	TRICE_CNTC(8);                   \
	TRICE_PUT32_2(v0, v1);           \
	TRICE_LEAVE

//! TRICE32_3 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 32 bit bit values
#define TRICE32_3(tid, pFmt, v0, v1, v2) \
	TRICE_ENTER tid;                     \
	TRICE_CNTC(12);                      \
	TRICE_PUT32_3(v0, v1, v2);           \
	TRICE_LEAVE

//! TRICE32_4 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 32 bit bit values
#define TRICE32_4(tid, pFmt, v0, v1, v2, v3) \
	TRICE_ENTER tid;                         \
	TRICE_CNTC(16);                          \
	TRICE_PUT32_4(v0, v1, v2, v3);           \
	TRICE_LEAVE

//! TRICE32_5 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 32 bit bit values
#define TRICE32_5(tid, pFmt, v0, v1, v2, v3, v4) \
	TRICE_ENTER tid;                             \
	TRICE_CNTC(20);                              \
	TRICE_PUT32_5(v0, v1, v2, v3, v4);           \
	TRICE_LEAVE

//! TRICE32_6 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 32 bit bit values
#define TRICE32_6(tid, pFmt, v0, v1, v2, v3, v4, v5) \
	TRICE_ENTER tid;                                 \
	TRICE_CNTC(24);                                  \
	TRICE_PUT32_6(v0, v1, v2, v3, v4, v5);           \
	TRICE_LEAVE

//! TRICE32_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 32 bit bit values
#define TRICE32_7(tid, pFmt, v0, v1, v2, v3, v4, v5, v6) \
	TRICE_ENTER tid;                                     \
	TRICE_CNTC(28);                                      \
	TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6);           \
	TRICE_LEAVE

//! TRICE32_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 32 bit bit values
#define TRICE32_8(tid, pFmt, v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_ENTER tid;                                         \
	TRICE_CNTC(32);                                          \
	TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7);           \
	TRICE_LEAVE

//! TRICE32_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 32 bit bit values
#define TRICE32_9(tid, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER tid;                                             \
	TRICE_CNTC(36);                                              \
	TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);           \
	TRICE_LEAVE

//! TRICE32_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 32 bit bit values
#define TRICE32_10(tid, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER tid;                                                  \
	TRICE_CNTC(40);                                                   \
	TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);           \
	TRICE_LEAVE

//! TRICE32_8 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 32 bit bit values
#define TRICE32_11(tid, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER tid;                                                       \
	TRICE_CNTC(44);                                                        \
	TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);           \
	TRICE_LEAVE

//! TRICE32_12 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 32 bit bit values
#define TRICE32_12(tid, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER tid;                                                            \
	TRICE_CNTC(48);                                                             \
	TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)            \
	TRICE_LEAVE

