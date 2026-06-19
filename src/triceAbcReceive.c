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
//! It returns the used or not used logical byte count or a negative error value.
//! If an entry in triceAbc[] is executed, is not visible in this function signature.
//! The return values are only used for buffer parsing, to make it easy to parse a byte stream.
int TriceAbcOnReceive(const uint8_t* pBuf, int len) {
	triceAbcRx_t abc = {0};
	const triceAbc_t* entry;
	uint16_t w;
	int offset = 2;

	if (pBuf == 0 || len < 4) {
		return TRICE_ABC_RX_E_SHORT;
	}

	w = triceAbcReadU16(pBuf); // selector & id
	abc.id = (uint16_t)(w & 0x3FFFu);

	switch (w >> 14) { // selector bits
	case 1u: // no stamp
		// abc.stampBits is 0u
		// abc.stamp is 0u
		break;

	case 2u: // 16-bit stamp
		abc.stampBits = 16u;
#if TRICE_DOUBLED_16BIT_ID == 1
		offset += 2;
#endif
		if (len < offset + 2) {
			return TRICE_ABC_RX_E_SHORT;
		}
		abc.stamp = triceAbcReadU16(pBuf + offset);
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

	default: // only 0 possible
#if TRICE_TX_X0_COUNTED_BUFFER_SUPPORT == 1
		return w; // a counted typeX0 packet
#else
		return TRICE_ABC_RX_E_PAYLOAD;
#endif
	}

	if (len < offset + 2) {
		return TRICE_ABC_RX_E_SHORT;
	}

	w = triceAbcReadU16(pBuf + offset); // nc
	offset += 2;

	// No cycle counter check here, to keep it simple.
	abc.payloadBytes = (w & 0x8000u) ? (uint16_t)(w & 0x7FFFu) : (uint16_t)(w >> 8);

	if (len < offset + (int)abc.payloadBytes) {
		return TRICE_ABC_RX_E_SHORT;
	}

	// abc.payload is 0
	if (abc.payloadBytes != 0u) {
		abc.payload = pBuf + offset;
	}

	entry = triceAbcFind(abc.id);
	if (entry != 0 && entry->fn != 0) {
		abc.bitWidth = entry->bitWidth;

		if (abc.payloadBytes != 0u) {
			// Payload with bitWidth 0 is invalid. No-payload records may use bitWidth 0.
			if (abc.bitWidth == 0u) {
				return TRICE_ABC_RX_E_PAYLOAD;
			}
		
			// Multi-byte payload elements must be complete elements.
			if (abc.bitWidth > 8u &&
			    (abc.payloadBytes & (uint16_t)((abc.bitWidth >> 3) - 1u)) != 0u) {
				return TRICE_ABC_RX_E_PAYLOAD;
			}
		}

		entry->fn(&abc);
	}

	return offset + (int)abc.payloadBytes;
}

#endif // #if TRICE_ABC_RECEIVE_SUPPORT == 1
