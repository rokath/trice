// SPDX-License-Identifier: MIT

#include "../NodeLib/node.h"

#include <stdio.h>

int main(void) {
    node_t node;
    unsigned loop;

    if (nodeOpen(&node, "N6_rx", 0, 1, 1) < 0) {
        fprintf(stderr, "N6_rx: unable to open abc.bus\n");
        return 1;
    }

    for (loop = 0u; loop < 32u; ++loop) {
        (void)nodePoll(&node);
        nodeSleepMs(100u);
    }

    nodeClose(&node);
    return 0;
}
