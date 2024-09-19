//! \file triceUart.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"
#include <stdint.h>
#include <stdlib.h>

#if TRICE_DEFERRED_UARTA == 1 && TRICE_OFF == 0

#include "triceUart.h" // User has to provide this hardeware specific file, see examples folders.

//! triceOutBufferUartA points into the double or ring buffer to the next (encoded) trice package.
static const uint8_t* triceOutBufferUartA;

//! triceOutCountUartA is the not yet transmitted byte count after a TriceNonBlockingWriteUartA() call.
static size_t triceOutCountUartA = 0;

//! triceOutIndexUartA is the triceOutBufferUartA offset to the next to transmit byte.
static unsigned triceOutIndexUartA = 0;

//! TriceNonBlockingWriteUartA registers a buffer for TRICE_UARTA transmission.
//! \param buf is byte buffer start.
//! \param nByte is the number of bytes to transfer
void TriceNonBlockingWriteUartA(const void* buf, size_t nByte) {
#if TRICE_CGO == 1 // automated tests
	TriceWriteDeviceCgo(buf, nByte);
#else // #if TRICE_CGO == 1// automated tests
	TRICE_ENTER_CRITICAL_SECTION
#if 0
            triceOutBufferUartA = buf;
#else
	static uint8_t t[TRICE_DEFERRED_BUFFER_SIZE / 2]; // todo: find a better solution to avoid RAM wasting
	memcpy(t, buf, nByte);
	triceOutBufferUartA = t;
#endif
	triceOutIndexUartA = 0;
	triceOutCountUartA = nByte;
	triceEnableTxEmptyInterruptUartA(); // triceTriggerTransmitUartA();
	TRICE_LEAVE_CRITICAL_SECTION
#endif // #else // #if TRICE_CGO == 1// automated tests
}

//! TriceOutDepthUartA returns the amount of bytes not written yet to UARTB.
unsigned TriceOutDepthUartA(void) {
	unsigned depth;
	TRICE_ENTER_CRITICAL_SECTION
	depth = triceOutCountUartA - triceOutIndexUartA;
	TRICE_LEAVE_CRITICAL_SECTION
	return depth;
}

//! triceNextUint8UartA returns the next trice byte for transmission to TRICE_UARTA.
TRICE_INLINE uint8_t triceNextUint8UartA(void) {
	uint8_t c;
	TRICE_ENTER_CRITICAL_SECTION
	c = triceOutBufferUartA[triceOutIndexUartA++];
	TRICE_LEAVE_CRITICAL_SECTION
	return c;
}

//! triceServeTransmitUartA must be called cyclically to proceed ongoing write out.
//! A good place is UARTA ISR.
void triceServeTransmitUartA(void) {
	TRICE_ENTER_CRITICAL_SECTION
	triceTransmitData8UartA(triceNextUint8UartA());
	if (0 == TriceOutDepthUartA()) { // no more bytes
		triceDisableTxEmptyInterruptUartA();
	}
	TRICE_LEAVE_CRITICAL_SECTION
}

//  // triceTriggerTransmitUartA must be called cyclically to initialize write out.
//  void triceTriggerTransmitUartA(void){
//      if( TriceOutDepthUartA() && triceTxDataRegisterEmptyUartA() ){
//          triceEnableTxEmptyInterruptUartA(); // next bytes
//      }
//  }

#endif // #if TRICE_DEFERRED_UARTA == 1

#if TRICE_DEFERRED_UARTB == 1

#include "triceUart.h" // User has to provide this hardeware specific file, see examples folders.

//! triceOutBufferUartB holds the uart out buffer address.
static const uint8_t* triceOutBufferUartB;

//! triceOutCountUartB holds th uarts out buffer size.
static size_t triceOutCountUartB = 0;

//! triceOutIndexUartB is the next to transmit byte index.
static unsigned triceOutIndexUartB = 0;

//! TriceNonBlockingWriteUartB registers a buffer for TRICE_UARTB transmission.
//! \param buf is byte buffer start.
//! \param nByte is the number of bytes to transfer
void TriceNonBlockingWriteUartB(const void* buf, size_t nByte) {
#if TRICE_CGO == 1 // automated tests
	TriceWriteDeviceCgo(buf, nByte);
#else  // #if TRICE_CGO == 1// automated tests
	triceOutBufferUartB = buf;
	triceOutIndexUartB = 0;
	triceOutCountUartB = nByte;
	triceTriggerTransmitUartB();
#endif // #else // #if TRICE_CGO == 1// automated tests
}

//! TriceOutDepthUartB returns the amount of bytes not written yet to UARTB.
unsigned TriceOutDepthUartB(void) {
	// unsigned depthRtt0 = 0; -> assuming RTT is fast enough
	unsigned depth = triceOutCountUartB - triceOutIndexUartB;
	return depth;
}

//! triceNextUint8UartB returns the next trice byte for transmission to TRICE_UARTA.
TRICE_INLINE uint8_t triceNextUint8UartB(void) {
	return triceOutBufferUartB[triceOutIndexUartB++];
}

//! triceServeTransmitUartB must be called cyclically to proceed ongoing write out.
//! A good place is UARTA ISR.
void triceServeTransmitUartB(void) {
	triceTransmitData8UartB(triceNextUint8UartB());
	if (0 == TriceOutDepthUartB()) { // no more bytes
		triceDisableTxEmptyInterruptUartB();
	}
}

// triceTriggerTransmitUartB must be called cyclically to initialize write out.
void triceTriggerTransmitUartB(void) {
	if (TriceOutDepthUartB() && triceTxDataRegisterEmptyUartB()) {
		triceEnableTxEmptyInterruptUartB(); // next bytes
	}
}

#endif // #if TRICE_DEFERRED_UARTB == 1 && TRICE_OFF == 0
