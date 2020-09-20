/*! \file triceInterfaceUART.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_INTERFACE_UART_H_
#define TRICE_INTERFACE_UART_H_

#include <stdint.h>
#include "main.h" // hardware specific stuff
#include "triceConfigCompiler.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Set according to hardware
#define TRICE_UART USART2

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


extern int triceBytesBufferIndex;
extern int triceBytesBufferIndexLimit;
extern uint8_t triceBytesBuffer[8];
//! triceServeBareFifoTransmit must be called cyclically to proceed ongoing write out.
//! A good place: sysTick ISR and UART ISR (both together).
//! TODO: endianess with compiler macros.
TRICE_INLINE void triceServeBareFifoTransmit(void) {
    if (!triceTxDataRegisterEmpty()) {
        for (;;); // unexpected case
    }
    if (triceBytesBufferIndexLimit == triceBytesBufferIndex) {
        for (;;); // unexpected case
    }
    // next byte
    triceTransmitData8(triceBytesBuffer[triceBytesBufferIndex++]);
    if (sizeof(triceBytesBuffer) == triceBytesBufferIndex) { // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}




int triceEscFifoDepth(void);
uint8_t tricePopByteEscFifo(void);
//! triceServeEscFifoTransmit must be called cyclically to proceed ongoing write out.
//! A good place is UART ISR.
TRICE_INLINE void triceServeEscFifoTransmit(void) {
    if (!triceTxDataRegisterEmpty()) {
        for (;;); // unexpected case
    }
    // next byte
    triceTransmitData8(tricePopByteEscFifo());
    if (0 == triceEscFifoDepth()) { // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}




#ifdef __cplusplus
}
#endif

#endif /* TRICE_INTERFACE_UART_H_ */
