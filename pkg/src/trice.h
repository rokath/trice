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
// Compiler Adaption
//
#if defined( __GNUC__ ) /* gnu compiler ###################################### */ \
 || defined(__IAR_SYSTEMS_ICC__) /* IAR compiler ############################# */ \
 || defined(__TASKING__) /* TASKING compiler (same bugs as GNU!)############## */

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4                                  //!< align to 4 byte boundary preamble
#define ALIGN4_END __attribute__ ((aligned(4))) //!< align to 4 byte boundary post declaration

//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
#define TRICE_ENTER_CRITICAL_SECTION { // to do

//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
#define TRICE_LEAVE_CRITICAL_SECTION } // to do

#elif defined(__arm__) // ARMkeil IDE #########################################

#include <cmsis_armcc.h>

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4 __align(4) //!< align to 4 byte boundary preamble
#define ALIGN4_END        //!< align to 4 byte boundary post declaration
//#define PACKED __packed   //!< pack data preamble
//#define PACKED_END        //!< pack data post declaration

//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro empty for more speed. Use only '{' in that case.
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {

//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro pair empty for more speed. Use only '}' in that case.
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

#elif 1 // ####################################################################
#error "add new compiler here"
#else // ######################################################################
#error unknown compliler
#endif // compiler adaptions ##################################################
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Declarations and Defaults

#define TRICE_LITTLE_ENDIANNESS 0x00112233 //!< TRICE_LITTLE_ENDIANNESS is the default for TRICE_HARDWARE_ENDIANNESS and TRICE_TRANSFER_ENDIANNESS.
#define TRICE_BIG_ENDIANNESS    0x33221100 //!< TRICE_BIG_ENDIANNESS is the option for TRICE_HARDWARE_ENDIANNESS and TRICE_TRANSFER_ENDIANNESS.

#ifndef TRICE_CYCLE_COUNTER
#define TRICE_CYCLE_COUNTER 1 //! TRICE_CYCLE_COUNTER adds a cycle counter to each trice message. The TRICE macros are a bit slower. Lost TRICEs are detectable by the trice tool.
#endif

#ifndef TRICE_SINGLE_MAX_SIZE
#define TRICE_SINGLE_MAX_SIZE 1008 //!< TRICE_SINGLE_MAX_SIZE ist the head size plus string length size plus max dynamic string size. Must be a multiple of 4. 1008 is the max allowed value.
#endif

#ifndef TRICE_TRANSFER_INTERVAL_MS
//! TRICE_TRANSFER_INTERVAL_MS is the milliseconds interval for TRICE buffer read out.
//! This time should be shorter than visible delays. The TRICE_HALF_BUFFER_SIZE must be able to hold all trice messages possibly occouring in this time.
#define TRICE_TRANSFER_INTERVAL_MS 10
#endif

#if TRICE_SINGLE_MAX_SIZE < 256
#define TRICE_DATA_OFFSET 4 //! TRICE_DATA_OFFSET is the space in front of trice data for in-buffer COBS encoding. It be a multiple of uint32_t.
#else
#define TRICE_DATA_OFFSET 8 //! TRICE_DATA_OFFSET is the space in front of trice data for in-buffer COBS encoding. It be a multiple of uint32_t.
#endif

extern unsigned TriceDepthMax;
unsigned TriceCOBSEncode( uint8_t* restrict output, const uint8_t * restrict input, unsigned length);
void TriceTransfer( void );
void triceCheckSet( int index ); //!< tests

//
///////////////////////////////////////////////////////////////////////////////

#define PUT(x) do{ *wTb++ = x; }while(0) //! PUT copies a 32 bit x into the TRICE buffer.
#define PUT_BUFFER( buf, len ) do{ memcpy( wTb, buf, len ); wTb += (len+3)>>2; }while(0) //! PUT_BUFFER copies a buffer into the TRICE buffer.

#ifdef ENCRYPT // to do
// #define DECRYPT //!< usually not needed
void triceServeFifoEncryptedToBytesBuffer(void);
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
#ifdef TRICE_UART
#include "main.h" // hardware specific stuff

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

#endif // #ifdef TRICE_STM32
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Endianness
//
#ifndef TRICE_HARDWARE_ENDIANNESS
//! Set endianess according to target hardware. Options: TRICE_BIG_ENDIANNESS, TRICE_LITTLE_ENDIANNESS.
//! Some compiler offer an automatic detection for this.
#define TRICE_HARDWARE_ENDIANNESS TRICE_LITTLE_ENDIANNESS 
#endif

#ifndef TRICE_TRANSFER_ENDIANNESS
//! Set byte order according desired transfer format. Options: TRICE_BIG_ENDIANNESS, TRICE_LITTLE_ENDIANNESS. 
//! TRICE_BIG_ENDIANNESS is network order.
//! If TRICE_TRANSFER_ENDIANNESS is equal to TRICE_HARDWARE_ENDIANNESS the trice code is smaller and more efficient.
//! When set to TRICE_LITTLE_ENDIANNESS the trice tool -encoding format specifier is extended by a letter 'L'.
//! Example -encoding "flexL".
#define TRICE_TRANSFER_ENDIANNESS TRICE_LITTLE_ENDIANNESS 
#endif
//
///////////////////////////////////////////////////////////////////////////////

#ifdef TRICE_UART
int TriceNonBlockingWrite( void const * buf, int nByte );
int TriceWriteOutDepth( void );
uint8_t TriceNextUint8( void );

//! triceServeTransmit as triceServeU8FifoTransmit must be called cyclically to proceed ongoing write out.
//! A good place is UART ISR.
TRICE_INLINE void triceServeTransmit(void) {
    triceTransmitData8(TriceNextUint8());
    if (0 == TriceWriteOutDepth()) { // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}

// triceTriggerTransmit as triceTriggerU8FifoTransmit must be called cyclically to initialize write out.
TRICE_INLINE void triceTriggerTransmit(void){
    if( TriceWriteOutDepth() && triceTxDataRegisterEmpty() ){
        triceEnableTxEmptyInterrupt(); // next bytes
    }
}
#endif // #ifdef TRICE_UART

///////////////////////////////////////////////////////////////////////////////
// Encryption
//
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
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Variadic macros
//
#define TRICE8_COUNT(_1,_2,_3,_4,_5,_6,_7,_8, NAME,...) NAME
#define TRICE8(id,frmt, ...) TRICE8_COUNT(__VA_ARGS__,TRICE8_8,TRICE8_7,TRICE8_6,TRICE8_5,TRICE8_4,TRICE8_3,TRICE8_2,TRICE8_1)(id,frmt, __VA_ARGS__)

#define TRICE16_COUNT(_1,_2,_3,_4, NAME,...) NAME
#define TRICE16(id,frmt, ...) TRICE16_COUNT(__VA_ARGS__,TRICE16_4,TRICE16_3,TRICE16_2,TRICE16_1)(id,frmt, __VA_ARGS__)

#define TRICE32_COUNT(_1,_2,_3,_4, NAME,...) NAME
#define TRICE32(id,frmt, ...) TRICE32_COUNT(__VA_ARGS__,TRICE32_4,TRICE32_3,TRICE32_2,TRICE32_1)(id,frmt, __VA_ARGS__)

#define TRICE64_COUNT(_1,_2,_3,_4, NAME,...) NAME
#define TRICE64(id,frmt, ...) TRICE64_COUNT(__VA_ARGS__,TRICE64_4,TRICE64_3,TRICE64_2,TRICE64_1)(id,frmt, __VA_ARGS__)
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// TRICE macros
//
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
#else // #ifdef TRICE_OFF

#include <stdint.h>
#include <string.h>

extern uint32_t* wTb;

#define Id(n) ((uint32_t)n<<16) //!< Id() is a 16 bit id 0-65535 as upper 2 bytes in head

#if TRICE_CYCLE_COUNTER == 1
extern uint8_t TriceCycle;
#define TRICE_CYCLE TriceCycle++ //! TRICE_CYCLE is the trice cycle counter as 8 bit count 0-255.
#else
#define TRICE_CYCLE 0xC0 //! TRICE_CYCLE is no trice cycle counter, just a static value.
#endif

#if 1 // uint32_t count
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
    PUT( id | 0x0100 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) ); \
    TRICE_LEAVE

//! TRICE8_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v2 are 8 bit bit values
#define TRICE8_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER \
    PUT( id | 0x0100 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) ); \
    TRICE_LEAVE

//! TRICE8_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v3 are 8 bit bit values
#define TRICE8_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    PUT( id | 0x0100 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    TRICE_LEAVE

//! TRICE8_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v4 are 8 bit bit values
#define TRICE8_5( id, pFmt, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    PUT( id | 0x0200 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    PUT( (uint8_t)(v4) ); \
    TRICE_LEAVE

//! TRICE8_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v5 are 8 bit bit values
#define TRICE8_6( id, pFmt, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    PUT( id | 0x0200 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v6 are 8 bit bit values
#define TRICE8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    PUT( id | 0x0200 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) | ((uint32_t)(0xff&(v6))<<16) ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    PUT( id | 0x0200 | TRICE_CYCLE ); \
    PUT( (uint8_t)(v0) | ((uint16_t)(v1)<<8) | ((uint32_t)(0xff&(v2))<<16) | ((uint32_t)(v3)<<24) ); \
    PUT( (uint8_t)(v4) | ((uint16_t)(v5)<<8) | ((uint32_t)(0xff&(v6))<<16) | ((uint32_t)(v7)<<24) ); \
    TRICE_LEAVE

//! TRICE16_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 a 16 bit value
#define TRICE16_1( id, pFmt, v0 ) \
    TRICE_ENTER \
    PUT( id | 0x0100 | TRICE_CYCLE ); \
    PUT( (uint16_t)(v0) ); \
    TRICE_LEAVE

//! TRICE16_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v1 are 16 bit values
#define TRICE16_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER \
    PUT( id | 0x0100 | TRICE_CYCLE ); \
    PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    TRICE_LEAVE

//! TRICE16_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v2 are 16 bit values
#define TRICE16_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER \
    PUT( id | 0x0200 | TRICE_CYCLE ); \
    PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    PUT( (uint16_t)(v2) ); \
    TRICE_LEAVE

//! TRICE16_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v3 are 16 bit values
#define TRICE16_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    PUT( id | 0x0200 | TRICE_CYCLE ); \
    PUT((uint16_t)(v0) | ((uint32_t)(v1)<<16) ); \
    PUT((uint16_t)(v2) | ((uint32_t)(v3)<<16) ); \
    TRICE_LEAVE

//! TRICE32_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 the 32 bit value
#define TRICE32_1( id, pFmt, v0 ) \
    TRICE_ENTER \
    PUT( id | 0x0100 | TRICE_CYCLE); \
    PUT( (uint32_t)(v0) ); /* little endian*/ \
    TRICE_LEAVE

//! TRICE32_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v1 are 32 bit values
#define TRICE32_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER \
    PUT(id | 0x0200 | TRICE_CYCLE ); \
    PUT( (uint32_t)(v0) ); \
    PUT( (uint32_t)(v1) ); \
    TRICE_LEAVE

//! TRICE32_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v2 are 32 bit values
#define TRICE32_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER \
    PUT( id | 0x0300 | TRICE_CYCLE ); \
    PUT( (uint32_t)(v0) ); \
    PUT( (uint32_t)(v1) ); \
    PUT( (uint32_t)(v2) ); \
    TRICE_LEAVE

//! TRICE32_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v3 are 32 bit values
#define TRICE32_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    PUT( id | 0x0400 | TRICE_CYCLE ); \
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
    PUT( id | 0x0200 | TRICE_CYCLE ); \
    PUT( (uint32_t)(v0) ); \
    PUT( (uint32_t)((uint64_t)(v0)>>32) ); \
    TRICE_LEAVE

//! TRICE64_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id
//! \param v0 - v1 are 64 bit values
#define TRICE64_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER \
    PUT( id | 0x0400 | TRICE_CYCLE ); \
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
    PUT( id | 0x0600 | TRICE_CYCLE ); \
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
    PUT( id | 0x0800 | TRICE_CYCLE ); \
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
#error to do TRICE_HARDWARE_ENDIANNESS == TRIC_EIG_ENDIANNESS
#endif // #else // #if TRICE_HARDWARE_ENDIANNESS == TRICE_LITTLE_ENDIANNESS
#if 0
#define TRICE8P( id, pFmt, buf, len) do { \
    if( len <= 0 ){ \
        break; \
    } \
    if( len > 1000 ){ \
        len = 1000; \
    } \
    TRICE_ENTER \
    PUT( id | (0xff00 & ((len+4)<<6)) | TRICE_CYCLE ); /* +4 for the buf size value transmitted in the payload to get the last 2 bits. */ \
    PUT( len ); /* len does not contain the exact buf len anymore, so transmit it to the host */ \
           /* len is needed for non string buffers because the last 2 bits not stored in head. */ \
                                           /* All trices know the data length but not TRICE8B. */ \
    PUT_BUFFER( buf, len ); \
    TRICE_LEAVE \
} while(0)
#endif

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
    if( len > TRICE_SINGLE_MAX_SIZE-8 ){ \
        dynString[TRICE_SINGLE_MAX_SIZE-8] = 0; \
        len = TRICE_SINGLE_MAX_SIZE-8; \
    } \
    TRICE_ENTER \
    PUT( id | (0xff00 & ((len+7)<<6)) | TRICE_CYCLE ); /* +3 for padding, +4 for the buf size value transmitted in the payload to get the last 2 bits. */ \
    PUT( len ); /* len as byte does not contain the exact buf len anymore, so transmit it to the host */ \
    /* len is needed for non string buffers because the last 2 bits not stored in head. */ \
    /* All trices know the data length but not TRICE8P. len byte values 0xFC, xFD, xFE, xFF are reserved for future extensions. */ \
    PUT_BUFFER( dynString, len ); \
    TRICE_LEAVE \
} while(0)

#else // #if 1 // uint32_t count
#endif // #else // #if 1 // uint32_t count
#endif // #else // #ifdef TRICE_OFF



#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
