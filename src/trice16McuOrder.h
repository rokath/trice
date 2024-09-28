/*! \file trice16McuOrder.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#define trice16m_0(tid)                                              \
	TRICE_ENTER                                                      \
	TRICE_PUT((0 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_LEAVE

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

#define Trice16m_0(tid)                            \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid)); \
	TRICE_PUT(0 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_LEAVE

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

#define TRice16m_0(tid)                                    \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(0 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_LEAVE

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
