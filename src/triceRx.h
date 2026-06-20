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
	TRICE_RX_E_SHORT = 0, // de-facto tells that no bytes where consumed, so the caller can decide to wait for more data or skip the record.
	TRICE_RX_E_ARG = -1,
	TRICE_RX_E_PAYLOAD = -2,
	TRICE_RX_E_UNSUPPORTED = -3,
	TRICE_RX_E_NOT_FOUND = -4,
	TRICE_RX_E_BIT_WIDTH_CONFLICT = -5,
	TRICE_RX_E_RECORD = -6
};

// triceRx_t describes one parsed receive record plus optional resolved metadata.
// payload points into the caller-owned input buffer; no payload bytes are copied.
// It is a struct which gets filled during trice rx.
typedef struct triceRx_t {
    uint16_t id;            // Trice id
    uint8_t  bitWidth;      // payload bithWidth, 0, 8, 16, 32, 64 or 0xffu for unknown. Resolved from triceAbc[] list or til.json.
    uint8_t  stampBits;     //  `0`, `16`, or `32` valid stamp bits or 0xffu for typeX0 Trices.
    uint32_t stamp;         // usually timestamp for Trice messages, any stamp for Trice ABC messages
    const uint8_t* payload; // points into the input buffer
    uint16_t payloadBytes;  // byte count or count typeX0 Trice payload.
    uint8_t cycleCounter;   // optional part of the nc field

#if TRICE_RX_ABC_SUPPORT == 1
	void (*fn)(const triceRx_t* rx); // abc function handler resolved from generated triceAbc[]. ( triceFn_t fn; )
#endif

#if TRICE_RX_LOG_SUPPORT == 1
    const char* pTrice; // Pointer to used Trice macro name.
    const char* pFmt;   // Trice format string resolved from til.json.
#if TRICE_LOCATION_SUPPORT == 1
    const char* file; // `file` is name where the Trice statements was used. Resolved from li.json.
    uint32_t line;    // Source code line in `file` where the Trice statements was used. Resolved from li.json.
#endif
#endif
} triceRx_t;

// triceFn_t is the generated ABC handler signature. The receive record is
// const during handler execution so nested dispatch cannot mutate the caller's
// current record by accident.
typedef void (*triceFn_t)(const triceRx_t* rx);

// triceRxInit clears a receive record and sets unresolved metadata sentinels.
static inline void triceRxInit(triceRx_t* rx) {
	memset(rx, 0, sizeof(*rx));
	rx->bitWidth = TRICE_BIT_WIDTH_UNKNOWN;
}

//! \brief parses exactly one already deframed/decrypted Trice record at buf[0].
//! \details
//! It returns the logical record length and never consumes alignment padding after the record.
//! It does not parse multiple records, does not deframe, does not decrypt and does not use a TIL table. 
//! It expects a plain trice binary stream according to the [Trice Binary Encoding](./TriceUserManual.md#binary-encoding)
//! 
//! \li It fills:
//! \li   rx->id
//! \li   rx->stampBits
//! \li   rx->stamp
//! \li   rx->payload - Important: `payload` points into `buf`; no payload copy is made.
//! \li   rx->payloadBytes
//! \li   rx->cycleCounter
//! \li It initializes or leaves as unknown:
//! \li   rx->bitWidth = TRICE_BIT_WIDTH_UNKNOWN
//! \li   rx->fn = NULL, if present
//! \li   rx->pTrice = NULL, if present
//! \li   rx->pFmt = NULL, if present
//! \li   rx->file = NULL, if present
//! \li   rx->line = 0, if present
//! 
//! \retval >= 0  number of consumed bytes for the next record
//! \retval <  0  negative error code
int triceParseNextRecord(triceRx_t* rx, const uint8_t* buf, size_t len);

#if TRICE_RX_ABC_SUPPORT == 1

//! \brief triceAbc_t is the generated ABC table entry type. 
//! \details The field order keeps existing generated positional initializers compatible: { id, bitWidth, fn }.
//! \li `triceAbc_t` is the type for generated triceAbc[] list.
//! \li ABC metadata should be generated from `trice generate -abc path/target` and should contain ID, bit width, and function pointer.
typedef struct {
    uint16_t id;       // Trice id
    uint8_t  bitWidth; // payload bithWidth, 0, 8, 16, 32, 64 or 0xffu for unknown. Resolved from triceAbc[] list or til.json.
	triceFn_t fn;      // abc function handler resolved from generated triceAbc[]. 
} triceAbc_t;

extern const triceAbc_t triceAbc[]; // generated ABC resolver table, defined in <device>_abc.c
extern const unsigned triceAbcElements;

//! triceResolveAbc attaches generated ABC metadata to a parsed record.
//! \details Resolve ABC metadata
//! \li The resolver searches `list[0..count-1]` for `rx->id`. If found, it fills:
//! \li    rx->bitWidth
//! \li    rx->fn
//! 
//! If `rx->bitWidth` is still `TRICE_BIT_WIDTH_UNKNOWN`, the resolver sets it. 
//! If it is already set and differs from the table entry, the resolver returns a bit-width conflict error.
//! 
//! \retval If not found, it should return a non-fatal “not found” code. Unknown IDs are normal in mixed streams.
int triceResolveAbc(triceRx_t* rx, const triceAbc_t* list, size_t count);

//! \brief triceDispatchAbc validates a resolved ABC record and calls rx->fn when set.
//! A small helper can dispatch resolved ABC records.
//! Semantics:
//! \li if `rx == NULL`: error,
//! \li if `rx->fn == NULL`: ignored/not selected,
//! \li if `rx->bitWidth == 0xffu`: error
//! \li if payload length is incompatible with `rx->bitWidth`: error,
//! \li otherwise call `rx->fn(rx)` and return success.
int triceDispatchAbc(const triceRx_t* rx);

// TriceAbcOnReceive is the ABC convenience entry point built from the generic
// parser, generated ABC resolver table, and ABC dispatcher.
int TriceAbcOnReceive(const uint8_t* pBuf, int len);

#endif // #if TRICE_RX_ABC_SUPPORT == 1

#if TRICE_RX_LOG_SUPPORT == 1

// triceLogEntry_t is a future log metadata table entry derived from TIL data.
typedef struct {
	const char* pTrice; // Pointer to used Trice macro name. Resolved from til.json.
	const char* pFmt;   // Trice format string resolved from til.json.
	uint16_t id;        // Trice id
} triceLog_t;

extern const triceLog_t triceLog[]; // generated Log resolver table, defined in til.json
extern const unsigned triceLogElements; // generated Log resolver table element count, defined in til.json, must be equal to triceLocationElements

//! \brief triceResolveLog attaches log rendering metadata to a parsed record.
//! \details The resolver searches for `rx->id`. If found, it fills:
//! \li   rx->pTrice; // Pointer to used Trice macro name. This influences the pFmt interpretation for logging.
//! \li   rx->pFmt;   // Trice format string resolved from til.json.
//! 
//! The bit-width is implicit coded in pTrice and derived from there. 
//! If already assigned, the value must be identical or an error is reported.
//! The `tricelog[]` list is derivable from `til.json`. It does not carry a bitWidth value. 
//! The bitWidth is implicit coded in the Trice name and is determined (and checked if possible) on the fly.
//! \retval 0 == faund or negative error value
int triceResolveLog(triceRx_t* rx, const triceLog_t* list, size_t count);

#if TRICE_LOCATION_SUPPORT == 1

// triceLocation_t is a future location metadata table entry derived from
// LI data. The resolver is separate because location data is optional even when
// log formatting is present.
typedef struct {
	const char* file; // `file` is name where the Trice statements was used. Resolved from li.json.
	uint32_t line;    // Source code line in `file` where the Trice statements was used. Resolved from li.json.
	uint16_t id;      // Trice id
} triceLocation_t;

extern const triceLocation_t triceLocation[]; // generated Location resolver table, defined in li.json
extern const unsigned triceLocationElements; // generated Location resolver table element count, defined in li.json, must be equal to triceLogElements

//! triceResolveLocation attaches source file and line metadata to a record.
//! \details It fills location data from a generated/transformed location table derived from `li.json`.
//! \li   rx->file
//! \li   rx->line
//! The data source would be generated or transformed from `li.json`. This is orthogonal to ABC and log formatting.
//! Line numbers > 65534 are set to 65535 as line number by the generator/transformer.
int triceResolveLocation(triceRx_t* rx, const triceLocation_t* list, size_t count);

#endif // #if TRICE_LOCATION_SUPPORT == 1

// triceDispatchLog is a placeholder for a future C log formatter entry point.
int triceDispatchLog(const triceRx_t* rx);

#endif // #if TRICE_RX_LOG_SUPPORT == 1

#ifdef __cplusplus
}
#endif

#endif // TRICE_RX_H_
