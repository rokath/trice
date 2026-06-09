// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

#include "device_abc.h"

// triceAbcCall_rx_no_payload adapts the generated table callback to the local handler.
static void triceAbcCall_rx_no_payload(const triceAbcRx_t* rx) {
	rx_no_payload(rx);
}

// triceAbcCall_rx_i16 adapts the generated table callback to the local handler.
static void triceAbcCall_rx_i16(const triceAbcRx_t* rx) {
	rx_i16(rx);
}

// triceAbcCall_rx_i32 adapts the generated table callback to the local handler.
static void triceAbcCall_rx_i32(const triceAbcRx_t* rx) {
	rx_i32(rx);
}

// triceAbcCall_rx_i64 adapts the generated table callback to the local handler.
static void triceAbcCall_rx_i64(const triceAbcRx_t* rx) {
	rx_i64(rx);
}

// triceAbcCall_rx_nested adapts the generated table callback to the nested-call handler.
static void triceAbcCall_rx_nested(const triceAbcRx_t* rx) {
	rx_nested(rx);
}

//! triceAbc is the generated receive table consumed by TriceAbcOnReceive.
const triceAbc_t triceAbc[] = {
	{ 1001u, 0u, triceAbcCall_rx_no_payload },
	{ 1002u, 16u, triceAbcCall_rx_i16 },
	{ 1003u, 32u, triceAbcCall_rx_i32 },
	{ 1004u, 64u, triceAbcCall_rx_i64 },
	{ 1005u, 0u, triceAbcCall_rx_nested },
};

//! triceAbcElements is the generated receive table entry count.
const unsigned triceAbcElements = sizeof(triceAbc) / sizeof(triceAbc[0]);
