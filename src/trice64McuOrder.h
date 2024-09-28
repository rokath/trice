/*! \file trice64McuOrder.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#define trice64m_0(tid)                                              \
	TRICE_ENTER                                                      \
	TRICE_PUT((0 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_LEAVE

#define trice64m_1(tid, v0)                                          \
	TRICE_ENTER                                                      \
	TRICE_PUT((8 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_1(v0)                                                \
	TRICE_LEAVE

#define trice64m_2(tid, v0, v1)                                       \
	TRICE_ENTER                                                       \
	TRICE_PUT((16 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_2(v0, v1);                                            \
	TRICE_LEAVE

#define trice64m_3(tid, v0, v1, v2)                                   \
	TRICE_ENTER                                                       \
	TRICE_PUT((24 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_3(v0, v1, v2);                                        \
	TRICE_LEAVE

#define trice64m_4(tid, v0, v1, v2, v3)                               \
	TRICE_ENTER                                                       \
	TRICE_PUT((32 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_4(v0, v1, v2, v3);                                    \
	TRICE_LEAVE

#define trice64m_5(tid, v0, v1, v2, v3, v4)                           \
	TRICE_ENTER                                                       \
	TRICE_PUT((40 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                                \
	TRICE_LEAVE

#define trice64m_6(tid, v0, v1, v2, v3, v4, v5)                       \
	TRICE_ENTER                                                       \
	TRICE_PUT((48 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                            \
	TRICE_LEAVE

#define trice64m_7(tid, v0, v1, v2, v3, v4, v5, v6)                   \
	TRICE_ENTER                                                       \
	TRICE_PUT((56 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);                        \
	TRICE_LEAVE

#define trice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)               \
	TRICE_ENTER                                                       \
	TRICE_PUT((64 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);                    \
	TRICE_LEAVE

#define trice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)           \
	TRICE_ENTER                                                       \
	TRICE_PUT((72 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                \
	TRICE_LEAVE

#define trice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)      \
	TRICE_ENTER                                                       \
	TRICE_PUT((80 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);           \
	TRICE_LEAVE

#define trice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	TRICE_PUT((88 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define trice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	TRICE_PUT((96 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid)));      \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#define Trice64m_0(tid)                          \
	TRICE_ENTER                                  \
	uint16_t ts = TriceStamp16;                  \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid)); \
	TRICE_PUT((TRICE_CYCLE << 16) | ts);         \
	TRICE_LEAVE

#define Trice64m_1(tid, v0)                        \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(8 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_1(v0)                              \
	TRICE_LEAVE

#define Trice64m_2(tid, v0, v1)                     \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(16 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_2(v0, v1);                          \
	TRICE_LEAVE

#define Trice64m_3(tid, v0, v1, v2)                 \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(24 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_3(v0, v1, v2);                      \
	TRICE_LEAVE

#define Trice64m_4(tid, v0, v1, v2, v3)             \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(32 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_4(v0, v1, v2, v3);                  \
	TRICE_LEAVE

#define Trice64m_5(tid, v0, v1, v2, v3, v4)         \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(40 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);              \
	TRICE_LEAVE

#define Trice64m_6(tid, v0, v1, v2, v3, v4, v5)     \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(48 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);          \
	TRICE_LEAVE

#define Trice64m_7(tid, v0, v1, v2, v3, v4, v5, v6) \
	TRICE_ENTER                                     \
	uint16_t ts = TriceStamp16;                     \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));    \
	TRICE_PUT(56 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);      \
	TRICE_LEAVE

#define Trice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_ENTER                                         \
	uint16_t ts = TriceStamp16;                         \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));        \
	TRICE_PUT(64 << 24 | (TRICE_CYCLE << 16) | ts);     \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);      \
	TRICE_LEAVE

#define Trice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint16_t ts = TriceStamp16;                             \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));            \
	TRICE_PUT(72 << 24 | (TRICE_CYCLE << 16) | ts);         \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define Trice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint16_t ts = TriceStamp16;                                  \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                 \
	TRICE_PUT(80 << 24 | (TRICE_CYCLE << 16) | ts);              \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint16_t ts = TriceStamp16;                                       \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                      \
	TRICE_PUT(88 << 24 | (TRICE_CYCLE << 16) | ts);                   \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint16_t ts = TriceStamp16;                                            \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                           \
	TRICE_PUT(96 << 24 | (TRICE_CYCLE << 16) | ts);                        \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#define TRice64m_0(tid)                                    \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(0 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_LEAVE

#define TRice64m_1(tid, v0)                                \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(8 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_1(v0)                                      \
	TRICE_LEAVE

#define TRice64m_2(tid, v0, v1)                             \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(16 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_2(v0, v1);                                  \
	TRICE_LEAVE

#define TRice64m_3(tid, v0, v1, v2)                         \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(24 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_3(v0, v1, v2);                              \
	TRICE_LEAVE

#define TRice64m_4(tid, v0, v1, v2, v3)                     \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(32 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_4(v0, v1, v2, v3);                          \
	TRICE_LEAVE

#define TRice64m_5(tid, v0, v1, v2, v3, v4)                 \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(40 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_5(v0, v1, v2, v3, v4);                      \
	TRICE_LEAVE

#define TRice64m_6(tid, v0, v1, v2, v3, v4, v5)             \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(48 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_6(v0, v1, v2, v3, v4, v5);                  \
	TRICE_LEAVE

#define TRice64m_7(tid, v0, v1, v2, v3, v4, v5, v6)         \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(56 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_7(v0, v1, v2, v3, v4, v5, v6);              \
	TRICE_LEAVE

#define TRice64m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)     \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(64 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_8(v0, v1, v2, v3, v4, v5, v6, v7);          \
	TRICE_LEAVE

#define TRice64m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                             \
	uint32_t ts = TriceStamp32;                             \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(72 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT64_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define TRice64m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                  \
	uint32_t ts = TriceStamp32;                                  \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                      \
	TRICE_PUT(80 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));      \
	TRICE_PUT64_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice64m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                       \
	uint32_t ts = TriceStamp32;                                       \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                           \
	TRICE_PUT(88 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));           \
	TRICE_PUT64_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice64m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                            \
	uint32_t ts = TriceStamp32;                                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                                \
	TRICE_PUT(96 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));                \
	TRICE_PUT64_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE
