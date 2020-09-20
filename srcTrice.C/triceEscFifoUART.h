/*! \file triceEscFifoUART.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_ESC_FIFO_UART_H_
#define TRICE_ESC_FIFO_UART_H_

#include <stdint.h>
#include "main.h" // hardware specific stuff
#include "triceConfigCompiler.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Set according to hardware
#define TRICE_UART USART2

//! triceServeOut must be called cyclically like every 1-100 ms for examle in main loop or a background task.
void triceServeOut(void);

//! triceServeTransmit is to be lacated inside the according UART ISR.
void triceServeTransmit(void);

extern int triceFifoMaxDepth; //!< usabble for diagnostics

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t

triceTxDataRegisterEmpty(void) {
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

#endif /* TRICE_ESC_FIFO_UART_H_ */
