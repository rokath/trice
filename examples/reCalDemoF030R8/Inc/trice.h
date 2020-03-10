/*! \file trice.h
\brief Software tracer header file
\details This file is included in target code files. If TRICE_LEVEL is defined 
as 0 (globally or file specific) the TRICE* macros generate no code. 
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_H_
#define TRICE_H_

#include "config.h" 
#include <stdint.h>

#ifndef TRICE_LEVEL
#define TRICE_LEVEL 100 //!< switch trices off with 0, define TRICE_LEVEL globally or on top of file (before including config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void triceTxStart( int* pTxState );
void triceTxContinue( int* pTxState );
int tricePrintfAdapter( const char* pFmt, ... );
void triceString( int rightBound, const char* s );

void TxStart( void );
void TxContinue( void );

#define Id( n ) (n) //!< Macro for improved trice readability and better source code parsing.

#if 0 == TRICE_LEVEL // no trice code generation

#define TRICE0( id, pFmt )
#define TRICE8_1( id, pFmt, v0                             )
#define TRICE8_2( id, pFmt, v0, v1                         )
#define TRICE8_3( id, pFmt, v0, v1, v2                     )
#define TRICE8_4( id, pFmt, v0, v1, v2, v3                 )
#define TRICE8_5( id, pFmt, v0, v1, v2, v3, v4             )
#define TRICE8_6( id, pFmt, v0, v1, v2, v3, v4, v5         )
#define TRICE8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6     )
#define TRICE8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 )
#define TRICE16_1( id, pFmt, v0             )
#define TRICE16_2( id, pFmt, v0, v1         )
#define TRICE16_3( id, pFmt, v0, v1, v2     )
#define TRICE16_4( id, pFmt, v0, v1, v2, v3 )
#define TRICE32_1( id, pFmt, v0     )
#define TRICE32_2( id, pFmt, v0, v1 )
#define TRICE32_3( id, pFmt, v0, v1, v2 )
#define TRICE32_4( id, pFmt, v0, v1, v2, v3 )
#define TRICE64_1( id, pFmt, v0 )
#define TRICE64_2( id, pFmt, v0, v1 )

#else // #if 0 == TRICE_LEVEL

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
// TRICE message struct
//

typedef PACKED struct{
    uint8_t start; // trice message header start value
    uint8_t cad;   // client address
    uint8_t sad;   // server address
    uint8_t crc8;  // ab^cad^sad^load[0]^load[1]^load[2]^load[3]
}PACKED_END triceMsgHeader_t; //!< trice message header for routing, syncing and conistency check

typedef PACKED union {
    PACKED struct{
    uint16_t Id; // trice id
    uint16_t d;  // 2 data byte
    }PACKED_END trice;
    uint8_t load[4]; // for crc8 computation
}PACKED_END triceMsgLoad_t; //!< trice message payload

typedef PACKED struct {
    triceMsgHeader_t hd; // header
    triceMsgLoad_t ld; // payload
}PACKED_END triceMsg_t; //! trice message

///////////////////////////////////////////////////////////////////////////////
// fifo functionality
//

#define TRICE_FIFO_MASK ((TRICE_FIFO_SIZE>>2)-1) //!< max possible count of items in fifo

extern uint32_t triceFifo[];
extern uint32_t rdIndexTriceFifo;
extern uint32_t wrIndexTriceFifo;

/*! put one trice into trice fifo
\param v trice id with 2 byte data
trice time critical part
*/
TRICE_INLINE void triceFifoPush( uint32_t v ){
    triceFifo[wrIndexTriceFifo++] = v;
    wrIndexTriceFifo &= TRICE_FIFO_MASK;
}

/*! get one trice from trice fifo
\param p address for trice id with 2 byte data
*/
TRICE_INLINE void triceFifoPop( uint32_t* p ){
    *p = triceFifo[rdIndexTriceFifo++];
    rdIndexTriceFifo &= TRICE_FIFO_MASK;
}

/* trice item count inside trice fifo
\return count of buffered trices
*/
TRICE_INLINE size_t triceFifoDepth( void ){
    return (wrIndexTriceFifo - rdIndexTriceFifo) & TRICE_FIFO_MASK;
}

#if 0 == TRICE_SHORT_MEMORY // #################################################

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//

//! basic trice macro, assumes d16 to be 
//! id trice identifier
//! \param d16 a 16 bit value
#define TRICE( id, d16 ) do{ \
    triceFifoPush( (((uint32_t)(d16))<<16) | (id)); \
} while(0)

//! basic trice macro, assumes d16 to be a 16 bit value
//! id is 0
//! \param d16 a 16 bit value
#define TRICE_ID0( d16 ) do{ \
    triceFifoPush( ((uint32_t)(d16))<<16); \
} while(0)

//! trace Id protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
#define TRICE0( Id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    triceFifoPush( Id ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TRICE8_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE( Id, d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE8_2( Id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE( Id, (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
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
    TRICE_ID0( (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRICE( Id, d2 ); \
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
    TRICE_ID0( (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRICE( Id, (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
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
    TRICE_ID0( (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRICE_ID0( (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRICE( Id, d4 ); \
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
    TRICE_ID0( (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRICE_ID0( (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRICE( Id, (((uint16_t)(d5))<<8) | (uint8_t)(d4) ) ; \
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
    TRICE_ID0( (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRICE_ID0( (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRICE_ID0( (((uint16_t)(d5))<<8) | (uint8_t)(d4) ) ; \
    TRICE( Id, d6 ); \
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
    TRICE_ID0( (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRICE_ID0( (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRICE_ID0( (((uint16_t)(d5))<<8) | (uint8_t)(d4) ) ; \
    TRICE( Id, (((uint16_t)(d7))<<8) | (uint8_t)(d6) ) ; \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 16-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE16_1( Id, pFmt, d0 ) do{ \
    TRICE( Id, d0 ); \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE16_2( Id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( d0 ); \
    TRICE( Id, d1 ); \
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
    TRICE( Id, d2 ); \
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
    TRICE( Id, d3 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 32-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE32_1( Id, pFmt, d0 ) do{ \
    uint32_t x = (uint32_t)d0; \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( x ); \
    TRICE( Id, x>>16 ); \
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
    TRICE_ID0( x0 ); \
    TRICE_ID0( x0>>16 ); \
    TRICE_ID0( x1 ); \
    TRICE( Id, x1>>16 ); \
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
    TRICE_ID0( x0 ); \
    TRICE_ID0( x0>>16 ); \
    TRICE_ID0( x1 ); \
    TRICE_ID0( x1>>16 ); \
    TRICE_ID0( x2 ); \
    TRICE( Id, x2>>16 ); \
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
    TRICE_ID0( x0 ); \
    TRICE_ID0( x0>>16 ); \
    TRICE_ID0( x1 ); \
    TRICE_ID0( x1>>16 ); \
    TRICE_ID0( x2 ); \
    TRICE_ID0( x2>>16 ); \
    TRICE_ID0( x3 ); \
    TRICE( Id, x3>>16 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 64-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( Id, pFmt, d0 ) do{ \
    uint64_t x = (uint64_t)d0; \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_ID0( x>>00 ); /*ll*/ \
    TRICE_ID0( x>>16 ); /*hl*/\
    TRICE_ID0( x>>32 ); /*lh*/ \
    TRICE( Id, x>>48 ); /*hh*/ \
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
    TRICE_ID0( x0>>00 ); /*ll*/ \
    TRICE_ID0( x0>>16 ); /*hl*/\
    TRICE_ID0( x0>>32 ); /*lh*/ \
    TRICE_ID0( x0>>48 ); /*hh*/ \
    TRICE_ID0( x1>>00 ); /*ll*/ \
    TRICE_ID0( x1>>16 ); /*hl*/\
    TRICE_ID0( x1>>32 ); /*lh*/ \
    TRICE( Id, x1>>48 ); /*hh*/ \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


#else // #if 0 == TRICE_SHORT_MEMORY // ##########################################

///////////////////////////////////////////////////////////////////////////////
// internal trice functions
//

//! trace Id unprotected (inside critical section)
//! \param Id trice identifier
TRICE_INLINE void trice_0_ics( uint16_t Id ){
    triceFifoPush(Id);
}

//! trace Id protected (outside critical section)
//! \param Id trice identifier
TRICE_INLINE void trice_0_ocs( uint16_t Id ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_0_ics(Id);
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8- or 16-bit value unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
TRICE_INLINE void trice_8_1_ics( uint16_t Id, uint32_t d0 ){
    triceFifoPush( (d0<<16) | Id);
}

//! trace Id and 8- or 16-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
TRICE_INLINE void trice_8_1_ocs( uint16_t Id, uint32_t d0 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_1_ics(Id, d0);
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id==0 and 8-bit values unprotected (inside critical section)
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_8_02_ics( uint32_t d0, uint32_t d1 ){
    triceFifoPush( (d1<<24) | (d0<<16) );
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_8_2_ics( uint16_t Id, uint32_t d0, uint32_t d1 ){
    triceFifoPush( (d1<<24) | (d0<<16) | Id);
}

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_8_2_ocs( uint16_t Id, uint32_t d0, uint32_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_2_ics( Id, d0, d1 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TRICE_INLINE void trice_8_3_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    trice_8_02_ics( d0, d1 );
    trice_8_1_ics( Id, d2 );
}

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TRICE_INLINE void trice_8_3_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_3_ics( Id, d0, d1, d2 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_8_4_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    trice_8_02_ics( d0, d1 );
    trice_8_2_ics( Id, d2, d3 );
}

//! trace Id==0 and 8-bit values unprotected (inside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_8_04_ics( uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    trice_8_02_ics( d0, d1 );
    trice_8_02_ics( d2, d3 );
}

//! trace Id==0 and 8-bit values protected (outside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_8_4_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_4_ics( Id, d0, d1, d2, d3 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
TRICE_INLINE void trice_8_5_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4 ){
    trice_8_04_ics( d0, d1, d2, d3 );
    trice_8_1_ics( Id, d2 );
}

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
TRICE_INLINE void trice_8_5_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_5_ics( Id, d0, d1, d2, d3, d4 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
TRICE_INLINE void trice_8_6_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5 ){
    trice_8_04_ics( d0, d1, d2, d3 );
    trice_8_2_ics( Id, d4, d5 );
}
/*
//! trace Id==0 and 8-bit values unprotected (inside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
TRICE_INLINE void trice_8_06_ics( uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5 ){
    trice_8_04_ics( d0, d1, d2, d3 );
    trice_8_02_ics( d4, d5 );
}*/

//! trace Id==0 and 8-bit values protected (outside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
TRICE_INLINE void trice_8_6_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_6_ics( Id, d0, d1, d2, d3, d4, d5 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
TRICE_INLINE void trice_8_7_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6 ){
    trice_8_04_ics( d0, d1, d2, d3 );
    trice_8_3_ics( Id, d4, d5, d6 );
}

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
TRICE_INLINE void trice_8_7_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_7_ics( Id, d0, d1, d2, d3, d4, d5, d6 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
TRICE_INLINE void trice_8_8_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7 ){
    trice_8_04_ics( d0, d1, d2, d3 );
    trice_8_4_ics( Id, d4, d5, d6, d7 );
}
/*
//! trace Id==0 and 8-bit values unprotected (inside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
TRICE_INLINE void trice_8_08_ics( uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7 ){
    trice_8_04_ics( d0, d1, d2, d3 );
    trice_8_04_ics( d4, d5, d6, d7 );
}*/

//! trace Id==0 and 8-bit values protected (outside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
TRICE_INLINE void trice_8_8_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_8_8_ics( Id, d0, d1, d2, d3, d4, d5, d6, d7 );
    TRICE_LEAVE_CRITICAL_SECTION
}

#define trice_16_1_ics trice_8_1_ics //!< same code
#define trice_16_1_ocs trice_8_1_ocs //!< same code

//! trace Id==0 and 16-bit value unprotected (inside critical section)
//! \param d0 payload
TRICE_INLINE void trice_16_01_ics( uint32_t d0 ){
    triceFifoPush( (d0<<16) );
}

//! trace Id==0 and 8-bit values unprotected (inside critical section)
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_16_02_ics( uint32_t d0, uint32_t d1 ){
    trice_16_01_ics( d0 );
    trice_16_01_ics( d1 );
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_16_2_ics( uint16_t Id, uint32_t d0, uint32_t d1 ){
    trice_16_01_ics( d0 );
    trice_16_1_ics( Id, d1 );
}

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_16_2_ocs( uint16_t Id, uint32_t d0, uint32_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_16_2_ics( Id, d0, d1 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TRICE_INLINE void trice_16_3_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    trice_16_02_ics( d0, d1 );
    trice_16_1_ics( Id, d2 );
}

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TRICE_INLINE void trice_16_3_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_16_3_ics( Id, d0, d1, d2 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_16_4_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    trice_16_02_ics( d0, d1 );
    trice_16_2_ics( Id, d2, d3 );
}

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_16_4_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_16_4_ics( Id, d0, d1, d2, d3 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 32-bit value unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
TRICE_INLINE void trice_32_1_ics( uint16_t Id, uint32_t d0 ){
    trice_16_01_ics( d0 );
    trice_16_1_ics( Id, d0>>16 );
}

//! trace Id and 32-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
TRICE_INLINE void trice_32_1_ocs( uint16_t Id, uint32_t d0 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_32_1_ics( Id, d0 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 32-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_32_2_ics( uint16_t Id, uint32_t d0, uint32_t d1 ){
    trice_16_02_ics( d0, d0>>16 );
    trice_16_2_ics( Id, d1, d1>>16 );
}

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_32_2_ocs( uint16_t Id, uint32_t d0, uint32_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_32_2_ics( Id, d0, d1 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 32-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TRICE_INLINE void trice_32_3_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    trice_16_02_ics( d0, d0>>16 );
    trice_16_4_ics( Id, d1, d1>>16 , d2, d2>>16 );
}

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TRICE_INLINE void trice_32_3_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_32_3_ics( Id, d0, d1, d2 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 32-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_32_4_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    trice_16_4_ics( 0, d0, d0>>16, d1, d1>>16  );
    trice_16_4_ics( Id, d2, d2>>16, d3, d3>>16  );
}

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TRICE_INLINE void trice_32_4_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_32_4_ics( Id, d0, d1, d2, d3 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 64-bit value unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
TRICE_INLINE void trice_64_1_ics( uint16_t Id, uint64_t d0 ){
    trice_16_02_ics( (uint16_t)d0, (uint16_t)(d0>>16) );
    trice_16_2_ics( Id, (uint16_t)(d0>>32), (uint16_t)(d0>>48 ));
}

//! trace Id and 64-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_64_2_ics( uint16_t Id, uint64_t d0, uint64_t d1 ){
    trice_16_02_ics( (uint16_t)d0, (uint16_t)(d0>>16) );
    trice_16_02_ics( (uint16_t)(d0>>32), (uint16_t)(d0>>48) );
    trice_16_02_ics( (uint16_t)d1, (uint16_t)(d1>>16) );
    trice_16_2_ics( Id, (uint16_t)(d1>>32), (uint16_t)(d1>>48 ));
}

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//

//! trace Id protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
#define TRICE0( Id, pFmt ) do{ \
    trice_0_ocs( Id ); \
} while(0)

//! trace Id protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TRICE8_1( Id, pFmt, d0 ) do{ \
    trice_8_1_ocs( Id, (uint8_t)d0 ); \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE8_2( Id, pFmt, d0, d1 ) do{ \
    trice_8_2_ocs( Id, (uint8_t)d0, (uint8_t)d1 ); \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE8_3( Id, pFmt, d0, d1, d2 ) do{ \
    trice_8_3_ocs( Id, (uint8_t)d0, (uint8_t)d1, (uint8_t)d2 ); \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE8_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    trice_8_4_ocs( Id, (uint8_t)d0, (uint8_t)d1, (uint8_t)d2, (uint8_t)d3 ); \
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
    trice_8_5_ocs( Id, (uint8_t)d0, (uint8_t)d1, (uint8_t)d2, (uint8_t)d3, (uint8_t)d4 ); \
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
    trice_8_6_ocs( Id, (uint8_t)d0, (uint8_t)d1, (uint8_t)d2, (uint8_t)d3, (uint8_t)d4, (uint8_t)d5 ); \
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
    trice_8_7_ocs( Id, (uint8_t)d0, (uint8_t)d1, (uint8_t)d2, (uint8_t)d3, (uint8_t)d4, (uint8_t)d5, (uint8_t)d6 ); \
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
    trice_8_8_ocs( Id, (uint8_t)d0, (uint8_t)d1, (uint8_t)d2, (uint8_t)d3, (uint8_t)d4, (uint8_t)d5, (uint8_t)d6, (uint8_t)d7 ); \
} while(0)

//! trace Id and 16-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE16_1( Id, pFmt, d0 ) do{ \
    trice_16_1_ocs( Id, (uint16_t)d0 ); \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE16_2( Id, pFmt, d0, d1 ) do{ \
    trice_16_2_ocs( Id, (uint16_t)d0, (uint16_t)d1 ); \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE16_3( Id, pFmt, d0, d1, d2 ) do{ \
    trice_16_3_ocs( Id, (uint16_t)d0, (uint16_t)d1, (uint16_t)d2 ); \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE16_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    trice_16_4_ocs( Id, (uint16_t)d0, (uint16_t)d1, (uint16_t)d2, (uint16_t)d3 ); \
} while(0)

//! trace Id and 32-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE32_1( Id, pFmt, d0 ) do{ \
    trice_32_1_ocs( Id, (uint32_t)d0 ); \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE32_2( Id, pFmt, d0, d1 ) do{ \
    trice_32_2_ocs( Id, (uint32_t)d0, (uint32_t)d1 ); \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE32_3( Id, pFmt, d0, d1, d2 ) do{ \
    trice_32_3_ocs( Id, (uint32_t)d0, (uint32_t)d1, (uint32_t)d2 ); \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE32_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    trice_32_4_ocs( Id, (uint32_t)d0, (uint32_t)d1, (uint32_t)d2, (uint32_t)d3 ); \
} while(0)


//! trace Id and 64-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( Id, pFmt, d0 ) do{ \
    trice_64_1_ocs( Id, (uint64_t)d0 ); \
} while(0)


//! trace Id and 64-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
TRICE_INLINE void trice_64_1_ocs( uint16_t Id, uint64_t d0 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_64_1_ics( Id, d0 );
    TRICE_LEAVE_CRITICAL_SECTION
}

//! trace Id and 64-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1( Id, pFmt, d0 ) do{ \
    trice_64_1_ocs( Id, (uint64_t)d0 ); \
} while(0)


//! trace Id and 64-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_64_2_ocs( uint16_t Id, uint64_t d0, uint64_t d1 ){
    TRICE_ENTER_CRITICAL_SECTION
    trice_64_2_ics( Id, d0, d1 );
    TRICE_LEAVE_CRITICAL_SECTION
}


//! trace Id and 64-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE64_2( Id, pFmt, d0, d1 ) do{ \
    trice_64_2_ocs( Id, (uint64_t)d0, (uint64_t)d1 ); \
} while(0)


#endif //#else // #if 0 == TRICE_SHORT_MEMORY // #################################

/*
///////////////////////////////////////////////////////////////////////////////
// little trice helper for trice usage
//

#if 1 == TRICE_SHORT_MEMORY

TRICE_INLINE void triceStringUnbound( const char* s ){
    while( *s )
    {
        TRICE8_1( Id(3), "%c", *s );
        s++;
    }
}

TRICE_INLINE void triceString( int rightBound, const char* s ){
    size_t len = strlen( s );
    int spaces = rightBound - len;
    spaces = spaces < 0 ? 0 : spaces;
    TRICE_ENTER_CRITICAL_SECTION
    while( spaces-->0 )
    {
        TRICE0( Id(27950), " " );
    }
    triceStringUnbound( s );
    TRICE_LEAVE_CRITICAL_SECTION
}

#else // #if 1 == TRICE_SHORT_MEMORY

// for performance no check of strlen( s ) here (internal usage)
TRICE_INLINE void triceStringN( size_t len, const char* s ){
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
#else
*/
/*
\code
// legacy reCal packet format
  |--------------------------------- fixed packet start0 byte 0xc0 
  |    |---------------------------- following data package count 
  |    |   |------------------------ packet index (2 lsb packet type and and 6 msb cycle counter)
  |    |   |   |-------------------- local address byte
  |    |   |   |   |---------------- server address byte (broadcast address possible for no anser calls)
  |    |   |   |   |   |------------ type identifyer byte
  |    |   |   |   |   |   |-------- function identifyer byte
  |    |   |   |   |   |   |   |---- exclusive-or checksum byte
  v    v   v   v   v   v   v   v
0xc0  dpc pix cad sad tid fid crc8

// trice format
  |--------------------------------- fixed packet start0 byte 0xeb 
  |   |----------------------------- client address (local address byte)
  |   |   |------------------------- server address (destination)
  |   |   |   |--------------------- exclusive-or checksum byte
  |   |   |   |   |----------------- ID low part
  |   |   |   |   |   |------------- ID high part
  |   |   |   |   |   |   |--------- Value Low part
  |   |   |   |   |   |   |   |----- Value High part
  v   v   v   v   v   v   v   v
0xeb cad sad cr8 idL idH vaL  vaH

// reCal packet format 2
  |--------------------------------- fixed packet start0 byte 0xc0 
  |   |----------------------------- following data package count fixed 1 for trice strings
  |   |   |------------------------- server address (destination)
  |   |   |   |--------------------- exclusive-or checksum byte
  |   |   |   |   |----------------- type identifyer byte
  |   |   |   |   |   |------------- function identifyer byte
  |   |   |   |   |   |   |--------- packet index (2 lsb packet type and and 6 msb cycle counter)
  |   |   |   |   |   |   |   |----- data package count
  v   v   v   v   v   v   v   v
0xc0 cad sad cr8 tid fid pix dpc 

// 0xff == tid -> pure string package with fid as string package id

remote call type: (part of pix) 
      bit1      |      bit0       | meaning
  RC_CMD_FLAG   | RC_ANSWER_FLAG  |
----------------|-----------------|------------------------------------------
        1       |        1        | \b Cmd = command expecting answer
        0       |        1        | answer to a command expecting answer
        1       |        0        | \b Msg = command not expecting an answer (message)
        0       |        0        | trice string package


\endcode
*/

// #include "Fifo.h"
// extern Fifo_t wrFifo;
// 
// 

// /*! Report name and line number over trice
// \param pFileName pointer to 0-terminated filename or function name
// \param Line line number
// \param Value for context display
// */
// TRICE_INLINE void reportLocation( const char* const pFileName, int Line, int Value ){
//     TRICE0( Id(7), "sig:" );
//     if( pFileName )
//     {
//         triceString( 0, (char*)pFileName );
//     }
//     TRICE32_2( Id(5), " line %d (Value = %d)\n", Line, Value );
// }
// 
// /*! Report name and line number over trice as Failure
// \param pName pointer to 0-terminated filename or function name
// \param Line line number
// \param Value for context display
// */
// TRICE_INLINE void reportFailure( const char* const pName, int Line, int Value ){
//     TRICE0( Id(6), "err: Failure in " );
//     reportLocation( pName, Line, Value );
// }
// 
// /*! Report filename and line number over trice
// \param pFileName pointer to 0-terminated filename
// \param Line line number
// \param Value for context display
// */
// TRICE_INLINE void reportPassage( char *pFileName, int Line, int Value ){
//     TRICE0( Id(4), "att: Passage in " );
//     if( pFileName )
//     {
//         triceString( 0, pFileName );
//     }
//     TRICE32_2( Id(5), " line %d (Value = %d)\n", Line, Value );
// }
// 
// TRICE_INLINE void triceSrcLocation(char const *file, int line){
//       TRICE0( Id(31976), "err: Error in file " );
//       triceString( 0, file );
//       TRICE16_1( Id(8272), " at line %d\n", line );
// }


/*

///////////////////////////////////////////////////////////////////////////////
// REMOTE CALL
//#define RC_ADDR_OFFSET 0x60 //!< offest for easier debugging
//#define RC_ADDR(n) (RC_ADDR_OFFSET+(n)) //!< remote call address transformation
#define RC_LOCAL_ADDRESS (TRICE_LOCAL_ADDR) //!< remote call addess of this device
#define RCTX_FIFO_SIZE 0x600 //!< also tracelog messages are buffered here
#define RCRX_FIFO_SIZE 0x200 //!< mainly for responses
//
///////////////////////////////////////////////////////////////////////////////

#define LEFT 0

// ### DefaultMacros.h
//#define RESULT_NEXT_HEADER_IN_FIFO 0xC0
//#define RESULT_ANSWER   1 //!< macro for a received remote call answer
#define RESULT_OK       0 //!< macro for successful end of function
#define RESULT_ERROR   -1 //!< macro for unsuccessful end of function
//#define RESULT_NACK    -2 //!< macro for RBL ack answer NACK
//#define RESULT_EMPTY   -3 //!< macro for empty channel
#define RESULT_TIMEOUT -4 //!< see CONTROL_GEAR_ANSWER_TIMEOUT_ERROR
#define RESULT_LOCKED  -5 //!< no reentrance allowed
//
#define UNUSED_PARAMETER( x ) { x = x; }


#define DEFAULT_MS_TIMEOUT 1000

// ### ReadAndWrite.h

//extern uint32_t WriteTimeoutCount; //!< write error counter
//extern uint32_t ReadTimeoutCount; //!< read error counter
//
//
//
//#define IO_NEXT_DEFAULT_MS_TIMEOUT 30
//
//
int Read(int fd, uint8_t* pBuf, size_t count);
//int Read1(int fd, uint8_t* pBuf, size_t count); // tryout code!!!!
int Write(int fd, uint8_t const * pBuf, size_t count);
//

static inline void Pause( void ){}


//#define REPORT_LINE(Value) do{ REPORT_FILE(); TRICE16_2( Id( 8986), " in line %d (0x%02x)\n", __LINE__, Value ); }while(0)
#define REPORT_FAILURE(Value) do{ REPORT_FILE(); TRICE32_2( Id(46005), "ERR: in line %d (0x%08x)\n", __LINE__, Value ); }while(0)
//#define REPORT_FAILURE16(a,b,c) do{ REPORT_FILE(); TRICE16_4( Id(17192), "ERR: in line %d (0x%04x,0x%04x,0x%04x)\n", __LINE__, a,b,c ); }while(0)
//#define REPORT_VALUE(Value)   do{ REPORT_FILE(); TRICE32_2( Id(11917),  "att: line %d, value = 0x%08x\r\n", __LINE__, Value ); }while(0)
//#define REPORT_ONLY_VALUE(Value)   do{  TRICE32_2( Id(33840),  "att: line %d, value = 0x%08x\r\n", __LINE__, Value ); }while(0)
#define ASSERTION do{ REPORT_FILE(); TRICE16_2( Id(16598), "err:local address 0x%02x:!ASSERT in line %d\n", RC_LOCAL_ADDRESS, __LINE__ ); }while(0)
//#define ASSERT( flag ) if(!(flag)) { ASSERTION; } //!< report if flag is not true
//#define ASSERT_OR_RETURN( flag )                if(!(flag)) { ASSERTION; return;              } //!< if flag is not true return result
#define ASSERT_OR_RETURN_RESULT( flag, result ) if(!(flag)) { ASSERTION; return result;       } //!< if flag is not true return result
#define ASSERT_OR_RETURN_RESULT_ERROR( flag )   if(!(flag)) { ASSERTION; return RESULT_ERROR; } //!< if flag is not true return result

*/


#endif // #else // #if 0 == TRICE_LEVEL


#include "triceUtilities.h"

#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
