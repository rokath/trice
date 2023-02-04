//! \file cobsDecode.c copied from https://github.com/rokath/cobs
//! \author Thomas.Hoehenleitner [at] seerose.net

#include "cobs.h"

//! COBSDecode decodes data from in buffer.
//! @param in Pointer to encoded input bytes.
//! @param length Number of bytes to decode.
//! @param out Pointer to decoded output data.
//! @return Number of bytes successfully decoded.
//! @note Stops decoding if delimiter byte is found. Code taken from Wikipedia and slightly modyfied.
size_t COBSDecode(void * restrict out, const void * restrict in, size_t length ) {
	uint8_t* data = out;
	const uint8_t * buffer = in;
	const uint8_t *byte = buffer; // Encoded input byte pointer
	uint8_t *decode = (uint8_t *)data; // Decoded output byte pointer

	for (uint8_t code = 0xff, block = 0; byte < buffer + length; --block) {
		if (block) { // Decode block byte
			*decode++ = *byte++;
		} else {
			if (code != 0xff) { // Encoded zero, write it
				*decode++ = 0;
			}
			block = code = *byte++; // Next block length
			if (!code) { // Delimiter code found
				break;
			}
		}
	}
	return (size_t)(decode - data);
}
