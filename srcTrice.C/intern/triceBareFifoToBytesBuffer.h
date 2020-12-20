/*! \file triceBareFifoToBytesBuffer.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_BARE_FIFO_TO_BYTES_BUFFER_H_
#define TRICE_BARE_FIFO_TO_BYTES_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//! triceServeBareFifoToBytesBuffer must be called cyclically like every 1-100 ms for examle in main loop or a background task.
void triceServeBareFifoToBytesBuffer(int encoding);

int triceBytesByfferDepth( void );

extern int triceBytesBufferIndex;
extern uint8_t triceBytesBuffer[8];
extern int const triceBytesBufferIndexLimit; // sizeof(triceBytesBuffer[8]);

void triceServeBareFifoSyncedToBytesBuffer(void);
void triceServeBareFifoWrappedToBytesBuffer(void);

#ifdef __cplusplus
}
#endif

#endif /* TRICE_BARE_FIFO_TO_BYTES_BUFFER_H_ */
