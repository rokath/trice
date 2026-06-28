// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceRx.c
//! \brief Common Trice receive record parser and metadata resolvers.

#include "triceRx.h"

#if TRICE_RX_SUPPORT == 1

#include <string.h>

// triceRxInit clears all optional fields and marks metadata that must be
// supplied later by an ABC or log resolver. `paramCount` stays neutral at 0
// here because the dynamic log sentinel is resolved metadata, not parse-state.
// It stays private because callers should receive initialized records only
// from TriceParseNextRecord().
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
	if((rx->payloadBytes > 0 && bitWidth == 0)
	 ||(rx->payloadBytes & 7 && bitWidth ==64)
	 ||(rx->payloadBytes & 3 && bitWidth ==32)
	 ||(rx->payloadBytes & 1 && bitWidth ==16)){
		return TRICE_RX_E_PAYLOAD;

	if (rx->bitWidth == TRICE_BIT_WIDTH_UNKNOWN) {
		rx->bitWidth = bitWidth;
		return TRICE_RX_OK;
	}
	return (rx->bitWidth == bitWidth) ? TRICE_RX_OK : TRICE_RX_E_BIT_WIDTH_CONFLICT;
}

int TriceParseNextRecord(triceRx_t* rx, const uint8_t* buf, size_t len) {
	uint16_t firstWord;
	uint16_t nc;
	uint8_t selector;
	size_t offset = 2u;

	//  if (rx == 0) {
	//  	return TRICE_RX_E_ARG;
	//  }
	triceRxInit(rx);
	//  if (buf == 0) {
	//  	return TRICE_RX_E_ARG;
	//  }
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
		// Add specific typeXO code here, if not using counted typeX0 format.
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

	default: // 3u
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

// validatePayloadLength cares about payload shape.
static int validatePayloadLength(const triceRx_t* rx) {
	if (rx->bitWidth == TRICE_BIT_WIDTH_UNKNOWN) {
		return TRICE_RX_E_PAYLOAD;
	}
	if((rx->payloadBytes > 0 && rx->bitWidth == 0)
	 ||(rx->payloadBytes & 7 && rx->bitWidth ==64)
	 ||(rx->payloadBytes & 3 && rx->bitWidth ==32)
	 ||(rx->payloadBytes & 1 && rx->bitWidth ==16)){
		return TRICE_RX_E_PAYLOAD;
	}
	return TRICE_RX_OK;
}

#if TRICE_RX_ABC_SUPPORT == 1

int triceResolveAbc(triceRx_t* rx, const triceAbc_t* list, size_t count) {
	size_t i;
	int e;

	//  if (rx == 0 || (list == 0 && count != 0u)) {
	//  	return TRICE_RX_E_ARG;
	//  }
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


int triceDispatchAbc(const triceRx_t* rx) {
	int e;

	//  if (rx == 0) {
	//  	return TRICE_RX_E_ARG;
	//  }
	//  if (rx->fn == 0) {
	//  	return TRICE_RX_E_NOT_FOUND;
	//  }
	e = validatePayloadLength(rx);
	if (e != TRICE_RX_OK) {
		return e;
	}
	rx->fn(rx);
	rx->executed = 1u;
	return TRICE_RX_OK;
}

int TriceAbcOnReceive(const uint8_t* pBuf, int len) {
	triceRx_t rx;
	int used;
	int e;

	if (pBuf == 0 || len < 4) {
		return TRICE_RX_E_SHORT;
	}

	used = TriceParseNextRecord(&rx, pBuf, (size_t)len);
	if (used <= 0) {
		return (used == TRICE_RX_E_SHORT) ? TRICE_RX_E_SHORT : TRICE_RX_E_PAYLOAD;
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

int triceResolveLog(triceRx_t* rx, const triceLog_t* list, size_t count) {
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
			rx->paramCount = list[i].paramCount;
			rx->pFmt = list[i].pFmt;
			return TRICE_RX_OK;
		}
	}
	return TRICE_RX_E_NOT_FOUND;
}

// bitWidthByteCount normalizes generated payload widths to bytes once so
// the ABC and LOG validators do not need duplicate switch logic. Width 0 means
// "no payload", not "unknown", and therefore has no element size.
static int bitWidthByteCount(uint8_t bitWidth, uint8_t* widthBytes) {
	//  if (widthBytes == 0) {
	//  	return TRICE_RX_E_ARG;
	//  }
	switch (bitWidth) {
	case 0u:  // -> 0
	case 8u:  // -> 1
	case 16u: // -> 2
	case 32u: // -> 4
	case 64u: // -> 8
		*widthBytes = (uint8_t)(bitWidth >> 3);
		return TRICE_RX_OK;
	default:
		return TRICE_RX_E_UNSUPPORTED;
	}
}

//
//! triceValidateLogPayload checks the generated count contract. Fixed counts
//! must match exactly; the sentinel means "record counted", so only width
//! alignment can be checked before a formatter interprets the bytes.
//
int triceValidateLogPayload(const triceRx_t* rx) {
	uint8_t widthBytes; // 0, 1, 2, 4, 8
	//int e;

	if (rx->paramCount != TRICE_LOG_PARAM_COUNT_DYNAMIC) {
		return validatePayloadLength(rx);
	}
	
	//  e = bitWidthByteCount(rx->bitWidth, &widthBytes);
	//  if (e != TRICE_RX_OK) {
	//  	return e;
	//  }
	if (rx->paramCount == TRICE_LOG_PARAM_COUNT_DYNAMIC) {
		if (rx->bitWidth == 0u) {
			return (rx->payloadBytes == 0u) ? TRICE_RX_OK : TRICE_RX_E_PAYLOAD;
		}
		return ((rx->payloadBytes % widthBytes) == 0u) ? TRICE_RX_OK : TRICE_RX_E_PAYLOAD;
	}
	if (rx->bitWidth == 0u) {
		return (rx->payloadBytes == 0u && rx->paramCount == 0u) ? TRICE_RX_OK : TRICE_RX_E_PAYLOAD;
	}
	{
		uint32_t expectedBytes = (uint32_t)rx->paramCount * (uint32_t)(rx->bitWidth>>3);
		return (rx->payloadBytes == expectedBytes) ? TRICE_RX_OK : TRICE_RX_E_PAYLOAD;
	}
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

int TriceLogOnReceive(const uint8_t* pBuf, int len) {
	triceRx_t rx;
	int used;
	int e;

	if (pBuf == 0 || len < 4) {
		return TRICE_RX_E_SHORT;
	}

	used = TriceParseNextRecord(&rx, pBuf, (size_t)len);
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

	e = triceValidateLogPayload(&rx);
	return (e == TRICE_RX_E_UNSUPPORTED || e == TRICE_RX_E_NOT_FOUND) ? used : e;
}

// Trice Log evaluates rx and emits
int TriceLog(triceRx_t const* rx){

// Parse, classify, and dispatch one fully decoded Trice record.
int TriceRxHandleRecord(const uint8_t* record, size_t len) {
	triceRx_t rx;
	int used = TriceParseNextRecord(&rx, record, len);
	if (used <= 0) {
		// nodePrintLineF("%s: rx parse error=%d\n", node->name, used);
		return used;
	}

#if TRICE_RX_ABC_SUPPORT == 1
	if (triceResolveAbc(&rx, triceAbc, (size_t)triceAbcElements) == TRICE_RX_OK) {
		e = validatePayloadLength(rx);
		if (e != TRICE_RX_OK) {
			return e;
		}
		rx->fn(rx);
		rx->executed = 1u;	
	}
	#endif
//  #if TRICE_RX_ABC_SUPPORT == 1
//      if (triceResolveAbc(&rx, triceAbc, (size_t)triceAbcElements) == TRICE_RX_OK) {
//          int e = triceDispatchAbc(&rx);
//          if(e == TRICE_RX_OK) {
//              executed_or_logged = 1;
//          }else{
//              // nodePrintLineF("%s: abc dispatch error=%d id=%u\n", node->name, e, (unsigned)rx.id);
//          }
//      }
//  #endif

#if TRICE_RX_LOG_SUPPORT == 1
	if( triceResolveLog(&rx, triceLog, (size_t)triceLogElements) == TRICE_RX_OK ){
        int e = triceValidateLogPayload(&rx);
		if(e == TRICE_RX_OK) {
		    nodePrintResolvedLog(node, &rx);
            executed_or_logged = 1;
        }else{
            nodePrintLineF("%s: log dispatch error=%d id=%u\n", node->name, e, (unsigned)rx.id);
        }
    }
#endif

	if (rx.stampBits == TRICE_STAMP_BITS_UNKNOWN) {
		nodePrintX0(node, &rx);
		executed_or_logged = 1;
	}	
}

#endif // #if TRICE_RX_LOG_SUPPORT == 1

#endif // #if TRICE_RX_SUPPORT == 1
