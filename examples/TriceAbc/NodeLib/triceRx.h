#ifndef TRICE_RX_H_
#define TRICE_RX_H_

/*
 * triceRx.h
 *
 * Common receive-side record structure for the TriceAbc example nodes.
 *
 * The structure is intentionally broader than the information parsed from raw
 * bytes. triceParseNextRecord() fills only fields that are physically present in
 * the record byte stream. Resolver functions can then add generated metadata,
 * such as bit width, handler function pointer, or an optional format string.
 */

#include <stddef.h>
#include <stdint.h>

/*
 * Enable log-format metadata fields in triceRx_t.
 *
 * Default is 0 because the initial TriceAbc node examples focus on command
 * routing and do not need formatted log output inside the node process.
 */
#ifndef TRICE_RX_LOG_ENABLE
#define TRICE_RX_LOG_ENABLE 0
#endif

/*
 * Enable source-location metadata fields in triceRx_t.
 *
 * This is intentionally separate from TRICE_RX_LOG_ENABLE. A future location
 * resolver may use a location index file to fill file and line information.
 */
#ifndef TRICE_LOG_WITH_LOCATION
#define TRICE_LOG_WITH_LOCATION 0
#endif

/*
 * Sentinel value for an unresolved bit width.
 *
 * Normal bit widths are small values such as 0, 8, 16, 32, or 64. The value
 * 0xff is therefore available as a clear "not resolved yet" marker without an
 * extra validity flag.
 */
#define TRICE_BIT_WIDTH_UNKNOWN ((uint8_t)0xffu)

/*
 * Common return codes for the lightweight node helper layer.
 *
 * Parse functions return consumed byte counts on success, so all errors are
 * negative. Resolver functions return TRICE_RX_OK on success and a negative
 * value on failure.
 */
enum {
    TRICE_RX_OK = 0,                       /* Generic success value. */
    TRICE_RX_E_ARG = -1,                   /* Invalid pointer or argument. */
    TRICE_RX_E_SHORT = -2,                 /* More input bytes are needed. */
    TRICE_RX_E_UNSUPPORTED = -3,           /* Record shape is not supported by this small parser. */
    TRICE_RX_E_NOT_FOUND = -4,             /* The ID was not found in a resolver table. */
    TRICE_RX_E_BIT_WIDTH_CONFLICT = -5,    /* Two resolvers disagree about bit width. */
    TRICE_RX_E_RECORD = -6                 /* Generic malformed-record error for later use. */
};

/*
 * triceRx_t describes one received record after parsing and optional metadata
 * resolution.
 *
 * The typedef and the struct tag intentionally use the same spelling. Inside
 * the struct definition, the function pointer refers to 'struct triceRx_t'
 * because the typedef name is not complete until the closing brace.
 */
typedef struct triceRx_t {
    uint16_t id;       /* Record ID parsed from the record header. */
    uint8_t bitWidth;  /* Payload element width. 0xff means not resolved yet. */
    uint8_t stampBits; /* 0, 16, or 32 depending on the record stamp variant. */
    uint32_t stamp;    /* Parsed stamp value. Zero when stampBits is zero. */

    /*
     * Pointer to payload bytes inside the interpreted input buffer.
     *
     * No payload is copied. This saves RAM and is suitable for immediate handler
     * execution. A delayed handler must copy the payload before the input buffer
     * is reused or released.
     */
    const uint8_t* payload;

    /* Number of payload bytes available at payload. Zero means no payload. */
    uint16_t payloadBytes;

#if TRICE_RX_LOG_ENABLE == 1
    const char* pTrice; /* Optional Trice name string pointer filled by triceResolveLog(). */    
    const char* pFmt; /* Optional format string pointer filled by triceResolveLog(). */
#endif

#if TRICE_LOG_WITH_LOCATION == 1
    /* Optional source file name filled by a future location resolver. */
    const char* file;

    /* Optional source line number filled by a future location resolver. */
    uint32_t line;
#endif

    /* Optional handler function pointer filled by triceResolveAbc(). */
    void (*fn)(const struct triceRx_t* rx);
} triceRx_t;

/* Initialize all fields to safe defaults and bitWidth to TRICE_BIT_WIDTH_UNKNOWN. */
void triceRxInit(triceRx_t* rx);

#endif /* TRICE_RX_H_ */
