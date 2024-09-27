/*! \file Trice32McuReverse.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#define trice32m_0(tid)                      \
	TRICE_ENTER                              \
	TRICE_PUT(((TRICE_CYCLE) << 24) | idLH); \
	TRICE_LEAVE

#define trice32m_1(tid, v0)                              \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (4 << 16) | idLH); \
	TRICE_PUT32_1(v0)                                    \
	TRICE_LEAVE

#define trice32m_2(tid, v0, v1)                          \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (8 << 16) | idLH); \
	TRICE_PUT32_2(v0, v1);                               \
	TRICE_LEAVE

#define trice32m_3(tid, v0, v1, v2)                       \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (12 << 16) | idLH); \
	TRICE_PUT32_3(v0, v1, v2);                            \
	TRICE_LEAVE

#define trice32m_4(tid, v0, v1, v2, v3)                   \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (16 << 16) | idLH); \
	TRICE_PUT32_4(v0, v1, v2, v3);                        \
	TRICE_LEAVE

#define trice32m_5(tid, v0, v1, v2, v3, v4)               \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (20 << 16) | idLH); \
	TRICE_PUT32_5(v0, v1, v2, v3, v4);                    \
	TRICE_LEAVE

#define trice32m_6(tid, v0, v1, v2, v3, v4, v5)           \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (24 << 16) | idLH); \
	TRICE_PUT32_6(v0, v1, v2, v3, v4, v5);                \
	TRICE_LEAVE

#define trice32m_7(tid, v0, v1, v2, v3, v4, v5, v6)       \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (28 << 16) | idLH); \
	TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6);            \
	TRICE_LEAVE

#define trice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)   \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (32 << 16) | idLH); \
	TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7);        \
	TRICE_LEAVE

#define trice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (36 << 16) | idLH);   \
	TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define trice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (40 << 16) | idLH);        \
	TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define trice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (44 << 16) | idLH);             \
	TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define trice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (48 << 16) | idLH);                  \
	TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#define Trice32m_0(tid)                         \
	TRICE_ENTER                                 \
	uint16_t ts = TriceStamp16;                 \
	TRICE_PUT(((IdLH) << 16) | IdLH)            \
	TRICE_PUT((TRICE_CYCLE << 24) | tsL | tsH); \
	TRICE_LEAVE

#define Trice32m_1(tid, v0)                                 \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (4 << 16) | tsL | tsH); \
	TRICE_PUT32_1(v0)                                       \
	TRICE_LEAVE

#define Trice32m_2(tid, v0, v1)                             \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | tsL | tsH); \
	TRICE_PUT32_2(v0, v1);                                  \
	TRICE_LEAVE

#define Trice32m_3(tid, v0, v1, v2)                          \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (12 << 16) | tsL | tsH); \
	TRICE_PUT32_3(v0, v1, v2);                               \
	TRICE_LEAVE

#define Trice32m_4(tid, v0, v1, v2, v3)                      \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (16 << 16) | tsL | tsH); \
	TRICE_PUT32_4(v0, v1, v2, v3);                           \
	TRICE_LEAVE

#define Trice32m_5(tid, v0, v1, v2, v3, v4)                  \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (20 << 16) | tsL | tsH); \
	TRICE_PUT32_5(v0, v1, v2, v3, v4);                       \
	TRICE_LEAVE

#define Trice32m_6(tid, v0, v1, v2, v3, v4, v5)              \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (24 << 16) | tsL | tsH); \
	TRICE_PUT32_6(v0, v1, v2, v3, v4, v5);                   \
	TRICE_LEAVE

#define Trice32m_7(tid, v0, v1, v2, v3, v4, v5, v6)          \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (28 << 16) | tsL | tsH); \
	TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6);               \
	TRICE_LEAVE

#define Trice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)      \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (32 << 16) | tsL | tsH); \
	TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7);           \
	TRICE_LEAVE

#define Trice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)  \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (36 << 16) | tsL | tsH); \
	TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);       \
	TRICE_LEAVE

#define Trice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint16_t ts = TriceStamp16;                                  \
	TRICE_PUT(((IdLH) << 16) | IdLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (40 << 16) | tsL | tsH);     \
	TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint16_t ts = TriceStamp16;                                       \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                  \
	TRICE_PUT((TRICE_CYCLE << 24) | (44 << 16) | tsL | tsH);          \
	TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint16_t ts = TriceStamp16;                                            \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                       \
	TRICE_PUT((TRICE_CYCLE << 24) | (48 << 16) | tsL | tsH);               \
	TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#define TRice32m_0(tid)                          \
	TRICE_ENTER                                  \
	uint32_t ts = TriceStamp32;                  \
	TRICE_PUT(tsHL | tsHH | IDLH)                \
	TRICE_PUT((TRICE_CYCLE << 24) | tsLL | tsLH) \
	TRICE_LEAVE

#define TRice32m_1(tid, v0)                                  \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (4 << 16) | tsLL | tsLH) \
	TRICE_PUT32_1(v0)                                        \
	TRICE_LEAVE

#define TRice32m_2(tid, v0, v1)                              \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | tsLL | tsLH) \
	TRICE_PUT32_2(v0, v1);                                   \
	TRICE_LEAVE

#define TRice32m_3(tid, v0, v1, v2)                           \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (12 << 16) | tsLL | tsLH) \
	TRICE_PUT32_3(v0, v1, v2);                                \
	TRICE_LEAVE

#define TRice32m_4(tid, v0, v1, v2, v3)                       \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (16 << 16) | tsLL | tsLH) \
	TRICE_PUT32_4(v0, v1, v2, v3);                            \
	TRICE_LEAVE

#define TRice32m_5(tid, v0, v1, v2, v3, v4)                   \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (20 << 16) | tsLL | tsLH) \
	TRICE_PUT32_5(v0, v1, v2, v3, v4);                        \
	TRICE_LEAVE

#define TRice32m_6(tid, v0, v1, v2, v3, v4, v5)               \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (24 << 16) | tsLL | tsLH) \
	TRICE_PUT32_6(v0, v1, v2, v3, v4, v5);                    \
	TRICE_LEAVE

#define TRice32m_7(tid, v0, v1, v2, v3, v4, v5, v6)           \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (28 << 16) | tsLL | tsLH) \
	TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6);                \
	TRICE_LEAVE

#define TRice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)       \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (32 << 16) | tsLL | tsLH) \
	TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7);            \
	TRICE_LEAVE

#define TRice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)   \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (36 << 16) | tsLL | tsLH) \
	TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);        \
	TRICE_LEAVE

#define TRice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint32_t ts = TriceStamp32;                                  \
	TRICE_PUT(tsHL | tsHH | IDLH)                                \
	TRICE_PUT((TRICE_CYCLE << 24) | (40 << 16) | tsLL | tsLH)    \
	TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint32_t ts = TriceStamp32;                                       \
	TRICE_PUT(tsHL | tsHH | IDLH)                                     \
	TRICE_PUT((TRICE_CYCLE << 24) | (44 << 16) | tsLL | tsLH)         \
	TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint32_t ts = TriceStamp32;                                            \
	TRICE_PUT(tsHL | tsHH | IDLH)                                          \
	TRICE_PUT((TRICE_CYCLE << 24) | (48 << 16) | tsLL | tsLH)              \
	TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE
