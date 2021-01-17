/*! \file tricePackEncoder.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_PACK_ENCODER_H_
#define TRICE_PACK_ENCODER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#define Id(n) (((uint32_t)(n))<<16) //!< Prepare ID for transmission

extern uint8_t triceCycle;

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//
/*
Transfer is in network order (big endian) or little endian acording TRICE_TRANSFER_ENDIANESS.
Inside a 32-bit sequence the 16-bit ID comes together with 16-bit count in specified transfer endianess:
This count is the payload size without counting padding bytes. The ID is in the 2 msb and the count in the 2 lsb.
\code
head = (id<<16)| (count<<8) | cycle
\endcode
8-bit values follow packt as uint32_t in specified transfer endianess:
\code
 0  0  0 b0 // TRICE8_1
 0  0 b0 b1 // TRICE8_2
 0 b0 b1 b2 // TRICE8_3
b0 b1 b2 b3 // TRICE8_4
b0 b1 b2 b3  0  0  0 b4 // TRICE8_5
b0 b1 b2 b3  0  0 b4 b5 // TRICE8_6
b0 b1 b2 b3  0 b4 b5 b6 // TRICE8_7
b0 b1 b2 b3 b4 b5 b6 b7 // TRICE8_8
\endcode
16-bit values follow in sequence optionally with not counted 2 padding bytes forming a 32-bit aligment.
*/

#define TRICE_SYNC do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( 0x89abcdef ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id protected (outside critical section), 16 bit data are 0
//! \param id trice identifier
//! \param pFmt formatstring for trice
#define TRICE0( id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|triceCycle++ ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TRICE8_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0100|triceCycle++ ); \
    TRICE_HTON_U32PUSH( (uint8_t)(d0) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

// 0x44332211

//! trace id and 8-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE8_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0200|triceCycle++ ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0,d1) ); \
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
    TRICE_HTON_U32PUSH( id|0x0300|triceCycle++ ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN( 0,d0)<<16) | TRICE_U8_JOIN(d1,d2) ); /*TRICE_HTON_U32PUSH( ((uint8_t)((uint32_t)(d0)<<16)) | TRICE_U8_JOIN(d1,d2) ); */ \
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
    TRICE_HTON_U32PUSH( id|0x0400|triceCycle++ ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
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
    TRICE_HTON_U32PUSH( id|0x0500|triceCycle++ ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_HTON_U32PUSH( (uint8_t)(d4) ); \
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
    TRICE_HTON_U32PUSH( id|0x0600|triceCycle++ ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d4,d5) ); \
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
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle++ ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN( 0,d4)<<16) | TRICE_U8_JOIN(d5,d6) ); \
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
    TRICE_HTON_U32PUSH( id|0x0800|triceCycle++ ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d4,d5)<<16) | TRICE_U8_JOIN(d6,d7) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 16-bit value protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE16_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0200|triceCycle++ ); \
    TRICE_HTON_U32PUSH( (uint16_t)d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 16-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE16_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0400|triceCycle++ ); \
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d0,d1) ); \
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
    TRICE_HTON_U32PUSH( id|0x0600|triceCycle++ ); \
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d0,d1) ); \
    TRICE_HTON_U32PUSH( (uint16_t)(d2) ); \
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
    TRICE_HTON_U32PUSH( id|0x0800|triceCycle++ ); \
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d0,d1) ); \
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d2,d3) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 32-bit value protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE32_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0400|triceCycle++ ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 32-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE32_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0800|triceCycle++ ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( d1 ); \
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
    TRICE_HTON_U32PUSH( id|0x0c00|triceCycle++ ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( d1 ); \
    TRICE_HTON_U32PUSH( d2 ); \
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
    TRICE_HTON_U32PUSH( id|0x1000|triceCycle++ ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( d1 ); \
    TRICE_HTON_U32PUSH( d2 ); \
    TRICE_HTON_U32PUSH( d3 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

#if TRICE_TRANSFER_ENDIANESS == TRICE_BIG_ENDIANESS

//! trace id and 64-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0800|triceCycle++ ); \
    TRICE_HTON_U32PUSH( (uint64_t)(d0)>>32 ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 64-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE64_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x1000|triceCycle++ ); \
    TRICE_HTON_U32PUSH( (uint64_t)(d0)>>32 ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( (uint64_t)(d1)>>32 ); \
    TRICE_HTON_U32PUSH( d1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

#else // TRICE_TRANSFER_ENDIANESS == TRICE_LITTLE_ENDIANESS

//! trace id and 64-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0800|triceCycle++ ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( (uint64_t)(d0)>>32 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace id and 64-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE64_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x1000|triceCycle++ ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( (uint64_t)(d0)>>32 ); \
    TRICE_HTON_U32PUSH( d1 ); \
    TRICE_HTON_U32PUSH( (uint64_t)(d1)>>32 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

#endif

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
    if( 255 < len ){ 
        s[255] = 0; // truncate
        len = 255;
    }
    TRICE_HTON_U32PUSH( id|(len<<8)|triceCycle++ ); // on PC side the id reception gives the TRICE_S and the format string information
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

#endif /* TRICE_PACK_ENCODER_H_ */
