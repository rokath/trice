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

#define Trice64m_0(tid)                                                                               \
	TRICE_ENTER                                                                                       \
	uint16_t ts_Trice64m_0 = TriceStamp16;                                                            \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                  \
	TRICE_PUT((TRICE_CYCLE << 24) | TRICE16_LO_BYTE(ts_Trice64m_0) | TRICE16_HI_BYTE(ts_Trice64m_0)); \
	TRICE_LEAVE

#define Trice64m_1(tid, v0)                                                                                       \
	TRICE_ENTER                                                                                                   \
	uint16_t ts_Trice64m_1 = TriceStamp16;                                                                        \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                              \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | TRICE16_LO_BYTE(ts_Trice64m_1) | TRICE16_HI_BYTE(ts_Trice64m_1)); \
	TRICE_PUT64_1(v0)                                                                                             \
	TRICE_LEAVE

#define Trice64m_2(tid, v0, v1)                                                                                    \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice64m_2 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (16 << 16) | TRICE16_LO_BYTE(ts_Trice64m_2) | TRICE16_HI_BYTE(ts_Trice64m_2)); \
	TRICE_PUT64_2(v0, v1);                                                                                         \
	TRICE_LEAVE

#define Trice64m_3(tid, v0, v1, v2)                                                                                \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice64m_3 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (24 << 16) | TRICE16_LO_BYTE(ts_Trice64m_3) | TRICE16_HI_BYTE(ts_Trice64m_3)); \
	TRICE_PUT64_3(v0, v1, v2);                                                                                     \
	TRICE_LEAVE

#define Trice64m_4(tid, v0, v1, v2, v3)                                                                            \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice64m_4 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (32 << 16) | TRICE16_LO_BYTE(ts_Trice64m_4) | TRICE16_HI_BYTE(ts_Trice64m_4)); \
	TRICE_PUT64_4(v0, v1, v2, v3);                                                                                 \
	TRICE_LEAVE

#define Trice64m_5(tid, v0, v1, v2, v3, v4)                                                                        \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice64m_5 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (40 << 16) | TRICE16_LO_BYTE(ts_Trice64m_5) | TRICE16_HI_BYTE(ts_Trice64m_5)); \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                                                                             \
	TRICE_LEAVE

#define Trice64m_6(tid, v0, v1, v2, v3, v4, v5)                                                                    \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice64m_6 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (48 << 16) | TRICE16_LO_BYTE(ts_Trice64m_6) | TRICE16_HI_BYTE(ts_Trice64m_6)); \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                                                                         \
	TRICE_LEAVE

#define Trice64m_7(tid, v0, v1, v2, v3, v4, v5, v6)                                                                \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice64m_7 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (56 << 16) | TRICE16_LO_BYTE(ts_Trice64m_7) | TRICE16_HI_BYTE(ts_Trice64m_7)); \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);                                                                     \
	TRICE_LEAVE

#define Trice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)                                                            \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice64m_8 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (64 << 16) | TRICE16_LO_BYTE(ts_Trice64m_8) | TRICE16_HI_BYTE(ts_Trice64m_8)); \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);                                                                 \
	TRICE_LEAVE

#define Trice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)                                                        \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice64m_9 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (72 << 16) | TRICE16_LO_BYTE(ts_Trice64m_9) | TRICE16_HI_BYTE(ts_Trice64m_9)); \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                                                             \
	TRICE_LEAVE

#define Trice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)                                                     \
	TRICE_ENTER                                                                                                      \
	uint16_t ts_Trice64m_10 = TriceStamp16;                                                                          \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                                 \
	TRICE_PUT((TRICE_CYCLE << 24) | (80 << 16) | TRICE16_LO_BYTE(ts_Trice64m_10) | TRICE16_HI_BYTE(ts_Trice64m_10)); \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);                                                          \
	TRICE_LEAVE

#define Trice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)                                                \
	TRICE_ENTER                                                                                                      \
	uint16_t ts_Trice64m_11 = TriceStamp16;                                                                          \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                                 \
	TRICE_PUT((TRICE_CYCLE << 24) | (88 << 16) | TRICE16_LO_BYTE(ts_Trice64m_11) | TRICE16_HI_BYTE(ts_Trice64m_11)); \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);                                                     \
	TRICE_LEAVE

#define Trice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                                           \
	TRICE_ENTER                                                                                                      \
	uint16_t ts_Trice64m_12 = TriceStamp16;                                                                          \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                                 \
	TRICE_PUT((TRICE_CYCLE << 24) | (96 << 16) | TRICE16_LO_BYTE(ts_Trice64m_12) | TRICE16_HI_BYTE(ts_Trice64m_12)); \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                                                 \
	TRICE_LEAVE

#define TRice64m_0(tid)                                                                                        \
	TRICE_ENTER                                                                                                \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                      \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH);                           \
	TRICE_PUT((TRICE_CYCLE << 24) | (0 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_LEAVE

#define TRice64m_1(tid, v0)                                                                                    \
	TRICE_ENTER                                                                                                \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                      \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_PUT64_1(v0)                                                                                          \
	TRICE_LEAVE

#define TRice64m_2(tid, v0, v1)                                                                                 \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (16 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_PUT64_2(v0, v1);                                                                                      \
	TRICE_LEAVE

#define TRice64m_3(tid, v0, v1, v2)                                                                             \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (24 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_PUT64_3(v0, v1, v2);                                                                                  \
	TRICE_LEAVE

#define TRice64m_4(tid, v0, v1, v2, v3)                                                                         \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (32 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_PUT64_4(v0, v1, v2, v3);                                                                              \
	TRICE_LEAVE

#define TRice64m_5(tid, v0, v1, v2, v3, v4)                                                                     \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (40 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                                                                          \
	TRICE_LEAVE

#define TRice64m_6(tid, v0, v1, v2, v3, v4, v5)                                                                 \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (48 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                                                                      \
	TRICE_LEAVE

#define TRice64m_7(tid, v0, v1, v2, v3, v4, v5, v6)                                                             \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (56 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);                                                                  \
	TRICE_LEAVE

#define TRice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)                                                         \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (64 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);                                                              \
	TRICE_LEAVE

#define TRice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)                                                     \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (72 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                                                          \
	TRICE_LEAVE

#define TRice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)                                                \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (80 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);                                                     \
	TRICE_LEAVE

#define TRice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)                                           \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (88 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);                                                \
	TRICE_LEAVE

#define TRice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                                      \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice64m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice64m_) | TRICE32_HH_BYTE(ts_TRice64m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (96 << 16) | TRICE32_LL_BYTE(ts_TRice64m_) | TRICE32_LH_BYTE(ts_TRice64m_)) \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                                            \
	TRICE_LEAVE
