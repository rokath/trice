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

int triceLogMinimalSigned(char* dst, size_t size, int64_t value) {
	char reversed[21]; // UINT64_MAX needs 20 decimal digits; one byte remains for simple bounds reasoning.
	size_t count = 0u;
	int negative = value < 0;
	// Converting -(INT64_MIN) in the signed domain is undefined. The unsigned
	// expression below obtains its magnitude without overflowing.
	uint64_t magnitude = negative ? (uint64_t)(-(value + 1)) + 1u : (uint64_t)value;
	do {
		reversed[count++] = (char)('0' + (magnitude % 10u));
		magnitude /= 10u;
	} while (magnitude != 0u);
	return triceLogCopyReversed(dst, size, reversed, count, negative);
}

int triceLogMinimalHex(char* dst, size_t size, uint64_t value) {
	static const char digits[] = "0123456789abcdef";
	char reversed[16]; // UINT64_MAX needs exactly 16 hexadecimal digits.
	size_t count = 0u;
	do {
		reversed[count++] = digits[value & 0x0fu];
		value >>= 4;
	} while (value != 0u);
	return triceLogCopyReversed(dst, size, reversed, count, 0);
}

#endif // (TRICE_LOCAL_LOG == 1) && (TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER == 1)
