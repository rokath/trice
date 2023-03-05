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

//! Variadic macros (https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms)
//! See for more explanation https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/preprocessor/macros/__VA_ARGS__/count-arguments
//! This is extendable until a 32767 bytes payload.
#define TRICE_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME

#include <stdint.h> //lint !e537
#include <string.h>
#include "triceConfig.h"
#include "./box/trice8.h"
#include "./box/trice16.h"
#include "./box/trice32.h"
#include "./box/trice64.h"

unsigned TriceOutDepthCGO( void ); // only needed for testing C-sources from Go

//! TRICE_DEFERRED_OUT is a helper macro.
#define TRICE_DEFERRED_OUT ((TRICE_MODE == TRICE_DOUBLE_BUFFER) || (TRICE_MODE == TRICE_STREAM_BUFFER) )

#ifdef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN

// Swap a 16-bit integer (https://www.oryx-embedded.com/doc/cpu__endian_8h_source.html)
#define TRICE_SWAPINT16(x) ( \
    (((uint16_t)(x) & 0x00FFU) << 8) | \
    (((uint16_t)(x) & 0xFF00U) >> 8))
  
//Swap a 32-bit integer (https://www.oryx-embedded.com/doc/cpu__endian_8h_source.html)
#define TRICE_SWAPINT32(x) ( \
    (((uint32_t)(x) & 0x000000FFUL) << 24) | \
    (((uint32_t)(x) & 0x0000FF00UL) << 8) | \
    (((uint32_t)(x) & 0x00FF0000UL) >> 8) | \
    (((uint32_t)(x) & 0xFF000000UL) >> 24))

    //! TRICE_HTOTS reorders short values from host order into trice transfer order. 
    #define TRICE_HTOTS(x) TRICE_SWAPINT16(x)

    //! TRICE_HTOTL reorders long values from host order x into trice transfer order. 
    #define TRICE_HTOTL(x) TRICE_SWAPINT32(x)

    //! TRICE_TTOHS reorders short values from trice transfer order into host order. 
    #define TRICE_TTOHS(x) TRICE_SWAPINT16(x)

#else // #ifdef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN

    //! TRICE_HTOTS reorders short values from hos // t order into trice transfer order.
    #define TRICE_HTOTS(x) (x)

    //! TRICE_HTOTL reorders long values from host order x into trice transfer order. 
    #define TRICE_HTOTL(x) (x)

    //! TRICE_TTOHS reorders short values from trice transfer order into host order.  
    #define TRICE_TTOHS(x) (x)

#endif // #else // #ifdef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN

//! TriceStamp16 returns a 16-bit value to stamp `Id` TRICE macros. Usually it is a timestamp, but could also be a destination address or a counter for example.
//! This function is user provided.
uint16_t TriceStamp16( void );

//! TriceStamp32 returns a 32-bit value to stamp `ID` TRICE macros. Usually it is a timestamp, but could also be a destination address or a counter for example.
//! This function is user provided.
uint32_t TriceStamp32( void );

#if TRICE_MODE == TRICE_STACK_BUFFER 

#define TRICE_STACK_BUFFER_MAX_SIZE TRICE_BUFFER_SIZE

//! Start of TRICE macro
#define TRICE_ENTER { /*TRICE_MODE == TRICE_STACK_BUFFER*/ \
    uint32_t co[TRICE_STACK_BUFFER_MAX_SIZE>>2]; /* Check TriceDepthMax at runtime. */ \
    uint32_t* TriceBufferWritePosition = co + (TRICE_DATA_OFFSET>>2);

//! End of TRICE macro
#define TRICE_LEAVE { /* TRICE_MODE == TRICE_STACK_BUFFER*/ \
    unsigned tLen = ((TriceBufferWritePosition - co)<<2) - TRICE_DATA_OFFSET; \
    TriceOut( co, tLen ); } }

#endif // #if TRICE_MODE == TRICE_STACK_BUFFER

#if TRICE_MODE == TRICE_DOUBLE_BUFFER
//! TRICE_HALF_BUFFER_SIZE is the size of each of both buffers. Must be able to hold the max TRICE burst count within TRICE_TRANSFER_INTERVAL_MS or even more,
//! if the write out speed is small. Must not exceed SEGGER BUFFER_SIZE_UP
#define TRICE_HALF_BUFFER_SIZE  (((TRICE_BUFFER_SIZE/2) + 3) & ~3)

#ifdef TRICE_RTT0 // special case
// When RTT is configured, we want to get out the trice directly and not deferred.

void TriceOutRtt0( uint32_t* tb, size_t tLen ); // todo

// When TRICE_MODE != TRICE_STACK_BUFFER (deferred mode for UART as example) and TRICE_RTT0 is defined
// we need this value because TRICE_RTT0 is always in direct mode to support post mortem analysis.
#define TRICE_STACK_BUFFER_MAX_SIZE TRICE_BUFFER_SIZE 

//! TRICE_ENTER is the start of TRICE macro. The TRICE macros are a bit slower. Inside interrupts TRICE macros allowed.
#define TRICE_ENTER \
    TRICE_ENTER_CRITICAL_SECTION { \
    uint32_t* TriceBufferWritePositionRtt0 = TriceBufferWritePosition;

//! TRICE_LEAVE is the end of TRICE macro.
//! Here we use a combination of direct buffer out for RTT and deferred buffer out for UART.
//! The TRICE_PUT macros use the pointer TriceBufferWritePosition and the trice data are one after
//! the other inside of the double buffer which has a TRICE_DATA_OFFSET space at the beginning.
//! For direct out we need this space in front of each trice, therefore we must copy it.
#define TRICE_LEAVE \
    { \
        unsigned tLenRtt0 = ((TriceBufferWritePosition - TriceBufferWritePositionRtt0)<<2); \
        uint32_t co[TRICE_STACK_BUFFER_MAX_SIZE>>2]; \
        memcpy( co + (TRICE_DATA_OFFSET>>2), TriceBufferWritePositionRtt0, tLenRtt0 ); \
        TriceOutRtt0( co, tLenRtt0 ); \
    } \
    } TRICE_LEAVE_CRITICAL_SECTION

#else // #ifdef TRICE_RTT0

//! TRICE_ENTER is the start of TRICE macro. The TRICE macros are a bit slower. Inside interrupts TRICE macros allowed.
#define TRICE_ENTER TRICE_ENTER_CRITICAL_SECTION \

//! TRICE_LEAVE is the end of TRICE macro.
#define TRICE_LEAVE TRICE_LEAVE_CRITICAL_SECTION

#endif // #else // #ifdef TRICE_RTT0

#endif // #if TRICE_MODE == TRICE_DOUBLE_BUFFER

#if TRICE_MODE == TRICE_STREAM_BUFFER // todo: add RTT0 handling here too

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
void TriceDiagnostics( int index );

extern uint32_t* TriceBufferWritePosition;

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
void TriceCheck( int index ); //!< tests

#ifdef TRICE_RTT0
#include "./box/SEGGER_RTT.h"
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
#define TRICE_PUT(x) do{ *TriceBufferWritePosition++ = TRICE_HTOTL(x); }while(0); //! PUT copies a 32 bit x into the TRICE buffer.
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

//! NTH_ARGUMENT just evaluates to the 15th argument. It is extendable.
#define NTH_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, ...) a14 

//! COUNT_ARGUMENTS builds upon NTH_ARGUMENT. The more arguments that are passed to COUNT_ARGUMENTS, 
//! the more the »counting arguments« (12, 11, 10, 9, 8, 7…) are pushed to the right. 
//! Thus the macro evaluates to the number of arguments that are passed to the macro.
//! If you set the C language to strict C (C90, C99, C11 or C17) the `##` operator doesn't remove the comma before it when `__VA_ARGS__` expands to nothing.
//! In this case, the TRICE macro doesn't work with no parameters. You must then explicitly use TRICE0 instead of TRICE for a no parameter value TRICE.
//! For more details see closed Issue #279. Special thanks @escherstair.
//! If for example using CLANG 6.18 set C-language to gnu11, gnu99 or std to avoid the comma issue when no parameters are in a TRICE  macro.
//! In case you have to set the C-Language to c11 or c99 you can use the TRICE0 macro directly instead of TRICE when no value parameters.
#define COUNT_ARGUMENTS(...) NTH_ARGUMENT(dummy, ## __VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

//! CONCAT concatenates the 2 arguments a and b (helper macro).
#define CONCAT(a, b) a ## b 

//! CONCAT2 concatenates the 2 arguments a and b (helper macro).
#define CONCAT2(a, b) CONCAT(a, b)

//! TRICE_VARIABLE_ARGUMENTS concatenates TRICE_ with the result of COUNT_ARGUMENTS to produce something like TRICE_2 which takes a printf-format and two arguments.
#define TRICE( tid, fmt, ...) CONCAT2(TRICE_, COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRice( tid, fmt, ...) CONCAT2(TRice_, COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define Trice( tid, fmt, ...) CONCAT2(Trice_, COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define trice( tid, fmt, ...) CONCAT2(trice_, COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

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
#define TRice_0  TRice0  //!< Only the format string without parameter values.
#define Trice_0  Trice0  //!< Only the format string without parameter values.
#define trice_0  trice0  //!< Only the format string without parameter values.

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
        TRICE32( id( 4325), "wrn:Transmit buffer truncated from %u to %u\n", len_, limit ); \
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

//! iD is just a code parsing helper.
#define iD(n) (n)

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

TRICE_INLINE void TRice0( uint16_t tid, char* pFmt ){
    TRice32m_0( tid ); \
}

TRICE_INLINE void Trice0( uint16_t tid, char* pFmt ){
    Trice32m_0( tid ); \
}

TRICE_INLINE void trice0( uint16_t tid, char* pFmt ){
    trice32m_0( tid ); \
}

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

#if defined(TRICE_LOG_OVER_MODBUS_FUNC24) && (TRICE_MODE != TRICE_STREAM_BUFFER)
#error TRICE_LOG_OVER_MODBUS_FUNC24 works only with TRICE_MODE == TRICE_STREAM_BUFFER.
#endif

void TriceLogDepthMax( void );
void TriceNonBlockingWriteModbusBuffer( uint8_t const * buf, unsigned len );
size_t TriceModbusAlsoFetch( int index, uint8_t* tBuf );
size_t TriceModbusOnlyFetch( int index, uint8_t* tBuf );

extern unsigned RTT0_writeSpaceMin; //! RTT0_writeSpaceMin is usable for diagnostics.
extern unsigned RTT0_bytesInBufferMax; //! RTT0_bytesInBufferMax is usable for diagnostics.

#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
