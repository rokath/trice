#ifndef TRICE_PARSE_NEXT_RECORD_H_
#define TRICE_PARSE_NEXT_RECORD_H_

#include "triceRx.h"
#include <stddef.h>
#include <stdint.h>

/*
 * Parse one already decoded Trice record from the beginning of buf.
 *
 * Return value:
 *   >= 0  number of bytes consumed by the parsed record
 *   <  0  TRICE_RX_E_* error code
 *
 * This function is not a framing, COBS/TCOBS, or decryption decoder. Such layers
 * must run before this function. The parser fills only byte-record facts:
 * id, stampBits, stamp, payload pointer, and payloadBytes.
 */
int triceParseNextRecord(triceRx_t* rx, const uint8_t* buf, size_t len);

#endif /* TRICE_PARSE_NEXT_RECORD_H_ */
