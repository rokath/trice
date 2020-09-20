/*! \file triceBareFifoUART.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_BARE_FIFO_UART_H_
#define TRICE_BARE_FIFO_UART_H_

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

extern uint32_t triceFifoMaxDepthTrices; //!< usabble for diagnostics

//! TRICE_BARE_SYNC_LEVEL is the max amount of trices bulk transmitted without a sync trice injected.
//! Assuming triceServeOut() is called every ms this is also the max ms abount without a sync trice.
//! Bigger numbers decrease the bandwidth needs but increase the possible loss of trices in case of
//! re-sync need. A number of 10-100 is recommended: every 10-100 ms or 10-100 trices a sync trice is transmitted
//! increasing the bandwidth need by 10-1% ans loosing 10 trices in the worst case.
#define TRICE_BARE_SYNC_LEVEL 10

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

#endif /* TRICE_BARE_FIFO_UART_H_ */
