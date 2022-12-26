/*! \file trice.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_H_
#define TRICE_H_

#ifdef __cplusplus
extern "C" {
#endif

//lint -emacro( 123, TRICE* )
//lint -emacro( 571, TRICE* )
//lint -emacro( 572, TRICE* )
//lint -emacro( 578, TRICE* )
//lint -emacro( 648, TRICE* )
//lint -emacro( 665, TRICE* )
//lint -emacro( 666, TRICE* )
//lint -emacro( 670, TRICE* )
//lint -emacro( 694, TRICE* )
//lint -emacro( 717, TRICE* )
//lint -emacro( 718, TRICE* )
//lint -emacro( 732, TRICE* )
//lint -emacro( 734, TRICE* )
//lint -emacro( 736, TRICE* )
//lint -emacro( 740, TRICE* )
//lint -emacro( 746, TRICE* )
//lint -emacro( 747, TRICE* )
//lint -emacro( 774, TRICE* )
//lint -emacro( 778, TRICE* )
//lint -emacro( 826, TRICE* )
//lint -emacro( 830, TRICE* )
//lint -emacro( 831, TRICE* )
//lint -emacro( 835, TRICE* )

//lint -emacro( 545, SCOPY, DCOPY )
//lint -emacro( 732, SCOPY )
//lint -emacro( 717, DCOPY, SCOPY )
//lint -emacro( 732, DCOPY )

//lint -ecall( 666, aFloat, aDouble )
//lint -efunc( 666, aFloat, aDouble )
//lint -esym( 666, aFloat, aDouble )


#ifdef TRICE_OFF // do not generate trice code for files defining TRICE_OFF before including "trice.h"
#define TRICE_CYCLE_COUNTER 0 // todo: why needed here?
#define TRICE_INTO
#define TRICE_PUT(n) do{ ((void)(n)); }while(0)
#define PUT_BUFFER(b,l) do{ ((void)(b)); ((void)(l)); }while(0)
#define TRICE_LEAVE
#define TRICE_S( id, p, s )  do{ ((void)(id)); ((void)(p)); ((void)(s)); }while(0)
#define TRICE_N( id, p, s, n )  do{ ((void)(id)); ((void)(p)); ((void)(s)); ((void)(n)); }while(0)
#endif

///////////////////////////////////////////////////////////////////////////////
// helper macros
//

//! TRICE_SAFE_SINGLE_MODE is the recommended TRICE_TRANSFER_MODE. It packs each trice in a separate TCOBS package with a following 0-delimiter byte. 
//! Single trices need a bit more transfer data. In case of a data disruption, only a single trice messages can get lost.
#define TRICE_SAFE_SINGLE_MODE 11

//! TRICE_PACK_MULTI_MODE packs all trices of a buffer in a single TCOBS package and a following 0-delimiter byte. 
//! Grouped trices need a bit less transfer data. In case of a data disruption, multiple trice messages can get lost.
#define TRICE_PACK_MULTI_MODE  22

//! Set TRICE_MODE to TRICE_STACK_BUFFER if the stack is used for singe trices. 
//! The TRICE macro not usable inside interrupts with this setting.
#define	TRICE_STACK_BUFFER    111

//! Set TRICE_MODE to TRICE_DOUBLE_BUFFER for fastest trices. 
//! The TRICE macro is usable inside interrupts with this setting.
#define	TRICE_DOUBLE_BUFFER   222

//! Set TRICE_MODE to TRICE_STREAM_BUFFER for fast trices and several output options
//! The TRICE macro is usable inside interrupts with this setting.
#define TRICE_STREAM_BUFFER   333

#define TRICE_FRAMING_TCOBS  9999
#define TRICE_FRAMING_COBS   8888
#define TRICE_FRAMING_NONE   7777

//
///////////////////////////////////////////////////////////////////////////////

//! TriceWriteDevice is a symbolic address to a trice data output device.
typedef enum{
    UartA,
    UartB,
    Rtt0,
    Cgo
} TriceWriteDevice_t;

#include "triceConfig.h"
#include <stdint.h> //lint !e537
#include <string.h>

#ifdef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN
// Swap a 16-bit integer (https://www.oryx-embedded.com/doc/cpu__endian_8h_source.html)
#define SWAPINT16(x) ( \
    (((uint16_t)(x) & 0x00FFU) << 8) | \
    (((uint16_t)(x) & 0xFF00U) >> 8))
  
//Swap a 32-bit integer (https://www.oryx-embedded.com/doc/cpu__endian_8h_source.html)
#define SWAPINT32(x) ( \
    (((uint32_t)(x) & 0x000000FFUL) << 24) | \
    (((uint32_t)(x) & 0x0000FF00UL) << 8) | \
    (((uint32_t)(x) & 0x00FF0000UL) >> 8) | \
    (((uint32_t)(x) & 0xFF000000UL) >> 24))

    //! TRICE_HTOTS reorders short values from host order into trice transfer order. 
    #define TRICE_HTOTS(x) SWAPINT16(x)

    //! TRICE_HTOTL reorders long values from host order x into trice transfer order. 
    #define TRICE_HTOTL(x) SWAPINT32(x)

    //! TRICE_TTOHS reorders short values from trice transfer order into host order. 
    #define TRICE_TTOHS(x) SWAPINT16(x)
#else // #ifdef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN 
    //! TRICE_HTOTS reorders short values from hos // t order into trice transfer order.
    #define TRICE_HTOTS(x) (x)

    //! TRICE_HTOTL reorders long values from host order x into trice transfer order. 
    #define TRICE_HTOTL(x) (x)

    //! TRICE_TTOHS reorders short values from trice transfer order into host order.  
    #define TRICE_TTOHS(x) (x)
#endif // #else // #ifdef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN

void TriceWriteDevice( TriceWriteDevice_t device, uint8_t *buf, size_t len );

//! TriceStamp16 returns a 16-bit value to stamp `Id` TRICE macros. Usually it is a timestamp, but could also be a destination address or a counter for example.
//! This function is user provided.
uint16_t TriceStamp16( void );

//! TriceStamp32 returns a 32-bit value to stamp `ID` TRICE macros. Usually it is a timestamp, but could also be a destination address or a counter for example.
//! This function is user provided.
uint32_t TriceStamp32( void );

#if TRICE_MODE == TRICE_STACK_BUFFER

#define TRICE_STACK_BUFFER_MAX_SIZE TRICE_BUFFER_SIZE 

//! Start of TRICE macro
#define TRICE_ENTER { \
    uint32_t co[TRICE_STACK_BUFFER_MAX_SIZE>>2]; /* Check TriceDepthMax at runtime. */ \
    uint32_t* TriceBufferWritePosition = co + (TRICE_DATA_OFFSET>>2);

//! End of TRICE macro
#define TRICE_LEAVE { \
    unsigned tLen = ((TriceBufferWritePosition - co)<<2) - TRICE_DATA_OFFSET; \
    TriceOut( co, tLen ); } }

#endif // #if TRICE_MODE == TRICE_STACK_BUFFER

#if TRICE_MODE == TRICE_DOUBLE_BUFFER

//! TRICE_HALF_BUFFER_SIZE is the size of each of both buffers. Must be able to hold the max TRICE burst count within TRICE_TRANSFER_INTERVAL_MS or even more,
//! if the write out speed is small. Must not exceed SEGGER BUFFER_SIZE_UP
#define TRICE_HALF_BUFFER_SIZE  (((TRICE_BUFFER_SIZE/2) + 3) & ~3)

//! TRICE_ENTER is the start of TRICE macro. The TRICE macros are a bit slower. Inside interrupts TRICE macros allowed.
#define TRICE_ENTER TRICE_ENTER_CRITICAL_SECTION 

//! TRICE_LEAVE is the end of TRICE macro.
#define TRICE_LEAVE TRICE_LEAVE_CRITICAL_SECTION

#endif // #if TRICE_MODE == TRICE_DOUBLE_BUFFER

#if TRICE_MODE == TRICE_STREAM_BUFFER

//! TRICE_STREAM_BUFFER_SIZE is the total size of the stream buffer. Must be able to hold the max TRICE burst count or even more,
//! if the write out speed is small.
#define TRICE_STREAM_BUFFER_SIZE  (TRICE_BUFFER_SIZE)

//! TRICE_ENTER is the start of TRICE macro. The TRICE macros are a bit slower. Inside interrupts TRICE macros allowed.
#define TRICE_ENTER TRICE_ENTER_CRITICAL_SECTION { \
    uint32_t* ta = TriceBufferWritePosition; /* The last position points to free space.*/ \
    TriceBufferWritePosition += (TRICE_DATA_OFFSET>>2); /* add offset */ 

//! TRICE_LEAVE is the end of the TRICE macro.
//! The triceFifo stores the write position before and after the TRICE macro execution.
//! This is needed to know the trice size later in a general way. In the future several trice fifos possible. 
#define TRICE_LEAVE \
    TriceFifoPush( ta ); /* Store start of trice including TRICE_DATA_OFFSET. */ \
    TriceFifoPush( TriceBufferWritePosition ); /* Store current limit for size detection in case of extended trices. */ \
    TriceBufferWritePosition = TriceNextStreamBuffer(); /* Wrap stream buffer if needed. */ \
    } TRICE_LEAVE_CRITICAL_SECTION

#endif // #if TRICE_MODE == TRICE_STREAM_BUFFER

//! TRICE_DEFERRED_OUT is a helper macro.
#define TRICE_DEFERRED_OUT ((TRICE_MODE == TRICE_DOUBLE_BUFFER) || (TRICE_MODE == TRICE_STREAM_BUFFER) )

///////////////////////////////////////////////////////////////////////////////
// Declarations and Defaults

#ifndef TRICE_COMMAND_SIZE_MAX
#define TRICE_COMMAND_SIZE_MAX 120 //!< trice tool could transmit command strings to target
#endif
extern char* const triceCommandBuffer;
extern int triceCommandFlag;
extern int triceCommandLength;

size_t TriceDepth( void );
size_t TriceDepthMax( void );
#if TRICE_DEFERRED_OUT
extern uint32_t* TriceBufferWritePosition;
#endif

size_t triceNonBlockingWriteUartA( void const * buf, size_t nByte );
size_t triceNonBlockingWriteUartB( void const * buf, size_t nByte );

#if TRICE_MODE == TRICE_STREAM_BUFFER
void TriceFifoPush( void* ta );
uint32_t* TriceNextStreamBuffer( void );
#endif

extern unsigned triceDepthMax;
extern size_t triceFifoDepthMax;
extern size_t triceStreamBufferDepthMax;

void TriceOut( uint32_t* tb, size_t tLen );
void TriceLogBufferInfo( void );

void TriceTransfer( void );
void TriceCheckSet( int index ); //!< tests

#ifdef TRICE_RTT0
#include "SEGGER_RTT.h"
#if defined(TRICE_HALF_BUFFER_SIZE) && TRICE_HALF_BUFFER_SIZE > BUFFER_SIZE_UP
#error
#endif
#if defined(TRICE_STACK_BUFFER_SIZE) && TRICE_STACK_BUFFER_SIZE > BUFFER_SIZE_UP
#error
#endif
#endif // #ifdef TRICE_RTT0

//! TRICE_DATA_OFFSET is the space in front of trice data for in-buffer (T)COBS encoding. It must be be a multiple of uint32_t.
#ifndef TRICE_DATA_OFFSET
#if defined(TRICE_HALF_BUFFER_SIZE)
#define TRICE_DATA_OFFSET (((TRICE_HALF_BUFFER_SIZE/3)+4)&~3) // In worst case the buffer gets filled to the end only with 4-byte trices and each gets an additional sigil and a 0, so 33% are safe.
#elif defined(TRICE_STACK_BUFFER_MAX_SIZE)
#define TRICE_DATA_OFFSET ((TRICE_STACK_BUFFER_MAX_SIZE/31+5)&~3) // For single trices the worst case is +1 for each 31 plus terminating 0 at the end
#else
#define TRICE_DATA_OFFSET ((TRICE_SINGLE_MAX_SIZE/31+5)&~3) // For single trices the worst case is +1 for each 31 plus terminating 0 at the end. Must be a multiple of 4.
#endif
#endif // #ifndef TRICE_DATA_OFFSET

#if TRICE_CYCLE_COUNTER == 1
extern uint8_t TriceCycle;
#define TRICE_CYCLE TriceCycle++ //! TRICE_CYCLE is the trice cycle counter as 8 bit count 0-255.
#else
#define TRICE_CYCLE 0xC0 //! TRICE_CYCLE is no trice cycle counter, just a static value.
#endif

//
///////////////////////////////////////////////////////////////////////////////

#ifndef TRICE_PUT
#define TRICE_PUT(x) do{ *TriceBufferWritePosition++ = TRICE_HTOTL(x); }while(0) //! PUT copies a 32 bit x into the TRICE buffer.
#endif

#if ( defined( TRICE_MCU_IS_BIG_ENDIAN ) &&  defined( TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN ) ) \
  ||(!defined( TRICE_MCU_IS_BIG_ENDIAN ) && !defined( TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN ) )
    #define TRICE_PUT64(x) TRICE_PUT( (uint32_t)(x) ); TRICE_PUT( (uint32_t)((uint64_t)(x)>>32) ); // little endian
#else
    #define TRICE_PUT64(x) TRICE_PUT( (uint32_t)((uint64_t)(x)>>32) ); TRICE_PUT( (uint32_t)(x) ); // big endian
#endif

#ifndef TRICE_PUTBUFFER
//! TRICE_PUTBUFFER copies a buffer into the TRICE buffer.
#define TRICE_PUTBUFFER( buf, len ) do{ memcpy( TriceBufferWritePosition, buf, len ); TriceBufferWritePosition += (len+3)>>2; }while(0)
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

#if defined( TRICE_UARTA ) // && TRICE_DEFERRED_OUT // buffered out to UARTA
void TriceBlockingWriteUartA( uint8_t const * buf, unsigned len );
uint8_t TriceNextUint8UartA( void );
void triceServeTransmitUartA(void);
void triceTriggerTransmitUartA(void);
unsigned TriceOutDepthUartA( void );
#endif

#if defined( TRICE_UARTB ) // && TRICE_DEFERRED_OUT // buffered out to UARTB
void TriceBlockingWriteUartB( uint8_t const * buf, unsigned len );
uint8_t TriceNextUint8UartB( void );
void triceServeTransmitUartB(void);
void triceTriggerTransmitUartB(void);
unsigned TriceOutDepthUartB( void );
#endif

unsigned TriceOutDepth( void );
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Encryption
//
#ifdef XTEA_ENCRYPT_KEY
void XTEAEncrypt( uint32_t* p, unsigned count );
void XTEADecrypt( uint32_t* p, unsigned count );
void XTEAInitTable(void);
#endif // #ifdef XTEA_ENCRYPT_KEY
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
//! If you set the C language to strict C (C90, C99, C11 or C17) the `##` operator doesn't remove the comma before it when `__VA_ARGS__` expands to nothing.
//! In this case, the TRICE macro doesn't work with no parameters. You must explicitly use TRICE0 instead of TRICE for a no parameter value TRICE in this case.
//! For more details see closed Issue #279. Special thanks @escherstair.
//! If for example using CLANG 6.18 set C-language to gnu11, gnu99 or std to avoid the comma issue when no parameters are in a TRICE  macro.
//! In case you have to set the C-Language to c11 or c99 you can use the TRICE0 macro directly instead of TRICE when no value parameters.
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

#ifndef TRICE8_B
//!TRICE_B expects inside pFmt only one format specifier, which is used n times by using pFmt n times.
#define TRICE8_B( id, pFmt, buf, n) do { \
    TRICE_N( id, pFmt, buf, n); \
} while(0)
#endif // #ifndef TRICE_8B

#ifndef TRICE16_B
//! TRICE16_B expects inside pFmt only one format specifier, which is used n times by using pFmt n times.
#define TRICE16_B( id, pFmt, buf, n) do { \
    TRICE_N( id, pFmt, buf, 2*n); \
} while(0)
#endif // #ifndef TRICE_16B

#ifndef TRICE32_B
//! TRICE32_B expects inside pFmt only one format specifier, which is used n times by using pFmt n times.
#define TRICE32_B( id, pFmt, buf, n) do { \
    TRICE_N( id, pFmt, buf, 4*n); \
} while(0)
#endif // #ifndef TRICE_32B

#ifndef TRICE64_B
//! TRICE64_B expects inside pFmt only one format specifier, which is used n times by using pFmt n times.
#define TRICE64_B( id, pFmt, buf, n) do { \
    TRICE_N( id, pFmt, buf, 8*n); \
} while(0)
#endif // #ifndef TRICE_64B

#define TRICE8_F  TRICE8_B 
#define TRICE16_F TRICE16_B 
#define TRICE32_F TRICE32_B 
#define TRICE64_F TRICE64_B 

#define TRICE_B  TRICE8_B 
#define TRICE_F  TRICE8_B

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
#define TRICE_N( tid, pFmt, buf, n) do { \
    uint32_t limit = TRICE_SINGLE_MAX_SIZE-8; /* 8 = head + max timestamp size --> todo: consider 64-bit stamp! */ \
    uint32_t len_ = n; /* n could be a constant */ \
    if( len_ > limit ){ \
        TRICE32( id( 2240), "wrn:Transmit buffer truncated from %u to %u\n", len_, limit ); \
        len_ = limit; \
    } \
    TRICE_ENTER tid; \
    if( len_ <= 127 ){ CNTC(len_); }else{ LCNT(len_); } \
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

#ifndef TRICE_PUT16
//! TRICE_PUT16 copies a 16 bit x into the TRICE buffer.
#define TRICE_PUT16(x) do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = TRICE_HTOTS(x); TriceBufferWritePosition = (uint32_t*)p; }while(0)
#endif

extern const int TriceTypeS0;
extern const int TriceTypeS2;
extern const int TriceTypeS4;
extern const int TriceTypeX0;

#ifdef TRICE_MCU_IS_BIG_ENDIAN
//! TRICE_PUT1616 writes a 32-bit value in 2 16-bit steps to avoid memory alignment hard fault.
#define TRICE_PUT1616( ts ) TRICE_PUT16( ts >> 16 ); TRICE_PUT16( ts ); 
#else
//! TRICE_PUT1616 writes a 32-bit value in 2 16-bit steps to avoid memory alignment hard fault.
#define TRICE_PUT1616( ts ) TRICE_PUT16( ts );       TRICE_PUT16( ((ts) >> 16) ); 
#endif

//! ID writes 14-bit id with 11 as 2 most significant bits, followed by a 32-bit stamp.
//! 11iiiiiiI TT | TT (NC) | ...
//! C000 = 1100 0000 0000 0000
#define ID(n) { uint32_t ts = TriceStamp32(); TRICE_PUT16( (0xC000|(n))); TRICE_PUT1616(ts); }

//! Id writes 14-bit id with 10 as 2 most significant bits two times, followed by a 16-bit stamp.
//! 10iiiiiiI 10iiiiiiI | TT (NC) | ...
//! 8000 = 1000 0000 0000 0000
#define Id(n) { uint16_t ts = TriceStamp16(); TRICE_PUT((0x80008000|((n)<<16)|(n))); TRICE_PUT16(ts); }

//! id writes 14-bit id with 01 as 2 most significant bits, followed by no stamp.
//! 01iiiiiiI (NC) | ...
//! 4000 = 0100 0000 0000 0000
#define id(n) TRICE_PUT16( 0x4000|(n));

//! CNTC writes 7-bit byte count and 8-bit cycle counter.
#define CNTC(count) do{ uint16_t v = ((count)<<8) | TRICE_CYCLE; TRICE_PUT16( v ); }while(0)

#if TRICE_CYCLE_COUNTER == 1
//! LCNT writes 1 as most significant bit and 15-bit byte count. It does not write the cycle counter but increments the cycle counter.
#define LCNT(count) TRICE_PUT16( (0x8000 | (count)) ); TRICE_CYCLE  // increment TRICE_CYCLE but do not transmit it
#else
//! LCNT writes 1 as most significant bit and 15-bit byte count. It does not write the cycle counter but increments the cycle counter.
#define LCNT(count) TRICE_PUT16( (0x8000 | (count)) );  // no TRICE_CYCLE
#endif

//! TRICE0 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define TRICE0( id, pFmt ) \
	TRICE_ENTER id; CNTC(0); \
    TRICE_LEAVE

#ifdef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN

#define TRICE_BYTE3(v)((uint8_t)(v))
#define TRICE_BYTE2(v)(0x0000FF00 &  ((uint32_t)(v)<< 8))
#define TRICE_BYTE1(v)(0x00FF0000 &  ((uint32_t)(v)<<16))
#define TRICE_BYTE0(v)((uint32_t)(v)<<24)

#define TRICE_SHORT1( v ) (uint16_t)(v)
#define TRICE_SHORT0( v ) ((uint32_t)(v)<<16) 

#else // #ifdef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN

#define TRICE_BYTE0(v)((uint8_t)(v))
#define TRICE_BYTE1(v)(0x0000FF00 &  ((uint32_t)(v)<< 8))
#define TRICE_BYTE2(v)(0x00FF0000 &  ((uint32_t)(v)<<16))
#define TRICE_BYTE3(v)((uint32_t)(v)<<24)

#define TRICE_SHORT0( v ) (uint16_t)(v)
#define TRICE_SHORT1( v ) ((uint32_t)(v)<<16) 


#endif // #else // #ifdef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN


//! TRICE8_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define TRICE8_1( tid, pFmt, v0 ) \
    TRICE_ENTER tid; CNTC(1); \
    TRICE_PUT(                                                   TRICE_BYTE0(v0)); /* little endian*/ \
    TRICE_LEAVE

//! TRICE8_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 8 bit bit values
#define TRICE8_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER id; CNTC(2); \
    TRICE_PUT(                                  TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_LEAVE

//! TRICE8_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 8 bit bit values
#define TRICE8_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER id; CNTC(3); \
    TRICE_PUT(                  TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_LEAVE

//! TRICE8_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 8 bit bit values
#define TRICE8_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER id; CNTC(4); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_LEAVE

//! TRICE8_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 8 bit bit values
#define TRICE8_5( id, pFmt, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER id; CNTC(5); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT(                                                    TRICE_BYTE0(v4)); \
    TRICE_LEAVE

//! TRICE8_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 8 bit bit values
#define TRICE8_6( id, pFmt, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER id; CNTC(6); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT(                                   TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 8 bit bit values
#define TRICE8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER id; CNTC(7); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT(                  TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER id; CNTC(8); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_9( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER id; CNTC(9); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_PUT(                                                    TRICE_BYTE0(v8)); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_10( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER id; CNTC(10); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_PUT(                                   TRICE_BYTE1(v9) |TRICE_BYTE0(v8)); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_11( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER id; CNTC(11); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_PUT(                  TRICE_BYTE2(v10)|TRICE_BYTE1(v9) |TRICE_BYTE0(v8)); \
    TRICE_LEAVE

//! TRICE8_12 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 8 bit bit values
#define TRICE8_12( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER id; CNTC(12); \
    TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
    TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
    TRICE_PUT( TRICE_BYTE3(v11)|TRICE_BYTE2(v10)|TRICE_BYTE1(v9) |TRICE_BYTE0(v8)); \
    TRICE_LEAVE

//! TRICE16_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 16 bit value
#define TRICE16_1( id, pFmt, v0 ) \
    TRICE_ENTER id; CNTC(2); \
    TRICE_PUT(  TRICE_SHORT0( v0 ) ); \
    TRICE_LEAVE

//! TRICE16_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 16 bit values
#define TRICE16_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER id; CNTC(4); \
    TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
    TRICE_LEAVE

//! TRICE16_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 16 bit values
#define TRICE16_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER id; CNTC(6); \
    TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
    TRICE_PUT(TRICE_SHORT0(v2) ); \
    TRICE_LEAVE

//! TRICE16_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 16 bit values
#define TRICE16_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER id; CNTC(8); \
    TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
    TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
    TRICE_LEAVE

//! TRICE16_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 16 bit values
#define TRICE16_5( id, pFmt, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER id; CNTC(10); \
    TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
    TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
    TRICE_PUT(TRICE_SHORT0(v4) ); \
    TRICE_LEAVE

//! TRICE16_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 16 bit values
#define TRICE16_6( id, pFmt, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER id; CNTC(12); \
    TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
    TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
    TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
    TRICE_LEAVE

//! TRICE16_7 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 16 bit values
#define TRICE16_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER id; CNTC(14); \
    TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
    TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
    TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
    TRICE_PUT(TRICE_SHORT0(v6) ); \
    TRICE_LEAVE

//! TRICE16_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 16 bit values
#define TRICE16_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER id; CNTC(16); \
    TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
    TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
    TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
    TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7) ); \
    TRICE_LEAVE
    
//! TRICE16_9 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v8 are 16 bit values
#define TRICE16_9( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER id; CNTC(18); \
    TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
    TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
    TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
    TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7) ); \
    TRICE_PUT(TRICE_SHORT0(v8) ); \
    TRICE_LEAVE

//! TRICE16_10 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v9 are 16 bit values
#define TRICE16_10( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER id; CNTC(20); \
    TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
    TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
    TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
    TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7) ); \
    TRICE_PUT(TRICE_SHORT0(v8) | TRICE_SHORT1(v9) ); \
    TRICE_LEAVE
    
//! TRICE16_11 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v10 are 16 bit values
#define TRICE16_11( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER id; CNTC(22); \
    TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
    TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
    TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
    TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7) ); \
    TRICE_PUT(TRICE_SHORT0(v8) | TRICE_SHORT1(v9) ); \
    TRICE_PUT(TRICE_SHORT0(v10)); \
    TRICE_LEAVE
    
//! TRICE16_12 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 16 bit values
#define TRICE16_12( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER id; CNTC(24); \
    TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
    TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
    TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
    TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7) ); \
    TRICE_PUT(TRICE_SHORT0(v8) | TRICE_SHORT1(v9) ); \
    TRICE_PUT(TRICE_SHORT0(v10)| TRICE_SHORT1(v11)); \
    TRICE_LEAVE

//! TRICE32_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 the 32 bit value
#define TRICE32_1( id, pFmt, v0 ) \
    TRICE_ENTER id; CNTC(4); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_LEAVE

//! TRICE32_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 32 bit values
#define TRICE32_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER id; CNTC(8); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_LEAVE

//! TRICE32_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 32 bit values
#define TRICE32_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER id; CNTC(12); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_LEAVE

//! TRICE32_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 32 bit values
#define TRICE32_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER id; CNTC(16); \
    TRICE_PUT( (uint32_t)(v0) ); \
    TRICE_PUT( (uint32_t)(v1) ); \
    TRICE_PUT( (uint32_t)(v2) ); \
    TRICE_PUT( (uint32_t)(v3) ); \
    TRICE_LEAVE

//! TRICE32_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 32 bit values
#define TRICE32_5( id, pFmt,  v0, v1, v2, v3, v4 ) \
    TRICE_ENTER id; CNTC(20); \
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
    TRICE_ENTER id; CNTC(24); \
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
    TRICE_ENTER id; CNTC(28); \
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
    TRICE_ENTER id; CNTC(32); \
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
    TRICE_ENTER id; CNTC(36); \
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
    TRICE_ENTER id; CNTC(40); \
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
    TRICE_ENTER id; CNTC(44); \
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
    TRICE_ENTER id; CNTC(48); \
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
    TRICE_ENTER id; CNTC(8); \
    TRICE_PUT64( v0 ); \
    TRICE_LEAVE

//! TRICE64_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 64 bit values
#define TRICE64_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER id; CNTC(16); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_LEAVE

//! TRICE64_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 64 bit values
#define TRICE64_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER id; CNTC(24); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_LEAVE


//! TRICE64_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 64 bit values
#define TRICE64_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER id; CNTC(32); \
    TRICE_PUT64( v0 ); \
    TRICE_PUT64( v1 ); \
    TRICE_PUT64( v2 ); \
    TRICE_PUT64( v3 ); \
    TRICE_LEAVE

//! TRICE64_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 64 bit values
#define TRICE64_5( id, pFmt,  v0, v1, v2, v3, v4 ) \
    TRICE_ENTER id; CNTC(40); \
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
    TRICE_ENTER id; CNTC(48); \
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
    TRICE_ENTER id; CNTC(56); \
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
    TRICE_ENTER id; CNTC(64); \
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
    TRICE_ENTER id; CNTC(72); \
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
    TRICE_ENTER id; CNTC(80); \
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
    TRICE_ENTER id; CNTC(88); \
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
    TRICE_ENTER id; CNTC(96); \
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
