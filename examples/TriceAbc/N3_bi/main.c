// SPDX-License-Identifier: MIT

#include "../NodeLib/node.h"

#include <stdio.h>

// Emit normal log traffic so N6_rx can show TIL-C based decoding.
static void sendTraffic(unsigned loop) {
    static const char text[] = "N3 bidirectional";
    uint8_t x0[5];
    unsigned i;

    trice("log:tick=%u\n", 200u + loop);
    trice("log:from=%u phase=%u\n", 3u, loop & 3u);
    triceS("log:text=%s\n", text);

    for (i = 0u; i < sizeof(x0); ++i) {
        x0[i] = (uint8_t)(0x30u + loop + i);
    }
    triceX0(x0, (uint16_t)(2u + (loop & 1u)));
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

    switch (loop) {
    case 1u:
        triceC("cmd:getLeds");
        nodePrintLineF("N3_tx: ABC-> cmd:getLeds\n"); // log
        break;
    case 3u:
        trice8C("cmd:setLeds", &leds, 1);
        nodePrintLineF("N3_tx: ABC-> cmd:setLeds(%02x)\n", leds);
        break;
    case 5u:
        trice8C("cmd:setKey", key, sizeof(key));
        nodePrintLineF("N3_tx: ABC-> cmd:setKey(n3-key)\n");
        break;
    case 6u:
        triceC("cmd:logState");
        nodePrintLineF("N3_tx: ABC-> cmd:logState\n");
        break;
    case 7u:
        args[0] = aFloat(6.28f);
        args[1] = aFloat(2.0f);
        trice32C("cmd:divide", args, 2);
        nodePrintLineF("N3_tx: ABC-> cmd:divide(6.28)(2.0)\n");
        break;
    case 8u:
        TRiceC("cmd:getLeds", 0x00000001u);
        nodePrintLineF("N3_tx: ABC-> cmd:getLeds(with 32-bit stamp)\n");
        break;
    case 9u:
        TRiceC("cmd:getLeds", 0x00000002u);
        nodePrintLineF("N3_tx: ABC-> cmd:getLeds(with 32-bit stamp)\n");
        break;
    case 10u:
        args[0] = aFloat(9.0f);
        args[1] = aFloat(3.0f);
        TRice32C("cmd:divide", 0x00000005u, args, 2);
        nodePrintLineF("N3_tx: ABC-> cmd:divide(with 32-bit stamp)(9.0)(3.0\n", leds);
        break;
    case 11u:
        TRiceC("cmd:getLeds", 0x00000003u);
        nodePrintLineF("N3_tx: ABC-> cmd:getLeds(with 32-bit stampx)\n");
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
        sendTraffic(loop);
        sendCommand(loop);
        nodeSleepMs(170u);
    }

    for (loop = 0u; loop < 10u; ++loop) {
        (void)nodePoll(&node);
        nodeSleepMs(120u);
    }

    nodeClose(&node);
    return 0;
}
