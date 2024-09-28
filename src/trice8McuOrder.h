/*! \file trice8McuOrder.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#define trice8m_0(tid)                                               \
	TRICE_ENTER                                                      \
	TRICE_PUT((0 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_LEAVE

#define trice8m_1(tid, v0)                                           \
	TRICE_ENTER                                                      \
	TRICE_PUT((1 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT8_1(v0)                                                 \
	TRICE_LEAVE

#define trice8m_2(tid, v0, v1)                                       \
	TRICE_ENTER                                                      \
	TRICE_PUT((2 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT8_2(v0, v1);                                            \
	TRICE_LEAVE

#define trice8m_3(tid, v0, v1, v2)                                   \
	TRICE_ENTER                                                      \
	TRICE_PUT((3 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT8_3(v0, v1, v2);                                        \
	TRICE_LEAVE

#define trice8m_4(tid, v0, v1, v2, v3)                               \
	TRICE_ENTER                                                      \
	TRICE_PUT((4 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT8_4(v0, v1, v2, v3);                                    \
	TRICE_LEAVE

#define trice8m_5(tid, v0, v1, v2, v3, v4)                           \
	TRICE_ENTER                                                      \
	TRICE_PUT((5 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT8_5(v0, v1, v2, v3, v4);                                \
	TRICE_LEAVE

#define trice8m_6(tid, v0, v1, v2, v3, v4, v5)                       \
	TRICE_ENTER                                                      \
	TRICE_PUT((6 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT8_6(v0, v1, v2, v3, v4, v5);                            \
	TRICE_LEAVE

#define trice8m_7(tid, v0, v1, v2, v3, v4, v5, v6)                   \
	TRICE_ENTER                                                      \
	TRICE_PUT((7 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT8_7(v0, v1, v2, v3, v4, v5, v6);                        \
	TRICE_LEAVE

#define trice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)               \
	TRICE_ENTER                                                      \
	TRICE_PUT((8 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT8_8(v0, v1, v2, v3, v4, v5, v6, v7);                    \
	TRICE_LEAVE

#define trice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)           \
	TRICE_ENTER                                                      \
	TRICE_PUT((9 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT8_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                \
	TRICE_LEAVE

#define trice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)       \
	TRICE_ENTER                                                       \
	TRICE_PUT((10 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT8_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);            \
	TRICE_LEAVE

#define trice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)  \
	TRICE_ENTER                                                       \
	TRICE_PUT((11 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid))); \
	TRICE_PUT8_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);       \
	TRICE_LEAVE

#define trice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                           \
	TRICE_PUT((12 << 24) | ((TRICE_CYCLE) << 16) | (0x4000 | (tid)));     \
	TRICE_PUT8_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#define Trice8m_0(tid)                            \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(0 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_LEAVE

#define Trice8m_1(tid, v0)                         \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(1 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT8_1(v0)                               \
	TRICE_LEAVE

#define Trice8m_2(tid, v0, v1)                     \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(2 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT8_2(v0, v1);                          \
	TRICE_LEAVE

#define Trice8m_3(tid, v0, v1, v2)                 \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(3 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT8_3(v0, v1, v2);                      \
	TRICE_LEAVE

#define Trice8m_4(tid, v0, v1, v2, v3)             \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(4 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT8_4(v0, v1, v2, v3);                  \
	TRICE_LEAVE

#define Trice8m_5(tid, v0, v1, v2, v3, v4)         \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(5 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT8_5(v0, v1, v2, v3, v4);              \
	TRICE_LEAVE

#define Trice8m_6(tid, v0, v1, v2, v3, v4, v5)     \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(6 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT8_6(v0, v1, v2, v3, v4, v5);          \
	TRICE_LEAVE

#define Trice8m_7(tid, v0, v1, v2, v3, v4, v5, v6) \
	TRICE_ENTER                                    \
	uint16_t ts = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));   \
	TRICE_PUT(7 << 24 | (TRICE_CYCLE << 16) | ts); \
	TRICE_PUT8_7(v0, v1, v2, v3, v4, v5, v6);      \
	TRICE_LEAVE

#define Trice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7) \
	TRICE_ENTER                                        \
	uint16_t ts = TriceStamp16;                        \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));       \
	TRICE_PUT(8 << 24 | (TRICE_CYCLE << 16) | ts);     \
	TRICE_PUT8_8(v0, v1, v2, v3, v4, v5, v6, v7);      \
	TRICE_LEAVE

#define Trice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                            \
	uint16_t ts = TriceStamp16;                            \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));           \
	TRICE_PUT(9 << 24 | (TRICE_CYCLE << 16) | ts);         \
	TRICE_PUT8_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define Trice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                 \
	uint16_t ts = TriceStamp16;                                 \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                \
	TRICE_PUT(10 << 24 | (TRICE_CYCLE << 16) | ts);             \
	TRICE_PUT8_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                      \
	uint16_t ts = TriceStamp16;                                      \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                     \
	TRICE_PUT(11 << 24 | (TRICE_CYCLE << 16) | ts);                  \
	TRICE_PUT8_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                           \
	uint16_t ts = TriceStamp16;                                           \
	TRICE_PUT(0x80008000 | (tid << 16) | (tid));                          \
	TRICE_PUT(12 << 24 | (TRICE_CYCLE << 16) | ts);                       \
	TRICE_PUT8_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#define TRice8m_0(tid)                                     \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(0 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_LEAVE

#define TRice8m_1(tid, v0)                                 \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(1 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT8_1(v0)                                       \
	TRICE_LEAVE

#define TRice8m_2(tid, v0, v1)                             \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(2 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT8_2(v0, v1);                                  \
	TRICE_LEAVE

#define TRice8m_3(tid, v0, v1, v2)                         \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(3 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT8_3(v0, v1, v2);                              \
	TRICE_LEAVE

#define TRice8m_4(tid, v0, v1, v2, v3)                     \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(4 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT8_4(v0, v1, v2, v3);                          \
	TRICE_LEAVE

#define TRice8m_5(tid, v0, v1, v2, v3, v4)                 \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(5 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT8_5(v0, v1, v2, v3, v4);                      \
	TRICE_LEAVE

#define TRice8m_6(tid, v0, v1, v2, v3, v4, v5)             \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(6 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT8_6(v0, v1, v2, v3, v4, v5);                  \
	TRICE_LEAVE

#define TRice8m_7(tid, v0, v1, v2, v3, v4, v5, v6)         \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(7 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT8_7(v0, v1, v2, v3, v4, v5, v6);              \
	TRICE_LEAVE

#define TRice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)     \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(8 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT8_8(v0, v1, v2, v3, v4, v5, v6, v7);          \
	TRICE_LEAVE

#define TRice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8) \
	TRICE_ENTER                                            \
	uint32_t ts = TriceStamp32;                            \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                \
	TRICE_PUT(9 << 24 | (TRICE_CYCLE << 16) | (ts >> 16)); \
	TRICE_PUT8_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);      \
	TRICE_LEAVE

#define TRice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                 \
	uint32_t ts = TriceStamp32;                                 \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                     \
	TRICE_PUT(10 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));     \
	TRICE_PUT8_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define TRice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                      \
	uint32_t ts = TriceStamp32;                                      \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                          \
	TRICE_PUT(11 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));          \
	TRICE_PUT8_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define TRice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                           \
	uint32_t ts = TriceStamp32;                                           \
	TRICE_PUT((ts << 16) | 0xc000 | (tid));                               \
	TRICE_PUT(12 << 24 | (TRICE_CYCLE << 16) | (ts >> 16));               \
	TRICE_PUT8_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE
