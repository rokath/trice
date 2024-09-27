/*! \file triceMcuOrder.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

//! TRICE_HTOTS reorders short values from hos // t order into trice transfer order.
#define TRICE_HTOTS(x) ((uint16_t)(x))

//! TRICE_HTOTL reorders long values from host order x into trice transfer order.
#define TRICE_HTOTL(x) ((uint32_t)(x))

//! TRICE_TTOHS reorders short values from trice transfer order into host order.
#define TRICE_TTOHS(x) ((uint16_t)(x))

#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 0

//! TRICE_PUT16_1616 writes a 16-bit value followed by a 32-bit value in 2 16-bit steps to avoid memory alignment hard fault.
#define TRICE_PUT16_1616(x, ts) /* little endian */        \
	do {                                                   \
		uint16_t* p = (uint16_t*)TriceBufferWritePosition; \
		*p++ = x;                                          \
		*p++ = ts;         /* lo */                        \
		*p++ = (ts) >> 16; /* hi */                        \
		TriceBufferWritePosition = (uint32_t*)p;           \
	} while (0)

#define TRICE_PUT64(x)        \
	TRICE_PUT((uint32_t)(x)); \
	TRICE_PUT((uint32_t)((uint64_t)(x) >> 32)); // little endian

#else                           // #if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 0

//! TRICE_PUT16_1616 writes a 16-bit value followed by a 32-bit value in 2 16-bit steps to avoid memory alignment hard fault.
#define TRICE_PUT16_1616(x, ts) /* big endian */           \
	do {                                                   \
		uint16_t* p = (uint16_t*)TriceBufferWritePosition; \
		*p++ = x;                                          \
		*p++ = (ts) >> 16; /* hi */                        \
		*p++ = ts;         /* lo */                        \
		TriceBufferWritePosition = (uint32_t*)p;           \
	} while (0)

#define TRICE_PUT64(x)              \
	TRICE_PUT((uint64_t)(x) >> 32); \
	TRICE_PUT((uint32_t)(x)); // big endian

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 0

#if TRICE_REVERSE == 0

#else // #if TRICE_REVERSE == 0

#endif // #else // #if TRICE_REVERSE == 0
