// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceRx.c
//! \brief Common Trice receive record parser and metadata resolvers.

#include "triceRx.h"

#if TRICE_RX_SUPPORT == 1

#include <string.h>

// readU16 reads a transfer-order 16-bit value without assuming alignment.
static uint16_t readU16(const uint8_t* p) {
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
	return (uint16_t)(((uint16_t)p[0] << 8) | (uint16_t)p[1]);
#else
	return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
#endif
}

// readU32 reads a transfer-order 32-bit value without assuming alignment.
static uint32_t readU32(const uint8_t* p) {
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

// setBitWidthAndParamCount assigns the first resolved width and rejects later conflicts.
// Additionally it checks for matching rx->payloadBytes and assigns paramCount.
static int setBitWidthAndParamCount(triceRx_t* rx, uint8_t bitWidth) {
	if (rx->bitWidth == bitWidth){
		return TRICE_RX_RESULT_OK; // already set and identical
	}
	if (rx->bitWidth != TRICE_BIT_WIDTH_UNKNOWN) {
		return TRICE_RX_ERR_BIT_WIDTH_CONFLICT; // already set but different
	}	
	switch( bitWidth ){
	case  0: 
		if (rx->payloadBytes > 0){
			return TRICE_RX_ERR_PAYLOAD_LEN_BITWIDTH_MISMATCH;
		}
		rx->paramCount = 0;
	break;
	case  8:
		rx->paramCount = rx->payloadBytes;
	break;
	case  16:
		if (rx->payloadBytes & 1){
			return TRICE_RX_ERR_PAYLOAD_LEN_BITWIDTH_MISMATCH;
		}
		rx->paramCount = rx->payloadBytes>>1;
	break;
	case  32:
		if (rx->payloadBytes & 3){
			return TRICE_RX_ERR_PAYLOAD_LEN_BITWIDTH_MISMATCH;
		}
		rx->paramCount = rx->payloadBytes>>2;
	break;
	case  64:
		if (rx->payloadBytes & 7){
			return TRICE_RX_ERR_PAYLOAD_LEN_BITWIDTH_MISMATCH;
		}
		rx->paramCount = rx->payloadBytes>>3;
	break;
	default: // de-facto impossible because of generated tables
		return TRICE_RX_ERR_INVALID_BITWIDTH_VALUE;
	break;
	}
	rx->bitWidth = bitWidth;
	return TRICE_RX_RESULT_OK;
}

int TriceParseRecord(triceRx_t* rx, const uint8_t* buf, size_t len) {
	uint16_t firstWord;
	uint16_t nc;
	uint8_t selector;
	size_t offset = 2u;
	memset(rx, 0, sizeof(*rx));
	rx->bitWidth = TRICE_BIT_WIDTH_UNKNOWN;

	if (len < 2u) {
		return TRICE_RX_LEN_TOO_SHORT;
	}

	firstWord = readU16(buf);
	selector = (uint8_t)(firstWord >> 14);

	if (selector == 0u) {
#if TRICE_RX_X0_COUNTED_BUFFER_SUPPORT == 1
		rx->stampBits = TRICE_STAMP_BITS_UNKNOWN;
		rx->payloadBytes = (uint16_t)(firstWord & 0x3fffu);
		if (len < 2u + (size_t)rx->payloadBytes) {
			return TRICE_RX_LEN_TOO_SHORT;
		}
		rx->payload = (rx->payloadBytes == 0u) ? 0 : (buf + 2u);
		return (int)(2u + (size_t)rx->payloadBytes);
#else
		// Add specific typeXO code here, if not using counted typeX0 format.
		return TRICE_RX_ERR_UNSUPPORTED_X0_FRAME;
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
			return TRICE_RX_LEN_TOO_SHORT;
		}
		rx->stamp = (uint32_t)readU16(buf + offset);
		offset += 2u;
		break;

	default: // 3u
		rx->stampBits = 32u;
		if (len < offset + 4u) {
			return TRICE_RX_LEN_TOO_SHORT;
		}
		rx->stamp = readU32(buf + offset);
		offset += 4u;
		break;
	}

	if (len < offset + 2u) {
		return TRICE_RX_LEN_TOO_SHORT;
	}

	nc = readU16(buf + offset);
	offset += 2u;
	if ((nc & 0x8000u) != 0u) {
		rx->payloadBytes = (uint16_t)(nc & 0x7fffu);
		rx->cycleCounter = 0u;
	} else {
		rx->payloadBytes = (uint16_t)(nc >> 8);
		rx->cycleCounter = (uint8_t)nc;
	}

	if (len < offset + (size_t)rx->payloadBytes) {
		return TRICE_RX_LEN_TOO_SHORT;
	}
	if (rx->payloadBytes > 0u){
		rx->payload = buf + offset;
	}
	
	return (int)(offset + (size_t)rx->payloadBytes);
}

#if TRICE_RX_ABC_SUPPORT == 1

int TriceResolveAbc(triceRx_t* rx, const triceAbc_t* list, size_t count) {
	for (size_t i = 0u; i < count; ++i) {
		if (list[i].id == rx->id) {
			int e = setBitWidthAndParamCount(rx, list[i].bitWidth);
			if (e != TRICE_RX_RESULT_OK) {
				return e;
			}
			rx->fn = list[i].fn;
			return TRICE_RX_RESULT_OK;
		}
	}
	return TRICE_RX_ID_WITHOUT_ABC_HANDLER;
}

#endif // #if TRICE_RX_ABC_SUPPORT == 1

#if TRICE_RX_LOG_SUPPORT == 1

int TriceResolveLog(triceRx_t* rx, const triceLog_t* list, size_t count) {
	for (size_t i = 0u; i < count; ++i) {
		if (list[i].id == rx->id) {
			int e = setBitWidthAndParamCount(rx, list[i].bitWidth);
			if (e != TRICE_RX_RESULT_OK) {
				return e;
			}
			if ((list[i].paramCount != TRICE_LOG_PARAM_COUNT_DYNAMIC) 
			 && (list[i].paramCount != rx->paramCount)){
				return TRICE_RX_ERR_PARAM_COUNT_MISMATCH;
			}
			rx->paramCount = list[i].paramCount;
			rx->pFmt = list[i].pFmt;
			return TRICE_RX_RESULT_OK;
		}
	}
	return TRICE_RX_ERR_LOG_ID_NOT_FOUND;
}

#if TRICE_LOCATION_SUPPORT == 1

int TriceResolveLocation(triceRx_t* rx, const triceLocation_t* list, size_t count) {
	for (size_t i = 0u; i < count; ++i) {
		if (list[i].id == rx->id) {
			rx->file = list[i].file;
			rx->line = list[i].line;
			return TRICE_RX_RESULT_OK;
		}
	}
	return TRICE_RX_ERR_LOCATION_ID_NOT_FOUND;
}

#endif // #if TRICE_LOCATION_SUPPORT == 1
#endif // #if TRICE_RX_LOG_SUPPORT == 1

#if TRICE_RX_LOG_SUPPORT == 1
triceNodeFn_t fn_TricePrintLog = 0;
#endif

#if TRICE_RX_X0_COUNTED_BUFFER_SUPPORT == 1
triceNodeFn_t fn_TriceHandleTypeX0 = 0;
#endif


//! \brief parse, classify, and dispatch one fully decoded Trice record.
//! \param record data start
//! \param len record byte count 
//! \retval
int TriceRxHandleRecord(triceRx_t* rx, const uint8_t* record, size_t len) {
	int used = TriceParseRecord(rx, record, len);
	if (used <= 0) {
		return used; // parse error
	}

#if TRICE_RX_ABC_SUPPORT == 1
	if (TriceResolveAbc(rx, triceAbc, (size_t)triceAbcElements) == TRICE_RX_RESULT_OK) {
		rx->fn(rx);
		rx->executed = 1u;	
	}
#endif

#if TRICE_RX_LOG_SUPPORT == 1
	if (fn_TricePrintLog != 0 && TriceResolveLog(rx, triceLog, (size_t)triceLogElements) == TRICE_RX_RESULT_OK){
		fn_TricePrintLog(rx);
		rx->logged = 1;
	}
#endif

#if TRICE_RX_X0_COUNTED_BUFFER_SUPPORT == 1
	if (fn_TriceHandleTypeX0 != 0 && rx->stampBits == TRICE_STAMP_BITS_UNKNOWN) {
		fn_TriceHandleTypeX0(rx);
		rx->handled = 1;
	}	
#endif

	return used;
}

// Advance from one logical record to the next possible record start.
//
// `TriceParseRecord()` intentionally reports only the logical record size.
// The target-side transport can still append zero padding up to the next
// 32-bit boundary. The demo consumes that padding only when all expected bytes
// are actually zero. Otherwise the following byte is treated as the next record
// start so packed records stay decodable.
static size_t TriceAdvanceAlignedRecord(const uint8_t* decoded, size_t decodedLen, size_t offset, size_t logicalUsed) {
	size_t next = offset + logicalUsed;
	size_t alignedNext = (next + 3u) & ~(size_t)3u;
	size_t i;

	if (alignedNext > decodedLen) {
		return next;
	}

	for (i = next; i < alignedNext; ++i) {
		if (decoded[i] != 0u) {
			return next;
		}
	}

	return alignedNext;
}

int TriceHandleDecodedRecord(const uint8_t* record, size_t decodedLen) {
	size_t offset = 0u;
	while (offset < decodedLen) {
		triceRx_t rx;
		int used = TriceRxHandleRecord(&rx, record + offset, decodedLen - offset);
		if (used <= 0) {
			return used; // not enough data or some error
		}
		offset = TriceAdvanceAlignedRecord(record, decodedLen, offset, (size_t)used);
	}
	return offset;
}

#endif // #if TRICE_RX_SUPPORT == 1
