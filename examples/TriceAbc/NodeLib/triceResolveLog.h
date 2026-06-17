#ifndef TRICE_RESOLVE_LOG_H_
#define TRICE_RESOLVE_LOG_H_

/*
 * triceResolveLog.h
 *
 * Metadata resolver for log-format information.
 *
 * This resolver is independent from triceResolveAbc(). A node can use either
 * resolver alone or both resolvers together. When both are used, their bit width
 * information must agree.
 */

#include "triceRx.h"
#include <stddef.h>
#include <stdint.h>

/* One generated or transformed log metadata entry. */
typedef struct {
    uint16_t id;      /* Record ID this log metadata entry describes. */
    uint8_t bitWidth; /* Payload element width derived from log metadata. */
#if TRICE_LOG_ENABLE == 1
    const char* pFmt; /* Optional format string pointer for log rendering. */
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
