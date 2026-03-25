// SPDX-License-Identifier: MIT

//! \file tcobs.h
//! \brief tcobs declarations and macros.


#ifndef TCOBS_H_
#define TCOBS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

//! \brief Encode a raw byte stream with TCOBSv1.
//! \param output Destination buffer for encoded data.
//! \param input Source buffer with raw data to encode.
//! \param length Number of bytes in \p input.
//! \return Number of bytes written to \p output, or a negative value on error.
//! \details
//! A zero delimiter byte is not appended, so multiple encoded payloads can be concatenated
//! before adding a final `0x00` delimiter.
//! Buffer overlap is allowed when input lies inside output with sufficient positive offset.
//! In worst case, one sigil byte is inserted per 31 input bytes. Therefore provide at least
//! `length + ceil(length/31)` bytes in \p output.
//! \note If your compiler uses a pre-C99 dialect and does not support "__restrict",
//! define it in project settings.
int TCOBSEncode(void* __restrict output, const void* __restrict input, size_t length);

//! \brief Decode a TCOBSv1 payload.
//! \param output Destination buffer written backwards from `output + max`.
//! \param max Capacity of \p output in bytes.
//! \param input Source buffer with encoded payload (without delimiter).
//! \param length Number of bytes in \p input.
//! \return Number of decoded bytes, or a negative value on error.
//! \details
//! Decoding runs backwards from the end of \p input and writes backwards into \p output.
//! The decoded payload starts at `output + max - return_value`.
//! If the (positive) return value is larger than \p max, the output buffer was too small.
//! Partial overlap can work only when output is sufficiently behind input, because decoded
//! data can be longer than encoded data.
//! \note If your compiler uses a pre-C99 dialect and does not support "__restrict",
//! define it in project settings.
int TCOBSDecode(void* __restrict output, size_t max, const void* __restrict input, size_t length);

//! \brief Error code from TCOBSDecode when output capacity is insufficient.
#define OUT_BUFFER_TOO_SMALL (-1000000)
//! \brief Error code from TCOBSDecode when encoded input data is invalid.
#define INPUT_DATA_CORRUPTED (-2000000)

#ifdef __cplusplus
}
#endif

#endif // TCOBS_H_
