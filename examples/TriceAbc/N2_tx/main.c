// SPDX-License-Identifier: MIT

#include "../NodeLib/node.h"

#include <stdio.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <time.h>
#endif

/* A small delay between bursts keeps the multi-process output readable. */
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

/* This node emits a second visible pattern so the shared log stream is varied. */
static void sendTraffic(unsigned loop) {
    static const char text[] = "N2 sends data";
    uint8_t x0[7];
    unsigned i;

    trice("log:tick=%u\n", 100u + loop);
    trice("log:from=%u phase=%u\n", 2u, (loop + 1u) & 3u);
    triceS("log:text=%s\n", text);

    for (i = 0u; i < sizeof(x0); ++i) {
        x0[i] = (uint8_t)(0x20u + (i * 2u) + loop);
    }
    triceX0(x0, (uint16_t)(4u + (loop & 1u)));
}

/* The TX-only nodes never send divide requests because they cannot receive answers. */
static void sendCommand(unsigned loop) {
    static const uint8_t key[] = { 'b', 'r', 'a', 'v', 'o', '7' };
    uint8_t leds = (uint8_t)(0x80u >> (loop & 3u));

    switch (loop & 3u) {
    case 0u:
        trice8C("cmd:setLeds", &leds, 1);
        break;
    case 1u:
        triceC("cmd:logState");
        break;
    case 2u:
        trice8C("cmd:setKey", key, sizeof(key));
        break;
    default:
        triceC("cmd:getLeds");
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
        sleepMs(240u);
    }

    nodeClose(&node);
    return 0;
}
