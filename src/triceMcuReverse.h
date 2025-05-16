/*! \file triceMcuReverse.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

//! Note:  https://gcc.gnu.org/onlinedocs/cpp/_005f_005fhas_005finclude.html
#if defined __has_include //! __has_include is a macro defined in several compilers including GCC and integrated into the C23 standard (Note N2799)
#if __has_include(<byteswap.h>)
#include <byteswap.h> // https://codereview.stackexchange.com/questions/151049/endianness-conversion-in-c
#define TRICE_LIBC_BYTESWAP 1
#endif
#endif

// Swap a 16-bit integer (https://www.oryx-embedded.com/doc/cpu__endian_8h_source.html)
TRICE_INLINE uint16_t TriceReverse16(uint16_t value) {
#if (defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 403)) || (defined(__clang__) && __has_builtin(__builtin_bswap16))
	return __builtin_bswap16(value);
#elif (defined(TRICE_LIBC_BYTESWAP) && TRICE_LIBC_BYTESWAP == 1)
	return __bswap_16(value);
#else
	return (((value & 0x00FF) << 8) |
	        ((value & 0xFF00) >> 8));
#endif
}
// Swap a 32-bit integer (https://www.oryx-embedded.com/doc/cpu__endian_8h_source.html)
TRICE_INLINE uint32_t TriceReverse32(uint32_t value) {
#if (defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 403)) || (defined(__clang__) && __has_builtin(__builtin_bswap32))
	return __builtin_bswap32(value);
#elif (defined(TRICE_LIBC_BYTESWAP) && TRICE_LIBC_BYTESWAP == 1)
	return __bswap_32(value);
#else
	return (((value & 0x000000FF) << 24) |
	        ((value & 0x0000FF00) << 8) |
	        ((value & 0x00FF0000) >> 8) |
	        ((value & 0xFF000000) >> 24));
#endif
}

#if !defined(TRICE_HTOTS) && !defined(TRICE_HTOTL) && !defined(TRICE_TTOHS)
#define TRICE_HTOTS(x) TriceReverse16(x) //!< TRICE_HTOTS reorders short values from host order into trice transfer order.
#define TRICE_HTOTL(x) TriceReverse32(x) //!< TRICE_HTOTL reorders long values from host order x into trice transfer order.
#define TRICE_TTOHS(x) TriceReverse16(x) //!< TRICE_TTOHS reorders short values from trice transfer order into host order.
#endif

#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 0

//! TRICE_PUT16_1616 writes a 16-bit value followed by a 32-bit value in 2 16-bit steps to avoid memory alignment hard fault.
#define TRICE_PUT16_1616(x, ts) /* little endian */                         \
	do {                                                                    \
		uint16_t* p_TRICE_PUT16_1616 = (uint16_t*)TriceBufferWritePosition; \
		*p_TRICE_PUT16_1616++ = TRICE_HTOTS(x);                             \
		*p_TRICE_PUT16_1616++ = TRICE_HTOTS(ts);         /* lo */           \
		*p_TRICE_PUT16_1616++ = TRICE_HTOTS((ts) >> 16); /* hi */           \
		TriceBufferWritePosition = (uint32_t*)p_TRICE_PUT16_1616;           \
	} while (0)

#define TRICE_PUT64(x)                     \
	TRICE_PUT(TRICE_HTOTL((uint32_t)(x))); \
	TRICE_PUT(TRICE_HTOTL((uint32_t)((uint64_t)(x) >> 32))); // little endian

#else                           // #if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 0

//! TRICE_PUT16_1616 writes a 16-bit value followed by a 32-bit value in 2 16-bit steps to avoid memory alignment hard fault.
#define TRICE_PUT16_1616(x, ts) /* big endian */                            \
	do {                                                                    \
		uint16_t* p_TRICE_PUT16_1616 = (uint16_t*)TriceBufferWritePosition; \
		*p_TRICE_PUT16_1616++ = TRICE_HTOTS(x);                             \
		*p_TRICE_PUT16_1616++ = TRICE_HTOTS((ts) >> 16); /* hi */           \
		*p_TRICE_PUT16_1616++ = TRICE_HTOTS(ts);         /* lo */           \
		TriceBufferWritePosition = (uint32_t*)p_TRICE_PUT16_1616;           \
	} while (0)

#define TRICE_PUT64(x)                           \
	TRICE_PUT(TRICE_HTOTL((uint64_t)(x) >> 32)); \
	TRICE_PUT(TRICE_HTOTL((uint32_t)(x))); // big endian

#endif // #else // #if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 0

#define idLH TRICE_HTOTS(0x4000 | (tid)) //!< idLH is the no-stamp tid, byte swapped to be used in TRICE_PUT, when TRICE_REVERSE == 1.
#define IdLH TRICE_HTOTS(0x8000 | (tid)) //!< IdLH is the 16-bit-stamp tid, byte swapped to be used in TRICE_PUT, when TRICE_REVERSE == 1.
#define IDLH TRICE_HTOTS(0xc000 | (tid)) //!< IDLH is the 32-bit-stamp tid, byte swapped to be used in TRICE_PUT, when TRICE_REVERSE == 1.

#define TRICE16_LO_BYTE(x16) ((0x00ff & x16) << 8) // was #define tsL ((0x00ff & ts) << 8)
#define TRICE16_HI_BYTE(x16) ((0xff00 & x16) >> 8) // was #define tsH ((0xff00 & ts) >> 8)

#define TRICE32_HH_BYTE(x32) ((0xFF000000 & x32) >> 8) // was #define tsHH ((0xFF000000 & ts) >> 8)
#define TRICE32_HL_BYTE(x32) ((0x00FF0000 & x32) << 8) // was #define tsHL ((0x00FF0000 & ts) << 8)
#define TRICE32_LH_BYTE(x32) ((0x0000FF00 & x32) >> 8) // was #define tsLH ((0x0000FF00 & ts) >> 8)
#define TRICE32_LL_BYTE(x32) ((0x000000FF & x32) << 8) // was #define tsLL ((0x000000FF & ts) << 8)
