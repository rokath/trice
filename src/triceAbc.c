// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceAbcReceive.c
//! \brief Trice ABC receive runtime.

#include "triceAbcReceive.h"

#if TRICE_ABC_RECEIVE_SUPPORT == 1

//! triceAbcReadU16 reads one 16-bit Trice transfer-order value from p.
//!
//! The value is assembled byte-wise, so the code is independent of MCU byte
//! order and works also for unaligned input data. The byte order used here is
//! the Trice transfer order, selected by TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN.
static uint16_t triceAbcReadU16(const uint8_t* p) {
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
	return (uint16_t)(((uint16_t)p[0] << 8) | (uint16_t)p[1]);
#else
	return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
#endif
}

//! triceAbcReadU32 reads one 32-bit Trice transfer-order value from p.
//!
//! The value is assembled byte-wise, so the code is independent of MCU byte
//! order and works also for unaligned input data. The byte order used here is
//! the Trice transfer order, selected by TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN.
static uint32_t triceAbcReadU32(const uint8_t* p) {
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

//! triceAbcFind returns the selected handler table entry for id or 0 when id is not selected locally.
static const triceAbc_t* triceAbcFind(uint16_t id) {
	for (unsigned i = 0; i < triceAbcElements; i++) {
		if (triceAbc[i].id == id) {
			return &triceAbc[i];
		}
	}
	return 0;
}

//! TriceAbcOnReceive parses one decoded Trice record and calls the selected local handler directly.
int TriceAbcOnReceive(const uint8_t* pBuf, int len) {
	triceAbcRx_t abc = {0};
	const triceAbc_t* entry;
	uint16_t w;
	int offset = 2;

	if (pBuf == 0 || len < 4) {
		return TRICE_ABC_RX_E_SHORT;
	}

	w = triceAbcReadU16(pBuf);
	abc.id = (uint16_t)(w & 0x3FFFu);

	entry = triceAbcFind(abc.id);
	if (entry == 0) {
		return TRICE_ABC_RX_IGNORED;
	}

	abc.bitWidth = entry->bitWidth;

	switch ((uint8_t)(w >> 14)) { // selector bits 
	case 1u: // no stamp
		abc.stampBits = 0u;
		break;

	case 2u: // 16-bit stamp
		abc.stampBits = 16u;
#if TRICE_DOUBLED_16BIT_ID == 1
		offset += 2;
#endif
		if (len < offset + 2) {
			return TRICE_ABC_RX_E_SHORT;
		}
		abc.stamp = (uint32_t)triceAbcReadU16(pBuf + offset);
		offset += 2;
		break;

	case 3u: // 32-bit stamp
		abc.stampBits = 32u;
		if (len < offset + 4) {
			return TRICE_ABC_RX_E_SHORT;
		}
		abc.stamp = triceAbcReadU32(pBuf + offset);
		offset += 4;
		break;

	default:
		return TRICE_ABC_RX_E_SELECTOR;
	}

	if (len < offset + 2) {
		return TRICE_ABC_RX_E_SHORT;
	}

	w = triceAbcReadU16(pBuf + offset); // nc
	offset += 2;

	abc.payloadBytes = (w & 0x8000u) ? (uint16_t)(w & 0x7FFFu) : (uint16_t)(w >> 8);

	if (len < offset + (int)abc.payloadBytes) {
		return TRICE_ABC_RX_E_SHORT;
	}

	if (abc.payloadBytes != 0u) {
		if (abc.bitWidth > 8u && (abc.payloadBytes & (uint16_t)((abc.bitWidth >> 3) - 1u)) != 0u) {
			return TRICE_ABC_RX_E_PAYLOAD;
		}
		abc.payload = pBuf + offset;
	}

	if (entry->fn == 0) {
		return TRICE_ABC_RX_E_HANDLER;
	}
	entry->fn(&abc);
	return offset + (int)abc.payloadBytes;
}

/*
//! TriceAbcOnReceive parses one decoded Trice record and calls the selected local handler directly.
int TriceAbcOnReceive(const uint8_t* pBuf, int len) {
	uint16_t firstWord;
	uint16_t id;
	uint16_t payloadBytes;
	uint16_t nc;
	uint8_t selector;
	uint8_t stampBits;
	uint32_t stamp = 0u;
	int offset = 2;
	int logicalLen;
	const uint8_t* payload;
	const triceAbc_t* entry;
	triceAbcRx_t abc;

	if (pBuf == 0 || len < 4) {
		return TRICE_ABC_RX_E_SHORT;
	}

	firstWord = triceAbcReadU16(pBuf);
	id = (uint16_t)(firstWord & 0x3FFFu);

	entry = triceAbcFind(id);
	if (entry == 0) {
		return TRICE_ABC_RX_IGNORED;
	}
	if (entry->fn == 0) {
		return TRICE_ABC_RX_E_HANDLER;
	}
	
	selector = (uint8_t)(firstWord >> 14);
	switch (selector) {
	case 1u:
		stampBits = 0u;
		break;
	case 2u:
		stampBits = 16u;
		#if TRICE_DOUBLED_16BIT_ID == 1
		if (len < 8) {
			return TRICE_ABC_RX_E_SHORT;
		}
		offset += 2;
		#endif // #if TRICE_DOUBLED_16BIT_ID == 1
		if (len < 6) {
			return TRICE_ABC_RX_E_SHORT;
		}
		stamp = (uint32_t)triceAbcReadU16(pBuf + offset);
		offset += 2;
		break;
	case 3u:
		stampBits = 32u;
		if (len < 8) {
			return TRICE_ABC_RX_E_SHORT;
		}
		stamp = triceAbcReadU32(pBuf + offset);
		offset += 4;
		break;
	default:
		return TRICE_ABC_RX_E_SELECTOR;
	}

	if (len < offset + 2) {
		return TRICE_ABC_RX_E_SHORT;
	}
	nc = triceAbcReadU16(pBuf + offset);
	offset += 2;
	if ((nc & 0x8000u) != 0u) {
		payloadBytes = (uint16_t)(nc & 0x7FFFu);
	} else {
		payloadBytes = (uint16_t)(nc >> 8);
	}

	logicalLen = offset + (int)payloadBytes;
	if (logicalLen > len) {
		return TRICE_ABC_RX_E_SHORT;
	}
    if (payloadBytes != 0u && entry->bitWidth > 8u &&
        (payloadBytes & ((entry->bitWidth >> 3) - 1u)) != 0u) {
        return TRICE_ABC_RX_E_PAYLOAD;
    }
	
	payload = payloadBytes == 0u ? 0 : (pBuf + offset);
	abc.id = id;
	abc.stampBits = stampBits;
	abc.bitWidth = entry->bitWidth;
	abc.stamp = stamp;
	abc.payload = payload;
	abc.payloadBytes = payloadBytes;
	entry->fn(&abc);
	return logicalLen;
}
*/
#endif // #if TRICE_ABC_RECEIVE_SUPPORT == 1
