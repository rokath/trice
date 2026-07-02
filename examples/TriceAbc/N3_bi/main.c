// SPDX-License-Identifier: MIT
#include "../NodeLib/node.h"
#include <stdio.h>

// Emit normal log traffic so N6_rx can show TIL-C based decoding.
static void sendTraffic(unsigned loop) {
	static const char text[] = "N3 bidirectional";
	uint8_t x0[5];
	uint16_t x0Len = (uint16_t)(2u + (loop & 1u));
	unsigned i;

	/* Show the local TX side because BcSim suppresses self echo on receive. */

	trice("log:(from node=N3_bi) tick=%u\n", 200u + loop);
	// Demo-only TX trace: make ABC replies visible at the sender as well.
	nodePrintLineF("N3_bi: TX-> log:tick=%u\n", 200u + loop);

	trice("log:(from node=N3_bi) from=%u phase=%u\n", 3u, loop & 3u);
	// Demo-only TX trace: make ABC replies visible at the sender as well.
	nodePrintLineF("N3_bi: TX-> log:from=%u phase=%u\n", 3u, loop & 3u);

	triceS("log:(from node=N3_bi) text=%s\n", text);
	// Demo-only TX trace: make ABC replies visible at the sender as well.
	nodePrintLineF("N3_bi: TX-> log:text=%s\n", text);

	for (i = 0u; i < sizeof(x0); ++i) {
		x0[i] = (uint8_t)(0x30u + loop + i);
	}

	triceX0(x0, x0Len);
	// Demo-only TX trace: make ABC replies visible at the sender as well.
	nodePrintLineF("N3_bi: TX-> x0 %u bytes\n", (unsigned)x0Len);
}

// This node sends both broadcast and stamp-routed ABC requests.
//
// The low 32-bit stamp bits are used as a responder bitmap in this demo:
// 0x00000001 -> N7_bi
// 0x00000002 -> N8_bi
// 0x00000004 -> N9_bi
static void sendCommand(unsigned loop) {
	static const uint8_t key[] = { 'n', '3', '-', 'k', 'e', 'y' };
	uint32_t args[2];
	uint8_t leds = (uint8_t)(0x03u << (loop & 2u));

	/* Show the local TX side because BcSim suppresses self echo on receive. */
	
	switch (loop) {
	case 1u:
		TriceC("cmd:getLeds", 103);
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N3_tx: ABC-> cmd:getLeds\n");
		break;

	case 3u:
		Trice8C("cmd:setLeds", 103, &leds, 1);
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N3_tx: ABC-> cmd:setLeds(%02x)\n", leds);
		break;

	case 5u:
		Trice8C("cmd:setKey", 103, key, sizeof(key));
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N3_tx: ABC-> cmd:setKey(n3-key)\n");
		break;

	case 6u:
		TriceC("cmd:logState", 103);
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N3_tx: ABC-> cmd:logState\n");
		break;

	case 7u:
		args[0] = aFloat(6.28f);
		args[1] = aFloat(2.0f);
		Trice32C("cmd:divide", 103, args, 2);
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N3_tx: ABC-> cmd:divide(6.28)(2.0)\n");
		break;

	case 8u:
		TRiceC("cmd:getLeds", 103);
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N3_tx: ABC-> cmd:getLeds(stamp=0x00000001)\n");
		break;

	case 9u:
		TRiceC("cmd:getLeds", 103);
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N3_tx: ABC-> cmd:getLeds(stamp=0x00000002)\n");
		break;

	case 10u:
		args[0] = aFloat(9.0f);
		args[1] = aFloat(3.0f);
		Trice32C("cmd:divide", 103, args, 2);
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N3_tx: ABC-> cmd:divide(stamp=0x00000005)(9.0)(3.0)\n");
		break;

	case 11u:
		TRiceC("cmd:getLeds", 103);
		// Demo-only TX trace: make ABC replies visible at the sender as well.
		nodePrintLineF("N3_tx: ABC-> cmd:getLeds(stamp=0x00000003)\n");
		break;

	default:
		break;
	}
}

int main(void) {
	node_t node;
	unsigned loop;

	if (nodeOpen(&node, "N3_bi", 1, 1, 0) < 0) {
		fprintf(stderr, "N3_bi: unable to open abc.bus\n");
		return 1;
	}

	node.replyStampMask = 0u;

	for (loop = 0u; loop < 12u; ++loop) {
		(void)nodePoll(&node);
		sendTraffic(loop); // N3_bi
		sendCommand(loop); // stamp=103
		nodeSleepMs(170u);
	}

	for (loop = 0u; loop < 10u; ++loop) {
		(void)nodePoll(&node);
		nodeSleepMs(120u);
	}

	nodeClose(&node);
	return 0;
}
