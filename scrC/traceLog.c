/*! \file traceLog.c
\brief traceLogs to transfer buffer functionality
\details The traceLogs are dumped as 32bit values into a 32 bit fifo.
That is the time critical part. 
\li a traceLog is a 16 bit ID with a 16 bit data value
\li traceLogs with more data are split into several 32bit basic traceLog values with IDs = 0 in front.
\li So a basic traceLog (subtrace) consists always of 4 byte.
\li for traceLog transmission each basic traceLog gets a header of additional 4 bytes.
\li The header contains a startbyte, client and server address and a crc8 exOr checksum.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include "traceLog.h"

#ifdef TRACELOG_OFF

/*! This function should be called inside the transmit done device interrupt.
Also it should be called cyclically to trigger transmission start.
*/
void traceLogTxHandler( void ){}

#else // #ifdef TRACELOG_OFF

#include <stdint.h>

//! traceLog fifo instance
ALIGN4 uint32_t tlFifo[ TL_FIFO_SIZE>>2 ] ALIGN4_END;

uint32_t rdIndexTlFifo = 0; //!< traceLog fifo read index
uint32_t wrIndexTlFifo = 0; //!< traceLog fifo write index

//! partial prefilled traceLog message transmit buffer 
ALIGN4 static traceLogMsg_t traceLogMsg ALIGN4_END = {
    { TL_START_BYTE,  TL_LOCAL_ADDR,  TL_DISPL_ADDR, 0 }, // crc8
    { 0, 0 } // 16bit ID, 16bit data
};

static uint8_t const * const limit = (uint8_t*)(&traceLogMsg + 1); //!< traceLog message buffer limit
static uint8_t       *       pRead = (uint8_t*)(&traceLogMsg + 1); //!< traceLog message buffer read pointer

/*! get next traceLog byte for transmission from traceLog message buffer, no depth check here
\retval data byte
*/
TL_INLINE uint8_t traceLogMsgNextByte( void ){
    return *pRead++;
}

/*! return count of bytes ready for transmission
\retval count
*/
static size_t traceLogMsgDepth( void ){
    size_t count = limit - pRead;
    if( count ){
        return count;
    } else {
        if( tlFifoDepth() ){
            tlFifoPop( (uint32_t*)(&(traceLogMsg.ld)) );
            pRead = (uint8_t*)&traceLogMsg;
            traceLogMsg.hd.crc8  = TL_START_BYTE ^ TL_LOCAL_ADDR ^ TL_DISPL_ADDR
                                 ^ traceLogMsg.ld.load[0]
                                 ^ traceLogMsg.ld.load[1]
                                 ^ traceLogMsg.ld.load[2]
                                 ^ traceLogMsg.ld.load[3];
            return 8;
        }
    }
    return 0;
}

/*! This function should be called inside the transmit done device interrupt.
Also it should be called cyclically to trigger transmission start.
*/
void traceLogTxHandler( void ){
    if( traceLogTxDataRegisterEmpty() ){ 
        if( traceLogMsgDepth() ){
            uint8_t x = traceLogMsgNextByte();
            traceLogTransmitData8( x );
            traceLogEableTxEmptyInterrupt(); 
        }else{
            traceLogDisableTxEmptyInterrupt();
        }
    }
}


#if 1 == TL_SHORT_MEMORY // #################################################

#undef TL_INLINE
#define TL_INLINE

///////////////////////////////////////////////////////////////////////////////
// internal traceLog functions
//

//! trace Id unprotected (inside critical section)
//! \param Id tracelog identifier
TL_INLINE void tl_0_ics( uint16_t Id ){
    tlFifoPush(Id);
}

//! trace Id protected (outside critical section)
//! \param Id tracelog identifier
TL_INLINE void tl_0_ocs( uint16_t Id ){
    TL_ENTER_CRITICAL_SECTION
    tl_0_ics(Id);
    TL_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8- or 16-bit value unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
TL_INLINE void tl_8_1_ics( uint16_t Id, uint32_t d0 ){
    tlFifoPush( (d0<<16) | Id);
}

//! trace Id and 8- or 16-bit value protected (outside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
TL_INLINE void tl_8_1_ocs( uint16_t Id, uint32_t d0 ){
    TL_ENTER_CRITICAL_SECTION
    tl_8_1_ics(Id, d0);
    TL_LEAVE_CRITICAL_SECTION
}

//! trace Id==0 and 8-bit values unprotected (inside critical section)
//! \param d0 payload
//! \param d1 payload
TL_INLINE void tl_8_02_ics( uint32_t d0, uint32_t d1 ){
    tlFifoPush( (d1<<24) | (d0<<16) );
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
TL_INLINE void tl_8_2_ics( uint16_t Id, uint32_t d0, uint32_t d1 ){
    tlFifoPush( (d1<<24) | (d0<<16) | Id);
}

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
TL_INLINE void tl_8_2_ocs( uint16_t Id, uint32_t d0, uint32_t d1 ){
    TL_ENTER_CRITICAL_SECTION
    tl_8_2_ics( Id, d0, d1 );
    TL_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TL_INLINE void tl_8_3_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    tl_8_02_ics( d0, d1 );
    tl_8_1_ics( Id, d2 );
}

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TL_INLINE void tl_8_3_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    TL_ENTER_CRITICAL_SECTION
    tl_8_3_ics( Id, d0, d1, d2 );
    TL_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TL_INLINE void tl_8_4_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    tl_8_02_ics( d0, d1 );
    tl_8_2_ics( Id, d2, d3 );
}

//! trace Id==0 and 8-bit values unprotected (inside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TL_INLINE void tl_8_04_ics( uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    tl_8_02_ics( d0, d1 );
    tl_8_02_ics( d2, d3 );
}

//! trace Id==0 and 8-bit values protected (outside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TL_INLINE void tl_8_4_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    TL_ENTER_CRITICAL_SECTION
    tl_8_4_ics( Id, d0, d1, d2, d3 );
    TL_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
TL_INLINE void tl_8_5_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4 ){
    tl_8_04_ics( d0, d1, d2, d3 );
    tl_8_1_ics( Id, d2 );
}

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
TL_INLINE void tl_8_5_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4 ){
    TL_ENTER_CRITICAL_SECTION
    tl_8_5_ics( Id, d0, d1, d2, d3, d4 );
    TL_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
TL_INLINE void tl_8_6_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5 ){
    tl_8_04_ics( d0, d1, d2, d3 );
    tl_8_2_ics( Id, d4, d5 );
}
/*
//! trace Id==0 and 8-bit values unprotected (inside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
TL_INLINE void tl_8_06_ics( uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5 ){
    tl_8_04_ics( d0, d1, d2, d3 );
    tl_8_02_ics( d4, d5 );
}*/

//! trace Id==0 and 8-bit values protected (outside critical section)
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
TL_INLINE void tl_8_6_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5 ){
    TL_ENTER_CRITICAL_SECTION
    tl_8_6_ics( Id, d0, d1, d2, d3, d4, d5 );
    TL_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
TL_INLINE void tl_8_7_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6 ){
    tl_8_04_ics( d0, d1, d2, d3 );
    tl_8_3_ics( Id, d4, d5, d6 );
}

//! trace Id and 8-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
TL_INLINE void tl_8_7_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6 ){
    TL_ENTER_CRITICAL_SECTION
    tl_8_7_ics( Id, d0, d1, d2, d3, d4, d5, d6 );
    TL_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
//! \param d4 payload
//! \param d5 payload
//! \param d6 payload
//! \param d7 payload
TL_INLINE void tl_8_8_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7 ){
    tl_8_04_ics( d0, d1, d2, d3 );
    tl_8_4_ics( Id, d4, d5, d6, d7 );
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
TL_INLINE void tl_8_08_ics( uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7 ){
    tl_8_04_ics( d0, d1, d2, d3 );
    tl_8_04_ics( d4, d5, d6, d7 );
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
TL_INLINE void tl_8_8_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3, uint32_t d4, uint32_t d5, uint32_t d6, uint32_t d7 ){
    TL_ENTER_CRITICAL_SECTION
    tl_8_8_ics( Id, d0, d1, d2, d3, d4, d5, d6, d7 );
    TL_LEAVE_CRITICAL_SECTION
}

#define tl_16_1_ics tl_8_1_ics //!< same code
#define tl_16_1_ocs tl_8_1_ocs //!< same code

//! trace Id==0 and 16-bit value unprotected (inside critical section)
//! \param d0 payload
TL_INLINE void tl_16_01_ics( uint32_t d0 ){
    tlFifoPush( (d0<<16) );
}

//! trace Id==0 and 8-bit values unprotected (inside critical section)
//! \param d0 payload
//! \param d1 payload
TL_INLINE void tl_16_02_ics( uint32_t d0, uint32_t d1 ){
    tl_16_01_ics( d0 );
    tl_16_01_ics( d1 );
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
TL_INLINE void tl_16_2_ics( uint16_t Id, uint32_t d0, uint32_t d1 ){
    tl_16_01_ics( d0 );
    tl_16_1_ics( Id, d1 );
}

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
TL_INLINE void tl_16_2_ocs( uint16_t Id, uint32_t d0, uint32_t d1 ){
    TL_ENTER_CRITICAL_SECTION
    tl_16_2_ics( Id, d0, d1 );
    TL_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TL_INLINE void tl_16_3_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    tl_16_02_ics( d0, d1 );
    tl_16_1_ics( Id, d2 );
}

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
TL_INLINE void tl_16_3_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2 ){
    TL_ENTER_CRITICAL_SECTION
    tl_16_3_ics( Id, d0, d1, d2 );
    TL_LEAVE_CRITICAL_SECTION
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TL_INLINE void tl_16_4_ics( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    tl_16_02_ics( d0, d1 );
    tl_16_2_ics( Id, d2, d3 );
}

//! trace Id and 16-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
TL_INLINE void tl_16_4_ocs( uint16_t Id, uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3 ){
    TL_ENTER_CRITICAL_SECTION
    tl_16_4_ics( Id, d0, d1, d2, d3 );
    TL_LEAVE_CRITICAL_SECTION
}

//! trace Id and 32-bit value unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
TL_INLINE void tl_32_1_ics( uint16_t Id, uint32_t d0 ){
    tl_16_01_ics( d0 );
    tl_16_1_ics( Id, d0>>16 );
}

//! trace Id and 32-bit value protected (outside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
TL_INLINE void tl_32_1_ocs( uint16_t Id, uint32_t d0 ){
    TL_ENTER_CRITICAL_SECTION
    tl_32_1_ics( Id, d0 );
    TL_LEAVE_CRITICAL_SECTION
}

//! trace Id and 32-bit values unprotected (inside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
TL_INLINE void tl_32_2_ics( uint16_t Id, uint32_t d0, uint32_t d1 ){
    tl_16_02_ics( d0, d0>>16 );
    tl_16_2_ics( Id, d1, d1>>16 );
}

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id tracelog identifier
//! \param d0 payload
//! \param d1 payload
TL_INLINE void tl_32_2_ocs( uint16_t Id, uint32_t d0, uint32_t d1 ){
    TL_ENTER_CRITICAL_SECTION
    tl_32_2_ics( Id, d0, d1 );
    TL_LEAVE_CRITICAL_SECTION
}

#endif // #if 1 == TL_SHORT_MEMORY // #######################################

///////////////////////////////////////////////////////////////////////////////
// little helper for traceLog usage
//

/*! trace log a string
\details not very effective but better than no strings
This function could also be useful, if the string is generated dynamically.
\param s 0-terminated string
*/
void TraceString( char* s ){
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
void ReportLocation( const char* const pFileName, int Line, int Value ){
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
void ReportFailure( const char* const pName, int Line, int Value ){
    TL0( Id(6), "err: Failure in " );
    ReportLocation( pName, Line, Value );
}

/*! Report filename and line number over trace log
\param pFileName pointer to 0-terminated filename
\param Line line number
\param Value for context display
*/
void ReportPassage( char *pFileName, int Line, int Value ){
    TL0( Id(4), "att: Passage in " );
    if( pFileName )
    {
        TraceString( pFileName );
    }
    TL32_2( Id(5), " line %d (Value = %d)\n", Line, Value );
}

void TraceSrcLocation(char *file, int line){
      TL0( Id(31976), "err: Error in file " );
      TraceString( file );
      TL16_1( Id(8272), " at line %d\n", line );
}

#endif // #else // #ifdef TRACELOG_OFF
