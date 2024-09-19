/*! \file internalTrice64.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#if ((TRICE_MCU_IS_BIG_ENDIAN == 1) && (TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 1)) || ((TRICE_MCU_IS_BIG_ENDIAN == 0) && (TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0))

#define TRICE_PUT64(x)        \
	TRICE_PUT((uint32_t)(x)); \
	TRICE_PUT((uint32_t)((uint64_t)(x) >> 32)); // little endian

#else

#define TRICE_PUT64(x)                          \
	TRICE_PUT((uint32_t)((uint64_t)(x) >> 32)); \
	TRICE_PUT((uint32_t)(x)); // big endian

#endif

#define TRICE64(tid, fmt, ...) TRICE_CONCAT2(TRICE64_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

#if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define trice64(fmt, ...)
#define Trice64(fmt, ...)
#define TRice64(fmt, ...)

#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define trice64(tid, fmt, ...) TRICE_CONCAT2(trice64_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define Trice64(tid, fmt, ...) TRICE_CONCAT2(Trice64_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRice64(tid, fmt, ...) TRICE_CONCAT2(TRice64_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

//! TRICE64_B expects inside pFmt only one format specifier, which is used n times by using pFmt n times.
//!  It is usable for showing n 64-bit values.
#define TRICE64_B(id, pFmt, buf, n)    \
	do {                               \
		TRICE_N(id, pFmt, buf, 8 * n); \
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

#if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

//! trice64m_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define trice64m_0(tid)                                              \
	TRICE_ENTER                                                      \
	TRICE_PUT((0 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_LEAVE

//! trice64m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 64 bit value
#define trice64m_1(tid, v0)                                          \
	TRICE_ENTER                                                      \
	TRICE_PUT((8 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_1(v0)                                                \
	TRICE_LEAVE

#define trice64m_2(tid, v0, v1)                                       \
	TRICE_ENTER                                                       \
	TRICE_PUT((16 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_2(v0, v1);                                            \
	TRICE_LEAVE

#define trice64m_3(tid, v0, v1, v2)                                   \
	TRICE_ENTER                                                       \
	TRICE_PUT((24 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_3(v0, v1, v2);                                        \
	TRICE_LEAVE

#define trice64m_4(tid, v0, v1, v2, v3)                               \
	TRICE_ENTER                                                       \
	TRICE_PUT((32 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_4(v0, v1, v2, v3);                                    \
	TRICE_LEAVE

#define trice64m_5(tid, v0, v1, v2, v3, v4)                           \
	TRICE_ENTER                                                       \
	TRICE_PUT((40 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                                \
	TRICE_LEAVE

#define trice64m_6(tid, v0, v1, v2, v3, v4, v5)                       \
	TRICE_ENTER                                                       \
	TRICE_PUT((48 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                            \
	TRICE_LEAVE

#define trice64m_7(tid, v0, v1, v2, v3, v4, v5, v6)                   \
	TRICE_ENTER                                                       \
	TRICE_PUT((56 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);                        \
	TRICE_LEAVE

#define trice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)               \
	TRICE_ENTER                                                       \
	TRICE_PUT((64 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);                    \
	TRICE_LEAVE

#define trice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)           \
	TRICE_ENTER                                                       \
	TRICE_PUT((72 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                \
	TRICE_LEAVE

#define trice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)      \
	TRICE_ENTER                                                       \
	TRICE_PUT((80 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);           \
	TRICE_LEAVE

#define trice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	TRICE_PUT((88 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define trice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	TRICE_PUT((96 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid)));      \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

//! trice64m_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define trice64m_0(tid)                                                    \
	TRICE_ENTER                                                            \
	TRICE_PUT((0 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_LEAVE

//! trice64m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 64 bit value
#define trice64m_1(tid, v0)                                                \
	TRICE_ENTER                                                            \
	TRICE_PUT((8 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT64_1(v0)                                                      \
	TRICE_LEAVE

#define trice64m_2(tid, v0, v1)                                             \
	TRICE_ENTER                                                             \
	TRICE_PUT((16 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT64_2(v0, v1);                                                  \
	TRICE_LEAVE

#define trice64m_3(tid, v0, v1, v2)                                         \
	TRICE_ENTER                                                             \
	TRICE_PUT((24 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT64_3(v0, v1, v2);                                              \
	TRICE_LEAVE

#define trice64m_4(tid, v0, v1, v2, v3)                                     \
	TRICE_ENTER                                                             \
	TRICE_PUT((32 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT64_4(v0, v1, v2, v3);                                          \
	TRICE_LEAVE

#define trice64m_5(tid, v0, v1, v2, v3, v4)                                 \
	TRICE_ENTER                                                             \
	TRICE_PUT((40 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                                      \
	TRICE_LEAVE

#define trice64m_6(tid, v0, v1, v2, v3, v4, v5)                             \
	TRICE_ENTER                                                             \
	TRICE_PUT((48 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                                  \
	TRICE_LEAVE

#define trice64m_7(tid, v0, v1, v2, v3, v4, v5, v6)                         \
	TRICE_ENTER                                                             \
	TRICE_PUT((56 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);                              \
	TRICE_LEAVE

#define trice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)                     \
	TRICE_ENTER                                                             \
	TRICE_PUT((64 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);                          \
	TRICE_LEAVE

#define trice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)                 \
	TRICE_ENTER                                                             \
	TRICE_PUT((72 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                      \
	TRICE_LEAVE

#define trice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)            \
	TRICE_ENTER                                                             \
	TRICE_PUT((80 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);                 \
	TRICE_LEAVE

#define trice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)       \
	TRICE_ENTER                                                             \
	TRICE_PUT((88 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);            \
	TRICE_LEAVE

#define trice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)  \
	TRICE_ENTER                                                             \
	TRICE_PUT((96 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)        \
	TRICE_LEAVE

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define trice64_0(fmt)                                                    //!< trice64_1 is a macro calling a function to reduce code size.
#define trice64_1(fmt, v0)                                                //!< trice64_1 is a macro calling a function to reduce code size.
#define trice64_2(fmt, v0, v1)                                            //!< trice64_2 is a macro calling a function to reduce code size.
#define trice64_3(fmt, v0, v1, v2)                                        //!< trice64_3 is a macro calling a function to reduce code size.
#define trice64_4(fmt, v0, v1, v2, v3)                                    //!< trice64_4 is a macro calling a function to reduce code size.
#define trice64_5(fmt, v0, v1, v2, v3, v4)                                //!< trice64_5 is a macro calling a function to reduce code size.
#define trice64_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< trice64_6 is a macro calling a function to reduce code size.
#define trice64_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< trice64_7 is a macro calling a function to reduce code size.
#define trice64_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< trice64_8 is a macro calling a function to reduce code size.
#define trice64_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< trice64_9 is a macro calling a function to reduce code size.
#define trice64_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< trice64_10 is a macro calling a function to reduce code size.
#define trice64_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< trice64_11 is a macro calling a function to reduce code size.
#define trice64_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< trice64_12 is a macro calling a function to reduce code size.

#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define trice64_0(tid, fmt) trice64fn_0(tid)                                                                                                                                                                                                                                                       //!< trice64_1 is a macro calling a function to reduce code size.
#define trice64_1(tid, fmt, v0) trice64fn_1(tid, (uint64_t)(v0))                                                                                                                                                                                                                                   //!< trice64_1 is a macro calling a function to reduce code size.
#define trice64_2(tid, fmt, v0, v1) trice64fn_2(tid, (uint64_t)(v0), (uint64_t)(v1))                                                                                                                                                                                                               //!< trice64_2 is a macro calling a function to reduce code size.
#define trice64_3(tid, fmt, v0, v1, v2) trice64fn_3(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2))                                                                                                                                                                                           //!< trice64_3 is a macro calling a function to reduce code size.
#define trice64_4(tid, fmt, v0, v1, v2, v3) trice64fn_4(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3))                                                                                                                                                                       //!< trice64_4 is a macro calling a function to reduce code size.
#define trice64_5(tid, fmt, v0, v1, v2, v3, v4) trice64fn_5(tid, (uint64_t)v0, (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4))                                                                                                                                                     //!< trice64_5 is a macro calling a function to reduce code size.
#define trice64_6(tid, fmt, v0, v1, v2, v3, v4, v5) trice64fn_6(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5))                                                                                                                               //!< trice64_6 is a macro calling a function to reduce code size.
#define trice64_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) trice64fn_7(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6))                                                                                                           //!< trice64_7 is a macro calling a function to reduce code size.
#define trice64_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) trice64fn_8(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7))                                                                                       //!< trice64_8 is a macro calling a function to reduce code size.
#define trice64_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) trice64fn_9(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8))                                                                   //!< trice64_9 is a macro calling a function to reduce code size.
#define trice64_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) trice64fn_10(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9))                                             //!< trice64_10 is a macro calling a function to reduce code size.
#define trice64_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) trice64fn_11(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9), (uint64_t)(v10))                       //!< trice64_11 is a macro calling a function to reduce code size.
#define trice64_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) trice64fn_12(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9), (uint64_t)(v10), (uint64_t)(v11)) //!< trice64_12 is a macro calling a function to reduce code size.

void trice64fn_0(uint16_t tid);
void trice64fn_1(uint16_t tid, uint64_t v0);
void trice64fn_2(uint16_t tid, uint64_t v0, uint64_t v1);
void trice64fn_3(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2);
void trice64fn_4(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3);
void trice64fn_5(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4);
void trice64fn_6(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5);
void trice64fn_7(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6);
void trice64fn_8(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7);
void trice64fn_9(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8);
void trice64fn_10(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9);
void trice64fn_11(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10);
void trice64fn_12(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10, uint64_t v11);

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#define Trice64m_0(tid)                          \
	TRICE_ENTER                                  \
	uint16_t ts = TriceStamp16;                  \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid)); \
	TRICE_PUT((TRICE_CYCLE << 16) | ts);         \
	TRICE_LEAVE

//! Trice64m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 64 bit value
#define Trice64m_1(tid, v0)                        \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(8 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_1(v0)                              \
	TRICE_LEAVE

#define Trice64m_2(tid, v0, v1)                     \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(16 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_2(v0, v1);                          \
	TRICE_LEAVE

#define Trice64m_3(tid, v0, v1, v2)                 \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(24 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_3(v0, v1, v2);                      \
	TRICE_LEAVE

#define Trice64m_4(tid, v0, v1, v2, v3)             \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(32 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_4(v0, v1, v2, v3);                  \
	TRICE_LEAVE

#define Trice64m_5(tid, v0, v1, v2, v3, v4)         \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(40 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);              \
	TRICE_LEAVE

#define Trice64m_6(tid, v0, v1, v2, v3, v4, v5)     \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(48 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);          \
	TRICE_LEAVE

#define Trice64m_7(tid, v0, v1, v2, v3, v4, v5, v6) \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(56 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);      \
	TRICE_LEAVE

#define Trice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_ENTER                                         \
	uint16_t ts = TriceStamp16;                         \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));        \
	TRICE_PUT(64 << 24 | (TRICE_CYCLE << 16) | ts);     \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);      \
	TRICE_LEAVE

#define Trice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));            \
	TRICE_PUT(72 << 24 | (TRICE_CYCLE << 16) | ts);         \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define Trice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint16_t ts = TriceStamp16;                                  \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                 \
	TRICE_PUT(80 << 24 | (TRICE_CYCLE << 16) | ts);              \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint16_t ts = TriceStamp16;                                       \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                      \
	TRICE_PUT(88 << 24 | (TRICE_CYCLE << 16) | ts);                   \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint16_t ts = TriceStamp16;                                            \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                           \
	TRICE_PUT(96 << 24 | (TRICE_CYCLE << 16) | ts);                        \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#define Trice64m_0(tid)                                  \
	TRICE_ENTER                                          \
	uint16_t ts = TriceStamp16;                          \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));         \
	TRICE_PUT(0 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_LEAVE

//! Trice64m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 64 bit value
#define Trice64m_1(tid, v0)                              \
	TRICE_ENTER                                          \
	uint16_t ts = TriceStamp16;                          \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));         \
	TRICE_PUT(8 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_1(v0)                                    \
	TRICE_LEAVE

#define Trice64m_2(tid, v0, v1)                           \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(16 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_2(v0, v1);                                \
	TRICE_LEAVE

#define Trice64m_3(tid, v0, v1, v2)                       \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(24 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_3(v0, v1, v2);                            \
	TRICE_LEAVE

#define Trice64m_4(tid, v0, v1, v2, v3)                   \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(32 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_4(v0, v1, v2, v3);                        \
	TRICE_LEAVE

#define Trice64m_5(tid, v0, v1, v2, v3, v4)               \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(40 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                    \
	TRICE_LEAVE

#define Trice64m_6(tid, v0, v1, v2, v3, v4, v5)           \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(48 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                \
	TRICE_LEAVE

#define Trice64m_7(tid, v0, v1, v2, v3, v4, v5, v6)       \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(56 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);            \
	TRICE_LEAVE

#define Trice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)   \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));          \
	TRICE_PUT(64 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);        \
	TRICE_LEAVE

#define Trice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));            \
	TRICE_PUT(72 << 8 | (TRICE_CYCLE << 0) | (ts << 16));   \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define Trice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint16_t ts = TriceStamp16;                                  \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                 \
	TRICE_PUT(80 << 8 | (TRICE_CYCLE << 0) | (ts << 16));        \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint16_t ts = TriceStamp16;                                       \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                      \
	TRICE_PUT(88 << 8 | (TRICE_CYCLE << 0) | (ts << 16));             \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint16_t ts = TriceStamp16;                                            \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                           \
	TRICE_PUT(96 << 8 | (TRICE_CYCLE << 0) | (ts << 16));                  \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define Trice64_0(fmt)                                                    //!< Trice64_1 is a macro calling a function to reduce code size.
#define Trice64_1(fmt, v0)                                                //!< Trice64_1 is a macro calling a function to reduce code size.
#define Trice64_2(fmt, v0, v1)                                            //!< Trice64_2 is a macro calling a function to reduce code size.
#define Trice64_3(fmt, v0, v1, v2)                                        //!< Trice64_3 is a macro calling a function to reduce code size.
#define Trice64_4(fmt, v0, v1, v2, v3)                                    //!< Trice64_4 is a macro calling a function to reduce code size.
#define Trice64_5(fmt, v0, v1, v2, v3, v4)                                //!< Trice64_5 is a macro calling a function to reduce code size.
#define Trice64_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< Trice64_6 is a macro calling a function to reduce code size.
#define Trice64_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< Trice64_7 is a macro calling a function to reduce code size.
#define Trice64_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< Trice64_8 is a macro calling a function to reduce code size.
#define Trice64_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< Trice64_9 is a macro calling a function to reduce code size.
#define Trice64_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< Trice64_10 is a macro calling a function to reduce code size.
#define Trice64_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< Trice64_11 is a macro calling a function to reduce code size.
#define Trice64_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< Trice64_12 is a macro calling a function to reduce code size.

#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define Trice64_0(tid, fmt) Trice64fn_0(tid)                                                                                                                                                                                                                                                       //!< Trice64_0 is a macro calling a function to reduce code size.
#define Trice64_1(tid, fmt, v0) Trice64fn_1(tid, (uint64_t)(v0))                                                                                                                                                                                                                                   //!< Trice64_1 is a macro calling a function to reduce code size.
#define Trice64_2(tid, fmt, v0, v1) Trice64fn_2(tid, (uint64_t)(v0), (uint64_t)(v1))                                                                                                                                                                                                               //!< Trice64_2 is a macro calling a function to reduce code size.
#define Trice64_3(tid, fmt, v0, v1, v2) Trice64fn_3(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2))                                                                                                                                                                                           //!< Trice64_3 is a macro calling a function to reduce code size.
#define Trice64_4(tid, fmt, v0, v1, v2, v3) Trice64fn_4(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3))                                                                                                                                                                       //!< Trice64_4 is a macro calling a function to reduce code size.
#define Trice64_5(tid, fmt, v0, v1, v2, v3, v4) Trice64fn_5(tid, (uint64_t)v0, (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4))                                                                                                                                                     //!< Trice64_5 is a macro calling a function to reduce code size.
#define Trice64_6(tid, fmt, v0, v1, v2, v3, v4, v5) Trice64fn_6(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5))                                                                                                                               //!< Trice64_6 is a macro calling a function to reduce code size.
#define Trice64_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) Trice64fn_7(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6))                                                                                                           //!< Trice64_7 is a macro calling a function to reduce code size.
#define Trice64_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) Trice64fn_8(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7))                                                                                       //!< Trice64_8 is a macro calling a function to reduce code size.
#define Trice64_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) Trice64fn_9(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8))                                                                   //!< Trice64_9 is a macro calling a function to reduce code size.
#define Trice64_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) Trice64fn_10(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9))                                             //!< Trice64_10 is a macro calling a function to reduce code size.
#define Trice64_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) Trice64fn_11(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9), (uint64_t)(v10))                       //!< Trice64_11 is a macro calling a function to reduce code size.
#define Trice64_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) Trice64fn_12(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9), (uint64_t)(v10), (uint64_t)(v11)) //!< Trice64_12 is a macro calling a function to reduce code size.

void Trice64fn_0(uint16_t tid);
void Trice64fn_1(uint16_t tid, uint64_t v0);
void Trice64fn_2(uint16_t tid, uint64_t v0, uint64_t v1);
void Trice64fn_3(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2);
void Trice64fn_4(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3);
void Trice64fn_5(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4);
void Trice64fn_6(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5);
void Trice64fn_7(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6);
void Trice64fn_8(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7);
void Trice64fn_9(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8);
void Trice64fn_10(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9);
void Trice64fn_11(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10);
void Trice64fn_12(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10, uint64_t v11);

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

//! TRice64m_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 14 bit Trice id in upper 2 bytes of a 32 bit value
#define TRice64m_0(tid)                                    \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(0 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_LEAVE

//! TRice64m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 14 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 64 bit value
#define TRice64m_1(tid, v0)                                \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(8 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_1(v0)                                      \
	TRICE_LEAVE

#define TRice64m_2(tid, v0, v1)                             \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(16 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_2(v0, v1);                                  \
	TRICE_LEAVE

#define TRice64m_3(tid, v0, v1, v2)                         \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(24 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_3(v0, v1, v2);                              \
	TRICE_LEAVE

#define TRice64m_4(tid, v0, v1, v2, v3)                     \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(32 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_4(v0, v1, v2, v3);                          \
	TRICE_LEAVE

#define TRice64m_5(tid, v0, v1, v2, v3, v4)                 \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(40 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                      \
	TRICE_LEAVE

#define TRice64m_6(tid, v0, v1, v2, v3, v4, v5)             \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(48 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                  \
	TRICE_LEAVE

#define TRice64m_7(tid, v0, v1, v2, v3, v4, v5, v6)         \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(56 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);              \
	TRICE_LEAVE

#define TRice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)     \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(64 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);          \
	TRICE_LEAVE

#define TRice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(72 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define TRice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint32_t ts = TriceStamp32;                                  \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                      \
	TRICE_PUT(80 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));      \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint32_t ts = TriceStamp32;                                       \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                           \
	TRICE_PUT(88 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));           \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint32_t ts = TriceStamp32;                                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                                \
	TRICE_PUT(96 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));                \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

//! TRice64m_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 14 bit Trice id in upper 2 bytes of a 32 bit value
#define TRice64m_0(tid)                                    \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));      \
	TRICE_PUT((0 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_LEAVE

//! TRice64m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 14 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 64 bit value
#define TRice64m_1(tid, v0)                                \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));      \
	TRICE_PUT((8 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_1(v0)                                      \
	TRICE_LEAVE

#define TRice64m_2(tid, v0, v1)                             \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((16 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_2(v0, v1);                                  \
	TRICE_LEAVE

#define TRice64m_3(tid, v0, v1, v2)                         \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((24 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_3(v0, v1, v2);                              \
	TRICE_LEAVE

#define TRice64m_4(tid, v0, v1, v2, v3)                     \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((32 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_4(v0, v1, v2, v3);                          \
	TRICE_LEAVE

#define TRice64m_5(tid, v0, v1, v2, v3, v4)                 \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((40 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                      \
	TRICE_LEAVE

#define TRice64m_6(tid, v0, v1, v2, v3, v4, v5)             \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((48 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                  \
	TRICE_LEAVE

#define TRice64m_7(tid, v0, v1, v2, v3, v4, v5, v6)         \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((56 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);              \
	TRICE_LEAVE

#define TRice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)     \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((64 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);          \
	TRICE_LEAVE

#define TRice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((72 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define TRice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint32_t ts = TriceStamp32;                                  \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));            \
	TRICE_PUT((80 << 8) | (TRICE_CYCLE << 0) | (ts << 16));      \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint32_t ts = TriceStamp32;                                       \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));                 \
	TRICE_PUT((88 << 8) | (TRICE_CYCLE << 0) | (ts << 16));           \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint32_t ts = TriceStamp32;                                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));                      \
	TRICE_PUT((96 << 8) | (TRICE_CYCLE << 0) | (ts << 16));                \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define TRice64_0(fmt)                                                    //!< TRice64_1 is a macro calling a function to reduce code size.
#define TRice64_1(fmt, v0)                                                //!< TRice64_1 is a macro calling a function to reduce code size.
#define TRice64_2(fmt, v0, v1)                                            //!< TRice64_2 is a macro calling a function to reduce code size.
#define TRice64_3(fmt, v0, v1, v2)                                        //!< TRice64_3 is a macro calling a function to reduce code size.
#define TRice64_4(fmt, v0, v1, v2, v3)                                    //!< TRice64_4 is a macro calling a function to reduce code size.
#define TRice64_5(fmt, v0, v1, v2, v3, v4)                                //!< TRice64_5 is a macro calling a function to reduce code size.
#define TRice64_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< TRice64_6 is a macro calling a function to reduce code size.
#define TRice64_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< TRice64_7 is a macro calling a function to reduce code size.
#define TRice64_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< TRice64_8 is a macro calling a function to reduce code size.
#define TRice64_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< TRice64_9 is a macro calling a function to reduce code size.
#define TRice64_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< TRice64_10 is a macro calling a function to reduce code size.
#define TRice64_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< TRice64_11 is a macro calling a function to reduce code size.
#define TRice64_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< TRice64_12 is a macro calling a function to reduce code size.

#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define TRice64_0(tid, fmt) TRice64fn_0(tid)                                                                                                                                                                                                                                                       //!< TRice64_0 is a macro calling a function to reduce code size.
#define TRice64_1(tid, fmt, v0) TRice64fn_1(tid, (uint64_t)(v0))                                                                                                                                                                                                                                   //!< TRice64_1 is a macro calling a function to reduce code size.
#define TRice64_2(tid, fmt, v0, v1) TRice64fn_2(tid, (uint64_t)(v0), (uint64_t)(v1))                                                                                                                                                                                                               //!< TRice64_2 is a macro calling a function to reduce code size.
#define TRice64_3(tid, fmt, v0, v1, v2) TRice64fn_3(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2))                                                                                                                                                                                           //!< TRice64_3 is a macro calling a function to reduce code size.
#define TRice64_4(tid, fmt, v0, v1, v2, v3) TRice64fn_4(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3))                                                                                                                                                                       //!< TRice64_4 is a macro calling a function to reduce code size.
#define TRice64_5(tid, fmt, v0, v1, v2, v3, v4) TRice64fn_5(tid, (uint64_t)v0, (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4))                                                                                                                                                     //!< TRice64_5 is a macro calling a function to reduce code size.
#define TRice64_6(tid, fmt, v0, v1, v2, v3, v4, v5) TRice64fn_6(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5))                                                                                                                               //!< TRice64_6 is a macro calling a function to reduce code size.
#define TRice64_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) TRice64fn_7(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6))                                                                                                           //!< TRice64_7 is a macro calling a function to reduce code size.
#define TRice64_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) TRice64fn_8(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7))                                                                                       //!< TRice64_64_M is a macro calling a function to reduce code size.
#define TRice64_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) TRice64fn_9(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8))                                                                   //!< TRice64_9 is a macro calling a function to reduce code size.
#define TRice64_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) TRice64fn_10(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9))                                             //!< TRice64_10 is a macro calling a function to reduce code size.
#define TRice64_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) TRice64fn_11(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9), (uint64_t)(v10))                       //!< TRice64_11 is a macro calling a function to reduce code size.
#define TRice64_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) TRice64fn_12(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9), (uint64_t)(v10), (uint64_t)(v11)) //!< TRice64_12 is a macro calling a function to reduce code size.

void TRice64fn_0(uint16_t tid);
void TRice64fn_1(uint16_t tid, uint64_t v0);
void TRice64fn_2(uint16_t tid, uint64_t v0, uint64_t v1);
void TRice64fn_3(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2);
void TRice64fn_4(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3);
void TRice64fn_5(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4);
void TRice64fn_6(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5);
void TRice64fn_7(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6);
void TRice64fn_8(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7);
void TRice64fn_9(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8);
void TRice64fn_10(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9);
void TRice64fn_11(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10);
void TRice64fn_12(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10, uint64_t v11);

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

///////////////////////////////////////////////////////////////////////////////
//

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

//
///////////////////////////////////////////////////////////////////////////////
