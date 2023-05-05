/*! \file xtea.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#include "xtea.h"
#include "trice.h"

//! golang XTEA works with 64 rounds
static const unsigned int numRounds = 64;

#ifndef XTEA_ENCRYPT_KEY
#define XTEA_ENCRYPT_KEY XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret
#endif

//! 128 bit static key
static const uint32_t k[4] = XTEA_ENCRYPT_KEY; 

//! internal constant
static const uint32_t delta = 0x9E3779B9;

//! precomputed values for faster execution
static uint32_t table[64];

//! XTEAInitTable precalculates the table.
//! It is possible to put this table completely into FLASH by precomputing it during compile time.
void XTEAInitTable( void ){
    uint32_t sum = 0;
    int i;
    // Two rounds of XTEA applied per loop
    for( i = 0; i < numRounds; ) {
        table[i] = sum + k[sum&3];
        i++;
        sum += delta;
        table[i] = sum + k[(sum>>11)&3];
        i++;
    }
}

// encipher converts 64 bits.
//! Code taken and adapted from xtea\block.go
//!\param v 64 bits of data in v[0] and v[1] are encoded in place
static void encipher( uint32_t v[2] ) {
    uint32_t v0 = v[0], v1 = v[1];
    int i;
    for( i=0; i < numRounds; ) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ table[i];
        i++;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ table[i];
        i++;
    }
    v[0] = v0; v[1] = v1;
}

#ifdef XTEA_DECRYPT
//! decipher reverses encipher action.
//! Code taken and adapted from xtea\block.go
//!\param v 64 bits of data in v[0] and v[1] are decoded in place
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

//! XTEADecrypt re-converts from xtea cipher.
//! \param p pointer to 8 byte buffer
//! count is expected to be an even number.
void XTEADecrypt( uint32_t* p, unsigned count ){
    for( int i = 0; i < count; i +=2 ){
        decipher( &p[i]); // byte swapping is done inside receiver according to endianness.
    }
}
#endif // #ifdef XTEA_DECRYPT

//! XTEAEncrypt converts to xtea cipher.
//! \param p pointer to 8 byte buffer.
//! count is expected to be an even number.
void XTEAEncrypt( uint32_t* p, unsigned count ){
    int i;
    for( i = 0; i < count; i +=2 ){
        encipher( &p[i] ); // byte swap is done inside receiver
    }
}
