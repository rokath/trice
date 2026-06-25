// SPDX-License-Identifier: MIT
#include "../NodeLib/node.h"
#include <stdio.h>

/* This node emits a second visible pattern so the shared log stream is varied. */
static void sendTraffic(unsigned loop) {
	static const char text[] = "N2 sends data";
	uint8_t x0[7];
	uint16_t x0Len = (uint16_t)(4u + (loop & 1u));
	unsigned i;

	/* Show the local TX side because BcSim suppresses self echo on receive. */

	trice("log:tick=%u\n", 100u + loop);
	// Demo-only TX trace: make ABC replies visible at the sender as well.
	nodePrintLineF("N2_tx: TX-> log:tick=%u\n", 100u + loop);

	trice("log:from=%u phase=%u\n", 2u, (loop + 1u) & 3u);
	// Demo-only TX trace: make ABC replies visible at the sender as well.
	nodePrintLineF("N2_tx: TX-> log:from=%u phase=%u\n", 2u, (loop + 1u) & 3u);

	triceS("log:text=%s\n", text);
	// Demo-only TX trace: make ABC replies visible at the sender as well.
	nodePrintLineF("N2_tx: TX-> log:text=%s\n", text);

	for (i = 0u; i < sizeof(x0); ++i) {
		x0[i] = (uint8_t)(0x20u + (i * 2u) + loop);
	}

	triceX0(x0, x0Len);
	// Demo-only TX trace: make ABC replies visible at the sender as well.
	nodePrintLineF("N2_tx: TX-> x0 %u bytes\n", (unsigned)x0Len);
}

/* The TX-only nodes never send divide requests because they cannot receive answers. */
static void sendCommand(unsigned loop) {
	static const uint8_t key[] = { 'b', 'r', 'a', 'v', 'o', '7' };
	uint8_t leds = (uint8_t)(0x80u >> (loop & 3u));

	/* Show the local TX side because BcSim suppresses self echo on receive. */
	
	switch (loop & 3u) {
	case 0u:
		trice8C("cmd:setLeds", &leds, 1);
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N2_tx: ABC-> cmd:setLeds(%02x)\n", leds);
		break;

	case 1u:
		triceC("cmd:logState");
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N2_tx: ABC-> cmd:logState\n");
		break;

	case 2u:
		trice8C("cmd:setKey", key, sizeof(key));
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N2_tx: ABC-> cmd:setKey(bravo7)\n");
		break;

	default:
		triceC("cmd:getLeds");
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N2_tx: ABC-> cmd:getLeds\n");
		break;
	}
}

int main(void) {
	node_t node;
	unsigned loop;

	if (nodeOpen(&node, "N2_tx", 1, 0, 0) < 0) {
		fprintf(stderr, "N2_tx: unable to open abc.bus\n");
		return 1;
	}

	for (loop = 0u; loop < 8u; ++loop) {
		sendTraffic(loop);
		sendCommand(loop);
		nodeSleepMs(240u);
	}

	nodeClose(&node);
	return 0;
}
