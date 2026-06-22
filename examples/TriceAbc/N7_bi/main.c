// SPDX-License-Identifier: MIT

#include "../NodeLib/node.h"

#include <stdio.h>

/* N7_bi also emits some own traffic so the log printer sees mixed origins. */
static void sendTraffic(unsigned loop) {
    static const char text[] = "N7 observer";
    uint8_t x0[4];
    unsigned i;

    trice("log:tick=%u\n", 300u + loop);
    trice("log:from=%u phase=%u\n", 7u, (loop + 2u) & 3u);
    triceS("log:text=%s\n", text);

    for (i = 0u; i < sizeof(x0); ++i) {
        x0[i] = (uint8_t)(0x70u + loop + i);
    }
    triceX0(x0, (uint16_t)sizeof(x0));
}

int main(void) {
    node_t node;
    unsigned loop;

    if (nodeOpen(&node, "N7_bi", 1, 1, 1) < 0) {
        fprintf(stderr, "N7_bi: unable to open abc.bus\n");
        return 1;
    }

    for (loop = 0u; loop < 12u; ++loop) {
        (void)nodePoll(&node);
        if ((loop & 3u) == 2u) {
            sendTraffic(loop);
        }
        nodeSleepMs(140u);
    }

    for (loop = 0u; loop < 8u; ++loop) {
        (void)nodePoll(&node);
        nodeSleepMs(120u);
    }

    nodeClose(&node);
    return 0;
}
