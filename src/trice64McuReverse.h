/*! \file trice64McuReverse.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#define trice64m_0(tid)                      \
	TRICE_ENTER                              \
	TRICE_PUT(((TRICE_CYCLE) << 24) | idLH); \
	TRICE_LEAVE

#define trice64m_1(tid, v0)                              \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (8 << 16) | idLH); \
	TRICE_PUT64_1(v0)                                    \
	TRICE_LEAVE

#define trice64m_2(tid, v0, v1)                           \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (16 << 16) | idLH); \
	TRICE_PUT64_2(v0, v1);                                \
	TRICE_LEAVE

#define trice64m_3(tid, v0, v1, v2)                       \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (24 << 16) | idLH); \
	TRICE_PUT64_3(v0, v1, v2);                            \
	TRICE_LEAVE

#define trice64m_4(tid, v0, v1, v2, v3)                   \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (32 << 16) | idLH); \
	TRICE_PUT64_4(v0, v1, v2, v3);                        \
	TRICE_LEAVE

#define trice64m_5(tid, v0, v1, v2, v3, v4)               \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (40 << 16) | idLH); \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                    \
	TRICE_LEAVE

#define trice64m_6(tid, v0, v1, v2, v3, v4, v5)           \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (48 << 16) | idLH); \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                \
	TRICE_LEAVE

#define trice64m_7(tid, v0, v1, v2, v3, v4, v5, v6)       \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (56 << 16) | idLH); \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);            \
	TRICE_LEAVE

#define trice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)   \
	TRICE_ENTER                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (64 << 16) | idLH); \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);        \
	TRICE_LEAVE

#define trice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (72 << 16) | idLH);   \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define trice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (80 << 16) | idLH);        \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define trice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (88 << 16) | idLH);             \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define trice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (96 << 16) | idLH);                  \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#define Trice64m_0(tid)                         \
	TRICE_ENTER                                 \
	uint16_t ts = TriceStamp16;                 \
	TRICE_PUT(((IdLH) << 16) | IdLH)            \
	TRICE_PUT((TRICE_CYCLE << 24) | tsL | tsH); \
	TRICE_LEAVE

#define Trice64m_1(tid, v0)                                 \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | tsL | tsH); \
	TRICE_PUT64_1(v0)                                       \
	TRICE_LEAVE

#define Trice64m_2(tid, v0, v1)                              \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (16 << 16) | tsL | tsH); \
	TRICE_PUT64_2(v0, v1);                                   \
	TRICE_LEAVE

#define Trice64m_3(tid, v0, v1, v2)                          \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (24 << 16) | tsL | tsH); \
	TRICE_PUT64_3(v0, v1, v2);                               \
	TRICE_LEAVE

#define Trice64m_4(tid, v0, v1, v2, v3)                      \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (32 << 16) | tsL | tsH); \
	TRICE_PUT64_4(v0, v1, v2, v3);                           \
	TRICE_LEAVE

#define Trice64m_5(tid, v0, v1, v2, v3, v4)                  \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (40 << 16) | tsL | tsH); \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                       \
	TRICE_LEAVE

#define Trice64m_6(tid, v0, v1, v2, v3, v4, v5)              \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (48 << 16) | tsL | tsH); \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                   \
	TRICE_LEAVE

#define Trice64m_7(tid, v0, v1, v2, v3, v4, v5, v6)          \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (56 << 16) | tsL | tsH); \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);               \
	TRICE_LEAVE

#define Trice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)      \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (64 << 16) | tsL | tsH); \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);           \
	TRICE_LEAVE

#define Trice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)  \
	TRICE_ENTER                                              \
	uint16_t ts = TriceStamp16;                              \
	TRICE_PUT(((IdLH) << 16) | IdLH)                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (72 << 16) | tsL | tsH); \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);       \
	TRICE_LEAVE

#define Trice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint16_t ts = TriceStamp16;                                  \
	TRICE_PUT(((IdLH) << 16) | IdLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (80 << 16) | tsL | tsH);     \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint16_t ts = TriceStamp16;                                       \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                  \
	TRICE_PUT((TRICE_CYCLE << 24) | (88 << 16) | tsL | tsH);          \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint16_t ts = TriceStamp16;                                            \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                       \
	TRICE_PUT((TRICE_CYCLE << 24) | (96 << 16) | tsL | tsH);               \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#define TRice64m_0(tid)                                      \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH);                           \
	TRICE_PUT((TRICE_CYCLE << 24) | (0 << 16) | tsLL | tsLH) \
	TRICE_LEAVE

#define TRice64m_1(tid, v0)                                  \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | tsLL | tsLH) \
	TRICE_PUT64_1(v0)                                        \
	TRICE_LEAVE

#define TRice64m_2(tid, v0, v1)                               \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (16 << 16) | tsLL | tsLH) \
	TRICE_PUT64_2(v0, v1);                                    \
	TRICE_LEAVE

#define TRice64m_3(tid, v0, v1, v2)                           \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (24 << 16) | tsLL | tsLH) \
	TRICE_PUT64_3(v0, v1, v2);                                \
	TRICE_LEAVE

#define TRice64m_4(tid, v0, v1, v2, v3)                       \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (32 << 16) | tsLL | tsLH) \
	TRICE_PUT64_4(v0, v1, v2, v3);                            \
	TRICE_LEAVE

#define TRice64m_5(tid, v0, v1, v2, v3, v4)                   \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (40 << 16) | tsLL | tsLH) \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                        \
	TRICE_LEAVE

#define TRice64m_6(tid, v0, v1, v2, v3, v4, v5)               \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (48 << 16) | tsLL | tsLH) \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                    \
	TRICE_LEAVE

#define TRice64m_7(tid, v0, v1, v2, v3, v4, v5, v6)           \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (56 << 16) | tsLL | tsLH) \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);                \
	TRICE_LEAVE

#define TRice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)       \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (64 << 16) | tsLL | tsLH) \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);            \
	TRICE_LEAVE

#define TRice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)   \
	TRICE_ENTER                                               \
	uint32_t ts = TriceStamp32;                               \
	TRICE_PUT(tsHL | tsHH | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (72 << 16) | tsLL | tsLH) \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);        \
	TRICE_LEAVE

#define TRice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint32_t ts = TriceStamp32;                                  \
	TRICE_PUT(tsHL | tsHH | IDLH)                                \
	TRICE_PUT((TRICE_CYCLE << 24) | (80 << 16) | tsLL | tsLH)    \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint32_t ts = TriceStamp32;                                       \
	TRICE_PUT(tsHL | tsHH | IDLH)                                     \
	TRICE_PUT((TRICE_CYCLE << 24) | (88 << 16) | tsLL | tsLH)         \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint32_t ts = TriceStamp32;                                            \
	TRICE_PUT(tsHL | tsHH | IDLH)                                          \
	TRICE_PUT((TRICE_CYCLE << 24) | (96 << 16) | tsLL | tsLH)              \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE
