 /*! \file bare.h
\brief Software tracer header file
\details This file is included in target code files. If TRICE_CODE is defined
as NO_CODE (globally or file specific) the TRICE* macros generate no code.
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef BARE_H_
#define BARE_H_

#ifdef __cplusplus
extern "C" {
#endif

//! used as TRICE_CODE macro option for more flash occupation, but decreases execution time and needs smaller buffers
#define MORE_FLASH_AND_SPEED 30 //!< value is only to distinguish from LESS_FLASH and NO_CODE

//! used as TRICE_CODE macro option for less flash occupation, but increases execution time and needs bigger buffers
#define LESS_FLASH_AND_SPEED 20 //!< value is only to distinguish from MORE_FLASH and NO_CODE

//! used as TRICE_CODE macro option for no trice code generation
#define NO_CODE 10 //!< value is only to distinguish from MORE_FLASH or LESS_FLASH ans must be 0

#include "bareConfig.h"

#define TRICE_PUSH(v) triceTricePush(v)

//! TRICE_SYNC is an optional trice sync message for syncing, when bare transmission is used.
//! The value 35243 (0x89ab) is a reserved pattern used as ID with value DA 0xcdef.
//! The byte sequence of the sync message is 0x89 0xab 0xcd 0xef.
//! It cannot occure in the trice stream in another way due to ID generaion policy.
//! Sync package is IDDA=89abcdef
//!
//! To avoid wrong syncing these ID's are excluded: xx89, abcd, cdef, efxx (514 pieces)
//!
//! Possible:    IH IL DH DL IH IL DH DL IH IL DH DL (1 right)
//!              xx xx xx xx xx 89 ab cd ef xx xx xx -> avoid IL=89, IH=ef
//!
//! Possible:    IH IL DH DL IH IL DH DL IH IL DH DL (2 right)
//!              xx xx xx xx xx xx 89 ab cd ef xx xx -> avoid ID=cdef
//!
//! Possible:    IH IL DH DL IH IL DH DL IH IL DH DL (3 right)
//!              xx xx xx xx xx xx xx 89 ab cd ef xx -> avoid ID=abcd
//!
//! Sync packet: IH IL DH DL IH IL DH DL IH IL DH DL
//!              xx xx xx xx 89 ab cd ef xx xx xx xx -> use ID=89ab with DA=cdef as sync packet
//!
//!  Possible:   IH IL DH DL IH IL DH DL IH IL DH DL (1 left)
//!              xx xx xx 89 ab cd ef xx xx xx xx xx -> avoid ID=abcd
//!
//!  Possible:   IH IL DH DL IH IL DH DL IH IL DH DL (2 left)
//!              xx xx 89 ab cd ef xx xx xx xx xx xx -> avoid ID=cdef
//!
//!  Possible:   IH IL DH DL IH IL DH DL IH IL DH DL (3 left)
//!              xx 89 ab cd ef xx xx xx xx xx xx xx ->  avoid IL=nn89, IH=ef
//!
//! If an ID=89ab with DA!=cdef is detected -> out of sync!
//! If an IH=ef is detected -> out of sync, all 256 IDs starting with 0xef are excluded
//! If an IL=89 is detected -> out of sync, all 256 IDs ending with 0x89 are excluded
//! If an ID=abcd is detected -> out of sync, ID 0xabcd is excluded
//! If an ID=cdef is detected -> out of sync, ID 0xcdef is excluded
//! ID 0x89ab is reserved for this trice sync package.
//! The trice sync message payload must be 0xcdef.
//! You must not change any of the above demands. Otherwise the syncing will not work.
//! If for some reason the Id changes during 'trice u', probably when the string changed accidently, 
//! you need to remove the old pattern from til.json and put Id(35243) manually here.
//! The trice sync string makes the trice sync info invisible just in case,
//! but the trice tool will filter them out anyway. The trice tool automatic id generation
//! follows these rules.
#define TRICE_SYNC do{ TRICE16_1( Id(35243), "%04x\b\b\b\b", 0xcdef ); }while(0)

#define Id( n ) (n) //!< Macro for improved trice readability and better source code parsing.

#define TRICE_FIFO_MASK (((TRICE_FIFO_BYTE_SIZE)>>2)-1) //!< max possible trices count in fifo

extern uint32_t triceFifo[];
extern uint32_t triceFifoWriteIndex;
extern uint32_t triceFifoReadIndex;
extern uint32_t triceFifoMaxDepthTrices; //!< usabble for diagnostics
extern uint8_t triceBytesBuffer[8];
extern int triceBytesBufferIndex;

void triceServeOut( void );

//! triceTricePush puts one trice into trice fifo.
//! This is a trice time critical part.
//! \param v trice id with 2 byte data
TRICE_INLINE void triceTricePush( uint32_t v ){
    triceFifo[triceFifoWriteIndex++] = v;
    triceFifoWriteIndex &= TRICE_FIFO_MASK;
}

//! triceTricePop gets one trice from trice fifo.
//! \return trice id with 2 byte data in one uint32_t.
TRICE_INLINE uint32_t triceTricePop(){
    uint32_t v = triceFifo[triceFifoReadIndex++];
    triceFifoReadIndex &= TRICE_FIFO_MASK;
    return v;
}

//! triceFifoDepth determines trices count inside trice fifo.
//! \return count of buffered trices
TRICE_INLINE unsigned triceFifoDepth( void ){
    unsigned triceDepth = (triceFifoWriteIndex - triceFifoReadIndex) & TRICE_FIFO_MASK;
    triceFifoMaxDepthTrices = triceDepth < triceFifoMaxDepthTrices ? triceFifoMaxDepthTrices : triceDepth; // diagnostics
    return triceDepth;
}

//! triceServeUartOut() must be called cyclically to proceed ongoing write out.
//! A good place: sysTick ISR and UART ISR (both together).
//! TODO: endianess with compiler macros.
TRICE_INLINE void triceServeTransmit( void ){
    if( sizeof(triceBytesBuffer) <= triceBytesBufferIndex  ){ // no more bytes
        triceBytesBufferIndex = sizeof(triceBytesBuffer)+1; // signal tx done
        triceDisableTxEmptyInterrupt();
        return;
    }
    // next byte
    triceTransmitData8( triceBytesBuffer[triceBytesBufferIndex++] );
    triceEnableTxEmptyInterrupt();
}

#if NO_CODE == TRICE_CODE // no trice code generation

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

#endif // #if NO_CODE == TRICE_CODE // trice code generation

#if MORE_FLASH_AND_SPEED == TRICE_CODE // ###############################################

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//

//! basic trice macro, assumes d16 to be
//! id trice identifier
//! \param d16 a 16 bit value
#define TRICE( id, d16 ) do{ \
    TRICE_PUSH( (((uint32_t)(uint16_t)(d16))<<16) | (id)); \
} while(0)

//! basic trice macro, assumes d16 to be a 16 bit value
//! id is 0
//! \param d16 a 16 bit value
#define TRICE_ID0( d16 ) do{ \
    TRICE_PUSH( ((uint32_t)(uint16_t)(d16))<<16); \
} while(0)

//! trace Id protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
#define TRICE0( Id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE_PUSH( Id ); \
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
    TRICE( Id,((((uint32_t)((uint8_t)(d1)))<<8) | ((uint8_t)(d0)))) ; \
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
    TRICE_ID0( (((uint32_t)((uint8_t)(d1)))<<8) | (uint8_t)(d0) ) ; \
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
    TRICE_ID0(((((uint32_t)(uint8_t)(d1))<<8) | (uint8_t)(d0))) ; \
    TRICE( Id,((((uint32_t)(uint8_t)(d3))<<8) | (uint8_t)(d2))) ; \
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
    TRICE_ID0(((((uint32_t)(uint8_t)(d1))<<8) | (uint8_t)(d0))) ; \
    TRICE_ID0(((((uint32_t)(uint8_t)(d3))<<8) | (uint8_t)(d2))) ; \
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
    TRICE_ID0(((((uint32_t)(uint8_t)(d1))<<8) | (uint8_t)(d0))) ; \
    TRICE_ID0(((((uint32_t)(uint8_t)(d3))<<8) | (uint8_t)(d2))) ; \
    TRICE( Id,((((uint32_t)(uint8_t)(d5))<<8) | (uint8_t)(d4))) ; \
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
    TRICE_ID0( (((uint32_t)(uint8_t)(d1))<<8) | (uint8_t)(d0) ) ; \
    TRICE_ID0( (((uint32_t)(uint8_t)(d3))<<8) | (uint8_t)(d2) ) ; \
    TRICE_ID0( (((uint32_t)(uint8_t)(d5))<<8) | (uint8_t)(d4) ) ; \
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
    TRICE_ID0(((((uint32_t)(uint8_t)(d1))<<8) | (uint8_t)(d0))) ; \
    TRICE_ID0(((((uint32_t)(uint8_t)(d3))<<8) | (uint8_t)(d2))) ; \
    TRICE_ID0(((((uint32_t)(uint8_t)(d5))<<8) | (uint8_t)(d4))) ; \
    TRICE( Id,((((uint32_t)(uint8_t)(d7))<<8) | (uint8_t)(d6))) ; \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 16-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE16_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    TRICE( Id, d0 ); \
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

#endif // #if MORE_FLASH_AND_SPEED == TRICE_CODE // ######################################

#if LESS_FLASH_AND_SPEED == TRICE_CODE // ################################################

///////////////////////////////////////////////////////////////////////////////
// internal trice functions
//

//! trace Id unprotected (inside critical section)
//! \param Id trice identifier
TRICE_INLINE void trice_0_ics( uint16_t Id ){
    TRICE_PUSH(Id);
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
    TRICE_PUSH( (d0<<16) | Id);
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
    TRICE_PUSH( (d1<<24) | (d0<<16) );
}

//! trace Id and 8-bit values unprotected (inside critical section)
//! \param Id trice identifier
//! \param d0 payload
//! \param d1 payload
TRICE_INLINE void trice_8_2_ics( uint16_t Id, uint32_t d0, uint32_t d1 ){
    TRICE_PUSH( (d1<<24) | (d0<<16) | Id);
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
    trice_8_1_ics( Id, d4 );
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
    TRICE_PUSH( (d0<<16) );
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

#endif // #if LESS_FLASH_AND_SPEED == TRICE_CODE // #####################################

#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
