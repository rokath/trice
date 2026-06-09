// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceAbc.c
//! \brief Trice ABC receive runtime.

#include "triceAbcReceive.h"

#if TRICE_ABC_RECEIVE_SUPPORT == 1

//! triceAbcPayloadElementSize returns the receive handler element size for one table bit width.
static uint8_t triceAbcPayloadElementSize(uint8_t bitWidth) {
	switch (bitWidth) {
	case 0:
		return 0;
	case 8:
		return 1;
	case 16:
		return 2;
	case 32:
		return 4;
	case 64:
		return 8;
	default:
		return 0xffu;
	}
}

//! TriceAbcOnReceive validates one decoded ABC record and calls the selected local handler.
TriceAbcRxResult TriceAbcOnReceive(uint16_t id, uint8_t stampBits, uint32_t stamp, const uint8_t* payload, uint16_t payloadBytes) {
	const triceAbc_t* entry = 0;
	for (unsigned i = 0; i < triceAbcElements; i++) {
		if (triceAbc[i].id == id) {
			entry = &triceAbc[i];
			break;
		}
	}
	if (entry == 0) {
		return TRICE_ABC_RX_IGNORED;
	}
	if ((stampBits != 0u) && (stampBits != 16u) && (stampBits != 32u)) {
		return TRICE_ABC_RX_BAD_PAYLOAD;
	}
	if (entry->fn == 0) {
		return TRICE_ABC_RX_BAD_PAYLOAD;
	}

	uint8_t elementSize = triceAbcPayloadElementSize(entry->bitWidth);
	if (elementSize == 0xffu) {
		return TRICE_ABC_RX_BAD_PAYLOAD;
	}
	if (payloadBytes > 0u && payload == 0) {
		return TRICE_ABC_RX_BAD_PAYLOAD;
	}
	if (elementSize == 0u) {
		if (payloadBytes != 0u) {
			return TRICE_ABC_RX_BAD_PAYLOAD;
		}
	} else if ((payloadBytes % elementSize) != 0u) {
		return TRICE_ABC_RX_BAD_PAYLOAD;
	}

	triceAbcRx_t rx = { id, stampBits, entry->bitWidth, stamp, payload, payloadBytes };
	entry->fn(&rx);
	return TRICE_ABC_RX_EXECUTED;
}

#endif // #if TRICE_ABC_RECEIVE_SUPPORT == 1
