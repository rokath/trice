#ifndef TRICE_RESOLVE_ABC_H_
#define TRICE_RESOLVE_ABC_H_

#include "triceRx.h"
#include <stddef.h>
#include <stdint.h>

/* One generated ABC metadata entry. */
typedef struct {
    uint16_t id;
    uint8_t bitWidth;
    void (*fn)(const triceRx_t* rx);
} triceAbcEntry_t;

/*
 * Resolve rx->id against a generated ABC metadata list.
 *
 * On success, the function fills rx->bitWidth and rx->fn and returns
 * TRICE_RX_OK. If rx->bitWidth was already resolved by another resolver, the
 * new bit width must match or TRICE_RX_E_BIT_WIDTH_CONFLICT is returned.
 */
int triceResolveAbc(triceRx_t* rx, const triceAbcEntry_t* list, size_t count);

#endif /* TRICE_RESOLVE_ABC_H_ */
