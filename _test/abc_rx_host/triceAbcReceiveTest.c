// TRICE_INSERT_OFF - Trice parser exclusion marker
// This is a test file. It must not get the Trice IDs changed!

#include <string.h>

#include "device_abc.h"

// triceLog is a small generated-table stand-in for testing log resolution
// without adding another generated fixture file.
const triceLog_t triceLog[] = {
	{ 2001u, TRICE_DEFAULT_PARAMETER_BIT_WIDTH, 1u, "default:%d" },
	{ 2002u, 0u, 0u, "cmd:no-payload" },
	{ 2003u, 16u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "cmd:%d" },
};

// triceLogElements mirrors the generated element-count symbol used by runtime wrappers.
const unsigned triceLogElements = sizeof(triceLog) / sizeof(triceLog[0]);

// rxCheckCase selects the expected assertions for the current fixture call.
static int rxCheckCase;

// rxCalls counts executed handlers so validation failures can distinguish ignore from dispatch.
static int rxCalls;

// rxFailures accumulates C-side assertion failures and is returned to Go.
static int rxFailures;

//! rxPutU16 writes one 16-bit value in configured transfer order.
static void rxPutU16(uint8_t* p, uint16_t value) {
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
	p[0] = (uint8_t)(value >> 8);
	p[1] = (uint8_t)value;
#else
	p[0] = (uint8_t)value;
	p[1] = (uint8_t)(value >> 8);
#endif
}

//! rxPutU32 writes one 32-bit value in configured transfer order.
static void rxPutU32(uint8_t* p, uint32_t value) {
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
	rxPutU16(p, (uint16_t)(value >> 16));
	rxPutU16(p + 2, (uint16_t)value);
#else
	rxPutU16(p, (uint16_t)value);
	rxPutU16(p + 2, (uint16_t)(value >> 16));
#endif
}

//! rxBuildAbcRecord writes one decoded ABC record into pBuf and optionally appends zero padding up to the next 32-bit boundary.
static int rxBuildAbcRecord(uint8_t* pBuf, uint16_t id, uint8_t stampBits, uint32_t stamp, const void* payload, uint16_t payloadBytes, int appendPadding) {
	uint16_t head;
	uint16_t nc;
	int offset = 0;
	if (stampBits == 0u) {
		head = (uint16_t)(0x4000u | id);
	} else if (stampBits == 16u) {
		head = (uint16_t)(0x8000u | id);
	} else {
		head = (uint16_t)(0xC000u | id);
	}
	rxPutU16(pBuf + offset, head);
	offset += 2;
	if (stampBits == 16u) {
		rxPutU16(pBuf + offset, (uint16_t)stamp);
		offset += 2;
	} else if (stampBits == 32u) {
		rxPutU32(pBuf + offset, stamp);
		offset += 4;
	}
	if (payloadBytes <= 127u) {
		nc = (uint16_t)((payloadBytes << 8) | 0x00c0u);
	} else {
		nc = (uint16_t)(0x8000u | payloadBytes);
	}
	rxPutU16(pBuf + offset, nc);
	offset += 2;
	if (payloadBytes > 0u) {
		memcpy(pBuf + offset, payload, payloadBytes);
		offset += payloadBytes;
	}
	if (appendPadding) {
		int alignedLen = (offset + 3) & ~3;
		memset(pBuf + offset, 0, (size_t)(alignedLen - offset));
		offset = alignedLen;
	}
	return offset;
}

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
void rx_no_payload(const triceRx_t* rx) {
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
	} else if (rxCheckCase == 15) {
		rxFailUnless(rx->stampBits == 0u);
		rxFailUnless(rx->stamp == 0u);
	} else {
		rxFailures++;
	}
}

// rx_i8_bulk verifies the long-count path and byte-granular payload handling.
void rx_i8_bulk(const triceRx_t* rx) {
	rxCalls++;
	rxFailUnless(rxCheckCase == 10);
	rxFailUnless(rx->id == 1006u);
	rxFailUnless(rx->stampBits == 32u);
	rxFailUnless(rx->stamp == 0x55667788u);
	rxFailUnless(rx->bitWidth == 8u);
	rxFailUnless(rx->payloadBytes == 130u);
	rxFailUnless(rx->payload != 0);
	rxFailUnless(rx->payload[0] == 0u);
	rxFailUnless(rx->payload[64] == 64u);
	rxFailUnless(rx->payload[129] == 129u);
}

// rx_i16 verifies byte-oriented payload delivery from an intentionally unaligned input pointer.
void rx_i16(const triceRx_t* rx) {
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
void rx_i32(const triceRx_t* rx) {
	int32_t value;
	memcpy(&value, rx->payload, sizeof(value));
	rxCalls++;
	rxFailUnless(rxCheckCase == 3 || rxCheckCase == 15);
	rxFailUnless(rx->stampBits == 0u);
	rxFailUnless(rx->bitWidth == 32u);
	rxFailUnless(rx->payloadBytes == sizeof(value));
	rxFailUnless(value == 0x11223344);
}

// rx_i64 verifies the widest first-version ABC payload without typed pointer casts.
void rx_i64(const triceRx_t* rx) {
	int64_t value;
	memcpy(&value, rx->payload, sizeof(value));
	rxCalls++;
	rxFailUnless(rxCheckCase == 4);
	rxFailUnless(rx->bitWidth == 64u);
	rxFailUnless(rx->payloadBytes == sizeof(value));
	rxFailUnless(value == (int64_t)0x1122334455667788ll);
}

// rx_nested verifies that nested dispatch does not mutate the outer context object.
void rx_nested(const triceRx_t* rx) {
	uint8_t nested[8];
	int used;
	rxCalls++;
	rxFailUnless(rxCheckCase == 9);
	rxFailUnless(rx->stampBits == 32u);
	rxFailUnless(rx->stamp == 0x11111111u);
	used = rxBuildAbcRecord(nested, 1001u, 16u, 0x2222u, 0, 0u, 1);
	rxFailUnless(TriceAbcOnReceive(nested, used) == 6);
	rxFailUnless(rx->stampBits == 32u);
	rxFailUnless(rx->stamp == 0x11111111u);
}

// TriceAbcRxHostCheck runs one receive-runtime scenario and returns the failure count.
int TriceAbcRxHostCheck(int n) {
	int result;
	rxReset(n);
	switch (n) {
	case 1: { // Selected no-payload ABC with 16-bit stamp returns logical length without padding.
		uint8_t record[8];
		int used = rxBuildAbcRecord(record, 1001u, 16u, 0x0000beefu, 0, 0u, 1);
		result = TriceAbcOnReceive(record, used);
		rxFailUnless(result == 6);
		rxFailUnless(rxCalls == 1);
		break;
	}
	case 2: { // Selected int16 payload ABC with 32-bit stamp dispatches one aligned payload record.
		int16_t payload[2] = { 123, -456 };
		uint8_t record[16];
		int used = rxBuildAbcRecord(record, 1002u, 32u, 0x12345678u, payload, (uint16_t)sizeof(payload), 0);
		result = TriceAbcOnReceive(record, used);
		rxFailUnless(result == used);
		rxFailUnless(rxCalls == 1);
		break;
	}
	case 3: { // Selected int32 payload ABC without stamp dispatches one no-stamp record.
		int32_t payload[1] = { 0x11223344 };
		uint8_t record[8];
		int used = rxBuildAbcRecord(record, 1003u, 0u, 0u, payload, (uint16_t)sizeof(payload), 0);
		result = TriceAbcOnReceive(record, used);
		rxFailUnless(result == used);
		rxFailUnless(rxCalls == 1);
		break;
	}
	case 4: { // Selected int64 payload ABC with 16-bit stamp returns logical length without padding.
		int64_t payload[1] = { (int64_t)0x1122334455667788ll };
		uint8_t record[16];
		int used = rxBuildAbcRecord(record, 1004u, 16u, 0x8877u, payload, (uint16_t)sizeof(payload), 1);
		result = TriceAbcOnReceive(record, used);
		rxFailUnless(result == 14);
		rxFailUnless(rxCalls == 1);
		break;
	}
	case 5: { // Unknown ABC ID is parsed but ignored without executing a handler.
		uint8_t record[8];
		int used = rxBuildAbcRecord(record, 9999u, 32u, 0x01020304u, 0, 0u, 1);
		result = TriceAbcOnReceive(record, used);
		rxFailUnless(result == 8);
		rxFailUnless(rxCalls == 0);
		break;
	}
	case 6: { // Selector 0 is rejected as payload error when counted typeX0 support is disabled.
		uint8_t record[4];
		rxPutU16(record, 0x0001u);
		rxPutU16(record + 2, 0x00c0u);
		result = TriceAbcOnReceive(record, 4);
		rxFailUnless(result == TRICE_RX_E_PAYLOAD);
		rxFailUnless(rxCalls == 0);
		break;
	}
	case 7: { // Misaligned int32 payload size is rejected before handler execution.
		uint8_t payload[2] = { 1u, 2u };
		uint8_t record[8];
		int used = rxBuildAbcRecord(record, 1003u, 0u, 0u, payload, (uint16_t)sizeof(payload), 0);
		result = TriceAbcOnReceive(record, used);
		rxFailUnless(result == TRICE_RX_E_PAYLOAD);
		rxFailUnless(rxCalls == 0);
		break;
	}
	case 8: { // Truncated selected record is rejected before handler execution.
		int16_t payload[2] = { 123, -456 };
		uint8_t record[16];
		int used = rxBuildAbcRecord(record, 1002u, 32u, 0x12345678u, payload, (uint16_t)sizeof(payload), 0);
		result = TriceAbcOnReceive(record, used - 1);
		rxFailUnless(result == TRICE_RX_E_SHORT);
		rxFailUnless(rxCalls == 0);
		break;
	}
	case 9: { // Nested ABC dispatch executes inner and outer handlers without corrupting outer context.
		uint8_t record[8];
		int used = rxBuildAbcRecord(record, 1005u, 32u, 0x11111111u, 0, 0u, 0);
		result = TriceAbcOnReceive(record, used);
		rxFailUnless(result == used);
		rxFailUnless(rxCalls == 2);
		break;
	}
	case 10: { // Long-count int8 payload dispatch returns logical length without appended padding.
		uint8_t payload[130];
		uint8_t record[140];
		int used;
		for (int i = 0; i < (int)sizeof(payload); i++) {
			payload[i] = (uint8_t)i;
		}
		used = rxBuildAbcRecord(record, 1006u, 32u, 0x55667788u, payload, (uint16_t)sizeof(payload), 1);
		result = TriceAbcOnReceive(record, used);
		rxFailUnless(result == 138);
		rxFailUnless(rxCalls == 1);
		break;
	}
	case 11: { // Log resolver applies the configured default bit width to no-width log names.
		int32_t payload[1] = { 0x01020304 };
		uint8_t record[8];
		triceRx_t rx;
		int used = rxBuildAbcRecord(record, 2001u, 0u, 0u, payload, (uint16_t)sizeof(payload), 0);
		result = TriceParseNextRecord(&rx, record, (size_t)used);
		rxFailUnless(result == used);
		rxFailUnless(rx.bitWidth == TRICE_BIT_WIDTH_UNKNOWN);
		rxFailUnless(rx.paramCount == 0u);
		result = triceResolveLog(&rx, triceLog, (size_t)triceLogElements);
		rxFailUnless(result == TRICE_RX_OK);
		rxFailUnless(rx.bitWidth == TRICE_DEFAULT_PARAMETER_BIT_WIDTH);
		rxFailUnless(rx.paramCount == 1u);
		rxFailUnless(rx.pFmt == triceLog[0].pFmt);
		rxFailUnless(rxCalls == 0);
		break;
	}
	case 12: { // Log resolver keeps no-width ABC C names payload-free instead of using the default width.
		uint8_t record[4];
		triceRx_t rx;
		int used = rxBuildAbcRecord(record, 2002u, 0u, 0u, 0, 0u, 0);
		result = TriceParseNextRecord(&rx, record, (size_t)used);
		rxFailUnless(result == used);
		result = triceResolveLog(&rx, triceLog, (size_t)triceLogElements);
		rxFailUnless(result == TRICE_RX_OK);
		rxFailUnless(rx.bitWidth == 0u);
		rxFailUnless(rx.paramCount == 0u);
		rxFailUnless(rx.payload == 0);
		rxFailUnless(rx.payloadBytes == 0u);
		result = triceValidateLogPayload(&rx);
		rxFailUnless(result == TRICE_RX_E_UNSUPPORTED);
		rxFailUnless(rxCalls == 0);
		break;
	}
	case 13: { // Explicit log bit widths still win over the no-width default for resolved metadata.
		int16_t payload[2] = { 11, 22 };
		uint8_t record[8];
		triceRx_t rx;
		int used = rxBuildAbcRecord(record, 2003u, 0u, 0u, payload, (uint16_t)sizeof(payload), 0);
		result = TriceParseNextRecord(&rx, record, (size_t)used);
		rxFailUnless(result == used);
		rxFailUnless(rx.paramCount == 0u);
		result = triceResolveLog(&rx, triceLog, (size_t)triceLogElements);
		rxFailUnless(result == TRICE_RX_OK);
		rxFailUnless(rx.bitWidth == 16u);
		rxFailUnless(rx.paramCount == TRICE_LOG_PARAM_COUNT_DYNAMIC);
		rxFailUnless(rx.payloadBytes == sizeof(payload));
		result = triceValidateLogPayload(&rx);
		rxFailUnless(result == TRICE_RX_E_UNSUPPORTED);
		rxFailUnless(rxCalls == 0);
		break;
	}
	case 14: { // TriceLogOnReceive consumes known and unknown log IDs without dispatch side effects.
		int32_t payload[1] = { 0x55667788 };
		uint8_t record[8];
		int used = rxBuildAbcRecord(record, 2001u, 0u, 0u, payload, (uint16_t)sizeof(payload), 0);
		result = TriceLogOnReceive(record, used);
		rxFailUnless(result == used);
		used = rxBuildAbcRecord(record, 2999u, 0u, 0u, 0, 0u, 0);
		result = TriceLogOnReceive(record, used);
		rxFailUnless(result == used);
		result = TriceLogOnReceive(record, used - 1);
		rxFailUnless(result == TRICE_RX_E_SHORT);
		used = rxBuildAbcRecord(record, 2001u, 0u, 0u, 0, 0u, 0);
		result = TriceLogOnReceive(record, used);
		rxFailUnless(result == TRICE_RX_E_PAYLOAD);
		rxFailUnless(rxCalls == 0);
		break;
	}
	case 15: { // A caller advances through a stream because TriceAbcOnReceive consumes only one record.
		int32_t payload[1] = { 0x11223344 };
		uint8_t stream[16];
		int firstLen;
		int secondLen;
		int streamLen;

		firstLen = rxBuildAbcRecord(stream, 1001u, 0u, 0u, 0, 0u, 0);
		secondLen = rxBuildAbcRecord(stream + firstLen, 1003u, 0u, 0u, payload, (uint16_t)sizeof(payload), 0);
		streamLen = firstLen + secondLen;

		result = TriceAbcOnReceive(stream, streamLen);
		rxFailUnless(result == firstLen);
		rxFailUnless(rxCalls == 1);

		result = TriceAbcOnReceive(stream + firstLen, streamLen - firstLen);
		rxFailUnless(result == secondLen);
		rxFailUnless(rxCalls == 2);
		break;
	}
	default:
		rxFailures++;
		break;
	}
	return rxFailures;
}
