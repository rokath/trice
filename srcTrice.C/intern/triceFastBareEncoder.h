/*! \file triceFastBare.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_FAST_BARE_H_
#define TRICE_FAST_BARE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#define TRICE_S(id, pFmt, dynString) do{ TRICE0( Id(28489), "wrn:TRICES_1(id, pFmt, dynString) macro is not supported in bare encoding.\nmsg:See TRICE_RTS macro in triceCheck.c for an alternative or use a different encoding.\n"); }while(0)


#define Id(n) ((uint32_t)(n)<<16) //!< Macro for improved trice readability and better source code parsing.


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

//! trace Id protected (outside critical section), 16 bit data are 0
//! \param Id trice identifier
//! \param pFmt formatstring for trice
#define TRICE0( Id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH(Id); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TRICE8_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( Id | (uint8_t)d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE8_2( Id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( Id | TRICE_U8_JOIN(d0, d1) ); \
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
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0, d1) ); \
    TRICE_HTON_U32PUSH( Id | (uint8_t)(d2) ); \
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
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0, d1) ); \
    TRICE_HTON_U32PUSH( Id | TRICE_U8_JOIN(d2, d3) ); \
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
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0, d1) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d2, d3) ); \
    TRICE_HTON_U32PUSH( Id | (uint8_t)(d4) ); \
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
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0, d1) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d2, d3) ); \
    TRICE_HTON_U32PUSH( Id | TRICE_U8_JOIN(d4, d5) ); \
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
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0, d1) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d2, d3) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d4, d5) ); \
    TRICE_HTON_U32PUSH( Id | (uint8_t)(d6) ); \
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
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0, d1) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d2, d3) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d4, d5) ); \
    TRICE_HTON_U32PUSH( Id | TRICE_U8_JOIN(d6, d7) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 16-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE16_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( Id | (uint16_t)d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
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

//! trace Id and 16-bit values protected (outside critical section)
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

//! trace Id and 16-bit values protected (outside critical section)
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

//! trace Id and 32-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE32_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( (uint32_t)(d0)>>16 ); \
    TRICE_HTON_U32PUSH( Id | (uint16_t)d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
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

//! trace Id and 32-bit values protected (outside critical section)
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

//! trace Id and 32-bit values protected (outside critical section)
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

//! trace Id and 64-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( Id, pFmt, d0 ) do{ \
    uint64_t x = (uint64_t)(d0); \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( (uint16_t)( x>>48 )); /*hh*/ \
    TRICE_HTON_U32PUSH( (uint16_t)( x>>32 )); /*hl*/\
    TRICE_HTON_U32PUSH( (uint16_t)( x>>16 )); /*lh*/ \
    TRICE_HTON_U32PUSH( Id | (uint16_t)x ); /*ll*/ \
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

#ifdef __cplusplus
}
#endif

#endif /* TRICE_BARE_H_ */
