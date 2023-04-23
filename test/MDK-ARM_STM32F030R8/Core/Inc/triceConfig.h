/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

//! TRICE_DIRECT_OUT needs to be defined if at least one out channel is used in direct mode (usually RTT).
#define TRICE_DIRECT_OUT

//! TRICE_DEFERRED_OUT needs to be defined if at least one out channel is used in deferred mode (usually UART).
#define TRICE_DEFERRED_OUT

//! TRICE_INTERMEDIATE_BUFFER selects, where the TRICE macros accumultate the trice data during a single TRICE execution.
//! By executing TRICE_LEAVE these data are copied away, so it is just an intermediade storage.
//! Because TRICE supports up to 32767 data bytes, this buffer could get big. Anyway it cannot exeed TRICE_DATA_OFFSET+TRICE_SINGLE_MAX_SIZE.
//! - Selectable options:
//!   - TRICE_STACK_BUFFER means that no additional buffer is needed, what makes sense for single task systems. Direct mode is possible.
//!   - TRICE_STATIC_BUFFER means single trice allocation in a separate static buffer, what makes sense in multi-tasking systems. Direct mode is possible.
//!   - TRICE_DOUBLE_BUFFER no additional buffer nor stack is used for the TRICE macros. They write direct into the double buffer without 
//!     any additional management action. This is the fastest execution option for TRICE macros but needs more RAM. Direct mode is NOT possible.
#define TRICE_INTERMEDIATE_BUFFER TRICE_STATIC_BUFFER

//! TRICE_SINGLE_MAX_SIZE is used to truncate long dynamically generated strings and to detect the need of a stream buffer wrap.
//! - Be careful with this value: When using 12 64-bit values with a 32-bit stamp the trice size is 2(id) + 4(stamp) + 2(count) + 12*8(values) = 104 bytes.
//! - In direct mode, and also when you enabled TRICE_RTT0, this plus TRICE_DATA_OFFSET is the max allocation size on the target stack with TRICE_MODE == TRICE_STACK_BUFFER.
//! - When short of RAM and, for example, max 2 32-bit values with a 32-bit stamp are used, the max trice size is 2 + 4 + 2 + 2*8 = 24 bytes.
//! - You should then also disable all then forbidden trices. Example: `#define ENABLE_TRice32fn_3 0` and so on at the end of this file.
#define TRICE_SINGLE_MAX_SIZE 104 // no need for a power of 2 here

//! TRICE_DATA_OFFSET is the space in front of trice data for in-buffer (T)COBS encoding. It must be be a sizeof(uint32_t) multiple.
//! When using real big buffers, 16 may be not enough.
#define TRICE_DATA_OFFSET 16

//! TRICE_ACCUMULATION_BUFFER is the buffer type used for deferred trices.
//! - TRICE_DOUBLE_BUFFER:
//!   - Double Buffering output to UART. Fastest TRICE macro execution. It is the users reposibility to switch the 2 buffers leke every 10 or 100 milliseconds.
//! - TRICE_STREAM_BUFFER: (legacy)
//!   - Stream Buffering output to UART. Needs less buffer memory for the price of being a bit slower.
//!   - Single Trice buffer address and length do into a fifo during the TRICE macro, which is read out and processed in the background then.
//! - TRICE_DEFERRED_BUFFER
#define TRICE_ACCUMULATION_BUFFER TRICE_DOUBLE_BUFFER

//! TRICE_DEFAULT_PARAMETER_BIT_WIDTH is the default parameter bit width for TRICE macros not specifying the parameter bit width: 8, 16, 32 or 64.
//! If for example the majority of your values is 16 bit, it makes sense to set this value to 16 to use TRICE for them and to use TRICE32 explicitely for 32-bit values.
//! The trice tool CLI switch "-defaultTRICEBitwidth" needs to be set to the same bit width, default is 32.
#define TRICE_DEFAULT_PARAMETER_BIT_WIDTH 32

//! TRICE_B is a shortcut for TRICE8_B, TRICE16_B, TRICE32_B or TRICE64_B usable in your project.
#define TRICE_B  TRICE8_B

//! TRICE_F is a shortcut for TRICE8_F, TRICE16_F, TRICE32_F or TRICE64_F usable in your project.
#define TRICE_F  TRICE8_F


//! Select trice mode for RTT0. Recommended is TRICE_DIRECT_MODE, when RTT0 is the only output device.
//! When using an other output device too, and this in TRICE_DEFERRED_MODE, selecting TRICE_DEFERRED_MODE
//! here reduces the overgead a bit, but TRICE_DIRECT_MODE is still recommeded here.
//! Options:
//! - TRICE_DIRECT_MODE (recommended)
//! - TRICE_DEFERRED_MODE (use only if TRICE_INTERMEDIATE_BUFFER == TRICE_DOUBLE_BUFFER)
#define TRICE_RTT_MODE TRICE_DIRECT_MODE






/**/
///////////////////////////////////////////////////////////////////////////////
// Multi selecet physical out channels, the ID ranges are allowed to overlap.
// 

//! Enable and set channel number for SeggerRTT usage. Only channel 0 is supported right now.
//! RTT is usable in direct mode (recommended) and deferred mode (not recommended) 
#define TRICE_RTT 0 // comment out, if you do not use RTT

//! Select trice mode for RTT0. Recommended is TRICE_DIRECT_MODE, when RTT0 is the only output device.
//! When using an other output device too, and this in TRICE_DEFERRED_MODE, selecting TRICE_DEFERRED_MODE
//! here reduces the overgead a bit, but TRICE_DIRECT_MODE is still recommeded here.
//! Options:
//! - TRICE_DIRECT_MODE (recommended)
//! - TRICE_DEFERRED_MODE (use only if TRICE_INTERMEDIATE_BUFFER == TRICE_DOUBLE_BUFFER)
#define TRICE_RTT_MODE TRICE_DIRECT_MODE

//! TRICE_SEGGER_RTT_32BIT_WRITE==1 speeds up RTT transfer but needs function SEGGER_Write_RTT0_NoCheck32.
//! - This setting is only relevant for TRICE_FRAMING_NONE.
//! - If you do not need extrem speed, simply set this value to 0.
#define TRICE_SEGGER_RTT_32BIT_WRITE 1 

//#define TRICE_RTT_MIN_ID 1           //!< TRICE_RTT0_MIN_ID is the smallest ID transferred to RTT0. Define with TRICE_RTT0_MAX_ID if you want select trice output here. Valid only for TRICE_RTT_MODE == TRICE_DEFERRED_MODE.
//#define TRICE_RTT_MAX_ID ((1<<14)-1) //!< TRICE_RTT0_MAX_ID is the biggest  ID transferred to RTT0. Define with TRICE_RTT0_MIN_ID if you want select trice output here. Valid only for TRICE_RTT_MODE == TRICE_DEFERRED_MODE.

//! Enable and set UARTA for serial output.
#define TRICE_UARTA USART2 // comment out, if you do not use TRICE_UARTA

//! TRICE_UARTA_MODE selects trice mode for UARTA.
//! - TRICE_DEFERRED_MODE is recommendated..
//! - TRICE_DIRECT_MODE could be a choice here for no time constraints like trice inside interrupts and the wish to transfer data immediately.
#define TRICE_UARTA_MODE TRICE_DEFERRED_MODE // TRICE_DIRECT_MODE or TRICE_DEFERRED_MODE

#define TRICE_UARTA_BUFFER TRICE_DOUBLE_BUFFER // TRICE_NO_BUFFER, TRICE_STACK_BUFFER, TRICE_STATIC_BUFFER, TRICE_DOUBLE_BUFFER, TRICE_STREAM_BUFFER


//#define TRICE_UARTA_MIN_ID 1           //!< TRICE_UARTA_MIN_ID is the smallest ID transferred to UARTA. Define with TRICE_UARTA_MAX_ID if you want select trice output here.
//#define TRICE_UARTA_MAX_ID ((1<<14)-1) //!< TRICE_UARTA_MAX_ID is the biggest  ID transferred to UARTA. Define with TRICE_UARTA_MIN_ID if you want select trice output here.


//! Enable and set UARTB for serial output.
//#define TRICE_UARTB USART1 // comment out, if you do not use TRICE_UARTB

//! Select trice mode for UARTB. Recommended is TRICE_DEFERRED_MODE.
//! The TRICE_DIRECT_MODE could be a choice here for no time constraints like trice inside interrupts.
//#define TRICE_UARTB_MODE TRICE_NO_MODE // TRICE_DIRECT_MODE or TRICE_DEFERRED_MODE, set to TRICE_NO_MODE if device not used

#define TRICE_UARTB_BUFFER TRICE_NO_BUFFER // TRICE_NO_BUFFER, TRICE_STACK_BUFFER, TRICE_STATIC_BUFFER, TRICE_DOUBLE_BUFFER, TRICE_STREAM_BUFFER


//#define TRICE_UARTB_MIN_ID 1           //!< TRICE_UARTB_MIN_ID is the smallest ID transferred to UARTB. Define with TRICE_UARTB_MAX_ID if you want select trice output here.
//#define TRICE_UARTB_MAX_ID ((1<<14)-1) //!< TRICE_UARTB_MAX_ID is the biggest  ID transferred to UARTB. Define with TRICE_UARTB_MIN_ID if you want select trice output here.


//! CGO interface (for testing the target code with Go only, do not enable)
//#define TRICE_CGO 

//! Enable option for an additional interface, you can define by your own.
#define TRICE_AUXILIARY

//! Select trice mode for AUXILIARY. 
//! The TRICE_DIRECT_MODE is recommended if the AUXILIARY write function is non blocking, means so fast, to be executed inside an interrupt as well.
//! The TRICE_DERFERRED_MODE is recommended if the AUXILIARY write function is blocking, means so slow, to be not executable inside an interrupt.
//#define TRICE_AUXILIARY_MODE TRICE_NO_MODE // TRICE_DIRECT_MODE or TRICE_DEFERRED_MODE, set to TRICE_NO_MODE if device not used.

#define TRICE_AUXIOLARY_BUFFER TRICE_NO_BUFFER // TRICE_NO_BUFFER, TRICE_STACK_BUFFER, TRICE_STATIC_BUFFER, TRICE_DOUBLE_BUFFER, TRICE_STREAM_BUFFER

//#define TRICE_AUXILIARY_MIN_ID 1           //!< TRICE_AUXILIARY_MIN_ID is the smallest ID transferred to AUXILIARY. Define with TRICE_AUXILIARY_MAX_ID if you want select trice output here.
//#define TRICE_AUXILIARY_MAX_ID ((1<<14)-1) //!< TRICE_AUXILIARY_MAX_ID is the biggest  ID transferred to AUXILIARY. Define with TRICE_AUXILIARY_MIN_ID if you want select trice output here.


/**/


///////////////////////////////////////////////////////////////////////////////
// Select trice mode and general settings.
//

//! TRICE_MODE is a predefined trice transfer method. Options: TRICE_STACK_BUFFER, TRICE_DOUBLE_BUFFER, TRICE_STREAM_BUFFER
//! The TRICE_MODE decision is a general one concering the inner data handling and is (nearly) independent from the out channel(s).
//!

#ifndef TRICE_OFF 
#define TRICE_MODE TRICE_STATIC_BUFFER 
#endif



#if TRICE_MODE == TRICE_STACK_BUFFER
#define TRICE_BUFFER_SIZE (TRICE_SINGLE_MAX_SIZE + TRICE_DATA_OFFSET) //!< TRICE_BUFFER_SIZE is the used additional max stack size for a single TRICE macro. Recommended value: TRICE_SINGLE_MAX_SIZE plus 8.
#elif TRICE_MODE == TRICE_STREAM_BUFFER 
#define TRICE_FIFO_ELEMENTS 128 //!< Must be a power of 2. The half number is the amount of bufferable trices before they go out.
#define TRICE_BUFFER_SIZE 2048 //!< TRICE_BUFFER_SIZE is the used max buffer size for a TRICE macro burst. Recommended value: 2000.
#elif TRICE_MODE == TRICE_STATIC_BUFFER 
//#define TRICE_FIFO_ELEMENTS 128 //!< Must be a power of 2. The half number is the amount of bufferable trices before they go out.
#define TRICE_BUFFER_SIZE 2048 //!< TRICE_BUFFER_SIZE is the used max buffer size for a TRICE macro burst. Recommended value: 2000.
#elif TRICE_MODE == TRICE_DOUBLE_BUFFER 
#define TRICE_BUFFER_SIZE 2048 //!< TRICE_BUFFER_SIZE is the double half buffer size usable for a TRICE macro burst. Recommended value: 2000.
#endif

// Enabling next line results in XTEA TriceEncryption  with the key.
//#define XTEA_ENCRYPT_KEY XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret
//#define XTEA_DECRYPT //!< XTEA_DECRYPT is usually not needed. Enable for checks.

#ifdef XTEA_ENCRYPT_KEY
//! TRICE_FRAMING is recommended to be TRICE_FRAMING_COBS when XTEA is used. The trice tool takes it as default then.
#define TRICE_FRAMING TRICE_FRAMING_COBS
#else // #ifdef XTEA_ENCRYPT_KEY
//! TRICE_FRAMING defines the framing method of the binary trice data stream. Default is TRICE_FRAMING_TCOBS if XTEA is off.
//! - options: TRICE_FRAMING_TCOBS, TRICE_FRAMING_COBS, TRICE_FRAMING_NONE
//! - When changing to TRICE_FRAMING_COBS, the trice tool needs an additional switch `-packageFraming COBS`.
//!   - TRICE_FRAMING_COBS is useful if you intend to decode the binary trice date with Python or an other language.
//!   - When using encryption TRICE_FRAMING_TCOBS has no advantage over TRICE_FRAMING_COBS, so TRICE_FRAMING_COBS is better then.
//! - When changing to TRICE_FRAMING_NONE, the trice tool needs an additional switch `-packageFraming none`.
//!   - This mode may be helpful if you write your own trice viewer without a decoder.
//!   - In environments, where no data can get lost, TRICE_FRAMING_NONE may make sense too.
#define TRICE_FRAMING TRICE_FRAMING_TCOBS
#endif // #else // #ifdef XTEA_ENCRYPT_KEY

///////////////////////////////////////////////////////////////////////////////
// Multi selecet physical out channels, the ID ranges are allowed to overlap.
// 

//! Enable and set channel number for SeggerRTT usage. Only channel 0 works right now for some reason.
#define TRICE_RTT0 0 // comment out, if you do not use RTT

//! Enable and set UARTA for serial output.
#define TRICE_UARTA USART2 // comment out, if you do not use TRICE_UARTA
#ifdef TRICE_UARTA
//#define TRICE_UARTA_MIN_ID 1           //!< TRICE_UARTA_MIN_ID is the smallest ID transferred to UARTA.
//#define TRICE_UARTA_MAX_ID ((1<<14)-1) //!< TRICE_UARTA_MAX_ID is the biggest  ID transferred to UARTA.
#endif

//! Enable and set UARTB for serial output.
// #define TRICE_UARTB USART1 // comment out, if you do not use TRICE_UARTB
#ifdef TRICE_UARTB
//#define TRICE_UARTB_MIN_ID 1           //!< TRICE_UARTB_MIN_ID is the smallest ID transferred to UARTB.
//#define TRICE_UARTB_MAX_ID ((1<<14)-1) //!< TRICE_UARTB_MAX_ID is the biggest  ID transferred to UARTB.
#endif

//! CGO interface (for testing)
//#define TRICE_CGO 

//! Enable option for an dditional interface, you can define by your own.
#define TRICE_AUXILIARY
//#define TRICE_AUXILIARY_MIN_ID 1           //!< TRICE_AUXILIARY_MIN_ID is the smallest ID transferred to AUXILIARY.
//#define TRICE_AUXILIARY_MAX_ID ((1<<14)-1) //!< TRICE_AUXILIARY_MAX_ID is the biggest  ID transferred to AUXILIARY.



//! TRICE_TRANSFER_MODE is the selected trice transfer method. Options: TRICE_SAFE_SINGLE_MODE (recommended), TRICE_PACK_MULTI_MODE.
//! TRICE_PACK_MULTI_MODE is usable only in TRICE_MODE TRICE_DOUBLE_BUFFER.
#define TRICE_TRANSFER_MODE TRICE_SAFE_SINGLE_MODE

//! TRICE_CYCLE_COUNTER adds a cycle counter to each trice message.
//! If 0, do not add cycle counter. The TRICE macros are a bit faster. Lost TRICEs are not detectable by the trice tool.
//! If 1, add an 8-bit cycle counter. The TRICE macros are a bit slower. Lost TRICEs are detectable by the trice tool. (reccommended)
#define TRICE_CYCLE_COUNTER 1 

//! TRICE_MCU_IS_BIG_ENDIAN needs to be defined for TRICE64 macros on big endian MCUs.
//#define TRICE_MCU_IS_BIG_ENDIAN 

//! TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN can be defined on little endian MCUs if the trice data are needed in network order,
//! or on big endian MCUs if the trice data are needed in little endian order. You should avoid using this macro because
//! it increases the trice storage time and the needed code amount. 
//#define TRICE_TRANSFER_ORDER_IS_NOT_MCU_ENDIAN

//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Headline info
//
#if TRICE_MODE == TRICE_STACK_BUFFER
#define LOG_TRICE_MODE trice( iD( 2545), "s:     NUCLEO-F030R8  STACK_BUFFER MODE     \n" );
#elif TRICE_MODE == TRICE_STREAM_BUFFER
#define LOG_TRICE_MODE trice( iD( 2171), "s:     NUCLEO-F030R8 STREAM_BUFFER MODE     \n" );
#elif TRICE_MODE == TRICE_DOUBLE_BUFFER
#define LOG_TRICE_MODE trice( iD( 3758), "s:     NUCLEO-F030R8 DOUBLE_BUFFER MODE     \n" );
#else
#define LOG_TRICE_MODE // #error or TRICE_OFF
#endif

//! This is usable as the very first trice sequence after restart. Adapt and use it or ignore it.
#define TRICE_HEADLINE \
    LOG_TRICE_MODE \
    trice( iD( 7746), "s:     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     \n" ); \
    trice( iD( 2661), "s:     " ); \
    TriceLogBufferInfo(); \
    trice( iD( 6427), "s:     \n" ); \
    trice( iD( 3248), "s:     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     \n");

//
///////////////////////////////////////////////////////////////////////////////

//! TRICE_SEGGER_RTT_DIAGNOSTICS allows to track SEGGER RTT buffer usage, if enabled here.
//   #define TRICE_SEGGER_RTT_DIAGNOSTICS

//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
#define TRICE_ENTER_CRITICAL_SECTION SEGGER_RTT_LOCK() 

//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
#define TRICE_LEAVE_CRITICAL_SECTION SEGGER_RTT_UNLOCK() 

///////////////////////////////////////////////////////////////////////////////
// Compiler Adaption
//

//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro empty for more speed. Use only '{' in that case.
//  #define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {

//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro pair empty for more speed. Use only '}' in that case.
//  #define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

#if (defined( __GNUC__ ) && !defined(__ARMCC_VERSION)) /* gnu compiler ####### */ \
 || defined(__IAR_SYSTEMS_ICC__) /* IAR compiler ############################# */ \
 || defined(__TASKING__) /* TASKING compiler (same bugs as GNU!)############## */

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4                                  //!< align to 4 byte boundary preamble
#define ALIGN4_END __attribute__ ((aligned(4))) //!< align to 4 byte boundary post declaration

//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
//#define TRICE_ENTER_CRITICAL_SECTION { uint32_t old_mask = cm_mask_interrupts(1); { // copied from test/OpenCM3_STM32F411_Nucleo/triceConfig.h

//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
//#define TRICE_LEAVE_CRITICAL_SECTION  } cm_mask_interrupts(old_mask); } // copied from test/OpenCM3_STM32F411_Nucleo/triceConfig.h

#elif defined(__ARMCC_VERSION) /* Arm Compiler ############################### */

#if defined ( __CC_ARM ) /* Arm Compiler 4/5 ################################# */
#include <cmsis_armcc.h>
#elif (__ARMCC_VERSION >= 6100100) /* Arm Compiler 6 ######################### */
#include <cmsis_armclang.h>
#endif

#define TRICE_INLINE static inline //! used for trice code

#if defined ( __CC_ARM ) /* Arm Compiler 4/5 ################################# */
#define ALIGN4 __align(4) //!< align to 4 byte boundary preamble
#elif (__ARMCC_VERSION >= 6100100) /* Arm Compiler 6 ######################### */
#define ALIGN4 __attribute__((aligned(4))) //!< align to 4 byte boundary preamble
#endif
#define ALIGN4_END        //!< align to 4 byte boundary post declaration
//#define PACKED __packed   //!< pack data preamble
//#define PACKED_END        //!< pack data post declaration

//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro empty for more speed. Use only '{' in that case.
//#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {

//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro pair empty for more speed. Use only '}' in that case.
//#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

#elif 1 // ####################################################################
#error "add new compiler here"
#else // ######################################################################
#error unknown compliler
#endif // compiler adaptions ##################################################

//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Optical feedback: Adapt to your device or comment header and body out
//

#include "main.h" // LD2_GPIO_Port, LD2_Pin
static inline void ToggleOpticalFeedbackLED( void ){
    LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// UART interface: Adapt to your device.
//

#ifdef TRICE_UARTA
#include "main.h" // hardware specific stuff

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
#include "main.h" // hardware specific stuff

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

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// See trice/doc/TriceProjectImageSizeOptimization.md
//

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

//
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */



#if TRICE_RTT_MODE ==  TRICE_DIRECT_MODE && TRICE_INTERMEDIATE_BUFFER == TRICE_DOUBLE_BUFFER
#error
#endif


//!     - Recommended for RTT-only trice output. Segger code needed.
//!     - Direct output to UART or RTT. For UART transfer trices inside interrupts forbidden. Direct TRICE macro execution.
//!     - This mode is mainly for a quick UART tryout start and if no timing constrains for the TRICE macros exist.
//!     - Only a putchar() function is required - look for triceBlockingPutChar().
//!     - UART Command line similar to: `trice log -p COM1 -baud 115200`
//!     - RTT needs additional tools installed - see RTT documentation.
//!     -  J-LINK Command line similar to: `trice log -p  JLINK -args="-Device STM32G071RB -if SWD -Speed 4000 -RTTChannel 0 "`
//!     - ST-LINK Command line similar to: `trice log -p STLINK -args="-Device STM32G071RB -if SWD -Speed 4000 -RTTChannel 0"`
//!

///////////////////////////////////////////////////////////////////////////////
// Modbus support s not working yet.
// 
//  
//  //! TRICE_LOG_OVER_MODBUS_FUNC24_ALSO allows to access the trice messages over modbus.
//  //! TRICE_LOG_OVER_MODBUS_FUNC24_ONLY works with all TRICE_MODE settings.
//  //! Other trice output channels are supported in this mode.
//  //! See comment on TriceModbusAlsoFetch() for more details.
//  #define TRICE_LOG_OVER_MODBUS_FUNC24_ALSO
//  
//  //! TRICE_LOG_OVER_MODBUS_FUNC24_ONLY allows to access the trice messages over modbus.
//  //! TRICE_LOG_OVER_MODBUS_FUNC24_ONLY works only with TRICE_MODE == TRICE_STREAM_BUFFER.
//  //! Other trice output channels are not supported in this mode.
//  //! See comment on TriceModbusOnlyFetch() for more details.
//  //#define TRICE_LOG_OVER_MODBUS_FUNC24_ONLY
//  
//  //! TRICE_LOG_FIFO_MODBUS_START_ADDRESS is the used virtual modbus address for modbus trice fifo read out.
//  //! The trice tool assumes 47400 as default value. The limit is 47400+(TRICE_FIFO_ELEMENTS>>1).
//  //! If you change this here you need to use the appropriate trice tool CLI switch.
//  #define TRICE_LOG_FIFO_MODBUS_START_ADDRESS 47400
//  
//  
//  #ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO
//  #define TRICE_MODBUS_MIN_ID 1           //!< TRICE_MODBUS_BUFFER_MIN_ID is the smallest ID transferred to MODBUS_BUFFER.
//  #define TRICE_MODBUS_MAX_ID ((1<<14)-1) //!< TRICE_MODBUS_BUFFER_MAX_ID is the biggest  ID transferred to MODBUS_BUFFER.
//  
//  #define TRICE_MODBUS_BUFFER_SIZE 240
//  #define TRICE_MODBUS_FIFO_ELEMENTS 32 //!< Must be a power of 2. The half number is the amount of bufferable trices before they go out.
//  #endif // #ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO

//
///////////////////////////////////////////////////////////////////////////////
