// SPDX-License-Identifier: MIT

//! \file xtea.h
//! \brief xtea declarations and macros.


#ifndef TRICE_XTEA_H_
#define TRICE_XTEA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h> //lint !e537 !e451  Warning 537: Repeated include file,  Warning 451: Header file repeatedly included but does not have a standard

//! \brief Precompute round constants table used by XTEA encrypt/decrypt.
void XTEAInitTable(void);

//! \brief Decrypt \p count 32-bit words in-place (must be a multiple of 2).
//! \param p Pointer to 64-bit blocks as uint32_t pairs.
//! \param count Number of uint32_t words to process.
void XTEADecrypt(uint32_t* p, unsigned count);

//! \brief Encrypt \p count 32-bit words in-place (must be a multiple of 2).
//! \param p Pointer to 64-bit blocks as uint32_t pairs.
//! \param count Number of uint32_t words to process.
void XTEAEncrypt(uint32_t* p, unsigned count);

//! \brief Build a 128-bit XTEA key from byte literals in little-endian order.
//! \note For big-endian targets adapt byte order accordingly.
#define XTEA_KEY(b00, b01, b02, b03, \
                 b10, b11, b12, b13, \
                 b20, b21, b22, b23, \
                 b30, b31, b32, b33) \
	{                                \
	    0x##b00##b01##b02##b03,      \
	    0x##b10##b11##b12##b13,      \
	    0x##b20##b21##b22##b23,      \
	    0x##b30##b31##b32##b33}

#ifdef __cplusplus
}
#endif

#endif // TRICE_XTEA_H_
