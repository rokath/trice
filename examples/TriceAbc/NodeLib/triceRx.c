#include "triceRx.h"

void triceRxInit(triceRx_t* rx) {
    if (rx == 0) {
        return;
    }
    rx->id = 0u;
    rx->bitWidth = TRICE_BIT_WIDTH_UNKNOWN;
    rx->stampBits = 0u;
    rx->stamp = 0u;
    rx->payload = 0;
    rx->payloadBytes = 0u;
#if TRICE_LOG_ENABLE == 1
    rx->pFmt = 0;
#endif
#if TRICE_LOG_WITH_LOCATION == 1
    rx->file = 0;
    rx->line = 0u;
#endif
    rx->fn = 0;
}
