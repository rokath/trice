// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceRx.c
//! \brief Common Trice receive record parser and metadata resolvers.

#include "triceRx.h"

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
		return TRICE_RX_E_RECORD;
	}
	if (rx->payloadBytes != 0u && rx->bitWidth == 0u) {
		return TRICE_RX_E_RECORD;
	}
	if (rx->bitWidth <= 8u) {
		return TRICE_RX_OK;
	}
	if (rx->bitWidth != 16u && rx->bitWidth != 32u && rx->bitWidth != 64u) {
		return TRICE_RX_E_UNSUPPORTED;
	}

	widthBytes = (uint8_t)(rx->bitWidth >> 3);
	return ((rx->payloadBytes % widthBytes) == 0u) ? TRICE_RX_OK : TRICE_RX_E_RECORD;
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

// triceAbcMapError preserves the compact ABC receive error contract while the
// generic receive layer keeps more detailed internal error reasons.
static int triceAbcMapError(int e) {
	return (e == TRICE_RX_E_SHORT) ? TRICE_ABC_RX_E_SHORT : TRICE_ABC_RX_E_PAYLOAD;
}

int TriceAbcOnReceive(const uint8_t* pBuf, int len) {
	triceRx_t rx;
	int used;
	int e;

	if (pBuf == 0 || len < 4) {
		return TRICE_ABC_RX_E_SHORT;
	}

	used = triceParseNextRecord(&rx, pBuf, (size_t)len);
	if (used < 0) {
		return triceAbcMapError(used);
	}

	e = triceResolveAbc(&rx, triceAbc, (size_t)triceAbcElements);
	if (e == TRICE_RX_E_NOT_FOUND) {
		return used;
	}
	if (e != TRICE_RX_OK) {
		return TRICE_ABC_RX_E_PAYLOAD;
	}

	e = triceDispatchAbc(&rx);
	return (e == TRICE_RX_OK || e == TRICE_RX_E_NOT_FOUND) ? used : TRICE_ABC_RX_E_PAYLOAD;
}

#endif // #if TRICE_RX_ABC_SUPPORT == 1

#if TRICE_RX_LOG_SUPPORT == 1

// triceBitWidthFromName extracts explicit widths from common Trice macro names.
// Names without an explicit 8/16/32/64 marker leave bitWidth unresolved.
static uint8_t triceBitWidthFromName(const char* name) {
	const char* p = name;
	if (p == 0) {
		return TRICE_BIT_WIDTH_UNKNOWN;
	}
	while (*p != '\0') {
		if (p[0] == '8') {
			return 8u;
		}
		if (p[0] == '1' && p[1] == '6') {
			return 16u;
		}
		if (p[0] == '3' && p[1] == '2') {
			return 32u;
		}
		if (p[0] == '6' && p[1] == '4') {
			return 64u;
		}
		++p;
	}
	return TRICE_BIT_WIDTH_UNKNOWN;
}

int triceResolveLog(triceRx_t* rx, const triceLogEntry_t* list, size_t count) {
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

int triceResolveLocation(triceRx_t* rx, const triceLocationEntry_t* list, size_t count) {
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

#endif // #if TRICE_RX_LOG_SUPPORT == 1
