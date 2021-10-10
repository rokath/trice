/*! \file triceUART_LL_STM32.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_UART_LL_STM32_H_
#define TRICE_UART_LL_STM32_H_

#include <stdint.h>
#include "main.h" // hardware specific stuff

#ifdef __cplusplus
extern "C" {
#endif

//! Set according to hardware
#ifndef TRICE_UART
#error "Need '#define TRICE_UART USARTn' in config file. Example: '#define TRICE_UART USART2'"
#endif

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

#ifdef __cplusplus
}
#endif

#endif /* TRICE_UART_LL_STM32_H_ */
