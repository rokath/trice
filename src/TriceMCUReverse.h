/*! \file triceMcuReverse.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_USE_BYTE_SWAP_HEADER
#define TRICE_USE_BYTE_SWAP_HEADER 0
#endif

#ifndef TRICE_USE_BYTE_SWAP_MACROS
#define TRICE_USE_BYTE_SWAP_MACROS 0
#endif

#ifndef TRICE_USE_BYTE_SWAP_INLINE
#define TRICE_USE_BYTE_SWAP_INLINE 1
#endif

#if (TRICE_USE_BYTE_SWAP_HEADER + TRICE_USE_BYTE_SWAP_MACROS + TRICE_USE_BYTE_SWAP_INLINE) > 1
#error "Need max one of them defined: TRICE_USE_BYTE_SWAP_HEADER, TRICE_USE_BYTE_SWAP_MACROS, TRICE_USE_BYTE_SWAP_INLINE"
#endif

#if ((TRICE_USE_BYTE_SWAP_HEADER + TRICE_USE_BYTE_SWAP_MACROS + TRICE_USE_BYTE_SWAP_INLINE) == 0) && !defined(TRICE_HTOTS)
#error "Need one of them defined: TRICE_USE_BYTE_SWAP_HEADER, TRICE_USE_BYTE_SWAP_MACROS, TRICE_USE_BYTE_SWAP_INLINE, TRICE_HTOTS"
#endif

#if ((TRICE_USE_BYTE_SWAP_HEADER + TRICE_USE_BYTE_SWAP_MACROS + TRICE_USE_BYTE_SWAP_INLINE) == 0) && !defined(TRICE_HTOTL)
#error "Need one of them defined: TRICE_USE_BYTE_SWAP_HEADER, TRICE_USE_BYTE_SWAP_MACROS, TRICE_USE_BYTE_SWAP_INLINE, TRICE_HTOTL"
#endif

#if ((TRICE_USE_BYTE_SWAP_HEADER + TRICE_USE_BYTE_SWAP_MACROS + TRICE_USE_BYTE_SWAP_INLINE) == 0) && !defined(TRICE_TTOHS)
#error "Need one of them defined: TRICE_USE_BYTE_SWAP_HEADER, TRICE_USE_BYTE_SWAP_MACROS, TRICE_USE_BYTE_SWAP_INLINE, TRICE_TTOHS"
#endif

#if TRICE_USE_BYTE_SWAP_HEADER == 1
// https://codereview.stackexchange.com/questions/151049/endianness-conversion-in-c
#include <byteswap.h>

#define TRICE_HTOTS(x) __bswap_16(x) //!< TRICE_HTOTS reorders short values from host order into trice transfer order.
#define TRICE_HTOTL(x) __bswap_32(x) //!< TRICE_HTOTL reorders long values from host order x into trice transfer order.
#define TRICE_TTOHS(x) __bswap_16(x) //!< TRICE_TTOHS reorders short values from trice transfer order into host order.

#endif // #if TRICE_USE_BYTE_SWAP_HEADER == 1

#if TRICE_USE_BYTE_SWAP_MACROS == 1

// Swap a 16-bit integer (https://www.oryx-embedded.com/doc/cpu__endian_8h_source.html)
#define TRICE_SWAPINT16(x) (           \
	(((uint16_t)(x) & 0x00FFU) << 8) | \
	(((uint16_t)(x) & 0xFF00U) >> 8))

// Swap a 32-bit integer (https://www.oryx-embedded.com/doc/cpu__endian_8h_source.html)
#define TRICE_SWAPINT32(x) (                 \
	(((uint32_t)(x) & 0x000000FFUL) << 24) | \
	(((uint32_t)(x) & 0x0000FF00UL) << 8) |  \
	(((uint32_t)(x) & 0x00FF0000UL) >> 8) |  \
	(((uint32_t)(x) & 0xFF000000UL) >> 24))

#define TRICE_HTOTS(x) TRICE_SWAPINT16(x) //!< TRICE_HTOTS reorders short values from host order into trice transfer order.
#define TRICE_HTOTL(x) TRICE_SWAPINT32(x) //!< TRICE_HTOTL reorders long values from host order x into trice transfer order.
#define TRICE_TTOHS(x) TRICE_SWAPINT16(x) //!< TRICE_TTOHS reorders short values from trice transfer order into host order.

#endif // #if TRICE_USE_BYTE_SWAP_MACROS == 1

#if TRICE_USE_BYTE_SWAP_INLINE == 1

//! TriceReverse16 swaps low byte and high byte of value and returns it.
TRICE_INLINE uint16_t TriceReverse16(uint16_t value) {
	return (((value & 0x00FF) << 8) |
	        ((value & 0xFF00) >> 8));
}

//! TriceReverse32 converts byte order ov vakue and returns it.
TRICE_INLINE uint32_t TriceReverse32(uint32_t value) {
	return (((value & 0x000000FF) << 24) |
	        ((value & 0x0000FF00) << 8) |
	        ((value & 0x00FF0000) >> 8) |
	        ((value & 0xFF000000) >> 24));
}

#define TRICE_HTOTS(x) TriceReverse16(x) //!< TRICE_HTOTS reorders short values from host order into trice transfer order.
#define TRICE_HTOTL(x) TriceReverse32(x) //!< TRICE_HTOTL reorders long values from host order x into trice transfer order.
#define TRICE_TTOHS(x) TriceReverse16(x) //!< TRICE_TTOHS reorders short values from trice transfer order into host order.

#endif // #if TRICE_USE_BYTE_SWAP_INLINE == 1

#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 0

//! TRICE_PUT16_1616 writes a 16-bit value followed by a 32-bit value in 2 16-bit steps to avoid memory alignment hard fault.
#define TRICE_PUT16_1616(x, ts) /* little endian */        \
	do {                                                   \
		uint16_t* p = (uint16_t*)TriceBufferWritePosition; \
		*p++ = TRICE_HTOTS(x);                             \
		*p++ = TRICE_HTOTS(ts);         /* lo */           \
		*p++ = TRICE_HTOTS((ts) >> 16); /* hi */           \
		TriceBufferWritePosition = (uint32_t*)p;           \
	} while (0)

#define TRICE_PUT64(x)                     \
	TRICE_PUT(TRICE_HTOTL((uint32_t)(x))); \
	TRICE_PUT(TRICE_HTOTL((uint32_t)((uint64_t)(x) >> 32))); // little endian

#else                           // #if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 0

//! TRICE_PUT16_1616 writes a 16-bit value followed by a 32-bit value in 2 16-bit steps to avoid memory alignment hard fault.
#define TRICE_PUT16_1616(x, ts) /* big endian */           \
	do {                                                   \
		uint16_t* p = (uint16_t*)TriceBufferWritePosition; \
		*p++ = TRICE_HTOTS(x);                             \
		*p++ = TRICE_HTOTS((ts) >> 16); /* hi */           \
		*p++ = TRICE_HTOTS(ts);         /* lo */           \
		TriceBufferWritePosition = (uint32_t*)p;           \
	} while (0)

#define TRICE_PUT64(x)                           \
	TRICE_PUT(TRICE_HTOTL((uint64_t)(x) >> 32)); \
	TRICE_PUT(TRICE_HTOTL((uint32_t)(x))); // big endian

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 0

#define idLH TRICE_HTOTS(0x4000 | (tid)) //!< idLH is the no-stamp tid, byte swapped to be used in TRICE_PUT, when TRICE_REVERSE == 1.
#define IdLH TRICE_HTOTS(0x8000 | (tid)) //!< IdLH is the 16-bit-stamp tid, byte swapped to be used in TRICE_PUT, when TRICE_REVERSE == 1.
#define IDLH TRICE_HTOTS(0xc000 | (tid)) //!< IDLH is the 32-bit-stamp tid, byte swapped to be used in TRICE_PUT, when TRICE_REVERSE == 1.

#define tsL ((0x00ff & ts) << 8)
#define tsH ((0xff00 & ts) >> 8)

#define tsHH ((0xFF000000 & ts) >> 8)
#define tsHL ((0x00FF0000 & ts) << 8)
#define tsLH ((0x0000FF00 & ts) >> 8)
#define tsLL ((0x000000FF & ts) << 8)

