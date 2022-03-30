/*! \file TCOBS.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TCOBS_H_
#define TCOBS_H_

#include <stdint.h>

//! TCOBSEncode stuffs "length" bytes of data at the location pointed to by "input"
//! and writes the output to the location pointed to by "output".
//! A 0-delimiter is added as last byte. Returns the number of bytes written to "output". 
//! Buffer overlapping is allowed, when input lays inside output with a sufficient offset.
//! The offset should be > 1+(length>>5) because in the worst case for each 32 bytes an additional sigil byte is inserted.
//! The provided output buffer size should be > length + > 1+(length>>5).
//! Remove the "restrict" qualifiers if compiling with a pre-C99 C dialect.
//! (copied and adapted from https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing/blob/master/cobs.c)
unsigned TCOBSEncode( uint8_t* restrict output, const uint8_t * restrict input, unsigned length);

//! TCOBSDencode decodes 0-delimited data at the location pointed to by "input"
//! and writes the output to the location pointed to by "output" with a maximum size of max.
//! Returns the number of bytes written to "output". If the returned value is equal max, 
//! this is an error "output buffer too small". If input points to a 0, the returned length is 0.
//! Buffer overlapping is not allowed because the decoded data can get much longer.
//! Remove the "restrict" qualifiers if compiling with a pre-C99 C dialect.
//! (copied and adapted from https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing/blob/master/cobs.c)
unsigned TCOBSDecode( uint8_t* restrict output, unsigned max, const uint8_t * restrict input );

#ifdef __cplusplus
}
#endif

#endif // TCOBS_H_
