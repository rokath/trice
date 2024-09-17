/*! \file internalTrice16.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 1

#define TRICE_SHORT1(v) (uint16_t)(v)
#define TRICE_SHORT0(v) ((uint32_t)(v) << 16)

#else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 1

#define TRICE_SHORT0(v) (uint16_t)(v)
#define TRICE_SHORT1(v) ((uint32_t)(v) << 16)

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 1

#define TRICE16(tid, fmt, ...) TRICE_CONCAT2(TRICE16_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

#if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define trice16(fmt, ...)
#define Trice16(fmt, ...)
#define TRice16(fmt, ...)

#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define trice16(tid, fmt, ...) TRICE_CONCAT2(trice16_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define Trice16(tid, fmt, ...) TRICE_CONCAT2(Trice16_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRice16(tid, fmt, ...) TRICE_CONCAT2(TRice16_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

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

#if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

//! trice16m_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define trice16m_0(tid)                                              \
	TRICE_ENTER                                                      \
	TRICE_PUT((0 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_LEAVE

//! trice16m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define trice16m_1(tid, v0)                                          \
	TRICE_ENTER                                                      \
	TRICE_PUT((2 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT16_1(v0)                                                \
	TRICE_LEAVE

#define trice16m_2(tid, v0, v1)                                      \
	TRICE_ENTER                                                      \
	TRICE_PUT((4 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT16_2(v0, v1);                                           \
	TRICE_LEAVE

#define trice16m_3(tid, v0, v1, v2)                                  \
	TRICE_ENTER                                                      \
	TRICE_PUT((6 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT16_3(v0, v1, v2);                                       \
	TRICE_LEAVE

#define trice16m_4(tid, v0, v1, v2, v3)                              \
	TRICE_ENTER                                                      \
	TRICE_PUT((8 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT16_4(v0, v1, v2, v3);                                   \
	TRICE_LEAVE

#define trice16m_5(tid, v0, v1, v2, v3, v4)                           \
	TRICE_ENTER                                                       \
	TRICE_PUT((10 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT16_5(v0, v1, v2, v3, v4);                                \
	TRICE_LEAVE

#define trice16m_6(tid, v0, v1, v2, v3, v4, v5)                       \
	TRICE_ENTER                                                       \
	TRICE_PUT((12 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT16_6(v0, v1, v2, v3, v4, v5);                            \
	TRICE_LEAVE

#define trice16m_7(tid, v0, v1, v2, v3, v4, v5, v6)                   \
	TRICE_ENTER                                                       \
	TRICE_PUT((14 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6);                        \
	TRICE_LEAVE

#define trice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)               \
	TRICE_ENTER                                                       \
	TRICE_PUT((16 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7);                    \
	TRICE_LEAVE

#define trice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)           \
	TRICE_ENTER                                                       \
	TRICE_PUT((18 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                \
	TRICE_LEAVE

#define trice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)      \
	TRICE_ENTER                                                       \
	TRICE_PUT((20 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);           \
	TRICE_LEAVE

#define trice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	TRICE_PUT((22 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define trice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	TRICE_PUT((24 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid)));      \
	TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

//! trice16m_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define trice16m_0(tid)                                                    \
	TRICE_ENTER                                                            \
	TRICE_PUT((0 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_LEAVE

//! trice16m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define trice16m_1(tid, v0)                                                \
	TRICE_ENTER                                                            \
	TRICE_PUT((2 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT16_1(v0)                                                      \
	TRICE_LEAVE

#define trice16m_2(tid, v0, v1)                                            \
	TRICE_ENTER                                                            \
	TRICE_PUT((4 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT16_2(v0, v1);                                                 \
	TRICE_LEAVE

#define trice16m_3(tid, v0, v1, v2)                                        \
	TRICE_ENTER                                                            \
	TRICE_PUT((6 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT16_3(v0, v1, v2);                                             \
	TRICE_LEAVE

#define trice16m_4(tid, v0, v1, v2, v3)                                    \
	TRICE_ENTER                                                            \
	TRICE_PUT((8 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT16_4(v0, v1, v2, v3);                                         \
	TRICE_LEAVE

#define trice16m_5(tid, v0, v1, v2, v3, v4)                                 \
	TRICE_ENTER                                                             \
	TRICE_PUT((10 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT16_5(v0, v1, v2, v3, v4);                                      \
	TRICE_LEAVE

#define trice16m_6(tid, v0, v1, v2, v3, v4, v5)                             \
	TRICE_ENTER                                                             \
	TRICE_PUT((12 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT16_6(v0, v1, v2, v3, v4, v5);                                  \
	TRICE_LEAVE

#define trice16m_7(tid, v0, v1, v2, v3, v4, v5, v6)                         \
	TRICE_ENTER                                                             \
	TRICE_PUT((14 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6);                              \
	TRICE_LEAVE

#define trice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)                     \
	TRICE_ENTER                                                             \
	TRICE_PUT((16 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7);                          \
	TRICE_LEAVE

#define trice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)                 \
	TRICE_ENTER                                                             \
	TRICE_PUT((18 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                      \
	TRICE_LEAVE

#define trice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)            \
	TRICE_ENTER                                                             \
	TRICE_PUT((20 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);                 \
	TRICE_LEAVE

#define trice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)       \
	TRICE_ENTER                                                             \
	TRICE_PUT((22 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);            \
	TRICE_LEAVE

#define trice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)  \
	TRICE_ENTER                                                             \
	TRICE_PUT((24 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16)); \
	TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)        \
	TRICE_LEAVE

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define trice16_0(fmt)                                                    //!< trice16_1 is a macro calling a function to reduce code size.
#define trice16_1(fmt, v0)                                                //!< trice16_1 is a macro calling a function to reduce code size.
#define trice16_2(fmt, v0, v1)                                            //!< trice16_2 is a macro calling a function to reduce code size.
#define trice16_3(fmt, v0, v1, v2)                                        //!< trice16_3 is a macro calling a function to reduce code size.
#define trice16_4(fmt, v0, v1, v2, v3)                                    //!< trice16_4 is a macro calling a function to reduce code size.
#define trice16_5(fmt, v0, v1, v2, v3, v4)                                //!< trice16_5 is a macro calling a function to reduce code size.
#define trice16_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< trice16_6 is a macro calling a function to reduce code size.
#define trice16_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< trice16_7 is a macro calling a function to reduce code size.
#define trice16_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< trice16_8 is a macro calling a function to reduce code size.
#define trice16_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< trice16_9 is a macro calling a function to reduce code size.
#define trice16_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< trice16_10 is a macro calling a function to reduce code size.
#define trice16_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< trice16_11 is a macro calling a function to reduce code size.
#define trice16_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< trice16_12 is a macro calling a function to reduce code size.

#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define trice16_0(tid, fmt) trice16fn_0(tid)                                                                                                                                                                                                                                                       //!< trice16_0 is a macro calling a function to reduce code size.
#define trice16_1(tid, fmt, v0) trice16fn_1(tid, (uint16_t)(v0))                                                                                                                                                                                                                                   //!< trice16_1 is a macro calling a function to reduce code size.
#define trice16_2(tid, fmt, v0, v1) trice16fn_2(tid, (uint16_t)(v0), (uint16_t)(v1))                                                                                                                                                                                                               //!< trice16_2 is a macro calling a function to reduce code size.
#define trice16_3(tid, fmt, v0, v1, v2) trice16fn_3(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2))                                                                                                                                                                                           //!< trice16_3 is a macro calling a function to reduce code size.
#define trice16_4(tid, fmt, v0, v1, v2, v3) trice16fn_4(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3))                                                                                                                                                                       //!< trice16_4 is a macro calling a function to reduce code size.
#define trice16_5(tid, fmt, v0, v1, v2, v3, v4) trice16fn_5(tid, (uint16_t)v0, (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4))                                                                                                                                                     //!< trice16_5 is a macro calling a function to reduce code size.
#define trice16_6(tid, fmt, v0, v1, v2, v3, v4, v5) trice16fn_6(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5))                                                                                                                               //!< trice16_6 is a macro calling a function to reduce code size.
#define trice16_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) trice16fn_7(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6))                                                                                                           //!< trice16_7 is a macro calling a function to reduce code size.
#define trice16_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) trice16fn_8(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7))                                                                                       //!< trice16_8 is a macro calling a function to reduce code size.
#define trice16_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) trice16fn_9(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8))                                                                   //!< trice16_9 is a macro calling a function to reduce code size.
#define trice16_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) trice16fn_10(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9))                                             //!< trice16_10 is a macro calling a function to reduce code size.
#define trice16_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) trice16fn_11(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10))                       //!< trice16_11 is a macro calling a function to reduce code size.
#define trice16_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) trice16fn_12(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10), (uint16_t)(v11)) //!< trice16_12 is a macro calling a function to reduce code size.

void trice16fn_0(uint16_t tid);
void trice16fn_1(uint16_t tid, uint16_t v0);
void trice16fn_2(uint16_t tid, uint16_t v0, uint16_t v1);
void trice16fn_3(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2);
void trice16fn_4(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3);
void trice16fn_5(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4);
void trice16fn_6(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5);
void trice16fn_7(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6);
void trice16fn_8(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7);
void trice16fn_9(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8);
void trice16fn_10(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9);
void trice16fn_11(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10);
void trice16fn_12(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10, uint16_t v11);

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#define Trice16m_0(tid)                            \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid)); \
	TRICE_PUT(0 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_LEAVE

//! Trice16m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define Trice16m_1(tid, v0)                        \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid)); \
	TRICE_PUT(2 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT16_1(v0)                              \
	TRICE_LEAVE

#define Trice16m_2(tid, v0, v1)                    \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid)); \
	TRICE_PUT(4 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT16_2(v0, v1);                         \
	TRICE_LEAVE

#define Trice16m_3(tid, v0, v1, v2)                \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid)); \
	TRICE_PUT(6 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT16_3(v0, v1, v2);                     \
	TRICE_LEAVE

#define Trice16m_4(tid, v0, v1, v2, v3)            \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid)); \
	TRICE_PUT(8 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT16_4(v0, v1, v2, v3);                 \
	TRICE_LEAVE

#define Trice16m_5(tid, v0, v1, v2, v3, v4)         \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));  \
	TRICE_PUT(10 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT16_5(v0, v1, v2, v3, v4);              \
	TRICE_LEAVE

#define Trice16m_6(tid, v0, v1, v2, v3, v4, v5)     \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));  \
	TRICE_PUT(12 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT16_6(v0, v1, v2, v3, v4, v5);          \
	TRICE_LEAVE

#define Trice16m_7(tid, v0, v1, v2, v3, v4, v5, v6) \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));  \
	TRICE_PUT(14 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6);      \
	TRICE_LEAVE

#define Trice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_ENTER                                         \
	uint16_t ts = TriceStamp16;                         \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));      \
	TRICE_PUT(16 << 24 | (TRICE_CYCLE << 16) | ts);     \
	TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7);      \
	TRICE_LEAVE

#define Trice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));          \
	TRICE_PUT(18 << 24 | (TRICE_CYCLE << 16) | ts);         \
	TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define Trice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint16_t ts = TriceStamp16;                                  \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));               \
	TRICE_PUT(20 << 24 | (TRICE_CYCLE << 16) | ts);              \
	TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint16_t ts = TriceStamp16;                                       \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));                    \
	TRICE_PUT(22 << 24 | (TRICE_CYCLE << 16) | ts);                   \
	TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint16_t ts = TriceStamp16;                                            \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));                         \
	TRICE_PUT(24 << 24 | (TRICE_CYCLE << 16) | ts);                        \
	TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#define Trice16m_0(tid)                                  \
	TRICE_ENTER                                          \
	uint16_t ts = TriceStamp16;                          \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));       \
	TRICE_PUT(0 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_LEAVE

//! Trice16m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define Trice16m_1(tid, v0)                              \
	TRICE_ENTER                                          \
	uint16_t ts = TriceStamp16;                          \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));       \
	TRICE_PUT(2 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_1(v0)                                    \
	TRICE_LEAVE

#define Trice16m_2(tid, v0, v1)                          \
	TRICE_ENTER                                          \
	uint16_t ts = TriceStamp16;                          \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));       \
	TRICE_PUT(4 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_2(v0, v1);                               \
	TRICE_LEAVE

#define Trice16m_3(tid, v0, v1, v2)                      \
	TRICE_ENTER                                          \
	uint16_t ts = TriceStamp16;                          \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));       \
	TRICE_PUT(6 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_3(v0, v1, v2);                           \
	TRICE_LEAVE

#define Trice16m_4(tid, v0, v1, v2, v3)                  \
	TRICE_ENTER                                          \
	uint16_t ts = TriceStamp16;                          \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));       \
	TRICE_PUT(8 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_4(v0, v1, v2, v3);                       \
	TRICE_LEAVE

#define Trice16m_5(tid, v0, v1, v2, v3, v4)               \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));        \
	TRICE_PUT(10 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_5(v0, v1, v2, v3, v4);                    \
	TRICE_LEAVE

#define Trice16m_6(tid, v0, v1, v2, v3, v4, v5)           \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));        \
	TRICE_PUT(12 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_6(v0, v1, v2, v3, v4, v5);                \
	TRICE_LEAVE

#define Trice16m_7(tid, v0, v1, v2, v3, v4, v5, v6)       \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));        \
	TRICE_PUT(14 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6);            \
	TRICE_LEAVE

#define Trice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)   \
	TRICE_ENTER                                           \
	uint16_t ts = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));        \
	TRICE_PUT(16 << 8 | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7);        \
	TRICE_LEAVE

#define Trice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));          \
	TRICE_PUT(18 << 8 | (TRICE_CYCLE << 0) | (ts << 16));   \
	TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define Trice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint16_t ts = TriceStamp16;                                  \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));               \
	TRICE_PUT(20 << 8 | (TRICE_CYCLE << 0) | (ts << 16));        \
	TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint16_t ts = TriceStamp16;                                       \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));                    \
	TRICE_PUT(22 << 8 | (TRICE_CYCLE << 0) | (ts << 16));             \
	TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint16_t ts = TriceStamp16;                                            \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));                         \
	TRICE_PUT(24 << 8 | (TRICE_CYCLE << 0) | (ts << 16));                  \
	TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define Trice16_0(fmt)                                                    //!< Trice16_1 is a macro calling a function to reduce code size.
#define Trice16_1(fmt, v0)                                                //!< Trice16_1 is a macro calling a function to reduce code size.
#define Trice16_2(fmt, v0, v1)                                            //!< Trice16_2 is a macro calling a function to reduce code size.
#define Trice16_3(fmt, v0, v1, v2)                                        //!< Trice16_3 is a macro calling a function to reduce code size.
#define Trice16_4(fmt, v0, v1, v2, v3)                                    //!< Trice16_4 is a macro calling a function to reduce code size.
#define Trice16_5(fmt, v0, v1, v2, v3, v4)                                //!< Trice16_5 is a macro calling a function to reduce code size.
#define Trice16_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< Trice16_6 is a macro calling a function to reduce code size.
#define Trice16_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< Trice16_7 is a macro calling a function to reduce code size.
#define Trice16_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< Trice16_8 is a macro calling a function to reduce code size.
#define Trice16_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< Trice16_9 is a macro calling a function to reduce code size.
#define Trice16_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< Trice16_10 is a macro calling a function to reduce code size.
#define Trice16_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< Trice16_11 is a macro calling a function to reduce code size.
#define Trice16_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< Trice16_12 is a macro calling a function to reduce code size.

#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define Trice16_0(tid, fmt) Trice16fn_0(tid)                                                                                                                                                                                                                                                       //!< Trice16_0 is a macro calling a function to reduce code size.
#define Trice16_1(tid, fmt, v0) Trice16fn_1(tid, (uint16_t)(v0))                                                                                                                                                                                                                                   //!< Trice16_1 is a macro calling a function to reduce code size.
#define Trice16_2(tid, fmt, v0, v1) Trice16fn_2(tid, (uint16_t)(v0), (uint16_t)(v1))                                                                                                                                                                                                               //!< Trice16_2 is a macro calling a function to reduce code size.
#define Trice16_3(tid, fmt, v0, v1, v2) Trice16fn_3(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2))                                                                                                                                                                                           //!< Trice16_3 is a macro calling a function to reduce code size.
#define Trice16_4(tid, fmt, v0, v1, v2, v3) Trice16fn_4(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3))                                                                                                                                                                       //!< Trice16_4 is a macro calling a function to reduce code size.
#define Trice16_5(tid, fmt, v0, v1, v2, v3, v4) Trice16fn_5(tid, (uint16_t)v0, (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4))                                                                                                                                                     //!< Trice16_5 is a macro calling a function to reduce code size.
#define Trice16_6(tid, fmt, v0, v1, v2, v3, v4, v5) Trice16fn_6(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5))                                                                                                                               //!< Trice16_6 is a macro calling a function to reduce code size.
#define Trice16_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) Trice16fn_7(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6))                                                                                                           //!< Trice16_7 is a macro calling a function to reduce code size.
#define Trice16_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) Trice16fn_8(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7))                                                                                       //!< Trice16_8 is a macro calling a function to reduce code size.
#define Trice16_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) Trice16fn_9(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8))                                                                   //!< Trice16_9 is a macro calling a function to reduce code size.
#define Trice16_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) Trice16fn_10(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9))                                             //!< Trice16_10 is a macro calling a function to reduce code size.
#define Trice16_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) Trice16fn_11(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10))                       //!< Trice16_11 is a macro calling a function to reduce code size.
#define Trice16_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) Trice16fn_12(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10), (uint16_t)(v11)) //!< Trice16_12 is a macro calling a function to reduce code size.

void Trice16fn_0(uint16_t tid);
void Trice16fn_1(uint16_t tid, uint16_t v0);
void Trice16fn_2(uint16_t tid, uint16_t v0, uint16_t v1);
void Trice16fn_3(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2);
void Trice16fn_4(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3);
void Trice16fn_5(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4);
void Trice16fn_6(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5);
void Trice16fn_7(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6);
void Trice16fn_8(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7);
void Trice16fn_9(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8);
void Trice16fn_10(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9);
void Trice16fn_11(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10);
void Trice16fn_12(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10, uint16_t v11);

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#define TRice16m_0(tid)                                    \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(0 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_LEAVE

//! TRice16m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 14 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define TRice16m_1(tid, v0)                                \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(2 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT16_1(v0)                                      \
	TRICE_LEAVE

#define TRice16m_2(tid, v0, v1)                            \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(4 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT16_2(v0, v1);                                 \
	TRICE_LEAVE

#define TRice16m_3(tid, v0, v1, v2)                        \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(6 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT16_3(v0, v1, v2);                             \
	TRICE_LEAVE

#define TRice16m_4(tid, v0, v1, v2, v3)                    \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(8 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT16_4(v0, v1, v2, v3);                         \
	TRICE_LEAVE

#define TRice16m_5(tid, v0, v1, v2, v3, v4)                 \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(10 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT16_5(v0, v1, v2, v3, v4);                      \
	TRICE_LEAVE

#define TRice16m_6(tid, v0, v1, v2, v3, v4, v5)             \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(12 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT16_6(v0, v1, v2, v3, v4, v5);                  \
	TRICE_LEAVE

#define TRice16m_7(tid, v0, v1, v2, v3, v4, v5, v6)         \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(14 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6);              \
	TRICE_LEAVE

#define TRice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)     \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(16 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7);          \
	TRICE_LEAVE

#define TRice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(18 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define TRice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint32_t ts = TriceStamp32;                                  \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                      \
	TRICE_PUT(20 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));      \
	TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint32_t ts = TriceStamp32;                                       \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                           \
	TRICE_PUT(22 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));           \
	TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint32_t ts = TriceStamp32;                                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                                \
	TRICE_PUT(24 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));                \
	TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#define TRice16m_0(tid)                                    \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));      \
	TRICE_PUT((0 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_LEAVE

//! TRice16m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 14 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define TRice16m_1(tid, v0)                                \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));      \
	TRICE_PUT((2 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_1(v0)                                      \
	TRICE_LEAVE

#define TRice16m_2(tid, v0, v1)                            \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));      \
	TRICE_PUT((4 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_2(v0, v1);                                 \
	TRICE_LEAVE

#define TRice16m_3(tid, v0, v1, v2)                        \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));      \
	TRICE_PUT((6 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_3(v0, v1, v2);                             \
	TRICE_LEAVE

#define TRice16m_4(tid, v0, v1, v2, v3)                    \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));      \
	TRICE_PUT((8 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_4(v0, v1, v2, v3);                         \
	TRICE_LEAVE

#define TRice16m_5(tid, v0, v1, v2, v3, v4)                 \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((10 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_5(v0, v1, v2, v3, v4);                      \
	TRICE_LEAVE

#define TRice16m_6(tid, v0, v1, v2, v3, v4, v5)             \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((12 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_6(v0, v1, v2, v3, v4, v5);                  \
	TRICE_LEAVE

#define TRice16m_7(tid, v0, v1, v2, v3, v4, v5, v6)         \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((14 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6);              \
	TRICE_LEAVE

#define TRice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)     \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((16 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7);          \
	TRICE_LEAVE

#define TRice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));       \
	TRICE_PUT((18 << 8) | (TRICE_CYCLE << 0) | (ts << 16)); \
	TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define TRice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint32_t ts = TriceStamp32;                                  \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));            \
	TRICE_PUT((20 << 8) | (TRICE_CYCLE << 0) | (ts << 16));      \
	TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint32_t ts = TriceStamp32;                                       \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));                 \
	TRICE_PUT((22 << 8) | (TRICE_CYCLE << 0) | (ts << 16));           \
	TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint32_t ts = TriceStamp32;                                            \
	TRICE_PUT((ts >> 16) | ((0xc000 | (tid)) << 16));                      \
	TRICE_PUT((24 << 8) | (TRICE_CYCLE << 0) | (ts << 16));                \
	TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN == 0

#if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define TRice16_0(fmt)                                                    //!< TRice16_1 is a macro calling a function to reduce code size.
#define TRice16_1(fmt, v0)                                                //!< TRice16_1 is a macro calling a function to reduce code size.
#define TRice16_2(fmt, v0, v1)                                            //!< TRice16_2 is a macro calling a function to reduce code size.
#define TRice16_3(fmt, v0, v1, v2)                                        //!< TRice16_3 is a macro calling a function to reduce code size.
#define TRice16_4(fmt, v0, v1, v2, v3)                                    //!< TRice16_4 is a macro calling a function to reduce code size.
#define TRice16_5(fmt, v0, v1, v2, v3, v4)                                //!< TRice16_5 is a macro calling a function to reduce code size.
#define TRice16_6(fmt, v0, v1, v2, v3, v4, v5)                            //!< TRice16_6 is a macro calling a function to reduce code size.
#define TRice16_7(fmt, v0, v1, v2, v3, v4, v5, v6)                        //!< TRice16_7 is a macro calling a function to reduce code size.
#define TRice16_8(fmt, v0, v1, v2, v3, v4, v5, v6, v7)                    //!< TRice16_8 is a macro calling a function to reduce code size.
#define TRice16_9(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8)                //!< TRice16_9 is a macro calling a function to reduce code size.
#define TRice16_10(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)           //!< TRice16_10 is a macro calling a function to reduce code size.
#define TRice16_11(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)      //!< TRice16_11 is a macro calling a function to reduce code size.
#define TRice16_12(fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) //!< TRice16_12 is a macro calling a function to reduce code size.

#else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

#define TRice16_0(tid, fmt) TRice16fn_0(tid)                                                                                                                                                                                                                                                       //!< TRice16_0 is a macro calling a function to reduce code size.
#define TRice16_1(tid, fmt, v0) TRice16fn_1(tid, (uint16_t)(v0))                                                                                                                                                                                                                                   //!< TRice16_1 is a macro calling a function to reduce code size.
#define TRice16_2(tid, fmt, v0, v1) TRice16fn_2(tid, (uint16_t)(v0), (uint16_t)(v1))                                                                                                                                                                                                               //!< TRice16_2 is a macro calling a function to reduce code size.
#define TRice16_3(tid, fmt, v0, v1, v2) TRice16fn_3(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2))                                                                                                                                                                                           //!< TRice16_3 is a macro calling a function to reduce code size.
#define TRice16_4(tid, fmt, v0, v1, v2, v3) TRice16fn_4(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3))                                                                                                                                                                       //!< TRice16_4 is a macro calling a function to reduce code size.
#define TRice16_5(tid, fmt, v0, v1, v2, v3, v4) TRice16fn_5(tid, (uint16_t)v0, (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4))                                                                                                                                                     //!< TRice16_5 is a macro calling a function to reduce code size.
#define TRice16_6(tid, fmt, v0, v1, v2, v3, v4, v5) TRice16fn_6(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5))                                                                                                                               //!< TRice16_6 is a macro calling a function to reduce code size.
#define TRice16_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) TRice16fn_7(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6))                                                                                                           //!< TRice16_7 is a macro calling a function to reduce code size.
#define TRice16_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) TRice16fn_8(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7))                                                                                       //!< TRice16_16_M is a macro calling a function to reduce code size.
#define TRice16_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) TRice16fn_9(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8))                                                                   //!< TRice16_9 is a macro calling a function to reduce code size.
#define TRice16_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) TRice16fn_10(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9))                                             //!< TRice16_10 is a macro calling a function to reduce code size.
#define TRice16_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) TRice16fn_11(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10))                       //!< TRice16_11 is a macro calling a function to reduce code size.
#define TRice16_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) TRice16fn_12(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10), (uint16_t)(v11)) //!< TRice16_12 is a macro calling a function to reduce code size.

void TRice16fn_0(uint16_t tid);
void TRice16fn_1(uint16_t tid, uint16_t v0);
void TRice16fn_2(uint16_t tid, uint16_t v0, uint16_t v1);
void TRice16fn_3(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2);
void TRice16fn_4(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3);
void TRice16fn_5(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4);
void TRice16fn_6(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5);
void TRice16fn_7(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6);
void TRice16fn_8(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7);
void TRice16fn_9(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8);
void TRice16fn_10(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9);
void TRice16fn_11(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10);
void TRice16fn_12(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10, uint16_t v11);

#endif // #else // #if TRICE_OFF == 1 || TRICE_CLEAN == 1

///////////////////////////////////////////////////////////////////////////////
//

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

//
///////////////////////////////////////////////////////////////////////////////
