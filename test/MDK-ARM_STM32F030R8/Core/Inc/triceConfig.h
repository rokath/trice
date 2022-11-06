/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Select trice mode and general settings.
//

//! TRICE_MODE is a predefined trice transfer method. Options: TRICE_STACK_BUFFER, TRICE_DOUBLE_BUFFER, TRICE_STREAM_BUFFER
//!
//! TRICE_STACK_BUFFER:
//! \li Direct output to UART or RTT with cycle counter. For UART transfer trices inside interrupts forbidden. Direct TRICE macro execution.
//! \li This mode is mainly for a quick tryout start or if no timing constrains for the TRICE macros exist.
//! \li Only a putchar() function is required - look for triceBlockingPutChar().
//! \li UART Command line similar to: `trice log -p COM1 -baud 115200`
//! \li RTT needs additional tools installed - see RTT documentation.
//! \li J-LINK Command line similar to: `trice log -args="-Device STM32G071RB -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"`
//! \li ST-LINK Command line similar to: `trice log -p ST-LINK -args="-Device STM32G071RB -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"`
//!
//! TRICE_DOUBLE_BUFFER:
//! \li Double Buffering output to UART with cycle counter. Trices inside interrupts allowed. Fast TRICE macro execution.
//! \li Works also with RTT but makes usually no sense because then TRICE_MODE == TRICE_STACK_BUFFER is more effective and needs less memory.
//! \li UART Command line similar to: `trice log -p COM1 -baud 115200`
//! \li RTT Command line similar to: `trice l -args="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"`
//!
//! TRICE_STREAM_BUFFER:
//! \li Stream Buffering output to UART. Needs less buffer memory for the price of being a bit slower.
//! \li Command line similar to: `trice log -p COM1 -baud 115200`
#define TRICE_MODE TRICE_STREAM_BUFFER 

//! TRICE_SINGLE_MAX_SIZE is used to truncate long dynamically generated strings and to detect the need of a stream buffer wrap.
//! Be careful with this value: When using 12 64-bit values with a 64-bit stamp the trice size is 2 + 8 + 2 + 12*8 = 108 bytes 
#define TRICE_SINGLE_MAX_SIZE 120 // no need for a power of 2 here

#if TRICE_MODE == TRICE_STACK_BUFFER
#define TRICE_BUFFER_SIZE (TRICE_SINGLE_MAX_SIZE + 8) //!< TRICE_BUFFER_SIZE is the used additional max stack size for a single TRICE macro. Recommended value: TRICE_SINGLE_MAX_SIZE plus 8.
#elif TRICE_MODE == TRICE_STREAM_BUFFER 
#define TRICE_TRANSFER_INTERVAL_MS 10 //!< TRICE_TRANSFER_INTERVAL_MS is the milliseconds interval for a single TRICE read out. Each trigger transfers up to one trice, so make this value not too big to get all trices out in the average. This time should be shorter than visible delays. 
#define TRICE_FIFO_ELEMENTS 128 //!< Must be a power of 2. The half number is the amount of bufferable trices before they go out.
#define TRICE_BUFFER_SIZE 0x800 //!< TRICE_BUFFER_SIZE is the used max buffer size for a TRICE macro burst. Recommended value: 2000.
#elif TRICE_MODE == TRICE_DOUBLE_BUFFER 
#define TRICE_TRANSFER_INTERVAL_MS 100 //!< TRICE_TRANSFER_INTERVAL_MS is the milliseconds interval for TRICE buffer read out. Each trigger transfers all in a half buffer stored trices. The TRICE_HALF_BUFFER_SIZE must be able to hold all trice messages possibly occouring in this time. This time should be shorter than visible delays. 
#define TRICE_BUFFER_SIZE 0x800 //!< TRICE_BUFFER_SIZE is the double half buffer size usable for a TRICE macro burst. Recommended value: 2000.
#endif

//! Enable and set channel number for SeggerRTT usage. Only channel 0 works right now for some reason.
//#define TRICE_RTT_CHANNEL 0

//! Enable and set UART for serial output.
#define TRICE_UART USART2

//! TRICE_TRANSFER_MODE is the selected trice transfer method. Options: TRICE_SAFE_SINGLE_MODE (recommended), TRICE_PACK_MULTI_MODE.
#define TRICE_TRANSFER_MODE TRICE_SAFE_SINGLE_MODE

//! TRICE_CYCLE_COUNTER adds a cycle counter to each trice message.
//! If 0, do not add cycle counter. The TRICE macros are a bit faster. Lost TRICEs are not detectable by the trice tool.
//! If 1, add an 8-bit cycle counter. The TRICE macros are a bit slower. Lost TRICEs are detectable by the trice tool. (reccommended)
#define TRICE_CYCLE_COUNTER 1 

//!< TRICE_BIG_ENDIANNESS needs to be defined for TRICE64 macros on big endian devices.
//#define TRICE_BIG_ENDIANNESS 

// Enabling next 2 lines results in XTEA TriceEncryption  with the key.
//#define TRICE_ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret
//#define XTEA_ENCRYPT_KEY TRICE_ENCRYPT
//#define TRICE_DECRYPT //!< TRICE_DECRYPT is usually not needed. Enable for checks.

//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Headline info
//

//! This is usable as the very first trice sequence after restart. Adapt and use it or ignore it.
#define TRICE_HEADLINE \
    TRICE0( Id(10264), "s:                                          \n" ); \
    TRICE8( Id(13076), "s:     NUCLEO-F030R8     TRICE_MODE %3u     \n", TRICE_MODE ); \
    TRICE0( Id(12707), "s:                                          \n" ); \
    TRICE0( Id(15016), "s:     " ); \
    TriceLogBufferInfo(); \
    TRICE0( Id(13072), "s:     \n" ); \
    TRICE0( Id(10051), "s:                                          \n");

//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Compiler Adaption
//

#if (defined( __GNUC__ ) && !defined(__ARMCC_VERSION)) /* gnu compiler ####### */ \
 || defined(__IAR_SYSTEMS_ICC__) /* IAR compiler ############################# */ \
 || defined(__TASKING__) /* TASKING compiler (same bugs as GNU!)############## */

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4                                  //!< align to 4 byte boundary preamble
#define ALIGN4_END __attribute__ ((aligned(4))) //!< align to 4 byte boundary post declaration

//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
#define TRICE_ENTER_CRITICAL_SECTION { // to do

//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
#define TRICE_LEAVE_CRITICAL_SECTION } // to do

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
// Optical feedback: Adapt to your device.
//

#include "main.h" // LED_GREEN_GPIO_Port, LED_GREEN_Pin
static inline void ToggleOpticalFeedbackLED( void ){
    LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// UART interface: Adapt to your device.
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
    ToggleOpticalFeedbackLED();
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
// RTT interface: Adapt to your device.
//

#ifdef TRICE_RTT_CHANNEL
#include "SEGGER_RTT.h"

#define TRICE_WRITE( buf, len ) do{ \
    SEGGER_RTT_Write(TRICE_RTT_CHANNEL, buf, len ); \
    ToggleOpticalFeedbackLED(); \
}while(0)
#endif // #ifdef TRICE_RTT_CHANNEL

//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Default TRICE macro bitwidth: 32 (optionally adapt to MCU bit width)
//

#define TRICE_1  TRICE32_1  //!< Default parameter bit width for 1  parameter count TRICE is 32, change for a different value.
#define TRICE_2  TRICE32_2  //!< Default parameter bit width for 2  parameter count TRICE is 32, change for a different value.
#define TRICE_3  TRICE32_3  //!< Default parameter bit width for 3  parameter count TRICE is 32, change for a different value.
#define TRICE_4  TRICE32_4  //!< Default parameter bit width for 4  parameter count TRICE is 32, change for a different value.
#define TRICE_5  TRICE32_5  //!< Default parameter bit width for 5  parameter count TRICE is 32, change for a different value.
#define TRICE_6  TRICE32_6  //!< Default parameter bit width for 6  parameter count TRICE is 32, change for a different value.
#define TRICE_7  TRICE32_7  //!< Default parameter bit width for 7  parameter count TRICE is 32, change for a different value.
#define TRICE_8  TRICE32_8  //!< Default parameter bit width for 8  parameter count TRICE is 32, change for a different value.
#define TRICE_9  TRICE32_9  //!< Default parameter bit width for 9  parameter count TRICE is 32, change for a different value.
#define TRICE_10 TRICE32_10 //!< Default parameter bit width for 10 parameter count TRICE is 32, change for a different value.
#define TRICE_11 TRICE32_11 //!< Default parameter bit width for 11 parameter count TRICE is 32, change for a different value.
#define TRICE_12 TRICE32_12 //!< Default parameter bit width for 12 parameter count TRICE is 32, change for a different value.

//
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */

