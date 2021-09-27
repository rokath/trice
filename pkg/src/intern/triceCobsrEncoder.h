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

// 16 bit id 0-65535
// 8 bit count 0-255

#define Id(n) ((uint32_t)n<<16)
#define TRICE_CYCLE cycle++

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













#if 0 // legacy

//#define Id(n) ((((uint32_t)(n))<<(32-21)) |  0x80000000) //!< Prepare 20-bit ID for transmission, bit 31 is 1

//! TRICE_LONGCOUNT is an internal helper.
//#define TRICE_LONGCOUNT( n ) (((n) << 16) | (0xffff & ~(n)) )

#if 0
#define id(n) ((((uint32_t)(n))<<(32-16)) & ~0x80000000) //!< Prepare 15-bit ID for transmission, bit 31 is 0

//! Trice0 does trace id unprotected (inside critical section) in short mode.
//! \param id trice identifier
//! \param pFmt formatstring for trice
#define Trice0i( id, pFmt ) do{ \
    TRICE_HTON_U32PUSH( id ); \
} while(0)

//! Trice0 does trace id protected (outside critical section) in short mode.
//! \param id trice identifier
//! \param pFmt formatstring for trice
#define Trice0( id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    Trice0i( id, pFmt ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! Trice16_1i does trace id and 16-bit value unprotected (inside critical section) in short mode.
//! The passed d0 must not exeed the 16-bit size.
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define Trice16_1i( id, pFmt, d0 ) do{ \
    TRICE_HTON_U32PUSH( id | /*(uint16_t)*/d0 ); \
} while(0)

//! Trice16_1 does trace id and 16-bit value protected (outside critical section) in short mode.
//! The passed d0 must not exeed the 16-bit size.
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define Trice16_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    Trice16_1i( id, pFmt, d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

#define Trice8_1i Trice16_1i // same code
#define Trice8_1  Trice16_1  // same code

//! Trice8_2i does trace id and 8-bit values unprotected (inside critical section) in short mode.
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! This is a time optinized implementation.
#define Trice8_2i( id, pFmt, d0, d1 ) do{ \
    TRICE_HTON_U32PUSH( id|TRICE_U8_JOIN(d0,d1) ); \
} while(0)

//! Trice8_2 does trace id and 8-bit values protected (outside critical section) in short mode.
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! This is a time optinized implementation.
#define Trice8_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    Trice8_2i( id, pFmt, d0, d1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


//! TRICE_SYNC can be used for checks or payload filling. In is an invisible trice message.
#define TRICE_SYNC do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( 0x89abcdef ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

#endif

extern uint8_t triceCycle;

#define Id(n) ((uint16_t)(((uint16_t)(n))<<4)) //!< Prepare 12-bit ID for transmission, bit 16-19 are deleted



#define TRICE_HTON_U16_COBSR_PUSH(a) do { \
    TRICE_U8PUSH( a>>8 ); TRICE_U8PUSH( a ); \
} while(0)


//! TRICE0i does trace id unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! This is a time optinized implementation.
#define TRICE0i( id, pFmt ) do{ \
    uint8_t ii = (uint8_t)(id >> 8); \
    uint8_t ic = (uint8_t)(id | triceCycle++); \
    triceCycle &= 0x0f; \
    TRICE_U8PUSH( ic ); TRICE_U8PUSH( ii ); TRICE_U8PUSH( 0 ); \
} while(0)

//! TRICE0 does trace id protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! This is a time optinized implementation.
#define TRICE0( id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE0i( id, pFmt ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE8_1i does trace id and 8-bit value unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
//! This is a time optinized implementation.
#define TRICE8_1i( id, pFmt, d0 ) do{ \
    uint8_t ii = (uint8_t)(id >> 8); \
    uint8_t ic = (uint8_t)(id | triceCycle++); \
    triceCycle &= 0x0f; \
         if ( d0  > 3 ){ TRICE_U8PUSH( d0); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH( 0 ); } \
    else if ( d0 == 0 ){ TRICE_U8PUSH( 3 ); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH( 1 ); TRICE_U8PUSH( 0 ); } \
    else               { TRICE_U8PUSH( 4 ); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH(d0 ); TRICE_U8PUSH( 0 ); } \
} while(0)

//! TRICE8_1 does trace id and 8-bit value protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
//! This is a time optinized implementation.
#define TRICE8_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE8_1i( id, pFmt, d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE8_2i does trace id and 8-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! This is a time optinized implementation.
#define TRICE8_2i( id, pFmt, d0, d1 ) do{ \
    uint8_t ii = (uint8_t)(id >> 8); \
    uint8_t ic = (uint8_t)(id | triceCycle++); \
    triceCycle &= 0x0f; \
         if( d0 == 00 && d1 != 00 ) { TRICE_U8PUSH( 3); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH(d1); TRICE_U8PUSH( 0 ); } \
    else if( d0 == 00 && d1 == 00 ) { TRICE_U8PUSH( 3); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH( 1); TRICE_U8PUSH( 1); TRICE_U8PUSH( 0 ); } \
    else if( d0 != 00 && d1 == 00 ) { TRICE_U8PUSH( 4); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH(d0); TRICE_U8PUSH( 1); TRICE_U8PUSH( 0 ); } \
    else if( d0 != 00 && d1  <  5 ) { TRICE_U8PUSH( 5); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH(d0); TRICE_U8PUSH(d1); TRICE_U8PUSH( 0 ); } \
    else                            { TRICE_U8PUSH(d1); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH(d0); TRICE_U8PUSH( 0 ); } \
} while(0)

//! TRICE8_2 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! This is a time optinized implementation.
#define TRICE8_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE8_2i( id, pFmt, d0, d1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE8_3i does trace id and 8-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! This is a time optinized implementation.
#define TRICE8_3i( id, pFmt, d0, d1, d2 ) do{ \
    uint8_t src[5] = { (uint8_t)(id >> 8), (uint8_t)(id | triceCycle++), d0, d1, d2 }; \
    uint8_t len, dst[6]; \
    triceCycle &= 0x0f; \
    len = triceCOBSREncode(dst, src, 5); \
    for( int i = 0; i < len; i++ ){ TRICE_U8PUSH( dst[i] ); } \
    TRICE_U8PUSH(0); \
} while(0)

//! TRICE8_3 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! This is a time optinized implementation.
#define TRICE8_3( id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE8_3i( id, pFmt, d0, d1, d2 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE8_4i does trace id and 8-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! This is a time optinized implementation.
#define TRICE8_4i( id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0400|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
} while(0)

//! TRICE8_4 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! This is a time optinized implementation.
#define TRICE8_4( id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE8_4i( id, pFmt, d0, d1, d2, d3 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE8_5i does trace id and 8-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! This is a time optinized implementation.
#define TRICE8_5i( id, pFmt, d0, d1, d2, d3, d4 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(5) ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_HTON_U32PUSH( (uint8_t)(d4) ); \
} while(0)

//! TRICE8_5 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! This is a time optinized implementation.
#define TRICE8_5( id, pFmt, d0, d1, d2, d3, d4 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE8_5i( id, pFmt, d0, d1, d2, d3, d4 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE8_6i does trace id and 8-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! This is a time optinized implementation.
#define TRICE8_6i( id, pFmt, d0, d1, d2, d3, d4, d5 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(6) ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d4,d5) ); \
} while(0)

//! TRICE8_6 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! This is a time optinized implementation.
#define TRICE8_6( id, pFmt, d0, d1, d2, d3, d4, d5 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE8_6i( id, pFmt, d0, d1, d2, d3, d4, d5 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE8_7i does trace id and 8-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! This is a time optinized implementation.
#define TRICE8_7i( id, pFmt, d0, d1, d2, d3, d4, d5, d6 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(7) ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN( 0,d4)<<16) | TRICE_U8_JOIN(d5,d6) ); \
} while(0)


//! TRICE8_7 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! This is a time optinized implementation.
#define TRICE8_7( id, pFmt, d0, d1, d2, d3, d4, d5, d6 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE8_7i( id, pFmt, d0, d1, d2, d3, d4, d5, d6 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE8_8i does trace id and 8-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
//! This is a time optinized implementation.
#define TRICE8_8i( id, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(8) ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d4,d5)<<16) | TRICE_U8_JOIN(d6,d7) ); \
} while(0)

//! TRICE8_8 does trace id and 8-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
//! This is a time optinized implementation.
#define TRICE8_8( id, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE8_8i( id, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE16_1i does trace id and 16-bit value unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param v0 payload
//! This is a time optinized implementation.
#define TRICE16_1i( id, pFmt, v0 ) do{ \
    uint8_t ii = (uint8_t)(id >> 8); \
    uint8_t ic = (uint8_t)(id | triceCycle++); \
    uint8_t d0, d1; \
    triceCycle &= 0x0f; \
    d0 = (uint8_t)(v0); \
    d1 = (uint8_t)(v0>>8); \
         if( d0 == 00 && d1 != 00 ) { TRICE_U8PUSH( 3); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH(d1); TRICE_U8PUSH( 0 ); } \
    else if( d0 == 00 && d1 == 00 ) { TRICE_U8PUSH( 3); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH( 1); TRICE_U8PUSH( 1); TRICE_U8PUSH( 0 ); } \
    else if( d0 != 00 && d1 == 00 ) { TRICE_U8PUSH( 4); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH(d0); TRICE_U8PUSH( 1); TRICE_U8PUSH( 0 ); } \
    else if( d0 != 00 && d1  <  5 ) { TRICE_U8PUSH( 5); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH(d0); TRICE_U8PUSH(d1); TRICE_U8PUSH( 0 ); } \
    else                            { TRICE_U8PUSH(d1); TRICE_U8PUSH( ii ); TRICE_U8PUSH( ic ); TRICE_U8PUSH(d0); TRICE_U8PUSH( 0 ); } \
} while(0)

//! TRICE16_1 does trace id and 16-bit value protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! This is a time optinized implementation.
#define TRICE16_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE16_1i( id, pFmt, d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE16_2i does trace id and 16-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! This is a time optinized implementation.
#define TRICE16_2i( id, pFmt, d0, d1 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0400|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d0,d1) ); \
} while(0)

//! TRICE16_2 does trace id and 16-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! This is a time optinized implementation.
#define TRICE16_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE16_2i( id, pFmt, d0, d1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE16_3i does trace id and 16-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! This is a time optinized implementation.
#define TRICE16_3i( id, pFmt, d0, d1, d2 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(6) ); \
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d0,d1) ); \
    TRICE_HTON_U32PUSH( (uint16_t)(d2) ); \
} while(0)

//! TRICE16_3 does trace id and 16-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! This is a time optinized implementation.
#define TRICE16_3( id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE16_3i( id, pFmt, d0, d1, d2 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


//! TRICE16_4i does trace id and 16-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! This is a time optinized implementation.
#define TRICE16_4i( id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(8) ); \
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d0,d1) ); \
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d2,d3) ); \
} while(0)

//! TRICE16_4 does trace id and 16-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! This is a time optinized implementation.
#define TRICE16_4( id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE16_4i( id, pFmt, d0, d1, d2, d3 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


//! TRICE32_1i does trace id and 32-bit value unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! This is a time optinized implementation.
#define TRICE32_1i( id, pFmt, d0 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0400|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( d0 ); \
} while(0)

//! TRICE32_1 does trace id and 32-bit value protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! This is a time optinized implementation.
#define TRICE32_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE32_1i( id, pFmt, d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE32_2i does trace id and 32-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! This is a time optinized implementation.
#define TRICE32_2i( id, pFmt, d0, d1 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(8) ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( d1 ); \
} while(0)

//! TRICE32_2 does trace id and 32-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! This is a time optinized implementation.
#define TRICE32_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE32_2i( id, pFmt, d0, d1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE32_3i does trace id and 32-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! This is a time optinized implementation.
#define TRICE32_3i( id, pFmt, d0, d1, d2 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(12) ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( d1 ); \
    TRICE_HTON_U32PUSH( d2 ); \
} while(0)

//! TRICE32_3 does trace id and 32-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! This is a time optinized implementation.
#define TRICE32_3( id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE32_3i( id, pFmt, d0, d1, d2 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


//! TRICE32_4i does trace id and 32-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! This is a time optinized implementation.
#define TRICE32_4i( id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(16) ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( d1 ); \
    TRICE_HTON_U32PUSH( d2 ); \
    TRICE_HTON_U32PUSH( d3 ); \
} while(0)

//! TRICE32_4 does trace id and 32-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! This is a time optinized implementation.
#define TRICE32_4( id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE32_4i( id, pFmt, d0, d1, d2, d3 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

#if 1 // TRICE_TRANSFER_ENDIANNESS == TRICE_LITTLE_ENDIANNESS // to do: adapt decoder
#define TRICE_HTON_U64PUSH( d ) do { \
    TRICE_HTON_U32PUSH( (uint64_t)(d)>>32 ); \
    TRICE_HTON_U32PUSH( d ); \
} while(0); 
#else
#define TRICE_HTON_U64PUSH( d ) do { \
    TRICE_HTON_U32PUSH( d ); \
    TRICE_HTON_U32PUSH( (uint64_t)(d)>>32 ); \
} while(0); 
#endif
    
//! TRICE64_1i does trace id and 64-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! This is a time optinized implementation.
#define TRICE64_1i( id, pFmt, d0 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(8) ); \
    TRICE_HTON_U64PUSH( d0 ); \
} while(0)

//! TRICE64_1 does trace id and 64-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! This is a time optinized implementation.
#define TRICE64_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE64_1i( id, pFmt, d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE64_2i does trace id and 64-bit values unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! This is a time optinized implementation.
#define TRICE64_2i( id, pFmt, d0, d1 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(16) ); \
    TRICE_HTON_U64PUSH( d0 ); \
    TRICE_HTON_U64PUSH( d1 ); \
} while(0)

//! TRICE64_2 does trace id and 64-bit values protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! This is a time optinized implementation.
#define TRICE64_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE64_2i( id, pFmt, d0, d1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

#if 0
//! string transfer format: 
//!     id       count    cycle <--- id value in trice transfer order
//! c0     c1     c2       c3
//! ...
//! cLen-3 cLen-2 cLen-1   cLen
//! \todo code needs to be checked on a big endian machine.
TRICE_INLINE void trice_s(uint32_t id, char *s) {
    TRICE_ENTER_CRITICAL_SECTION
    int i = 0;
    int len = strlen( s );
    if( 65535 < len ){ 
        s[65535] = 0; // truncate
        len = 65535;
    }
    if( len <= 4 ){
        TRICE_HTON_U32PUSH( id|(len<<8)|triceCycle ); // on PC side the id reception gives the TRICE_S and the format string information
    }else{
        TRICE_HTON_U32PUSH( id|(0x7<<8)|triceCycle ); // on PC side the id reception gives the TRICE_S and the format string information
        TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(len) );
    }
    triceCycle++;
    while( 3 < len ){
        uint32_t* pos = (uint32_t*)(s+i);
        TRICE_U32PUSH( *pos );
        len -= 4;
        i += 4;
    }
    switch( len ){
        case 0: 
            break;
        case 1: 
            TRICE_U32PUSH( s[i+0] );
            break;
        case 2: 
            TRICE_U32PUSH( ((s[i+1]<<8)|s[i+0]) );
            break;
        case 3: 
            TRICE_U32PUSH( (s[i+2]<<16) | (s[i+1]<<8) | s[i+0] );
            break;
    }
    TRICE_LEAVE_CRITICAL_SECTION
}

//! Write id and dynString protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice (ignored here but used by the trice tool)
//! \param dynString 0-terminated runtime generated string
//! After the 4 byte trice message header are following 2^n bytes 
#define TRICE_S(id, pFmt, dynString) do{ trice_s(id, dynString); }while(0)
#endif

#endif // legacy

#ifdef __cplusplus
}
#endif

#endif /* TRICE_COBSR_ENCODER_H_ */
