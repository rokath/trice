/*! \file triceEscFifo.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_ESC_FIFO_H_
#define TRICE_ESC_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void triceWriteEscP( int count, uint8_t * buf );
void trice_s(uint16_t Id, char * dynString);

void tricePushByteEscFifo(uint8_t v);
uint8_t tricePopByteEscFifo(void);

int triceEscFifoDepth(void);
extern int triceEscFifoMaxDepth; //!< usabble for diagnostics

#ifdef __cplusplus
}
#endif

#endif /* TRICE_ESC_FIFO_H_ */
