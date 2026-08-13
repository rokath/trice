// SPDX-License-Identifier: MIT

#include "../NodeLib/node.h"

#include <stdio.h>

int main(void) {
	node_t node;
	unsigned loop;

	if (nodeOpen(&node, "N7_bi", 1, 1, 1) < 0) {
		fprintf(stderr, "N7_bi: unable to open abc.bus\n");
		return 1;
	}
	node.replyStampMask = 0x0001u;

	for (loop = 0u; loop < 34u; ++loop) {
		(void)nodePoll(&node);
		nodeSleepMs(100u);
	}

	nodeClose(&node);
	return 0;
}
