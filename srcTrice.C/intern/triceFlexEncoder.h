/*! \file tricePack2Encoder.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_FLEX_ENCODER_H_
#define TRICE_FLEX_ENCODER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#define id(n) ((((uint32_t)(n))<<(32-16)) & ~0x80000000) //!< Prepare 15-bit ID for transmission, bit 31 is 0
#define Id(n) ((((uint32_t)(n))<<(32-21)) |  0x80000000) //!< Prepare 20-bit ID for transmission, bit 31 is 1

//! TRICE_LONGCOUNT is an internal helper.
#define TRICE_LONGCOUNT( n ) (((n) << 16) | (0xffff & ~(n)) )

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


extern uint8_t triceCycle;

//! TRICE_SYNC can be used for checks or payload filling. In is an invisible trice message.
#define TRICE_SYNC do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( 0x89abcdef ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! TRICE0i does trace id unprotected (inside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! This is a time optinized implementation.
#define TRICE0i( id, pFmt ) do{ \
    TRICE_HTON_U32PUSH( id|triceCycle ); \
    triceCycle++; \
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
    TRICE_HTON_U32PUSH( id|0x0100|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( (uint8_t)(d0) ); \
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
    TRICE_HTON_U32PUSH( id|0x0200|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0,d1) ); \
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
    TRICE_HTON_U32PUSH( id|0x0300|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN( 0,d0)<<16) | TRICE_U8_JOIN(d1,d2) ); \
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
//! \param d0 payload
//! This is a time optinized implementation.
#define TRICE16_1i( id, pFmt, d0 ) do{ \
    TRICE_HTON_U32PUSH( id|0x0200|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( (uint16_t)d0 ); \
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


//! string transfer format: 
//!     id       count    cycle <--- id value in trice transfer order
//! c0     c1     c2       c3
//! ...
//! cLen-3 cLen-2 cLen-1   cLen
//! \todo code needs to be checked on a big endian mashine.
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


#ifdef __cplusplus
}
#endif

#endif /* TRICE_FLEX_ENCODER_H_ */
