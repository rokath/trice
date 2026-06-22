// SPDX-License-Identifier: MIT

#include "../NodeLib/node.h"

#include <stdio.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <time.h>
#endif

/* Sleep helper keeps the demo pacing readable on every supported host OS. */
static void sleepMs(unsigned ms) {
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000u);
    ts.tv_nsec = (long)((ms % 1000u) * 1000000u);
    (void)nanosleep(&ts, 0);
#endif
}

/* Emit a mix of normal Trices and one counted selector-0 payload. */
static void sendTraffic(unsigned loop) {
    static const char text[] = "N1 says hi";
    uint8_t x0[6];
    unsigned i;

    trice("log:tick=%u\n", loop);
    trice("log:from=%u phase=%u\n", 1u, loop & 3u);
    triceS("log:text=%s\n", text);

    for (i = 0u; i < sizeof(x0); ++i) {
        x0[i] = (uint8_t)(0x10u + loop + i);
    }
    triceX0(x0, (uint16_t)(3u + (loop & 1u)));
}

/* Rotate through the broadcast commands that make sense for a TX-only node. */
static void sendCommand(unsigned loop) {
    static const uint8_t key[] = { 'a', 'l', 'p', 'h', 'a' };
    uint8_t leds = (uint8_t)(1u << (loop & 3u));

    switch (loop & 3u) {
    case 0u:
        trice8C("cmd:setLeds", &leds, 1);
        break;
    case 1u:
        triceC("cmd:getLeds");
        break;
    case 2u:
        triceC("cmd:logState");
        break;
    default:
        trice8C("cmd:setKey", key, sizeof(key));
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
        sendTraffic(loop);
        sendCommand(loop);
        sleepMs(180u);
    }

    nodeClose(&node);
    return 0;
}
