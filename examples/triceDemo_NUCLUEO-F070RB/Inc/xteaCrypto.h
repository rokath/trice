/*!\file treyferCrypto.h
\brief 64 bit Crypto Module Code
\details Contains 64 bit encrypt and decrypt routines dclarations
*/
#ifndef TRICE_XTEACRYPTO_H_
#define TRICE_XTEACRYPTO_H_  /*!< avoid multiple inclusion of this file */

#include <stdint.h>

void encrypt( uint8_t* p );
void decrypt( uint8_t* p );

//! little endian! change byte order for big endian machines
#define XTEA_KEY( b00, b01, b02, b03, \
                  b10, b11, b12, b13, \
                  b20, b21, b22, b23, \
                  b30, b31, b32, b33 ) { \
    0x##b00##b01##b02##b03, \
    0x##b10##b11##b12##b13, \
    0x##b20##b21##b22##b23, \
    0x##b30##b31##b32##b33 }

void InitXteaTable( void );
    
#endif /* #ifndef TRICE_XTEACRYPTO_H_ */
