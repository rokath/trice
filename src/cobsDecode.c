// SPDX-License-Identifier: MIT

//! \file cobsDecode.c
//! \brief cobs Decode implementation.


#include "cobs.h"

// Public API is documented in cobs.h.
// Implementation adapted from the Wikipedia COBS reference.
size_t COBSDecode(void* __restrict out, const void* __restrict in, size_t length) {
	uint8_t* data = (uint8_t*) out;
	const uint8_t* buffer = (const uint8_t*) in;
	const uint8_t* byte = buffer; // Encoded input byte pointer
	uint8_t* decode = data;       // Decoded output byte pointer

	for (uint8_t code = 0xff, block = 0; byte < buffer + length; --block) {
		if (block) { // Decode block byte
			*decode++ = *byte++;
		} else {
			if (code != 0xff) { // Encoded zero, write it
				*decode++ = 0;
			}
			block = code = *byte++; // Next block length
			if (!code) {            // Delimiter code found
				break;
			}
		}
	}
	return (size_t)(decode - data);
}
