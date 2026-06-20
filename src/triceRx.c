// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceRx.c
//! \brief Common Trice receive record parser and metadata resolvers.

#include "triceRx.h"

#include <string.h>

// triceRxInit clears all optional fields and marks metadata that must be
// supplied later by an ABC or log resolver. It stays private because callers
// should receive initialized records only from triceParseNextRecord().
static void triceRxInit(triceRx_t* rx) {
	memset(rx, 0, sizeof(*rx));
	rx->bitWidth = TRICE_BIT_WIDTH_UNKNOWN;
}

// triceReadU16 reads a transfer-order 16-bit value without assuming alignment.
static uint16_t triceReadU16(const uint8_t* p) {
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
	return (uint16_t)(((uint16_t)p[0] << 8) | (uint16_t)p[1]);
#else
	return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
#endif
}

// triceReadU32 reads a transfer-order 32-bit value without assuming alignment.
static uint32_t triceReadU32(const uint8_t* p) {
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
	return ((uint32_t)p[0] << 24) |
	       ((uint32_t)p[1] << 16) |
	       ((uint32_t)p[2] << 8) |
	       (uint32_t)p[3];
#else
	return ((uint32_t)p[0]) |
	       ((uint32_t)p[1] << 8) |
	       ((uint32_t)p[2] << 16) |
	       ((uint32_t)p[3] << 24);
#endif
}

// triceSetBitWidth assigns the first resolved width and rejects later conflicts.
static int triceSetBitWidth(triceRx_t* rx, uint8_t bitWidth) {
	if (rx->bitWidth == TRICE_BIT_WIDTH_UNKNOWN) {
		rx->bitWidth = bitWidth;
		return TRICE_RX_OK;
	}
	return (rx->bitWidth == bitWidth) ? TRICE_RX_OK : TRICE_RX_E_BIT_WIDTH_CONFLICT;
}

int triceParseNextRecord(triceRx_t* rx, const uint8_t* buf, size_t len) {
	uint16_t firstWord;
	uint16_t nc;
	uint8_t selector;
	size_t offset = 2u;

	if (rx == 0) {
		return TRICE_RX_E_ARG;
	}
	triceRxInit(rx);
	if (buf == 0) {
		return TRICE_RX_E_ARG;
	}
	if (len < 2u) {
		return TRICE_RX_E_SHORT;
	}

	firstWord = triceReadU16(buf);
	selector = (uint8_t)(firstWord >> 14);

	if (selector == 0u) {
#if TRICE_RX_X0_COUNTED_BUFFER_SUPPORT == 1
		rx->stampBits = TRICE_STAMP_BITS_UNKNOWN;
		rx->payloadBytes = (uint16_t)(firstWord & 0x3fffu);
		if (len < 2u + (size_t)rx->payloadBytes) {
			return TRICE_RX_E_SHORT;
		}
		rx->payload = (rx->payloadBytes == 0u) ? 0 : (buf + 2u);
		return (int)(2u + (size_t)rx->payloadBytes);
#else
		return TRICE_RX_E_UNSUPPORTED;
#endif
	}

	rx->id = (uint16_t)(firstWord & 0x3fffu);
	switch (selector) {
	case 1u:
		rx->stampBits = 0u;
		break;

	case 2u:
		rx->stampBits = 16u;
#if TRICE_DOUBLED_16BIT_ID == 1
		offset += 2u;
#endif
		if (len < offset + 2u) {
			return TRICE_RX_E_SHORT;
		}
		rx->stamp = (uint32_t)triceReadU16(buf + offset);
		offset += 2u;
		break;

	default:
		rx->stampBits = 32u;
		if (len < offset + 4u) {
			return TRICE_RX_E_SHORT;
		}
		rx->stamp = triceReadU32(buf + offset);
		offset += 4u;
		break;
	}

	if (len < offset + 2u) {
		return TRICE_RX_E_SHORT;
	}

	nc = triceReadU16(buf + offset);
	offset += 2u;
	if ((nc & 0x8000u) != 0u) {
		rx->payloadBytes = (uint16_t)(nc & 0x7fffu);
		rx->cycleCounter = 0u;
	} else {
		rx->payloadBytes = (uint16_t)(nc >> 8);
		rx->cycleCounter = (uint8_t)nc;
	}

	if (len < offset + (size_t)rx->payloadBytes) {
		return TRICE_RX_E_SHORT;
	}
	rx->payload = (rx->payloadBytes == 0u) ? 0 : (buf + offset);
	return (int)(offset + (size_t)rx->payloadBytes);
}

#if TRICE_RX_ABC_SUPPORT == 1

int triceResolveAbc(triceRx_t* rx, const triceAbc_t* list, size_t count) {
	size_t i;
	int e;

	if (rx == 0 || (list == 0 && count != 0u)) {
		return TRICE_RX_E_ARG;
	}
	for (i = 0u; i < count; ++i) {
		if (list[i].id == rx->id) {
			e = triceSetBitWidth(rx, list[i].bitWidth);
			if (e != TRICE_RX_OK) {
				return e;
			}
			rx->fn = list[i].fn;
			return TRICE_RX_OK;
		}
	}
	return TRICE_RX_E_NOT_FOUND;
}

// triceValidateAbcPayload rejects records a typed ABC handler cannot interpret.
static int triceValidateAbcPayload(const triceRx_t* rx) {
	uint8_t widthBytes;

	if (rx->bitWidth == TRICE_BIT_WIDTH_UNKNOWN) {
		return TRICE_RX_E_PAYLOAD;
	}
	if (rx->payloadBytes != 0u && rx->bitWidth == 0u) {
		return TRICE_RX_E_PAYLOAD;
	}
	if (rx->bitWidth <= 8u) {
		return TRICE_RX_OK;
	}
	if (rx->bitWidth != 16u && rx->bitWidth != 32u && rx->bitWidth != 64u) {
		return TRICE_RX_E_UNSUPPORTED;
	}

	widthBytes = (uint8_t)(rx->bitWidth >> 3);
	return ((rx->payloadBytes % widthBytes) == 0u) ? TRICE_RX_OK : TRICE_RX_E_PAYLOAD;
}

int triceDispatchAbc(const triceRx_t* rx) {
	int e;

	if (rx == 0) {
		return TRICE_RX_E_ARG;
	}
	if (rx->fn == 0) {
		return TRICE_RX_E_NOT_FOUND;
	}
	e = triceValidateAbcPayload(rx);
	if (e != TRICE_RX_OK) {
		return e;
	}
	rx->fn(rx);
	return TRICE_RX_OK;
}

// triceAbcMapError keeps TriceAbcOnReceive compatible with the compact receive
// error policy: short input consumes no bytes, malformed records are payload errors.
static int triceAbcMapError(int e) {
	return (e == TRICE_RX_E_SHORT) ? TRICE_RX_E_SHORT : TRICE_RX_E_PAYLOAD;
}

int TriceAbcOnReceive(const uint8_t* pBuf, int len) {
	triceRx_t rx;
	int used;
	int e;

	if (pBuf == 0 || len < 4) {
		return TRICE_RX_E_SHORT;
	}

	used = triceParseNextRecord(&rx, pBuf, (size_t)len);
	if (used <= 0) {
		return triceAbcMapError(used);
	}

	e = triceResolveAbc(&rx, triceAbc, (size_t)triceAbcElements);
	if (e == TRICE_RX_E_NOT_FOUND) {
		return used;
	}
	if (e != TRICE_RX_OK) {
		return TRICE_RX_E_PAYLOAD;
	}

	e = triceDispatchAbc(&rx);
	return (e == TRICE_RX_OK || e == TRICE_RX_E_NOT_FOUND) ? used : TRICE_RX_E_PAYLOAD;
}

#endif // #if TRICE_RX_ABC_SUPPORT == 1

#if TRICE_RX_LOG_SUPPORT == 1

// triceStartsWithTrice recognizes the generated TIL type prefix independent of
// the stamp-encoding letter case. It intentionally checks only "trice" because
// the suffix carries the bit-width, payload, and type details.
static int triceStartsWithTrice(const char* name) {
	return name != 0 &&
	       (name[0] == 't' || name[0] == 'T') &&
	       (name[1] == 'r' || name[1] == 'R') &&
	       (name[2] == 'i' || name[2] == 'I') &&
	       (name[3] == 'c' || name[3] == 'C') &&
	       (name[4] == 'e' || name[4] == 'E');
}

// triceIsAbcNameWithoutWidth detects ABC command names like triceC, TRiceC,
// TRICE_C, or TRICe_C after the "trice" prefix. These no-width C forms carry
// no payload, so their width is 0 and must not fall back to the log default.
static int triceIsAbcNameWithoutWidth(const char* suffix) {
	return ((suffix[0] == 'C' || suffix[0] == 'c') && suffix[1] == '\0') ||
	       ((suffix[0] == '_') && (suffix[1] == 'C' || suffix[1] == 'c') && suffix[2] == '\0');
}

// triceBitWidthFromName derives the payload element width from a generated TIL
// type name. Explicit 8/16/32/64 suffixes win. Plain log names use the project
// default parameter width. Plain ABC C names stay width 0 because they have no
// payload and are not default-width log records.
static uint8_t triceBitWidthFromName(const char* name) {
	const char* suffix;
	if (!triceStartsWithTrice(name)) {
		return TRICE_BIT_WIDTH_UNKNOWN;
	}
	suffix = name + 5;
	if (suffix[0] == '8') {
		return 8u;
	}
	if (suffix[0] == '1' && suffix[1] == '6') {
		return 16u;
	}
	if (suffix[0] == '3' && suffix[1] == '2') {
		return 32u;
	}
	if (suffix[0] == '6' && suffix[1] == '4') {
		return 64u;
	}
	if (triceIsAbcNameWithoutWidth(suffix)) {
		return 0u;
	}
	return (uint8_t)TRICE_DEFAULT_PARAMETER_BIT_WIDTH;
}

int triceResolveLog(triceRx_t* rx, const triceLog_t* list, size_t count) {
	size_t i;
	uint8_t bitWidth;
	int e;

	if (rx == 0 || (list == 0 && count != 0u)) {
		return TRICE_RX_E_ARG;
	}
	for (i = 0u; i < count; ++i) {
		if (list[i].id == rx->id) {
			bitWidth = triceBitWidthFromName(list[i].pTrice);
			if (bitWidth != TRICE_BIT_WIDTH_UNKNOWN) {
				e = triceSetBitWidth(rx, bitWidth);
				if (e != TRICE_RX_OK) {
					return e;
				}
			}
			rx->pTrice = list[i].pTrice;
			rx->pFmt = list[i].pFmt;
			return TRICE_RX_OK;
		}
	}
	return TRICE_RX_E_NOT_FOUND;
}

#if TRICE_LOCATION_SUPPORT == 1

int triceResolveLocation(triceRx_t* rx, const triceLocation_t* list, size_t count) {
	size_t i;

	if (rx == 0 || (list == 0 && count != 0u)) {
		return TRICE_RX_E_ARG;
	}
	for (i = 0u; i < count; ++i) {
		if (list[i].id == rx->id) {
			rx->file = list[i].file;
			rx->line = list[i].line;
			return TRICE_RX_OK;
		}
	}
	return TRICE_RX_E_NOT_FOUND;
}

#endif // #if TRICE_LOCATION_SUPPORT == 1

int triceDispatchLog(const triceRx_t* rx) {
	if (rx == 0) {
		return TRICE_RX_E_ARG;
	}
	return (rx->pFmt == 0) ? TRICE_RX_E_NOT_FOUND : TRICE_RX_E_UNSUPPORTED;
}

int TriceLogOnReceive(const uint8_t* pBuf, int len) {
	triceRx_t rx;
	int used;
	int e;

	if (pBuf == 0 || len < 4) {
		return TRICE_RX_E_SHORT;
	}

	used = triceParseNextRecord(&rx, pBuf, (size_t)len);
	if (used < 0) {
		return used;
	}

	e = triceResolveLog(&rx, triceLog, (size_t)triceLogElements);
	if (e == TRICE_RX_E_NOT_FOUND) {
		return used;
	}
	if (e != TRICE_RX_OK) {
		return e;
	}

#if TRICE_LOCATION_SUPPORT == 1
	// Location data is optional metadata. Missing location entries must not
	// prevent consuming a valid log record, but malformed resolver arguments or
	// future resolver errors should still be visible to the caller.
	e = triceResolveLocation(&rx, triceLocation, (size_t)triceLocationElements);
	if (e != TRICE_RX_OK && e != TRICE_RX_E_NOT_FOUND) {
		return e;
	}
#endif

	return used;
}

#endif // #if TRICE_RX_LOG_SUPPORT == 1
