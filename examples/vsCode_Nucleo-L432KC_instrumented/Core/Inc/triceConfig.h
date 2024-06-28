/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern volatile uint32_t * const DWT_CONTROL;
extern volatile uint32_t * const DWT_CYCCNT ;
extern volatile uint32_t * const DEMCR;
extern volatile uint32_t * const LAR; // lock access register

#define TriceStamp16 (*DWT_CYCCNT) // 64 MHz wraps after >1ms 
#define TriceStamp32 ((*DWT_CYCCNT)>>6) // 64 MHz -> 1 µs
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_SEGGER_RTT_32BIT_DIRECT_WRITE 1

#define USE_SEGGER_RTT_LOCK_UNLOCK_MACROS 1
#define TRICE_ENTER_CRITICAL_SECTION { SEGGER_RTT_LOCK() { 
#define TRICE_LEAVE_CRITICAL_SECTION } SEGGER_RTT_UNLOCK() } 

// hardware interface:

#include "main.h" // hardware specific definitions

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

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
