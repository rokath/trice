/*! \file triceBare.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_BARE_H_
#define TRICE_BARE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include "SEGGER_RTT.h"

#define TRICE_S(id, pFmt, dynString) do{ TRICE0( Id(28489), "wrn:TRICES_1(id, pFmt, dynString) macro is not supported in bare encoding.\nmsg:See TRICE_RTS macro in triceCheck.c for an alternative or use a different encoding.\n"); }while(0)


#define Id(n) (((uint32_t)(n))<<16) //!< Macro for improved trice readability and better source code parsing. n must not exceed 2^16-1.

#if TRICE_COMPILE == TRICE_SPEED_OVER_SPACE

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//

#define TRICE_SYNC do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( 0x89abcdef ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


/*
Inside a 32 bit sequence the 16 bit ID comes first
When several data, the real ID comes in the last 32 bit sequence.
*/

//! trice Id protected (outside critical section), 16 bit data are 0
//! \param Id trice identifier
//! \param pFmt formatstring for trice
#define TRICE0( Id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH(Id); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TRICE8_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( Id | (uint8_t)d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE8_2( Id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( Id | TRICE_U8_JOIN(d0, d1) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE8_3( Id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0, d1) ); \
    TRICE_HTON_U32PUSH( Id | (uint8_t)(d2) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE8_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0, d1) ); \
    TRICE_HTON_U32PUSH( Id | TRICE_U8_JOIN(d2, d3) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
#define TRICE8_5( Id, pFmt, d0, d1, d2, d3, d4 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0, d1) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d2, d3) ); \
    TRICE_HTON_U32PUSH( Id | (uint8_t)(d4) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
#define TRICE8_6( Id, pFmt, d0, d1, d2, d3, d4, d5 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0, d1) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d2, d3) ); \
    TRICE_HTON_U32PUSH( Id | TRICE_U8_JOIN(d4, d5) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
#define TRICE8_7( Id, pFmt, d0, d1, d2, d3, d4, d5, d6 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0, d1) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d2, d3) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d4, d5) ); \
    TRICE_HTON_U32PUSH( Id | (uint8_t)(d6) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
#define TRICE8_8( Id, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0, d1) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d2, d3) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d4, d5) ); \
    TRICE_HTON_U32PUSH( Id | TRICE_U8_JOIN(d6, d7) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 16-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE16_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( Id | (uint16_t)d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE16_2( Id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( (uint16_t)(d0) ); /* This cast ensures that padding bytes are 0 */ \
    TRICE_HTON_U32PUSH( Id | (uint16_t)d1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE16_3( Id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( (uint16_t)(d0) ); /* This cast ensures that padding bytes are 0 */ \
    TRICE_HTON_U32PUSH( (uint16_t)(d1) ); /* This cast ensures that padding bytes are 0 */ \
    TRICE_HTON_U32PUSH( Id | (uint16_t)d2 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE16_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( (uint16_t)(d0) ); /* This cast ensures that padding bytes are 0 */ \
    TRICE_HTON_U32PUSH( (uint16_t)(d1) ); /* This cast ensures that padding bytes are 0 */ \
    TRICE_HTON_U32PUSH( (uint16_t)(d2) ); /* This cast ensures that padding bytes are 0 */ \
    TRICE_HTON_U32PUSH( Id | (uint16_t)d3 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 32-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE32_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( (uint32_t)(d0)>>16 ); \
    TRICE_HTON_U32PUSH( Id | (uint16_t)d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE32_2( Id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( (uint32_t)(d0)>>16 ); \
    TRICE_HTON_U32PUSH( (uint16_t)(d0) ); \
    TRICE_HTON_U32PUSH( (uint32_t)(d1)>>16 ); \
    TRICE_HTON_U32PUSH( Id | (uint16_t)d1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE32_3( Id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( (uint32_t)(d0)>>16 ); \
    TRICE_HTON_U32PUSH( (uint16_t)(d0) ); \
    TRICE_HTON_U32PUSH( (uint32_t)(d1)>>16 ); \
    TRICE_HTON_U32PUSH( (uint16_t)(d1) ); \
    TRICE_HTON_U32PUSH( (uint32_t)(d2)>>16 ); \
    TRICE_HTON_U32PUSH( Id | (uint16_t)d2 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE32_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( (uint32_t)(d0)>>16 ); \
    TRICE_HTON_U32PUSH( (uint16_t)(d0) ); \
    TRICE_HTON_U32PUSH( (uint32_t)(d1)>>16 ); \
    TRICE_HTON_U32PUSH( (uint16_t)(d1) ); \
    TRICE_HTON_U32PUSH( (uint32_t)(d2)>>16 ); \
    TRICE_HTON_U32PUSH( (uint16_t)(d2) ); \
    TRICE_HTON_U32PUSH( (uint32_t)(d3)>>16 ); \
    TRICE_HTON_U32PUSH( Id | (uint16_t)d3 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 64-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( Id, pFmt, d0 ) do{ \
    uint64_t d64 = (uint64_t)(d0); \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( (uint16_t)( d64>>48 )); /*hh*/ \
    TRICE_HTON_U32PUSH( (uint16_t)( d64>>32 )); /*hl*/\
    TRICE_HTON_U32PUSH( (uint16_t)( d64>>16 )); /*lh*/ \
    TRICE_HTON_U32PUSH( Id | (uint16_t)d64 ); /*ll*/ \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice Id and 64-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE64_2( Id, pFmt, d0, d1 ) do{ \
    uint64_t x0 = (uint64_t)d0, x1 = (uint64_t)d1; \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( (uint16_t)(x0>>48 )); /*hh*/ \
    TRICE_HTON_U32PUSH( (uint16_t)(x0>>32 )); /*hl*/ \
    TRICE_HTON_U32PUSH( (uint16_t)(x0>>16 )); /*lh*/ \
    TRICE_HTON_U32PUSH( (uint16_t)(x0 ));     /*ll*/ \
    TRICE_HTON_U32PUSH( (uint16_t)(x1>>48 )); /*hh*/ \
    TRICE_HTON_U32PUSH( (uint16_t)(x1>>32 )); /*hl*/ \
    TRICE_HTON_U32PUSH( (uint16_t)(x1>>16 )); /*lh*/ \
    TRICE_HTON_U32PUSH( Id | (uint16_t)x1 );  /*ll*/ \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

#endif // TRICE_COMPILE == TRICE_SPEED_OVER_SPACE

#if TRICE_COMPILE == TRICE_SPACE_OVER_SPEED

///////////////////////////////////////////////////////////////////////////////
// internal trice functions
//

//! trice Id unprotected (inside critical section)
//! \param Id trice identifier
TRICE_INLINE void trice_0_ics( uint32_t Id ){
    TRICE_HTON_U32PUSH(Id);
}

//! trice Id protected (outside critical section)
//! \param Id trice identifier
TRICE_INLINE void trice_0_ocs( uint32_t Id ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_0_ics(Id);
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 8- or 16-bit value unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
TRICE_INLINE void trice_8_1_ics( uint32_t Id, uint32_t d0 ){
    TRICE_HTON_U32PUSH( Id | d0 );
}

//! trice Id and 8- or 16-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
TRICE_INLINE void trice_8_1_ocs( uint32_t Id, uint32_t d0 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_1_ics(Id, d0);
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id==0 and 8-bit values unprotected (inside critical section)
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_8_02_ics( uint32_t d0, uint32_t d1 ){
    TRICE_HTON_U32PUSH( (d0<<8) | d1 );
}

//! trice Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_8_2_ics( uint32_t Id, uint32_t d0, uint32_t d1 ){
    TRICE_HTON_U32PUSH( Id | (d0<<8) | d1 );
}

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_8_2_ocs( uint32_t Id, uint32_t d0, uint32_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_2_ics( Id, d0, d1 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TRICE_INLINE void trice_8_3_ics( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    trice_8_02_ics( d0, d1 );
    trice_8_1_ics( Id, d2 );
}

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TRICE_INLINE void trice_8_3_ocs( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_3_ics( Id, d0, d1, d2 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_8_4_ics( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    trice_8_02_ics( d0, d1 );
    trice_8_2_ics( Id, d2, d3 );
}

//! trice Id==0 and 8-bit values unprotected (inside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_8_04_ics( uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    trice_8_02_ics( d0, d1 );
    trice_8_02_ics( d2, d3 );
}

//! trice Id==0 and 8-bit values protected (outside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_8_4_ocs( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_4_ics( Id, d0, d1, d2, d3 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
TRICE_INLINE void trice_8_5_ics( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4 ){
    trice_8_04_ics( d0, d1, d2, d3 );
    trice_8_1_ics( Id, d4 );
}

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
TRICE_INLINE void trice_8_5_ocs( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_5_ics( Id, d0, d1, d2, d3, d4 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
TRICE_INLINE void trice_8_6_ics( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5 ){
    trice_8_04_ics( d0, d1, d2, d3 );
    trice_8_2_ics( Id, d4, d5 );
}

//! trice Id==0 and 8-bit values protected (outside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
TRICE_INLINE void trice_8_6_ocs( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_6_ics( Id, d0, d1, d2, d3, d4, d5 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
TRICE_INLINE void trice_8_7_ics( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6 ){
    trice_8_04_ics( d0, d1, d2, d3 );
    trice_8_3_ics( Id, d4, d5, d6 );
}

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
TRICE_INLINE void trice_8_7_ocs( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_7_ics( Id, d0, d1, d2, d3, d4, d5, d6 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
TRICE_INLINE void trice_8_8_ics( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7 ){
    trice_8_04_ics( d0, d1, d2, d3 );
    trice_8_4_ics( Id, d4, d5, d6, d7 );
}

//! trice Id==0 and 8-bit values protected (outside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
TRICE_INLINE void trice_8_8_ocs( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_8_ics( Id, d0, d1, d2, d3, d4, d5, d6, d7 );
    TRICE_LEAVE_CRITICAL_SECTION
}

#define trice_16_1_ics trice_8_1_ics //!< same code
#define trice_16_1_ocs trice_8_1_ocs //!< same code

//! trice Id==0 and 16-bit value unprotected (inside critical section)
//! \param d0 payload
TRICE_INLINE void trice_16_01_ics( uint32_t d0 ){
    TRICE_HTON_U32PUSH( d0 );
}

//! trice Id==0 and 8-bit values unprotected (inside critical section)
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_16_02_ics( uint32_t d0, uint32_t d1 ){
    trice_16_01_ics( d0 );
    trice_16_01_ics( d1 );
}

//! trice Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_16_2_ics( uint32_t Id, uint32_t d0, uint32_t d1 ){
    trice_16_01_ics( d0 );
    trice_16_1_ics( Id, d1 );
}

//! trice Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_16_2_ocs( uint32_t Id, uint32_t d0, uint32_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_16_2_ics( Id, d0, d1 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TRICE_INLINE void trice_16_3_ics( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    trice_16_02_ics( d0, d1 );
    trice_16_1_ics( Id, d2 );
}

//! trice Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TRICE_INLINE void trice_16_3_ocs( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_16_3_ics( Id, d0, d1, d2 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_16_4_ics( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    trice_16_02_ics( d0, d1 );
    trice_16_2_ics( Id, d2, d3 );
}

//! trice Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_16_4_ocs( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_16_4_ics( Id, d0, d1, d2, d3 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 32-bit value unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
TRICE_INLINE void trice_32_1_ics( uint32_t Id, uint32_t d0 ){
#if TRICE_TRANSFER_ENDIANESS == TRICE_LITTLE_ENDIANESS
    trice_16_01_ics( (uint16_t)d0 ); // lo
    trice_16_1_ics( Id, d0>>16 );    // hi
#elif TRICE_TRANSFER_ENDIANESS == TRICE_BIG_ENDIANESS
    trice_16_01_ics( d0>>16 );             // hi
    trice_16_1_ics( Id, (uint16_t)d0 ); // lo
#else
#error
#endif
}

//! trice Id and 32-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
TRICE_INLINE void trice_32_1_ocs( uint32_t Id, uint32_t d0 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_32_1_ics( Id, d0 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 32-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_32_2_ics( uint32_t Id, uint32_t d0, uint32_t d1 ){
#if TRICE_TRANSFER_ENDIANESS == TRICE_LITTLE_ENDIANESS
    trice_16_02_ics( (uint16_t)d0, d0>>16 );    // lo, hi
    trice_16_2_ics( Id, (uint16_t)d1, d1>>16 ); // lo, hi
#elif TRICE_TRANSFER_ENDIANESS == TRICE_BIG_ENDIANESS
    #error
    trice_16_02_ics( d0>>16, (uint16_t)d0);    // hi, lo
    trice_16_2_ics( Id, d1>>16, (uint16_t)d1); // hi, lo
#else
#error
#endif
}

//! trice Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_32_2_ocs( uint32_t Id, uint32_t d0, uint32_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_32_2_ics( Id, d0, d1 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 32-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TRICE_INLINE void trice_32_3_ics( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
#if TRICE_TRANSFER_ENDIANESS == TRICE_LITTLE_ENDIANESS
    trice_16_02_ics( (uint16_t)d0, d0>>16 );                           // lo, hi
    trice_16_4_ics( Id, (uint16_t)d1, d1>>16 , (uint16_t)d2, d2>>16 ); // lo, hi
#elif TRICE_TRANSFER_ENDIANESS == TRICE_BIG_ENDIANESS
    trice_16_02_ics( d0>>16, (uint16_t)d0 );                             // hi, lo
    trice_16_4_ics( Id, d1>>16, (uint16_t)d1, d2>>16, (uint16_t)d2 ); // hi, lo
#else
#error
#endif
}

//! trice Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TRICE_INLINE void trice_32_3_ocs( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_32_3_ics( Id, d0, d1, d2 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 32-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_32_4_ics( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
#if TRICE_TRANSFER_ENDIANESS == TRICE_LITTLE_ENDIANESS
    trice_16_4_ics(  0, (uint16_t)d0, d0>>16, (uint16_t)d1, d1>>16  ); // lo, hi
    trice_16_4_ics( Id, (uint16_t)d2, d2>>16, (uint16_t)d3, d3>>16  ); // lo, hi
#elif TRICE_TRANSFER_ENDIANESS == TRICE_BIG_ENDIANESS
    trice_16_4_ics(  0, d0>>16, (uint16_t)d0, d1>>16, (uint16_t)d1  ); // hi, lo
    trice_16_4_ics( Id, d2>>16, (uint16_t)d2, d3>>16, (uint16_t)d3  ); // hi, lo
#else
#error
#endif
}

//! trice Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_32_4_ocs( uint32_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_32_4_ics( Id, d0, d1, d2, d3 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 64-bit value unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
TRICE_INLINE void trice_64_1_ics( uint32_t Id, uint64_t d0 ){
#if TRICE_TRANSFER_ENDIANESS == TRICE_LITTLE_ENDIANESS
    trice_16_02_ics( (uint16_t)d0, (uint16_t)(d0>>16) );          // ll, lh
    trice_16_2_ics( Id, (uint16_t)(d0>>32), (uint16_t)(d0>>48 )); // hl, hh
#elif TRICE_TRANSFER_ENDIANESS == TRICE_BIG_ENDIANESS
#error
#else
#error
#endif
}

//! trice Id and 64-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_64_2_ics( uint32_t Id, uint64_t d0, uint64_t d1 ){
#if TRICE_TRANSFER_ENDIANESS == TRICE_LITTLE_ENDIANESS
    trice_16_02_ics( (uint16_t)d0, (uint16_t)(d0>>16) );
    trice_16_02_ics( (uint16_t)(d0>>32), (uint16_t)(d0>>48) );
    trice_16_02_ics( (uint16_t)d1, (uint16_t)(d1>>16) );
    trice_16_2_ics( Id, (uint16_t)(d1>>32), (uint16_t)(d1>>48 ));
#elif TRICE_TRANSFER_ENDIANESS == TRICE_BIG_ENDIANESS
#error
#else
#error
#endif
}

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//

//! trice Id protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
#define TRICE0( Id, pFmt ) do{ \
    trice_0_ocs( Id ); \
} while(0)

//! trice Id protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TRICE8_1( Id, pFmt, d0 ) do{ \
    trice_8_1_ocs( Id, (uint8_t)d0 ); \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE8_2( Id, pFmt, d0, d1 ) do{ \
    trice_8_2_ocs( Id, (uint8_t)d0, (uint8_t)d1 ); \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE8_3( Id, pFmt, d0, d1, d2 ) do{ \
    trice_8_3_ocs( Id, (uint8_t)d0, (uint8_t)d1, (uint8_t)d2 ); \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE8_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    trice_8_4_ocs( Id, (uint8_t)d0, (uint8_t)d1, (uint8_t)d2, (uint8_t)d3 ); \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
#define TRICE8_5( Id, pFmt, d0, d1, d2, d3, d4 ) do{ \
    trice_8_5_ocs( Id, (uint8_t)d0, (uint8_t)d1, (uint8_t)d2, (uint8_t)d3, (uint8_t)d4 ); \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
#define TRICE8_6( Id, pFmt, d0, d1, d2, d3, d4, d5 ) do{ \
    trice_8_6_ocs( Id, (uint8_t)d0, (uint8_t)d1, (uint8_t)d2, (uint8_t)d3, (uint8_t)d4, (uint8_t)d5 ); \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
#define TRICE8_7( Id, pFmt, d0, d1, d2, d3, d4, d5, d6 ) do{ \
    trice_8_7_ocs( Id, (uint8_t)d0, (uint8_t)d1, (uint8_t)d2, (uint8_t)d3, (uint8_t)d4, (uint8_t)d5, (uint8_t)d6 ); \
} while(0)

//! trice Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
#define TRICE8_8( Id, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 ) do{ \
    trice_8_8_ocs( Id, (uint8_t)d0, (uint8_t)d1, (uint8_t)d2, (uint8_t)d3, (uint8_t)d4, (uint8_t)d5, (uint8_t)d6, (uint8_t)d7 ); \
} while(0)

//! trice Id and 16-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE16_1( Id, pFmt, d0 ) do{ \
    trice_16_1_ocs( Id, (uint16_t)d0 ); \
} while(0)

//! trice Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE16_2( Id, pFmt, d0, d1 ) do{ \
    trice_16_2_ocs( Id, (uint16_t)d0, (uint16_t)d1 ); \
} while(0)

//! trice Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE16_3( Id, pFmt, d0, d1, d2 ) do{ \
    trice_16_3_ocs( Id, (uint16_t)d0, (uint16_t)d1, (uint16_t)d2 ); \
} while(0)

//! trice Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE16_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    trice_16_4_ocs( Id, (uint16_t)d0, (uint16_t)d1, (uint16_t)d2, (uint16_t)d3 ); \
} while(0)

//! trice Id and 32-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE32_1( Id, pFmt, d0 ) do{ \
    trice_32_1_ocs( Id, (uint32_t)d0 ); \
} while(0)

//! trice Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE32_2( Id, pFmt, d0, d1 ) do{ \
    trice_32_2_ocs( Id, (uint32_t)d0, (uint32_t)d1 ); \
} while(0)

//! trice Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE32_3( Id, pFmt, d0, d1, d2 ) do{ \
    trice_32_3_ocs( Id, (uint32_t)d0, (uint32_t)d1, (uint32_t)d2 ); \
} while(0)

//! trice Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE32_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    trice_32_4_ocs( Id, (uint32_t)d0, (uint32_t)d1, (uint32_t)d2, (uint32_t)d3 ); \
} while(0)

//! trice Id and 64-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( Id, pFmt, d0 ) do{ \
    trice_64_1_ocs( Id, (uint64_t)d0 ); \
} while(0)


//! trice Id and 64-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
TRICE_INLINE void trice_64_1_ocs( uint32_t Id, uint64_t d0 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_64_1_ics( Id, d0 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 64-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( Id, pFmt, d0 ) do{ \
    trice_64_1_ocs( Id, (uint64_t)d0 ); \
} while(0)

//! trice Id and 64-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_64_2_ocs( uint32_t Id, uint64_t d0, uint64_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_64_2_ics( Id, d0, d1 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trice Id and 64-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE64_2( Id, pFmt, d0, d1 ) do{ \
    trice_64_2_ocs( Id, (uint64_t)d0, (uint64_t)d1 ); \
} while(0)

#endif // TRICE_COMPILE == TRICE_SPACE_OVER_SPEED

#ifdef __cplusplus
}
#endif

#endif /* TRICE_BARE_H_ */
