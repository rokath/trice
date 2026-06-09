// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

#include <string.h>

#include "device_abc.h"

// rxCheckCase selects the expected assertions for the current fixture call.
static int rxCheckCase;

// rxCalls counts executed handlers so validation failures can distinguish ignore from dispatch.
static int rxCalls;

// rxFailures accumulates C-side assertion failures and is returned to Go.
static int rxFailures;

// rxReset initializes one receive-runtime fixture case.
static void rxReset(int checkCase) {
	rxCheckCase = checkCase;
	rxCalls = 0;
	rxFailures = 0;
}

// rxFailUnless records one failed C-side assertion without aborting handler execution.
static void rxFailUnless(int condition) {
	if (!condition) {
		rxFailures++;
	}
}

// rx_no_payload verifies no-payload dispatch and direct stamp context delivery.
void rx_no_payload(const triceAbcRx_t* rx) {
	rxCalls++;
	rxFailUnless(rx->id == 1001u);
	rxFailUnless(rx->bitWidth == 0u);
	rxFailUnless(rx->payload == 0);
	rxFailUnless(rx->payloadBytes == 0u);
	if (rxCheckCase == 1) {
		rxFailUnless(rx->stampBits == 16u);
		rxFailUnless((uint16_t)rx->stamp == 0xbeefu);
		rxFailUnless(rx->stamp == 0x0000beefu);
	} else if (rxCheckCase == 9) {
		rxFailUnless(rx->stampBits == 16u);
		rxFailUnless((uint16_t)rx->stamp == 0x2222u);
	} else {
		rxFailures++;
	}
}

// rx_i16 verifies byte-oriented payload delivery from an intentionally unaligned input pointer.
void rx_i16(const triceAbcRx_t* rx) {
	int16_t values[2];
	memcpy(values, rx->payload, sizeof(values));
	rxCalls++;
	rxFailUnless(rxCheckCase == 2);
	rxFailUnless(rx->id == 1002u);
	rxFailUnless(rx->stampBits == 32u);
	rxFailUnless(rx->stamp == 0x12345678u);
	rxFailUnless(rx->bitWidth == 16u);
	rxFailUnless(rx->payloadBytes == sizeof(values));
	rxFailUnless(values[0] == 123);
	rxFailUnless(values[1] == -456);
}

// rx_i32 verifies int32_t payload bytes and no-stamp context.
void rx_i32(const triceAbcRx_t* rx) {
	int32_t value;
	memcpy(&value, rx->payload, sizeof(value));
	rxCalls++;
	rxFailUnless(rxCheckCase == 3);
	rxFailUnless(rx->stampBits == 0u);
	rxFailUnless(rx->bitWidth == 32u);
	rxFailUnless(rx->payloadBytes == sizeof(value));
	rxFailUnless(value == 0x11223344);
}

// rx_i64 verifies the widest first-version ABC payload without typed pointer casts.
void rx_i64(const triceAbcRx_t* rx) {
	int64_t value;
	memcpy(&value, rx->payload, sizeof(value));
	rxCalls++;
	rxFailUnless(rxCheckCase == 4);
	rxFailUnless(rx->bitWidth == 64u);
	rxFailUnless(rx->payloadBytes == sizeof(value));
	rxFailUnless(value == (int64_t)0x1122334455667788ll);
}

// rx_nested verifies that nested dispatch does not mutate the outer context object.
void rx_nested(const triceAbcRx_t* rx) {
	rxCalls++;
	rxFailUnless(rxCheckCase == 9);
	rxFailUnless(rx->stampBits == 32u);
	rxFailUnless(rx->stamp == 0x11111111u);
	rxFailUnless(TriceAbcOnReceive(1001u, 16u, 0x2222u, 0, 0u) == TRICE_ABC_RX_EXECUTED);
	rxFailUnless(rx->stampBits == 32u);
	rxFailUnless(rx->stamp == 0x11111111u);
}

// TriceAbcRxHostCheck runs one receive-runtime scenario and returns the failure count.
int TriceAbcRxHostCheck(int n) {
	TriceAbcRxResult result;
	rxReset(n);
	switch (n) {
	case 1:
		result = TriceAbcOnReceive(1001u, 16u, 0x0000beefu, 0, 0u);
		rxFailUnless(result == TRICE_ABC_RX_EXECUTED);
		rxFailUnless(rxCalls == 1);
		break;
	case 2: {
		int16_t payload[2] = { 123, -456 };
		uint8_t raw[sizeof(payload) + 1u];
		memcpy(raw + 1u, payload, sizeof(payload));
		result = TriceAbcOnReceive(1002u, 32u, 0x12345678u, raw + 1u, (uint16_t)sizeof(payload));
		rxFailUnless(result == TRICE_ABC_RX_EXECUTED);
		rxFailUnless(rxCalls == 1);
		break;
	}
	case 3: {
		int32_t payload[1] = { 0x11223344 };
		result = TriceAbcOnReceive(1003u, 0u, 0u, (const uint8_t*)payload, (uint16_t)sizeof(payload));
		rxFailUnless(result == TRICE_ABC_RX_EXECUTED);
		rxFailUnless(rxCalls == 1);
		break;
	}
	case 4: {
		int64_t payload[1] = { (int64_t)0x1122334455667788ll };
		result = TriceAbcOnReceive(1004u, 16u, 0x8877u, (const uint8_t*)payload, (uint16_t)sizeof(payload));
		rxFailUnless(result == TRICE_ABC_RX_EXECUTED);
		rxFailUnless(rxCalls == 1);
		break;
	}
	case 5:
		result = TriceAbcOnReceive(9999u, 99u, 0u, 0, 0u);
		rxFailUnless(result == TRICE_ABC_RX_IGNORED);
		rxFailUnless(rxCalls == 0);
		break;
	case 6:
		result = TriceAbcOnReceive(1001u, 7u, 0u, 0, 0u);
		rxFailUnless(result == TRICE_ABC_RX_BAD_PAYLOAD);
		rxFailUnless(rxCalls == 0);
		break;
	case 7: {
		uint8_t payload[2] = { 1u, 2u };
		result = TriceAbcOnReceive(1003u, 0u, 0u, payload, (uint16_t)sizeof(payload));
		rxFailUnless(result == TRICE_ABC_RX_BAD_PAYLOAD);
		rxFailUnless(rxCalls == 0);
		break;
	}
	case 8:
		result = TriceAbcOnReceive(1002u, 0u, 0u, 0, 2u);
		rxFailUnless(result == TRICE_ABC_RX_BAD_PAYLOAD);
		rxFailUnless(rxCalls == 0);
		break;
	case 9:
		result = TriceAbcOnReceive(1005u, 32u, 0x11111111u, 0, 0u);
		rxFailUnless(result == TRICE_ABC_RX_EXECUTED);
		rxFailUnless(rxCalls == 2);
		break;
	default:
		rxFailures++;
		break;
	}
	return rxFailures;
}
