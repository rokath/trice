/*! \file internalTrice32.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#define TRICE32(tid, fmt, ...) TRICE_CONCAT2(TRICE32_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

#if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define trice32(fmt, ...)
#define Trice32(fmt, ...)
#define TRice32(fmt, ...)

#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define trice32(tid, fmt, ...) TRICE_CONCAT2(trice32_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define Trice32(tid, fmt, ...) TRICE_CONCAT2(Trice32_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRice32(tid, fmt, ...) TRICE_CONCAT2(TRice32_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

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

#define TRICE_PUT32_1(v0) TRICE_PUT((uint32_t)(v0));

#define TRICE_PUT32_2(v0, v1)  \
	TRICE_PUT((uint32_t)(v0)); \
	TRICE_PUT((uint32_t)(v1));

#define TRICE_PUT32_3(v0, v1, v2) \
	TRICE_PUT((uint32_t)(v0));    \
	TRICE_PUT((uint32_t)(v1));    \
	TRICE_PUT((uint32_t)(v2));

#define TRICE_PUT32_4(v0, v1, v2, v3) \
	TRICE_PUT((uint32_t)(v0));        \
	TRICE_PUT((uint32_t)(v1));        \
	TRICE_PUT((uint32_t)(v2));        \
	TRICE_PUT((uint32_t)(v3));

#define TRICE_PUT32_5(v0, v1, v2, v3, v4) \
	TRICE_PUT((uint32_t)(v0));            \
	TRICE_PUT((uint32_t)(v1));            \
	TRICE_PUT((uint32_t)(v2));            \
	TRICE_PUT((uint32_t)(v3));            \
	TRICE_PUT((uint32_t)(v4));

#define TRICE_PUT32_6(v0, v1, v2, v3, v4, v5) \
	TRICE_PUT((uint32_t)(v0));                \
	TRICE_PUT((uint32_t)(v1));                \
	TRICE_PUT((uint32_t)(v2));                \
	TRICE_PUT((uint32_t)(v3));                \
	TRICE_PUT((uint32_t)(v4));                \
	TRICE_PUT((uint32_t)(v5));

#define TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6) \
	TRICE_PUT((uint32_t)(v0));                    \
	TRICE_PUT((uint32_t)(v1));                    \
	TRICE_PUT((uint32_t)(v2));                    \
	TRICE_PUT((uint32_t)(v3));                    \
	TRICE_PUT((uint32_t)(v4));                    \
	TRICE_PUT((uint32_t)(v5));                    \
	TRICE_PUT((uint32_t)(v6));

#define TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_PUT((uint32_t)(v0));                        \
	TRICE_PUT((uint32_t)(v1));                        \
	TRICE_PUT((uint32_t)(v2));                        \
	TRICE_PUT((uint32_t)(v3));                        \
	TRICE_PUT((uint32_t)(v4));                        \
	TRICE_PUT((uint32_t)(v5));                        \
	TRICE_PUT((uint32_t)(v6));                        \
	TRICE_PUT((uint32_t)(v7));

#define TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_PUT((uint32_t)(v0));                            \
	TRICE_PUT((uint32_t)(v1));                            \
	TRICE_PUT((uint32_t)(v2));                            \
	TRICE_PUT((uint32_t)(v3));                            \
	TRICE_PUT((uint32_t)(v4));                            \
	TRICE_PUT((uint32_t)(v5));                            \
	TRICE_PUT((uint32_t)(v6));                            \
	TRICE_PUT((uint32_t)(v7));                            \
	TRICE_PUT((uint32_t)(v8));

#define TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_PUT((uint32_t)(v0));                                 \
	TRICE_PUT((uint32_t)(v1));                                 \
	TRICE_PUT((uint32_t)(v2));                                 \
	TRICE_PUT((uint32_t)(v3));                                 \
	TRICE_PUT((uint32_t)(v4));                                 \
	TRICE_PUT((uint32_t)(v5));                                 \
	TRICE_PUT((uint32_t)(v6));                                 \
	TRICE_PUT((uint32_t)(v7));                                 \
	TRICE_PUT((uint32_t)(v8));                                 \
	TRICE_PUT((uint32_t)(v9));

#define TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_PUT((uint32_t)(v0));                                      \
	TRICE_PUT((uint32_t)(v1));                                      \
	TRICE_PUT((uint32_t)(v2));                                      \
	TRICE_PUT((uint32_t)(v3));                                      \
	TRICE_PUT((uint32_t)(v4));                                      \
	TRICE_PUT((uint32_t)(v5));                                      \
	TRICE_PUT((uint32_t)(v6));                                      \
	TRICE_PUT((uint32_t)(v7));                                      \
	TRICE_PUT((uint32_t)(v8));                                      \
	TRICE_PUT((uint32_t)(v9));                                      \
	TRICE_PUT((uint32_t)(v10));

#define TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_PUT((uint32_t)(v0));                                           \
	TRICE_PUT((uint32_t)(v1));                                           \
	TRICE_PUT((uint32_t)(v2));                                           \
	TRICE_PUT((uint32_t)(v3));                                           \
	TRICE_PUT((uint32_t)(v4));                                           \
	TRICE_PUT((uint32_t)(v5));                                           \
	TRICE_PUT((uint32_t)(v6));                                           \
	TRICE_PUT((uint32_t)(v7));                                           \
	TRICE_PUT((uint32_t)(v8));                                           \
	TRICE_PUT((uint32_t)(v9));                                           \
	TRICE_PUT((uint32_t)(v10));                                          \
	TRICE_PUT((uint32_t)(v11));

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

#if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

//! trice32m_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define trice32m_0(tid)                                              \
	TRICE_ENTER                                                      \
	TRICE_PUT((0 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_LEAVE

//! trice32m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 32 bit bit value
#define trice32m_1(tid, v0)                                          \
	TRICE_ENTER                                                      \
	TRICE_PUT((4 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT32_1(v0)                                                \
	TRICE_LEAVE

#define trice32m_2(tid, v0, v1)                                      \
	TRICE_ENTER                                                      \
	TRICE_PUT((8 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT32_2(v0, v1);                                           \
	TRICE_LEAVE

#define trice32m_3(tid, v0, v1, v2)                                   \
	TRICE_ENTER                                                       \
	TRICE_PUT((12 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT32_3(v0, v1, v2);                                        \
	TRICE_LEAVE

#define trice32m_4(tid, v0, v1, v2, v3)                               \
	TRICE_ENTER                                                       \
	TRICE_PUT((16 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT32_4(v0, v1, v2, v3);                                    \
	TRICE_LEAVE

#define trice32m_5(tid, v0, v1, v2, v3, v4)                           \
	TRICE_ENTER                                                       \
	TRICE_PUT((20 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT32_5(v0, v1, v2, v3, v4);                                \
	TRICE_LEAVE

#define trice32m_6(tid, v0, v1, v2, v3, v4, v5)                       \
	TRICE_ENTER                                                       \
	TRICE_PUT((24 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT32_6(v0, v1, v2, v3, v4, v5);                            \
	TRICE_LEAVE

#define trice32m_7(tid, v0, v1, v2, v3, v4, v5, v6)                   \
	TRICE_ENTER                                                       \
	TRICE_PUT((28 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6);                        \
	TRICE_LEAVE

#define trice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)               \
	TRICE_ENTER                                                       \
	TRICE_PUT((32 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7);                    \
	TRICE_LEAVE

#define trice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)           \
	TRICE_ENTER                                                       \
	TRICE_PUT((36 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                \
	TRICE_LEAVE

#define trice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)      \
	TRICE_ENTER                                                       \
	TRICE_PUT((40 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);           \
	TRICE_LEAVE

#define trice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	TRICE_PUT((44 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define trice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	TRICE_PUT((48 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid)));      \
	TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

//! trice32m_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define trice32m_0(tid)                                                    \
	TRICE_ENTER                                                            \
	TRICE_PUT((0 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_LEAVE

//! trice32m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 32 bit bit value
#define trice32m_1(tid, v0)                                                \
	TRICE_ENTER                                                            \
	TRICE_PUT((4 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT32_1(v0)                                                      \
	TRICE_LEAVE

#define trice32m_2(tid, v0, v1)                                            \
	TRICE_ENTER                                                            \
	TRICE_PUT((8 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT32_2(v0, v1);                                                 \
	TRICE_LEAVE

#define trice32m_3(tid, v0, v1, v2)                                         \
	TRICE_ENTER                                                             \
	TRICE_PUT((12 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT32_3(v0, v1, v2);                                              \
	TRICE_LEAVE

#define trice32m_4(tid, v0, v1, v2, v3)                                     \
	TRICE_ENTER                                                             \
	TRICE_PUT((16 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT32_4(v0, v1, v2, v3);                                          \
	TRICE_LEAVE

#define trice32m_5(tid, v0, v1, v2, v3, v4)                                 \
	TRICE_ENTER                                                             \
	TRICE_PUT((20 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT32_5(v0, v1, v2, v3, v4);                                      \
	TRICE_LEAVE

#define trice32m_6(tid, v0, v1, v2, v3, v4, v5)                             \
	TRICE_ENTER                                                             \
	TRICE_PUT((24 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT32_6(v0, v1, v2, v3, v4, v5);                                  \
	TRICE_LEAVE

#define trice32m_7(tid, v0, v1, v2, v3, v4, v5, v6)                         \
	TRICE_ENTER                                                             \
	TRICE_PUT((28 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6);                              \
	TRICE_LEAVE

#define trice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)                     \
	TRICE_ENTER                                                             \
	TRICE_PUT((32 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7);                          \
	TRICE_LEAVE

#define trice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)                 \
	TRICE_ENTER                                                             \
	TRICE_PUT((36 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                      \
	TRICE_LEAVE

#define trice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)            \
	TRICE_ENTER                                                             \
	TRICE_PUT((40 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);                 \
	TRICE_LEAVE

#define trice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)       \
	TRICE_ENTER                                                             \
	TRICE_PUT((44 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);            \
	TRICE_LEAVE

#define trice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)  \
	TRICE_ENTER                                                             \
	TRICE_PUT((48 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)        \
	TRICE_LEAVE

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define trice32_0(fmt)                                                    //!< trice32_1 is a macro calling a function to reduce code size.
#define trice32_1(fmt, v0)                                                //!< trice32_1 is a macro calling a function to reduce code size.
#define trice32_2(fmt, v0, v1)                                            //!< trice32_2 is a macro calling a function to reduce code size.
#define trice32_3(fmt, v0, v1, v2)                                        //!< trice32_3 is a macro calling a function to reduce code size.
#define trice32_4(fmt, v0, v1, v2, v3)                                    //!< trice32_4 is a macro calling a function to reduce code size.
#define trice32_5(fmt, v0, v1, v2, v3, v4)                                //!< trice32_5 is a macro calling a function to reduce code size.
#define trice32_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< trice32_6 is a macro calling a function to reduce code size.
#define trice32_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< trice32_7 is a macro calling a function to reduce code size.
#define trice32_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< trice32_8 is a macro calling a function to reduce code size.
#define trice32_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< trice32_9 is a macro calling a function to reduce code size.
#define trice32_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< trice32_10 is a macro calling a function to reduce code size.
#define trice32_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< trice32_11 is a macro calling a function to reduce code size.
#define trice32_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< trice32_12 is a macro calling a function to reduce code size.

#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define trice32_0(tid, fmt) trice32fn_0(tid)                                                                                                                                                                                                                                                       //!< trice32_0 is a macro calling a function to reduce code size.
#define trice32_1(tid, fmt, v0) trice32fn_1(tid, (uint32_t)(v0))                                                                                                                                                                                                                                   //!< trice32_1 is a macro calling a function to reduce code size.
#define trice32_2(tid, fmt, v0, v1) trice32fn_2(tid, (uint32_t)(v0), (uint32_t)(v1))                                                                                                                                                                                                               //!< trice32_2 is a macro calling a function to reduce code size.
#define trice32_3(tid, fmt, v0, v1, v2) trice32fn_3(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2))                                                                                                                                                                                           //!< trice32_3 is a macro calling a function to reduce code size.
#define trice32_4(tid, fmt, v0, v1, v2, v3) trice32fn_4(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3))                                                                                                                                                                       //!< trice32_4 is a macro calling a function to r  educe code size.
#define trice32_5(tid, fmt, v0, v1, v2, v3, v4) trice32fn_5(tid, (uint32_t)v0, (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4))                                                                                                                                                     //!< trice32_5 is a macro calling a function to reduce code size.
#define trice32_6(tid, fmt, v0, v1, v2, v3, v4, v5) trice32fn_6(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5))                                                                                                                               //!< trice32_6 is a macro calling a function to reduce code size.
#define trice32_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) trice32fn_7(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6))                                                                                                           //!< trice32_7 is a macro calling a function to reduce code size.
#define trice32_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) trice32fn_8(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7))                                                                                       //!< trice32_8 is a macro calling a function to reduce code size.
#define trice32_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) trice32fn_9(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8))                                                                   //!< trice32_9 is a macro calling a function to reduce code size.
#define trice32_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) trice32fn_10(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9))                                             //!< trice32_10 is a macro calling a function to reduce code size.
#define trice32_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) trice32fn_11(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10))                       //!< trice32_11 is a macro calling a function to reduce code size.
#define trice32_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) trice32fn_12(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10), (uint32_t)(v11)) //!< trice32_12 is a macro calling a function to reduce code size.

void trice32fn_0(uint16_t tid);
void trice32fn_1(uint16_t tid, uint32_t v0);
void trice32fn_2(uint16_t tid, uint32_t v0, uint32_t v1);
void trice32fn_3(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2);
void trice32fn_4(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3);
void trice32fn_5(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4);
void trice32fn_6(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5);
void trice32fn_7(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6);
void trice32fn_8(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7);
void trice32fn_9(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8);
void trice32fn_10(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9);
void trice32fn_11(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10);
void trice32fn_12(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10, uint32_t v11);

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#define Trice32m_0(tid)                            \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(0 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_LEAVE

//! Trice32m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 32 bit bit value
#define Trice32m_1(tid, v0)                        \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(4 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT32_1(v0)                              \
	TRICE_LEAVE

#define Trice32m_2(tid, v0, v1)                    \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(8 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT32_2(v0, v1);                         \
	TRICE_LEAVE

#define Trice32m_3(tid, v0, v1, v2)                 \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(12 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT32_3(v0, v1, v2);                      \
	TRICE_LEAVE

#define Trice32m_4(tid, v0, v1, v2, v3)             \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(16 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT32_4(v0, v1, v2, v3);                  \
	TRICE_LEAVE

#define Trice32m_5(tid, v0, v1, v2, v3, v4)         \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(20 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT32_5(v0, v1, v2, v3, v4);              \
	TRICE_LEAVE

#define Trice32m_6(tid, v0, v1, v2, v3, v4, v5)     \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(24 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT32_6(v0, v1, v2, v3, v4, v5);          \
	TRICE_LEAVE

#define Trice32m_7(tid, v0, v1, v2, v3, v4, v5, v6) \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(28 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6);      \
	TRICE_LEAVE

#define Trice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_ENTER                                         \
	uint16_t ts = TriceStamp16;                         \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));        \
	TRICE_PUT(32 << 24 | (TRICE_CYCLE << 16) | ts);     \
	TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7);      \
	TRICE_LEAVE

#define Trice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));            \
	TRICE_PUT(36 << 24 | (TRICE_CYCLE << 16) | ts);         \
	TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define Trice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint16_t ts = TriceStamp16;                                  \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                 \
	TRICE_PUT(40 << 24 | (TRICE_CYCLE << 16) | ts);              \
	TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint16_t ts = TriceStamp16;                                       \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                      \
	TRICE_PUT(44 << 24 | (TRICE_CYCLE << 16) | ts);                   \
	TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint16_t ts = TriceStamp16;                                            \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                           \
	TRICE_PUT(48 << 24 | (TRICE_CYCLE << 16) | ts);                        \
	TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#define Trice32m_0(tid)                                  \
	TRICE_ENTER                                          \
	uint16_t ts = TriceStamp16;                          \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));         \
	TRICE_PUT(0 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_LEAVE

//! Trice32m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 32 bit bit value
#define Trice32m_1(tid, v0)                              \
	TRICE_ENTER                                          \
	uint16_t ts = TriceStamp16;                          \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));         \
	TRICE_PUT(4 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_1(v0)                                    \
	TRICE_LEAVE

#define Trice32m_2(tid, v0, v1)                          \
	TRICE_ENTER                                          \
	uint16_t ts = TriceStamp16;                          \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));         \
	TRICE_PUT(8 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_2(v0, v1);                               \
	TRICE_LEAVE

#define Trice32m_3(tid, v0, v1, v2)                       \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(12 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_3(v0, v1, v2);                            \
	TRICE_LEAVE

#define Trice32m_4(tid, v0, v1, v2, v3)                   \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(16 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_4(v0, v1, v2, v3);                        \
	TRICE_LEAVE

#define Trice32m_5(tid, v0, v1, v2, v3, v4)               \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(20 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_5(v0, v1, v2, v3, v4);                    \
	TRICE_LEAVE

#define Trice32m_6(tid, v0, v1, v2, v3, v4, v5)           \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(24 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_6(v0, v1, v2, v3, v4, v5);                \
	TRICE_LEAVE

#define Trice32m_7(tid, v0, v1, v2, v3, v4, v5, v6)       \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(28 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6);            \
	TRICE_LEAVE

#define Trice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)   \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(32 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7);        \
	TRICE_LEAVE

#define Trice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));            \
	TRICE_PUT(36 << 8 | (TRICE_CYCLE << 0) | (ts << 16));   \
	TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define Trice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint16_t ts = TriceStamp16;                                  \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                 \
	TRICE_PUT(40 << 8 | (TRICE_CYCLE << 0) | (ts << 16));        \
	TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint16_t ts = TriceStamp16;                                       \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                      \
	TRICE_PUT(44 << 8 | (TRICE_CYCLE << 0) | (ts << 16));             \
	TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint16_t ts = TriceStamp16;                                            \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                           \
	TRICE_PUT(48 << 8 | (TRICE_CYCLE << 0) | (ts << 16));                  \
	TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define Trice32_0(fmt)                                                    //!< Trice32_1 is a macro calling a function to reduce code size.
#define Trice32_1(fmt, v0)                                                //!< Trice32_1 is a macro calling a function to reduce code size.
#define Trice32_2(fmt, v0, v1)                                            //!< Trice32_2 is a macro calling a function to reduce code size.
#define Trice32_3(fmt, v0, v1, v2)                                        //!< Trice32_3 is a macro calling a function to reduce code size.
#define Trice32_4(fmt, v0, v1, v2, v3)                                    //!< Trice32_4 is a macro calling a function to reduce code size.
#define Trice32_5(fmt, v0, v1, v2, v3, v4)                                //!< Trice32_5 is a macro calling a function to reduce code size.
#define Trice32_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< Trice32_6 is a macro calling a function to reduce code size.
#define Trice32_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< Trice32_7 is a macro calling a function to reduce code size.
#define Trice32_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< Trice32_8 is a macro calling a function to reduce code size.
#define Trice32_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< Trice32_9 is a macro calling a function to reduce code size.
#define Trice32_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< Trice32_10 is a macro calling a function to reduce code size.
#define Trice32_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< Trice32_11 is a macro calling a function to reduce code size.
#define Trice32_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< Trice32_12 is a macro calling a function to reduce code size.

#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define Trice32_0(tid, fmt) Trice32fn_0(tid)                                                                                                                                                                                                                                                       //!< Trice32_0 is a macro calling a function to reduce code size.
#define Trice32_1(tid, fmt, v0) Trice32fn_1(tid, (uint32_t)(v0))                                                                                                                                                                                                                                   //!< Trice32_1 is a macro calling a function to reduce code size.
#define Trice32_2(tid, fmt, v0, v1) Trice32fn_2(tid, (uint32_t)(v0), (uint32_t)(v1))                                                                                                                                                                                                               //!< Trice32_2 is a macro calling a function to reduce code size.
#define Trice32_3(tid, fmt, v0, v1, v2) Trice32fn_3(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2))                                                                                                                                                                                           //!< Trice32_3 is a macro calling a function to reduce code size.
#define Trice32_4(tid, fmt, v0, v1, v2, v3) Trice32fn_4(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3))                                                                                                                                                                       //!< Trice32_4 is a macro calling a function to reduce code size.
#define Trice32_5(tid, fmt, v0, v1, v2, v3, v4) Trice32fn_5(tid, (uint32_t)v0, (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4))                                                                                                                                                     //!< Trice32_5 is a macro calling a function to reduce code size.
#define Trice32_6(tid, fmt, v0, v1, v2, v3, v4, v5) Trice32fn_6(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5))                                                                                                                               //!< Trice32_6 is a macro calling a function to reduce code size.
#define Trice32_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) Trice32fn_7(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6))                                                                                                           //!< Trice32_7 is a macro calling a function to reduce code size.
#define Trice32_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) Trice32fn_8(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7))                                                                                       //!< Trice32_8 is a macro calling a function to reduce code size.
#define Trice32_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) Trice32fn_9(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8))                                                                   //!< Trice32_9 is a macro calling a function to reduce code size.
#define Trice32_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) Trice32fn_10(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9))                                             //!< Trice32_10 is a macro calling a function to reduce code size.
#define Trice32_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) Trice32fn_11(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10))                       //!< Trice32_11 is a macro calling a function to reduce code size.
#define Trice32_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) Trice32fn_12(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10), (uint32_t)(v11)) //!< Trice32_12 is a macro calling a function to reduce code size.

void Trice32fn_0(uint16_t tid);
void Trice32fn_1(uint16_t tid, uint32_t v0);
void Trice32fn_2(uint16_t tid, uint32_t v0, uint32_t v1);
void Trice32fn_3(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2);
void Trice32fn_4(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3);
void Trice32fn_5(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4);
void Trice32fn_6(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5);
void Trice32fn_7(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6);
void Trice32fn_8(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7);
void Trice32fn_9(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8);
void Trice32fn_10(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9);
void Trice32fn_11(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10);
void Trice32fn_12(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10, uint32_t v11);

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#define TRice32m_0(tid)                                    \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(0 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_LEAVE

//! TRice32m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 14 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 32 bit bit value
#define TRice32m_1(tid, v0)                                \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(4 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT32_1(v0)                                      \
	TRICE_LEAVE

#define TRice32m_2(tid, v0, v1)                            \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(8 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT32_2(v0, v1);                                 \
	TRICE_LEAVE

#define TRice32m_3(tid, v0, v1, v2)                         \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(12 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT32_3(v0, v1, v2);                              \
	TRICE_LEAVE

#define TRice32m_4(tid, v0, v1, v2, v3)                     \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(16 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT32_4(v0, v1, v2, v3);                          \
	TRICE_LEAVE

#define TRice32m_5(tid, v0, v1, v2, v3, v4)                 \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(20 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT32_5(v0, v1, v2, v3, v4);                      \
	TRICE_LEAVE

#define TRice32m_6(tid, v0, v1, v2, v3, v4, v5)             \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(24 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT32_6(v0, v1, v2, v3, v4, v5);                  \
	TRICE_LEAVE

#define TRice32m_7(tid, v0, v1, v2, v3, v4, v5, v6)         \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(28 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6);              \
	TRICE_LEAVE

#define TRice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)     \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(32 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7);          \
	TRICE_LEAVE

#define TRice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(36 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define TRice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint32_t ts = TriceStamp32;                                  \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                      \
	TRICE_PUT(40 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));      \
	TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint32_t ts = TriceStamp32;                                       \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                           \
	TRICE_PUT(44 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));           \
	TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint32_t ts = TriceStamp32;                                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                                \
	TRICE_PUT(48 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));                \
	TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#define TRice32m_0(tid)                                    \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));      \
	TRICE_PUT((0 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_LEAVE

//! TRice32m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 14 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 32 bit bit value
#define TRice32m_1(tid, v0)                                \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));      \
	TRICE_PUT((4 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_1(v0)                                      \
	TRICE_LEAVE

#define TRice32m_2(tid, v0, v1)                            \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));      \
	TRICE_PUT((8 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_2(v0, v1);                                 \
	TRICE_LEAVE

#define TRice32m_3(tid, v0, v1, v2)                         \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((12 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_3(v0, v1, v2);                              \
	TRICE_LEAVE

#define TRice32m_4(tid, v0, v1, v2, v3)                     \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((16 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_4(v0, v1, v2, v3);                          \
	TRICE_LEAVE

#define TRice32m_5(tid, v0, v1, v2, v3, v4)                 \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((20 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_5(v0, v1, v2, v3, v4);                      \
	TRICE_LEAVE

#define TRice32m_6(tid, v0, v1, v2, v3, v4, v5)             \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((24 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_6(v0, v1, v2, v3, v4, v5);                  \
	TRICE_LEAVE

#define TRice32m_7(tid, v0, v1, v2, v3, v4, v5, v6)         \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((28 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6);              \
	TRICE_LEAVE

#define TRice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)     \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((32 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7);          \
	TRICE_LEAVE

#define TRice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((36 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define TRice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint32_t ts = TriceStamp32;                                  \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));            \
	TRICE_PUT((40 << 8) | (TRICE_CYCLE << 0) | (ts << 16));      \
	TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint32_t ts = TriceStamp32;                                       \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));                 \
	TRICE_PUT((44 << 8) | (TRICE_CYCLE << 0) | (ts << 16));           \
	TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint32_t ts = TriceStamp32;                                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));                      \
	TRICE_PUT((48 << 8) | (TRICE_CYCLE << 0) | (ts << 16));                \
	TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define TRice32_0(fmt)                                                    //!< TRice32_1 is a macro calling a function to reduce code size.
#define TRice32_1(fmt, v0)                                                //!< TRice32_1 is a macro calling a function to reduce code size.
#define TRice32_2(fmt, v0, v1)                                            //!< TRice32_2 is a macro calling a function to reduce code size.
#define TRice32_3(fmt, v0, v1, v2)                                        //!< TRice32_3 is a macro calling a function to reduce code size.
#define TRice32_4(fmt, v0, v1, v2, v3)                                    //!< TRice32_4 is a macro calling a function to reduce code size.
#define TRice32_5(fmt, v0, v1, v2, v3, v4)                                //!< TRice32_5 is a macro calling a function to reduce code size.
#define TRice32_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< TRice32_6 is a macro calling a function to reduce code size.
#define TRice32_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< TRice32_7 is a macro calling a function to reduce code size.
#define TRice32_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< TRice32_8 is a macro calling a function to reduce code size.
#define TRice32_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< TRice32_9 is a macro calling a function to reduce code size.
#define TRice32_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< TRice32_10 is a macro calling a function to reduce code size.
#define TRice32_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< TRice32_11 is a macro calling a function to reduce code size.
#define TRice32_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< TRice32_12 is a macro calling a function to reduce code size.

#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define TRice32_0(tid, fmt) TRice32fn_0(tid)                                                                                                                                                                                                                                                       //!< TRice32_1 is a macro calling a function to reduce code size.
#define TRice32_1(tid, fmt, v0) TRice32fn_1(tid, (uint32_t)(v0))                                                                                                                                                                                                                                   //!< TRice32_1 is a macro calling a function to reduce code size.
#define TRice32_2(tid, fmt, v0, v1) TRice32fn_2(tid, (uint32_t)(v0), (uint32_t)(v1))                                                                                                                                                                                                               //!< TRice32_2 is a macro calling a function to reduce code size.
#define TRice32_3(tid, fmt, v0, v1, v2) TRice32fn_3(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2))                                                                                                                                                                                           //!< TRice32_3 is a macro calling a function to reduce code size.
#define TRice32_4(tid, fmt, v0, v1, v2, v3) TRice32fn_4(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3))                                                                                                                                                                       //!< TRice32_4 is a macro calling a function to reduce code size.
#define TRice32_5(tid, fmt, v0, v1, v2, v3, v4) TRice32fn_5(tid, (uint32_t)v0, (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4))                                                                                                                                                     //!< TRice32_5 is a macro calling a function to reduce code size.
#define TRice32_6(tid, fmt, v0, v1, v2, v3, v4, v5) TRice32fn_6(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5))                                                                                                                               //!< TRice32_6 is a macro calling a function to reduce code size.
#define TRice32_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) TRice32fn_7(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6))                                                                                                           //!< TRice32_7 is a macro calling a function to reduce code size.
#define TRice32_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) TRice32fn_8(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7))                                                                                       //!< TRice32_32_M is a macro calling a function to reduce code size.
#define TRice32_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) TRice32fn_9(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8))                                                                   //!< TRice32_9 is a macro calling a function to reduce code size.
#define TRice32_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) TRice32fn_10(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9))                                             //!< TRice32_10 is a macro calling a function to reduce code size.
#define TRice32_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) TRice32fn_11(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10))                       //!< TRice32_11 is a macro calling a function to reduce code size.
#define TRice32_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) TRice32fn_12(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10), (uint32_t)(v11)) //!< TRice32_12 is a macro calling a function to reduce code size.

void TRice32fn_0(uint16_t tid);
void TRice32fn_1(uint16_t tid, uint32_t v0);
void TRice32fn_2(uint16_t tid, uint32_t v0, uint32_t v1);
void TRice32fn_3(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2);
void TRice32fn_4(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3);
void TRice32fn_5(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4);
void TRice32fn_6(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5);
void TRice32fn_7(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6);
void TRice32fn_8(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7);
void TRice32fn_9(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8);
void TRice32fn_10(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9);
void TRice32fn_11(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10);
void TRice32fn_12(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10, uint32_t v11);

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

///////////////////////////////////////////////////////////////////////////////
//

#if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 32

// #define TRICE_0  TRICE32_0  //!< Default parameter bit width for 0  parameter count TRICE is 32, change for a different value.
#define TRICE_1 TRICE32_1   //!< Default parameter bit width for 1  parameter count TRICE is 32, change for a different value.
#define TRICE_2 TRICE32_2   //!< Default parameter bit width for 2  parameter count TRICE is 32, change for a different value.
#define TRICE_3 TRICE32_3   //!< Default parameter bit width for 3  parameter count TRICE is 32, change for a different value.
#define TRICE_4 TRICE32_4   //!< Default parameter bit width for 4  parameter count TRICE is 32, change for a different value.
#define TRICE_5 TRICE32_5   //!< Default parameter bit width for 5  parameter count TRICE is 32, change for a different value.
#define TRICE_6 TRICE32_6   //!< Default parameter bit width for 6  parameter count TRICE is 32, change for a different value.
#define TRICE_7 TRICE32_7   //!< Default parameter bit width for 7  parameter count TRICE is 32, change for a different value.
#define TRICE_8 TRICE32_8   //!< Default parameter bit width for 8  parameter count TRICE is 32, change for a different value.
#define TRICE_9 TRICE32_9   //!< Default parameter bit width for 9  parameter count TRICE is 32, change for a different value.
#define TRICE_10 TRICE32_10 //!< Default parameter bit width for 10 parameter count TRICE is 32, change for a different value.
#define TRICE_11 TRICE32_11 //!< Default parameter bit width for 11 parameter count TRICE is 32, change for a different value.
#define TRICE_12 TRICE32_12 //!< Default parameter bit width for 12 parameter count TRICE is 32, change for a different value.

// #define trice_0  trice32_0  //!< Default parameter bit width for 0  parameter count trice is 32, change for a different value.
#define trice_1 trice32_1   //!< Default parameter bit width for 1  parameter count trice is 32, change for a different value.
#define trice_2 trice32_2   //!< Default parameter bit width for 2  parameter count trice is 32, change for a different value.
#define trice_3 trice32_3   //!< Default parameter bit width for 3  parameter count trice is 32, change for a different value.
#define trice_4 trice32_4   //!< Default parameter bit width for 4  parameter count trice is 32, change for a different value.
#define trice_5 trice32_5   //!< Default parameter bit width for 5  parameter count trice is 32, change for a different value.
#define trice_6 trice32_6   //!< Default parameter bit width for 6  parameter count trice is 32, change for a different value.
#define trice_7 trice32_7   //!< Default parameter bit width for 7  parameter count trice is 32, change for a different value.
#define trice_8 trice32_8   //!< Default parameter bit width for 8  parameter count trice is 32, change for a different value.
#define trice_9 trice32_9   //!< Default parameter bit width for 9  parameter count trice is 32, change for a different value.
#define trice_10 trice32_10 //!< Default parameter bit width for 10 parameter count trice is 32, change for a different value.
#define trice_11 trice32_11 //!< Default parameter bit width for 11 parameter count trice is 32, change for a different value.
#define trice_12 trice32_12 //!< Default parameter bit width for 12 parameter count trice is 32, change for a different value.

// #define Trice_0  Trice32_0  //!< Default parameter bit width for 0  parameter count Trice is 32, change for a different value.
#define Trice_1 Trice32_1   //!< Default parameter bit width for 1  parameter count Trice is 32, change for a different value.
#define Trice_2 Trice32_2   //!< Default parameter bit width for 2  parameter count Trice is 32, change for a different value.
#define Trice_3 Trice32_3   //!< Default parameter bit width for 3  parameter count Trice is 32, change for a different value.
#define Trice_4 Trice32_4   //!< Default parameter bit width for 4  parameter count Trice is 32, change for a different value.
#define Trice_5 Trice32_5   //!< Default parameter bit width for 5  parameter count Trice is 32, change for a different value.
#define Trice_6 Trice32_6   //!< Default parameter bit width for 6  parameter count Trice is 32, change for a different value.
#define Trice_7 Trice32_7   //!< Default parameter bit width for 7  parameter count Trice is 32, change for a different value.
#define Trice_8 Trice32_8   //!< Default parameter bit width for 8  parameter count Trice is 32, change for a different value.
#define Trice_9 Trice32_9   //!< Default parameter bit width for 9  parameter count Trice is 32, change for a different value.
#define Trice_10 Trice32_10 //!< Default parameter bit width for 10 parameter count Trice is 32, change for a different value.
#define Trice_11 Trice32_11 //!< Default parameter bit width for 11 parameter count Trice is 32, change for a different value.
#define Trice_12 Trice32_12 //!< Default parameter bit width for 12 parameter count Trice is 32, change for a different value.

// #define TRice_0  TRice32_0  //!< Default parameter bit width for 0  parameter count TRice is 32, change for a different value.
#define TRice_1 TRice32_1   //!< Default parameter bit width for 1  parameter count TRice is 32, change for a different value.
#define TRice_2 TRice32_2   //!< Default parameter bit width for 2  parameter count TRice is 32, change for a different value.
#define TRice_3 TRice32_3   //!< Default parameter bit width for 3  parameter count TRice is 32, change for a different value.
#define TRice_4 TRice32_4   //!< Default parameter bit width for 4  parameter count TRice is 32, change for a different value.
#define TRice_5 TRice32_5   //!< Default parameter bit width for 5  parameter count TRice is 32, change for a different value.
#define TRice_6 TRice32_6   //!< Default parameter bit width for 6  parameter count TRice is 32, change for a different value.
#define TRice_7 TRice32_7   //!< Default parameter bit width for 7  parameter count TRice is 32, change for a different value.
#define TRice_8 TRice32_8   //!< Default parameter bit width for 8  parameter count TRice is 32, change for a different value.
#define TRice_9 TRice32_9   //!< Default parameter bit width for 9  parameter count TRice is 32, change for a different value.
#define TRice_10 TRice32_10 //!< Default parameter bit width for 10 parameter count TRice is 32, change for a different value.
#define TRice_11 TRice32_11 //!< Default parameter bit width for 11 parameter count TRice is 32, change for a different value.
#define TRice_12 TRice32_12 //!< Default parameter bit width for 12 parameter count TRice is 32, change for a different value.

#endif // #if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 32

//
///////////////////////////////////////////////////////////////////////////////
