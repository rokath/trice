/*! \file trice32McuReverse.h
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

#define Trice32m_0(tid)                                                                               \
	TRICE_ENTER                                                                                       \
	uint16_t ts_Trice32m_0 = TriceStamp16;                                                            \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                  \
	TRICE_PUT((TRICE_CYCLE << 24) | TRICE16_LO_BYTE(ts_Trice32m_0) | TRICE16_HI_BYTE(ts_Trice32m_0)); \
	TRICE_LEAVE

#define Trice32m_1(tid, v0)                                                                                       \
	TRICE_ENTER                                                                                                   \
	uint16_t ts_Trice32m_1 = TriceStamp16;                                                                        \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                              \
	TRICE_PUT((TRICE_CYCLE << 24) | (4 << 16) | TRICE16_LO_BYTE(ts_Trice32m_1) | TRICE16_HI_BYTE(ts_Trice32m_1)); \
	TRICE_PUT32_1(v0)                                                                                             \
	TRICE_LEAVE

#define Trice32m_2(tid, v0, v1)                                                                                   \
	TRICE_ENTER                                                                                                   \
	uint16_t ts_Trice32m_2 = TriceStamp16;                                                                        \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                              \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | TRICE16_LO_BYTE(ts_Trice32m_2) | TRICE16_HI_BYTE(ts_Trice32m_2)); \
	TRICE_PUT32_2(v0, v1);                                                                                        \
	TRICE_LEAVE

#define Trice32m_3(tid, v0, v1, v2)                                                                                \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice32m_3 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (12 << 16) | TRICE16_LO_BYTE(ts_Trice32m_3) | TRICE16_HI_BYTE(ts_Trice32m_3)); \
	TRICE_PUT32_3(v0, v1, v2);                                                                                     \
	TRICE_LEAVE

#define Trice32m_4(tid, v0, v1, v2, v3)                                                                            \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice32m_4 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (16 << 16) | TRICE16_LO_BYTE(ts_Trice32m_4) | TRICE16_HI_BYTE(ts_Trice32m_4)); \
	TRICE_PUT32_4(v0, v1, v2, v3);                                                                                 \
	TRICE_LEAVE

#define Trice32m_5(tid, v0, v1, v2, v3, v4)                                                                        \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice32m_5 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (20 << 16) | TRICE16_LO_BYTE(ts_Trice32m_5) | TRICE16_HI_BYTE(ts_Trice32m_5)); \
	TRICE_PUT32_5(v0, v1, v2, v3, v4);                                                                             \
	TRICE_LEAVE

#define Trice32m_6(tid, v0, v1, v2, v3, v4, v5)                                                                    \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice32m_6 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (24 << 16) | TRICE16_LO_BYTE(ts_Trice32m_6) | TRICE16_HI_BYTE(ts_Trice32m_6)); \
	TRICE_PUT32_6(v0, v1, v2, v3, v4, v5);                                                                         \
	TRICE_LEAVE

#define Trice32m_7(tid, v0, v1, v2, v3, v4, v5, v6)                                                                \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice32m_7 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (28 << 16) | TRICE16_LO_BYTE(ts_Trice32m_7) | TRICE16_HI_BYTE(ts_Trice32m_7)); \
	TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6);                                                                     \
	TRICE_LEAVE

#define Trice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)                                                            \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice32m_8 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (32 << 16) | TRICE16_LO_BYTE(ts_Trice32m_8) | TRICE16_HI_BYTE(ts_Trice32m_8)); \
	TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7);                                                                 \
	TRICE_LEAVE

#define Trice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)                                                        \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice32m_9 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (36 << 16) | TRICE16_LO_BYTE(ts_Trice32m_9) | TRICE16_HI_BYTE(ts_Trice32m_9)); \
	TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                                                             \
	TRICE_LEAVE

#define Trice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)                                                     \
	TRICE_ENTER                                                                                                      \
	uint16_t ts_Trice32m_10 = TriceStamp16;                                                                          \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                                 \
	TRICE_PUT((TRICE_CYCLE << 24) | (40 << 16) | TRICE16_LO_BYTE(ts_Trice32m_10) | TRICE16_HI_BYTE(ts_Trice32m_10)); \
	TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);                                                          \
	TRICE_LEAVE

#define Trice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)                                                \
	TRICE_ENTER                                                                                                      \
	uint16_t ts_Trice32m_11 = TriceStamp16;                                                                          \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                                 \
	TRICE_PUT((TRICE_CYCLE << 24) | (44 << 16) | TRICE16_LO_BYTE(ts_Trice32m_11) | TRICE16_HI_BYTE(ts_Trice32m_11)); \
	TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);                                                     \
	TRICE_LEAVE

#define Trice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                                           \
	TRICE_ENTER                                                                                                      \
	uint16_t ts_Trice32m_12 = TriceStamp16;                                                                          \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                                 \
	TRICE_PUT((TRICE_CYCLE << 24) | (48 << 16) | TRICE16_LO_BYTE(ts_Trice32m_12) | TRICE16_HI_BYTE(ts_Trice32m_12)); \
	TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                                                 \
	TRICE_LEAVE

#define TRice32m_0(tid)                                                                              \
	TRICE_ENTER                                                                                      \
	uint32_t ts_TRice32m_0 = TriceStamp32;                                                           \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_0) | TRICE32_HH_BYTE(ts_TRice32m_0) | IDLH)                \
	TRICE_PUT((TRICE_CYCLE << 24) | TRICE32_LL_BYTE(ts_TRice32m_0) | TRICE32_LH_BYTE(ts_TRice32m_0)) \
	TRICE_LEAVE

#define TRice32m_1(tid, v0)                                                                                      \
	TRICE_ENTER                                                                                                  \
	uint32_t ts_TRice32m_1 = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_1) | TRICE32_HH_BYTE(ts_TRice32m_1) | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (4 << 16) | TRICE32_LL_BYTE(ts_TRice32m_1) | TRICE32_LH_BYTE(ts_TRice32m_1)) \
	TRICE_PUT32_1(v0)                                                                                            \
	TRICE_LEAVE

#define TRice32m_2(tid, v0, v1)                                                                                  \
	TRICE_ENTER                                                                                                  \
	uint32_t ts_TRice32m_2 = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_2) | TRICE32_HH_BYTE(ts_TRice32m_2) | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | TRICE32_LL_BYTE(ts_TRice32m_2) | TRICE32_LH_BYTE(ts_TRice32m_2)) \
	TRICE_PUT32_2(v0, v1);                                                                                       \
	TRICE_LEAVE

#define TRice32m_3(tid, v0, v1, v2)                                                                               \
	TRICE_ENTER                                                                                                   \
	uint32_t ts_TRice32m_3 = TriceStamp32;                                                                        \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_3) | TRICE32_HH_BYTE(ts_TRice32m_3) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (12 << 16) | TRICE32_LL_BYTE(ts_TRice32m_3) | TRICE32_LH_BYTE(ts_TRice32m_3)) \
	TRICE_PUT32_3(v0, v1, v2);                                                                                    \
	TRICE_LEAVE

#define TRice32m_4(tid, v0, v1, v2, v3)                                                                           \
	TRICE_ENTER                                                                                                   \
	uint32_t ts_TRice32m_4 = TriceStamp32;                                                                        \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_4) | TRICE32_HH_BYTE(ts_TRice32m_4) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (16 << 16) | TRICE32_LL_BYTE(ts_TRice32m_4) | TRICE32_LH_BYTE(ts_TRice32m_4)) \
	TRICE_PUT32_4(v0, v1, v2, v3);                                                                                \
	TRICE_LEAVE

#define TRice32m_5(tid, v0, v1, v2, v3, v4)                                                                       \
	TRICE_ENTER                                                                                                   \
	uint32_t ts_TRice32m_5 = TriceStamp32;                                                                        \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_5) | TRICE32_HH_BYTE(ts_TRice32m_5) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (20 << 16) | TRICE32_LL_BYTE(ts_TRice32m_5) | TRICE32_LH_BYTE(ts_TRice32m_5)) \
	TRICE_PUT32_5(v0, v1, v2, v3, v4);                                                                            \
	TRICE_LEAVE

#define TRice32m_6(tid, v0, v1, v2, v3, v4, v5)                                                                   \
	TRICE_ENTER                                                                                                   \
	uint32_t ts_TRice32m_6 = TriceStamp32;                                                                        \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_6) | TRICE32_HH_BYTE(ts_TRice32m_6) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (24 << 16) | TRICE32_LL_BYTE(ts_TRice32m_6) | TRICE32_LH_BYTE(ts_TRice32m_6)) \
	TRICE_PUT32_6(v0, v1, v2, v3, v4, v5);                                                                        \
	TRICE_LEAVE

#define TRice32m_7(tid, v0, v1, v2, v3, v4, v5, v6)                                                               \
	TRICE_ENTER                                                                                                   \
	uint32_t ts_TRice32m_7 = TriceStamp32;                                                                        \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_7) | TRICE32_HH_BYTE(ts_TRice32m_7) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (28 << 16) | TRICE32_LL_BYTE(ts_TRice32m_7) | TRICE32_LH_BYTE(ts_TRice32m_7)) \
	TRICE_PUT32_7(v0, v1, v2, v3, v4, v5, v6);                                                                    \
	TRICE_LEAVE

#define TRice32m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)                                                           \
	TRICE_ENTER                                                                                                   \
	uint32_t ts_TRice32m_8 = TriceStamp32;                                                                        \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_8) | TRICE32_HH_BYTE(ts_TRice32m_8) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (32 << 16) | TRICE32_LL_BYTE(ts_TRice32m_8) | TRICE32_LH_BYTE(ts_TRice32m_8)) \
	TRICE_PUT32_8(v0, v1, v2, v3, v4, v5, v6, v7);                                                                \
	TRICE_LEAVE

#define TRice32m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)                                                       \
	TRICE_ENTER                                                                                                   \
	uint32_t ts_TRice32m_9 = TriceStamp32;                                                                        \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_9) | TRICE32_HH_BYTE(ts_TRice32m_9) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (36 << 16) | TRICE32_LL_BYTE(ts_TRice32m_9) | TRICE32_LH_BYTE(ts_TRice32m_9)) \
	TRICE_PUT32_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                                                            \
	TRICE_LEAVE

#define TRice32m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)                                                    \
	TRICE_ENTER                                                                                                     \
	uint32_t ts_TRice32m_10 = TriceStamp32;                                                                         \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_10) | TRICE32_HH_BYTE(ts_TRice32m_10) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (40 << 16) | TRICE32_LL_BYTE(ts_TRice32m_10) | TRICE32_LH_BYTE(ts_TRice32m_10)) \
	TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);                                                         \
	TRICE_LEAVE

#define TRice32m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)                                               \
	TRICE_ENTER                                                                                                     \
	uint32_t ts_TRice32m_11 = TriceStamp32;                                                                         \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_11) | TRICE32_HH_BYTE(ts_TRice32m_11) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (44 << 16) | TRICE32_LL_BYTE(ts_TRice32m_11) | TRICE32_LH_BYTE(ts_TRice32m_11)) \
	TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);                                                    \
	TRICE_LEAVE

#define TRice32m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                                          \
	TRICE_ENTER                                                                                                     \
	uint32_t ts_TRice32m_12 = TriceStamp32;                                                                         \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice32m_12) | TRICE32_HH_BYTE(ts_TRice32m_12) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (48 << 16) | TRICE32_LL_BYTE(ts_TRice32m_12) | TRICE32_LH_BYTE(ts_TRice32m_12)) \
	TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                                                \
	TRICE_LEAVE
