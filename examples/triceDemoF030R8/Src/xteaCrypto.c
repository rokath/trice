/*!\file xteaCrypto.c
\brief 64 bit Crypto Module Code
\details Contains 64 bit encrypt and decrypt routines. 
see also https://www.emsec.ruhr-uni-bochum.de/media/crypto/veroeffentlichungen/2011/01/29/lw_speed2007.pdf
*/

#include <config.h>

#ifdef ENCRYPT

#include "xteaCrypto.h"

//! golang XTEA works with 64 rounds
static const unsigned int numRounds = 64;

//! 128 bit static key
static const uint32_t k[4] = ENCRYPT; 

//! internal constant
static const uint32_t delta = 0x9E3779B9;

//! precomputed values for faster execution
static uint32_t table[64];

//! Precalculate the table
//! It is possible to put this table completely into FLASH by precomputing it during compile time
void InitXteaTable( void ){
    uint32_t sum = 0;
    // Two rounds of XTEA applied per loop
    for( int i = 0; i < numRounds; ) {
        table[i] = sum + k[sum&3];
        i++;
        sum += delta;
        table[i] = sum + k[(sum>>11)&3];
        i++;
    }
}

//!code taken and adapted from xtea\block.go
//!\param v take 64 bits of data in v[0] and v[1]
static void encipher( uint32_t v[2] ) {
    uint32_t v0 = v[0], v1 = v[1];
    for( int i=0; i < numRounds; ) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ table[i];
        i++;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ table[i];
        i++;
    }
    v[0] = v0; v[1] = v1;
}

//!code taken and adapted from xtea\block.go
//!\param v take 64 bits of data in v[0] and v[1]
static void decipher( uint32_t v[2] ) {
    uint32_t v0 = v[0], v1 = v[1];
    for( int i=numRounds; i > 0; ) {
        i--;
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ table[i];
        i--;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ table[i];
    }
    v[0] = v0; v[1] = v1;
}

//! convert to xtea cipher
//! \param p pointer to 8 byte buffer
void encrypt( uint8_t* p ){
    encipher( (uint32_t*)p ); // byte swap is done inside trice tool
}

//! re-convert from xtea cipher
//! \param p pointer to 8 byte buffer
void decrypt( uint8_t* p ){
    decipher( (uint32_t*)p ); // byte swap is done inside trice tool
}

#endif // #ifdef ENCRYPT
