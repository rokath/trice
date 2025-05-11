/*! \file trice8McuReverse.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#define trice8m_0(tid)                       \
	TRICE_ENTER                              \
	TRICE_PUT(((TRICE_CYCLE) << 24) | idLH); \
	TRICE_LEAVE

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

#define Trice8m_0(tid)                          \
	TRICE_ENTER                                 \
	uint16_t ts_Trice8m_0 = TriceStamp16;                 \
	TRICE_PUT(((IdLH) << 16) | IdLH)            \
	TRICE_PUT((TRICE_CYCLE << 24) | TRICE16_LO_BYTE(ts_Trice8m_0)|TRICE16_HI_BYTE(ts_Trice8m_0)); \
	TRICE_LEAVE

#define Trice8m_1(tid, v0)                                  \
	TRICE_ENTER                                             \
	uint16_t ts_Trice8m_1 = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (1 << 16) | TRICE16_LO_BYTE(ts_Trice8m_1)|TRICE16_HI_BYTE(ts_Trice8m_1)); \
	TRICE_PUT8_1(v0)                                        \
	TRICE_LEAVE

#define Trice8m_2(tid, v0, v1)                              \
	TRICE_ENTER                                             \
	uint16_t ts_Trice8m_2 = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (2 << 16) | TRICE16_LO_BYTE(ts_Trice8m_2)|TRICE16_HI_BYTE(ts_Trice8m_2)); \
	TRICE_PUT8_2(v0, v1);                                   \
	TRICE_LEAVE

#define Trice8m_3(tid, v0, v1, v2)                          \
	TRICE_ENTER                                             \
	uint16_t ts_Trice8m_3 = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (3 << 16) | TRICE16_LO_BYTE(ts_Trice8m_3)|TRICE16_HI_BYTE(ts_Trice8m_3)); \
	TRICE_PUT8_3(v0, v1, v2);                               \
	TRICE_LEAVE

#define Trice8m_4(tid, v0, v1, v2, v3)                      \
	TRICE_ENTER                                             \
	uint16_t ts_Trice8m_4 = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (4 << 16) | TRICE16_LO_BYTE(ts_Trice8m_4)|TRICE16_HI_BYTE(ts_Trice8m_4)); \
	TRICE_PUT8_4(v0, v1, v2, v3);                           \
	TRICE_LEAVE

#define Trice8m_5(tid, v0, v1, v2, v3, v4)                  \
	TRICE_ENTER                                             \
	uint16_t ts_Trice8m_5 = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (5 << 16) | TRICE16_LO_BYTE(ts_Trice8m_5)|TRICE16_HI_BYTE(ts_Trice8m_5)); \
	TRICE_PUT8_5(v0, v1, v2, v3, v4);                       \
	TRICE_LEAVE

#define Trice8m_6(tid, v0, v1, v2, v3, v4, v5)              \
	TRICE_ENTER                                             \
	uint16_t ts_Trice8m_6 = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (6 << 16) | TRICE16_LO_BYTE(ts_Trice8m_6)|TRICE16_HI_BYTE(ts_Trice8m_6)); \
	TRICE_PUT8_6(v0, v1, v2, v3, v4, v5);                   \
	TRICE_LEAVE

#define Trice8m_7(tid, v0, v1, v2, v3, v4, v5, v6)          \
	TRICE_ENTER                                             \
	uint16_t ts_Trice8m_7 = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (7 << 16) | TRICE16_LO_BYTE(ts_Trice8m_7)|TRICE16_HI_BYTE(ts_Trice8m_7)); \
	TRICE_PUT8_7(v0, v1, v2, v3, v4, v5, v6);               \
	TRICE_LEAVE

#define Trice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)      \
	TRICE_ENTER                                             \
	uint16_t ts_Trice8m_8 = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | TRICE16_LO_BYTE(ts_Trice8m_8)|TRICE16_HI_BYTE(ts_Trice8m_8)); \
	TRICE_PUT8_8(v0, v1, v2, v3, v4, v5, v6, v7);           \
	TRICE_LEAVE

#define Trice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)  \
	TRICE_ENTER                                             \
	uint16_t ts_Trice8m_9 = TriceStamp16;                             \
	TRICE_PUT(((IdLH) << 16) | IdLH)                        \
	TRICE_PUT((TRICE_CYCLE << 24) | (9 << 16) | TRICE16_LO_BYTE(ts_Trice8m_9)|TRICE16_HI_BYTE(ts_Trice8m_9)); \
	TRICE_PUT8_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);       \
	TRICE_LEAVE

#define Trice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                 \
	uint16_t ts_Trice8m_10 = TriceStamp16;                                 \
	TRICE_PUT(((IdLH) << 16) | IdLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (10 << 16) | TRICE16_LO_BYTE(ts_Trice8m_10)|TRICE16_HI_BYTE(ts_Trice8m_10));    \
	TRICE_PUT8_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                      \
	uint16_t ts_Trice8m_11 = TriceStamp16;                                      \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                 \
	TRICE_PUT((TRICE_CYCLE << 24) | (11 << 16) | TRICE16_LO_BYTE(ts_Trice8m_11)|TRICE16_HI_BYTE(ts_Trice8m_11));         \
	TRICE_PUT8_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                           \
	uint16_t ts_Trice8m_12 = TriceStamp16;                                           \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                      \
	TRICE_PUT((TRICE_CYCLE << 24) | (12 << 16) | TRICE16_LO_BYTE(ts_Trice8m_12)|TRICE16_HI_BYTE(ts_Trice8m_12));              \
	TRICE_PUT8_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#define TRice8m_0(tid)                           \
	TRICE_ENTER                                  \
	uint32_t ts_TRice8m_0 = TriceStamp32;                  \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_0) | TRICE32_HH_BYTE(ts_TRice8m_0)| IDLH)                \
	TRICE_PUT((TRICE_CYCLE << 24) | TRICE32_LL_BYTE(ts_TRice8m_0) | TRICE32_LH_BYTE(ts_TRice8m_0)) \
	TRICE_LEAVE

#define TRice8m_1(tid, v0)                                   \
	TRICE_ENTER                                              \
	uint32_t ts_TRice8m_1 = TriceStamp32;                              \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_1) | TRICE32_HH_BYTE(ts_TRice8m_1)| IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (1 << 16) | TRICE32_LL_BYTE(ts_TRice8m_1) | TRICE32_LH_BYTE(ts_TRice8m_1)) \
	TRICE_PUT8_1(v0)                                         \
	TRICE_LEAVE

#define TRice8m_2(tid, v0, v1)                               \
	TRICE_ENTER                                              \
	uint32_t ts_TRice8m_2 = TriceStamp32;                              \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_2) | TRICE32_HH_BYTE(ts_TRice8m_2)| IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (2 << 16) | TRICE32_LL_BYTE(ts_TRice8m_2) | TRICE32_LH_BYTE(ts_TRice8m_2)) \
	TRICE_PUT8_2(v0, v1);                                    \
	TRICE_LEAVE

#define TRice8m_3(tid, v0, v1, v2)                           \
	TRICE_ENTER                                              \
	uint32_t ts_TRice8m_3 = TriceStamp32;                              \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_3) | TRICE32_HH_BYTE(ts_TRice8m_3)| IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (3 << 16) | TRICE32_LL_BYTE(ts_TRice8m_3) | TRICE32_LH_BYTE(ts_TRice8m_3)) \
	TRICE_PUT8_3(v0, v1, v2);                                \
	TRICE_LEAVE

#define TRice8m_4(tid, v0, v1, v2, v3)                       \
	TRICE_ENTER                                              \
	uint32_t ts_TRice8m_4 = TriceStamp32;                              \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_4) | TRICE32_HH_BYTE(ts_TRice8m_4)| IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (4 << 16) | TRICE32_LL_BYTE(ts_TRice8m_4) | TRICE32_LH_BYTE(ts_TRice8m_4)) \
	TRICE_PUT8_4(v0, v1, v2, v3);                            \
	TRICE_LEAVE

#define TRice8m_5(tid, v0, v1, v2, v3, v4)                   \
	TRICE_ENTER                                              \
	uint32_t ts_TRice8m_5 = TriceStamp32;                              \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_5) | TRICE32_HH_BYTE(ts_TRice8m_5)| IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (5 << 16) | TRICE32_LL_BYTE(ts_TRice8m_5) | TRICE32_LH_BYTE(ts_TRice8m_5)) \
	TRICE_PUT8_5(v0, v1, v2, v3, v4);                        \
	TRICE_LEAVE

#define TRice8m_6(tid, v0, v1, v2, v3, v4, v5)               \
	TRICE_ENTER                                              \
	uint32_t ts_TRice8m_6 = TriceStamp32;                              \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_6) | TRICE32_HH_BYTE(ts_TRice8m_6)| IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (6 << 16) | TRICE32_LL_BYTE(ts_TRice8m_6) | TRICE32_LH_BYTE(ts_TRice8m_6)) \
	TRICE_PUT8_6(v0, v1, v2, v3, v4, v5);                    \
	TRICE_LEAVE

#define TRice8m_7(tid, v0, v1, v2, v3, v4, v5, v6)           \
	TRICE_ENTER                                              \
	uint32_t ts_TRice8m_7 = TriceStamp32;                              \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_7) | TRICE32_HH_BYTE(ts_TRice8m_7)| IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (7 << 16) | TRICE32_LL_BYTE(ts_TRice8m_7) | TRICE32_LH_BYTE(ts_TRice8m_7)) \
	TRICE_PUT8_7(v0, v1, v2, v3, v4, v5, v6);                \
	TRICE_LEAVE

#define TRice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)       \
	TRICE_ENTER                                              \
	uint32_t ts_TRice8m_8 = TriceStamp32;                              \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_8) | TRICE32_HH_BYTE(ts_TRice8m_8)| IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | TRICE32_LL_BYTE(ts_TRice8m_8) | TRICE32_LH_BYTE(ts_TRice8m_8)) \
	TRICE_PUT8_8(v0, v1, v2, v3, v4, v5, v6, v7);            \
	TRICE_LEAVE

#define TRice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)   \
	TRICE_ENTER                                              \
	uint32_t ts_TRice8m_9 = TriceStamp32;                              \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_9) | TRICE32_HH_BYTE(ts_TRice8m_9)| IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (9 << 16) | TRICE32_LL_BYTE(ts_TRice8m_9) | TRICE32_LH_BYTE(ts_TRice8m_9)) \
	TRICE_PUT8_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);        \
	TRICE_LEAVE

#define TRice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                 \
	uint32_t ts_TRice8m_10 = TriceStamp32;                                 \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_10) | TRICE32_HH_BYTE(ts_TRice8m_10)| IDLH)                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (10 << 16) | TRICE32_LL_BYTE(ts_TRice8m_10) | TRICE32_LH_BYTE(ts_TRice8m_10))   \
	TRICE_PUT8_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                      \
	uint32_t ts_TRice8m_11 = TriceStamp32;                                      \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_11) | TRICE32_HH_BYTE(ts_TRice8m_11)| IDLH)                                    \
	TRICE_PUT((TRICE_CYCLE << 24) | (11 << 16) | TRICE32_LL_BYTE(ts_TRice8m_11) | TRICE32_LH_BYTE(ts_TRice8m_11))        \
	TRICE_PUT8_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                           \
	uint32_t ts_TRice8m_12 = TriceStamp32;                                           \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice8m_12) | TRICE32_HH_BYTE(ts_TRice8m_12)| IDLH)                                         \
	TRICE_PUT((TRICE_CYCLE << 24) | (12 << 16) | TRICE32_LL_BYTE(ts_TRice8m_12) | TRICE32_LH_BYTE(ts_TRice8m_12))             \
	TRICE_PUT8_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE
