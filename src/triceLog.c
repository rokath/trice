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

TriceLogPrintfFn_t UserTriceLogPrintfFn = 0;

// triceLogReadUnsigned reads one generated-width payload element in transfer
// order. The binary record itself stays in the deferred buffer throughout.
static uint64_t triceLogReadUnsigned(const uint8_t* p, uint8_t bitWidth) {
	uint64_t value = 0u;
	size_t bytes = (size_t)bitWidth >> 3;
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
	for (size_t index = 0u; index < bytes; ++index) {
		value = (value << 8) | p[index];
	}
#else
	for (size_t index = bytes; index != 0u; --index) {
		value = (value << 8) | p[index - 1u];
	}
#endif
	return value;
}

// triceLogSigned sign-extends a payload value according to generated metadata.
static int64_t triceLogSigned(uint64_t value, uint8_t bitWidth) {
	switch (bitWidth) {
	case 8:
		return (int8_t)value;
	case 16:
		return (int16_t)value;
	case 32:
		return (int32_t)value;
	default:
		return (int64_t)value;
	}
}

// triceLogCopyLiteral appends one literal byte while retaining space for NUL.
static int triceLogCopyLiteral(char* outbuf, size_t maxlen, size_t* used, char value) {
	if (*used + 1u >= maxlen) {
		return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
	}
	outbuf[(*used)++] = value;
	outbuf[*used] = 0;
	return 0;
}

// triceLogConversion describes one validated printf conversion and the
// normalized hook format. Length modifiers from source are replaced by the
// argument width that TriceLog actually passes to the variadic hook.
typedef struct {
	size_t next;
	size_t fieldWidth;
	size_t precision;
	char conversion;
	uint8_t precisionSpecified;
	uint8_t leftJustified;
	char hookFormat[32];
} triceLogConversion_t;

// triceLogParseDecimal reads one optional decimal width or precision without
// allowing size_t wraparound. The caller decides whether an absent sequence is
// meaningful; this helper deliberately leaves value at zero in that case.
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

// triceLogParseConversion accepts the local logging printf subset. Dynamic
// width or precision would consume additional arguments and is deliberately
// rejected. Integer lengths are normalized to the type passed to the hook;
// float lengths are removed because variadic float arguments arrive as double.
static int triceLogParseConversion(const char* format, size_t start, uint8_t bitWidth, triceLogConversion_t* result) {
	size_t index = start + 1u;
	size_t prefixEnd;
	size_t lengthStart;
	size_t lengthEnd;
	size_t out = 0u;
	const char* flags = "#0- +'";
	int stringFlagsSupported = 1;

	memset(result, 0, sizeof(*result));

	while (format[index] != 0 && strchr(flags, format[index]) != 0) {
		if (format[index] == '-') {
			result->leftJustified = 1u;
		} else {
			stringFlagsSupported = 0;
		}
		index++;
	}
	if (format[index] == '*') {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (triceLogParseDecimal(format, &index, &result->fieldWidth) < 0) {
		return TRICE_LOG_ERR_FORMAT;
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
	}
	prefixEnd = index;
	lengthStart = index;
	if (format[index] == 'h' && format[index + 1u] == 'h') {
		index += 2u;
	} else if (format[index] == 'l' && format[index + 1u] == 'l') {
		index += 2u;
	} else if (format[index] != 0 && strchr("hljztL", format[index]) != 0) {
		index++;
	}
	lengthEnd = index;
	result->conversion = format[index];
	if (result->conversion == 0 || strchr("diuoxXceEfFgGs", result->conversion) == 0) {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (result->conversion == 's' && (lengthStart != lengthEnd || !stringFlagsSupported)) {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (strchr("eEfFgG", result->conversion) != 0) {
		// C printf accepts no length, l, or L for floating conversions. Trice
		// always supplies a promoted double to the hook, so all three spellings
		// normalize to the same conversion.
		if (lengthEnd - lengthStart > 1u ||
		    (lengthStart != lengthEnd && format[lengthStart] != 'l' && format[lengthStart] != 'L')) {
			return TRICE_LOG_ERR_FORMAT;
		}
	} else if (lengthStart != lengthEnd && format[lengthStart] == 'L') {
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
	if (result->conversion != 'c' && result->conversion != 's' && strchr("eEfFgG", result->conversion) == 0) {
		lengthCharacters = bitWidth == 64u ? 2u : 1u;
	}
	// Reserve the normalized length, the conversion itself, and the final NUL.
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

// triceLogFormatString writes exactly the bytes belonging to one dynamic
// string record. It never relies on payload padding or a following record to
// contain a NUL byte. An embedded NUL from an explicitly sized triceN payload
// still has ordinary printf %s semantics and ends the visible string early.
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
	size_t padding = conversion->fieldWidth > textBytes ? conversion->fieldWidth - textBytes : 0u;
	if (textBytes >= size || padding > size - textBytes - 1u) {
		return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
	}
	if (textBytes > (size_t)INT_MAX || padding > (size_t)INT_MAX - textBytes) {
		return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
	}

	char* output = dst;
	if (!conversion->leftJustified) {
		memset(output, ' ', padding);
		output += padding;
	}
	if (textBytes != 0u) {
		memcpy(output, payload, textBytes);
		output += textBytes;
	}
	if (conversion->leftJustified) {
		memset(output, ' ', padding);
		output += padding;
	}
	*output = 0;
	return (int)(textBytes + padding);
}

// triceLogFormatValue writes one scalar directly at the current output
// position. No intermediate value array or formatted-value buffer is used.
static int triceLogFormatValue(char* dst, size_t size, const triceLogConversion_t* conversion, uint64_t value, uint8_t bitWidth) {
	if (UserTriceLogPrintfFn == 0) {
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

	if (conversion->conversion == 'c') {
		return UserTriceLogPrintfFn(dst, size, conversion->hookFormat, (int)value);
	}
	if (conversion->conversion == 'd' || conversion->conversion == 'i') {
		int64_t signedValue = triceLogSigned(value, bitWidth);
		if (bitWidth == 64u) {
			return UserTriceLogPrintfFn(dst, size, conversion->hookFormat, (long long)signedValue);
		}
		return UserTriceLogPrintfFn(dst, size, conversion->hookFormat, (long)signedValue);
	}
	if (strchr("eEfFgG", conversion->conversion) != 0) {
		double floatingValue;
		if (bitWidth == 32u) {
			uint32_t bits = (uint32_t)value;
			float sourceValue;
			if (sizeof(sourceValue) != sizeof(bits)) {
				return TRICE_LOG_ERR_FORMAT;
			}
			memcpy(&sourceValue, &bits, sizeof(sourceValue));
			floatingValue = (double)sourceValue;
		} else if (bitWidth == 64u) {
			if (sizeof(floatingValue) != sizeof(value)) {
				return TRICE_LOG_ERR_FORMAT;
			}
			memcpy(&floatingValue, &value, sizeof(floatingValue));
		} else {
			return TRICE_LOG_ERR_FORMAT;
		}
		return UserTriceLogPrintfFn(dst, size, conversion->hookFormat, floatingValue);
	}
	if (bitWidth == 64u) {
		return UserTriceLogPrintfFn(dst, size, conversion->hookFormat, (unsigned long long)value);
	}
	return UserTriceLogPrintfFn(dst, size, conversion->hookFormat, (unsigned long)value);
}

// triceLogFormatDynamicString handles the one dynamic family that has an
// unambiguous local text representation. A one-%s format identifies triceS or
// string-form triceN without expanding the generated metadata table with a
// second type field. Other dynamic families retain the same sentinel but are
// rejected by their different format syntax.
static int triceLogFormatDynamicString(char* outbuf, size_t maxlen, const triceRx_t* rx) {
	size_t used = 0u;
	size_t formatIndex = 0u;
	int stringSeen = 0;
	outbuf[0] = 0;

	if (rx->bitWidth != 8u || rx->pFmt == 0) {
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
		if (result < 0 || conversion.conversion != 's' || stringSeen) {
			outbuf[0] = 0;
			return TRICE_LOG_ERR_FORMAT;
		}
		result = triceLogFormatString(outbuf + used, maxlen - used, &conversion, rx->payload, rx->payloadBytes);
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
	outbuf[used] = 0;
	return (int)used;
}

// triceLogFormatRecord transforms one resolved record into text. Dynamic %s is
// length-bounded; all remaining dynamic payload families stay unsupported.
// Every exit leaves the application-owned output buffer NUL-terminated.
static int triceLogFormatRecord(char* outbuf, size_t maxlen, const triceRx_t* rx) {
	size_t used = 0u;
	size_t formatIndex = 0u;
	size_t valueIndex = 0u;
	size_t valueBytes;
	outbuf[0] = 0;

	if (rx->pFmt == 0) {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (rx->paramCount == TRICE_LOG_PARAM_COUNT_DYNAMIC) {
		return triceLogFormatDynamicString(outbuf, maxlen, rx);
	}
	if (rx->bitWidth != 0u && rx->bitWidth != 8u && rx->bitWidth != 16u && rx->bitWidth != 32u && rx->bitWidth != 64u) {
		return TRICE_LOG_ERR_FORMAT;
	}
	valueBytes = (size_t)rx->bitWidth >> 3;
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
		if (valueIndex >= rx->paramCount) {
			outbuf[0] = 0;
			return TRICE_LOG_ERR_FORMAT;
		}

		triceLogConversion_t conversion;
		int result = triceLogParseConversion(rx->pFmt, formatIndex, rx->bitWidth, &conversion);
		if (result < 0) {
			outbuf[0] = 0;
			return result;
		}
		if (conversion.conversion == 's') {
			outbuf[0] = 0;
			return TRICE_LOG_ERR_FORMAT;
		}
		uint64_t value = triceLogReadUnsigned(rx->payload + valueIndex * valueBytes, rx->bitWidth);
		result = triceLogFormatValue(outbuf + used, maxlen - used, &conversion, value, rx->bitWidth);
		if (result < 0) {
			outbuf[0] = 0;
			return result == TRICE_LOG_ERR_OUTPUT_TOO_SMALL || result == TRICE_LOG_ERR_FORMAT ? result : TRICE_LOG_ERR_PRINTF;
		}
		if ((size_t)result >= maxlen - used) {
			outbuf[0] = 0;
			return TRICE_LOG_ERR_OUTPUT_TOO_SMALL;
		}
		used += (size_t)result;
		formatIndex = conversion.next;
		valueIndex++;
	}
	if (valueIndex != rx->paramCount) {
		outbuf[0] = 0;
		return TRICE_LOG_ERR_FORMAT;
	}
	outbuf[used] = 0;
	return (int)used;
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
			// A known ID whose generated width/count conflicts with the binary
			// length has no trustworthy next-record boundary. Drop the queue so
			// stale metadata or a corrupt length cannot wedge or misalign it.
			triceLogBufferDiscardAll();
			return TRICE_LOG_ERR_RECORD;
		}
		int formatResult = triceLogFormatRecord(outbuf, maxlen, &rx);
		if (triceLogBufferRelease(storageBytes) < 0) {
			outbuf[0] = 0;
			return TRICE_LOG_ERR_BUFFER;
		}
		if (formatResult != 0) {
			return formatResult;
		}
		// Consume a valid empty message and continue. This preserves zero as the
		// unambiguous indication that no printable record is available.
	}
}

#endif // TRICE_LOCAL_LOG == 1
