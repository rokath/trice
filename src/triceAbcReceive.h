// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceAbcReceive.h
//! \brief Trice ABC receive declarations independent from normal Trice output.

#ifndef TRICE_ABC_RECEIVE_H_
#define TRICE_ABC_RECEIVE_H_

#include <stdint.h>

#include "triceConfig.h"

#ifndef TRICE_ABC_RECEIVE_SUPPORT
#define TRICE_ABC_RECEIVE_SUPPORT 0
#endif

#ifndef TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN
#define TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN 0
#endif

#ifndef TRICE_DOUBLED_16BIT_ID
#define TRICE_DOUBLED_16BIT_ID 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if TRICE_ABC_RECEIVE_SUPPORT == 1

//! triceAbcRx_t describes one decoded ABC record during handler execution.
typedef struct {
	uint16_t id;
	uint8_t stampBits;
	uint8_t bitWidth;
	uint32_t stamp;
	const uint8_t* payload;
	uint16_t payloadBytes;
} triceAbcRx_t;

//! triceAbcHandler_t is the uniform generated ABC table callback type.
typedef void (*triceAbcHandler_t)(const triceAbcRx_t* rx);

//! triceAbc_t maps one Trice ID to one locally selected ABC handler.
typedef struct {
	uint16_t id;
	uint8_t bitWidth;
	triceAbcHandler_t fn;
} triceAbc_t;

enum {
	TRICE_ABC_RX_IGNORED = 0,
	TRICE_ABC_RX_EXECUTED = 1,
	TRICE_ABC_RX_E_SHORT = -1,
	TRICE_ABC_RX_E_SELECTOR = -2,
	TRICE_ABC_RX_E_BIT_WIDTH = -3,
	TRICE_ABC_RX_E_PAYLOAD = -4,
	TRICE_ABC_RX_E_HANDLER = -5,
};

extern const triceAbc_t triceAbc[];
extern const unsigned triceAbcElements;

//! TriceAbcOnReceive parses one decoded Trice record, dispatches selected ABC handlers, and returns the consumed byte count or a status/error value.
int TriceAbcOnReceive(const uint8_t* pBuf, int len);

#endif // #if TRICE_ABC_RECEIVE_SUPPORT == 1

#ifdef __cplusplus
}
#endif

#endif /* TRICE_ABC_RECEIVE_H_ */
