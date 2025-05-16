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

#define Trice16m_0(tid)                                                                               \
	TRICE_ENTER                                                                                       \
	uint16_t ts_Trice16m_0 = TriceStamp16;                                                            \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                  \
	TRICE_PUT((TRICE_CYCLE << 24) | TRICE16_LO_BYTE(ts_Trice16m_0) | TRICE16_HI_BYTE(ts_Trice16m_0)); \
	TRICE_LEAVE

//! Trice16m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define Trice16m_1(tid, v0)                                                                                       \
	TRICE_ENTER                                                                                                   \
	uint16_t ts_Trice16m_1 = TriceStamp16;                                                                        \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                              \
	TRICE_PUT((TRICE_CYCLE << 24) | (2 << 16) | TRICE16_LO_BYTE(ts_Trice16m_1) | TRICE16_HI_BYTE(ts_Trice16m_1)); \
	TRICE_PUT16_1(v0)                                                                                             \
	TRICE_LEAVE

#define Trice16m_2(tid, v0, v1)                                                                                   \
	TRICE_ENTER                                                                                                   \
	uint16_t ts_Trice16m_2 = TriceStamp16;                                                                        \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                              \
	TRICE_PUT((TRICE_CYCLE << 24) | (4 << 16) | TRICE16_LO_BYTE(ts_Trice16m_2) | TRICE16_HI_BYTE(ts_Trice16m_2)); \
	TRICE_PUT16_2(v0, v1);                                                                                        \
	TRICE_LEAVE

#define Trice16m_3(tid, v0, v1, v2)                                                                               \
	TRICE_ENTER                                                                                                   \
	uint16_t ts_Trice16m_3 = TriceStamp16;                                                                        \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                              \
	TRICE_PUT((TRICE_CYCLE << 24) | (6 << 16) | TRICE16_LO_BYTE(ts_Trice16m_3) | TRICE16_HI_BYTE(ts_Trice16m_3)); \
	TRICE_PUT16_3(v0, v1, v2);                                                                                    \
	TRICE_LEAVE

#define Trice16m_4(tid, v0, v1, v2, v3)                                                                           \
	TRICE_ENTER                                                                                                   \
	uint16_t ts_Trice16m_4 = TriceStamp16;                                                                        \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                              \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | TRICE16_LO_BYTE(ts_Trice16m_4) | TRICE16_HI_BYTE(ts_Trice16m_4)); \
	TRICE_PUT16_4(v0, v1, v2, v3);                                                                                \
	TRICE_LEAVE

#define Trice16m_5(tid, v0, v1, v2, v3, v4)                                                                        \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice16m_5 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (10 << 16) | TRICE16_LO_BYTE(ts_Trice16m_5) | TRICE16_HI_BYTE(ts_Trice16m_5)); \
	TRICE_PUT16_5(v0, v1, v2, v3, v4);                                                                             \
	TRICE_LEAVE

#define Trice16m_6(tid, v0, v1, v2, v3, v4, v5)                                                                    \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice16m_6 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (12 << 16) | TRICE16_LO_BYTE(ts_Trice16m_6) | TRICE16_HI_BYTE(ts_Trice16m_6)); \
	TRICE_PUT16_6(v0, v1, v2, v3, v4, v5);                                                                         \
	TRICE_LEAVE

#define Trice16m_7(tid, v0, v1, v2, v3, v4, v5, v6)                                                                \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice16m_7 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (14 << 16) | TRICE16_LO_BYTE(ts_Trice16m_7) | TRICE16_HI_BYTE(ts_Trice16m_7)); \
	TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6);                                                                     \
	TRICE_LEAVE

#define Trice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)                                                            \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice16m_8 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (16 << 16) | TRICE16_LO_BYTE(ts_Trice16m_8) | TRICE16_HI_BYTE(ts_Trice16m_8)); \
	TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7);                                                                 \
	TRICE_LEAVE

#define Trice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)                                                        \
	TRICE_ENTER                                                                                                    \
	uint16_t ts_Trice16m_9 = TriceStamp16;                                                                         \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                               \
	TRICE_PUT((TRICE_CYCLE << 24) | (18 << 16) | TRICE16_LO_BYTE(ts_Trice16m_9) | TRICE16_HI_BYTE(ts_Trice16m_9)); \
	TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                                                             \
	TRICE_LEAVE

#define Trice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)                                                     \
	TRICE_ENTER                                                                                                      \
	uint16_t ts_Trice16m_10 = TriceStamp16;                                                                          \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                                 \
	TRICE_PUT((TRICE_CYCLE << 24) | (20 << 16) | TRICE16_LO_BYTE(ts_Trice16m_10) | TRICE16_HI_BYTE(ts_Trice16m_10)); \
	TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);                                                          \
	TRICE_LEAVE

#define Trice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)                                                \
	TRICE_ENTER                                                                                                      \
	uint16_t ts_Trice16m_11 = TriceStamp16;                                                                          \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                                 \
	TRICE_PUT((TRICE_CYCLE << 24) | (22 << 16) | TRICE16_LO_BYTE(ts_Trice16m_11) | TRICE16_HI_BYTE(ts_Trice16m_11)); \
	TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);                                                     \
	TRICE_LEAVE

#define Trice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                                           \
	TRICE_ENTER                                                                                                      \
	uint16_t ts_Trice16m_12 = TriceStamp16;                                                                          \
	TRICE_PUT(((IdLH) << 16) | IdLH)                                                                                 \
	TRICE_PUT((TRICE_CYCLE << 24) | (24 << 16) | TRICE16_LO_BYTE(ts_Trice16m_12) | TRICE16_HI_BYTE(ts_Trice16m_12)); \
	TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                                                 \
	TRICE_LEAVE

#define TRice16m_0(tid)                                                                            \
	TRICE_ENTER                                                                                    \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                          \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                \
	TRICE_PUT((TRICE_CYCLE << 24) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_LEAVE

//! TRice16m_1 writes trice data as fast as possible in a buffer.
//! \param tid is a 14 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define TRice16m_1(tid, v0)                                                                                    \
	TRICE_ENTER                                                                                                \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                                      \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (2 << 16) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_PUT16_1(v0)                                                                                          \
	TRICE_LEAVE

#define TRice16m_2(tid, v0, v1)                                                                                \
	TRICE_ENTER                                                                                                \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                                      \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (4 << 16) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_PUT16_2(v0, v1);                                                                                     \
	TRICE_LEAVE

#define TRice16m_3(tid, v0, v1, v2)                                                                            \
	TRICE_ENTER                                                                                                \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                                      \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (6 << 16) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_PUT16_3(v0, v1, v2);                                                                                 \
	TRICE_LEAVE

#define TRice16m_4(tid, v0, v1, v2, v3)                                                                        \
	TRICE_ENTER                                                                                                \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                                      \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                            \
	TRICE_PUT((TRICE_CYCLE << 24) | (8 << 16) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_PUT16_4(v0, v1, v2, v3);                                                                             \
	TRICE_LEAVE

#define TRice16m_5(tid, v0, v1, v2, v3, v4)                                                                     \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (10 << 16) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_PUT16_5(v0, v1, v2, v3, v4);                                                                          \
	TRICE_LEAVE

#define TRice16m_6(tid, v0, v1, v2, v3, v4, v5)                                                                 \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (12 << 16) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_PUT16_6(v0, v1, v2, v3, v4, v5);                                                                      \
	TRICE_LEAVE

#define TRice16m_7(tid, v0, v1, v2, v3, v4, v5, v6)                                                             \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (14 << 16) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_PUT16_7(v0, v1, v2, v3, v4, v5, v6);                                                                  \
	TRICE_LEAVE

#define TRice16m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)                                                         \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (16 << 16) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_PUT16_8(v0, v1, v2, v3, v4, v5, v6, v7);                                                              \
	TRICE_LEAVE

#define TRice16m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)                                                     \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (18 << 16) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_PUT16_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                                                          \
	TRICE_LEAVE

#define TRice16m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)                                                \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (20 << 16) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);                                                     \
	TRICE_LEAVE

#define TRice16m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)                                           \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (22 << 16) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);                                                \
	TRICE_LEAVE

#define TRice16m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                                      \
	TRICE_ENTER                                                                                                 \
	uint32_t ts_TRice16m_ = TriceStamp32;                                                                       \
	TRICE_PUT(TRICE32_HL_BYTE(ts_TRice16m_) | TRICE32_HH_BYTE(ts_TRice16m_) | IDLH)                             \
	TRICE_PUT((TRICE_CYCLE << 24) | (24 << 16) | TRICE32_LL_BYTE(ts_TRice16m_) | TRICE32_LH_BYTE(ts_TRice16m_)) \
	TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)                                            \
	TRICE_LEAVE
