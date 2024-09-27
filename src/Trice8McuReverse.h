/*! \file Trice8McuReverse.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

//! trice8m_0 writes trice data as fast as possible in a buffer.
//! This macro is used internally and not intended for user applications.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define trice8m_0(tid)                       \
	TRICE_ENTER                              \
	TRICE_PUT(((TRICE_CYCLE) << 24) | idLH); \
	TRICE_LEAVE

//! trice8m_1 writes trice data as fast as possible in a buffer.
//! This macro is used internally and not intended for user applications.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define trice8m_1(tid, v0)                               \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (1 << 16) | idLH); \
	TRICE_PUT8_1(v0)                                     \
	TRICE_LEAVE

#define trice8m_2(tid, v0, v1)                           \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (2 << 16) | idLH); \
	TRICE_PUT8_2(v0, v1);                                \
	TRICE_LEAVE

#define trice8m_3(tid, v0, v1, v2)                       \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (3 << 16) | idLH); \
	TRICE_PUT8_3(v0, v1, v2);                            \
	TRICE_LEAVE

#define trice8m_4(tid, v0, v1, v2, v3)                   \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (4 << 16) | idLH); \
	TRICE_PUT8_4(v0, v1, v2, v3);                        \
	TRICE_LEAVE

#define trice8m_5(tid, v0, v1, v2, v3, v4)               \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (5 << 16) | idLH); \
	TRICE_PUT8_5(v0, v1, v2, v3, v4);                    \
	TRICE_LEAVE

#define trice8m_6(tid, v0, v1, v2, v3, v4, v5)           \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (6 << 16) | idLH); \
	TRICE_PUT8_6(v0, v1, v2, v3, v4, v5);                \
	TRICE_LEAVE

#define trice8m_7(tid, v0, v1, v2, v3, v4, v5, v6)       \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (7 << 16) | idLH); \
	TRICE_PUT8_7(v0, v1, v2, v3, v4, v5, v6);            \
	TRICE_LEAVE

#define trice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)   \
	TRICE_ENTER                                          \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (8 << 16) | idLH); \
	TRICE_PUT8_8(v0, v1, v2, v3, v4, v5, v6, v7);        \
	TRICE_LEAVE

#define trice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                            \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (9 << 16) | idLH);   \
	TRICE_PUT8_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define trice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                 \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (10 << 16) | idLH);       \
	TRICE_PUT8_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define trice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                      \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (11 << 16) | idLH);            \
	TRICE_PUT8_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define trice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                           \
	TRICE_PUT(((TRICE_CYCLE) << 24) | (12 << 16) | idLH);                 \
	TRICE_PUT8_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

//! Trice8m_0 writes trice data as fast as possible in a buffer.
//! This macro is used internally and not intended for user applications.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define Trice8m_0(tid)                          \
	TRICE_ENTER                                 \
	uint16_t ts = TriceStamp16;                 \
	TRICE_PUT(((IdLH) << 16) | IdLH)            \
	TRICE_PUT((TRICE_CYCLE << 24) | tsL | tsH); \
	TRICE_LEAVE

//! Trice8m_1 writes trice data as fast as possible in a buffer.
//! This macro is used internally and not intended for user applications.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define Trice8m_1(tid, v0)                                  \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (1 << 16) | tsL | tsH); \
	TRICE_PUT8_1(v0)                                        \
	TRICE_LEAVE

#define Trice8m_2(tid, v0, v1)                              \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (2 << 16) | tsL | tsH); \
	TRICE_PUT8_2(v0, v1);                                   \
	TRICE_LEAVE

#define Trice8m_3(tid, v0, v1, v2)                          \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (3 << 16) | tsL | tsH); \
	TRICE_PUT8_3(v0, v1, v2);                               \
	TRICE_LEAVE

#define Trice8m_4(tid, v0, v1, v2, v3)                      \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (4 << 16) | tsL | tsH); \
	TRICE_PUT8_4(v0, v1, v2, v3);                           \
	TRICE_LEAVE

#define Trice8m_5(tid, v0, v1, v2, v3, v4)                  \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (5 << 16) | tsL | tsH); \
	TRICE_PUT8_5(v0, v1, v2, v3, v4);                       \
	TRICE_LEAVE

#define Trice8m_6(tid, v0, v1, v2, v3, v4, v5)              \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (6 << 16) | tsL | tsH); \
	TRICE_PUT8_6(v0, v1, v2, v3, v4, v5);                   \
	TRICE_LEAVE

#define Trice8m_7(tid, v0, v1, v2, v3, v4, v5, v6)          \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (7 << 16) | tsL | tsH); \
	TRICE_PUT8_7(v0, v1, v2, v3, v4, v5, v6);               \
	TRICE_LEAVE

#define Trice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)      \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | tsL | tsH); \
	TRICE_PUT8_8(v0, v1, v2, v3, v4, v5, v6, v7);           \
	TRICE_LEAVE

#define Trice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)  \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (9 << 16) | tsL | tsH); \
	TRICE_PUT8_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);       \
	TRICE_LEAVE

#define Trice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                 \
	uint16_t ts = TriceStamp16;                                 \
	TRICE_PUT(((IdLH) << 16) | IdLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (10 << 16) | tsL | tsH);    \
	TRICE_PUT8_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                      \
	uint16_t ts = TriceStamp16;                                      \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                 \
	TRICE_PUT((TRICE_CYCLE << 24) | (11 << 16) | tsL | tsH);         \
	TRICE_PUT8_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                           \
	uint16_t ts = TriceStamp16;                                           \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                      \
	TRICE_PUT((TRICE_CYCLE << 24) | (12 << 16) | tsL | tsH);              \
	TRICE_PUT8_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

//! TRice8m_0 writes trice data as fast as possible in a buffer.
//! \param tid is a 14 bit Trice id in upper 2 bytes of a 32 bit value
#define TRice8m_0(tid)                           \
	TRICE_ENTER                                  \
	uint32_t ts = TriceStamp32;                  \
	TRICE_PUT(tsHL | tsHH | IDLH)                \
	TRICE_PUT((TRICE_CYCLE << 24) | tsLL | tsLH) \
	TRICE_LEAVE

//! TRice8m_1 writes a Trice header with 32-bit stamp and 8-bit trice data as fast as possible in a buffer.
//! \param tid is a 14 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define TRice8m_1(tid, v0)                                   \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (1 << 16) | tsLL | tsLH) \
	TRICE_PUT8_1(v0)                                         \
	TRICE_LEAVE

#define TRice8m_2(tid, v0, v1)                               \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (2 << 16) | tsLL | tsLH) \
	TRICE_PUT8_2(v0, v1);                                    \
	TRICE_LEAVE

#define TRice8m_3(tid, v0, v1, v2)                           \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (3 << 16) | tsLL | tsLH) \
	TRICE_PUT8_3(v0, v1, v2);                                \
	TRICE_LEAVE

#define TRice8m_4(tid, v0, v1, v2, v3)                       \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (4 << 16) | tsLL | tsLH) \
	TRICE_PUT8_4(v0, v1, v2, v3);                            \
	TRICE_LEAVE

#define TRice8m_5(tid, v0, v1, v2, v3, v4)                   \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (5 << 16) | tsLL | tsLH) \
	TRICE_PUT8_5(v0, v1, v2, v3, v4);                        \
	TRICE_LEAVE

#define TRice8m_6(tid, v0, v1, v2, v3, v4, v5)               \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (6 << 16) | tsLL | tsLH) \
	TRICE_PUT8_6(v0, v1, v2, v3, v4, v5);                    \
	TRICE_LEAVE

#define TRice8m_7(tid, v0, v1, v2, v3, v4, v5, v6)           \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (7 << 16) | tsLL | tsLH) \
	TRICE_PUT8_7(v0, v1, v2, v3, v4, v5, v6);                \
	TRICE_LEAVE

#define TRice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)       \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | tsLL | tsLH) \
	TRICE_PUT8_8(v0, v1, v2, v3, v4, v5, v6, v7);            \
	TRICE_LEAVE

#define TRice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)   \
	TRICE_ENTER                                              \
	uint32_t ts = TriceStamp32;                              \
	TRICE_PUT(tsHL | tsHH | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (9 << 16) | tsLL | tsLH) \
	TRICE_PUT8_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);        \
	TRICE_LEAVE

#define TRice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                 \
	uint32_t ts = TriceStamp32;                                 \
	TRICE_PUT(tsHL | tsHH | IDLH)                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (10 << 16) | tsLL | tsLH)   \
	TRICE_PUT8_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                      \
	uint32_t ts = TriceStamp32;                                      \
	TRICE_PUT(tsHL | tsHH | IDLH)                                    \
	TRICE_PUT((TRICE_CYCLE << 24) | (11 << 16) | tsLL | tsLH)        \
	TRICE_PUT8_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                           \
	uint32_t ts = TriceStamp32;                                           \
	TRICE_PUT(tsHL | tsHH | IDLH)                                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (12 << 16) | tsLL | tsLH)             \
	TRICE_PUT8_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE
