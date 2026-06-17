#ifndef TRICE_RESOLVE_LOG_H_
#define TRICE_RESOLVE_LOG_H_

#include "triceRx.h"
#include <stddef.h>
#include <stdint.h>

/* One generated or transformed log metadata entry. */
typedef struct {
    uint16_t id;
    uint8_t bitWidth;
#if TRICE_LOG_ENABLE == 1
    const char* pFmt;
#endif
} triceLogEntry_t;

/*
 * Resolve rx->id against a log metadata list.
 *
 * On success, the function fills rx->bitWidth and, when TRICE_LOG_ENABLE is 1,
 * rx->pFmt. It deliberately does not resolve file/line information; that is a
 * separate future resolver concern.
 */
int triceResolveLog(triceRx_t* rx, const triceLogEntry_t* list, size_t count);

#endif /* TRICE_RESOLVE_LOG_H_ */
