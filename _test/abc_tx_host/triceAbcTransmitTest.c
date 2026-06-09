// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

#include <string.h>

#include "trice.h"

// abcTxWords is the synthetic Trice write buffer used by the custom TRICE_ENTER/LEAVE hooks.
uint32_t abcTxWords[16];

// abcTxLength records the number of payload bytes written by one ABC macro call.
uint32_t abcTxLength;

// abcTxReset clears the synthetic buffer before one transmit macro fixture case.
static void abcTxReset(void) {
	memset(abcTxWords, 0xa5, sizeof(abcTxWords));
	abcTxLength = 0u;
}

// abcTxEquals compares the raw generated Trice bytes against one expected fixture.
static int abcTxEquals(const uint8_t* expected, uint32_t expectedLength) {
	return (abcTxLength == expectedLength) && (memcmp(abcTxWords, expected, expectedLength) == 0);
}

// TriceAbcTxHostCheck runs one transmit macro scenario and returns zero on success.
int TriceAbcTxHostCheck(int n) {
	abcTxReset();
	switch (n) {
	case 1: {
		static const uint8_t expected[] = { 0x34u, 0x52u, 0xc0u, 0x00u };
		triceC(iD(0x1234), "cmd:stop");
		return abcTxEquals(expected, sizeof(expected)) ? 0 : 1;
	}
	case 2: {
		static const uint8_t expected[] = { 0x23u, 0x81u, 0x23u, 0x81u, 0xcdu, 0xabu, 0xc0u, 0x00u };
		TriceC(iD(0x0123), "cmd:stop", 0xabcdu);
		return abcTxEquals(expected, sizeof(expected)) ? 0 : 2;
	}
	case 3: {
		static const uint8_t expected[] = { 0x23u, 0xc1u, 0x44u, 0x33u, 0x22u, 0x11u, 0xc0u, 0x00u };
		TRiceC(iD(0x0123), "cmd:stop", 0x11223344u);
		return abcTxEquals(expected, sizeof(expected)) ? 0 : 3;
	}
	case 4: {
		int8_t payload[] = { 1, 2, 3 };
		static const uint8_t expected[] = { 0x55u, 0x40u, 0xc0u, 0x03u, 0x01u, 0x02u, 0x03u, 0x00u };
		trice8C(iD(0x0055), "cmd:set_leds", payload, 3);
		return abcTxEquals(expected, sizeof(expected)) ? 0 : 4;
	}
	case 5: {
		int16_t payload[] = { 0x1122, 0x3344 };
		static const uint8_t expected[] = { 0x23u, 0x81u, 0x23u, 0x81u, 0xcdu, 0xabu, 0xc0u, 0x04u, 0x22u, 0x11u, 0x44u, 0x33u };
		Trice16C(iD(0x0123), "cmd:set_pwm", 0xabcdu, payload, 2);
		return abcTxEquals(expected, sizeof(expected)) ? 0 : 5;
	}
	case 6: {
		int32_t payload[] = { 0x11223344 };
		static const uint8_t expected[] = { 0x23u, 0xc1u, 0x04u, 0x03u, 0x02u, 0x01u, 0xc0u, 0x04u, 0x44u, 0x33u, 0x22u, 0x11u };
		TRice32C(iD(0x0123), "cmd:set_time", 0x01020304u, payload, 1);
		return abcTxEquals(expected, sizeof(expected)) ? 0 : 6;
	}
	default:
		return 99;
	}
}
