//! \file cobsEncode.c copied from https://github.com/rokath/cobs
//! \author Thomas.Hoehenleitner [at] seerose.net

#include "cobs.h"

//! COBSEncode encodes data to output.
//! @param in Pointer to input data to encode.
//! @param length Number of bytes to encode.
//! @param out Pointer to encoded output buffer.
//! @return Encoded buffer length in bytes.
//! @note Does not output delimiter byte. Code taken from Wikipedia and slightly adapted.
size_t COBSEncode( void * restrict out, const void * restrict in, size_t length) {
	uint8_t * buffer = out;
	uint8_t *encode = buffer; // Encoded byte pointer
	uint8_t *codep = encode++; // Output code pointer
	uint8_t code = 1; // Code value

	for (const uint8_t *byte = (const uint8_t *)in; length--; ++byte) {
		if (*byte) { // Byte not zero, write it
			*encode++ = *byte, ++code;
		}
		if (!*byte || code == 0xff) { // Input is zero or block completed, restart
			*codep = code, code = 1, codep = encode;
			if (!*byte || length) {
				++encode;
			}
		}
	}
	*codep = code; // Write final code value
	return (size_t)(encode - buffer);
}
