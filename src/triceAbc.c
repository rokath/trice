// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceAbcReceive.c
//! \brief Trice ABC receive runtime.

#include "triceAbcReceive.h"

#if TRICE_ABC_RECEIVE_SUPPORT == 1

//! triceAbcReadU16 reads one 16-bit transfer-order value from p.
static uint16_t triceAbcReadU16(const uint8_t* p) {
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
	return (uint16_t)(((uint16_t)p[0] << 8) | (uint16_t)p[1]);
#else
	return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
#endif
}

//! triceAbcReadU32 reads one 32-bit transfer-order value from p.
static uint32_t triceAbcReadU32(const uint8_t* p) {
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
	return ((uint32_t)triceAbcReadU16(p) << 16) | (uint32_t)triceAbcReadU16(p + 2);
#else
	return (uint32_t)triceAbcReadU16(p) | ((uint32_t)triceAbcReadU16(p + 2) << 16);
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

#endif // #if TRICE_ABC_RECEIVE_SUPPORT == 1
