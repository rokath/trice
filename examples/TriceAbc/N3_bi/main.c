// SPDX-License-Identifier: MIT

#include "../NodeLib/node.h"

#include <stdio.h>

/* Emit normal log traffic so N6_rx and N7_bi can show TIL-C based decoding. */
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

/* This node can request status and also demonstrate a divide request/response round trip. */
static void sendCommand(unsigned loop) {
    static const uint8_t key[] = { 'n', '3', '-', 'k', 'e', 'y' };
    uint32_t args[2];
    uint8_t leds = (uint8_t)(0x03u << (loop & 2u));

    switch (loop) {
    case 1u:
        triceC("cmd:getLeds");
        break;
    case 3u:
        trice8C("cmd:setLeds", &leds, 1);
        break;
    case 5u:
        trice8C("cmd:setKey", key, sizeof(key));
        break;
    case 6u:
        triceC("cmd:logState");
        break;
    case 7u:
        args[0] = aFloat(6.28f);
        args[1] = aFloat(2.0f);
        trice32C("cmd:divide", args, 2);
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

    for (loop = 0u; loop < 12u; ++loop) {
        (void)nodePoll(&node);
        sendTraffic(loop);
        sendCommand(loop);
        nodeSleepMs(170u);
    }

    for (loop = 0u; loop < 6u; ++loop) {
        (void)nodePoll(&node);
        nodeSleepMs(120u);
    }

    nodeClose(&node);
    return 0;
}
