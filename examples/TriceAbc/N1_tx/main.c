// SPDX-License-Identifier: MIT
#include "../NodeLib/node.h"
#include <stdio.h>

/* Emit a mix of normal Trices and one counted selector-0 payload. */
static void sendTraffic(unsigned loop) {
	static const char text[] = "N1 says hi";
	uint8_t x0[6];
	uint16_t x0Len = (uint16_t)(3u + (loop & 1u));
	unsigned i;

	/* Show the local TX side because BcSim suppresses self echo on receive. */

	trice("log:node=N1_tx, tick=%u\n", loop);
    // Demo-only TX trace: make ABC replies visible at the sender as well.
	nodePrintLineF("N1_tx: TX-> log:tick=%u\n", loop);

	trice("log:node=N1_tx, from=%u phase=%u\n", 1u, loop & 3u);
    // Demo-only TX trace: make ABC replies visible at the sender as well.
	nodePrintLineF("N1_tx: TX-> log:from=%u phase=%u\n", 1u, loop & 3u);

	triceS("log:text=%s\n", text);
    // Demo-only TX trace: make ABC replies visible at the sender as well.
	nodePrintLineF("N1_tx: TX-> log:text=%s\n", text);

	for (i = 0u; i < sizeof(x0); ++i) {
		x0[i] = (uint8_t)(0x10u + loop + i);
	}

	triceX0(x0, x0Len);
    // Demo-only TX trace: make ABC replies visible at the sender as well.
	nodePrintLineF("N1_tx: TX-> x0 %u bytes\n", (unsigned)x0Len);
}

/* Rotate through the broadcast commands that make sense for a TX-only node. */
static void sendCommand(unsigned loop) {
	static const uint8_t key[] = { 'a', 'l', 'p', 'h', 'a' };
	uint8_t leds = (uint8_t)(1u << (loop & 3u));

    /* Show the local TX side because BcSim suppresses self echo on receive. */

	switch (loop & 3u) {
	case 0u:
		Trice8C("cmd:setLeds", 101, &leds, 1);
        // Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N1_tx: ABC-> cmd:setLeds(%02x)\n", leds);
		break;

	case 1u:
		TriceC("cmd:getLeds", 101);
        // Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N1_tx: ABC-> cmd:getLeds\n");
		break;

	case 2u:
		TriceC("cmd:logState", 101);
        // Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N1_tx: ABC-> cmd:logState\n");
		break;

	default:
		Trice8C("cmd:setKey", 101, key, sizeof(key));
        // Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N1_tx: ABC-> cmd:setKey(alpha)\n");
		break;
	}
}

int main(void) {
	node_t node;
	unsigned loop;

	if (nodeOpen(&node, "N1_tx", 1, 0, 0) < 0) {
		fprintf(stderr, "N1_tx: unable to open abc.bus\n");
		return 1;
	}

	for (loop = 0u; loop < 8u; ++loop) {
		sendTraffic(loop); // stamp=101
		sendCommand(loop); // stamp=101
		nodeSleepMs(180u);
	}

	nodeClose(&node);
	return 0;
}
