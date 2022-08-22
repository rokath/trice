/*! \file tcobs.h
\author thomas.hoehenleitner [at] seerose.net
\details See ./TCOBSv1Specification.md.
*******************************************************************************/

#ifndef TCOBS_H_
#define TCOBS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

//! TCOBSEncode stuffs "length" bytes of data beginning at the location pointed to by "input" and writes the output to the 
//! location pointed to by "output". Returns the number of bytes written to "output" or a negative value in error case.
//! A 0-delimiter is NOT added as last byte allowing concatenating TCOBS encoded buffers to one bigger buffer before the 
//! 00-byte delimiting. Buffer overlapping is allowed, when input lays inside output with a sufficient offset. The offset 
//! should be >= next larger whole number of length/31. because in the worst case for each 31 bytes an additional sigil byte 
//! is inserted. The provided output buffer size should be >= length + next larger whole number of length/31. This is a 
//! responsibility of the caller and not checked for efficiency. Remove the "restrict" qualifiers if compiling with a pre-C99 C dialect.
int TCOBSEncode( void * restrict output, const void * restrict input, size_t length);

//! TCOBSDecode decodes data ending at the location pointed to by "input" backwards (starting with the last byte)
//! and writes the output also backwards to the location pointed to by "output" with a maximum size of max.
//! Returns the number of bytes written to "output". Only max bytes are written. If the returned value is
//! > max, this is an error "output buffer too small". In case of an error, a negative value is returned.
//! THIS IS **IMPORTANT**: The decoded data start at output+max-returned, because output is filled from the end.
//! Buffer overlapping is partially possible if output limit is _behind_ input limit with sufficient distance,
//! but data can get much longer. Remove the "restrict" qualifiers if compiling with a pre-C99 C dialect.
int TCOBSDecode( void * restrict output, size_t max, const void * restrict input, size_t length );

#define OUT_BUFFER_TOO_SMALL -1000000 //!< OUT_BUFFER_TOO_SMALL is TCOBSDecode return error code.
#define INPUT_DATA_CORRUPTED -2000000 //!< INPUT_DATA_CORRUPTED is TCOBSDecode return error code.

#ifdef __cplusplus
}
#endif

#endif // TCOBS_H_
