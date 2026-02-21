// SPDX-License-Identifier: MIT

//! \file cobs.h
//! \brief cobs declarations and macros.


#ifndef COBS_H_
#define COBS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> //lint !e537 !e451  Warning 537: Repeated include file,  Warning 451: Header file repeatedly included but does not have a standard
#include <stdint.h> //lint !e537 !e451  Warning 537: Repeated include file,  Warning 451: Header file repeatedly included but does not have a standard

//! \brief Encode data using COBS.
//! \param out Destination buffer.
//! \param in Source buffer.
//! \param length Number of source bytes.
//! \return Encoded byte count (delimiter byte is not appended).
//! \note If your compiler uses a pre-C99 C dialect and does not know "__restrict", define it in project settings.
size_t COBSEncode(void* __restrict out, const void* __restrict in, size_t length);

//! \brief Decode COBS data.
//! \param out Destination buffer.
//! \param in Source buffer.
//! \param length Number of encoded bytes.
//! \return Decoded byte count.
//! \note If your compiler uses a pre-C99 C dialect and does not know "__restrict", define it in project settings.
size_t COBSDecode(void* __restrict out, const void* __restrict in, size_t length);

#ifdef __cplusplus
}
#endif

#endif
