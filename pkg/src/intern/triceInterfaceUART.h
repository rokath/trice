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

#if TRICE_FLEX_ENCODING == TRICE_ENCODING && defined(ENCRYPT)
extern uint8_t triceBytesBuffer[];
extern int const triceBytesBufferIndexLimit;
extern int triceBytesBufferIndex;

TRICE_INLINE int triceBytesByfferDepth( void ){
    return triceBytesBufferIndexLimit - triceBytesBufferIndex;
}

//! triceServeTransmit as triceServeBytesBufferTransmit must be called cyclically to proceed ongoing write out.
//! A good place: sysTick ISR and UART ISR (both together).
//! TODO: endianess with compiler macros.
TRICE_INLINE void triceServeTransmit(void) {
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

// triceTriggerTransmit as triceTriggerBytesBufferTransmit must be called cyclically to initialize write out.
TRICE_INLINE void triceTriggerTransmit(void){
    if( triceBytesByfferDepth() && triceTxDataRegisterEmpty() ){
        triceEnableTxEmptyInterrupt(); // next bytes
    }
}
#endif // #if TRICE_FLEX_ENCODING == TRICE_ENCODING && defined(ENCRYPT)

#if TRICE_FLEX_ENCODING == TRICE_ENCODING && !defined(ENCRYPT)

//! triceServeTransmit as triceServeU32WriteU8ReadFifoTransmit must be called cyclically to proceed ongoing write out.
//! A good place: sysTick ISR and UART ISR (both together).
TRICE_INLINE void triceServeTransmit(void) {
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

// triceTriggerTransmit as triceTriggerU32WriteU8ReadFifoTransmit must be called cyclically to initialize write out.
TRICE_INLINE void triceTriggerTransmit(void){
    if( triceU32WriteU8ReadFifoDepth() && triceTxDataRegisterEmpty() ){
        triceEnableTxEmptyInterrupt(); // next bytes
    }
}

#endif // #if TRICE_FLEX_ENCODING == TRICE_ENCODING && !defined(ENCRYPT)

#if TRICE_ESC_ENCODING == TRICE_ENCODING

//! triceServeTransmit as triceServeU8FifoTransmit must be called cyclically to proceed ongoing write out.
//! A good place is UART ISR.
TRICE_INLINE void triceServeTransmit(void) {
    triceTransmitData8(triceU8Pop());
    if (0 == triceU8FifoDepth()) { // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}

// triceTriggerTransmit as triceTriggerU8FifoTransmit must be called cyclically to initialize write out.
TRICE_INLINE void triceTriggerTransmit(void){
    if( triceU8FifoDepth() && triceTxDataRegisterEmpty() ){
        triceEnableTxEmptyInterrupt(); // next bytes
    }
}

#endif // #elif TRICE_ESC_ENCODING == TRICE_ENCODING

#ifdef __cplusplus
}
#endif

#endif /* TRICE_INTERFACE_UART_H_ */
