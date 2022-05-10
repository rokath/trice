/*! \file trice.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_H_
#define TRICE_H_

#ifdef TRICE_OFF // do not generate trice code for files defining TRICE_OFF before including "trice.h"
#define TRICE_CYCLE_COUNTER 0 // why needed here?
#define TRICE_INTO
#define TRICE_PUT(n) do{ ((void)(n)); }while(0)
#define PUT_BUFFER(b,l) do{ ((void)(b)); ((void)(l)); }while(0)
#define TRICE_LEAVE
#define TRICE_S( id, p, s )  do{ ((void)(id)); ((void)(p)); ((void)(s)); }while(0)
#define TRICE_N( id, p, s, n )  do{ ((void)(id)); ((void)(p)); ((void)(s)); ((void)(n)); }while(0)
#endif

#include "triceConfig.h"
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Declarations and Defaults

size_t TriceDepthMax( void );
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
#if defined(TRICE_HALF_BUFFER_SIZE) && TRICE_HALF_BUFFER_SIZE > BUFFER_SIZE_UP
#error
#endif
#if defined(TRICE_STACK_BUFFER_SIZE) && TRICE_STACK_BUFFER_SIZE > BUFFER_SIZE_UP
#error
#endif
//#define TRICE_WRITE( buf, len ) do{ SEGGER_RTT_Write(TRICE_RTT_CHANNEL, buf, len ); }while(0)
static inline int TriceOutDepth( void ){ return 0; }
#endif // #ifdef TRICE_RTT_CHANNEL

//! The TRICE_PUT_PREFIX macro adds optionally target timestamp and location in front of each trice
#if !defined(TRICE_LOCATION) && !defined(TRICE_TIMESTAMP)
#define TRICE_COBS_PACKAGE_MODE 0
#define TRICE_PUT_PREFIX
#define TRICE_PREFIX_SIZE 0
#endif
#if !defined(TRICE_LOCATION) &&  defined(TRICE_TIMESTAMP)
#define TRICE_COBS_PACKAGE_MODE 1
#define TRICE_PUT_PREFIX TRICE_PUT(TRICE_TIMESTAMP);
#define TRICE_PREFIX_SIZE 4
#endif
#if  defined(TRICE_LOCATION) && !defined(TRICE_TIMESTAMP)
#define TRICE_COBS_PACKAGE_MODE 2
#define TRICE_PUT_PREFIX TRICE_PUT(TRICE_LOCATION); 
#define TRICE_PREFIX_SIZE 4
#endif
#if  defined(TRICE_LOCATION) &&  defined(TRICE_TIMESTAMP)
#define TRICE_COBS_PACKAGE_MODE 3
#define TRICE_PUT_PREFIX TRICE_PUT(TRICE_LOCATION); TRICE_PUT(TRICE_TIMESTAMP); 
#define TRICE_PREFIX_SIZE 8
#endif

#ifndef TRICE_CYCLE_COUNTER
#define TRICE_CYCLE_COUNTER 1 //! TRICE_CYCLE_COUNTER adds a cycle counter to each trice message. The TRICE macros are a bit slower. Lost TRICEs are detectable by the trice tool.
#endif

//! TRICE_DATA_OFFSET is the space in front of trice data for in-buffer COBS encoding. It must be be a multiple of uint32_t.
#if defined(TRICE_HALF_BUFFER_SIZE)
#define TRICE_DATA_OFFSET ((9+(TRICE_HALF_BUFFER_SIZE/256))&~3) // 9: COBS_DESCRIPTOR size plus start byte plus up to 4 0-delimiters
#else
#define TRICE_DATA_OFFSET 16 // usually 8 is enough: 4 for COBS_DESCRIPTOR and additional bytes for COBS encoding, but the buffer can get big.
#endif

#if defined(TRICE_STACK_BUFFER_MAX_SIZE) && !defined(TRICE_SINGLE_MAX_SIZE)
#define TRICE_SINGLE_MAX_SIZE (TRICE_STACK_BUFFER_MAX_SIZE - TRICE_DATA_OFFSET)
#endif

#ifndef TRICE_SINGLE_MAX_SIZE
#define TRICE_SINGLE_MAX_SIZE 1008 //!< TRICE_SINGLE_MAX_SIZE ist the head size plus string length size plus max dynamic string size. Must be a multiple of 4. 1008 is the max allowed value.
#endif

#if TRICE_SINGLE_MAX_SIZE > 1008
#error
#endif

#if defined(TRICE_STACK_BUFFER_MAX_SIZE) && defined(TRICE_SINGLE_MAX_SIZE) && TRICE_SINGLE_MAX_SIZE + TRICE_DATA_OFFSET > TRICE_STACK_BUFFER_MAX_SIZE
#error
#endif

#if defined(TRICE_HALF_BUFFER_SIZE) && TRICE_HALF_BUFFER_SIZE < TRICE_SINGLE_MAX_SIZE + TRICE_DATA_OFFSET
#error
#endif

#ifndef TRICE_TRANSFER_INTERVAL_MS
//! TRICE_TRANSFER_INTERVAL_MS is the milliseconds interval for TRICE buffer read out.
//! This time should be shorter than visible delays. The TRICE_HALF_BUFFER_SIZE must be able to hold all trice messages possibly occouring in this time.
#define TRICE_TRANSFER_INTERVAL_MS 100
#endif

#if TRICE_CYCLE_COUNTER == 1
extern uint8_t TriceCycle;
#define TRICE_CYCLE TriceCycle++ //! TRICE_CYCLE is the trice cycle counter as 8 bit count 0-255.
#else
#define TRICE_CYCLE 0xC0 //! TRICE_CYCLE is no trice cycle counter, just a static value.
#endif

//
///////////////////////////////////////////////////////////////////////////////

#ifndef TRICE_PUT
#define TRICE_PUT(x) do{ *TriceBufferWritePosition++ = x; }while(0) //! PUT copies a 32 bit x into the TRICE buffer.
#endif

#ifdef TRICE_BIG_ENDIANNESS
#define TRICE_PUT64(x) TRICE_PUT( (uint32_t)((uint64_t)(x)>>32) ); TRICE_PUT( (uint32_t)(x) ); 
#else
#define TRICE_PUT64(x) TRICE_PUT( (uint32_t)(x) ); TRICE_PUT( (uint32_t)((uint64_t)(x)>>32) );
#endif

#ifndef TRICE_PUTBUFFER
#define TRICE_PUTBUFFER( buf, len ) do{ memcpy( TriceBufferWritePosition, buf, len ); TriceBufferWritePosition += (len+3)>>2; }while(0) //! TRICE_PUTBUFFER copies a buffer into the TRICE buffer.
#endif

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
void TriceBlockingWrite( uint8_t const * buf, unsigned len );
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
// Variadic macros (https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms)
// This is extendable until a 1000 bytes payload.
#define TRICE8_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define TRICE8(id,frmt, ...) TRICE8_COUNT(__VA_ARGS__,TRICE8_12,TRICE8_11,TRICE8_10,TRICE8_9,TRICE8_8,TRICE8_7,TRICE8_6,TRICE8_5,TRICE8_4,TRICE8_3,TRICE8_2,TRICE8_1)(id,frmt, __VA_ARGS__)

#define TRICE16_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define TRICE16(id,frmt, ...) TRICE16_COUNT(__VA_ARGS__,TRICE16_12,TRICE16_11,TRICE16_10,TRICE16_9,TRICE16_8,TRICE16_7,TRICE16_6,TRICE16_5,TRICE16_4,TRICE16_3,TRICE16_2,TRICE16_1)(id,frmt, __VA_ARGS__)

#define TRICE32_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define TRICE32(id,frmt, ...) TRICE32_COUNT(__VA_ARGS__,TRICE32_12,TRICE32_11,TRICE32_10,TRICE32_9,TRICE32_8,TRICE32_7,TRICE32_6,TRICE32_5,TRICE32_4,TRICE32_3,TRICE32_2,TRICE32_1)(id,frmt, __VA_ARGS__)

#define TRICE64_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define TRICE64(id,frmt, ...) TRICE64_COUNT(__VA_ARGS__,TRICE64_12,TRICE64_11,TRICE64_10,TRICE64_9,TRICE64_8,TRICE64_7,TRICE64_6,TRICE64_5,TRICE64_4,TRICE64_3,TRICE64_2,TRICE64_1)(id,frmt, __VA_ARGS__)

// See for more explanation https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/preprocessor/macros/__VA_ARGS__/count-arguments

//! NTH_ARGUMENT just evaluates to the 14th argument. It is extendable.
#define NTH_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, ...) a14 

//! COUNT_ARGUMENTS builds upon NTH_ARGUMENT. The more arguments that are passed to COUNT_ARGUMENTS, 
//! the more the »counting arguments« (12, 11, 10, 9, 8, 7…) are pushed to the right. 
//! Thus the macro evaluates to the number of arguments that are passed to the macro.
//! The expression `## __VA_ARGS__` ist not supported by older compilers. You can remove the `##` and use TRICE0 instead of TRICE for a no parameter value TRICE in that case.
#define COUNT_ARGUMENTS(...) NTH_ARGUMENT(dummy, ## __VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

//! CONCAT concatenates the 2 arguments a and b (helper macro).
#define CONCAT(a, b) a ## b 

//! CONCAT2 concatenates the 2 arguments a and b (helper macro).
#define CONCAT2(a, b) CONCAT(a, b)

//! TRICE_VARIABLE_ARGUMENTS concatenates DEBUG_ with the result of COUNT_ARGUMENTS to produce something like DEBUG_2 which takes a printf-format and two arguments.
#define TRICE(id, fmt, ...) CONCAT2(TRICE_, COUNT_ARGUMENTS(__VA_ARGS__))(id, fmt, ##__VA_ARGS__)

//
///////////////////////////////////////////////////////////////////////////////


/* pre C99
// aFloat returns passed float value x as bit pattern in a uint32_t type.
static inline uint32_t aFloat( float x ){
    union {
        float f;
        uint32_t u;
    } t;
    t.f = x;
    return t.u;
}
*/

// aFloat returns passed float value x as bit pattern in a uint32_t type.
static inline uint32_t aFloat( float f ){
    union {
        float    from;
        uint32_t to;
    } pun = { .from = f };
    return pun.to;
}

// asFloat returns passed uint32_t value x bit pattern as float type.
static inline float asFloat( uint32_t x ){
    union {
        uint32_t from;
        float    to;
    } pun = { .from = x };
    return pun.to;
}

// aDouble returns passed double value x as bit pattern in a uint64_t type.
static inline uint64_t aDouble( double x ){
    union {
        double d;
        uint64_t u;
    } t;
    t.d = x;
    return t.u;
}

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//

#define TRICE_0  TRICE0  //!< Only the format string without parameter values.

#ifndef TRICE_INTO
#define TRICE_INTO TRICE_ENTER TRICE_PUT_PREFIX;
#endif

#ifndef TRICE_N
//! TRICE_N writes id and buffer of size len.
//! \param id trice identifier
//! \param pFmt formatstring for trice (ignored here but used by the trice tool), could contain any add on information. The trice tool "sees" the "TRICE_N" and can handle that.
//! \param dynString 0-terminated runtime generated string
//! After the 4 byte trice message header are following 4 bytes coding n (only 2 used) and the buffer
//! transfer format: 
//! idH    idL    len    cycle <- trice header
//! n0     n1     n2     n3    <- payload count without paddings bytes (transmittable len)
//! c0     c1     c2     c3    <- buffer
//! ...                        <- buffer
//! cLen-3 cLen-2 cLen-1 cLen  <- buffer ending with maybe 1-3 undetermined padding bytes
//
// todo: for some reason this macro is not working well wit name len instead of len_, probably when injected len as value.
//
#define TRICE_N( id, pFmt, buf, n) do { \
    uint32_t limit = TRICE_SINGLE_MAX_SIZE-TRICE_PREFIX_SIZE-8; /* 8 = head + len size */ \
    uint32_t len_ = n; /* n could be a constant */ \
    if( len_ > limit ){ \
        TRICE32( Id(61732), "wrn:Transmit buffer truncated from %u to %u\n", limit, len_ ); \
        len_ = limit; \
    } \
    TRICE_INTO \
    TRICE_PUT( id | (0xff00 & ((len_+7)<<6)) | TRICE_CYCLE ); /* +3 for padding, +4 for the buf size value transmitted in the payload to get the last 2 bits. */ \
    TRICE_PUT( len_ ); /* len as byte does not contain the exact buf len anymore, so transmit it to the host */ \
    /* len is needed for non string buffers because the last 2 bits not stored in head. */ \
    /* All trices know the data length but not TRICE8P. len byte values 0xFC, xFD, xFE, xFF are reserved for future extensions. */ \
    TRICE_PUTBUFFER( buf, len_ ); \
    TRICE_LEAVE \
} while(0)
#endif // #ifndef TRICE_N

#ifndef TRICE_S
//! TRICE_S writes id and dynString.
//! \param id trice identifier
//! \param pFmt formatstring for trice (ignored here but used by the trice tool)
//! \param dynString 0-terminated runtime generated string
#define TRICE_S( id, pFmt, dynString) do { \
    uint32_t ssiz = strlen( dynString ); \
    TRICE_N( id, pFmt, dynString, ssiz ); \
} while(0)
#endif // #ifndef TRICE_S

#define Id(n) ((uint32_t)n<<16) //!< Id() is a 16 bit id 0-65535 as upper 2 bytes in head

//! TRICE0 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define TRICE0( id, pFmt ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0000 | TRICE_CYCLE ); \
    TRICE_LEAVE

#define TRICE_BYTE0(v)((uint8_t)(v))
#define TRICE_BYTE1(v)(0x0000FF00 &  ((uint32_t)(v)<< 8))
#define TRICE_BYTE2(v)(0x00FF0000 &  ((uint32_t)(v)<<16))
#define TRICE_BYTE3(v)((uint32_t)(v)<<24) 

//! TRICE8_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define TRICE8_1( id, pFmt, v0 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE ); \
    TRICE_PUT(                                                   TRICE_BYTE0(v0)); /* little endian*/ \
    TRICE_LEAVE

//! TRICE8_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 8 bit bit values
#define TRICE8_2( id, pFmt, v0, v1 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE ); \
    TRICE_PUT(                                  TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_LEAVE

//! TRICE8_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 8 bit bit values
#define TRICE8_3( id, pFmt, v0, v1, v2 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE ); \
    TRICE_PUT(                  TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_LEAVE

//! TRICE8_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 8 bit bit values
#define TRICE8_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE ); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_LEAVE

//! TRICE8_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 8 bit bit values
#define TRICE8_5( id, pFmt, v0, v1, v2, v3, v4 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT(                                                    TRICE_BYTE0(v4)); \
    TRICE_LEAVE

//! TRICE8_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 8 bit bit values
#define TRICE8_6( id, pFmt, v0, v1, v2, v3, v4, v5 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT(                                   TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 8 bit bit values
#define TRICE8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT(                  TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_9( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_PUT(                                                    TRICE_BYTE0(v8)); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_10( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_PUT(                                   TRICE_BYTE1(v9) |TRICE_BYTE0(v8)); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_11( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_PUT(                  TRICE_BYTE2(v10)|TRICE_BYTE1(v9) |TRICE_BYTE0(v8)); \
    TRICE_LEAVE

//! TRICE8_12 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 8 bit bit values
#define TRICE8_12( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_PUT( TRICE_BYTE3(v11)|TRICE_BYTE2(v10)|TRICE_BYTE1(v9) |TRICE_BYTE0(v8)); \
    TRICE_LEAVE

//! TRICE16_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define TRICE16_1( id, pFmt, v0 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE ); \
    TRICE_PUT( (uint16_t)(v0) ); \
    TRICE_LEAVE

//! TRICE16_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 16 bit values
#define TRICE16_2( id, pFmt, v0, v1 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_LEAVE

//! TRICE16_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 16 bit values
#define TRICE16_3( id, pFmt, v0, v1, v2 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT( (uint16_t)(v2) ); \
    TRICE_LEAVE

//! TRICE16_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 16 bit values
#define TRICE16_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_LEAVE

//! TRICE16_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 16 bit values
#define TRICE16_5( id, pFmt, v0, v1, v2, v3, v4 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_PUT((uint16_t)(v4) ); \
    TRICE_LEAVE

//! TRICE16_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 16 bit values
#define TRICE16_6( id, pFmt, v0, v1, v2, v3, v4, v5 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_PUT((uint16_t)(v4) | ((uint32_t)(v5)<<16) ); \
    TRICE_LEAVE

//! TRICE16_7 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 16 bit values
#define TRICE16_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
    TRICE_PUT( id | 0x0100 | TRICE_CYCLE); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_LEAVE

//! TRICE32_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 32 bit values
#define TRICE32_2( id, pFmt, v0, v1 ) \
    TRICE_INTO \
    TRICE_PUT(id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_LEAVE

//! TRICE32_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 32 bit values
#define TRICE32_3( id, pFmt, v0, v1, v2 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0300 | TRICE_CYCLE ); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_LEAVE

//! TRICE32_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 32 bit values
#define TRICE32_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
    TRICE_PUT( id | 0x0200 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_LEAVE

//! TRICE64_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 64 bit values
#define TRICE64_2( id, pFmt, v0, v1 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0400 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_LEAVE

//! TRICE64_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 64 bit values
#define TRICE64_3( id, pFmt, v0, v1, v2 ) \
    TRICE_INTO \
    TRICE_PUT( id | 0x0600 | TRICE_CYCLE ); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_LEAVE


//! TRICE64_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 64 bit values
#define TRICE64_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
    TRICE_INTO \
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
