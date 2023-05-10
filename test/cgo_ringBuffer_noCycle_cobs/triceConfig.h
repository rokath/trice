/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//! TriceStamp16 returns a 16-bit value to stamp `Id` TRICE macros. Usually it is a timestamp, but could also be a destination address or a counter for example.
//! The user has to provide this function. Defining a macro here, instead if providing `int16_t TriceStamp16( void );` has significant speed impact.
#define TriceStamp16() (0x1616) 

//! TriceStamp32 returns a 32-bit value to stamp `ID` TRICE macros. Usually it is a timestamp, but could also be a destination address or a counter for example.
//! The user has to provide this function. Defining a macro here, instead if providing `int32_t TriceStamp32( void );` has significant speed impact.
#define TriceStamp32() (0x32323232)

//!  TRICE_BUFFER selects, where the TRICE macros accumulate the trice data during a single TRICE execution. Selectable options:
//! - TRICE_STACK_BUFFER: No additional buffer is needed, what makes sense for single task systems with direct output only.
//! - TRICE_STATIC_BUFFER: A single trice is stored in a separate static buffer, what makes sense in single- and multi-tasking systems with direct output only.
//! - TRICE_DOUBLE_BUFFER: TRICE macros write direct into a double buffer without any additional management action.
//!   This is the fastest execution option for TRICE macros but needs more RAM. Used for deferred output and optional additional direct output.
//! - TRICE_RING_BUFFER: TRICE macros write direct into a ring buffer without any additional management action.
//!   This is not the fastest execution option for TRICE macros but needs less RAM. Used for deferred output and optional additional direct output.
//! If unsure select TRICE_DOUBLE_BUFFER. The TRICE_RING_BUFFER option works, but is experimental.
#define TRICE_BUFFER TRICE_RING_BUFFER

//! TRICE_DIRECT_OUTPUT == 0: only deferred output, usually UART output only
//! TRICE_DIRECT_OUTPUT == 1: with direct output, usually RTT
//! Setting TRICE_BUFFER to TRICE_STACK_BUFFER or TRICE_STATIC_BUFFER demands TRICE_DIRECT_OUTPUT == 1, no deferred output at all.
//! When TRICE_BUFFER == TRICE_RING_BUFFER or TRICE_BUFFER == TRICE_DOUBLE_BUFFER for deferred output, additional direct output can be switched on here.
//! For example it is possible to have direct 32-bit wise RTT TRICE_FRAMING_NONE output and deferred UART TRICE_FRAMING_COBS output.
//! TRICE_BUFFER == TRICE_STACK_BUFFER or TRICE_BUFFER TRICE_STATIC_BUFFER needs TRICE_DIRECT_OUTPUT == 1.
#define TRICE_DIRECT_OUTPUT 1

//! TRICE_DATA_OFFSET is the space in front of single trice data for in-buffer (T)COBS encoding.
//! - When using real big buffers, 16 may be not enough.
//! - When having only short trices but lots of trice bursts, it may make sense to reduce this value to 4.
//! - Without encoding/framing this value can be 0.
#define TRICE_DATA_OFFSET 16 // must be a multiple of 4

//! TRICE_SINGLE_MAX_SIZE is used to truncate long dynamically generated strings and to detect the need of a ring buffer wrap.
//! - Be careful with this value: When using 12 64-bit values with a 32-bit stamp the trice size is 2(id) + 4(stamp) + 2(count) + 12*8(values) = 104 bytes.
//! - In direct mode, and also when you enabled TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE, this plus TRICE_DATA_OFFSET is the max allocation size on the target stack with TRICE_BUFFER == TRICE_STACK_BUFFER.
//! - When short of RAM and, for example, max 2 32-bit values with a 32-bit stamp are used, the max trice size is 2 + 4 + 2 + 2*4 = 16 bytes.
//! - You should then also disable all then forbidden trices to avoid mistakes. Example: `#define ENABLE_TRice32fn_3 0` and so on at the end of this file.
#define TRICE_SINGLE_MAX_SIZE 112 // must be a multiple of 4

//! TRICE_DEFERRED_BUFFER_SIZE needs to be capable to hold trice bursts until they are transmitted.
//! When TRICE_BUFFER == TRICE_STACK_BUFFER this value is not used.
//! When TRICE_BUFFER == TRICE_STATIC_BUFFER this value is not used.
//! When TRICE_BUFFER == TRICE_DOUBLE_BUFFER, this is the sum of both half buffers. 
//! When TRICE_BUFFER == TRICE_RING_BUFFER, this is the whole buffer. 
#define TRICE_DEFERRED_BUFFER_SIZE 2048 // must be a multiple of 4

//! TRICE_MCU_IS_BIG_ENDIAN needs to be defined for TRICE64 macros on big endian MCUs.
//#define TRICE_MCU_IS_BIG_ENDIAN 

//! TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN can be defined on little endian MCUs if the trice data are needed in network order,
//! or on big endian MCUs if the trice data are needed in little endian order. You should avoid using this macro because
//! it increases the trice storage time and the needed code amount. 
//#define TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN

//! TRICE_DIRECT_OUT_FRAMING defines the framing method of the binary trice data stream for direct output. Options: 
//! - TRICE_FRAMING_TCOBS: Recommended for internal transfer and trice tool visualization.
//! - TRICE_FRAMING_COBS: The trice tool needs switch `-pf COBS`. Useful with XTEA or to decode the binary trice data with a user tool.
//! - TRICE_FRAMING_NONE: The trice tool needs switch `-pf none`. TRICE_FRAMING_NONE is needed for fast RTT (32-bit access), recommended.
//! - With TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1 or TRICE_SEGGER_RTT_BIT_WRITE_DIRECT_WITHOUT_FRAMING == 1,
//!   the RTT data arrive unframed ignoring the TRICE_DIRECT_OUT_FRAMING setting here.
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_NONE

//! TRICE_DEFERRED_OUT_FRAMING defines the framing method of the binary trice data stream for deferred output. Options: 
//! - TRICE_FRAMING_TCOBS: Recommended for UART transfer and trice tool visualization.
//! - TRICE_FRAMING_COBS: The trice tool needs switch `-pf COBS`. Useful with XTEA or to decode the binary trice date with Python or an other language.
//! - TRICE_FRAMING_NONE: The trice tool needs switch `-pf none`. This mode may be helpful if you write your own trice viewer without a decoder.
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

// XTEA_ENCRYPT_KEY, when defined, enables XTEA TriceEncryption  with the key. (experimental)
//#define XTEA_ENCRYPT_KEY XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret

//! XTEA_DECRYPT, when defined, enables device local decryption. Usable for checks.
//#define XTEA_DECRYPT

//! With TRICE_DIAGNOSTICS == 0, additional trice diagnostics code is removed. 
#define TRICE_DIAGNOSTICS 1

//! TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1 speeds up RTT transfer by using function SEGGER_Write_RTT0_NoCheck32.
//! - This setting results in unframed RTT trice packages and requires the `-packageFraming none` switch for the appropriate trice tool instance.
//!   This squeezes the whole TRICE macro into about 100 processor clocks leaving the data already inside the SEGGER _acUpBuffer.
//! - If you do not wish RTT, or with RTT with framing, simply set this value to 0. 
#define TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE 0 

//! Enable and set UARTA for deferred serial output.
//#define TRICE_UARTA USART2 // comment out, if you do not use TRICE_UARTA
//#define TRICE_UARTA_MIN_ID 1           //!< TRICE_UARTA_MIN_ID is the smallest ID transferred to UARTA. Define with TRICE_UARTA_MAX_ID if you want select trice output here.
//#define TRICE_UARTA_MAX_ID ((1<<14)-1) //!< TRICE_UARTA_MAX_ID is the biggest  ID transferred to UARTA. Define with TRICE_UARTA_MIN_ID if you want select trice output here.

//! CGO interface (for testing the target code with Go only, do not enable)
#define TRICE_CGO 
#define TRICE_CYCLE_COUNTER 0

//! This is usable as the very first trice sequence after restart. Adapt it. Use a UTF-8 capable editor like VS-Code.
#define TRICE_HEADLINE \
        trice( iD( 7612), "\n\n        ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨        \n🎈🎈🎈🎈       CGO-Test       🎈🎈🎈🎈\n        🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃\n\n\n");

// Compiler Adaption:

//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro empty for more speed. Use only '{' in that case.
//! #define TRICE_ENTER_CRITICAL_SECTION { SEGGER_RTT_LOCK() { - does the job for many compilers.
//! #define TRICE_ENTER_CRITICAL_SECTION { 
//! #define TRICE_ENTER_CRITICAL_SECTION { uint32_t old_mask = cm_mask_interrupts(1); { // copied from test/OpenCM3_STM32F411_Nucleo/triceConfig.h
//! #define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {
#define TRICE_ENTER_CRITICAL_SECTION {  

//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro empty for more speed. Use only '}' in that case.
//! #define TRICE_LEAVE_CRITICAL_SECTION } SEGGER_RTT_UNLOCK() } - does the job for many compilers.
//! #define TRICE_LEAVE_CRITICAL_SECTION } 
//! #define TRICE_LEAVE_CRITICAL_SECTION } cm_mask_interrupts(old_mask); } // copied from test/OpenCM3_STM32F411_Nucleo/triceConfig.h
//! #define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }
#define TRICE_LEAVE_CRITICAL_SECTION }  

#define TRICE_INLINE static inline //! used for trice code

// hardware interface:

//#include "main.h" // hardware specific definitions

TRICE_INLINE void ToggleOpticalFeedbackLED( void ){
//    LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

#ifdef TRICE_UARTA

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmptyUartA(void) {
    return LL_USART_IsActiveFlag_TXE(TRICE_UARTA);
}

//! Write value v into trice transmit register.
//! \param v byte to transmit
//! User must provide this function.
TRICE_INLINE void triceTransmitData8UartA(uint8_t v) {
    LL_USART_TransmitData8(TRICE_UARTA, v);
    ToggleOpticalFeedbackLED();
}

//! Allow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceEnableTxEmptyInterruptUartA(void) {
    LL_USART_EnableIT_TXE(TRICE_UARTA);
}

//! Disallow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceDisableTxEmptyInterruptUartA(void) {
    LL_USART_DisableIT_TXE(TRICE_UARTA);
}
#endif // #ifdef TRICE_UARTA

#ifdef TRICE_UARTB

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmptyUartB(void) {
    return LL_USART_IsActiveFlag_TXE(TRICE_UARTB);
}

//! Write value v into trice transmit register.
//! \param v byte to transmit
//! User must provide this function.
TRICE_INLINE void triceTransmitData8UartB(uint8_t v) {
    LL_USART_TransmitData8(TRICE_UARTB, v);
    ToggleOpticalFeedbackLED();
}

//! Allow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceEnableTxEmptyInterruptUartB(void) {
    LL_USART_EnableIT_TXE(TRICE_UARTB);
}

//! Disallow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceDisableTxEmptyInterruptUartB(void) {
    LL_USART_DisableIT_TXE(TRICE_UARTB);
}
#endif // #ifdef TRICE_UARTB

#define TRICE_8_BIT_SUPPORT  1
#define TRICE_16_BIT_SUPPORT 1
#define TRICE_32_BIT_SUPPORT 1
#define TRICE_64_BIT_SUPPORT 1

// See trice/doc/TriceProjectImageSizeOptimization.md for details:

#if TRICE_8_BIT_SUPPORT

// without stamp 8-bit values functions
#define ENABLE_trice8fn_0  1
#define ENABLE_trice8fn_1  1
#define ENABLE_trice8fn_2  1
#define ENABLE_trice8fn_3  1
#define ENABLE_trice8fn_4  1
#define ENABLE_trice8fn_5  1
#define ENABLE_trice8fn_6  1
#define ENABLE_trice8fn_7  1
#define ENABLE_trice8fn_8  1
#define ENABLE_trice8fn_9  1
#define ENABLE_trice8fn_10 1
#define ENABLE_trice8fn_11 1
#define ENABLE_trice8fn_12 1

// with 16-bit stamp 8-bit values functions
#define ENABLE_Trice8fn_0  1
#define ENABLE_Trice8fn_1  1
#define ENABLE_Trice8fn_2  1
#define ENABLE_Trice8fn_3  1
#define ENABLE_Trice8fn_4  1
#define ENABLE_Trice8fn_5  1
#define ENABLE_Trice8fn_6  1
#define ENABLE_Trice8fn_7  1
#define ENABLE_Trice8fn_8  1
#define ENABLE_Trice8fn_9  1
#define ENABLE_Trice8fn_10 1
#define ENABLE_Trice8fn_11 1
#define ENABLE_Trice8fn_12 1

// with 32-bit stamp 8-bit values functions
#define ENABLE_TRice8fn_0  1
#define ENABLE_TRice8fn_1  1
#define ENABLE_TRice8fn_2  1
#define ENABLE_TRice8fn_3  1
#define ENABLE_TRice8fn_4  1
#define ENABLE_TRice8fn_5  1
#define ENABLE_TRice8fn_6  1
#define ENABLE_TRice8fn_7  1
#define ENABLE_TRice8fn_8  1
#define ENABLE_TRice8fn_9  1
#define ENABLE_TRice8fn_10 1
#define ENABLE_TRice8fn_11 1
#define ENABLE_TRice8fn_12 1

#endif // #if TRICE_8_BIT_SUPPORT

#if TRICE_16_BIT_SUPPORT

// without stamp 16-bit values functions
#define ENABLE_trice16fn_0  1
#define ENABLE_trice16fn_1  1
#define ENABLE_trice16fn_2  1
#define ENABLE_trice16fn_3  1
#define ENABLE_trice16fn_4  1
#define ENABLE_trice16fn_5  1
#define ENABLE_trice16fn_6  1
#define ENABLE_trice16fn_7  1
#define ENABLE_trice16fn_8  1
#define ENABLE_trice16fn_9  1
#define ENABLE_trice16fn_10 1
#define ENABLE_trice16fn_11 1
#define ENABLE_trice16fn_12 1

// with 16-bit stamp 16-bit values functions
#define ENABLE_Trice16fn_0  1
#define ENABLE_Trice16fn_1  1
#define ENABLE_Trice16fn_2  1
#define ENABLE_Trice16fn_3  1
#define ENABLE_Trice16fn_4  1
#define ENABLE_Trice16fn_5  1
#define ENABLE_Trice16fn_6  1
#define ENABLE_Trice16fn_7  1
#define ENABLE_Trice16fn_8  1
#define ENABLE_Trice16fn_9  1
#define ENABLE_Trice16fn_10 1
#define ENABLE_Trice16fn_11 1
#define ENABLE_Trice16fn_12 1

// with 32-bit stamp 16-bit values functions
#define ENABLE_TRice16fn_0  1
#define ENABLE_TRice16fn_1  1
#define ENABLE_TRice16fn_2  1
#define ENABLE_TRice16fn_3  1
#define ENABLE_TRice16fn_4  1
#define ENABLE_TRice16fn_5  1
#define ENABLE_TRice16fn_6  1
#define ENABLE_TRice16fn_7  1
#define ENABLE_TRice16fn_8  1
#define ENABLE_TRice16fn_9  1
#define ENABLE_TRice16fn_10 1
#define ENABLE_TRice16fn_11 1
#define ENABLE_TRice16fn_12 1

#endif // #if TRICE_16_BIT_SUPPORT

#if TRICE_32_BIT_SUPPORT

// without stamp 32-bit values functions
#define ENABLE_trice32fn_0  1
#define ENABLE_trice32fn_1  1
#define ENABLE_trice32fn_2  1
#define ENABLE_trice32fn_3  1
#define ENABLE_trice32fn_4  1
#define ENABLE_trice32fn_5  1
#define ENABLE_trice32fn_6  1
#define ENABLE_trice32fn_7  1
#define ENABLE_trice32fn_8  1
#define ENABLE_trice32fn_9  1
#define ENABLE_trice32fn_10 1
#define ENABLE_trice32fn_11 1
#define ENABLE_trice32fn_12 1

// with 16-bit stamp 32-bit values functions
#define ENABLE_Trice32fn_0  1
#define ENABLE_Trice32fn_1  1
#define ENABLE_Trice32fn_2  1
#define ENABLE_Trice32fn_3  1
#define ENABLE_Trice32fn_4  1
#define ENABLE_Trice32fn_5  1
#define ENABLE_Trice32fn_6  1
#define ENABLE_Trice32fn_7  1
#define ENABLE_Trice32fn_8  1
#define ENABLE_Trice32fn_9  1
#define ENABLE_Trice32fn_10 1
#define ENABLE_Trice32fn_11 1
#define ENABLE_Trice32fn_12 1

// with 32-bit stamp 32-bit values functions
#define ENABLE_TRice32fn_0  1
#define ENABLE_TRice32fn_1  1
#define ENABLE_TRice32fn_2  1
#define ENABLE_TRice32fn_3  1
#define ENABLE_TRice32fn_4  1
#define ENABLE_TRice32fn_5  1
#define ENABLE_TRice32fn_6  1
#define ENABLE_TRice32fn_7  1
#define ENABLE_TRice32fn_8  1
#define ENABLE_TRice32fn_9  1
#define ENABLE_TRice32fn_10 1
#define ENABLE_TRice32fn_11 1
#define ENABLE_TRice32fn_12 1

#endif // #if TRICE_32_BIT_SUPPORT

#if TRICE_64_BIT_SUPPORT

// without stamp 64-bit values functions
#define ENABLE_trice64fn_0  1
#define ENABLE_trice64fn_1  1
#define ENABLE_trice64fn_2  1
#define ENABLE_trice64fn_3  1
#define ENABLE_trice64fn_4  1
#define ENABLE_trice64fn_5  1
#define ENABLE_trice64fn_6  1
#define ENABLE_trice64fn_7  1
#define ENABLE_trice64fn_8  1
#define ENABLE_trice64fn_9  1
#define ENABLE_trice64fn_10 1
#define ENABLE_trice64fn_11 1
#define ENABLE_trice64fn_12 1

// with 16-bit stamp 64-bit values functions
#define ENABLE_Trice64fn_0  1
#define ENABLE_Trice64fn_1  1
#define ENABLE_Trice64fn_2  1
#define ENABLE_Trice64fn_3  1
#define ENABLE_Trice64fn_4  1
#define ENABLE_Trice64fn_5  1
#define ENABLE_Trice64fn_6  1
#define ENABLE_Trice64fn_7  1
#define ENABLE_Trice64fn_8  1
#define ENABLE_Trice64fn_9  1
#define ENABLE_Trice64fn_10 1
#define ENABLE_Trice64fn_11 1
#define ENABLE_Trice64fn_12 1

// with 32-bit stamp 64-bit values functions
#define ENABLE_TRice64fn_0  1
#define ENABLE_TRice64fn_1  1
#define ENABLE_TRice64fn_2  1
#define ENABLE_TRice64fn_3  1
#define ENABLE_TRice64fn_4  1
#define ENABLE_TRice64fn_5  1
#define ENABLE_TRice64fn_6  1
#define ENABLE_TRice64fn_7  1
#define ENABLE_TRice64fn_8  1
#define ENABLE_TRice64fn_9  1
#define ENABLE_TRice64fn_10 1
#define ENABLE_TRice64fn_11 1
#define ENABLE_TRice64fn_12 1

#endif // #if TRICE_32_BIT_SUPPORT

//
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
