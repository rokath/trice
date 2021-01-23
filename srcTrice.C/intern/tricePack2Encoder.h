/*! \file tricePack2Encoder.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_PACK2_ENCODER_H_
#define TRICE_PACK2_ENCODER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#define Id(n) (((uint32_t)(n))<<(32-20)) //!< Prepare 20-bit ID for transmission

extern uint8_t triceCycle;

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//
/*
Transfer is in network order (big endian) or little endian acording TRICE_TRANSFER_ENDIANESS.
Inside a 32-bit sequence the 24-bit ID i comes together with a 4-bit count c and a 8-bit cycle counter n in specified transfer endianess:
iiiiiiii iiiiiiii iiiicccc nnnnnnnn
The count c is the valid payload size without counting padding bytes and has valid values from 0 to 12.
c=0xd is indicating a longer 16 bit count l following in the next 4 bytes as 32-bit value:
llllllll llllllll LLLLLLLL LLLLLLLL, with L == ~l (all bits negated)
This coding gives some options for out-of-sync detection:
With 2^20 > 1000000 possible IDs there is a low chance to to hit a wrong id and a high chance to detect a miss.
The cycle counter must rotate continuously between 0...255.
If the count is >12 the long count is used but this is transmitted twice: as value and inverted value.
Also the optional 1-3 padding bytes are checked to be 0.
\code
head = (id<<12)| (count<<8) | cycle
\endcode

\li No parameters example:
\code
IIIII0NN // TRICE0
\endcode

\li 8-bit values follow packt as uint32_t in specified transfer endianess:
\code
IIIII1NN 000000b0          // TRICE8_1
IIIII2NN 0000b0b1          // TRICE8_2
IIIII3NN 00b0b1b2          // TRICE8_3
IIIII4NN b0b1b2b3          // TRICE8_4
IIIII5NN b0b1b2b3 000000b4 // TRICE8_5
IIIII6NN b0b1b2b3 0000b4b5 // TRICE8_6
IIIII7NN b0b1b2b3 00b4b5b6 // TRICE8_7
IIIII8NN b0b1b2b3 b4b5b6b7 // TRICE8_8
\endcode

\li 16-bit values follow in sequence optionally with not counted 2 padding bytes forming a 32-bit aligment.
\code
IIIII2NN 0000d0d0          // TRICE16_1
IIIII4NN d0d0d1d1          // TRICE16_2
IIIII6NN 0000d0d0 d1d1d2d2 // TRICE16_3
IIIII8NN d0d0d1d1 d1d1d2d2 // TRICE16_4
\endcode

\li 32-bit values follow in sequence optionally with a long count in case TRICE32_4.
\code
IIIII4NN d0d0d0d0                                    // TRICE32_1
IIIII8NN d0d0d0d0 d1d1d1d1                           // TRICE32_2
IIIIICNN d0d0d0d0 d1d1d1d1 d2d2d2d2                  // TRICE32_3
IIIIIDNN 0010FFEF d0d0d0d0 d1d1d1d1 d2d2d2d2 d3d3d3d3// TRICE32_4
\endcode

\li 64-bit values follow in sequence optionally with a long count in case TRICE64_2.
\code
IIIII8NN d0d0d0d0 d0d0d0d0                           // TRICE64_1
IIIIIDNN 0010FFEF d0d0d0d0d0d0d0d0 d1d1d1d1d1d1d1d1 // TRICE64_2
\endcode
*/

// TRICE_SYNC can be used for checks or payload filling. In is an invisible trice message.
#define TRICE_SYNC do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( 0x89abcdef ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

TRICE_INLINE void trice_sync( void ){
    TRICE_SYNC;
}

//! trace id protected (outside critical section), 16 bit data are 0
//! \param id trice identifier
//! \param pFmt formatstring for trice
#define TRICE0( id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|triceCycle ); \
    triceCycle++; \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

// trice_header is only for internal usage.
TRICE_INLINE void trice_header( uint32_t header ){
    TRICE_HTON_U32PUSH( header|triceCycle );
    triceCycle++; 
}

TRICE_INLINE void trice0( uint32_t id, char* pFmt ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id );
    TRICE_LEAVE_CRITICAL_SECTION
}
//! trace id protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TRICE8_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0100|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( (uint8_t)(d0) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


//! trice_18 is an internal helper.
TRICE_INLINE void trice_18( int8_t d0 ){
    TRICE_HTON_U32PUSH( (uint8_t)d0 );
}

TRICE_INLINE void trice8_1( uint32_t id, char* pFmt, int8_t d0 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0100 );
    trice_18( d0 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace id and 8-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE8_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0200|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0,d1) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


TRICE_INLINE void trice_28( int8_t d0 , int8_t d1 ){
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d0,d1) ); 
}

TRICE_INLINE void trice8_2( uint32_t id, char* pFmt, int8_t d0 , int8_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0200 );
    trice_28( d0, d1 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace id and 8-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE8_3( id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0300|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN( 0,d0)<<16) | TRICE_U8_JOIN(d1,d2) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


TRICE_INLINE void trice_38( int8_t d0 , int8_t d1, int8_t d2){
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN( 0,d0)<<16) | TRICE_U8_JOIN(d1,d2) );
}

TRICE_INLINE void trice8_3( uint32_t id, char* pFmt, int8_t d0 , int8_t d1, int8_t d2 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0300 );
    trice_38( d0, d1, d2 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace id and 8-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE8_4( id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0400|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice_48 is an internal helper.
TRICE_INLINE void trice_48( int8_t d0 , int8_t d1, int8_t d2, int8_t d3 ){
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) );
}

TRICE_INLINE void trice8_4( uint32_t id, char* pFmt, int8_t d0 , int8_t d1, int8_t d2, int8_t d3 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0400 );
    trice_48( d0, d1, d2, d3 );
    TRICE_LEAVE_CRITICAL_SECTION
}

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
    TRICE_HTON_U32PUSH( id|0x0500|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_HTON_U32PUSH( (uint8_t)(d4) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


TRICE_INLINE void trice8_5( uint32_t id, char* pFmt, int8_t d0 , int8_t d1, int8_t d2, int8_t d3, int8_t d4 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0500 );
    trice_48( d0, d1, d2, d3 );
    trice_18( d4 );
    TRICE_LEAVE_CRITICAL_SECTION
}

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
    TRICE_HTON_U32PUSH( id|0x0600|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_HTON_U32PUSH( TRICE_U8_JOIN(d4,d5) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


TRICE_INLINE void trice8_6( uint32_t id, char* pFmt, int8_t d0 , int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0600 );
    trice_48( d0, d1, d2, d3 );
    trice_28( d4, d5 );
    TRICE_LEAVE_CRITICAL_SECTION
}

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
    TRICE_HTON_U32PUSH( id|0x0700|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN( 0,d4)<<16) | TRICE_U8_JOIN(d5,d6) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

TRICE_INLINE void trice8_7( uint32_t id, char* pFmt, int8_t d0 , int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0700 );
    trice_48( d0, d1, d2, d3 );
    trice_38( d4, d5, d6 );
    TRICE_LEAVE_CRITICAL_SECTION
}

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
    TRICE_HTON_U32PUSH( id|0x0800|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d0,d1)<<16) | TRICE_U8_JOIN(d2,d3) ); \
    TRICE_HTON_U32PUSH( ((uint32_t)TRICE_U8_JOIN(d4,d5)<<16) | TRICE_U8_JOIN(d6,d7) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


TRICE_INLINE void trice8_8( uint32_t id, char* pFmt, int8_t d0 , int8_t d1, int8_t d2, int8_t d3, int8_t d4, int8_t d5, int8_t d6, int8_t d7 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0800 );
    trice_48( d0, d1, d2, d3 );
    trice_48( d4, d5, d6, d7 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace id and 16-bit value protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE16_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0200|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( (uint16_t)d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice_116 is an internal helper.
TRICE_INLINE void trice_116( int16_t d0 ){
    TRICE_HTON_U32PUSH( (uint16_t)d0 ); ;
}

TRICE_INLINE void trice16_1( uint32_t id, char* pFmt, int16_t d0 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0200 );
    trice_116( d0 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace id and 16-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE16_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0400|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d0,d1) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trice_216 is an internal helper.
TRICE_INLINE void trice_216( int16_t d0, int16_t d1 ){
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d0,d1) );
}

TRICE_INLINE void trice16_2( uint32_t id, char* pFmt, int16_t d0, int16_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0400 );
    trice_216( d0, d1 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace id and 16-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE16_3( id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0600|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d0,d1) ); \
    TRICE_HTON_U32PUSH( (uint16_t)(d2) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


TRICE_INLINE void trice16_3( uint32_t id, char* pFmt, int16_t d0, int16_t d1, int16_t d2 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0600 );
    trice_216( d0, d1 );
    trice_116( d2 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace id and 16-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE16_4( id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0800|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d0,d1) ); \
    TRICE_HTON_U32PUSH( TRICE_U16_JOIN(d2,d3) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

TRICE_INLINE void trice16_4( uint32_t id, char* pFmt, int16_t d0, int16_t d1, int16_t d2, int16_t d3 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0800 );
    trice_216( d0, d1 );
    trice_216( d2, d3 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace id and 32-bit value protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE32_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0400|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


//! trice_123 is an internal helper.
TRICE_INLINE void trice_132( int32_t d0 ){
    TRICE_HTON_U32PUSH( d0 ); ;
}

//! trice_223 is an internal helper.
TRICE_INLINE void trice_232( int32_t d0, uint32_t d1 ){
    trice_132( d0 );
    TRICE_HTON_U32PUSH( d1 );
}

//! trice_323 is an internal helper.
TRICE_INLINE void trice_332( int32_t d0, uint32_t d1, uint32_t d2 ){
    trice_232( d0, d1 );
    TRICE_HTON_U32PUSH( d2 );
}

//! trice_423 is an internal helper.
TRICE_INLINE void trice_432( int32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    trice_332( d0, d1, d2 );
    TRICE_HTON_U32PUSH( d3 );
}

TRICE_INLINE void trice32_1( uint32_t id, char* pFmt, int32_t d0 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0400 );
    trice_132( d0 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace id and 32-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE32_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0800|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( d1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

TRICE_INLINE void trice32_2( uint32_t id, char* pFmt, int32_t d0, int32_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0800 );
    trice_232( d0, d1 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace id and 32-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE32_3( id, pFmt, d0, d1, d2 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0c00|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( d1 ); \
    TRICE_HTON_U32PUSH( d2 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


TRICE_INLINE void trice32_3( uint32_t id, char* pFmt, int32_t d0, int32_t d1, int32_t d2 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0c00 );
    trice_332( d0, d1, d2 );
    TRICE_LEAVE_CRITICAL_SECTION
}

#define TRICE_LONGCOUNT( n ) (((n) << 16) | (0xffff & ~(n)) )

//! trace id and 32-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE32_4( id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0d00|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(16) ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( d1 ); \
    TRICE_HTON_U32PUSH( d2 ); \
    TRICE_HTON_U32PUSH( d3 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


TRICE_INLINE void trice32_4( uint32_t id, char* pFmt, int32_t d0, int32_t d1, int32_t d2, int32_t d3 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0d00 );
    trice_132( TRICE_LONGCOUNT(16) ); // long count 16
    trice_432( d0, d1, d2, d3 );
    TRICE_LEAVE_CRITICAL_SECTION
}

#if 1 // TRICE_TRANSFER_ENDIANESS == TRICE_LITTLE_ENDIANESS

//! trace id and 64-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0800|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( (uint64_t)(d0)>>32 ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

TRICE_INLINE void trice64_1( uint32_t id, char* pFmt, int64_t d0 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0800 );
    trice_232( (uint32_t)(d0>>32), (uint32_t)d0 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace id and 64-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE64_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0d00|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(16) ); \
    TRICE_HTON_U32PUSH( (uint64_t)(d0)>>32 ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( (uint64_t)(d1)>>32 ); \
    TRICE_HTON_U32PUSH( d1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


TRICE_INLINE void trice64_2( uint32_t id, char* pFmt, int64_t d0, int64_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0d00 );
    trice_132( TRICE_LONGCOUNT(16) ); // long count 16
    trice_432( (uint32_t)(d0>>32), (uint32_t)d0, (uint32_t)(d1>>32), (uint32_t)d1 );
    TRICE_LEAVE_CRITICAL_SECTION
}

#else // #if TRICE_TRANSFER_ENDIANESS == TRICE_LITTLE_ENDIANESS

//! trace id and 64-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0800|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( (uint64_t)(d0)>>32 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


TRICE_INLINE void trice64_1( uint32_t id, char* pFmt, int64_t d0 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0800 );
    trice_232( (uint32_t)d0, (uint32_t)(d0>>32) );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace id and 64-bit values protected (outside critical section)
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE64_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_HTON_U32PUSH( id|0x0d00|triceCycle ); \
    triceCycle++; \
    TRICE_HTON_U32PUSH( TRICE_LONGCOUNT(16) ); \
    TRICE_HTON_U32PUSH( d0 ); \
    TRICE_HTON_U32PUSH( (uint64_t)(d0)>>32 ); \
    TRICE_HTON_U32PUSH( d1 ); \
    TRICE_HTON_U32PUSH( (uint64_t)(d1)>>32 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


TRICE_INLINE void trice64_2( uint32_t id, char* pFmt, int64_t d0, int64_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_header( id|0x0d00 );
    trice_132( TRICE_LONGCOUNT(16) ); // long count 16
    trice_432( (uint32_t)d0, (uint32_t)(d0>>32), (uint32_t)d1, (uint32_t)(d1>>32) );
    TRICE_LEAVE_CRITICAL_SECTION
}


#endif // #else // #if TRICE_TRANSFER_ENDIANESS == TRICE_LITTLE_ENDIANESS

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
    if( len <= 12 ){
        TRICE_HTON_U32PUSH( id|(len<<8)|triceCycle ); // on PC side the id reception gives the TRICE_S and the format string information
    }else{
        TRICE_HTON_U32PUSH( id|(0xd<<8)|triceCycle ); // on PC side the id reception gives the TRICE_S and the format string information
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

#endif /* TRICE_PACK2_ENCODER_H_ */
