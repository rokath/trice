/*! \file triceEscFifoUART.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_ESC_FIFO_UART_H_
#define TRICE_ESC_FIFO_UART_H_

#include <stdint.h>
#include "main.h" // hardware specific stuff
#include "triceConfigCompiler.h"
#include "triceInterfaceUART.h"

#ifdef __cplusplus
extern "C" {
#endif

//! triceServeBareFifoOut must be called cyclically like every 1-100 ms for examle in main loop or a background task.
//void triceServeBareFifoOut(void);

//! triceServeEscFifoTransmit is to be lacated inside the according UART ISR.
void triceServeEscFifoTransmit(void);

extern int triceFifoMaxDepth; //!< usabble for diagnostics

#ifdef __cplusplus
}
#endif

#endif /* TRICE_ESC_FIFO_UART_H_ */
