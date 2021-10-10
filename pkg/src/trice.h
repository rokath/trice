/*! \file trice.h
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_H_
#define TRICE_H_

#include "triceConfig.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// compiler adjustment
//

#if defined( __GNUC__ ) /* gnu compiler ###################################### */ \
 || defined(__IAR_SYSTEMS_ICC__) /* IAR compiler ############################# */ \
 || defined(__TASKING__) /* TASKING compiler (same bugs as GNU!)############## */

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4                                  //!< align to 4 byte boundary preamble
#define ALIGN4_END __attribute__ ((aligned(4))) //!< align to 4 byte boundary post declaration
#define PACKED                                  //!< pack data preamble
#define PACKED_END __attribute__ ((packed))     //!< pack data post declaration

#ifndef TRICE_ENTER_CRITICAL_SECTION
//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro empty for more speed. Use only '{' in that case.
#define TRICE_ENTER_CRITICAL_SECTION { // to do
#endif

#ifndef TRICE_LEAVE_CRITICAL_SECTION
//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro pair empty for more speed. Use only '}' in that case.
#define TRICE_LEAVE_CRITICAL_SECTION } // to do
#endif

#elif defined(__arm__) // ARMkeil IDE #########################################

#include <cmsis_armcc.h>

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4 __align(4) //!< align to 4 byte boundary preamble
#define ALIGN4_END        //!< align to 4 byte boundary post declaration
#define PACKED __packed   //!< pack data preamble
#define PACKED_END        //!< pack data post declaration

#ifndef TRICE_ENTER_CRITICAL_SECTION
//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro empty for more speed. Use only '{' in that case.
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {
#endif 

#ifndef TRICE_LEAVE_CRITICAL_SECTION
//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro pair empty for more speed. Use only '}' in that case.
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }
#endif

#elif 1 // ####################################################################
#error "add new compiler here"
#else // ######################################################################
#error unknown compliler
#endif // compiler adaptions ##################################################

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// trice time measurement
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
#define SYSTICKVAL32 (*(volatile uint32_t*)0xE000E018UL)
#else
#error "unknown architecture"
#define SYSTICKVAL32 0
#endif

#define SYSTICKVAL16 (SYSTICKVAL32)

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// UART interface
//

#include "main.h" // hardware specific stuff

#define TRICE_UART USART2 //!< set UART number if UART is used

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmpty(void) {
    return LL_USART_IsActiveFlag_TXE(TRICE_UART);
}

//! Write value v into trice transmit register.
//! \param v byte to transmit
//! User must provide this function.
TRICE_INLINE void triceTransmitData8(uint8_t v) {
    LL_USART_TransmitData8(TRICE_UART, v);
}

//! Allow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceEnableTxEmptyInterrupt(void) {
    LL_USART_EnableIT_TXE(TRICE_UART);
}

//! Disallow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceDisableTxEmptyInterrupt(void) {
    LL_USART_DisableIT_TXE(TRICE_UART);
}
//
///////////////////////////////////////////////////////////////////////////////


#define TRICE_U8_FIFO_MASK ((TRICE_FIFO_BYTE_SIZE)-1) //!< max possible bytes count in fifo

int triceU8FifoDepth(void);
extern uint8_t* const triceU8Fifo;
extern int triceU8FifoWriteIndex;
extern int triceU8FifoReadIndex;
extern uint16_t TriceDepthMax;
void TriceReadAndTranslate( void );

#define TRICE_LITTLE_ENDIANNESS 0x00112233
#define TRICE_BIG_ENDIANNESS    0x33221100

#define TRICE_SINGLE_MESSAGE 1
#define TRICE_MULTI_MESSAGE  2

#ifdef ENCRYPT
void encrypt(uint8_t *p);
void decrypt(uint8_t *p);

//! little endian! change byte order for big endian machines
#define XTEA_KEY(b00, b01, b02, b03, \
                  b10, b11, b12, b13, \
                  b20, b21, b22, b23, \
                  b30, b31, b32, b33) { \
    0x##b00##b01##b02##b03, \
    0x##b10##b11##b12##b13, \
    0x##b20##b21##b22##b23, \
    0x##b30##b31##b32##b33 }

void InitXteaTable(void);
    
#endif // #ifdef ENCRYPT

#define TRICE8_COUNT(_1,_2,_3,_4,_5,_6,_7,_8, NAME,...) NAME
#define TRICE8(id,frmt, ...) TRICE8_COUNT(__VA_ARGS__,TRICE8_8,TRICE8_7,TRICE8_6,TRICE8_5,TRICE8_4,TRICE8_3,TRICE8_2,TRICE8_1)(id,frmt, __VA_ARGS__)

#define TRICE16_COUNT(_1,_2,_3,_4, NAME,...) NAME
#define TRICE16(id,frmt, ...) TRICE16_COUNT(__VA_ARGS__,TRICE16_4,TRICE16_3,TRICE16_2,TRICE16_1)(id,frmt, __VA_ARGS__)

#define TRICE32_COUNT(_1,_2,_3,_4, NAME,...) NAME
#define TRICE32(id,frmt, ...) TRICE32_COUNT(__VA_ARGS__,TRICE32_4,TRICE32_3,TRICE32_2,TRICE32_1)(id,frmt, __VA_ARGS__)

#define TRICE64_COUNT(_1,_2,_3,_4, NAME,...) NAME
#define TRICE64(id,frmt, ...) TRICE64_COUNT(__VA_ARGS__,TRICE64_4,TRICE64_3,TRICE64_2,TRICE64_1)(id,frmt, __VA_ARGS__)

#ifdef TRICE_OFF
#define Id(n) (n)
#define TRICE_S( id, p, s )  do{ ((void)(id)); ((void)(p)); ((void)(s)); }while(0)
#define TRICE0( id, pFmt )
#define TRICE8_1( id, pFmt, v0                              ) do{ ((void)(v0)); }while(0)
#define TRICE8_2( id, pFmt, v0, v1                          ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define TRICE8_3( id, pFmt, v0, v1, v2                      ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define TRICE8_4( id, pFmt, v0, v1, v2, v3                  ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define TRICE8_5( id, pFmt, v0, v1, v2, v3, v4              ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); }while(0)
#define TRICE8_6( id, pFmt, v0, v1, v2, v3, v4, v5          ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); }while(0)
#define TRICE8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6      ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); ((void)(v6)); }while(0)
#define TRICE8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7  ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); ((void)(v4)); ((void)(v5)); ((void)(v6)); ((void)(v7)); }while(0)
#define TRICE16_1( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)
#define TRICE16_2( id, pFmt, v0, v1                         ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define TRICE16_3( id, pFmt, v0, v1, v2                     ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define TRICE16_4( id, pFmt, v0, v1, v2, v3                 ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define TRICE32_1( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)
#define TRICE32_2( id, pFmt, v0, v1                         ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#define TRICE32_3( id, pFmt, v0, v1, v2                     ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); }while(0)
#define TRICE32_4( id, pFmt, v0, v1, v2, v3                 ) do{ ((void)(v0)); ((void)(v1)); ((void)(v2)); ((void)(v3)); }while(0)
#define TRICE64_1( id, pFmt, v0                             ) do{ ((void)(v0)); }while(0)
#define TRICE64_2( id, pFmt, v0, v1                         ) do{ ((void)(v0)); ((void)(v1)); }while(0)
#else

#include <stdint.h>
#include <string.h>

extern uint32_t* wTb;

#define Id(n) ((uint32_t)n<<16) //!< Id() is a 16 bit id 0-65535 as upper 2 bytes in head

#if TRICE_CYCLE_COUNTER == 1
extern uint8_t triceCycle;
#define TRICE_CYCLE triceCycle++ //! TRICE_CYCLE is the trice cycle counter as 8 bit count 0-255.
#else
#define TRICE_CYCLE 0xC0 //! TRICE_CYCLE is no trice cycle counter, just a static value.
#endif

//! TRICE0 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
#define TRICE0( id, pFmt ) \
    TRICE_ENTER \
    PUT( id | 0x0000 | TRICE_CYCLE ); \
    TRICE_LEAVE

//! TRICE8_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 a 8 bit bit value
#define TRICE8_1( id, pFmt, v0 ) \
    TRICE_ENTER \
    PUT( id | 0x0100 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) ); /* little endian*/ \
    TRICE_LEAVE

//! TRICE8_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v1 are 8 bit bit values
#define TRICE8_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER \
    PUT( id | 0x0200 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) ); \
    TRICE_LEAVE

//! TRICE8_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v2 are 8 bit bit values
#define TRICE8_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER \
    PUT( id | 0x0300 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) ); \
    TRICE_LEAVE

//! TRICE8_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v3 are 8 bit bit values
#define TRICE8_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    PUT( id | 0x0400 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    TRICE_LEAVE

//! TRICE8_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v4 are 8 bit bit values
#define TRICE8_5( id, pFmt, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    PUT( id | 0x0500 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    PUT( (uint8_t)(v4) ); \
    TRICE_LEAVE

//! TRICE8_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v5 are 8 bit bit values
#define TRICE8_6( id, pFmt, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    PUT( id | 0x0600 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v6 are 8 bit bit values
#define TRICE8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    PUT( id | 0x0700 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) | ((uint32_t)(0xff&(v6))<<16) ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    PUT( id | 0x0800 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) | ((uint32_t)(0xff&(v6))<<16) | ((uint32_t)(v7)<<24) ); \
    TRICE_LEAVE

//! TRICE16_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 a 16 bit value
#define TRICE16_1( id, pFmt, v0 ) \
    TRICE_ENTER \
    PUT( id | 0x0200 | TRICE_CYCLE ); \
    PUT( (uint16_t)(v0) ); \
    TRICE_LEAVE

//! TRICE16_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v1 are 16 bit values
#define TRICE16_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER \
    PUT( id | 0x0400 | TRICE_CYCLE ); \
    PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_LEAVE

//! TRICE16_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v2 are 16 bit values
#define TRICE16_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER \
    PUT( id | 0x0600 | TRICE_CYCLE ); \
    PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    PUT( (uint16_t)(v2) ); \
    TRICE_LEAVE

//! TRICE16_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v3 are 16 bit values
#define TRICE16_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    PUT( id | 0x0800 | TRICE_CYCLE ); \
    PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_LEAVE

//! TRICE32_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 the 32 bit value
#define TRICE32_1( id, pFmt, v0 ) \
    TRICE_ENTER \
    PUT( id | 0x0400 | TRICE_CYCLE); \
    PUT( (uint32_t)(v0) ); /* little endian*/ \
    TRICE_LEAVE

//! TRICE32_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v1 are 32 bit values
#define TRICE32_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER \
    PUT(id | 0x0800 | TRICE_CYCLE ); \
    PUT( (uint32_t)(v0) ); \
    PUT( (uint32_t)(v1) ); \
    TRICE_LEAVE

//! TRICE32_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v2 are 32 bit values
#define TRICE32_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER \
    PUT( id | 0x0c00 | TRICE_CYCLE ); \
    PUT( (uint32_t)(v0) ); \
    PUT( (uint32_t)(v1) ); \
    PUT( (uint32_t)(v2) ); \
    TRICE_LEAVE

//! TRICE32_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v3 are 32 bit values
#define TRICE32_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    PUT( id | 0x1000 | TRICE_CYCLE ); \
    PUT( (uint32_t)(v0) ); \
    PUT( (uint32_t)(v1) ); \
    PUT( (uint32_t)(v2) ); \
    PUT( (uint32_t)(v3) ); \
    TRICE_LEAVE

#if TRICE_HARDWARE_ENDIANNESS == TRICE_LITTLE_ENDIANNESS

//! TRICE64_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 is a 64 bit values
#define TRICE64_1( id, pFmt, v0 ) \
    TRICE_ENTER \
    PUT( id | 0x0800 | TRICE_CYCLE ); \
    PUT( (uint32_t)(v0) ); \
    PUT( (uint32_t)((uint64_t)(v0)>>32) ); \
    TRICE_LEAVE

//! TRICE64_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v1 are 64 bit values
#define TRICE64_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER \
    PUT( id | 0x1000 | TRICE_CYCLE ); \
    PUT( (uint32_t)(v0) ); \
    PUT( (uint32_t)((uint64_t)(v0)>>32) ); \
    PUT( (uint32_t)(v1) ); \
    PUT( (uint32_t)((uint64_t)(v1)>>32) ); \
    TRICE_LEAVE

//! TRICE64_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v2 are 64 bit values
#define TRICE64_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER \
    PUT( id | 0x1800 | TRICE_CYCLE ); \
    PUT( (uint32_t)(v0) ); \
    PUT( (uint32_t)(v0>>32) ); \
    PUT( (uint32_t)(v1) ); \
    PUT( (uint32_t)(v1>>32) ); \
    PUT( (uint32_t)(v2) ); \
    PUT( (uint32_t)(v2>>32) ); \
    TRICE_LEAVE


//! TRICE64_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v3 are 64 bit values
#define TRICE64_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    PUT( id | 0x2000 | TRICE_CYCLE ); \
    PUT( (uint32_t)(v0) ); \
    PUT( (uint32_t)(v0>>32) ); \
    PUT( (uint32_t)(v1) ); \
    PUT( (uint32_t)(v1>>32) ); \
    PUT( (uint32_t)(v2) ); \
    PUT( (uint32_t)(v2>>32) ); \
    PUT( (uint32_t)(v3) ); \
    PUT( (uint32_t)(v3>>32) ); \
    TRICE_LEAVE

#else // #if TRICE_HARDWARE_ENDIANNESS == TRICE_LITTLE_ENDIANNESS

#endif // #else // #if TRICE_HARDWARE_ENDIANNESS == TRICE_LITTLE_ENDIANNESS

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
    int len = strlen( dynString ); \
    if( len > 255 ){ \
        dynString[255] = 0; \
        len = 255; \
    } \
    TRICE_ENTER \
    PUT( id | (len<<8) | TRICE_CYCLE ); \
    PUT_BUFFER( dynString, len ); \
    TRICE_LEAVE \
} while(0)

#endif

/*
#ifdef TRICE_RTT_CHANNEL
#include "SEGGER_RTT.h"

//! put one trice into RTT0 buffer
//! \param v trice
//! trice time critical part
TRICE_INLINE void triceU32PushSeggerRTT(uint32_t v) {
    SEGGER_RTT_Write(TRICE_RTT_CHANNEL, &v, sizeof(v));
}

//! put one byte into RTT0 buffer
//! \param v byte
//! trice time critical part
TRICE_INLINE void triceU8PushSeggerRTT(uint8_t v) {
    SEGGER_RTT_Write(TRICE_RTT_CHANNEL, &v, sizeof(v));
}

#else // #ifdef TRICE_RTT_CHANNEL

#define triceU8PushSeggerRTT(v)
#define triceU32PushSeggerRTT(v)

#endif // #else // #ifdef TRICE_RTT_CHANNEL

#define TRICE_HTON_U32PUSH(v) TRICE_U32PUSH( TRICE_HTON(v) )

//! TRICE_FIFO_BYTE_SIZE must be a power of 2, one trice needs typically 4 or 8 bytes, max 32 bytes.
//! The fifo has to hold trice bursts until they are transmitted.
//! It is transmitted with lower priority in the background for example with the UART tx interrupt.
#ifndef TRICE_FIFO_BYTE_SIZE
#define TRICE_FIFO_BYTE_SIZE (2048)
#endif

#define TRICE_U32_FIFO_MASK (((TRICE_FIFO_BYTE_SIZE)>>2)-1) //!< max possible int32 count in fifo

extern uint32_t triceU32Fifo[ TRICE_FIFO_BYTE_SIZE>>2 ];


extern int triceU32FifoWriteIndex;
extern int triceU32FifoReadIndex;


extern int triceFifoMaxDepth;

//! triceU32Push puts one trice into trice fifo.
//! This is a trice time critical part.
//! \param v trice id with 2 byte data
TRICE_INLINE void triceU32Push(uint32_t v) {
    triceU32Fifo[triceU32FifoWriteIndex++] = v;
    triceU32FifoWriteIndex &= TRICE_U32_FIFO_MASK;
}

//! triceU8Push puts one byte into trice fifo.
//! This is a trice time critical part.
//! \param v trice id with 2 byte data
TRICE_INLINE void triceU8Push(uint8_t v) {
    triceU8Fifo[triceU8FifoWriteIndex++] = v;
    triceU8FifoWriteIndex &= TRICE_U8_FIFO_MASK;
}

//! triceU32Pop gets one trice from trice fifo.
//! \return trice id with 2 byte data in one uint32_t.
TRICE_INLINE uint32_t triceU32Pop(void) {
    uint32_t v = triceU32Fifo[triceU32FifoReadIndex++];
    triceU32FifoReadIndex &= TRICE_U32_FIFO_MASK;
    return v;
}


int triceU32FifoDepth(void);

int triceU32WriteU8ReadFifoDepth(void);
*/


//! triceU8Pop gets one trice from trice fifo.
//! \return trice id with 2 byte data in one uint32_t.
TRICE_INLINE uint8_t triceU8Pop(void) {
    uint8_t v = triceU8Fifo[triceU8FifoReadIndex++];
    triceU8FifoReadIndex &= TRICE_U8_FIFO_MASK;
    return v;
}



//! triceServeTransmit as triceServeU8FifoTransmit must be called cyclically to proceed ongoing write out.
//! A good place is UART ISR.
TRICE_INLINE void triceServeTransmit(void) {
    triceTransmitData8(triceU8Pop());
    if (0 == triceU8FifoDepth()) { // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}

// triceTriggerTransmit as triceTriggerU8FifoTransmit must be called cyclically to initialize write out.
TRICE_INLINE void triceTriggerTransmit(void){
    if( triceU8FifoDepth() && triceTxDataRegisterEmpty() ){
        triceEnableTxEmptyInterrupt(); // next bytes
    }
}


#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
