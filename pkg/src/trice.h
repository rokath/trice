/*! \file trice.h
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_H_
#define TRICE_H_

#include "triceConfig.h"
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Declarations and Defaults

extern unsigned TriceDepthMax;
extern uint32_t* TriceBufferWritePosition;
unsigned TriceCOBSEncode( uint8_t* restrict output, const uint8_t * restrict input, unsigned length);
void TriceOut( uint32_t* tb, size_t tLen );
void TriceTransfer( void );
void TriceCheckSet( int index ); //!< tests

#if defined( TRICE_UART ) && !defined( TRICE_HALF_BUFFER_SIZE ) // direct out to UART
#define TRICE_WRITE( buf, len ) do{ TriceBlockingWrite( buf, len ); }while(0)
#endif

#ifdef TRICE_RTT_CHANNEL
#include "SEGGER_RTT.h"
#if defined(TRICE_HALF_BUFFER_SIZE) && TRICE_HALF_BUFFER_SIZE > SEGGER BUFFER_SIZE_UP
#error
#endif
#define TRICE_WRITE( buf, len ) do{ SEGGER_RTT_Write(TRICE_RTT_CHANNEL, buf, len ); }while(0)
static inline int TriceOutDepth( void ){ return 0; }
#endif // #ifdef TRICE_RTT_CHANNEL

#ifdef TRICE_TIMESTAMP_VALUE
#define TRICE_COBS_PACKAGE_MODE 1 //! COBS package mode descriptor, 0: no timestamps, 1: 32-bit timestamps
#define TRICE_PUT_TIMESTAMP do{ TRICE_PUT(TRICE_TIMESTAMP_VALUE); }while(0)
#define TRICE_TIMESTAMP_SIZE 4
#else
#define TRICE_COBS_PACKAGE_MODE 0 //! COBS package mode descriptor, 0: no timestamps, 1: 32-bit timestamps
#define TRICE_PUT_TIMESTAMP
#define TRICE_TIMESTAMP_SIZE 0
#endif

#ifndef TRICE_CYCLE_COUNTER
#define TRICE_CYCLE_COUNTER 1 //! TRICE_CYCLE_COUNTER adds a cycle counter to each trice message. The TRICE macros are a bit slower. Lost TRICEs are detectable by the trice tool.
#endif

#ifndef TRICE_SINGLE_MAX_SIZE
#define TRICE_SINGLE_MAX_SIZE 1008 //!< TRICE_SINGLE_MAX_SIZE ist the head size plus string length size plus max dynamic string size. Must be a multiple of 4. 1008 is the max allowed value.
#endif

#if TRICE_SINGLE_MAX_SIZE > 1008
#error
#endif

#ifndef TRICE_TRANSFER_INTERVAL_MS
//! TRICE_TRANSFER_INTERVAL_MS is the milliseconds interval for TRICE buffer read out.
//! This time should be shorter than visible delays. The TRICE_HALF_BUFFER_SIZE must be able to hold all trice messages possibly occouring in this time.
#define TRICE_TRANSFER_INTERVAL_MS 100
#endif

#if TRICE_SINGLE_MAX_SIZE < 256
#define TRICE_DATA_OFFSET 8 //! TRICE_DATA_OFFSET is the space in front of trice data for in-buffer COBS encoding. It must be be a multiple of uint32_t.
#else
#define TRICE_DATA_OFFSET 12 //! TRICE_DATA_OFFSET is the space in front of trice data for in-buffer COBS encoding. It must be a multiple of uint32_t.
#endif

#if TRICE_CYCLE_COUNTER == 1
extern uint8_t TriceCycle;
#define TRICE_CYCLE TriceCycle++ //! TRICE_CYCLE is the trice cycle counter as 8 bit count 0-255.
#else
#define TRICE_CYCLE 0xC0 //! TRICE_CYCLE is no trice cycle counter, just a static value.
#endif

//
///////////////////////////////////////////////////////////////////////////////

#define TRICE_PUT(x) do{ *TriceBufferWritePosition++ = x; }while(0) //! PUT copies a 32 bit x into the TRICE buffer.

#ifdef TRICE_BIG_ENDIANNESS
#define TRICE_PUT64(x) TRICE_PUT( (uint32_t)((uint64_t)(x)>>32) ); TRICE_PUT( (uint32_t)(x) ); 
#else
#define TRICE_PUT64(x) TRICE_PUT( (uint32_t)(x) ); TRICE_PUT( (uint32_t)((uint64_t)(x)>>32) );
#endif

#define TRICE_PUTBUFFER( buf, len ) do{ memcpy( TriceBufferWritePosition, buf, len ); TriceBufferWritePosition += (len+3)>>2; }while(0) //! TRICE_PUTBUFFER copies a buffer into the TRICE buffer.

///////////////////////////////////////////////////////////////////////////////
// trice time measurement (STM32 only?)
//
#if defined( __arm__ )    /* Defined by GNU C and RealView */ \
 || defined( __thumb__ )  /* Defined by GNU C and RealView in Thumb mode */ \
 || defined( _ARM )       /* Defined by ImageCraft C */ \
 || defined( _M_ARM )     /* Defined by Visual Studio */ \
 || defined( _M_ARMT )    /* Defined by Visual Studio in Thumb mode */ \
 || defined( __arm )      /* Defined by Diab */ \
 || defined( __ICCARM__ ) /* IAR */ \
 || defined( __CC_ARM )   /* ARM's (RealView) compiler */ \
 || defined( __ARM__ )    /* TASKING VX ARM toolset C compiler */ \
 || defined( __CARM__ )   /* TASKING VX ARM toolset C compiler */ \
 || defined( __CPARM__ )  /* TASKING VX ARM toolset C++ compiler */
#define SYSTICKVAL (*(volatile uint32_t*)0xE000E018UL)
#else
//#error "unknown architecture"
#define SYSTICKVAL 0
#endif

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// UART interface
//

#if defined( TRICE_UART ) && !defined( TRICE_HALF_BUFFER_SIZE ) // direct out to UART
void TriceBlockingWrite( uint8_t const * buf, int len );
#endif

#if defined( TRICE_UART ) && defined( TRICE_HALF_BUFFER_SIZE ) // buffered out to UART
uint8_t TriceNextUint8( void );
void triceServeTransmit(void);
void triceTriggerTransmit(void);
int TriceOutDepth( void );
uint8_t TriceNextUint8( void );
#endif // #if defined( TRICE_UART ) && TRICE_MODE != 0

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Encryption
//
#ifdef TRICE_ENCRYPT

void TriceEncrypt( uint32_t* p, unsigned count );
void TriceDecrypt( uint32_t* p, unsigned count );

//! little endian! change byte order for big endian machines
#define XTEA_KEY(b00, b01, b02, b03, \
                  b10, b11, b12, b13, \
                  b20, b21, b22, b23, \
                  b30, b31, b32, b33) { \
    0x##b00##b01##b02##b03, \
    0x##b10##b11##b12##b13, \
    0x##b20##b21##b22##b23, \
    0x##b30##b31##b32##b33 }

void TriceInitXteaTable(void);
    
#endif // #ifdef TRICE_ENCRYPT
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Variadic macros
// This is extendable until a 1000 bytes payload.
#define TRICE8_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define TRICE8(id,frmt, ...) TRICE8_COUNT(__VA_ARGS__,TRICE8_12,TRICE8_11,TRICE8_10,TRICE8_9,TRICE8_8,TRICE8_7,TRICE8_6,TRICE8_5,TRICE8_4,TRICE8_3,TRICE8_2,TRICE8_1)(id,frmt, __VA_ARGS__)

#define TRICE16_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define TRICE16(id,frmt, ...) TRICE16_COUNT(__VA_ARGS__,TRICE16_12,TRICE16_11,TRICE16_10,TRICE16_9,TRICE16_8,TRICE16_7,TRICE16_6,TRICE16_5,TRICE16_4,TRICE16_3,TRICE16_2,TRICE16_1)(id,frmt, __VA_ARGS__)

#define TRICE32_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define TRICE32(id,frmt, ...) TRICE32_COUNT(__VA_ARGS__,TRICE32_12,TRICE32_11,TRICE32_10,TRICE32_9,TRICE32_8,TRICE32_7,TRICE32_6,TRICE32_5,TRICE32_4,TRICE32_3,TRICE32_2,TRICE32_1)(id,frmt, __VA_ARGS__)

#define TRICE64_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define TRICE64(id,frmt, ...) TRICE64_COUNT(__VA_ARGS__,TRICE64_12,TRICE64_11,TRICE64_10,TRICE64_9,TRICE64_8,TRICE64_7,TRICE64_6,TRICE64_5,TRICE64_4,TRICE64_3,TRICE64_2,TRICE64_1)(id,frmt, __VA_ARGS__)
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//

//! TRICE_S writes id and dynString.
//! \param id trice identifier
//! \param pFmt formatstring for trice (ignored here but used by the trice tool)
//! \param dynString 0-terminated runtime generated string
//! After the 4 byte trice message header are following 2^n bytes 
//! string transfer format: 
//! idH    idL    len    cycle
//! c0     c1     c2     c3
//! ...
//! cLen-3 cLen-2 cLen-1 cLen
#define TRICE_S( id, pFmt, dynString) do { \
    uint32_t len = strlen( dynString ); \
    uint32_t limit = TRICE_SINGLE_MAX_SIZE-TRICE_DATA_OFFSET-TRICE_TIMESTAMP_SIZE-8; /* 8 = head size plus len size */ \
    if( len > limit ){ \
        TRICE32( Id( 54343), "wrn:Dynamic string truncated from %u to %u\n", len, limit ); \
        len = limit; \
    } \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | (0xff00 & ((len+7)<<6)) | TRICE_CYCLE ); /* +3 for padding, +4 for the buf size value transmitted in the payload to get the last 2 bits. */ \
    TRICE_PUT( len ); /* len as byte does not contain the exact buf len anymore, so transmit it to the host */ \
    /* len is needed for non string buffers because the last 2 bits not stored in head. */ \
    /* All trices know the data length but not TRICE8P. len byte values 0xFC, xFD, xFE, xFF are reserved for future extensions. */ \
    TRICE_PUTBUFFER( dynString, len ); \
    TRICE_LEAVE \
} while(0)

#ifdef TRICE_OFF // do not generate trice code for files defining TRICE_OFF before including "trice.h"
#undef  TRICE_ENTER
#define TRICE_ENTER
#undef  PUT
#define TRICE_PUT(n)
#undef  TRICE_PUT_TIMESTAMP
#define TRICE_PUT_TIMESTAMP
#undef  PUT_BUFFER
#define PUT_BUFFER(b,l)
#undef  TRICE_LEAVE
#define TRICE_LEAVE
#undef  TRICE_S
#define TRICE_S( id, p, s )  do{ ((void)(id)); ((void)(p)); ((void)(s)); }while(0)
#endif // #ifdef TRICE_OFF

#define Id(n) ((uint32_t)n<<16) //!< Id() is a 16 bit id 0-65535 as upper 2 bytes in head

//! TRICE0 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define TRICE0( id, pFmt ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0000 | TRICE_CYCLE ); \
    TRICE_LEAVE

//! TRICE8_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define TRICE8_1( id, pFmt, v0 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE ); \
    TRICE_PUT( (uint8_t)(v0) ); /* little endian*/ \
    TRICE_LEAVE

//! TRICE8_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 8 bit bit values
#define TRICE8_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE ); \
    TRICE_PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) ); \
    TRICE_LEAVE

//! TRICE8_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 8 bit bit values
#define TRICE8_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE ); \
    TRICE_PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) ); \
    TRICE_LEAVE

//! TRICE8_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 8 bit bit values
#define TRICE8_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE ); \
    TRICE_PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    TRICE_LEAVE

//! TRICE8_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 8 bit bit values
#define TRICE8_5( id, pFmt, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    TRICE_PUT( (uint8_t)(v4) ); \
    TRICE_LEAVE

//! TRICE8_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 8 bit bit values
#define TRICE8_6( id, pFmt, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    TRICE_PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 8 bit bit values
#define TRICE8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    TRICE_PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) | ((uint32_t)(0xff&(v6))<<16) ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    TRICE_PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) | ((uint32_t)(0xff&(v6))<<16) | ((uint32_t)(v7)<<24) ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_9( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    TRICE_PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) | ((uint32_t)(0xff&(v6))<<16) | ((uint32_t)(v7)<<24) ); \
    TRICE_PUT( (uint8_t)(v8) ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_10( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    TRICE_PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) | ((uint32_t)(0xff&(v6))<<16) | ((uint32_t)(v7)<<24) ); \
    TRICE_PUT( (uint8_t)(v8) | ((uint16_t)(v9)<<8) ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_11( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    TRICE_PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) | ((uint32_t)(0xff&(v6))<<16) | ((uint32_t)(v7)<<24) ); \
    TRICE_PUT( (uint8_t)(v8) | ((uint16_t)(v9)<<8) | ((uint32_t)(0xff&(v10))<<16) ); \
    TRICE_LEAVE

//! TRICE8_12 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 8 bit bit values
#define TRICE8_12( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    TRICE_PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) | ((uint32_t)(0xff&(v6))<<16) | ((uint32_t)(v7)<<24) ); \
    TRICE_PUT( (uint8_t)(v8) | ((uint16_t)(v9)<<8) | ((uint32_t)(0xff&(v10))<<16)| ((uint32_t)(v11)<<24) ); \
    TRICE_LEAVE

//! TRICE16_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define TRICE16_1( id, pFmt, v0 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE ); \
    TRICE_PUT( (uint16_t)(v0) ); \
    TRICE_LEAVE

//! TRICE16_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 16 bit values
#define TRICE16_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_LEAVE

//! TRICE16_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 16 bit values
#define TRICE16_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT( (uint16_t)(v2) ); \
    TRICE_LEAVE

//! TRICE16_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 16 bit values
#define TRICE16_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_LEAVE

//! TRICE16_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 16 bit values
#define TRICE16_5( id, pFmt, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_PUT((uint16_t)(v4) ); \
    TRICE_LEAVE

//! TRICE16_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 16 bit values
#define TRICE16_6( id, pFmt, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_PUT((uint16_t)(v4) | ((uint32_t)(v5)<<16) ); \
    TRICE_LEAVE

//! TRICE16_7 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 16 bit values
#define TRICE16_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0400 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_PUT((uint16_t)(v4) | ((uint32_t)(v5)<<16) ); \
    TRICE_PUT((uint16_t)(v6) ); \
    TRICE_LEAVE

//! TRICE16_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 16 bit values
#define TRICE16_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0400 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_PUT((uint16_t)(v4) | ((uint32_t)(v5)<<16) ); \
    TRICE_PUT((uint16_t)(v6) | ((uint32_t)(v7)<<16) ); \
    TRICE_LEAVE
    
//! TRICE16_9 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v8 are 16 bit values
#define TRICE16_9( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0500 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_PUT((uint16_t)(v4) | ((uint32_t)(v5)<<16) ); \
    TRICE_PUT((uint16_t)(v6) | ((uint32_t)(v7)<<16) ); \
    TRICE_PUT((uint16_t)(v8) ); \
    TRICE_LEAVE

//! TRICE16_10 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v9 are 16 bit values
#define TRICE16_10( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0500 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_PUT((uint16_t)(v4) | ((uint32_t)(v5)<<16) ); \
    TRICE_PUT((uint16_t)(v6) | ((uint32_t)(v7)<<16) ); \
    TRICE_PUT((uint16_t)(v8) | ((uint32_t)(v9)<<16) ); \
    TRICE_LEAVE
    
//! TRICE16_11 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v10 are 16 bit values
#define TRICE16_11( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0600 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_PUT((uint16_t)(v4) | ((uint32_t)(v5)<<16) ); \
    TRICE_PUT((uint16_t)(v6) | ((uint32_t)(v7)<<16) ); \
    TRICE_PUT((uint16_t)(v8) | ((uint32_t)(v9)<<16) ); \
    TRICE_PUT((uint16_t)(v10) ); \
    TRICE_LEAVE
    
//! TRICE16_12 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 16 bit values
#define TRICE16_12( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0600 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_PUT((uint16_t)(v4) | ((uint32_t)(v5)<<16) ); \
    TRICE_PUT((uint16_t)(v6) | ((uint32_t)(v7)<<16) ); \
    TRICE_PUT((uint16_t)(v8) | ((uint32_t)(v9)<<16) ); \
    TRICE_PUT((uint16_t)(v10)| ((uint32_t)(v11)<<16) ); \
    TRICE_LEAVE

//! TRICE32_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 the 32 bit value
#define TRICE32_1( id, pFmt, v0 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_LEAVE

//! TRICE32_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 32 bit values
#define TRICE32_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT(id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_LEAVE

//! TRICE32_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 32 bit values
#define TRICE32_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_LEAVE

//! TRICE32_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 32 bit values
#define TRICE32_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0400 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_PUT( (uint32_t)(v3) ); \
    TRICE_LEAVE

//! TRICE32_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 32 bit values
#define TRICE32_5( id, pFmt,  v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0500 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_PUT( (uint32_t)(v3) ); \
    TRICE_PUT( (uint32_t)(v4) ); \
    TRICE_LEAVE

//! TRICE32_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 32 bit values
#define TRICE32_6( id, pFmt,  v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0600 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_PUT( (uint32_t)(v3) ); \
    TRICE_PUT( (uint32_t)(v4) ); \
    TRICE_PUT( (uint32_t)(v5) ); \
    TRICE_LEAVE

//! TRICE32_7 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 32 bit values
#define TRICE32_7( id, pFmt,  v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0700 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_PUT( (uint32_t)(v3) ); \
    TRICE_PUT( (uint32_t)(v4) ); \
    TRICE_PUT( (uint32_t)(v5) ); \
    TRICE_PUT( (uint32_t)(v6) ); \
    TRICE_LEAVE

//! TRICE32_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 32 bit values
#define TRICE32_8( id, pFmt,  v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0800 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_PUT( (uint32_t)(v3) ); \
    TRICE_PUT( (uint32_t)(v4) ); \
    TRICE_PUT( (uint32_t)(v5) ); \
    TRICE_PUT( (uint32_t)(v6) ); \
    TRICE_PUT( (uint32_t)(v7) ); \
    TRICE_LEAVE

//! TRICE32_9 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v8 are 32 bit values
#define TRICE32_9( id, pFmt,  v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0900 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_PUT( (uint32_t)(v3) ); \
    TRICE_PUT( (uint32_t)(v4) ); \
    TRICE_PUT( (uint32_t)(v5) ); \
    TRICE_PUT( (uint32_t)(v6) ); \
    TRICE_PUT( (uint32_t)(v7) ); \
    TRICE_PUT( (uint32_t)(v8) ); \
    TRICE_LEAVE

//! TRICE32_10 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - 9 are 32 bit values
#define TRICE32_10( id, pFmt,  v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0a00 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_PUT( (uint32_t)(v3) ); \
    TRICE_PUT( (uint32_t)(v4) ); \
    TRICE_PUT( (uint32_t)(v5) ); \
    TRICE_PUT( (uint32_t)(v6) ); \
    TRICE_PUT( (uint32_t)(v7) ); \
    TRICE_PUT( (uint32_t)(v8) ); \
    TRICE_PUT( (uint32_t)(v9) ); \
    TRICE_LEAVE

//! TRICE32_11 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v10 are 32 bit values
#define TRICE32_11( id, pFmt,  v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0b00 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_PUT( (uint32_t)(v3) ); \
    TRICE_PUT( (uint32_t)(v4) ); \
    TRICE_PUT( (uint32_t)(v5) ); \
    TRICE_PUT( (uint32_t)(v6) ); \
    TRICE_PUT( (uint32_t)(v7) ); \
    TRICE_PUT( (uint32_t)(v8) ); \
    TRICE_PUT( (uint32_t)(v9) ); \
    TRICE_PUT( (uint32_t)(v10) ); \
    TRICE_LEAVE

//! TRICE32_12 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 32 bit values
#define TRICE32_12( id, pFmt,  v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0c00 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_PUT( (uint32_t)(v3) ); \
    TRICE_PUT( (uint32_t)(v4) ); \
    TRICE_PUT( (uint32_t)(v5) ); \
    TRICE_PUT( (uint32_t)(v6) ); \
    TRICE_PUT( (uint32_t)(v7) ); \
    TRICE_PUT( (uint32_t)(v8) ); \
    TRICE_PUT( (uint32_t)(v9) ); \
    TRICE_PUT( (uint32_t)(v10) ); \
    TRICE_PUT( (uint32_t)(v11) ); \
    TRICE_LEAVE

//! TRICE64_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 is a 64 bit values
#define TRICE64_1( id, pFmt, v0 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_LEAVE

//! TRICE64_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 64 bit values
#define TRICE64_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0400 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_LEAVE

//! TRICE64_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 64 bit values
#define TRICE64_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0600 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_LEAVE


//! TRICE64_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 64 bit values
#define TRICE64_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0800 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_PUT64( v3 ); \
    TRICE_LEAVE

//! TRICE64_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 64 bit values
#define TRICE64_5( id, pFmt,  v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0a00 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_PUT64( v3 ); \
    TRICE_PUT64( v4 ); \
    TRICE_LEAVE

//! TRICE64_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 64 bit values
#define TRICE64_6( id, pFmt,  v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0c00 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_PUT64( v3 ); \
    TRICE_PUT64( v4 ); \
    TRICE_PUT64( v5 ); \
    TRICE_LEAVE

//! TRICE64_7 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 64 bit values
#define TRICE64_7( id, pFmt,  v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x0e00 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_PUT64( v3 ); \
    TRICE_PUT64( v4 ); \
    TRICE_PUT64( v5 ); \
    TRICE_PUT64( v6 ); \
    TRICE_LEAVE
    
//! TRICE64_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 64 bit values
#define TRICE64_8( id, pFmt,  v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x1000 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_PUT64( v3 ); \
    TRICE_PUT64( v4 ); \
    TRICE_PUT64( v5 ); \
    TRICE_PUT64( v6 ); \
    TRICE_PUT64( v7 ); \
    TRICE_LEAVE

//! TRICE64_9 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v8 are 64 bit values
#define TRICE64_9( id, pFmt,  v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x1200 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_PUT64( v3 ); \
    TRICE_PUT64( v4 ); \
    TRICE_PUT64( v5 ); \
    TRICE_PUT64( v6 ); \
    TRICE_PUT64( v7 ); \
    TRICE_PUT64( v8 ); \
    TRICE_LEAVE

//! TRICE64_10 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v9 are 64 bit values
#define TRICE64_10( id, pFmt,  v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x1400 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_PUT64( v3 ); \
    TRICE_PUT64( v4 ); \
    TRICE_PUT64( v5 ); \
    TRICE_PUT64( v6 ); \
    TRICE_PUT64( v7 ); \
    TRICE_PUT64( v8 ); \
    TRICE_PUT64( v9 ); \
    TRICE_LEAVE

//! TRICE64_11 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v10 are 64 bit values
#define TRICE64_11( id, pFmt,  v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x1600 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_PUT64( v3 ); \
    TRICE_PUT64( v4 ); \
    TRICE_PUT64( v5 ); \
    TRICE_PUT64( v6 ); \
    TRICE_PUT64( v7 ); \
    TRICE_PUT64( v8 ); \
    TRICE_PUT64( v9 ); \
    TRICE_PUT64( v10 ); \
    TRICE_LEAVE

//! TRICE64_12 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 64 bit values
#define TRICE64_12( id, pFmt,  v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    TRICE_PUT_TIMESTAMP; \
    TRICE_PUT( id | 0x1800 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_PUT64( v3 ); \
    TRICE_PUT64( v4 ); \
    TRICE_PUT64( v5 ); \
    TRICE_PUT64( v6 ); \
    TRICE_PUT64( v7 ); \
    TRICE_PUT64( v8 ); \
    TRICE_PUT64( v9 ); \
    TRICE_PUT64( v10 ); \
    TRICE_PUT64( v11 ); \
    TRICE_LEAVE

#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
