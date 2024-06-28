/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_ll_system.h"

// ms32 is a 32-bit millisecond counter, counting circular in steps of 1 every ms.
//extern uint32_t ms32;

#define TriceStamp16 0x1616 //(SysTick->VAL) // Counts from 31999 -> 0 in each ms.
#define TriceStamp32 0x32323232 // ms32

#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUTPUT 1


//! Enable and set UARTA for deferred serial output.
#define TRICE_UARTA USART2 // comment out, if you do not use TRICE_UARTA
//#define TRICE_UARTA_MIN_ID 1           //!< TRICE_UARTA_MIN_ID is the smallest ID transferred to UARTA. Define with TRICE_UARTA_MAX_ID if you want select trice output here.
//#define TRICE_UARTA_MAX_ID ((1<<14)-1) //!< TRICE_UARTA_MAX_ID is the biggest  ID transferred to UARTA. Define with TRICE_UARTA_MIN_ID if you want select trice output here.

// Compiler Adaption:

//! USE_SEGGER_RTT_LOCK_UNLOCK_MACROS == 1 includes SEGGER_RTT header files even SEGGER_RTT is not used.
#define USE_SEGGER_RTT_LOCK_UNLOCK_MACROS 1

//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro empty for more speed. Use only '{' in that case.
//! #define TRICE_ENTER_CRITICAL_SECTION { SEGGER_RTT_LOCK() { - does the job for many compilers.
//! #define TRICE_ENTER_CRITICAL_SECTION { 
//! #define TRICE_ENTER_CRITICAL_SECTION { uint32_t old_mask = cm_mask_interrupts(1); { // copied from test/OpenCM3_STM32F411_Nucleo/triceConfig.h
//! #define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {
#define TRICE_ENTER_CRITICAL_SECTION { SEGGER_RTT_LOCK() { 

//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro empty for more speed. Use only '}' in that case.
//! #define TRICE_LEAVE_CRITICAL_SECTION } SEGGER_RTT_UNLOCK() } - does the job for many compilers.
//! #define TRICE_LEAVE_CRITICAL_SECTION } 
//! #define TRICE_LEAVE_CRITICAL_SECTION } cm_mask_interrupts(old_mask); } // copied from test/OpenCM3_STM32F411_Nucleo/triceConfig.h
//! #define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }
#define TRICE_LEAVE_CRITICAL_SECTION } SEGGER_RTT_UNLOCK() } 

#define TRICE_INLINE static inline //! used for trice code

// hardware interface:

#include "main.h" // hardware specific definitions

TRICE_INLINE void ToggleOpticalFeedbackLED( void ){
    LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
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

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
