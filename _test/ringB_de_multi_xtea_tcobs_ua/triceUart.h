/*! \file triceUart.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_UART_H_
#define TRICE_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "trice.h"

#if TRICE_DEFERRED_UARTA == 1

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmptyUartA(void) {
	return 1; // LL_USART_IsActiveFlag_TXE(TRICE_UARTA);
}

//! Write value v into trice transmit register.
//! \param v byte to transmit
//! User must provide this function.
TRICE_INLINE void triceTransmitData8UartA(uint8_t v) {
	// LL_USART_TransmitData8(TRICE_UARTA, v);
}

//! Allow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceEnableTxEmptyInterruptUartA(void) {
	// LL_USART_EnableIT_TXE(TRICE_UARTA);
}

//! Disallow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceDisableTxEmptyInterruptUartA(void) {
	// LL_USART_DisableIT_TXE(TRICE_UARTA);
}
#endif // #if TRICE_DEFERRED_UARTA == 1

#if TRICE_DEFERRED_UARTB == 1

#endif // #if TRICE_DEFERRED_UARTB == 1

#ifdef __cplusplus
}
#endif

#endif /* TRICE_UART_H_ */
