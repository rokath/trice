/*! \file triceInterfaceUART.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_INTERFACE_UART_H_
#define TRICE_INTERFACE_UART_H_

#include <stdint.h>
#include "trice.h"
#ifdef UART_LL_STM32
#include "triceUART_LL_STM32.h" // LL STM32 hardware specific stuff
#elif defined(UART_XXX)
#include "triceUART_XXX.h" // any hardware specific stuff
#else
#error "missing interface"
#endif


#ifdef __cplusplus
extern "C" {
#endif


#ifdef ENCRYPT
#if TRICE_FLEX_ENCODING == TRICE_ENCODING
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
#endif
#else // #ifdef ENCRYPT
#if TRICE_FLEX_ENCODING == TRICE_ENCODING

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
#endif
#endif

#if ((TRICE_ESC_ENCODING == TRICE_ENCODING) || (TRICE_COBSR_ENCODING == TRICE_ENCODING))

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
