// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceLogMinimal.c
//! \brief Separately linkable fallback for exact %d and %x conversions.

#include "trice.h"
#include "triceLogInternal.h"

#if (TRICE_LOCAL_LOG == 1) && (TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER == 1)

// triceLogCopyReversed copies digits accumulated least-significant-first and
// applies the common snprintf-style capacity check. It has no global state and
// does not allocate memory.
static int triceLogCopyReversed(char* dst, size_t size, const char* reversed, size_t count, int negative) {
	size_t required = count + (negative ? 1u : 0u);
	if (required >= size) {
		return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
	}
	if (negative) {
		*dst++ = '-';
	}
	while (count != 0u) {
		*dst++ = reversed[--count];
	}
	*dst = 0;
	return (int)required;
}

// Width-specific aliases and digit capacities prevent 64-bit arithmetic and
// oversized stack arrays from entering a 32-bit-only object file.
#if TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 1
typedef int64_t triceLogMinimalSignedValue_t;
typedef uint64_t triceLogMinimalValue_t;
#define TRICE_LOG_MINIMAL_DECIMAL_DIGITS 21
#define TRICE_LOG_MINIMAL_HEX_DIGITS 16
#else
typedef int32_t triceLogMinimalSignedValue_t;
typedef uint32_t triceLogMinimalValue_t;
#define TRICE_LOG_MINIMAL_DECIMAL_DIGITS 11
#define TRICE_LOG_MINIMAL_HEX_DIGITS 8
#endif

int triceLogMinimalSigned(char* dst, size_t size, triceLogMinimalSignedValue_t value) {
	char reversed[TRICE_LOG_MINIMAL_DECIMAL_DIGITS];
	size_t count = 0u;
	int negative = value < 0;
	// Negating the minimum signed value in its own domain is undefined. The
	// unsigned expression obtains its magnitude without overflowing at either
	// configured width.
	triceLogMinimalValue_t magnitude = negative ? (triceLogMinimalValue_t)(-(value + 1)) + 1u : (triceLogMinimalValue_t)value;
	do {
		reversed[count++] = (char)('0' + (magnitude % 10u));
		magnitude /= 10u;
	} while (magnitude != 0u);
	return triceLogCopyReversed(dst, size, reversed, count, negative);
}

int triceLogMinimalHex(char* dst, size_t size, triceLogMinimalValue_t value) {
	static const char digits[] = "0123456789abcdef";
	char reversed[TRICE_LOG_MINIMAL_HEX_DIGITS];
	size_t count = 0u;
	do {
		reversed[count++] = digits[value & 0x0fu];
		value >>= 4;
	} while (value != 0u);
	return triceLogCopyReversed(dst, size, reversed, count, 0);
}

#undef TRICE_LOG_MINIMAL_DECIMAL_DIGITS
#undef TRICE_LOG_MINIMAL_HEX_DIGITS

#endif // (TRICE_LOCAL_LOG == 1) && (TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER == 1)
