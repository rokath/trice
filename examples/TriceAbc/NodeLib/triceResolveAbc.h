#ifndef TRICE_RESOLVE_ABC_H_
#define TRICE_RESOLVE_ABC_H_

/*
 * triceResolveAbc.h
 *
 * Metadata resolver for generated command-handler tables.
 *
 * triceParseNextRecord() can only parse data that is present in the record byte
 * stream. The payload element width and the local handler function are not part
 * of that byte stream; they come from generated node-specific metadata. This
 * resolver attaches that metadata to a triceRx_t object by looking up rx->id.
 */

#include "triceRx.h"
#include <stddef.h>
#include <stdint.h>

/* One generated command metadata entry. */
typedef struct {
    uint16_t id;      /* Record ID this generated table entry belongs to. */
    uint8_t bitWidth; /* Payload element width associated with the local handler. */
    void (*fn)(const triceRx_t* rx); /* Local handler function, or NULL if unused. */
} triceAbcEntry_t;

/*
 * Resolve rx->id against a generated metadata list.
 *
 * list/count describe a normal C array generated for one node. A count parameter
 * is used instead of a sentinel entry so the generator does not need to reserve
 * any special ID value.
 *
 * On success, the function fills rx->bitWidth and rx->fn and returns
 * TRICE_RX_OK. If rx->bitWidth was already resolved by another resolver, the
 * new bit width must match or TRICE_RX_E_BIT_WIDTH_CONFLICT is returned.
 */
int triceResolveAbc(triceRx_t* rx, const triceAbcEntry_t* list, size_t count);

#endif /* TRICE_RESOLVE_ABC_H_ */
