// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceLog.c
//! \brief Target-side formatting of one deferred binary Trice at a time.

#include "trice.h"
#include "triceLogInternal.h"

#if TRICE_LOCAL_LOG == 1

#include <limits.h>
#include <stdint.h>
#include <string.h>

#if TRICE_LOCAL_LOG_USE_PRINTF_HOOK == 1
TriceLogPrintfFn_t UserTriceLogPrintfFn = 0;
#endif

#if TRICE_LOCAL_LOG_USE_PREFIX_HOOK == 1
TriceLogPrefixFn_t UserTriceLogPrefixFn = 0;
#endif

// Keep the working integer at the largest configured payload width. A
// 32-bit-only build therefore contains neither 64-bit shifts nor 64-bit
// variadic arguments, even without link-time optimization.
#if TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 1
typedef uint64_t triceLogValue_t;
typedef int64_t triceLogSignedValue_t;
#else
typedef uint32_t triceLogValue_t;
typedef int32_t triceLogSignedValue_t;
#endif

// triceLogReadUnsigned reads one generated-width payload element in transfer
// order. The binary record remains in the deferred buffer; only the scalar
// currently being rendered is assembled in a register-sized local value.
static triceLogValue_t triceLogReadUnsigned(const uint8_t* p, uint8_t bitWidth) {
	triceLogValue_t value = 0u;
	size_t bytes = (size_t)bitWidth >> 3;
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
	for (size_t index = 0u; index < bytes; ++index) {
		value = (triceLogValue_t)((value << 8) | p[index]);
	}
#else
	for (size_t index = bytes; index != 0u; --index) {
		value = (triceLogValue_t)((value << 8) | p[index - 1u]);
	}
#endif
	return value;
}

#if TRICE_LOCAL_LOG_USE_PRINTF_HOOK == 1 || TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER == 1
// triceLogSigned sign-extends a payload value according to generated metadata.
static triceLogSignedValue_t triceLogSigned(triceLogValue_t value, uint8_t bitWidth) {
	switch (bitWidth) {
	case 8:
		return (int8_t)value;
	case 16:
		return (int16_t)value;
	case 32:
		return (int32_t)value;
#if TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 1
	case 64:
		return (int64_t)value;
#endif
	default:
		return 0;
	}
}
#endif

// triceLogCopyBytes appends known-length data while always reserving one byte
// for the public API's terminating NUL.
static int triceLogCopyBytes(char* outbuf, size_t maxlen, size_t* used, const char* source, size_t count) {
	if (count >= maxlen || *used > maxlen - count - 1u) {
		return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
	}
	if (count != 0u) {
		memcpy(outbuf + *used, source, count);
		*used += count;
	}
	outbuf[*used] = 0;
	return 0;
}

// triceLogCopyLiteral is the single-byte form used by format scanners.
static int triceLogCopyLiteral(char* outbuf, size_t maxlen, size_t* used, char value) {
	return triceLogCopyBytes(outbuf, maxlen, used, &value, 1u);
}

// triceLogConversion describes one validated conversion and the normalized
// format passed to an optional snprintf-compatible hook. Source length
// modifiers are replaced according to the generated payload width.
typedef struct {
	size_t next;
	size_t fieldWidth;
	size_t precision;
	char conversion;
	uint8_t fieldWidthSpecified;
	uint8_t precisionSpecified;
	uint8_t leftJustified;
	uint8_t zeroPadded;
	uint8_t alternateForm;
	char hookFormat[32];
} triceLogConversion_t;

// triceLogParseDecimal reads one optional decimal width or precision without
// allowing size_t wraparound. An absent sequence intentionally yields zero.
static int triceLogParseDecimal(const char* format, size_t* index, size_t* value) {
	*value = 0u;
	while ('0' <= format[*index] && format[*index] <= '9') {
		size_t digit = (size_t)(format[*index] - '0');
		if (*value > (SIZE_MAX - digit) / 10u) {
			return TRICE_LOG_ERR_FORMAT;
		}
		*value = *value * 10u + digit;
		(*index)++;
	}
	return 0;
}

// triceLogIsIntegerConversion disambiguates C's ptrdiff_t length modifier in
// "%td" from Trice's standalone boolean conversion "%t".
static int triceLogIsIntegerConversion(char conversion) {
	return strchr("diuoxXbOq", conversion) != 0;
}

// triceLogParseConversion accepts the deliberately bounded local formatter
// grammar. Unsupported syntax returns FORMAT; understood but disabled syntax
// returns FEATURE_DISABLED.
static int triceLogParseConversion(const char* format, size_t start, uint8_t bitWidth, triceLogConversion_t* result) {
	size_t index = start + 1u;
	size_t prefixEnd;
	size_t lengthStart;
	size_t lengthEnd;
	size_t out = 0u;
	int stringFlagsSupported = 1;
	int customNumericFlagsSupported = 1;

	memset(result, 0, sizeof(*result));
	while (format[index] != 0 && strchr("#0- +'", format[index]) != 0) {
		switch (format[index]) {
		case '-':
			result->leftJustified = 1u;
			break;
		case '0':
			result->zeroPadded = 1u;
			stringFlagsSupported = 0;
			break;
		case '#':
			result->alternateForm = 1u;
			stringFlagsSupported = 0;
			break;
		default:
			stringFlagsSupported = 0;
			customNumericFlagsSupported = 0;
			break;
		}
		index++;
	}
	if (result->alternateForm && TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG == 0) {
		return TRICE_LOG_ERR_FEATURE_DISABLED;
	}
	if (format[index] == '*') {
		return TRICE_LOG_ERR_FORMAT;
	}
	size_t widthStart = index;
	if (triceLogParseDecimal(format, &index, &result->fieldWidth) < 0) {
		return TRICE_LOG_ERR_FORMAT;
	}
	result->fieldWidthSpecified = (uint8_t)(widthStart != index);
	if (result->fieldWidthSpecified && TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS == 0) {
		return TRICE_LOG_ERR_FEATURE_DISABLED;
	}
	if (format[index] == '.') {
		result->precisionSpecified = 1u;
		index++;
		if (format[index] == '*') {
			return TRICE_LOG_ERR_FORMAT;
		}
		if (triceLogParseDecimal(format, &index, &result->precision) < 0) {
			return TRICE_LOG_ERR_FORMAT;
		}
		if (TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS == 0) {
			return TRICE_LOG_ERR_FEATURE_DISABLED;
		}
	}
	prefixEnd = index;
	lengthStart = index;
	if ((format[index] == 'h' && format[index + 1u] == 'h') || (format[index] == 'l' && format[index + 1u] == 'l')) {
		index += 2u;
	} else if (format[index] != 0 && strchr("hljzL", format[index]) != 0) {
		index++;
	} else if (format[index] == 't' && triceLogIsIntegerConversion(format[index + 1u])) {
		index++;
	}
	lengthEnd = index;
	result->conversion = format[index];
	if (result->conversion == 0 || strchr("diuoxXceEfFgGsbOtpq", result->conversion) == 0) {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (bitWidth == 64u && TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 0) {
		return TRICE_LOG_ERR_FEATURE_DISABLED;
	}
	if (result->conversion == 's' && (lengthStart != lengthEnd || !stringFlagsSupported)) {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (strchr("eEfFgG", result->conversion) != 0) {
		if (TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS == 0 || TRICE_LOCAL_LOG_USE_PRINTF_HOOK == 0) {
			return TRICE_LOG_ERR_FEATURE_DISABLED;
		}
		if (lengthEnd - lengthStart > 1u ||
		    (lengthStart != lengthEnd && format[lengthStart] != 'l' && format[lengthStart] != 'L')) {
			return TRICE_LOG_ERR_FORMAT;
		}
	} else if (lengthStart != lengthEnd && format[lengthStart] == 'L') {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (result->conversion == 'b' && TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS == 0) {
		return TRICE_LOG_ERR_FEATURE_DISABLED;
	}
	if (strchr("Otpq", result->conversion) != 0 && TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS == 0) {
		return TRICE_LOG_ERR_FEATURE_DISABLED;
	}
	if (strchr("bOp", result->conversion) != 0 && !customNumericFlagsSupported) {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (strchr("tq", result->conversion) != 0 && !stringFlagsSupported) {
		return TRICE_LOG_ERR_FORMAT;
	}
	if ((result->conversion == 't' || result->conversion == 'p') && lengthStart != lengthEnd) {
		return TRICE_LOG_ERR_FORMAT;
	}
	result->next = index + 1u;

	for (size_t source = start; source < prefixEnd; ++source) {
		if (out + 1u >= sizeof(result->hookFormat)) {
			return TRICE_LOG_ERR_FORMAT;
		}
		result->hookFormat[out++] = format[source];
	}
	size_t lengthCharacters = 0u;
	if (result->conversion != 'c' && result->conversion != 's' && strchr("eEfFgGbOtpq", result->conversion) == 0) {
		lengthCharacters = bitWidth == 64u ? 2u : 1u;
	}
	if (out + lengthCharacters + 1u >= sizeof(result->hookFormat)) {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (lengthCharacters != 0u) {
		result->hookFormat[out++] = 'l';
		if (lengthCharacters == 2u) {
			result->hookFormat[out++] = 'l';
		}
	}
	result->hookFormat[out++] = result->conversion;
	result->hookFormat[out] = 0;
	return 0;
}

#if TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES == 1 || TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS == 1
// triceLogFormatPaddedText applies string-like width rules to %s, %t, and %q.
static int triceLogFormatPaddedText(char* dst, size_t size, const triceLogConversion_t* conversion, const char* text, size_t textBytes) {
	size_t padding = conversion->fieldWidth > textBytes ? conversion->fieldWidth - textBytes : 0u;
	if (textBytes >= size || padding > size - textBytes - 1u || textBytes > (size_t)INT_MAX || padding > (size_t)INT_MAX - textBytes) {
		return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
	}
	size_t used = 0u;
	if (!conversion->leftJustified) {
		memset(dst, ' ', padding);
		used += padding;
	}
	if (textBytes != 0u) {
		memcpy(dst + used, text, textBytes);
		used += textBytes;
	}
	if (conversion->leftJustified) {
		memset(dst + used, ' ', padding);
		used += padding;
	}
	dst[used] = 0;
	return (int)used;
}
#endif

#if TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES == 1
// triceLogFormatString never assumes that a triceN payload has trailing NUL
// storage. An embedded NUL retains ordinary printf %s semantics.
static int triceLogFormatString(char* dst, size_t size, const triceLogConversion_t* conversion, const uint8_t* payload, size_t payloadBytes) {
	size_t textBytes = payloadBytes;
	if (payloadBytes != 0u) {
		const void* terminator = memchr(payload, 0, payloadBytes);
		if (terminator != 0) {
			textBytes = (size_t)((const uint8_t*)terminator - payload);
		}
	}
	if (conversion->precisionSpecified && textBytes > conversion->precision) {
		textBytes = conversion->precision;
	}
	return triceLogFormatPaddedText(dst, size, conversion, (const char*)payload, textBytes);
}
#endif

#if TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS == 1 || TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS == 1
// triceLogFormatUnsigned implements non-portable integer conversions. Width
// and precision follow printf placement rules, including zero fill after a
// prefix.
static int triceLogFormatUnsigned(char* dst, size_t size, const triceLogConversion_t* conversion, triceLogValue_t value, unsigned base, int uppercase, const char* prefix) {
#if TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 1
	char reverseDigits[65];
#else
	char reverseDigits[33];
#endif
	const char* alphabet = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
	size_t digits = 0u;
	size_t prefixBytes = strlen(prefix);
	do {
		reverseDigits[digits++] = alphabet[(unsigned)(value % base)];
		value /= base;
	} while (value != 0u);
	if (conversion->precisionSpecified && conversion->precision == 0u && digits == 1u && reverseDigits[0] == '0') {
		digits = 0u;
	}
	size_t precisionZeroes = conversion->precision > digits ? conversion->precision - digits : 0u;
	size_t content = prefixBytes + precisionZeroes + digits;
	size_t padding = conversion->fieldWidth > content ? conversion->fieldWidth - content : 0u;
	if (content >= size || padding > size - content - 1u || content > (size_t)INT_MAX || padding > (size_t)INT_MAX - content) {
		return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
	}
	size_t used = 0u;
	int widthIsZeroes = conversion->zeroPadded && !conversion->leftJustified && !conversion->precisionSpecified;
	if (!conversion->leftJustified && !widthIsZeroes) {
		memset(dst + used, ' ', padding);
		used += padding;
	}
	if (prefixBytes != 0u) {
		memcpy(dst + used, prefix, prefixBytes);
		used += prefixBytes;
	}
	if (widthIsZeroes) {
		memset(dst + used, '0', padding);
		used += padding;
	}
	memset(dst + used, '0', precisionZeroes);
	used += precisionZeroes;
	while (digits != 0u) {
		dst[used++] = reverseDigits[--digits];
	}
	if (conversion->leftJustified) {
		memset(dst + used, ' ', padding);
		used += padding;
	}
	dst[used] = 0;
	return (int)used;
}
#endif

#if TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS == 1
// triceLogQuoteByte emits C/Go-like escapes for one byte. Bytes above ASCII
// are copied unchanged so valid UTF-8 remains readable without locale code.
static size_t triceLogQuoteByte(char* dst, uint8_t value, char quote) {
	const char* escape = 0;
	switch (value) {
	case '\a':
		escape = "\\a";
		break;
	case '\b':
		escape = "\\b";
		break;
	case '\f':
		escape = "\\f";
		break;
	case '\n':
		escape = "\\n";
		break;
	case '\r':
		escape = "\\r";
		break;
	case '\t':
		escape = "\\t";
		break;
	case '\v':
		escape = "\\v";
		break;
	case '\\':
		escape = "\\\\";
		break;
	default:
		break;
	}
	if (value == (uint8_t)quote) {
		dst[0] = '\\';
		dst[1] = quote;
		return 2u;
	}
	if (escape != 0) {
		dst[0] = escape[0];
		dst[1] = escape[1];
		return 2u;
	}
	if (value < 0x20u || value == 0x7fu) {
		static const char hex[] = "0123456789abcdef";
		dst[0] = '\\';
		dst[1] = 'x';
		dst[2] = hex[value >> 4];
		dst[3] = hex[value & 15u];
		return 4u;
	}
	dst[0] = (char)value;
	return 1u;
}

// triceLogFormatQuotedString quotes a bounded dynamic payload without copying
// it into an intermediate buffer. A first pass computes the escaped length.
static int triceLogFormatQuotedString(char* dst, size_t size, const triceLogConversion_t* conversion, const uint8_t* payload, size_t payloadBytes) {
	size_t sourceBytes = payloadBytes;
	const void* terminator = payloadBytes == 0u ? 0 : memchr(payload, 0, payloadBytes);
	if (terminator != 0) {
		sourceBytes = (size_t)((const uint8_t*)terminator - payload);
	}
	if (conversion->precisionSpecified && sourceBytes > conversion->precision) {
		sourceBytes = conversion->precision;
	}
	size_t quotedBytes = 2u;
	char scratch[4];
	for (size_t index = 0u; index < sourceBytes; ++index) {
		size_t part = triceLogQuoteByte(scratch, payload[index], '"');
		if (quotedBytes > SIZE_MAX - part) {
			return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
		}
		quotedBytes += part;
	}
	size_t padding = conversion->fieldWidth > quotedBytes ? conversion->fieldWidth - quotedBytes : 0u;
	if (quotedBytes >= size || padding > size - quotedBytes - 1u || quotedBytes > (size_t)INT_MAX || padding > (size_t)INT_MAX - quotedBytes) {
		return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
	}
	size_t used = 0u;
	if (!conversion->leftJustified) {
		memset(dst, ' ', padding);
		used += padding;
	}
	dst[used++] = '"';
	for (size_t index = 0u; index < sourceBytes; ++index) {
		used += triceLogQuoteByte(dst + used, payload[index], '"');
	}
	dst[used++] = '"';
	if (conversion->leftJustified) {
		memset(dst + used, ' ', padding);
		used += padding;
	}
	dst[used] = 0;
	return (int)used;
}

// triceLogFormatQuotedCharacter follows host %q's character interpretation
// and preserves valid Unicode scalar values as UTF-8.
static int triceLogFormatQuotedCharacter(char* dst, size_t size, const triceLogConversion_t* conversion, triceLogValue_t value) {
	char quoted[10];
	size_t used = 0u;
	quoted[used++] = '\'';
	if (value <= 0xffu) {
		used += triceLogQuoteByte(quoted + used, (uint8_t)value, '\'');
	} else {
		uint32_t rune = (uint32_t)value;
		if (rune > 0x10ffffu || (0xd800u <= rune && rune <= 0xdfffu)) {
			rune = 0xfffdu;
		}
		if (rune <= 0x7ffu) {
			quoted[used++] = (char)(0xc0u | (rune >> 6));
			quoted[used++] = (char)(0x80u | (rune & 0x3fu));
		} else if (rune <= 0xffffu) {
			quoted[used++] = (char)(0xe0u | (rune >> 12));
			quoted[used++] = (char)(0x80u | ((rune >> 6) & 0x3fu));
			quoted[used++] = (char)(0x80u | (rune & 0x3fu));
		} else {
			quoted[used++] = (char)(0xf0u | (rune >> 18));
			quoted[used++] = (char)(0x80u | ((rune >> 12) & 0x3fu));
			quoted[used++] = (char)(0x80u | ((rune >> 6) & 0x3fu));
			quoted[used++] = (char)(0x80u | (rune & 0x3fu));
		}
	}
	quoted[used++] = '\'';
	return triceLogFormatPaddedText(dst, size, conversion, quoted, used);
}
#endif

// triceLogFormatValue writes one scalar at the current output position. The
// hook is called once per ordinary scalar; no value array is allocated.
static int triceLogFormatValue(char* dst, size_t size, const triceLogConversion_t* conversion, triceLogValue_t value, uint8_t bitWidth) {
	// A literal/string-only build still compiles the shared fixed-record scanner.
	// Mark its scalar-only inputs intentionally unused when every scalar backend
	// is removed, keeping strict -Werror builds usable without dummy hooks.
#if TRICE_LOCAL_LOG_USE_PRINTF_HOOK == 0 && TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER == 0 && TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS == 0 && TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS == 0
	(void)dst;
	(void)size;
	(void)conversion;
	(void)value;
#endif
	if (bitWidth == 64u && TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 0) {
		return TRICE_LOG_ERR_FEATURE_DISABLED;
	}
#if TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS == 1
	if (conversion->conversion == 'b') {
		return triceLogFormatUnsigned(dst, size, conversion, value, 2u, 0, conversion->alternateForm ? "0b" : "");
	}
#endif
#if TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS == 1
	if (conversion->conversion == 'O') {
		return triceLogFormatUnsigned(dst, size, conversion, value, 8u, 0, "0o");
	}
	if (conversion->conversion == 'p') {
		return triceLogFormatUnsigned(dst, size, conversion, value, 16u, 0, conversion->alternateForm ? "0x" : "");
	}
	if (conversion->conversion == 't') {
		if (conversion->precisionSpecified) {
			return TRICE_LOG_ERR_FORMAT;
		}
		const char* boolean = value == 0u ? "false" : "true";
		return triceLogFormatPaddedText(dst, size, conversion, boolean, strlen(boolean));
	}
	if (conversion->conversion == 'q') {
		if (conversion->precisionSpecified) {
			return TRICE_LOG_ERR_FORMAT;
		}
		return triceLogFormatQuotedCharacter(dst, size, conversion, value);
	}
#endif

#if TRICE_LOCAL_LOG_USE_PRINTF_HOOK == 1
	if (UserTriceLogPrintfFn != 0) {
		int result;
		if (conversion->conversion == 'c') {
			result = UserTriceLogPrintfFn(dst, size, conversion->hookFormat, (int)value);
		} else if (conversion->conversion == 'd' || conversion->conversion == 'i') {
			triceLogSignedValue_t signedValue = triceLogSigned(value, bitWidth);
#if TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 1
			if (bitWidth == 64u) {
				result = UserTriceLogPrintfFn(dst, size, conversion->hookFormat, (long long)signedValue);
			} else
#endif
			{
				result = UserTriceLogPrintfFn(dst, size, conversion->hookFormat, (long)signedValue);
			}
		} else if (strchr("eEfFgG", conversion->conversion) != 0) {
#if TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS == 1
			double floatingValue;
			if (bitWidth == 32u) {
				uint32_t bits = (uint32_t)value;
				float sourceValue;
				memcpy(&sourceValue, &bits, sizeof(sourceValue));
				floatingValue = (double)sourceValue;
#if TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 1
			} else if (bitWidth == 64u) {
				uint64_t bits = (uint64_t)value;
				memcpy(&floatingValue, &bits, sizeof(floatingValue));
#endif
			} else {
				return TRICE_LOG_ERR_FORMAT;
			}
			result = UserTriceLogPrintfFn(dst, size, conversion->hookFormat, floatingValue);
#else
			return TRICE_LOG_ERR_FEATURE_DISABLED;
#endif
		} else {
#if TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 1
			if (bitWidth == 64u) {
				result = UserTriceLogPrintfFn(dst, size, conversion->hookFormat, (unsigned long long)value);
			} else
#endif
			{
				result = UserTriceLogPrintfFn(dst, size, conversion->hookFormat, (unsigned long)value);
			}
		}
		return result < 0 ? TRICE_LOG_ERR_PRINTF : result;
	}
#endif

#if TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER == 1
	if (strcmp(conversion->hookFormat, bitWidth == 64u ? "%lld" : "%ld") == 0) {
		return triceLogMinimalSigned(dst, size, triceLogSigned(value, bitWidth));
	}
	if (strcmp(conversion->hookFormat, bitWidth == 64u ? "%llx" : "%lx") == 0) {
		return triceLogMinimalHex(dst, size, value);
	}
	return TRICE_LOG_ERR_FORMAT;
#else
	return TRICE_LOG_ERR_PRINTF;
#endif
}

// triceLogFormatDynamicString handles triceS and string-form triceN. Distinct
// generated family markers prevent buffer/F/ABC payload misinterpretation.
static int triceLogFormatDynamicString(char* outbuf, size_t maxlen, const triceRx_t* rx) {
#if TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES == 0
	(void)outbuf;
	(void)maxlen;
	(void)rx;
	return TRICE_LOG_ERR_FEATURE_DISABLED;
#else
	size_t used = 0u;
	size_t formatIndex = 0u;
	int stringSeen = 0;
	outbuf[0] = 0;
	if (rx->bitWidth != 8u) {
		return TRICE_LOG_ERR_FORMAT;
	}
	while (rx->pFmt[formatIndex] != 0) {
		if (rx->pFmt[formatIndex] != '%') {
			int result = triceLogCopyLiteral(outbuf, maxlen, &used, rx->pFmt[formatIndex++]);
			if (result < 0) {
				outbuf[0] = 0;
				return result;
			}
			continue;
		}
		if (rx->pFmt[formatIndex + 1u] == '%') {
			int result = triceLogCopyLiteral(outbuf, maxlen, &used, '%');
			if (result < 0) {
				outbuf[0] = 0;
				return result;
			}
			formatIndex += 2u;
			continue;
		}
		triceLogConversion_t conversion;
		int result = triceLogParseConversion(rx->pFmt, formatIndex, rx->bitWidth, &conversion);
		if (result < 0 || (conversion.conversion != 's' && conversion.conversion != 'q') || stringSeen) {
			outbuf[0] = 0;
			return result < 0 ? result : TRICE_LOG_ERR_FORMAT;
		}
		if (conversion.conversion == 's') {
			result = triceLogFormatString(outbuf + used, maxlen - used, &conversion, rx->payload, rx->payloadBytes);
		} else {
#if TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS == 1
			result = triceLogFormatQuotedString(outbuf + used, maxlen - used, &conversion, rx->payload, rx->payloadBytes);
#else
			result = TRICE_LOG_ERR_FEATURE_DISABLED;
#endif
		}
		if (result < 0) {
			outbuf[0] = 0;
			return result;
		}
		used += (size_t)result;
		formatIndex = conversion.next;
		stringSeen = 1;
	}
	if (!stringSeen) {
		outbuf[0] = 0;
		return TRICE_LOG_ERR_FORMAT;
	}
	return (int)used;
#endif
}

// triceLogFormatFixedRange renders a format substring with exactly valueCount
// scalar elements. Buffer Trices reuse it with one value at a time.
static int triceLogFormatFixedRange(char* outbuf, size_t maxlen, const triceRx_t* rx, const char* format, size_t formatBytes, const uint8_t* payload, size_t valueCount) {
	size_t used = 0u;
	size_t formatIndex = 0u;
	size_t valueIndex = 0u;
	size_t valueBytes = (size_t)rx->bitWidth >> 3;
	outbuf[0] = 0;
	while (formatIndex < formatBytes) {
		if (format[formatIndex] != '%') {
			int result = triceLogCopyLiteral(outbuf, maxlen, &used, format[formatIndex++]);
			if (result < 0) {
				outbuf[0] = 0;
				return result;
			}
			continue;
		}
		if (formatIndex + 1u < formatBytes && format[formatIndex + 1u] == '%') {
			int result = triceLogCopyLiteral(outbuf, maxlen, &used, '%');
			if (result < 0) {
				outbuf[0] = 0;
				return result;
			}
			formatIndex += 2u;
			continue;
		}
		if (valueIndex >= valueCount) {
			outbuf[0] = 0;
			return TRICE_LOG_ERR_FORMAT;
		}
		triceLogConversion_t conversion;
		int result = triceLogParseConversion(format, formatIndex, rx->bitWidth, &conversion);
		if (result < 0) {
			outbuf[0] = 0;
			return result;
		}
		if (conversion.next > formatBytes || conversion.conversion == 's') {
			outbuf[0] = 0;
			return TRICE_LOG_ERR_FORMAT;
		}
		triceLogValue_t value = triceLogReadUnsigned(payload + valueIndex * valueBytes, rx->bitWidth);
		result = triceLogFormatValue(outbuf + used, maxlen - used, &conversion, value, rx->bitWidth);
		if (result < 0) {
			outbuf[0] = 0;
			return result;
		}
		if ((size_t)result >= maxlen - used) {
			outbuf[0] = 0;
			return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
		}
		used += (size_t)result;
		formatIndex = conversion.next;
		valueIndex++;
	}
	if (valueIndex != valueCount) {
		outbuf[0] = 0;
		return TRICE_LOG_ERR_FORMAT;
	}
	return (int)used;
}

// triceLogFormatBuffer mirrors the host decoder's B-family convention. Text
// through the first ':' is emitted once; the remaining item format is repeated
// for every element; a trailing newline is emitted once.
static int triceLogFormatBuffer(char* outbuf, size_t maxlen, const triceRx_t* rx) {
#if TRICE_LOCAL_LOG_USE_BUFFER_TRICES == 0
	(void)outbuf;
	(void)maxlen;
	(void)rx;
	return TRICE_LOG_ERR_FEATURE_DISABLED;
#else
	if (rx->bitWidth != 8u && rx->bitWidth != 16u && rx->bitWidth != 32u && rx->bitWidth != 64u) {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (rx->bitWidth == 64u && TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 0) {
		return TRICE_LOG_ERR_FEATURE_DISABLED;
	}
	size_t valueBytes = (size_t)rx->bitWidth >> 3;
	if (valueBytes == 0u || rx->payloadBytes % valueBytes != 0u) {
		return TRICE_LOG_ERR_RECORD;
	}
	size_t used = 0u;
	size_t fullFormatBytes = strlen(rx->pFmt);
	size_t itemStart = 0u;
	const char* colon = strchr(rx->pFmt, ':');
	if (colon != 0) {
		itemStart = (size_t)(colon - rx->pFmt) + 1u;
		int result = triceLogCopyBytes(outbuf, maxlen, &used, rx->pFmt, itemStart);
		if (result < 0) {
			outbuf[0] = 0;
			return result;
		}
	}
	int trailingNewline = fullFormatBytes != 0u && rx->pFmt[fullFormatBytes - 1u] == '\n';
	size_t itemEnd = fullFormatBytes - (size_t)trailingNewline;
	if (itemStart >= itemEnd) {
		outbuf[0] = 0;
		return TRICE_LOG_ERR_FORMAT;
	}
	size_t count = rx->payloadBytes / valueBytes;
	if (count == 0u) {
#if TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 1
		uint8_t zero[8] = {0};
#else
		uint8_t zero[4] = {0};
#endif
		char scratch[2];
		int validation = triceLogFormatFixedRange(scratch, sizeof(scratch), rx, rx->pFmt + itemStart, itemEnd - itemStart, zero, 1u);
		if (validation < 0 && validation != TRICE_LOG_ERR_OUTPUT_TOO_SMALL) {
			outbuf[0] = 0;
			return validation;
		}
	}
	for (size_t index = 0u; index < count; ++index) {
		int result = triceLogFormatFixedRange(outbuf + used, maxlen - used, rx, rx->pFmt + itemStart, itemEnd - itemStart, rx->payload + index * valueBytes, 1u);
		if (result < 0) {
			outbuf[0] = 0;
			return result;
		}
		used += (size_t)result;
	}
	if (trailingNewline) {
		int result = triceLogCopyLiteral(outbuf, maxlen, &used, '\n');
		if (result < 0) {
			outbuf[0] = 0;
			return result;
		}
	}
	return (int)used;
#endif
}

// triceLogFormatRecord dispatches on the generated family marker. Null pFmt
// deliberately marks an ID filtered by configuration.
static int triceLogFormatRecord(char* outbuf, size_t maxlen, const triceRx_t* rx) {
	outbuf[0] = 0;
	if (rx->paramCount == TRICE_LOG_PARAM_COUNT_DYNAMIC_FUNCTION || rx->paramCount == TRICE_LOG_PARAM_COUNT_DYNAMIC_ABC) {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (rx->pFmt == 0) {
		return TRICE_LOG_ERR_FEATURE_DISABLED;
	}
	switch (rx->paramCount) {
	case TRICE_LOG_PARAM_COUNT_DYNAMIC_STRING:
		return triceLogFormatDynamicString(outbuf, maxlen, rx);
	case TRICE_LOG_PARAM_COUNT_DYNAMIC_BUFFER:
		return triceLogFormatBuffer(outbuf, maxlen, rx);
	default:
		break;
	}
	if (rx->bitWidth != 0u && rx->bitWidth != 8u && rx->bitWidth != 16u && rx->bitWidth != 32u && rx->bitWidth != 64u) {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (rx->bitWidth == 64u && rx->paramCount != 0u && TRICE_LOCAL_LOG_USE_64_BIT_VALUES == 0) {
		return TRICE_LOG_ERR_FEATURE_DISABLED;
	}
	return triceLogFormatFixedRange(outbuf, maxlen, rx, rx->pFmt, strlen(rx->pFmt), rx->payload, rx->paramCount);
}

int TriceLog(char* outbuf, size_t maxlen) {
	if (outbuf == 0 || maxlen == 0u) {
		return TRICE_LOG_ERR_ARGUMENT;
	}
	outbuf[0] = 0;

	for (;;) {
		const uint8_t* record = 0;
		size_t available = 0u;
		int bufferResult = triceLogBufferPeek(&record, &available);
		if (bufferResult <= 0) {
			return bufferResult == 0 ? 0 : TRICE_LOG_ERR_BUFFER;
		}

		triceRx_t rx;
		int recordBytes = TriceParseRecord(&rx, record, available);
		if (recordBytes <= 0) {
			triceLogBufferDiscardAll();
			return TRICE_LOG_ERR_RECORD;
		}
		size_t storageBytes = ((size_t)recordBytes + 3u) & ~(size_t)3u;
		if (storageBytes > available) {
			triceLogBufferDiscardAll();
			return TRICE_LOG_ERR_RECORD;
		}

		int resolveResult = TriceResolveLog(&rx, triceLog, (size_t)triceLogElements);
		if (resolveResult != TRICE_RX_RESULT_OK) {
			if (resolveResult == TRICE_RX_ERR_LOG_ID_NOT_FOUND) {
				if (triceLogBufferRelease(storageBytes) < 0) {
					return TRICE_LOG_ERR_BUFFER;
				}
				return TRICE_LOG_ERR_ID;
			}
			triceLogBufferDiscardAll();
			return TRICE_LOG_ERR_RECORD;
		}

		size_t prefixBytes = 0u;
#if TRICE_LOCAL_LOG_USE_PREFIX_HOOK == 1
		if (UserTriceLogPrefixFn != 0) {
			int prefixResult = UserTriceLogPrefixFn(outbuf, maxlen, rx.id, rx.stampBits, rx.stamp);
			if (prefixResult < 0) {
				outbuf[0] = 0;
				if (triceLogBufferRelease(storageBytes) < 0) {
					return TRICE_LOG_ERR_BUFFER;
				}
				return TRICE_LOG_ERR_PREFIX;
			}
			if ((size_t)prefixResult >= maxlen) {
				outbuf[0] = 0;
				if (triceLogBufferRelease(storageBytes) < 0) {
					return TRICE_LOG_ERR_BUFFER;
				}
				return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
			}
			prefixBytes = (size_t)prefixResult;
		}
#endif
		int formatResult = triceLogFormatRecord(outbuf + prefixBytes, maxlen - prefixBytes, &rx);
		if (formatResult < 0) {
			outbuf[0] = 0;
		} else if (formatResult > 0 && prefixBytes > (size_t)INT_MAX - (size_t)formatResult) {
			outbuf[0] = 0;
			formatResult = TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
		} else {
			formatResult += (int)prefixBytes;
		}
		if (triceLogBufferRelease(storageBytes) < 0) {
			outbuf[0] = 0;
			return TRICE_LOG_ERR_BUFFER;
		}
		if (formatResult != 0) {
			return formatResult;
		}
	}
}

#endif // TRICE_LOCAL_LOG == 1
