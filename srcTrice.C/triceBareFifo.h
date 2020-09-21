/*! \file triceBareFifo.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_BARE_FIFO_H_
#define TRICE_BARE_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void tricePushBareFifo(uint32_t v);
uint32_t tricePopBareFifo(void);

int triceBareFifoDepth(void);
extern int triceBareFifoMaxDepth; //!< diagnostics

#ifdef __cplusplus
}
#endif

#endif /* TRICE_BARE_FIFO_H_ */
