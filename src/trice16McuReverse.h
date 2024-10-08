/*! \file trice16McuReverse.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

//! trice16m_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define trice16m_0(tid)                      \
	TRICE_ENTER                              \
	TRICE_PUT(((TRICE_CYCLE) << 24) | idLH); \
	TRICE_LEAVE

//! trice16m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define trice16m_1(tid, v0)                                                    \
	TRICE_ENTER                                                                \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (2 << 16) | idLH);                       \
	/*TRICE_PUT((2 << 8) | ((TRICE_CYCLE) << 0) | ((0x4000 | (tid)) << 16));*/ \
	TRICE_PUT16_1(v0)                                                          \
	TRICE_LEAVE

#define trice16m_2(tid, v0, v1)                          \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (4 << 16) | idLH); \
	TRICE_PUT16_2(v0, v1);                               \
	TRICE_LEAVE

#define trice16m_3(tid, v0, v1, v2)                      \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (6 << 16) | idLH); \
	TRICE_PUT16_3(v0, v1, v2);                           \
	TRICE_LEAVE

#define trice16m_4(tid, v0, v1, v2, v3)                  \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (8 << 16) | idLH); \
	TRICE_PUT16_4(v0, v1, v2, v3);                       \
	TRICE_LEAVE

#define trice16m_5(tid, v0, v1, v2, v3, v4)               \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (10 << 16) | idLH); \
	TRICE_PUT16_5(v0, v1, v2, v3, v4);                    \
	TRICE_LEAVE

#define trice16m_6(tid, v0, v1, v2, v3, v4, v5)           \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (12 << 16) | idLH); \
	TRICE_PUT16_6(v0, v1, v2, v3, v4, v5);                \
	TRICE_LEAVE

#define trice16m_7(tid, v0, v1, v2, v3, v4, v5, v6)       \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (14 << 16) | idLH); \
	TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6);            \
	TRICE_LEAVE

#define trice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)   \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (16 << 16) | idLH); \
	TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7);        \
	TRICE_LEAVE

#define trice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (18 << 16) | idLH);   \
	TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define trice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (20 << 16) | idLH);        \
	TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define trice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (22 << 16) | idLH);             \
	TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define trice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (24 << 16) | idLH);                  \
	TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#define Trice16m_0(tid)                         \
	TRICE_ENTER                                 \
	uint16_t ts = TriceStamp16;                 \
	TRICE_PUT(((IdLH) << 16) | IdLH)            \
	TRICE_PUT((TRICE_CYCLE << 24) | tsL | tsH); \
	TRICE_LEAVE

//! Trice16m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define Trice16m_1(tid, v0)                                 \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (2 << 16) | tsL | tsH); \
	TRICE_PUT16_1(v0)                                       \
	TRICE_LEAVE

#define Trice16m_2(tid, v0, v1)                             \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (4 << 16) | tsL | tsH); \
	TRICE_PUT16_2(v0, v1);                                  \
	TRICE_LEAVE

#define Trice16m_3(tid, v0, v1, v2)                         \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (6 << 16) | tsL | tsH); \
	TRICE_PUT16_3(v0, v1, v2);                              \
	TRICE_LEAVE

#define Trice16m_4(tid, v0, v1, v2, v3)                     \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | tsL | tsH); \
	TRICE_PUT16_4(v0, v1, v2, v3);                          \
	TRICE_LEAVE

#define Trice16m_5(tid, v0, v1, v2, v3, v4)                  \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (10 << 16) | tsL | tsH); \
	TRICE_PUT16_5(v0, v1, v2, v3, v4);                       \
	TRICE_LEAVE

#define Trice16m_6(tid, v0, v1, v2, v3, v4, v5)              \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (12 << 16) | tsL | tsH); \
	TRICE_PUT16_6(v0, v1, v2, v3, v4, v5);                   \
	TRICE_LEAVE

#define Trice16m_7(tid, v0, v1, v2, v3, v4, v5, v6)          \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (14 << 16) | tsL | tsH); \
	TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6);               \
	TRICE_LEAVE

#define Trice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)      \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (16 << 16) | tsL | tsH); \
	TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7);           \
	TRICE_LEAVE

#define Trice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)  \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (18 << 16) | tsL | tsH); \
	TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);       \
	TRICE_LEAVE

#define Trice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint16_t ts = TriceStamp16;                                  \
	TRICE_PUT(((IdLH) << 16) | IdLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (20 << 16) | tsL | tsH);     \
	TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint16_t ts = TriceStamp16;                                       \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                  \
	TRICE_PUT((TRICE_CYCLE << 24) | (22 << 16) | tsL | tsH);          \
	TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint16_t ts = TriceStamp16;                                            \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                       \
	TRICE_PUT((TRICE_CYCLE << 24) | (24 << 16) | tsL | tsH);               \
	TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#define TRice16m_0(tid)                          \
	TRICE_ENTER                                  \
	uint32_t ts = TriceStamp32;                  \
	TRICE_PUT(tsHL | tsHH | IDLH)                \
	TRICE_PUT((TRICE_CYCLE << 24) | tsLL | tsLH) \
	TRICE_LEAVE

//! TRice16m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 14 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define TRice16m_1(tid, v0)                                  \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (2 << 16) | tsLL | tsLH) \
	TRICE_PUT16_1(v0)                                        \
	TRICE_LEAVE

#define TRice16m_2(tid, v0, v1)                              \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (4 << 16) | tsLL | tsLH) \
	TRICE_PUT16_2(v0, v1);                                   \
	TRICE_LEAVE

#define TRice16m_3(tid, v0, v1, v2)                          \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (6 << 16) | tsLL | tsLH) \
	TRICE_PUT16_3(v0, v1, v2);                               \
	TRICE_LEAVE

#define TRice16m_4(tid, v0, v1, v2, v3)                      \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | tsLL | tsLH) \
	TRICE_PUT16_4(v0, v1, v2, v3);                           \
	TRICE_LEAVE

#define TRice16m_5(tid, v0, v1, v2, v3, v4)                   \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (10 << 16) | tsLL | tsLH) \
	TRICE_PUT16_5(v0, v1, v2, v3, v4);                        \
	TRICE_LEAVE

#define TRice16m_6(tid, v0, v1, v2, v3, v4, v5)               \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (12 << 16) | tsLL | tsLH) \
	TRICE_PUT16_6(v0, v1, v2, v3, v4, v5);                    \
	TRICE_LEAVE

#define TRice16m_7(tid, v0, v1, v2, v3, v4, v5, v6)           \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (14 << 16) | tsLL | tsLH) \
	TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6);                \
	TRICE_LEAVE

#define TRice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)       \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (16 << 16) | tsLL | tsLH) \
	TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7);            \
	TRICE_LEAVE

#define TRice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)   \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (18 << 16) | tsLL | tsLH) \
	TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);        \
	TRICE_LEAVE

#define TRice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint32_t ts = TriceStamp32;                                  \
	TRICE_PUT(tsHL | tsHH | IDLH)                                \
	TRICE_PUT((TRICE_CYCLE << 24) | (20 << 16) | tsLL | tsLH)    \
	TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint32_t ts = TriceStamp32;                                       \
	TRICE_PUT(tsHL | tsHH | IDLH)                                     \
	TRICE_PUT((TRICE_CYCLE << 24) | (22 << 16) | tsLL | tsLH)         \
	TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint32_t ts = TriceStamp32;                                            \
	TRICE_PUT(tsHL | tsHH | IDLH)                                          \
	TRICE_PUT((TRICE_CYCLE << 24) | (24 << 16) | tsLL | tsLH)              \
	TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE
