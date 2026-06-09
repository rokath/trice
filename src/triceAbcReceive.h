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

#ifdef __cplusplus
extern "C" {
#endif

#if TRICE_ABC_RECEIVE_SUPPORT == 1

//! triceAbcRx_t describes one already decoded ABC record during handler execution.
typedef struct {
	uint16_t id;
	uint8_t stampBits;
	uint8_t bitWidth;
	uint32_t stamp;
	const uint8_t* payload;
	uint16_t payloadBytes;
} triceAbcRx_t;

//! triceAbcHandler_t is the uniform generated ABC table wrapper type.
typedef void (*triceAbcHandler_t)(const triceAbcRx_t* rx);

//! triceAbc_t maps one Trice ID to one locally selected ABC handler wrapper.
typedef struct {
	uint16_t id;
	uint8_t bitWidth;
	triceAbcHandler_t fn;
} triceAbc_t;

//! TriceAbcRxResult reports whether a decoded Trice ABC command was executed.
typedef enum {
	TRICE_ABC_RX_IGNORED = 0,
	TRICE_ABC_RX_EXECUTED = 1,
	TRICE_ABC_RX_BAD_PAYLOAD = 2,
} TriceAbcRxResult;

extern const triceAbc_t triceAbc[];
extern const unsigned triceAbcElements;

//! TriceAbcOnReceive validates one decoded ABC record and calls the selected local handler.
TriceAbcRxResult TriceAbcOnReceive(uint16_t id, uint8_t stampBits, uint32_t stamp, const uint8_t* payload, uint16_t payloadBytes);

#endif // #if TRICE_ABC_RECEIVE_SUPPORT == 1

#ifdef __cplusplus
}
#endif

#endif /* TRICE_ABC_RECEIVE_H_ */
