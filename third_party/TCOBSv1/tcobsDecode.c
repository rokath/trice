/*! \file tcobsDecode.c
\author Thomas.Hoehenleitner [at] seerose.net
\details See ./TCOBSv1Specification.md.
*******************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <string.h> // memcpy
#include "tcobs.h"
#include "tcobsInternal.h"

static int sigilAndOffset( uint8_t* sigil, uint8_t b );
static uint8_t repeatByte(int offset, uint8_t* in, int len);

int TCOBSDecode( void * restrict output, size_t max, const void * restrict input, size_t length ){
	uint8_t * in = (uint8_t*)input;
	int ilen = (int)length; // remaining input length
	uint8_t * out = (uint8_t*)output;
	int olen = 0; // output length
	while( ilen > 0 ){
		uint8_t next = in[ilen-1]; // get next sigil byte (starting from the end)
		uint8_t sigil;
		uint8_t r;
		int offset = sigilAndOffset(&sigil, next);
		if( offset+1 > ilen ){
			return INPUT_DATA_CORRUPTED - __LINE__;
		}
		ilen -= 1; // remove sigil byte from buffer
		switch( sigil ){
			case N:
				goto copyBytes;

			case Z3:
				olen += 1;
				out[max-olen] = 0;
				//fallthrough
			case Z2:
				olen += 1;
				out[max-olen] = 0;
				//fallthrough
			case Z1:
				olen += 1;
				out[max-olen] = 0;
				goto copyBytes;

			case F4:
				olen += 1;
				out[max-olen] = 0xFF;
				//fallthrough
			case F3:
				olen += 1;
				out[max-olen] = 0xFF;
				//fallthrough
			case F2:
				olen += 1;
				out[max-olen] = 0xFF;
				olen += 1;
				out[max-olen] = 0xFF;
				goto copyBytes;

			case R4:
				olen += 1;
				out[max-olen] = repeatByte(offset, in, ilen);
				//fallthrough
			case R3:
				olen += 1;
				out[max-olen] = repeatByte(offset, in, ilen);
				//fallthrough
			case R2:
				r = repeatByte(offset, in, ilen);
				olen += 1;
				out[max-olen] = r;
				olen += 1;
				out[max-olen] = r;
				goto copyBytes;

			default:
				return INPUT_DATA_CORRUPTED - __LINE__;
			}

		copyBytes:{
				uint8_t* to = out + max - olen - offset; //to := len(d) - n - offset
				uint8_t* from = in + ilen - offset; // from := len(in) - offset // sigil byte is already removed
				if( to < out ){
					return OUT_BUFFER_TOO_SMALL - __LINE__;
				}
				memcpy( to, from, offset ); // n += copy(d[to:], in[from:])
				olen += offset;
				ilen -= offset; // in = in[:len(in)-offset] // remove copied bytes
				continue;
		}
	}
	return olen;
}

// sigilAndOffset interprets b as sigil byte with offset, fills sigil and returns offset.
// For details see TCOBSv1Specification.md.
static int sigilAndOffset( uint8_t* sigil, uint8_t b ){
	int offset;
	*sigil = b & 0xE0;   // 0x11100000
	if( *sigil == 0 ){
		*sigil = b & 0xF8; // 0x11111000
		offset = 7 & b;    // 0x00000111
	}else{
		offset = 0x1F & b;   // 0x00011111
	}
	return offset;
}

// repeatByte returns the value to repeat
static uint8_t repeatByte(int offset, uint8_t* in, int len){
		if( offset == 0 ){ // left byte of Ri is a sigil byte (probably N)
				return in[len-2]; // a buffer cannot start with Ri
		}else{
				return in[len-1];
		}
}
