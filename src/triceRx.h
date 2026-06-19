// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceRx.h
//! \brief Common Trice receive record parser and resolver API.

#ifndef TRICE_RX_H_
#define TRICE_RX_H_

#include "triceConfig.h"
#include "triceRxDefaultConfig.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// TRICE_BIT_WIDTH_UNKNOWN marks records whose payload element width is not
// known from the byte stream itself. Resolvers may replace it with 0, 8, 16,
// 32, or 64 when generated metadata identifies the record.
#define TRICE_BIT_WIDTH_UNKNOWN ((uint8_t)0xffu)

// TRICE_STAMP_BITS_UNKNOWN marks selector-0 records, because they are not
// normal Trice records and do not carry a normal stamp selector.
#define TRICE_STAMP_BITS_UNKNOWN ((uint8_t)0xffu)

// Parser functions return a consumed byte count on success. Errors are negative
// so callers can distinguish "record consumed" from "need a policy decision".
enum {
	TRICE_RX_OK = 0,
	TRICE_RX_E_ARG = -1,
	TRICE_RX_E_SHORT = -2,
	TRICE_RX_E_UNSUPPORTED = -3,
	TRICE_RX_E_NOT_FOUND = -4,
	TRICE_RX_E_BIT_WIDTH_CONFLICT = -5,
	TRICE_RX_E_RECORD = -6
};

typedef struct triceRx_t triceRx_t;

// triceFn_t is the generated ABC handler signature. The receive record is
// const during handler execution so nested dispatch cannot mutate the caller's
// current record by accident.
typedef void (*triceFn_t)(const triceRx_t* rx);

// triceRx_t describes one parsed receive record plus optional resolved metadata.
// payload points into the caller-owned input buffer; no payload bytes are copied.
struct triceRx_t {
	uint16_t id;
	uint8_t bitWidth;
	uint8_t stampBits;
	uint32_t stamp;
	const uint8_t* payload;
	uint16_t payloadBytes;
	uint8_t cycleCounter;

#if TRICE_RX_ABC_SUPPORT == 1
	triceFn_t fn;
#endif

#if TRICE_RX_LOG_SUPPORT == 1
	const char* pTrice;
	const char* pFmt;
#if TRICE_LOCATION_SUPPORT == 1
	const char* file;
	uint32_t line;
#endif
#endif
};

// triceRxInit clears a receive record and sets unresolved metadata sentinels.
void triceRxInit(triceRx_t* rx);

// triceParseNextRecord parses exactly one already deframed/decrypted Trice
// record at buf[0]. It returns the logical record length and never consumes
// alignment padding after the record.
int triceParseNextRecord(triceRx_t* rx, const uint8_t* buf, size_t len);

#if TRICE_RX_ABC_SUPPORT == 1

// triceAbcEntry_t is the generated ABC table entry type. The field order keeps
// existing generated positional initializers compatible: { id, bitWidth, fn }.
typedef struct {
	uint16_t id;
	uint8_t bitWidth;
	triceFn_t fn;
} triceAbcEntry_t;

// Keep the established ABC receive names in the generic RX header. They are
// type aliases, not a separate receive implementation.
typedef triceRx_t triceAbcRx_t;
typedef triceFn_t triceAbcHandler_t;
typedef triceAbcEntry_t triceAbc_t;

enum {
	TRICE_ABC_RX_E_SHORT = -1,
	TRICE_ABC_RX_E_PAYLOAD = -2
};

extern const triceAbc_t triceAbc[];
extern const unsigned triceAbcElements;

// triceResolveAbc attaches generated ABC metadata to a parsed record.
int triceResolveAbc(triceRx_t* rx, const triceAbcEntry_t* list, size_t count);

// triceDispatchAbc validates a resolved ABC record and calls rx->fn when set.
int triceDispatchAbc(const triceRx_t* rx);

// TriceAbcOnReceive is the ABC convenience entry point built from the generic
// parser, generated ABC resolver table, and ABC dispatcher.
int TriceAbcOnReceive(const uint8_t* pBuf, int len);

#endif // #if TRICE_RX_ABC_SUPPORT == 1

#if TRICE_RX_LOG_SUPPORT == 1

// triceLogEntry_t is a future log metadata table entry derived from TIL data.
typedef struct {
	const char* pTrice;
	const char* pFmt;
	uint16_t id;
} triceLogEntry_t;

// triceResolveLog attaches log rendering metadata to a parsed record.
int triceResolveLog(triceRx_t* rx, const triceLogEntry_t* list, size_t count);

#if TRICE_LOCATION_SUPPORT == 1

// triceLocationEntry_t is a future location metadata table entry derived from
// LI data. The resolver is separate because location data is optional even when
// log formatting is present.
typedef struct {
	const char* file;
	uint32_t line;
	uint16_t id;
} triceLocationEntry_t;

// triceResolveLocation attaches source file and line metadata to a record.
int triceResolveLocation(triceRx_t* rx, const triceLocationEntry_t* list, size_t count);

#endif // #if TRICE_LOCATION_SUPPORT == 1

// triceDispatchLog is a placeholder for a future C log formatter entry point.
int triceDispatchLog(const triceRx_t* rx);

#endif // #if TRICE_RX_LOG_SUPPORT == 1

#ifdef __cplusplus
}
#endif

#endif // TRICE_RX_H_
