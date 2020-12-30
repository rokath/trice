/*! \file tricePack.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_PACK_H_
#define TRICE_PACK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

//#if TRICE_PACK_ENCODING == TRICE_ENCODING

#define Id(n) (((uint32_t)(n))<<16) //!< Prepare ID for transmission

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//
/*
Transfer is in network order (big endian).
Inside a 32-bit sequence the 16-bit ID comes first, followed by an 16-bit count.
This count is the payload size.
8-bit values follow in sequence optionally with not counted 1-3 padding bytes forming a 32-bit aligment.
16-bit values follow in sequence optionally with not counted 2 padding bytes forming a 32-bit aligment.
*/

#define TRICE_U8_JOIN( first, second ) ((uint16_t)((((uint16_t)(second))<< 8)|((uint8_t)(first)))) //!< helper macro

//! trace id protected (outside critical section), 16 bit data are 0
//! \param id trice identifier
//! \param pFmt formatstring for trice
#define TRICE0( id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TRICE8_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|1) ); \
    TRICE_U32PUSH((uint8_t)(d0)); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 8-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE8_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|2) ); \
    TRICE_U32PUSH( TRICE_U8_JOIN(d0,d1) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 8-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE8_3( id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|3) ); \
    TRICE_U32PUSH( TRICE_U8_JOIN(d0,d1) | ((uint8_t)(d2)<<16) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 8-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE8_4( id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|4) ); \
    TRICE_U32PUSH( TRICE_U8_JOIN(d0,d1) | ((uint32_t)TRICE_U8_JOIN(d2,d3)<<16) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 8-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
#define TRICE8_5( id, pFmt, d0, d1, d2, d3, d4 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|5) ); \
    TRICE_U32PUSH( TRICE_U8_JOIN(d0,d1) | ((uint32_t)TRICE_U8_JOIN(d2,d3)<<16) ); \
    TRICE_U32PUSH(((uint8_t)(d4))); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 8-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
#define TRICE8_6( id, pFmt, d0, d1, d2, d3, d4, d5 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|6) ); \
    TRICE_U32PUSH( TRICE_U8_JOIN(d0,d1) | ((uint32_t)TRICE_U8_JOIN(d2,d3)<<16) ); \
    TRICE_U32PUSH( TRICE_U8_JOIN(d4,d5) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 8-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
#define TRICE8_7( id, pFmt, d0, d1, d2, d3, d4, d5, d6 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|7) ); \
    TRICE_U32PUSH( TRICE_U8_JOIN(d0,d1) | ((uint32_t)TRICE_U8_JOIN(d2,d3)<<16) ); \
    TRICE_U32PUSH( TRICE_U8_JOIN(d4,d5) | ((uint32_t)TRICE_U8_JOIN(d6, 0)<<16) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 8-bit values protected (outside critical section)
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
#define TRICE8_8( id, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|8) ); \
    TRICE_U32PUSH( TRICE_U8_JOIN(d0,d1) | ((uint32_t)TRICE_U8_JOIN(d2,d3)<<16) ); \
    TRICE_U32PUSH( TRICE_U8_JOIN(d4,d5) | ((uint32_t)TRICE_U8_JOIN(d6,d7)<<16) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 16-bit value protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE16_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|2) ); \
    TRICE_U32PUSH( TRICE_HTON((uint32_t)(d0)<<16) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 16-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE16_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|4) ); \
    TRICE_U32PUSH( TRICE_HTON(((uint32_t)(d0)<<16)| (uint16_t)(d1) )); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 16-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE16_3( id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|6) ); \
    TRICE_U32PUSH( TRICE_HTON(((uint32_t)(d0)<<16)| (uint16_t)(d1)) ); \
    TRICE_U32PUSH( TRICE_HTON((uint32_t)(d0)<<16) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 16-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE16_4( id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|8) ); \
    TRICE_U32PUSH( TRICE_HTON(((uint32_t)(d0)<<16)| (uint16_t)(d1)) ); \
    TRICE_U32PUSH( TRICE_HTON(((uint32_t)(d2)<<16)| (uint16_t)(d3)) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 32-bit value protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE32_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|4) ); \
    TRICE_U32PUSH( TRICE_HTON(d0)); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 32-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE32_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|8) ); \
    TRICE_U32PUSH( TRICE_HTON(d0) ); \
    TRICE_U32PUSH( TRICE_HTON(d1) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 32-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE32_3( id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|12) ); \
    TRICE_U32PUSH( TRICE_HTON(d0) ); \
    TRICE_U32PUSH( TRICE_HTON(d1) ); \
    TRICE_U32PUSH( TRICE_HTON(d2) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 32-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE32_4( id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|16)  ); \
    TRICE_U32PUSH( TRICE_HTON(d0) ); \
    TRICE_U32PUSH( TRICE_HTON(d1) ); \
    TRICE_U32PUSH( TRICE_HTON(d2) ); \
    TRICE_U32PUSH( TRICE_HTON(d3) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 64-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|8)  ); \
    TRICE_U32PUSH( TRICE_HTON((uint64_t)(d0)>>32) ); \
    TRICE_U32PUSH( TRICE_HTON(d0) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 64-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE64_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_U32PUSH( TRICE_HTON(id|16) ); \
    TRICE_U32PUSH( TRICE_HTON((uint64_t)(d0)>>32) ); \
    TRICE_U32PUSH( TRICE_HTON(d0) ); \
    TRICE_U32PUSH( TRICE_HTON((uint64_t)(d1)>>32) ); \
    TRICE_U32PUSH( TRICE_HTON(d1) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)



//! string transfer format compatible to bare:
//! IH     IL     lenH     LenL
//! c0     c1     c2       c3
//! ...
//! cLen-3 cLen-2 cLen-1   cLen
//! optionally sync package sare injected
TRICE_INLINE void trice_s(uint32_t id, char *s) {
    TRICE_ENTER_CRITICAL_SECTION
    int i = 0;
    int len = strlen( s );
    if( 65535 < len ){
        s[65535] = 0; // truncate
        len = 65535;
    }
    TRICE_U32PUSH(  TRICE_HTON(id|len) ); // on PC side the id reception gives the TRICE_S and the format string information
    while( 3 < len ){
        uint32_t* pos = (uint32_t*)(s+i);
        TRICE_U32PUSH( *pos );
      //TRICE_U32PUSH( ((uint32_t)(s[i+0]) << 24)|((uint32_t)(s[i+1]) << 16)|((uint32_t)(s[i+2]) << 8)|(uint32_t)(s[i+3]) );
        len -= 4;
        i += 4;
    }
    switch( len ){
        case 0: 
            break;
        case 1: 
            TRICE_U32PUSH( s[i+0]);
            break;
        case 2: 
            TRICE_U32PUSH( TRICE_U8_JOIN(s[i+0],s[i+1]) ); \
            break;
        case 3: 
            TRICE_U32PUSH( TRICE_U8_JOIN(s[i+0],s[i+1]) | (s[i+2]<<16) );
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

/*
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
*/

//#endif // #if TRICE_PACK_ENCODING == TRICE_ENCODING

#ifdef __cplusplus
}
#endif

#endif /* TRICE_PACK_H_ */
