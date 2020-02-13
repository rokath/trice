/*! \file traceLog.h
\brief Software tracer header file
\details This file is included in target code files. If TRACELOG_OFF is defined
(globally or file specific) the TL* macros generate no code. 
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRACELOG_H_
#define TRACELOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "traceLogConfig.h" 

/*! This function should be called inside the transmit done device interrupt.
Also it should be called cyclically to trigger transmission start.
*/
void traceLogTxHandler( void );

#define Id( n ) (n) //!< Macro for improved trace log readability and better source code parsing.

#ifdef TRACELOG_OFF

#define TL0( id, pFmt )
#define TL8_1( id, pFmt, v0                             )
#define TL8_2( id, pFmt, v0, v1                         )
#define TL8_3( id, pFmt, v0, v1, v2                     )
#define TL8_4( id, pFmt, v0, v1, v2, v3                 )
#define TL8_5( id, pFmt, v0, v1, v2, v3, v4             )
#define TL8_6( id, pFmt, v0, v1, v2, v3, v4, v5         )
#define TL8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6     )
#define TL8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 )
#define TL16_1( id, pFmt, v0             )
#define TL16_2( id, pFmt, v0, v1         )
#define TL16_3( id, pFmt, v0, v1, v2     )
#define TL16_4( id, pFmt, v0, v1, v2, v3 )
#define TL32_1( id, pFmt, v0     )
#define TL32_2( id, pFmt, v0, v1 )

TL_INLINE void TraceString( char* s ){}
TL_INLINE void ReportLocation( const char* const pFileName, int Line, int Value ){}
TL_INLINE void ReportFailure( const char* const pName, int Line, int Value ){}
TL_INLINE void ReportPassage( char *pFileName, int Line, int Value ){}
TL_INLINE void TraceSrcLocation(char *file, int line){}

#else // #ifdef TRACELOG_OFF

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
// TL message struct
//

typedef PACKED struct{
    uint8_t start; // traceLog message header start value
    uint8_t cad;   // client address
    uint8_t sad;   // server address
    uint8_t crc8;  // ab^cad^sad^load[0]^load[1]^load[2]^load[3]
}PACKED_END traceLogMsgHeader_t; //!< traceLog message header for routing, syncing and conistency check

typedef PACKED union {
    PACKED struct{
    uint16_t Id; // traceLog id
    uint16_t d;  // 2 data byte
    }PACKED_END tl;
    uint8_t load[4]; // for crc8 computation
}PACKED_END traceLogMsgLoad_t; //!< traceLog message payload

typedef PACKED struct {
    traceLogMsgHeader_t hd; // header
    traceLogMsgLoad_t ld; // payload
}PACKED_END traceLogMsg_t; //! traceLog message

///////////////////////////////////////////////////////////////////////////////
// fifo functionality
//

#define TL_FIFO_MASK ((TL_FIFO_SIZE>>2)-1) //!< max possible count of items in fifo

extern uint32_t tlFifo[];
extern uint32_t rdIndexTlFifo;
extern uint32_t wrIndexTlFifo;

/*! put one tracelog into tracelog fifo
\param v tracelog id with 2 byte data
tracelog time critical part
*/
TL_INLINE void tlFifoPush( uint32_t v ){
    tlFifo[wrIndexTlFifo++] = v;
    wrIndexTlFifo &= TL_FIFO_MASK;
}

/*! get one traceLog from traceLog fifo
\param p address for traceLog id with 2 byte data
*/
TL_INLINE void tlFifoPop( uint32_t* p ){
    *p = tlFifo[rdIndexTlFifo++];
    rdIndexTlFifo &= TL_FIFO_MASK;
}

/* traceLog item count inside traceLog fifo
\return count of buffered traceLogs
*/
TL_INLINE size_t tlFifoDepth( void ){
    return (wrIndexTlFifo - rdIndexTlFifo) & TL_FIFO_MASK;
}

#if 0 == TL_SHORT_MEMORY // #################################################

///////////////////////////////////////////////////////////////////////////////
// TL macros
//

//! basic tracelog macro, assumes d16 to be 
//! id traceLog identifier
//! \param d16 a 16 bit value
#define TRACELOG( id, d16 ) do{ \
    tlFifoPush( (((uint32_t)(d16))<<16) | (id)); \
} while(0)

//! basic tracelog macro, assumes d16 to be a 16 bit value
//! id is 0
//! \param d16 a 16 bit value
#define TRACELOG0( d16 ) do{ \
    tlFifoPush( ((uint32_t)(d16))<<16); \
} while(0)

//! trace Id protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
#define TL0( Id, pFmt ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    tlFifoPush( Id ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TL8_1( Id, pFmt, d0 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG( Id, d0 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TL8_2( Id, pFmt, d0, d1 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG( Id, (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TL8_3( Id, pFmt, d0, d1, d2 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG0( (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRACELOG( Id, d2 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TL8_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG0(    (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRACELOG( Id, (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
#define TL8_5( Id, pFmt, d0, d1, d2, d3, d4 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG0( (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRACELOG0( (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRACELOG( Id, d4 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
#define TL8_6( Id, pFmt, d0, d1, d2, d3, d4, d5 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG0(    (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRACELOG0(    (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRACELOG( Id, (((uint16_t)(d5))<<8) | (uint8_t)(d4) ) ; \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
#define TL8_7( Id, pFmt, d0, d1, d2, d3, d4, d5, d6 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG0( (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRACELOG0( (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRACELOG0( (((uint16_t)(d5))<<8) | (uint8_t)(d4) ) ; \
    TRACELOG( Id, d5 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
#define TL8_8( Id, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG0( (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRACELOG0( (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRACELOG0( (((uint16_t)(d5))<<8) | (uint8_t)(d4) ) ; \
    TRACELOG( Id, (((uint16_t)(d7))<<8) | (uint8_t)(d6) ) ; \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 16-bit value protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TL16_1( Id, pFmt, d0 ) do{ \
    TRACELOG( Id, d0 ); \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TL16_2( Id, pFmt, d0, d1 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG0(    d0 ); \
    TRACELOG( Id, d1 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TL16_3( Id, pFmt, d0, d1, d2 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG0( d0 ); \
    TRACELOG0( d1 ); \
    TRACELOG( Id, d2 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TL16_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG0( d0 ); \
    TRACELOG0( d1 ); \
    TRACELOG0( d2 ); \
    TRACELOG( Id, d3 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 32-bit value protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TL32_1( Id, pFmt, d0 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG0( d0 ); \
    TRACELOG( Id, d0>>16 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TL32_2( Id, pFmt, d0, d1 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG0( d0 ); \
    TRACELOG0( d0>>16 ); \
    TRACELOG0( d1 ); \
    TRACELOG( Id, d1>>16 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#else // #if 0 == TL_SHORT_MEMORY // ########################################

void tl_0_ics( uint16_t Id );
void tl_0_ocs( uint16_t Id );
void tl_8_1_ics( uint16_t Id, uint32_t d0 );
void tl_8_1_ocs( uint16_t Id, uint32_t d0 );
void tl_8_02_ics( uint32_t d0, uint32_t d1 );
void tl_8_2_ics( uint16_t Id, uint32_t d0, uint32_t d1 );
void tl_8_2_ocs( uint16_t Id, uint32_t d0, uint32_t d1 );
void tl_8_3_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 );
void tl_8_3_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 );
void tl_8_4_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 );
void tl_8_04_ics( uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 );
void tl_8_4_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 );
void tl_8_5_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4 );
void tl_8_5_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4 );
void tl_8_6_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5 );
void tl_8_06_ics( uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5 );
void tl_8_6_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5 );
void tl_8_7_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6 );
void tl_8_7_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6 );
void tl_8_8_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7 );
void tl_8_08_ics( uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7 );
void tl_8_8_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7 );
#define tl_16_1_ics tl_8_1_ics //!< same code
#define tl_16_1_ocs tl_8_1_ocs //!< same code
void tl_16_01_ics( uint32_t d0 );
void tl_16_02_ics( uint32_t d0, uint32_t d1 );
void tl_16_2_ics( uint16_t Id, uint32_t d0, uint32_t d1 );
void tl_16_2_ocs( uint16_t Id, uint32_t d0, uint32_t d1 );
void tl_16_3_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 );
void tl_16_3_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 );
void tl_16_4_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 );
void tl_16_4_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 );
void tl_32_1_ics( uint16_t Id, uint32_t d0 );
void tl_32_1_ocs( uint16_t Id, uint32_t d0 );
void tl_32_2_ics( uint16_t Id, uint32_t d0, uint32_t d1 );
void tl_32_2_ocs( uint16_t Id, uint32_t d0, uint32_t d1 );

///////////////////////////////////////////////////////////////////////////////
// TL macros
//

//! trace Id protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
#define TL0( Id, pFmt ) do{ \
    tl_0_ocs( Id ); \
} while(0)

//! trace Id protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TL8_1( Id, pFmt, d0 ) do{ \
    tl_8_1_ocs( Id, d0 ); \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TL8_2( Id, pFmt, d0, d1 ) do{ \
    tl_8_2_ocs( Id, d0, d1 ); \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TL8_3( Id, pFmt, d0, d1, d2 ) do{ \
    tl_8_3_ocs( Id, d0, d1, d2 ); \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TL8_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    tl_8_4_ocs( Id, d0, d1, d2, d3 ); \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
#define TL8_5( Id, pFmt, d0, d1, d2, d3, d4 ) do{ \
    tl_8_5_ocs( Id, d0, d1, d2, d3, d4 ); \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
#define TL8_6( Id, pFmt, d0, d1, d2, d3, d4, d5 ) do{ \
    tl_8_6_ocs( Id, d0, d1, d2, d3, d4, d5 ); \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
#define TL8_7( Id, pFmt, d0, d1, d2, d3, d4, d5, d6 ) do{ \
    tl_8_7_ocs( Id, d0, d1, d2, d3, d4, d5, d6 ); \
} while(0)

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
#define TL8_8( Id, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 ) do{ \
    tl_8_8_ocs( Id, d0, d1, d2, d3, d4, d5, d6, d7 ); \
} while(0)

//! trace Id and 16-bit value protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TL16_1( Id, pFmt, d0 ) do{ \
    tl_16_1_ocs( Id, d0 ); \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TL16_2( Id, pFmt, d0, d1 ) do{ \
    tl_16_2_ocs( Id, d0, d1 ); \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TL16_3( Id, pFmt, d0, d1, d2 ) do{ \
    tl_16_3_ocs( Id, d0, d1, d2 ); \
} while(0)

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TL16_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    tl_16_4_ocs( Id, d0, d1, d2, d3 ); \
} while(0)

//! trace Id and 32-bit value protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TL32_1( Id, pFmt, d0 ) do{ \
    tl_32_1_ocs( Id, d0 ); \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TL32_2( Id, pFmt, d0, d1 ) do{ \
    tl_32_2_ocs( Id, d0, d1 ); \
} while(0)

#endif //#else // #if 0 == TL_SHORT_MEMORY // ###############################

void TraceString( char* s );
void ReportLocation( const char* const pFileName, int Line, int Value );
void ReportFailure( const char* const pName, int Line, int Value );
void ReportPassage( char *pFileName, int Line, int Value );
void TraceSrcLocation(char *file, int line);

#endif // #else // #ifdef TRACELOG_OFF

#ifdef __cplusplus
}
#endif

#endif // TRACELOG_H_
