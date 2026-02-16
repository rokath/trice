/*! \file trice8McuOrder.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE8_MCU_ORDER_H_
#define TRICE8_MCU_ORDER_H_

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

#define Trice8m_0(tid)                                       \
	TRICE_ENTER                                              \
	uint16_t ts_Trice8m_0 = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));             \
	TRICE_PUT(0 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_0); \
	TRICE_LEAVE

#define Trice8m_1(tid, v0)                                   \
	TRICE_ENTER                                              \
	uint16_t ts_Trice8m_1 = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));             \
	TRICE_PUT(1 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_1); \
	TRICE_PUT8_1(v0)                                         \
	TRICE_LEAVE

#define Trice8m_2(tid, v0, v1)                               \
	TRICE_ENTER                                              \
	uint16_t ts_Trice8m_2 = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));             \
	TRICE_PUT(2 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_2); \
	TRICE_PUT8_2(v0, v1);                                    \
	TRICE_LEAVE

#define Trice8m_3(tid, v0, v1, v2)                           \
	TRICE_ENTER                                              \
	uint16_t ts_Trice8m_3 = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));             \
	TRICE_PUT(3 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_3); \
	TRICE_PUT8_3(v0, v1, v2);                                \
	TRICE_LEAVE

#define Trice8m_4(tid, v0, v1, v2, v3)                       \
	TRICE_ENTER                                              \
	uint16_t ts_Trice8m_4 = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));             \
	TRICE_PUT(4 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_4); \
	TRICE_PUT8_4(v0, v1, v2, v3);                            \
	TRICE_LEAVE

#define Trice8m_5(tid, v0, v1, v2, v3, v4)                   \
	TRICE_ENTER                                              \
	uint16_t ts_Trice8m_5 = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));             \
	TRICE_PUT(5 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_5); \
	TRICE_PUT8_5(v0, v1, v2, v3, v4);                        \
	TRICE_LEAVE

#define Trice8m_6(tid, v0, v1, v2, v3, v4, v5)               \
	TRICE_ENTER                                              \
	uint16_t ts_Trice8m_6 = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));             \
	TRICE_PUT(6 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_6); \
	TRICE_PUT8_6(v0, v1, v2, v3, v4, v5);                    \
	TRICE_LEAVE

#define Trice8m_7(tid, v0, v1, v2, v3, v4, v5, v6)           \
	TRICE_ENTER                                              \
	uint16_t ts_Trice8m_7 = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));             \
	TRICE_PUT(7 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_7); \
	TRICE_PUT8_7(v0, v1, v2, v3, v4, v5, v6);                \
	TRICE_LEAVE

#define Trice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)       \
	TRICE_ENTER                                              \
	uint16_t ts_Trice8m_8 = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));             \
	TRICE_PUT(8 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_8); \
	TRICE_PUT8_8(v0, v1, v2, v3, v4, v5, v6, v7);            \
	TRICE_LEAVE

#define Trice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)   \
	TRICE_ENTER                                              \
	uint16_t ts_Trice8m_9 = TriceStamp16;                    \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));             \
	TRICE_PUT(9 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_9); \
	TRICE_PUT8_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);        \
	TRICE_LEAVE

#define Trice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) \
	TRICE_ENTER                                                 \
	uint16_t ts_Trice8m_10 = TriceStamp16;                      \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));                \
	TRICE_PUT(10 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_10);  \
	TRICE_PUT8_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);      \
	TRICE_LEAVE

#define Trice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) \
	TRICE_ENTER                                                      \
	uint16_t ts_Trice8m_11 = TriceStamp16;                           \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));                     \
	TRICE_PUT(11 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_11);       \
	TRICE_PUT8_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);      \
	TRICE_LEAVE

#define Trice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                           \
	uint16_t ts_Trice8m_12 = TriceStamp16;                                \
	TRICE_PUT(0x80008000 | ((tid) << 16) | (tid));                          \
	TRICE_PUT(12 << 24 | (TRICE_CYCLE << 16) | ts_Trice8m_12);            \
	TRICE_PUT8_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#define TRice8m_0(tid)                                               \
	TRICE_ENTER                                                      \
	uint32_t ts_TRice8m_0 = TriceStamp32;                            \
	TRICE_PUT((ts_TRice8m_0 << 16) | 0xc000 | (tid));                \
	TRICE_PUT(0 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_0 >> 16)); \
	TRICE_LEAVE

#define TRice8m_1(tid, v0)                                           \
	TRICE_ENTER                                                      \
	uint32_t ts_TRice8m_1 = TriceStamp32;                            \
	TRICE_PUT((ts_TRice8m_1 << 16) | 0xc000 | (tid));                \
	TRICE_PUT(1 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_1 >> 16)); \
	TRICE_PUT8_1(v0)                                                 \
	TRICE_LEAVE

#define TRice8m_2(tid, v0, v1)                                       \
	TRICE_ENTER                                                      \
	uint32_t ts_TRice8m_2 = TriceStamp32;                            \
	TRICE_PUT((ts_TRice8m_2 << 16) | 0xc000 | (tid));                \
	TRICE_PUT(2 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_2 >> 16)); \
	TRICE_PUT8_2(v0, v1);                                            \
	TRICE_LEAVE

#define TRice8m_3(tid, v0, v1, v2)                                   \
	TRICE_ENTER                                                      \
	uint32_t ts_TRice8m_3 = TriceStamp32;                            \
	TRICE_PUT((ts_TRice8m_3 << 16) | 0xc000 | (tid));                \
	TRICE_PUT(3 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_3 >> 16)); \
	TRICE_PUT8_3(v0, v1, v2);                                        \
	TRICE_LEAVE

#define TRice8m_4(tid, v0, v1, v2, v3)                               \
	TRICE_ENTER                                                      \
	uint32_t ts_TRice8m_4 = TriceStamp32;                            \
	TRICE_PUT((ts_TRice8m_4 << 16) | 0xc000 | (tid));                \
	TRICE_PUT(4 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_4 >> 16)); \
	TRICE_PUT8_4(v0, v1, v2, v3);                                    \
	TRICE_LEAVE

#define TRice8m_5(tid, v0, v1, v2, v3, v4)                           \
	TRICE_ENTER                                                      \
	uint32_t ts_TRice8m_5 = TriceStamp32;                            \
	TRICE_PUT((ts_TRice8m_5 << 16) | 0xc000 | (tid));                \
	TRICE_PUT(5 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_5 >> 16)); \
	TRICE_PUT8_5(v0, v1, v2, v3, v4);                                \
	TRICE_LEAVE

#define TRice8m_6(tid, v0, v1, v2, v3, v4, v5)                       \
	TRICE_ENTER                                                      \
	uint32_t ts_TRice8m_6 = TriceStamp32;                            \
	TRICE_PUT((ts_TRice8m_6 << 16) | 0xc000 | (tid));                \
	TRICE_PUT(6 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_6 >> 16)); \
	TRICE_PUT8_6(v0, v1, v2, v3, v4, v5);                            \
	TRICE_LEAVE

#define TRice8m_7(tid, v0, v1, v2, v3, v4, v5, v6)                   \
	TRICE_ENTER                                                      \
	uint32_t ts_TRice8m_7 = TriceStamp32;                            \
	TRICE_PUT((ts_TRice8m_7 << 16) | 0xc000 | (tid));                \
	TRICE_PUT(7 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_7 >> 16)); \
	TRICE_PUT8_7(v0, v1, v2, v3, v4, v5, v6);                        \
	TRICE_LEAVE

#define TRice8m_8(tid, v0, v1, v2, v3, v4, v5, v6, v7)               \
	TRICE_ENTER                                                      \
	uint32_t ts_TRice8m_8 = TriceStamp32;                            \
	TRICE_PUT((ts_TRice8m_8 << 16) | 0xc000 | (tid));                \
	TRICE_PUT(8 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_8 >> 16)); \
	TRICE_PUT8_8(v0, v1, v2, v3, v4, v5, v6, v7);                    \
	TRICE_LEAVE

#define TRice8m_9(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8)           \
	TRICE_ENTER                                                      \
	uint32_t ts_TRice8m_9 = TriceStamp32;                            \
	TRICE_PUT((ts_TRice8m_9 << 16) | 0xc000 | (tid));                \
	TRICE_PUT(9 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_9 >> 16)); \
	TRICE_PUT8_9(v0, v1, v2, v3, v4, v5, v6, v7, v8);                \
	TRICE_LEAVE

#define TRice8m_10(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)        \
	TRICE_ENTER                                                        \
	uint32_t ts_TRice8m_10 = TriceStamp32;                             \
	TRICE_PUT((ts_TRice8m_10 << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(10 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_10 >> 16)); \
	TRICE_PUT8_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);             \
	TRICE_LEAVE

#define TRice8m_11(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)   \
	TRICE_ENTER                                                        \
	uint32_t ts_TRice8m_11 = TriceStamp32;                             \
	TRICE_PUT((ts_TRice8m_11 << 16) | 0xc000 | (tid));                 \
	TRICE_PUT(11 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_11 >> 16)); \
	TRICE_PUT8_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10);        \
	TRICE_LEAVE

#define TRice8m_12(tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) \
	TRICE_ENTER                                                           \
	uint32_t ts_TRice8m_12 = TriceStamp32;                                \
	TRICE_PUT((ts_TRice8m_12 << 16) | 0xc000 | (tid));                    \
	TRICE_PUT(12 << 24 | (TRICE_CYCLE << 16) | (ts_TRice8m_12 >> 16));    \
	TRICE_PUT8_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11)       \
	TRICE_LEAVE

#endif // #ifndef TRICE8_MCU_ORDER_H_