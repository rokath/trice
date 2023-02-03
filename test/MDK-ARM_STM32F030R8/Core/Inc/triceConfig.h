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

//! TRICE_DEFAULT_PARAMETER_BIT_WIDTH is the default parameter bit width for TRICE macros not specifying the parameter bit width: 8, 16, 32 or 64.
//! If for example the majority of your values is 16 bit, it makes sense to set this value to 16 to use TRICE for them and to use TRICE32 explicitely for 32-bit values.
//! The trice tool CLI switch "-defaultTRICEBitwidth" needs to be set to the same bit width,
#define TRICE_DEFAULT_PARAMETER_BIT_WIDTH 32

//! TRICE_B is a shortcut for TRICE8_B, TRICE16_B, TRICE32_B or TRICE64_B usable in your project.
#define TRICE_B  TRICE8_B

//! TRICE_F is a shortcut for TRICE8_F, TRICE16_F, TRICE32_F or TRICE64_F usable in your project.
#define TRICE_F  TRICE8_F


///////////////////////////////////////////////////////////////////////////////
// Select trice mode and general settings.
//

//! TRICE_MODE is a predefined trice transfer method. Options: TRICE_STACK_BUFFER, TRICE_DOUBLE_BUFFER, TRICE_STREAM_BUFFER
//! The TRICE_MODE decision is a general one concering the inner data handling and is (nearly) independent from the out channel(s).
//!
//! TRICE_STACK_BUFFER:
//! \li Recommended for RTT-only trice output. Segger code needed.
//! \li Direct output to UART or RTT with cycle counter. For UART transfer trices inside interrupts forbidden. Direct TRICE macro execution.
//! \li This mode is mainly for a quick UART tryout start and if no timing constrains for the TRICE macros exist.
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
#define TRICE_MODE TRICE_DOUBLE_BUFFER 

//! TRICE_SINGLE_MAX_SIZE is used to truncate long dynamically generated strings and to detect the need of a stream buffer wrap.
//! Be careful with this value: When using 12 64-bit values with a 64-bit stamp the trice size is 2 + 8 + 2 + 12*8 = 108 bytes 
#define TRICE_SINGLE_MAX_SIZE 40 // no need for a power of 2 here

#if TRICE_MODE == TRICE_STACK_BUFFER
#define TRICE_BUFFER_SIZE (TRICE_SINGLE_MAX_SIZE + 8) //!< TRICE_BUFFER_SIZE is the used additional max stack size for a single TRICE macro. Recommended value: TRICE_SINGLE_MAX_SIZE plus 8.
#elif TRICE_MODE == TRICE_STREAM_BUFFER 
#define TRICE_TRANSFER_INTERVAL_MS 10 //!< TRICE_TRANSFER_INTERVAL_MS is the milliseconds interval for a single TRICE read out. Each trigger transfers up to one trice, so make this value not too big to get all trices out in the average. This time should be shorter than visible delays. 
#define TRICE_FIFO_ELEMENTS 128 //!< Must be a power of 2. The half number is the amount of bufferable trices before they go out.
#define TRICE_BUFFER_SIZE 0x800 //!< TRICE_BUFFER_SIZE is the used max buffer size for a TRICE macro burst. Recommended value: 2000.
#elif TRICE_MODE == TRICE_DOUBLE_BUFFER 
#define TRICE_TRANSFER_INTERVAL_MS 10 //!< TRICE_TRANSFER_INTERVAL_MS is the milliseconds interval for TRICE buffer read out. Each trigger transfers all in a half buffer stored trices. The TRICE_HALF_BUFFER_SIZE must be able to hold all trice messages possibly occouring in this time. This time should be shorter than visible delays. 
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

//! TRICE_DATA_OFFSET is the space in front of trice data for in-buffer (T)COBS encoding. It must be be a multiple of uint32_t.
#define TRICE_DATA_OFFSET 32


///////////////////////////////////////////////////////////////////////////////
// Multi selecet physical out channels, the ID ranges are allowed to overlap.
// 

//! Enable and set channel number for SeggerRTT usage. Only channel 0 works right now for some reason.
#define TRICE_RTT0 0 // comment out, if you do not use RTT

//! Enable and set UART2 for serial output.
#define TRICE_UARTA USART2 // comment out, if you do not use TRICE_UARTA
#define TRICE_UARTA_MIN_ID 1           //!< TRICE_UARTA_MIN_ID is the smallest ID transferred to UARTA.
#define TRICE_UARTA_MAX_ID ((1<<14)-1) //!< TRICE_UARTA_MAX_ID is the biggest ID transferred to UARTA.

//! Enable and set UART for serial output.
// #define TRICE_UARTB USART1 // comment out, if you do not use TRICE_UARTB
// #define TRICE_UARTB_MIN_ID 1           //!< TRICE_UARTB_MIN_ID is the smallest ID transferred to UARTB.
// #define TRICE_UARTB_MAX_ID ((1<<14)-1) //!< TRICE_UARTB_MAX_ID is the biggest ID transferred to UARTB.

//! CGO interface (for testing)
//#define TRICE_CGO 


//! TRICE_LOG_OVER_MODBUS_FUNC24_ALSO allows to access the trice messages over modbus.
//! TRICE_LOG_OVER_MODBUS_FUNC24_ONLY works with all TRICE_MODE settings.
//! Other trice output channels are supported in this mode.
//! See comment on TriceModbusAlsoFetch() for more details.
#define TRICE_LOG_OVER_MODBUS_FUNC24_ALSO

//! TRICE_LOG_OVER_MODBUS_FUNC24_ONLY allows to access the trice messages over modbus.
//! TRICE_LOG_OVER_MODBUS_FUNC24_ONLY works only with TRICE_MODE == TRICE_STREAM_BUFFER.
//! Other trice output channels are not supported in this mode.
//! See comment on TriceModbusOnlyFetch() for more details.
//#define TRICE_LOG_OVER_MODBUS_FUNC24_ONLY

//! TRICE_LOG_FIFO_MODBUS_START_ADDRESS is the used virtual modbus address for modbus trice fifo read out.
//! The trice tool assumes 47400 as default value. The limit is 47400+(TRICE_FIFO_ELEMENTS>>1).
//! If you change this here you need to use the appropriate trice tool CLI switch.
#define TRICE_LOG_FIFO_MODBUS_START_ADDRESS 47400


#ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO
#define TRICE_MODBUS_BUFFER_MIN_ID 1           //!< TRICE_MODBUS_BUFFER_MIN_ID is the smallest ID transferred to MODBUS_BUFFER.
#define TRICE_MODBUS_BUFFER_MAX_ID ((1<<14)-1) //!< TRICE_MODBUS_BUFFER_MAX_ID is the biggest ID transferred to MODBUS_BUFFER.

#define TRICE_MODBUS_BUFFER_SIZE 240
#define TRICE_MODBUS_FIFO_ELEMENTS 32 //!< Must be a power of 2. The half number is the amount of bufferable trices before they go out.
#endif // #ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO

//
///////////////////////////////////////////////////////////////////////////////

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

//! This is usable as the very first trice sequence after restart. Adapt and use it or ignore it.
#define TRICE_HEADLINE \
    TRICE0( ID( 1089), "s:                                          \n" ); \
    TRICE8( ID( 2473), "s:     NUCLEO-F030R8     TRICE_MODE %3u     \n", TRICE_MODE ); \
    TRICE0( ID( 5096), "s:                                          \n" ); \
    TRICE0( ID( 2966), "s:     " ); \
    TriceLogBufferInfo(); \
    TRICE0( ID( 1631), "s:     \n" ); \
    TRICE0( ID( 1493), "s:                                          \n");

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
#endif // #ifdef TRICE_STM32

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
#endif // #ifdef TRICE_STM32

//
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */

