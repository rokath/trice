// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceLog.c
//! \brief Target-side formatting of one deferred binary Trice at a time.

#include "trice.h"
#include "triceLogInternal.h"

#if TRICE_LOCAL_LOG == 1

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
	char conversion;
	char hookFormat[32];
} triceLogConversion_t;

// triceLogParseConversion accepts scalar integer printf syntax. Dynamic width
// or precision would consume additional arguments and is deliberately rejected.
static int triceLogParseConversion(const char* format, size_t start, uint8_t bitWidth, triceLogConversion_t* result) {
	size_t index = start + 1u;
	size_t prefixEnd;
	size_t out = 0u;
	const char* flags = "#0- +'";

	while (format[index] != 0 && strchr(flags, format[index]) != 0) {
		index++;
	}
	if (format[index] == '*') {
		return TRICE_LOG_ERR_FORMAT;
	}
	while ('0' <= format[index] && format[index] <= '9') {
		index++;
	}
	if (format[index] == '.') {
		index++;
		if (format[index] == '*') {
			return TRICE_LOG_ERR_FORMAT;
		}
		while ('0' <= format[index] && format[index] <= '9') {
			index++;
		}
	}
	prefixEnd = index;
	if (format[index] == 'h' && format[index + 1u] == 'h') {
		index += 2u;
	} else if (format[index] == 'l' && format[index + 1u] == 'l') {
		index += 2u;
	} else if (format[index] != 0 && strchr("hljzt", format[index]) != 0) {
		index++;
	}
	result->conversion = format[index];
	if (result->conversion == 0 || strchr("diuoxXc", result->conversion) == 0) {
		return TRICE_LOG_ERR_FORMAT;
	}
	result->next = index + 1u;

	for (size_t source = start; source < prefixEnd; ++source) {
		if (out + 1u >= sizeof(result->hookFormat)) {
			return TRICE_LOG_ERR_FORMAT;
		}
		result->hookFormat[out++] = format[source];
	}
	size_t lengthCharacters = result->conversion == 'c' ? 0u : (bitWidth == 64u ? 2u : 1u);
	// Reserve the normalized length, the conversion itself, and the final NUL.
	if (out + lengthCharacters + 1u >= sizeof(result->hookFormat)) {
		return TRICE_LOG_ERR_FORMAT;
	}
	if (result->conversion != 'c') {
		result->hookFormat[out++] = 'l';
		if (bitWidth == 64u) {
			result->hookFormat[out++] = 'l';
		}
	}
	result->hookFormat[out++] = result->conversion;
	result->hookFormat[out] = 0;
	return 0;
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
	if (bitWidth == 64u) {
		return UserTriceLogPrintfFn(dst, size, conversion->hookFormat, (unsigned long long)value);
	}
	return UserTriceLogPrintfFn(dst, size, conversion->hookFormat, (unsigned long)value);
}

// triceLogFormatRecord transforms one resolved scalar record into text. It
// rejects dynamic payload families and guarantees NUL termination on all exits.
static int triceLogFormatRecord(char* outbuf, size_t maxlen, const triceRx_t* rx) {
	size_t used = 0u;
	size_t formatIndex = 0u;
	size_t valueIndex = 0u;
	size_t valueBytes;
	outbuf[0] = 0;

	if (rx->pFmt == 0 || rx->paramCount == TRICE_LOG_PARAM_COUNT_DYNAMIC) {
		return TRICE_LOG_ERR_FORMAT;
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
