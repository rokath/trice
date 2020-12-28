/*! \file triceInterfaceUART.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_INTERFACE_UART_H_
#define TRICE_INTERFACE_UART_H_

#include <stdint.h>
#include "main.h" // hardware specific stuff
#include "trice.h"


#ifdef __cplusplus
extern "C" {
#endif

//! Set according to hardware
#ifndef TRICE_UART
#error "Need '#define TRICE_UART USARTn' in config file. '#define TRICE_UART USART2'"
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


//! triceServeBytesBufferTransmit must be called cyclically to proceed ongoing write out.
//! A good place: sysTick ISR and UART ISR (both together).
//! TODO: endianess with compiler macros.
TRICE_INLINE void triceServeBytesBufferTransmit(void) {
    if (!triceTxDataRegisterEmpty()) {
        for (;;); // unexpected case
    }
    if (triceBytesBufferIndexLimit == triceBytesBufferIndex) {
        for (;;); // unexpected case
    }
    // next byte
    triceTransmitData8(triceBytesBuffer[triceBytesBufferIndex++]);
    if (triceBytesBufferIndexLimit == triceBytesBufferIndex) { // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}


//! triceServeU32WriteU8ReadFifoTransmit must be called cyclically to proceed ongoing write out.
//! A good place: sysTick ISR and UART ISR (both together).
TRICE_INLINE void triceServeU32WriteU8ReadFifoTransmit(void) {
    if (!triceTxDataRegisterEmpty()) {
        for (;;); // unexpected case
    }
    if (0 == triceU32WriteU8ReadFifoDepth()) {
        for (;;); // unexpected case
    }
    // next byte
    triceTransmitData8(triceU8Pop());
    if (0 == triceU32WriteU8ReadFifoDepth()) { // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}


TRICE_INLINE void triceTriggerBytesBufferTransmit(void){
    if( triceBytesByfferDepth() && triceTxDataRegisterEmpty() ){
        triceEnableTxEmptyInterrupt(); // next bytes
    }
}


TRICE_INLINE void triceTriggerU32WriteU8ReadFifoTransmit(void){
    if( triceU32WriteU8ReadFifoDepth() && triceTxDataRegisterEmpty() ){
        triceEnableTxEmptyInterrupt(); // next bytes
    }
}


//! triceServeEscFifoTransmit must be called cyclically to proceed ongoing write out.
//! A good place is UART ISR.
TRICE_INLINE void triceServeEscFifoTransmit(void) {
    triceTransmitData8(triceU8Pop());
    if (0 == triceU8UsageFifoDepth()) { // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}

TRICE_INLINE void triceTriggerEscFifoTransmit(void){
    if( triceU8UsageFifoDepth() && triceTxDataRegisterEmpty() ){
        triceEnableTxEmptyInterrupt(); // next bytes
    }
}


#ifdef __cplusplus
}
#endif

#endif /* TRICE_INTERFACE_UART_H_ */
