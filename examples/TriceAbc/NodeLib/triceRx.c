#include "triceRx.h"

/*
 * Initialize a receive record object.
 *
 * This function is deliberately small and deterministic so it can be used at
 * the beginning of every parse operation. It leaves no stale metadata from a
 * previous parse/resolve cycle in the same triceRx_t object.
 */
void triceRxInit(triceRx_t* rx) {
    if (rx == 0) {
        return;
    }

    /* Values parsed directly from the record byte stream. */
    rx->id = 0u;
    rx->stampBits = 0u;
    rx->stamp = 0u;
    rx->payload = 0;
    rx->payloadBytes = 0u;

    /* Values resolved later from generated or transformed metadata tables. */
    rx->bitWidth = TRICE_BIT_WIDTH_UNKNOWN;
#if TRICE_LOG_ENABLE == 1
    rx->pFmt = 0;
#endif
#if TRICE_LOG_WITH_LOCATION == 1
    rx->file = 0;
    rx->line = 0u;
#endif
    rx->fn = 0;
}
