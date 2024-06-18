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
#define TriceStamp16() (0x1616) //(SysTick->VAL) 

//! TriceStamp32 returns a 32-bit value to stamp `ID` TRICE macros. Usually it is a timestamp, but could also be a destination address or a counter for example.
//! The user has to provide this function. Defining a macro here, instead if providing `int32_t TriceStamp32( void );` has significant speed impact.
#define TriceStamp32() (0x32323232) //Us32()

//! The TRICE_PROTECT switch is only relevant for the deferred trice modes TRICE_DOUBLE_BUFFER and TRICE_RING_BUFFER.
//! The trice library works well, when less data are produced in the average than transmitable and when in the double buffer case the TriceTransfer
//! function is called before too much data in a half buffer according to a good configuration. If that is guarantied you do not need to enable TRICE_PROTECT.
//! If because of an potential error this is not guarantied, you should enable TRICE_PROTECT. This slows down the TRICE macros a bit, but makes buffer overflows impossible.
//! A ring buffer cannot overflow in a first assunmption, because old, not yet transmitted, trices are overwritten by newer ones.
//! But that can happen only to parts of trices. The ring buffer read out function relies on consistent data. If it gets data garbage, wrong values
//! for the trice lengths are possible and buffer overruns not avoidable. When enabling TRICE_PROTECT, new trices are only written into the deferred buffer,
//! if there is enough space. That guaranties data consistency. Because a suppressed trice cannot cause a cycle error, there is TriceOverflowCount as diagnostic value. 
#define TRICE_PROTECT

//!  TRICE_BUFFER selects, where the TRICE macros accumulate the trice data during a single TRICE execution. Selectable options:
//! - TRICE_STACK_BUFFER: No additional buffer is needed, what makes sense for single task systems with direct output only.
//! - TRICE_STATIC_BUFFER: A single trice is stored in a separate static buffer, what makes sense for multi-tasking systems with direct output only.
//! - TRICE_DOUBLE_BUFFER: TRICE macros write direct into a double buffer without any additional management action.
//!   This is the fastest execution option for TRICE macros but needs more RAM. Used for deferred output and optional additional direct output.
//! - TRICE_RING_BUFFER: TRICE macros write direct into a ring buffer without any additional management action.
//!   This is a fast but not the fastest execution option for TRICE macros but needs less RAM. Used for deferred output and optional additional direct output.
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER

//! TRICE_DEFERRED_TRANSFER_MODE is the selected deferred trice transfer method. Options: 
//! - TRICE_SINGLE_PACK_MODE: Each package is followed by a 0-delimiter byte (recommended).
//! - TRICE_MULTI_PACK_MODE packs several trice messages before adding a 0-delimiter byte (reduces transmit byte count).
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#if TRICE_BUFFER == TRICE_RING_BUFFER
//! This is a helper to watch the ring buffer margins.
#define TRICE_RINGBUFFER_OVERFLOW_WATCH
void TriceInitRingBufferMargins( void );
void WatchRingBufferMargins( void );
#endif

//! TRICE_DIRECT_OUTPUT == 0: only deferred output, usually UART output only
//! TRICE_DIRECT_OUTPUT == 1: with direct output, usually RTT
//! Setting TRICE_BUFFER to TRICE_STACK_BUFFER or TRICE_STATIC_BUFFER demands TRICE_DIRECT_OUTPUT == 1, no deferred output at all.
//! When TRICE_BUFFER == TRICE_RING_BUFFER or TRICE_BUFFER == TRICE_DOUBLE_BUFFER for deferred output, additional direct output can be switched on here.
//! For example it is possible to have direct 32-bit wise RTT TRICE_FRAMING_NONE output and deferred UART TRICE_FRAMING_COBS output.
//! TRICE_BUFFER == TRICE_STACK_BUFFER or TRICE_BUFFER == TRICE_STATIC_BUFFER needs TRICE_DIRECT_OUTPUT == 1.
#define TRICE_DIRECT_OUTPUT 1

//! TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING == 1 makes only sense, when TRICE_DIRECT_OUTPUT is 1.
//! Enable this only, if you want only a specific ID ranges for direct Trice output.
//! If you use different direct output channels, you can change this only for all together.
//#define TRICE_DIRECT_OUTPUT_IS_WITH_ROUTING 1

//! TRICE_DATA_OFFSET is the space in front of single trice data for in-buffer (T)COBS encoding.
//! - When using real big buffers, 16 may be not enough.
//! - When having only short trices but lots of trice bursts, it may make sense to reduce this value to 4.
//! - Without encoding/framing this value can be 0.
//! - When using XTEA, this value should incorporate additinal 4 bytes, because of the 64-bit encryption units.
//! With TRICE_BUFFER == TRICE_RING_BUFFER, this amount of space is allocated in front of each single trice!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! With TRICE_BUFFER == TRICE_DOUBLE_BUFFER, this amount of space is allocated once in front of each half buffer.
#define TRICE_DATA_OFFSET 64 // must be a multiple of 4
#define TRICE_RING_BUFFER_DATA_OFFSET 16

//! TRICE_SINGLE_MAX_SIZE is used to truncate long dynamically generated strings, to detect the need of a ring buffer wrap or to protect against overflow.
//! - Be careful with this value: When using 12 64-bit values with a 32-bit stamp the trice size is 2(id) + 4(stamp) + 2(count) + 12*8(values) = 104 bytes.
//! - In direct mode, and also when you enabled TRICE_SEGGER_RTT_8BIT_DEFERRED_WRITE, this plus TRICE_DATA_OFFSET is the max allocation size on the target 
//!   stack with TRICE_BUFFER == TRICE_STACK_BUFFER.
//! - When short of RAM and, for example, max 2 32-bit values with a 32-bit stamp are used, the max trice size is 2 + 4 + 2 + 2*4 = 16 bytes.
//! - You should then also disable all then forbidden trices to avoid mistakes. Example: `#define ENABLE_TRice32fn_3 0` and so on at the end of this file.
//! - When not using dynamic string (or buffer) transfer, bigger values than 104 make no sense here and just spoiling RAM.
#define TRICE_SINGLE_MAX_SIZE 104 // must be a multiple of 4

//! TRICE_DEFERRED_BUFFER_SIZE needs to be capable to hold trice bursts until they are transmitted.
//! When TRICE_BUFFER == TRICE_STACK_BUFFER this value is not used.
//! When TRICE_BUFFER == TRICE_STATIC_BUFFER this value is not used.
//! When TRICE_BUFFER == TRICE_DOUBLE_BUFFER, this is the sum of both half buffers. 
//! When TRICE_BUFFER == TRICE_RING_BUFFER, this is the whole buffer. 
#define TRICE_DEFERRED_BUFFER_SIZE 2048 // must be a multiple of 4

//! TRICE_MCU_IS_BIG_ENDIAN needs to be defined for TRICE64 macros on big endian MCUs for correct 64-bit values and 32-bit timestamp encoding-
//#define TRICE_MCU_IS_BIG_ENDIAN 

//! TRICE_DIRECT_OUT_FRAMING defines the framing method of the binary trice data stream for direct output. Options: 
//! - TRICE_FRAMING_TCOBS: Recommended for internal transfer and trice tool visualization.
//! - TRICE_FRAMING_COBS: The trice tool needs switch `-pf COBS`. Useful with XTEA or to decode the binary trice data with a user tool.
//! - TRICE_FRAMING_NONE: The trice tool needs switch `-pf none`. TRICE_FRAMING_NONE is needed for fast RTT (32-bit access), recommended.
//! - With TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1 or TRICE_SEGGER_RTT_8BIT_WRITE_DIRECT_WITHOUT_FRAMING == 1,
//!   the RTT data arrive unframed ignoring the TRICE_DIRECT_OUT_FRAMING setting here.
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_NONE

//! TRICE_DEFERRED_OUT_FRAMING defines the framing method of the binary trice data stream for deferred output. Options: 
//! - TRICE_FRAMING_TCOBS: Recommended for UART transfer and trice tool visualization.
//! - TRICE_FRAMING_COBS: The trice tool needs switch `-pf COBS`. Useful with XTEA or to decode the binary trice date with Python or an other language.
//! - TRICE_FRAMING_NONE: The trice tool needs switch `-pf none`. This mode may be helpful if you write your own trice viewer without a decoder.
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_TCOBS

//! XTEA_ENCRYPT_KEY, when defined, enables XTEA TriceEncryption with the key.
//! To get your private XTEA_KEY, call just once "trice log -port ... -password YourSecret -showKey".
//! The byte sequence you see then, copy and paste it here.
//#define XTEA_ENCRYPT_KEY XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret

//! XTEA_DECRYPT, when defined, enables device local decryption. Usable for checks or if you use a trice capable node to read XTEA encrypted messages.
//#define XTEA_DECRYPT

//! TRICE_XTEA_DIRECT_ENCRYPT enables encryption for direct output. (experimental)
#define TRICE_XTEA_DIRECT_ENCRYPT 0

//! TRICE_XTEA_DEFERRED_ENCRYPT enables encryption for deferred output. (experimental)
#define TRICE_XTEA_DEFERRED_ENCRYPT 0

//! With TRICE_DIAGNOSTICS == 0, additional trice diagnostics code is removed. 
//! During developmemt TRICE_DIAGNOSTICS == 1 helps to optimize the trice buffer sizes.
#define TRICE_DIAGNOSTICS 1

//! TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE == 1 speeds up RTT transfer by using function SEGGER_Write_RTT0_NoCheck32 and needs ((TRICE_DIRECT_OUTPUT == 1).
//! - This setting results in unframed RTT trice packages and requires the `-packageFraming none` switch for the appropriate trice tool instance.
//!   This squeezes the whole TRICE macro into about 100 processor clocks leaving the data already inside the SEGGER _acUpBuffer.
//! - If you do not wish RTT, or wish RTT with framing, simply set this value to 0.
//! - The trice tool CLI switch -d16 is needed too, because for alignment reasons the 16bit ID field is doubled for 16bit timestamp trice messages.
#define TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE 1

//! TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE==1 uses standard RTT transfer by using function SEGGER_RTT_WriteNoLock and needs ((TRICE_DIRECT_OUTPUT == 1).
//! - This setting results in unframed RTT trice packages and requires the `-packageFraming none` switch for the appropriate trice tool instance.
//! - Not that fast as with TRICE_SEGGER_RTT_32BIT_WRITE == 1 but still fast and uses pure SEGGER functionality only.
#define TRICE_SEGGER_RTT_8BIT_DIRECT_WRITE 0

//! TRICE_DIRECT_AUXILIARY enables a user defined optionally routed direct trice write. (experimental)
#define TRICE_DIRECT_AUXILIARY 0

//! TRICE_DEFERRED_AUXILIARY enables a user defined optionally routed deferred trice write. (experimental)
#define TRICE_DEFERRED_AUXILIARY 0

//! Enable and set UARTA for deferred serial output.
//#define TRICE_UARTA USART2 // comment out, if you do not use TRICE_UARTA
//#define TRICE_UARTA_MIN_ID 1           //!< TRICE_UARTA_MIN_ID is the smallest ID transferred to UARTA. Define with TRICE_UARTA_MAX_ID if you want select trice output here.
//#define TRICE_UARTA_MAX_ID ((1<<14)-1) //!< TRICE_UARTA_MAX_ID is the biggest  ID transferred to UARTA. Define with TRICE_UARTA_MIN_ID if you want select trice output here.

//! CGO interface (for testing the target code with Go only, do not enable)
#define TRICE_CGO 
#define TRICE_CYCLE_COUNTER 0

// Compiler Adaption:

//! USE_SEGGER_RTT_LOCK_UNLOCK_MACROS == 1 includes SEGGER_RTT header files even SEGGER_RTT is not used.
#define USE_SEGGER_RTT_LOCK_UNLOCK_MACROS 0

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
    //LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
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
