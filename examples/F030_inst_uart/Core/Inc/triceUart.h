// SPDX-License-Identifier: MIT

#ifndef TRICE_UART_H_
#define TRICE_UART_H_

#include "trice.h"
#include "main.h"

// Report whether USART2 can accept the next encoded byte.
TRICE_INLINE uint32_t triceTxDataRegisterEmptyUartA(void) {
	return LL_USART_IsActiveFlag_TXE(TRICE_UARTA);
}

// Write one encoded byte without waiting for the complete frame.
TRICE_INLINE void triceTransmitData8UartA(uint8_t value) {
	LL_USART_TransmitData8(TRICE_UARTA, value);
}

// Start interrupt-driven transmission when queued bytes become available.
TRICE_INLINE void triceEnableTxEmptyInterruptUartA(void) {
	LL_USART_EnableIT_TXE(TRICE_UARTA);
}

// Stop transmit interrupts once all queued bytes have been sent.
TRICE_INLINE void triceDisableTxEmptyInterruptUartA(void) {
	LL_USART_DisableIT_TXE(TRICE_UARTA);
}

#endif /* TRICE_UART_H_ */
