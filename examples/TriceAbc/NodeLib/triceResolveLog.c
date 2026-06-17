#include "triceResolveLog.h"

/*
 * Assign or verify payload element bit width.
 *
 * This helper intentionally mirrors the one in triceResolveAbc.c. Keeping the
 * resolvers independent lets either file be used without linking the other.
 */
static int triceSetBitWidth(triceRx_t* rx, uint8_t bitWidth) {
    if (rx->bitWidth == TRICE_BIT_WIDTH_UNKNOWN) {
        rx->bitWidth = bitWidth;
        return TRICE_RX_OK;
    }
    return (rx->bitWidth == bitWidth) ? TRICE_RX_OK : TRICE_RX_E_BIT_WIDTH_CONFLICT;
}

int triceResolveLog(triceRx_t* rx, const triceLogEntry_t* list, size_t count) {
    size_t i;
    int e;

    if (rx == 0 || list == 0) {
        return TRICE_RX_E_ARG;
    }

    /* Linear search keeps generated metadata easy to read in this prepared demo. */
    for (i = 0u; i < count; ++i) {
        if (list[i].id == rx->id) {
            e = triceSetBitWidth(rx, list[i].bitWidth);
            if (e != TRICE_RX_OK) {
                return e;
            }
#if TRICE_LOG_ENABLE == 1
            rx->pFmt = list[i].pFmt;
#endif
            return TRICE_RX_OK;
        }
    }
    return TRICE_RX_E_NOT_FOUND;
}
