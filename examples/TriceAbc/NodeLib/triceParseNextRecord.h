#ifndef TRICE_PARSE_NEXT_RECORD_H_
#define TRICE_PARSE_NEXT_RECORD_H_

/*
 * triceParseNextRecord.h
 *
 * Small parser API for one already decoded Trice record.
 *
 * The word "decoded" here means that any transport layer work, such as byte
 * unstuffing, framing removal, or decryption, has already happened before this
 * function is called. This parser only interprets the record bytes themselves.
 */

#include "triceRx.h"
#include <stddef.h>
#include <stdint.h>

/*
 * Parse one record from the beginning of buf.
 *
 * Return value:
 *
 *     >= 0  number of bytes consumed by the parsed record
 *     <  0  TRICE_RX_E_* error code
 *
 * TRICE_RX_E_SHORT means that buf may contain a valid prefix but len is not
 * large enough yet. The caller should keep the bytes and retry when more input
 * arrives.
 *
 * The parser fills only byte-record facts: id, stampBits, stamp, payload pointer,
 * and payloadBytes. It does not resolve bitWidth, handler function pointer,
 * format string, file, or line information.
 */
int triceParseNextRecord(triceRx_t* rx, const uint8_t* buf, size_t len);

#endif /* TRICE_PARSE_NEXT_RECORD_H_ */
