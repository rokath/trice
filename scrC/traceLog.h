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
// fifo functions
//

#define TL_FIFO_MASK ((TL_FIFO_SIZE>>2)-1) //!< max possible count of items in fifo

extern uint32_t tlFifo[];
extern uint32_t rdIndexTlFifo;
extern uint32_t wrIndexTlFifo;

/*! put one traceLog into traceLog fifo
\param v traceLog id with 2 byte data
traceLog time critical part
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

///////////////////////////////////////////////////////////////////////////////
// TL macros
//

//! basic traceLog macro, assumes d16 to be a 16 bit value
//! TODO: assemler code here
#define TRACELOG( id, pFmt, d16 ) do{ \
    tlFifoPush( (((uint32_t)(d16))<<16) | (id)); \
} while(0)

#define TL0( Id, pFmt ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG( Id, pFmt, 0 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL8_1( Id, pFmt, d0 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG( Id, pFmt, d0 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL8_2( Id, pFmt, d0, d1 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG( Id, pFmt, (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL8_3( Id, pFmt, d0, d1, d2 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG(  0, pFmt, (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRACELOG( Id, pFmt, d2 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL8_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG(  0, pFmt, (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRACELOG( Id, pFmt, (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL8_5( Id, pFmt, d0, d1, d2, d3, d4 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG(  0, pFmt, (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRACELOG(  0, pFmt, (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRACELOG( Id, pFmt, d4 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL8_6( Id, pFmt, d0, d1, d2, d3, d4, d5 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG(  0, pFmt, (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRACELOG(  0, pFmt, (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRACELOG( Id, pFmt, (((uint16_t)(d5))<<8) | (uint8_t)(d4) ) ; \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL8_7( Id, pFmt, d0, d1, d2, d3, d4, d5, d6 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG(  0, pFmt, (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRACELOG(  0, pFmt, (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRACELOG(  0, pFmt, (((uint16_t)(d5))<<8) | (uint8_t)(d4) ) ; \
    TRACELOG( Id, pFmt, d5 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL8_8( Id, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG(  0, pFmt, (((uint16_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRACELOG(  0, pFmt, (((uint16_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRACELOG(  0, pFmt, (((uint16_t)(d5))<<8) | (uint8_t)(d4) ) ; \
    TRACELOG( Id, pFmt, (((uint16_t)(d7))<<8) | (uint8_t)(d6) ) ; \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL16_1( Id, pFmt, d0 ) do{ \
    TRACELOG( Id, pFmt, d0 ); \
} while(0)

#define TL16_2( Id, pFmt, d0, d1 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG(  0, pFmt, d0 ); \
    TRACELOG( Id, pFmt, d1 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL16_3( Id, pFmt, d0, d1, d2 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG(  0, pFmt, d0 ); \
    TRACELOG(  0, pFmt, d1 ); \
    TRACELOG( Id, pFmt, d2 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL16_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG(  0, pFmt, d0 ); \
    TRACELOG(  0, pFmt, d1 ); \
    TRACELOG(  0, pFmt, d2 ); \
    TRACELOG( Id, pFmt, d3 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL32_1( Id, pFmt, d0 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG(  0, pFmt, d0 ); \
    TRACELOG( Id, pFmt, d0>>16 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

#define TL32_2( Id, pFmt, d0, d1 ) do{ \
    TL_ENTER_CRITICAL_SECTION \
    TRACELOG(  0, pFmt, d0 ); \
    TRACELOG(  0, pFmt, d0>>16 ); \
    TRACELOG(  0, pFmt, d1 ); \
    TRACELOG( Id, pFmt, d1>>16 ); \
    TL_LEAVE_CRITICAL_SECTION \
} while(0)

///////////////////////////////////////////////////////////////////////////////
// little helper for traceLog usage
//

/*! trace log a string
\details not very effective but better than no strings
This function could also be useful, if the string is generated dynamically.
\param s 0-terminated string
*/
TL_INLINE void TraceString( char* s )
{
    while( *s )
    {
        TL8_1( Id(3), "%c", *s );
        s++;
    }
}

/*! Report name and line number over trace log
\param pFileName pointer to 0-terminated filename or function name
\param Line line number
\param Value for context display
*/
TL_INLINE void ReportLocation( const char* const pFileName, int Line, int Value )
{
    TL0( Id(7), "sig:" );
    if( pFileName )
    {
        TraceString( (char*)pFileName );
    }
    TL32_2( Id(5), " line %d (Value = %d)\n", Line, Value );
}

/*! Report name and line number over trace log as Failure
\param pName pointer to 0-terminated filename or function name
\param Line line number
\param Value for context display
*/
TL_INLINE void ReportFailure( const char* const pName, int Line, int Value )
{
    TL0( Id(6), "err: Failure in " );
    ReportLocation( pName, Line, Value );
}

/*! Report filename and line number over trace log
\param pFileName pointer to 0-terminated filename
\param Line line number
\param Value for context display
*/
TL_INLINE void ReportPassage( char *pFileName, int Line, int Value )
{
    TL0( Id(4), "att: Passage in " );
    if( pFileName )
    {
        TraceString( pFileName );
    }
    TL32_2( Id(5), " line %d (Value = %d)\n", Line, Value );
}

TL_INLINE void TraceSrcLocation(char *file, int line)
{
      TL0( Id(31976), "err: Error in file " );
      TraceString( file );
      TL16_1( Id(8272), " at line %d\n", line );
}

#endif // #else // #ifdef TRACELOG_OFF

#ifdef __cplusplus
}
#endif

#endif // TRACELOG_H_
