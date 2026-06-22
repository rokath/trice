// SPDX-License-Identifier: MIT

#include "../NodeLib/node.h"

#include <stdio.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <time.h>
#endif

/* N6_rx also enables normal Trice log printing through the shared TIL-C table. */
void setLeds(const triceRx_t* rx) {
    nodeHandleSetLeds(rx);
}

void getLeds(const triceRx_t* rx) {
    nodeHandleGetLeds(rx, 0);
}

void setKey(const triceRx_t* rx) {
    nodeHandleSetKey(rx);
}

void logState(const triceRx_t* rx) {
    nodeHandleLogState(rx);
}

void divide(const triceRx_t* rx) {
    nodeHandleDivide(rx, 0);
}

void LedsState(const triceRx_t* rx) {
    nodeHandleLedsState(rx);
}

void DivideResult(const triceRx_t* rx) {
    nodeHandleDivideResult(rx);
}

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

int main(void) {
    node_t node;
    unsigned loop;

    if (nodeOpen(&node, "N6_rx", 0, 1, 1) < 0) {
        fprintf(stderr, "N6_rx: unable to open abc.bus\n");
        return 1;
    }

    for (loop = 0u; loop < 32u; ++loop) {
        (void)nodePoll(&node);
        sleepMs(100u);
    }

    nodeClose(&node);
    return 0;
}
