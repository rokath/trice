/*! \file triceFifo.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/


#if 0 //ndef TRICE_FIFO_H_
#define TRICE_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "triceConfig.h"
#include "intern/triceConfigCompiler.h"

//! TRICE_FIFO_BYTE_SIZE must be a power of 2, one trice needs typically 4 or 8 bytes, max 32 bytes.
//! The fifo has to hold trice bursts until they are transmitted.
//! It is transmitted with lower priority in the background for example with the UART tx interrupt.
#ifndef TRICE_FIFO_BYTE_SIZE
#define TRICE_FIFO_BYTE_SIZE (2048)
#endif

#define TRICE_U32_FIFO_MASK (((TRICE_FIFO_BYTE_SIZE)>>2)-1) //!< max possible int32 count in fifo
#define TRICE_U8_FIFO_MASK ((TRICE_FIFO_BYTE_SIZE)-1) //!< max possible bytes count in fifo

extern uint32_t triceU32Fifo[ TRICE_FIFO_BYTE_SIZE>>2 ];
extern uint8_t* const triceU8Fifo;

extern int triceU32FifoWriteIndex;
extern int triceU32FifoReadIndex;

extern int triceU8FifoWriteIndex;
extern int triceU8FifoReadIndex;

extern int triceFifoMaxDepth;

//! triceU32Push puts one trice into trice fifo.
//! This is a trice time critical part.
//! \param v trice id with 2 byte data
TRICE_INLINE void triceU32Push(uint32_t v) {
    triceU32Fifo[triceU32FifoWriteIndex++] = v;
    triceU32FifoWriteIndex &= TRICE_U32_FIFO_MASK;
}

//! triceU8Push puts one byte into trice fifo.
//! This is a trice time critical part.
//! \param v trice id with 2 byte data
TRICE_INLINE void triceU8Push(uint8_t v) {
    triceU8Fifo[triceU8FifoWriteIndex++] = v;
    triceU8FifoWriteIndex &= TRICE_U8_FIFO_MASK;
}

//! triceU32Pop gets one trice from trice fifo.
//! \return trice id with 2 byte data in one uint32_t.
TRICE_INLINE uint32_t triceU32Pop(void) {
    uint32_t v = triceU32Fifo[triceU32FifoReadIndex++];
    triceU32FifoReadIndex &= TRICE_U32_FIFO_MASK;
    return v;
}

//! triceU8Pop gets one trice from trice fifo.
//! \return trice id with 2 byte data in one uint32_t.
TRICE_INLINE uint8_t triceU8Pop(void) {
    uint8_t v = triceU8Fifo[triceU8FifoReadIndex++];
    triceU8FifoReadIndex &= TRICE_U8_FIFO_MASK;
    return v;
}

int triceU32FifoDepth(void);
int triceU8FifoDepth(void);
int triceU32WriteU8ReadFifoDepth(void);


#ifdef __cplusplus
}
#endif

#endif /* TRICE_FIFO_H_ */
