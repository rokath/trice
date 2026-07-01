// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceRx.h
//! \brief Common Trice receive record parser and resolver API.

#ifndef TRICE_RX_H_
#define TRICE_RX_H_

#include "triceConfig.h"
#include "triceRxDefaultConfig.h"

#if TRICE_RX_SUPPORT == 1
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

// Parser functions return a positive consumed byte count on success.
// TRICE_RX_RESULT_OK is reserved for resolver and dispatcher success. All states/errors are
// negative so callers can distinguish "record consumed" from "not consumed".
enum {
	TRICE_RX_RESULT_OK = 0,                             // success
	TRICE_RX_LEN_TOO_SHORT = -1,                        // No bytes were consumed; the caller can wait for more data or drop the partial record.
	TRICE_RX_ID_WITHOUT_ABC_HANDLER = -2,               // The rexeived ID has no assigned handler, that's ok for many IDs.
	TRICE_RX_ERR_LOG_ID_NOT_FOUND = -3,                 // The generated trice ID-pFmt list maybe obsolete or data mismatch.
	TRICE_RX_ERR_LOCATION_ID_NOT_FOUND = -4,            // The generated trice ID-location list maybe obsolete or data mismatch.
	TRICE_RX_ERR_PAYLOAD_LEN_BITWIDTH_MISMATCH = -5,    // The payload length is not a multiple of the bithwith byte count.
	TRICE_RX_ERR_PAYLOAD_LEN_PARAM_COUNT_MISMATCH = -7, // The generated trice ID-pFmt list maybe obsolete or data mismatch.
	TRICE_RX_ERR_PARAM_COUNT_MISMATCH = -8,             // The generated Id-pFmt paramCount does not match the form buffer computed paramCount.
	TRICE_RX_ERR_BIT_WIDTH_CONFLICT = -9,               // The generated data do not match. Version conflict?
	TRICE_RX_ERR_UNSUPPORTED_X0_FRAME = -10,            // #if TRICE_RX_X0_COUNTED_BUFFER_SUPPORT == 0
	TRICE_RX_ERR_INVALID_BITWIDTH_VALUE = -11,          // unexpected
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
	uint8_t paramCount;     // Parsed records keep 0 here; log resolution replaces it with a fixed count.
    uint8_t cycleCounter;   // optional part of the nc field

#if TRICE_RX_ABC_SUPPORT == 1
	void (*fn)(const struct triceRx_t* rx); // abc function handler resolved from generated triceAbc[]. ( triceFn_t fn; )
#endif

#if TRICE_RX_LOG_SUPPORT == 1
    const char* pFmt; // Trice format string resolved from generated TIL metadata.
#if TRICE_LOCATION_SUPPORT == 1
    const char* file; // `file` is name where the Trice statements was used. Resolved from li.json.
    uint32_t line;    // Source code line in `file` where the Trice statements was used. Resolved from li.json.
#endif
#endif

	int executed_logged_handled; // if > 0, the id was recognized
} triceRx_t;

// triceFn_t is the generated ABC handler signature.
// The receive record is const during handler execution so nested 
// dispatch cannot mutate the caller's current record by accident.
typedef void (*triceFn_t)(const triceRx_t* rx);

// triceNodeFn_t is used for log and X0 execution.
//! \param node points to a node specific struct. Use NULL for it if not needed.
typedef void (*triceNodeFn_t)(const void* node, const triceRx_t* rx);

//! \brief parses exactly one already deframed/decrypted Trice record at buf[0].
//! \details
//! It returns the logical record length and never consumes alignment padding after the record.
//! It does not parse multiple records, expects deframed/decrypted data and does not use an ID table. 
//! It expects a plain trice binary stream according to the [Trice Binary Encoding](./TriceUserManual.md#binary-encoding)
//! \li It fills:
//! \li   rx->id
//! \li   rx->stampBits
//! \li   rx->stamp
//! \li   rx->payload - Important: `payload` points into `buf`; no payload copy is made.
//! \li   rx->payloadBytes
//! \li   rx->cycleCounter
//! \li It initializes:
//! \li   rx->bitWidth = TRICE_BIT_WIDTH_UNKNOWN (will be set after TriceResolve...)
//! \li   rx->fn = NULL, if present
//! \li   rx->paramCount = 0, as not yet resolved
//! \li   rx->pFmt = NULL, if present
//! \li   rx->file = NULL, if present
//! \li   rx->line = 0, if present
//! \retval >  0  number of consumed bytes for the next record
//! \retval <  0  negative error code
int TriceParseRecord(triceRx_t* rx, const uint8_t* buf, size_t len);

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

//! TriceResolveAbc attaches generated ABC metadata to a parsed record.
//! \details Resolve ABC metadata
//! \li The resolver searches `list[0..count-1]` for `rx->id`. If found, it fills:
//! \li    rx->bitWidth
//! \li    rx->fn
//! 
//! If `rx->bitWidth` is still `TRICE_BIT_WIDTH_UNKNOWN`, the resolver sets it. 
//! If it is already set and differs from the table entry, the resolver returns a bit-width conflict error.
//! 
//! \retval TRICE_RX_RESULT_OK on success.
//! \retval TRICE_RX_ERR_NOT_FOUND if the ID is not in the table. Unknown IDs are normal in mixed streams.
int TriceResolveAbc(triceRx_t* rx, const triceAbc_t* list, size_t count);

extern triceFn_t fn_TriceDispatchAbc;

#endif // #if TRICE_RX_ABC_SUPPORT == 1

#if TRICE_RX_LOG_SUPPORT == 1

// TRICE_LOG_PARAM_COUNT_DYNAMIC marks entries whose value count is controlled by
// the received byte count (specified in nc). Fixed scalar Trices use an exact count in 0..12.
#define TRICE_LOG_PARAM_COUNT_DYNAMIC ((uint8_t)0xffu)

// triceLog_t is the compact generated log metadata table entry derived from
// TIL data. bitWidth and paramCount are generated once so the RX code does not
// parse Trice macro names at runtime.
typedef struct {
	uint16_t id;        // Trice id.
	uint8_t bitWidth;   // Payload element width: 0, 8, 16, 32, 64, or TRICE_BIT_WIDTH_UNKNOWN.
	uint8_t paramCount; // Fixed value count or TRICE_LOG_PARAM_COUNT_DYNAMIC for string/buffer/ABC payload.
	const char* pFmt;   // Trice format string resolved from til.json.
} triceLog_t;

extern const triceLog_t triceLog[]; // generated log resolver table, defined in the generated TIL C source.
extern const unsigned triceLogElements; // generated log resolver table element count, must match triceLocationElements when locations are enabled.

//! \brief TriceResolveLog attaches log rendering metadata to a parsed record.
//! \details The resolver searches for `rx->id`. If found, it fills:
//! \li   rx->bitWidth
//! \li   rx->paramCount
//! \li   rx->pFmt;   // Trice format string resolved from til.json.
//! 
//! The bit width and parameter count are generated TIL facts, not parsed from the Trice macro name at runtime.
//! If already assigned, the value must be identical or an error is reported.
//! \retval TRICE_RX_RESULT_OK if found.
//! \retval TRICE_RX_ERR_NOT_FOUND if the ID is not in the table.
int TriceResolveLog(triceRx_t* rx, const triceLog_t* list, size_t count);

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

//! TriceResolveLocation attaches source file and line metadata to a record.
//! \details It fills location data from a generated/transformed location table derived from `li.json`.
//! \li   rx->file
//! \li   rx->line
//! The data source would be generated or transformed from `li.json`. This is orthogonal to ABC and log formatting.
//! Line numbers > 65534 are set to 65535 as line number by the generator/transformer.
int TriceResolveLocation(triceRx_t* rx, const triceLocation_t* list, size_t count);

#endif // #if TRICE_LOCATION_SUPPORT == 1

extern triceNodeFn_t fn_TricePrintLog;
#endif // #if TRICE_RX_LOG_SUPPORT == 1

#if TRICE_RX_X0_COUNTED_BUFFER_SUPPORT == 1
extern triceNodeFn_t fn_TriceHandleTypeX0;
#endif

#if TRICE_RX_SUPPORT == 1
extern triceNodeFn_t fn_TricePrintIgnoredID;
#endif

int TriceHandleDecodedRecord(const void* node, const uint8_t* record, size_t decodedLen);

#ifdef __cplusplus
}
#endif

#endif // #if TRICE_RX_SUPPORT == 1

#endif // TRICE_RX_H_
