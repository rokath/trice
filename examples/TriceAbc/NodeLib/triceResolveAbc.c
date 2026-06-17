#include "triceResolveAbc.h"

static int triceSetBitWidth(triceRx_t* rx, uint8_t bitWidth) {
    if (rx->bitWidth == TRICE_BIT_WIDTH_UNKNOWN) {
        rx->bitWidth = bitWidth;
        return TRICE_RX_OK;
    }
    return (rx->bitWidth == bitWidth) ? TRICE_RX_OK : TRICE_RX_E_BIT_WIDTH_CONFLICT;
}

int triceResolveAbc(triceRx_t* rx, const triceAbcEntry_t* list, size_t count) {
    size_t i;
    int e;

    if (rx == 0 || list == 0) {
        return TRICE_RX_E_ARG;
    }

    for (i = 0u; i < count; ++i) {
        if (list[i].id == rx->id) {
            e = triceSetBitWidth(rx, list[i].bitWidth);
            if (e != TRICE_RX_OK) {
                return e;
            }
            rx->fn = list[i].fn;
            return TRICE_RX_OK;
        }
    }
    return TRICE_RX_E_NOT_FOUND;
}
