/*! \file xtea.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_XTEA_H_
#define TRICE_XTEA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h> //lint !e537 !e451  Warning 537: Repeated include file,  Warning 451: Header file repeatedly included but does not have a standard

void XTEAInitTable(void);
void XTEADecrypt(uint32_t* p, unsigned count);
void XTEAEncrypt(uint32_t* p, unsigned count);

//! little endian! change byte order for big endian machines
#define XTEA_KEY(b00, b01, b02, b03, \
                 b10, b11, b12, b13, \
                 b20, b21, b22, b23, \
                 b30, b31, b32, b33) \
	{                                \
		0x##b00##b01##b02##b03,      \
		0x##b10##b11##b12##b13,      \
		0x##b20##b21##b22##b23,      \
		0x##b30##b31##b32##b33       \
	}

#ifdef __cplusplus
}
#endif

#endif // TRICE_XTEA_H_
