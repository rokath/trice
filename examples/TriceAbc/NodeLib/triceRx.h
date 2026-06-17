#ifndef TRICE_RX_H_
#define TRICE_RX_H_

#include <stddef.h>
#include <stdint.h>

#ifndef TRICE_LOG_ENABLE
#define TRICE_LOG_ENABLE 0
#endif

#ifndef TRICE_LOG_WITH_LOCATION
#define TRICE_LOG_WITH_LOCATION 0
#endif

#define TRICE_BIT_WIDTH_UNKNOWN ((uint8_t)0xffu)

/* Common negative return codes for the lightweight node helper layer. */
enum {
    TRICE_RX_OK = 0,
    TRICE_RX_E_ARG = -1,
    TRICE_RX_E_SHORT = -2,
    TRICE_RX_E_UNSUPPORTED = -3,
    TRICE_RX_E_NOT_FOUND = -4,
    TRICE_RX_E_BIT_WIDTH_CONFLICT = -5,
    TRICE_RX_E_RECORD = -6
};

/*
 * triceRx_t describes one received Trice record after parsing and optional
 * metadata resolution.
 *
 * triceParseNextRecord() fills only fields that are present in the byte record:
 * id, stampBits, stamp, payload, and payloadBytes. It initializes bitWidth to
 * TRICE_BIT_WIDTH_UNKNOWN and fn to NULL.
 *
 * Resolver functions may later fill bitWidth, fn, and optionally pFmt. Location
 * data is intentionally a separate future resolver concern.
 *
 * payload points into the interpreted input buffer. Immediate handlers may use
 * it directly. Delayed handlers must copy the payload if they need it later.
 */
typedef struct triceRx_t {
    uint16_t id;
    uint8_t bitWidth;
    uint8_t stampBits;
    uint32_t stamp;

    const uint8_t* payload;
    uint16_t payloadBytes;

#if TRICE_LOG_ENABLE == 1
    const char* pFmt;
#endif

#if TRICE_LOG_WITH_LOCATION == 1
    const char* file;
    uint32_t line;
#endif

    void (*fn)(const struct triceRx_t* rx);
} triceRx_t;

void triceRxInit(triceRx_t* rx);

#endif /* TRICE_RX_H_ */
