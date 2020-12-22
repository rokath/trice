/*! \file triceFastBareL.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_FAST_BARE_L_H_
#define TRICE_FAST_BARE_L_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "trice.h"

#if TRICE_FAST_BARE_L_SYNC_ENCODING == TRICE_ENCODING \
 || TRICE_FAST_BARE_L_WRAP_ENCODING == TRICE_ENCODING

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//
/*
Inside a 32 bit sequence the 16 bit ID comes first
When several data, the real ID comes in the last 32 bit sequence.
*/

#define TRICE_JOIN_U16(first, second) ( (((uint32_t)((uint16_t)(first)))<<16) | (uint16_t)(second) ) 
#define TRICE_JOIN_U8( first, second) ( (((uint16_t)((uint8_t) (first)))<< 8) | (uint8_t) (second) ) 

//! basic trice macro
//! \param id a 16 bit trice identifier, goes into upper 2 bytes to be transmitted first
//! \param d16 a 16 bit value
#define TRICE_JOINEDPUSH( id, d16 ) do{ \
    TRICE_U32PUSH( TRICE_JOIN_U16(id, d16)); /* ((((uint32_t)(id))<<16)) | ((uint16_t)(d16))); */ \
} while(0)

//! basic trice macro, assumes d16 to be a 16 bit value
//! id is 0, goes into upper 2 bytes to be transmitted first
//! \param d16 a 16 bit value
#define TRICE_ID0( d16 ) do{ \
    TRICE_U32PUSH((uint16_t)(d16)); \
} while(0)

//! trace Id protected (outside critical section), 16 bit data are 0
//! \param Id trice identifier
//! \param pFmt formatstring for trice
#define TRICE0( Id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( ((uint32_t)(Id))<<16 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TRICE8_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_JOIN_U16( Id, d0 )); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE8_2( Id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_JOINEDPUSH( Id, TRICE_JOIN_U8(d0, d1 )); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE8_3( Id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( TRICE_JOIN_U8(d0, d1 )); \
    TRICE_U32PUSH( TRICE_JOIN_U16( Id, ((uint16_t)(d2))<<8 )); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE8_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( TRICE_JOIN_U8(d0, d1 )); \
    TRICE_JOINEDPUSH( Id, TRICE_JOIN_U8(d2, d3 )); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
#define TRICE8_5( Id, pFmt, d0, d1, d2, d3, d4 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( TRICE_JOIN_U8(d0, d1 )); \
    TRICE_ID0( TRICE_JOIN_U8(d2, d3 )); \
    TRICE_U32PUSH( TRICE_JOIN_U16( Id, ((uint16_t)(d4))<<8 )); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
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
    TRICE_ID0( TRICE_JOIN_U8(d0, d1 )); \
    TRICE_ID0( TRICE_JOIN_U8(d2, d3 )); \
    TRICE_JOINEDPUSH( Id, TRICE_JOIN_U8(d4, d5 )); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
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
    TRICE_ID0( TRICE_JOIN_U8(d0, d1 )); \
    TRICE_ID0( TRICE_JOIN_U8(d2, d3 )); \
    TRICE_ID0( TRICE_JOIN_U8(d4, d5 )); \
    TRICE_U32PUSH( TRICE_JOIN_U16( Id, ((uint16_t)(d6))<<8 )); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
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
    TRICE_ID0( TRICE_JOIN_U8(d0, d1 )); \
    TRICE_ID0( TRICE_JOIN_U8(d2, d3 )); \
    TRICE_ID0( TRICE_JOIN_U8(d4, d5 )); \
    TRICE_JOINEDPUSH( Id, TRICE_JOIN_U8(d6, d7 )); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 16-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE16_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_JOINEDPUSH( Id, d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE16_2( Id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( d0 ); \
    TRICE_JOINEDPUSH( Id, d1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE16_3( Id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( d0 ); \
    TRICE_ID0( d1 ); \
    TRICE_JOINEDPUSH( Id, d2 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE16_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( d0 ); \
    TRICE_ID0( d1 ); \
    TRICE_ID0( d2 ); \
    TRICE_JOINEDPUSH( Id, d3 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 32-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE32_1( Id, pFmt, d0 ) do{ \
    uint32_t x = (uint32_t)d0; \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( x>>16 ); \
    TRICE_JOINEDPUSH( Id, x ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE32_2( Id, pFmt, d0, d1 ) do{ \
    uint32_t x0 = (uint32_t)d0, x1 = (uint32_t)d1; \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( x0>>16 ); \
    TRICE_ID0( x0 ); \
    TRICE_ID0( x1>>16 ); \
    TRICE_JOINEDPUSH( Id, x1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE32_3( Id, pFmt, d0, d1, d2 ) do{ \
    uint32_t x0 = (uint32_t)d0, x1 = (uint32_t)d1, x2 = (uint32_t)d2; \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( x0>>16 ); \
    TRICE_ID0( x0 ); \
    TRICE_ID0( x1>>16 ); \
    TRICE_ID0( x1 ); \
    TRICE_ID0( x2>>16 ); \
    TRICE_JOINEDPUSH( Id, x2 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE32_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    uint32_t x0 = (uint32_t)d0, x1 = (uint32_t)d1, x2 = (uint32_t)d2, x3 = (uint32_t)d3; \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( x0>>16 ); \
    TRICE_ID0( x0 ); \
    TRICE_ID0( x1>>16 ); \
    TRICE_ID0( x1 ); \
    TRICE_ID0( x2>>16 ); \
    TRICE_ID0( x2 ); \
    TRICE_ID0( x3>>16 ); \
    TRICE_JOINEDPUSH( Id, x3 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 64-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( Id, pFmt, d0 ) do{ \
    uint64_t x = (uint64_t)d0; \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( x>>48 ); /*hh*/ \
    TRICE_ID0( x>>32 ); /*hl*/\
    TRICE_ID0( x>>16 ); /*lh*/ \
    TRICE_JOINEDPUSH( Id, x ); /*ll*/ \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 64-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE64_2( Id, pFmt, d0, d1 ) do{ \
    uint64_t x0 = (uint64_t)d0, x1 = (uint64_t)d1; \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( x0>>48 ); /*hh*/ \
    TRICE_ID0( x0>>32 ); /*hl*/ \
    TRICE_ID0( x0>>16 ); /*lh*/ \
    TRICE_ID0( x0 );     /*ll*/ \
    TRICE_ID0( x1>>48 ); /*hh*/ \
    TRICE_ID0( x1>>32 ); /*hl*/ \
    TRICE_ID0( x1>>16 ); /*lh*/ \
    TRICE_JOINEDPUSH( Id, x1 ); /*ll*/ \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

#endif // #if TRICE_FAST_BARE_SYNC_ENCODING == TRICE_ENCODING || TRICE_FAST_BARE_WRAP_ENCODING == TRICE_ENCODING

#ifdef __cplusplus
}
#endif

#endif /* TRICE_BARE_L_H_ */
