/*! \file triceCobsrEncoder.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_COBSR_ENCODER_H_
#define TRICE_COBSR_ENCODER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include "triceConfig.h"

#define Id(n) ((uint32_t)n<<16) //!< Id() is a 16 bit id 0-65535 as upper 2 bytes in head

#ifdef TRICE_CYCLE_COUNTER
extern uint8_t triceCycle;
#define TRICE_CYCLE triceCycle++ //! TRICE_CYCLE is the trice cycle counter as 8 bit count 0-255.
#else
#define TRICE_CYCLE 0xC0 //! TRICE_CYCLE is no trice cycle counter, just a static value.
#endif

//! TRICE0 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
#define TRICE0( id, pFmt ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0000) | TRICE_CYCLE; \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE8_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 a 8 bit bit value
#define TRICE8_1( id, pFmt, v0 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0100) | TRICE_CYCLE; \
    *wTb++ = v0; \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE8_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v1 are 8 bit bit values
#define TRICE8_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0200) | TRICE_CYCLE; \
    *wTb++ = v0 | (v1<<8); \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE8_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v2 are 8 bit bit values
#define TRICE8_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0300) | TRICE_CYCLE; \
    *wTb++ = v0 | (v1<<8) | (v2<<16); \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE8_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v3 are 8 bit bit values
#define TRICE8_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0400) | TRICE_CYCLE; \
    *wTb++ = v0 | (v1<<8) | (v2<<16) | (v3<<24); \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE8_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v4 are 8 bit bit values
#define TRICE8_5( id, pFmt, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0500) | TRICE_CYCLE; \
    *wTb++ = v0 | (v1<<8) | (v2<<16) | (v3<<24); \
    *wTb++ = v4; \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE8_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v5 are 8 bit bit values
#define TRICE8_6( id, pFmt, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0600) | TRICE_CYCLE; \
    *wTb++ = v0 | (v1<<8) | (v2<<16) | (v3<<24); \
    *wTb++ = v4 | (v5<<8); \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v6 are 8 bit bit values
#define TRICE8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0700) | TRICE_CYCLE; \
    *wTb++ = v0 | (v1<<8) | (v2<<16) | (v3<<24); \
    *wTb++ = v4 | (v5<<8) | (v6<<16)  \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0800) | TRICE_CYCLE; \
    *wTb++ = v0 | (v1<<8) | (v2<<16) | (v3<<24); \
    *wTb++ = v4 | (v5<<8) | (v6<<16) | (v7<<24); \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE16_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 a 16 bit value
#define TRICE16_1( id, pFmt, v0 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0200) | TRICE_CYCLE; \
    *wTb++ = v0; \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE16_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v1 are 16 bit values
#define TRICE16_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0400) | TRICE_CYCLE; \
    *wTb++ = v0 | (v1<<16); \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE16_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v2 are 16 bit values
#define TRICE16_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0600) | TRICE_CYCLE; \
    *wTb++ = v0 | (v1<<16); \
    *wTb++ = v3; \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE16_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v3 are 16 bit values
#define TRICE16_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0800) | TRICE_CYCLE; \
    *wTb++ = v0 | (v1<<16); \
    *wTb++ = v2 | (v3<<16); \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE32_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 the 32 bit value
#define TRICE32_1( id, pFmt, v0 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0400) | TRICE_CYCLE; \
    *wTb++ = v0; \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE32_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v1 are 32 bit values
#define TRICE32_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0800) | TRICE_CYCLE; \
    *wTb++ = v0; *wTb++ = v1; \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE32_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v2 are 32 bit values
#define TRICE32_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0c00) | TRICE_CYCLE; \
    *wTb++ = v0; *wTb++ = v1; *wTb++ = v2; \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE32_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v3 are 32 bit values
#define TRICE32_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x1000) | TRICE_CYCLE; \
    *wTb++ = v0; *wTb++ = v1; *wTb++ = v2; *wTb++ = v3; \
    TRICE_LEAVE_CRITICAL_SECTION

//! TRICE64_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 is a 64 bit values
#define TRICE64_1( id, pFmt, v0 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x0800) | TRICE_CYCLE; \
    *(uint64_t*)wTb++ = v0; \
    TRICE_LEAVE_CRITICAL_SECTION


//! TRICE64_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v1 are 64 bit values
#define TRICE64_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x1000) | TRICE_CYCLE; \
    *(uint64_t*)wTb++ = v0; *(uint64_t*)wTb++ = v1; \
    TRICE_LEAVE_CRITICAL_SECTION


//! TRICE64_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v2 are 64 bit values
#define TRICE64_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x1800) | TRICE_CYCLE; \
    *(uint64_t*)wTb++ = v0; *(uint64_t*)wTb++ = v1; \
    *(uint64_t*)wTb++ = v2; \
    TRICE_LEAVE_CRITICAL_SECTION


//! TRICE64_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v3 are 64 bit values
#define TRICE64_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = (id | 0x2000) | TRICE_CYCLE; \
    *(uint64_t*)wTb++ = v0; *(uint64_t*)wTb++ = v1; \
    *(uint64_t*)wTb++ = v2; *(uint64_t*)wTb++ = v3; \
    TRICE_LEAVE_CRITICAL_SECTION


//! TRICES_1 writes id and dynString.
//! \param id trice identifier
//! \param pFmt formatstring for trice (ignored here but used by the trice tool)
//! \param dynString 0-terminated runtime generated string
//! After the 4 byte trice message header are following 2^n bytes 
//! string transfer format: 
//! idH    idL    len    cycle
//! c0     c1     c2     c3
//! ...
//! cLen-3 cLen-2 cLen-1 cLen
#define TRICES_1( id, pFmt, dynString) do { \
    int len = strlen( dynString ); \
    if( 255 < len ){ \
        dynString[255] = 0; \
        len = 255; \
    } \
    TRICE_ENTER_CRITICAL_SECTION \
    *wTb++ = id | (len<<8) | TRICE_CYCLE; \
    memcpy( *wTb, s, len ); \
    wTb += (len+3)>>2; \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

#ifdef __cplusplus
}
#endif

#endif /* TRICE_COBSR_ENCODER_H_ */
