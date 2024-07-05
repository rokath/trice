//! \file triceUart.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdlib.h>
#include "trice.h"

#if TRICE_DEFERRED_UARTA == 1

#include "triceUart.h" // User has to provide this hardeware specific file, see examples folders.

//! triceOutBufferUartA points into the double or ring buffer to the next (encoded) trice package.
static const uint8_t * triceOutBufferUartA;

//! triceOutCountUartA is the not yet transmitted byte count after a TriceNonBlockingWriteUartA() call.
static size_t triceOutCountUartA = 0;

//! triceOutIndexUartA is the triceOutBufferUartA offset to the next to transmit byte.
static unsigned triceOutIndexUartA = 0;

//! TriceNonBlockingWriteUartA registers a buffer for TRICE_UARTA transmission.
//! \param buf is byte buffer start.
//! \param nByte is the number of bytes to transfer
void TriceNonBlockingWriteUartA( const void * buf, size_t nByte ){
    #if TRICE_CGO == 1 // automated tests
        TriceWriteDeviceCgo( buf, nByte );
    #else // #if TRICE_CGO == 1// automated tests
        triceOutBufferUartA = buf;
        triceOutIndexUartA = 0;
        triceOutCountUartA = nByte;
    #endif // #else // #if TRICE_CGO == 1// automated tests
}

//! TriceOutDepthUartA returns the amount of bytes not written yet to UARTB.
unsigned TriceOutDepthUartA( void ){
    unsigned depth = triceOutCountUartA - triceOutIndexUartA;
    return depth;
}

//! TriceNextUint8UartA returns the next trice byte for transmission to TRICE_UARTA.
uint8_t TriceNextUint8UartA( void ){
    return triceOutBufferUartA[triceOutIndexUartA++];
}

//! triceServeTransmitUartA must be called cyclically to proceed ongoing write out.
//! A good place is UARTA ISR.
void triceServeTransmitUartA(void) {
    triceTransmitData8UartA(TriceNextUint8UartA());
    if (0 == TriceOutDepthUartA()) { // no more bytes
        triceDisableTxEmptyInterruptUartA();
    }
}

// triceTriggerTransmitUartA must be called cyclically to initialize write out.
void triceTriggerTransmitUartA(void){
    if( TriceOutDepthUartA() && triceTxDataRegisterEmptyUartA() ){
        triceEnableTxEmptyInterruptUartA(); // next bytes
    }
}

#endif // #if TRICE_DEFERRED_UARTA == 1

#if TRICE_DEFERRED_UARTB == 1

#include "triceUart.h" // User has to provide this hardeware specific file, see examples folders.

//! triceOutBufferUartB holds the uart out buffer address.
static const uint8_t * triceOutBufferUartB;

//! triceOutCountUartB holds th uarts out buffer size.
static size_t triceOutCountUartB = 0;

//! triceOutIndexUartB is the next to transmit byte index.
static unsigned triceOutIndexUartB = 0;

//! TriceNonBlockingWriteUartB registers a buffer for TRICE_UARTB transmission.
//! \param buf is byte buffer start.
//! \param nByte is the number of bytes to transfer
void TriceNonBlockingWriteUartB( const void * buf, size_t nByte ){
    #if TRICE_CGO == 1 // automated tests
        TriceWriteDeviceCgo( buf, nByte );
    #else // #if TRICE_CGO == 1// automated tests
        triceOutBufferUartB = buf;
        triceOutIndexUartB = 0;
        triceOutCountUartB = nByte;
    #endif // #else // #if TRICE_CGO == 1// automated tests
}

//! TriceOutDepthUartB returns the amount of bytes not written yet to UARTB.
unsigned TriceOutDepthUartB( void ){
    // unsigned depthRtt0 = 0; -> assuming RTT is fast enough
    unsigned depth = triceOutCountUartB - triceOutIndexUartB;
    return depth;
}

//! TriceNextUint8UartB returns the next trice byte for transmission to TRICE_UARTA.
uint8_t TriceNextUint8UartB( void ){
    return triceOutBufferUartB[triceOutIndexUartB++];
}

//! triceServeTransmitUartB must be called cyclically to proceed ongoing write out.
//! A good place is UARTA ISR.
void triceServeTransmitUartB(void) {
    triceTransmitData8UartB(TriceNextUint8UartB());
    if (0 == TriceOutDepthUartB()) { // no more bytes
        triceDisableTxEmptyInterruptUartB();
    }
}

// triceTriggerTransmitUartB must be called cyclically to initialize write out.
void triceTriggerTransmitUartB(void){
    if( TriceOutDepthUartB() && triceTxDataRegisterEmptyUartB() ){
        triceEnableTxEmptyInterruptUartB(); // next bytes
    }
}

#endif // #if TRICE_DEFERRED_UARTB == 1
