/*! \file trice.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_H_
#define TRICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#undef ID // acoid name clashes in case ID was used by an other library
#undef Id // acoid name clashes in case Id was used by an other library
#undef id // acoid name clashes in case id was used by an other library
#undef iD // acoid name clashes in case iD was used by an other library


//lint -e529  Warning 529: Symbol '_SEGGER_RTT__LockState' not subsequently referenced
//lint -e629  Warning 629: static class for function '' is non standard
 

//lint -emacro( 701, TRICE* ) Info 701: Shift left of signed quantity (int)

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

#define TRICE_ENTER
#define TRICE_LEAVE
#define TRICE_PUT(n)           // do{ ((void)(n)); }while(0)
#define TRICE_PUT16(n)         // do{ ((void)(n)); }while(0)
#define PUT_BUFFER(b,l)        // do{ ((void)(b)); ((void)(l)); }while(0)
#define TRICE_S( id, p, s )    // do{ ((void)(id)); ((void)(p)); ((void)(s)); }while(0)
#define TRICE_N( id, p, s, n ) // do{ ((void)(id)); ((void)(p)); ((void)(s)); ((void)(n)); }while(0)

#endif // #ifdef TRICE_OFF

// helper macros (the numbers are 32-bit random values)

//! TRICE_SINGLE_PACK_MODE is the recommended TRICE_DEFERRED_TRANSFER_MODE. It packs each trice in a separate TCOBS package with a following 0-delimiter byte. 
//! Single trices need a bit more transfer data. In case of a data disruption, only a single trice messages can get lost.
#define TRICE_SINGLE_PACK_MODE  787345706U

//! TRICE_MULTI_PACK_MODE packs all trices of a buffer in a single TCOBS package and a following 0-delimiter byte. 
//! Grouped trices need a bit less transfer data. In case of a data disruption, multiple trice messages can get lost.
//! Olny relevant when TRICE_DOUBLE_BUFFER is selected 
#define TRICE_MULTI_PACK_MODE  3987862482U

//! With TRICE_BUFFER == TRICE_STACK_BUFFER  the internal macro TRICE_PUT writes to the stack. 
//! This is direct logging. This reduces memory needs if only one stack is used.
#define TRICE_STACK_BUFFER  2645382063U

//! With TRICE_BUFFER == TRICE_STATIC_BUFFER the internal macro TRICE_PUT writes to a static buffer. 
//! This reduces memory needs if many stacks are used.
#define TRICE_STATIC_BUFFER 1763551404U

//! With TRICE_BUFFER == TRICE_DOUBLE_BUFFER the internal macro TRICE_PUT writes to a double buffer half. 
//! This is deferred logging using more space but the TRICE macros are executed faster. 
#define TRICE_DOUBLE_BUFFER 1950870368U

//! With TRICE_BUFFER == TRICE_RING_BUFFER the internal macro TRICE_PUT writes to a ring buffer segment. 
//! This is deferred logging using less space but the TRICE macros are executed a bit slower. 
#define TRICE_RING_BUFFER 2237719049U

//! TRICE_FRAMING_TCOBS is recommended for trice transfer over UART.
#define TRICE_FRAMING_TCOBS 3745917584U

//! TRICE_FRAMING_COBS is recommended for encryptede trices.
#define TRICE_FRAMING_COBS  2953804234U

//! TRICE_FRAMING_NONE is recommended for RTT in direct mode. One trice costs about 100 clocks and is completely done.
#define TRICE_FRAMING_NONE  1431860787U

//! Variadic macros (https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms)
//! See for more explanation https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/preprocessor/macros/__VA_ARGS__/count-arguments
//! This is extendable until a 32767 bytes payload.
#define TRICE_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME

//#include <stdint.h> //lint !e537
#include <string.h>
#include "triceConfig.h"

// defaults part 1:

#ifndef TRICE_CONFIG_WARNINGS
#define TRICE_CONFIG_WARNINGS 1
#endif

#if ( (TRICE_BUFFER == TRICE_RING_BUFFER) || (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) ) && !defined(TRICE_DEFERRED_OUTPUT)
#define TRICE_DEFERRED_OUTPUT 1
#endif

#ifndef TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE
//! TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE==1 uses standard RTT transfer by using function SEGGER_RTT_WriteNoLock and needs ((TRICE_DIRECT_OUTPUT == 1).
//! - This setting results in unframed RTT trice packages and requires the `-packageFraming none` switch for the appropriate trice tool instance.
//! - Not that fast as with TRICE_SEGGER_RTT_32BIT_WRITE == 1 but still fast and uses pure SEGGER functionality only.
#define TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE 0
#endif

#ifndef TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE
//! TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1 speeds up RTT transfer by using function SEGGER_Write_RTT0_NoCheck32.
//! - This setting results in unframed RTT trice packages and requires the `-packageFraming none` switch for the appropriate trice tool instance.
//!   This squeezes the whole TRICE macro into about 100 processor clocks leaving the data already inside the SEGGER _acUpBuffer.
//! - If you do not wish RTT, or with RTT with framing, simply set this value to 0. 
//! - The trice tool CLI switch -d16 is needed too, because for alignment reasons the 16bit ID field is doubled for 16bit timestamp trice messages.
#define TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE 0
#endif

#ifndef TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE
//! TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1 enables channel number 0 for SeggerRTT usage. Only channel 0 works right now for some reason.
//! Than the RTT trice packages are can be framed according to the set TRICE_DIRECT_OUT_FRAMING.
//! Not useable with TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE or TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE
//! Switch this on, if you wish deferred Segger RTT output.
#define TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE 0
#endif

#if (TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE == 1) \
 || (TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1) \
 || (TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE == 1)
#define SEGGER_RTT
#endif 

#if defined(SEGGER_RTT) || (USE_SEGGER_RTT_LOCK_UNLOCK_MACROS == 1)
#include "SEGGER_RTT.h"
#endif 

/*
typedef enum {
    direct_NoCryptNoPack,
    direct_XteaNoPack,
    direct_NoCryptCobsPack,
    direct_XteaCobsPack,
    direct_NoCryptTobsPack,
    direct_XteaTobsPack,

    defered_SingleNoCryptNoPack,
    defered_SingleXteaNoPack,
    defered_SingleNoCryptCobsPack,
    defered_SingleXteaCobsPack,
    defered_SingleNoCryptTobsPack,
    defered_SingleXteaTobsPack,
    
    deferred_MultiNoCryptNoPack,
    deferred_MultiXteaNoPack,
    deferred_MultiNoCryptCobsPack,
    deferred_MultiXteaCobsPack,
    deferred_MultiNoCryptTobsPack,
    deferred_MultiXteaTobsPack,
} encode_t;
*/
// global function prototypes:

void TriceCheck( int index ); // tests and examples
void TriceDiagnostics( int index );
void TriceNonBlockingDirectWrite( uint32_t* triceStart, unsigned wordCount );
void TriceNonBlockingDirectWrite8Auxiliary( const uint8_t * enc, size_t encLen );
void TriceNonBlockingDeferredWrite8Auxiliary( const uint8_t * enc, size_t encLen );
void TriceNonBlockingDirectWrite32Auxiliary( const uint32_t * enc, unsigned count );
void TriceNonBlockingDeferredWrite32Auxiliary( const uint32_t * enc, unsigned count );
void TriceInit( void );
void TriceLogDiagnosticValues( void );
void TriceLogSeggerDiagnostics( void );
void TriceNonBlockingDeferredWrite8( int ticeID, const uint8_t * enc, size_t encLen );
void TriceTransfer( void );
void TriceWriteDeviceCgo( const uint8_t * buf, unsigned len ); // only needed for testing C-sources from Go
size_t triceDataLen( const uint8_t * p );

//int TriceIDAndBuffer( const uint32_t * const pAddr, int* pWordCount, uint8_t** ppStart, size_t* pLength );
//int TriceNext( uint8_t** buf, size_t* pSize, uint8_t** pStart, size_t* pLen );
int TriceEnoughSpace( void );

unsigned TriceOutDepth( void );
unsigned TriceOutDepthCGO( void ); // only needed for testing C-sources from Go

// uint32_t Us32( void );

size_t TriceDepth( void );
size_t TriceDepthMax( void );
//size_t TriceDeferredEncode( uint8_t* enc, const uint8_t * buf, size_t len );
size_t TriceEncode( unsigned encrypt, unsigned framing, uint8_t* dst, const uint8_t * buf, size_t len );

// global defines

#define TRICE_TYPE_X0 0 //!< TRICE_TYPE_X0 ist a unspecified trice (reserved)
#define TRICE_TYPE_S0 1 //!< TRICE_TYPE_S0 ist a trice without stamp.
#define TRICE_TYPE_S2 2 //!< TRICE_TYPE_S2 ist a trice with 16-bit stamp.
#define TRICE_TYPE_S4 3 //!< TRICE_TYPE_S4 ist a trice with 32-bit stamp.

// global variables:

extern uint32_t * const triceSingleBufferStartWritePosition;
extern unsigned SingleTricesRingCount;
extern unsigned SingleTricesRingCountMax;
extern char triceCommandBuffer[];
extern int triceCommandFlag;
extern uint8_t TriceCycle;
extern const int TriceTypeS0;
extern const int TriceTypeS2;
extern const int TriceTypeS4;
extern const int TriceTypeX0;
extern unsigned RTT0_writeDepthMax;
extern unsigned TriceErrorCount;
extern unsigned TriceDynBufTruncateCount;
extern unsigned TriceDirectOverflowCount;
extern unsigned TriceDeferredOverflowCount;
extern uint32_t * const TriceRingBufferStart;
extern uint32_t * const triceRingBufferLimit;
extern unsigned TriceSingleMaxWordCount;
extern unsigned TriceRingBufferDepthMax;
extern unsigned TriceHalfBufferDepthMax;
extern uint16_t TriceHalfBufferDepth;
extern uint16_t TriceRingBufferDepth;
extern int TriceDataOffsetDepthMax;

#if (TRICE_BUFFER == TRICE_RING_BUFFER) || (TRICE_BUFFER == TRICE_DOUBLE_BUFFER)
extern uint32_t * TriceBufferWritePosition;
#endif

// defaults part 2:

#ifndef TRICE_DIRECT_AUXILIARY8
#define TRICE_DIRECT_AUXILIARY8 0 //!< TRICE_DIRECT_AUXILIARY8 enables a user defined direct trice write.
#endif

#ifndef TRICE_DEFERRED_AUXILIARY8
#define TRICE_DEFERRED_AUXILIARY8 0 //!< TRICE_DEFERRED_AUXILIARY8 enables a user defined deferred trice write.
#endif

#ifndef TRICE_DIRECT_AUXILIARY32
#define TRICE_DIRECT_AUXILIARY32 0 //!< TRICE_DIRECT_AUXILIARY32 enables a user defineddirect trice write.
#endif

#ifndef TRICE_DEFERRED_AUXILIARY32
#define TRICE_DEFERRED_AUXILIARY32 0 //!< TRICE_DEFERRED_AUXILIARY32 enables a user defined deferred trice write.
#endif

#ifndef TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING

//! TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING allows to send an ID range of trices directly to an output.
//! The called output function usually is executed inside an interrupt and should therefore be non-blocking and fast.
#define TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING 0

#endif

#ifndef TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING

//! TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING allows to send an ID range of trices directly to an output.
//! The called output function should be non-blocking. 
#define TRICE_DEFERRED_OUTPUT_IS_WITH_ROUTING 0

#endif

#ifndef TRICE_SEGGER_RTT_32BIT_DIRECT_XTEA_AND_COBS

//! TRICE_SEGGER_RTT_32BIT_DIRECT_XTEA_AND_COBS == 1 is a special case for RTT32 encryption and framing. (experimental)
#define TRICE_SEGGER_RTT_32BIT_DIRECT_XTEA_AND_COBS 0

#endif


//! TRICE_BUFFER_SIZE is
//! - the additional needed stack space when TRICE_BUFFER == TRICE_STACK_BUFFER
//! - the statically allocated buffer size when TRICE_BUFFER == TRICE_STATIC_BUFFER
//! - the value before Ringbuffer wraps, when TRICE_BUFFER == TRICE_RING_BUFFER 
#define TRICE_BUFFER_SIZE (TRICE_DATA_OFFSET + TRICE_SINGLE_MAX_SIZE)

#ifndef TRICE_DEFAULT_PARAMETER_BIT_WIDTH

//! TRICE_DEFAULT_PARAMETER_BIT_WIDTH is the default parameter bit width for TRICE macros not specifying the parameter bit width: 8, 16, 32 or 64.
//! If for example the majority of your values is 16 bit, it makes sense to set this value to 16 to use TRICE for them and to use TRICE32 explicitely for 32-bit values.
//! The trice tool CLI switch "-defaultTRICEBitwidth" needs to be set to the same bit width, default is 32.
#define TRICE_DEFAULT_PARAMETER_BIT_WIDTH 32

#endif

#ifndef TRICE_B

//! TRICE_B is a shortcut for TRICE8_B, TRICE16_B, TRICE32_B or TRICE64_B usable in your project.
#define TRICE_B  TRICE8_B

#endif

#ifndef TRICE_F

//! TRICE_F is a shortcut for TRICE8_F, TRICE16_F, TRICE32_F or TRICE64_F usable in your project.
#define TRICE_F  TRICE8_F

#endif

#ifndef TRICE_CYCLE_COUNTER
//#warning configuration: TRICE_CYCLE_COUNTER is not defined, setting it to 1.
//! TRICE_CYCLE_COUNTER adds a cycle counter to each trice message.
//! If 0, do not add cycle counter. The TRICE macros are a bit faster. Lost TRICEs are not detectable by the trice tool. The cycle conter byte ist statically 0xC0.
//! If 1, add an 8-bit cycle counter. The TRICE macros are a bit slower. Lost TRICEs are detectable by the trice tool. The cycle conter byte changes (reccommended).
#define TRICE_CYCLE_COUNTER 1 

#endif

#ifndef TRICE_COMMAND_SIZE_MAX

//! TRICE_COMMAND_SIZE_MAX is the length limit for command strings to target.
#define TRICE_COMMAND_SIZE_MAX 8

#endif

#if TRICE_CYCLE_COUNTER == 1

#define TRICE_CYCLE TriceCycle++ //! TRICE_CYCLE is the trice cycle counter as 8 bit count 0-255.

#else // #if TRICE_CYCLE_COUNTER == 1

#define TRICE_CYCLE 0xC0 //! TRICE_CYCLE is no trice cycle counter, just a static value.

#endif // #else // #if TRICE_CYCLE_COUNTER == 1

#include "trice8.h"
#include "trice16.h"
#include "trice32.h"
#include "trice64.h"


//! TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN can be defined on little endian MCUs if the trice data are needed in network order,
//! or on big endian MCUs if the trice data are needed in little endian order. You should avoid using this macro because
//! it increases the trice storage time and the needed code amount. 
//! This is not completely implemented and needs automatic tests as well.
//! The main to implement:
//! - implement compiler agnostic access macros (byte swapping)
//! - The TriceIDAndBuffer function and it relatives need to use access macros. 
//#define TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN

#ifdef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN
// https://codereview.stackexchange.com/questions/151049/endianness-conversion-in-c
//#include <byteswap.h>

/*
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
*/

static inline uint16_t Reverse16(uint16_t value)
{
    return (((value & 0x00FF) << 8) |
            ((value & 0xFF00) >> 8));
}
    
TRICE_INLINE uint32_t Reverse32(uint32_t value) 
{
    return (((value & 0x000000FF) << 24) |
            ((value & 0x0000FF00) <<  8) |
            ((value & 0x00FF0000) >>  8) |
            ((value & 0xFF000000) >> 24));
}

    //! TRICE_HTOTS reorders short values from host order into trice transfer order. 
    #define TRICE_HTOTS(x) Reverse16(x) // __bswap_16((x))) // TRICE_SWAPINT16(x)

    //! TRICE_HTOTL reorders long values from host order x into trice transfer order. 
    #define TRICE_HTOTL(x) Reverse32(x) // __bswap_32((x))) // TRICE_SWAPINT32(x)

    //! TRICE_TTOHS reorders short values from trice transfer order into host order. 
    #define TRICE_TTOHS(x) Reverse16(x) // __bswap_16((x))) // TRICE_SWAPINT16(x)

#else // #ifdef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN

    //! TRICE_HTOTS reorders short values from hos // t order into trice transfer order.
    #define TRICE_HTOTS(x) (x)

    //! TRICE_HTOTL reorders long values from host order x into trice transfer order. 
    #define TRICE_HTOTL(x) (x)

    //! TRICE_TTOHS reorders short values from trice transfer order into host order.  
    #define TRICE_TTOHS(x) (x)

#endif // #else // #ifdef TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN

#if TRICE_DIAGNOSTICS == 1

#define TRICE_DIAGNOSTICS_SINGLE_BUFFER_KEEP_START \
    uint32_t* const triceSingleBufferStartWritePosition = TriceBufferWritePosition;

#define TRICE_DIAGNOSTICS_SINGLE_BUFFER do { \
    unsigned wordCount = TriceBufferWritePosition - triceSingleBufferStartWritePosition; \
    TriceSingleMaxWordCount = (wordCount < TriceSingleMaxWordCount) ? TriceSingleMaxWordCount : wordCount; \
    } while(0);

#define TRICE_DIAGNOSTICS_SINGLE_BUFFER_USING_WORDCOUNT do { \
    TriceSingleMaxWordCount = (wordCount < TriceSingleMaxWordCount) ? TriceSingleMaxWordCount : wordCount; \
    } while(0);

#else // #if TRICE_DIAGNOSTICS == 1

#define TRICE_DIAGNOSTICS_SINGLE_BUFFER_KEEP_START
#define TRICE_DIAGNOSTICS_SINGLE_BUFFER
#define TRICE_DIAGNOSTICS_SINGLE_BUFFER_USING_WORDCOUNT 

#endif // #else // #if TRICE_DIAGNOSTICS == 1

#ifndef TRICE_ENTER

#if TRICE_BUFFER ==  TRICE_STACK_BUFFER 

    //! TRICE_ENTER is the start of TRICE macro.
    #define TRICE_ENTER \
    TRICE_ENTER_CRITICAL_SECTION { \
    { \
        uint32_t triceSingleBuffer[TRICE_BUFFER_SIZE>>2]; \
        uint32_t* const triceSingleBufferStartWritePosition = &triceSingleBuffer[TRICE_DATA_OFFSET>>2]; \
        uint32_t* TriceBufferWritePosition = triceSingleBufferStartWritePosition;

#endif // #if TRICE_BUFFER == TRICE_STACK_BUFFER

#if TRICE_BUFFER == TRICE_STATIC_BUFFER

    //! TRICE_ENTER is the start of TRICE macro.
    #define TRICE_ENTER \
    TRICE_ENTER_CRITICAL_SECTION { \
    { \
        uint32_t* TriceBufferWritePosition = triceSingleBufferStartWritePosition;

#endif // #if TRICE_BUFFER == TRICE_STATIC_BUFFER

#if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUTPUT == 1)

    #ifdef TRICE_PROTECT
    
        //! TRICE_ENTER is the start of TRICE macro.
        #define TRICE_ENTER \
        TRICE_ENTER_CRITICAL_SECTION { \
        if( TriceEnoughSpace() ){ \
            uint32_t* const triceSingleBufferStartWritePosition = TriceBufferWritePosition;

    #else //  #ifdef TRICE_PROTECT

        //! TRICE_ENTER is the start of TRICE macro.
        #define TRICE_ENTER \
        TRICE_ENTER_CRITICAL_SECTION { \
        { \
            uint32_t* const triceSingleBufferStartWritePosition = TriceBufferWritePosition;

    #endif // #else //  #ifdef TRICE_PROTECT

#endif // #if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUTPUT == 1)

#if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUTPUT == 0)

    #ifdef TRICE_PROTECT

        //! TRICE_ENTER is the start of TRICE macro.
        #define TRICE_ENTER \
        TRICE_ENTER_CRITICAL_SECTION { \
        if( TriceEnoughSpace() ){ \
            TRICE_DIAGNOSTICS_SINGLE_BUFFER_KEEP_START

    #else //  #ifdef TRICE_PROTECT
        
        //! TRICE_ENTER is the start of TRICE macro.
        #define TRICE_ENTER \
        TRICE_ENTER_CRITICAL_SECTION { \
        { \
            TRICE_DIAGNOSTICS_SINGLE_BUFFER_KEEP_START

    #endif // #else //  #ifdef TRICE_PROTECT

#endif // #if (TRICE_BUFFER == TRICE_DOUBLE_BUFFER) && (TRICE_DIRECT_OUTPUT == 0)

#if (TRICE_BUFFER == TRICE_RING_BUFFER) && (TRICE_DIRECT_OUTPUT == 1)

    #ifdef TRICE_PROTECT

        //! TRICE_ENTER is the start of TRICE macro.
        #define TRICE_ENTER \
        TRICE_ENTER_CRITICAL_SECTION { \
        if( TriceEnoughSpace() ){ \
            /* The TriceBufferWritePosition stays unchanged, when there is enough space for the next trice at the current write position.*/ \
            /* Because the the size of the next trice message is unknown here, the biggest value is assumed, that is TRICE_BUFFER_SIZE bytes. */ \
            /* If this space is not given anymore, the `TriceBufferWritePosition` is reset to the start address of the ring buffer. */ \
            /* This implementation is a bit different than a ring buffer is usually implemented. */ \
            TriceBufferWritePosition = (TriceBufferWritePosition + (TRICE_BUFFER_SIZE>>2)) <= triceRingBufferLimit ? TriceBufferWritePosition : TriceRingBufferStart; \
            /*TriceBufferWritePosition += (TRICE_DATA_OFFSET>>2);*/ /* space for in buffer encoding */ \
            uint32_t* const triceSingleBufferStartWritePosition = TriceBufferWritePosition; \
            SingleTricesRingCount++; // Because TRICE macros are an atomic instruction normally, this can be done here.

    #else //  #ifdef TRICE_PROTECT

        //! TRICE_ENTER is the start of TRICE macro.
        #define TRICE_ENTER \
        TRICE_ENTER_CRITICAL_SECTION { \
        { \
            /* The TriceBufferWritePosition stays unchanged, when there is enough space for the next trice at the current write position.*/ \
            /* Because the the size of the next trice message is unknown here, the biggest value is assumed, that is TRICE_BUFFER_SIZE bytes. */ \
            /* If this space is not given anymore, the `TriceBufferWritePosition` is reset to the start address of the ring buffer. */ \
            /* This implementation is a bit different than a ring buffer is usually implemented. */ \
            TriceBufferWritePosition = (TriceBufferWritePosition + (TRICE_BUFFER_SIZE>>2)) <= triceRingBufferLimit ? TriceBufferWritePosition : TriceRingBufferStart; \
            /*TriceBufferWritePosition += (TRICE_DATA_OFFSET>>2);*/ /* space for in buffer encoding */ \
            uint32_t* const triceSingleBufferStartWritePosition = TriceBufferWritePosition; \
            SingleTricesRingCount++; // Because TRICE macros are an atomic instruction normally, this can be done here.

    #endif // #else //  #ifdef TRICE_PROTECT

#endif // #if TRICE_BUFFER == TRICE_RING_BUFFER && (TRICE_DIRECT_OUTPUT == 1)

#if (TRICE_BUFFER == TRICE_RING_BUFFER) && (TRICE_DIRECT_OUTPUT == 0)

    #ifdef TRICE_PROTECT

        //! TRICE_ENTER is the start of TRICE macro.
        #define TRICE_ENTER \
        TRICE_ENTER_CRITICAL_SECTION { \
        if( TriceEnoughSpace() ){ \
            TriceBufferWritePosition = (TriceBufferWritePosition + (TRICE_BUFFER_SIZE>>2)) <= triceRingBufferLimit ? TriceBufferWritePosition : TriceRingBufferStart; \
            /*TriceBufferWritePosition += (TRICE_DATA_OFFSET>>2);*/ /* space for in buffer encoding */ \
            TRICE_DIAGNOSTICS_SINGLE_BUFFER_KEEP_START \
            SingleTricesRingCount++; // Because TRICE macros are an atomic instruction normally, this can be done here.

    #else //  #ifdef TRICE_PROTECT

        //! TRICE_ENTER is the start of TRICE macro.
        #define TRICE_ENTER \
        TRICE_ENTER_CRITICAL_SECTION { \
        { \
            TriceBufferWritePosition = (TriceBufferWritePosition + (TRICE_BUFFER_SIZE>>2)) <= triceRingBufferLimit ? TriceBufferWritePosition : TriceRingBufferStart; \
            /*TriceBufferWritePosition += (TRICE_DATA_OFFSET>>2);*/ /* space for in buffer encoding */ \
            TRICE_DIAGNOSTICS_SINGLE_BUFFER_KEEP_START \
            SingleTricesRingCount++; // Because TRICE macros are an atomic instruction normally, this can be done here.

    #endif // #else //  #ifdef TRICE_PROTECT

#endif // #if TRICE_BUFFER == TRICE_RING_BUFFER && (TRICE_DIRECT_OUTPUT == 0)

#endif

#ifndef TRICE_LEAVE
    #if TRICE_DIRECT_OUTPUT == 1

        //! TRICE_LEAVE is the end of TRICE macro. It is the same for all buffer variants.
        #define TRICE_LEAVE \
                /* wordCount is the amount of steps, the TriceBufferWritePosition went forward for the actual trice.  */ \
                /* The last written uint32_t trice value can contain 1 to 3 padding bytes. */ \
                unsigned wordCount = TriceBufferWritePosition - triceSingleBufferStartWritePosition; \
                TRICE_DIAGNOSTICS_SINGLE_BUFFER_USING_WORDCOUNT \
                TriceNonBlockingDirectWrite(triceSingleBufferStartWritePosition, wordCount); \
            } \
        } TRICE_LEAVE_CRITICAL_SECTION

    #else  //#if TRICE_DIRECT_OUTPUT == 1
        
        //! TRICE_LEAVE is the end of TRICE macro. It is the same for all buffer variants.
        #define TRICE_LEAVE \
                TRICE_DIAGNOSTICS_SINGLE_BUFFER \
            } \
        } TRICE_LEAVE_CRITICAL_SECTION

    #endif // #else  //#if TRICE_DIRECT_OUTPUT == 1
#endif // #ifndef TRICE_LEAVE

// trice macros:

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

#if defined( TRICE_UARTA ) // deferred out to UARTA
void TriceBlockingWriteUartA( const uint8_t * buf, unsigned len );
uint8_t TriceNextUint8UartA( void );
void triceServeTransmitUartA(void);
void triceTriggerTransmitUartA(void);
unsigned TriceOutDepthUartA( void );
#endif

#if defined( TRICE_UARTB ) // deferred out to UARTB
void TriceBlockingWriteUartB( const uint8_t * buf, unsigned len );
uint8_t TriceNextUint8UartB( void );
void triceServeTransmitUartB(void);
void triceTriggerTransmitUartB(void);
unsigned TriceOutDepthUartB( void );
#endif

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Encryption
//
#ifndef TRICE_XTEA_DIRECT_ENCRYPT
#define TRICE_XTEA_DIRECT_ENCRYPT 0
#endif

#ifndef TRICE_XTEA_DEFERRED_ENCRYPT
#define TRICE_XTEA_DEFERRED_ENCRYPT 0
#endif

#ifdef XTEA_ENCRYPT_KEY
    void XTEAEncrypt( uint32_t* p, unsigned count );
    void XTEADecrypt( uint32_t* p, unsigned count );
    void XTEAInitTable(void);
#endif // #ifdef XTEA_ENCRYPT_KEY
//
///////////////////////////////////////////////////////////////////////////////

//! TRICE_NTH_ARGUMENT just evaluates to the 15th argument. It is extendable.
#define TRICE_NTH_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, ...) a14 

//! TRICE_COUNT_ARGUMENTS builds upon TRICE_NTH_ARGUMENT. The more arguments that are passed to TRICE_COUNT_ARGUMENTS, 
//! the more the »counting arguments« (12, 11, 10, 9, 8, 7…) are pushed to the right. 
//! Thus the macro evaluates to the number of arguments that are passed to the macro.
//! If you set the C language to strict C (C90, C99, C11 or C17) the `##` operator doesn't remove the comma before it when `__VA_ARGS__` expands to nothing.
//! In this case, the TRICE macro doesn't work with no parameters. You must then explicitly use TRICE0 instead of TRICE for a no parameter value TRICE.
//! For more details see closed Issue #279. Special thanks @escherstair.
//! If for example using CLANG 6.18 set C-language to gnu11, gnu99 or std to avoid the comma issue when no parameters are in a TRICE  macro.
//! In case you have to set the C-Language to c11 or c99 you can use the TRICE0 macro directly instead of TRICE when no value parameters.
#define TRICE_COUNT_ARGUMENTS(...) TRICE_NTH_ARGUMENT(dummy, ## __VA_ARGS__, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

//! TRICE_CONCAT concatenates the 2 arguments a and b (helper macro).
#define TRICE_CONCAT(a, b) a ## b 

//! TRICE_CONCAT2 concatenates the 2 arguments a and b (helper macro).
#define TRICE_CONCAT2(a, b) TRICE_CONCAT(a, b)

//! TRICE_VARIABLE_ARGUMENTS concatenates TRICE_ with the result of TRICE_COUNT_ARGUMENTS to produce something like TRICE_2 which takes a printf-format and two arguments.
#define TRICE( tid, fmt, ...) TRICE_CONCAT2(TRICE_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

#ifdef TRICE_CLEAN

#define TRice( fmt, ...)
#define Trice( fmt, ...)
#define trice( fmt, ...)

#else // #ifdef TRICE_CLEAN

#define TRice( tid, fmt, ...) TRICE_CONCAT2(TRice_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define Trice( tid, fmt, ...) TRICE_CONCAT2(Trice_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define trice( tid, fmt, ...) TRICE_CONCAT2(trice_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

#endif // #else // #ifdef TRICE_CLEAN

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
//! \param tid trice identifier
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
    uint32_t limit = TRICE_SINGLE_MAX_SIZE-8; /* 8 = head(2) + max timestamp size(4) + count(2) */ \
    uint32_t len_ = n; /* n could be a constant */ \
    if( len_ > limit ){ \
        /*TRICE32( id( 5150), "wrn:Transmit buffer truncated from %u to %u\n", len_, limit );*/ \
        TriceDynBufTruncateCount++; \
        len_ = limit; \
    } \
    TRICE_ENTER tid; \
    if( len_ <= 127 ){ TRICE_CNTC(len_); }else{ TRICE_LCNT(len_); } \
    TRICE_PUTBUFFER( buf, len_ ); \
    TRICE_LEAVE \
} while(0)
#endif // #ifndef TRICE_N

#ifndef TRICE_S
//! TRICE_S writes id and dynString.
//! \param tid trice identifier
//! \param pFmt formatstring for trice (ignored here but used by the trice tool)
//! \param dynString 0-terminated runtime generated string
#define TRICE_S( tid, pFmt, dynString) do { \
    uint32_t ssiz = strlen( dynString ); \
    TRICE_N( tid, pFmt, dynString, ssiz ); \
} while(0)
#endif // #ifndef TRICE_S

#ifndef TRICE_PUT16

//! TRICE_PUT16 copies a 16 bit x into the TRICE buffer.
#define TRICE_PUT16(x) do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = TRICE_HTOTS(x); TriceBufferWritePosition = (uint32_t*)p; }while(0)

#endif

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
#define ID(n) do { uint32_t ts = TriceStamp32(); TRICE_PUT16( (0xC000|(n))); TRICE_PUT1616(ts); } while(0)

//! Id writes 14-bit id with 10 as 2 most significant bits two times, followed by a 16-bit stamp.
//! 10iiiiiiI 10iiiiiiI | TT (NC) | ...
//! 8000 = 1000 0000 0000 0000
#define Id(n) do { uint16_t ts = TriceStamp16(); TRICE_PUT((0x80008000|((n)<<16)|(n))); TRICE_PUT16(ts); } while(0)

//! id writes 14-bit id with 01 as 2 most significant bits, followed by no stamp.
//! 01iiiiiiI (NC) | ...
//! 4000 = 0100 0000 0000 0000
#define id(n) TRICE_PUT16( 0x4000|(n));

//! iD is just a code parsing helper.
#define iD(n) (n)

//! TRICE_CNTC writes 7-bit byte count and 8-bit cycle counter.
#define TRICE_CNTC(count) do{ uint16_t v = ((count)<<8) | TRICE_CYCLE; TRICE_PUT16( v ); }while(0)

#if TRICE_CYCLE_COUNTER == 1

//! TRICE_LCNT writes 1 as most significant bit and 15-bit byte count. It does not write the cycle counter but increments the cycle counter.
#define TRICE_LCNT(count) TRICE_PUT16( (0x8000 | (count)) ); TRICE_CYCLE  // increment TRICE_CYCLE but do not transmit it

#else

//! TRICE_LCNT writes 1 as most significant bit and 15-bit byte count. It does not write the cycle counter but increments the cycle counter.
#define TRICE_LCNT(count) TRICE_PUT16( (0x8000 | (count)) );  // no TRICE_CYCLE

#endif

//! TRICE0 writes trice data as fast as possible in a buffer.
//! \param tid is a 16 bit Trice id in upper 2 bytes of a 32 bit value
#define TRICE0( tid, pFmt ) \
    TRICE_ENTER tid; TRICE_CNTC(0); \
    TRICE_LEAVE

#ifdef TRICE_CLEAN

TRICE_INLINE void TRice0( const char * pFmt ){}
TRICE_INLINE void Trice0( const char * pFmt ){}
TRICE_INLINE void trice0( const char * pFmt ){}

#else // #ifdef TRICE_CLEAN

TRICE_INLINE void TRice0( uint16_t tid, const char * pFmt ){ TRice32m_0( tid ); }
TRICE_INLINE void Trice0( uint16_t tid, const char * pFmt ){ Trice32m_0( tid ); }
TRICE_INLINE void trice0( uint16_t tid, const char * pFmt ){ trice32m_0( tid ); }

#endif // #else // #ifdef TRICE_CLEAN

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

#ifdef TRICE_CLEAN

TRICE_INLINE void triceAssertTrue( int idN, char* msg, int flag ){}
TRICE_INLINE void TriceAssertTrue( int idN, char* msg, int flag ){}
TRICE_INLINE void TRiceAssertTrue( int idN, char* msg, int flag ){}

TRICE_INLINE void triceAssertFalse( int idN, char* msg, int flag ){}
TRICE_INLINE void TriceAssertFalse( int idN, char* msg, int flag ){}
TRICE_INLINE void TRiceAssertFalse( int idN, char* msg, int flag ){}

#else // #ifdef TRICE_CLEAN

void triceAssertTrue( int idN, char* msg, int flag );
void TriceAssertTrue( int idN, char* msg, int flag );
void TRiceAssertTrue( int idN, char* msg, int flag );

void triceAssertFalse( int idN, char* msg, int flag );
void TriceAssertFalse( int idN, char* msg, int flag );
void TRiceAssertFalse( int idN, char* msg, int flag );

#endif // #else // #ifdef TRICE_CLEAN

typedef void (*Write8AuxiliaryFn_t)( const uint8_t * enc, size_t encLen );
extern Write8AuxiliaryFn_t UserNonBlockingDirectWrite8AuxiliaryFn;
extern Write8AuxiliaryFn_t UserNonBlockingDeferredWrite8AuxiliaryFn;

typedef void (*Write32AuxiliaryFn_t)( const uint32_t * enc, unsigned count );
extern Write32AuxiliaryFn_t UserNonBlockingDirectWrite32AuxiliaryFn;
extern Write32AuxiliaryFn_t UserNonBlockingDeferredWrite32AuxiliaryFn;

#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
