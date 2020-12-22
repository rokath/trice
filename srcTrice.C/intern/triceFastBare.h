/*! \file triceFastBare.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_FAST_BARE_H_
#define TRICE_FAST_BARE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "trice.h"

#if TRICE_FAST_BARE_SYNC_ENCODING == TRICE_ENCODING \
 || TRICE_FAST_BARE_WRAP_ENCODING == TRICE_ENCODING


#define Id(n) ((n##u)<<16) //!< Macro for improved trice readability and better source code parsing.


///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//
/*
Inside a 32 bit sequence the 16 bit ID comes first
When several data, the real ID comes in the last 32 bit sequence.
*/


#define TRICE_JOIN_U8( first, second)(((uint16_t)(first)<< 8)|(uint8_t)(second))

//! basic trice macro
//! \param id a 16 bit trice identifier, goes into upper 2 bytes to be transmitted first
//! \param d16 a 16 bit value
#define TRICE_JOINEDPUSH( id, d16 ) do{ \
    TRICE_U32PUSH( id | (uint16_t)(d16)); \
} while(0)


//! trace Id protected (outside critical section), 16 bit data are 0
//! \param Id trice identifier
//! \param pFmt formatstring for trice
#define TRICE0( Id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH(Id); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TRICE8_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_JOINEDPUSH( Id, d0 ); \
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
    TRICE_U32PUSH( (uint16_t)TRICE_JOIN_U8(d0, d1 )); \
    TRICE_JOINEDPUSH( Id, (uint16_t)(d2)<<8 ); \
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
    TRICE_U32PUSH( (uint16_t)TRICE_JOIN_U8(d0, d1 )); \
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
    TRICE_U32PUSH( (uint16_t)TRICE_JOIN_U8(d0, d1 )); \
    TRICE_U32PUSH( (uint16_t)TRICE_JOIN_U8(d2, d3 )); \
    TRICE_JOINEDPUSH( Id, (uint16_t)(d4)<<8 ); \
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
    TRICE_U32PUSH( (uint16_t)TRICE_JOIN_U8(d0, d1 )); \
    TRICE_U32PUSH( (uint16_t)TRICE_JOIN_U8(d2, d3 )); \
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
    TRICE_U32PUSH( (uint16_t)TRICE_JOIN_U8(d0, d1 )); \
    TRICE_U32PUSH( (uint16_t)TRICE_JOIN_U8(d2, d3 )); \
    TRICE_U32PUSH( (uint16_t)TRICE_JOIN_U8(d4, d5 )); \
    TRICE_JOINEDPUSH( Id, (uint16_t)(d6)<<8 ); \
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
    TRICE_U32PUSH( (uint16_t)TRICE_JOIN_U8(d0, d1 )); \
    TRICE_U32PUSH( (uint16_t)TRICE_JOIN_U8(d2, d3 )); \
    TRICE_U32PUSH( (uint16_t)TRICE_JOIN_U8(d4, d5 )); \
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
    TRICE_U32PUSH( (uint16_t)(d0) ); \
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
    TRICE_U32PUSH( (uint16_t)(d0) ); \
    TRICE_U32PUSH( (uint16_t)(d1) ); \
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
    TRICE_U32PUSH( (uint16_t)(d0) ); \
    TRICE_U32PUSH( (uint16_t)(d1) ); \
    TRICE_U32PUSH( (uint16_t)(d2) ); \
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
    TRICE_U32PUSH(x>>16); \
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
    TRICE_U32PUSH(x0>>16); \
    TRICE_U32PUSH((uint16_t)(x0)); \
    TRICE_U32PUSH(x1>>16); \
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
    TRICE_U32PUSH(x0>>16); \
    TRICE_U32PUSH((uint16_t)(x0)); \
    TRICE_U32PUSH(x1>>16); \
    TRICE_U32PUSH((uint16_t)(x1)); \
    TRICE_U32PUSH(x2>>16); \
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
    TRICE_U32PUSH(x0>>16);\
    TRICE_U32PUSH((uint16_t)(x0)); \
    TRICE_U32PUSH(x1>>16); \
    TRICE_U32PUSH((uint16_t)(x1)); \
    TRICE_U32PUSH(x2>>16); \
    TRICE_U32PUSH((uint16_t)(x2)); \
    TRICE_U32PUSH( x3>>16 ); \
    TRICE_JOINEDPUSH( Id, (uint16_t)x3 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 64-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( Id, pFmt, d0 ) do{ \
    uint64_t x = (uint64_t)d0; \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( (uint16_t)( x>>48 )); /*hh*/ \
    TRICE_U32PUSH( (uint16_t)( x>>32 )); /*hl*/\
    TRICE_U32PUSH( (uint16_t)( x>>16 )); /*lh*/ \
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
    TRICE_U32PUSH( (uint16_t)(x0>>48 )); /*hh*/ \
    TRICE_U32PUSH( (uint16_t)(x0>>32 )); /*hl*/ \
    TRICE_U32PUSH( (uint16_t)(x0>>16 )); /*lh*/ \
    TRICE_U32PUSH( (uint16_t)(x0 ));     /*ll*/ \
    TRICE_U32PUSH( (uint16_t)(x1>>48 )); /*hh*/ \
    TRICE_U32PUSH( (uint16_t)(x1>>32 )); /*hl*/ \
    TRICE_U32PUSH( (uint16_t)(x1>>16 )); /*lh*/ \
    TRICE_JOINEDPUSH( Id, x1 ); /*ll*/ \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)



//! string transfer format compatible to bare:
//! IH     IL     lenH     LenL
//! c0     c1     c2       c3
//! ...
//! cLen-3 cLen-2 cLen-1   cLen
//! optionally sync package sare injected
TRICE_INLINE void trice_s(uint32_t Id, char *s) {
    TRICE_ENTER_CRITICAL_SECTION
    int i = 0;
    int len = strlen( s );
    if( 65535 < len ){
        s[65535] = 0; // truncate
        len = 65535;
    }
    TRICE_JOINEDPUSH( Id, len ); // on PC side the Id reception gives the TRICE_S and the format string information
    while( 3 < len ){
        TRICE_U32PUSH( ((uint32_t)(s[i+0]) << 24)|((uint32_t)(s[i+1]) << 16)|((uint32_t)(s[i+2]) << 8)|(uint32_t)(s[i+3]) );
        len -= 4;
        i += 4;
    }
    switch( len ){
        case 0: 
            break;
        case 1: TRICE_U32PUSH( ((uint32_t)(s[i+0]) << 24) ); 
            break;
        case 2: TRICE_U32PUSH( ((uint32_t)(s[i+0]) << 24)|((uint32_t)(s[i+1]) << 16) ); 
            break;
        case 3: TRICE_U32PUSH( ((uint32_t)(s[i+0]) << 24)|((uint32_t)(s[i+1]) << 16)|((uint32_t)(s[i+2]) << 8) );
            break;
    }
    TRICE_LEAVE_CRITICAL_SECTION
}

//! Write Id and dynString protected (outside critical section).
//! \param Id trice identifier
//! \param pFmt formatstring for trice (ignored here but used by the trice tool)
//! \param dynString 0-terminated runtime generated string
//! After the 4 byte trice message header are following 2^n bytes 
#define TRICE_S(Id, pFmt, dynString) do{ trice_s(Id, dynString); }while(0)


//! for performance no check of strlen( s ) here (internal usage)
static void triceRuntimeGeneratedStringUnbound( const char* s ){
    size_t len = strlen( s );
    char c1, c2, c3, c4, c5, c6, c7, c8;
    while( len ){
        switch( len ){
            case  0: return;
            case  1: c1=*s++;
                TRICE8_1( Id(36152), "%c", c1 ); return;
            case  2: c1=*s++; c2=*s++;
                TRICE8_2( Id(49862), "%c%c", c1, c2 ); return;
            case  3: c1=*s++; c2=*s++; c3=*s++;
                TRICE8_3( Id(60898), "%c%c%c", c1, c2, c3 ); return;
            case  4: c1=*s++; c2=*s++; c3=*s++; c4=*s++;
                TRICE8_4( Id(57970), "%c%c%c%c", c1, c2, c3, c4 ); return;
            case  5: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++;
                TRICE8_5( Id(49813), "%c%c%c%c%c", c1, c2, c3, c4, c5 ); return;
            case  6: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++; c6=*s++;
                TRICE8_6( Id(10201), "%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6 ); return;
            case  7: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++; c6=*s++; c7=*s++;
                TRICE8_7( Id(57439), "%c%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6, c7); return;
            case  8:
            default: c1 = *s++; c2 = *s++; c3 = *s++; c4 = *s++; c5 = *s++; c6 = *s++; c7 = *s++; c8 = *s++;
                TRICE8_8( Id(53018), "%c%c%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6, c7, c8 );
                len -= 8;
        }
    }
    return;
}

//! trice runtime string
#define TRICE_RTS(dynString) do{ triceRuntimeGeneratedStringUnbound(dynString); }while(0)


#endif // #if TRICE_FAST_BARE_SYNC_ENCODING == TRICE_ENCODING || TRICE_FAST_BARE_WRAP_ENCODING == TRICE_ENCODING

#ifdef __cplusplus
}
#endif

#endif /* TRICE_BARE_H_ */
