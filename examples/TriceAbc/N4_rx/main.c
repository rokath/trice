// SPDX-License-Identifier: MIT

#include "../NodeLib/node.h"

#include <stdio.h>

int main(void) {
    node_t node;
    unsigned loop;

    if (nodeOpen(&node, "N4_rx", 0, 1, 0) < 0) {
        fprintf(stderr, "N4_rx: unable to open abc.bus\n");
        return 1;
    }

    for (loop = 0u; loop < 30u; ++loop) {
        (void)nodePoll(&node);
        nodeSleepMs(110u);
    }

    nodeClose(&node);
    return 0;
}
