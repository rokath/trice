/*! \file triceFifo.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

//! TRICE_FIFO_BYTE_SIZE must be a power of 2, one trice needs typically 4 or 8 bytes, max 32 bytes.
//! The fifo has to hold trice bursts until they are transmitted.
//! It is transmitted with lower priority in the background for example with the UART tx interrupt.
#ifndef TRICE_FIFO_BYTE_SIZE
#define TRICE_FIFO_BYTE_SIZE (256)
#endif

#define TRICE_INT32_FIFO_MASK (((TRICE_FIFO_BYTE_SIZE)>>2)-1) //!< max possible int32 count in fifo
#define TRICE_INT8_FIFO_MASK ((TRICE_FIFO_BYTE_SIZE)-1) //!< max possible bytes count in fifo

extern uint32_t triceInt32Fifo[ TRICE_FIFO_BYTE_SIZE>>2 ];
extern uint8_t* const triceInt8Fifo;

extern int triceInt32FifoWriteIndex;
extern int triceInt32FifoReadIndex;

extern int triceInt8FifoWriteIndex;
extern int triceInt8FifoReadIndex;

extern int triceFifoMaxDepth;

//! triceInt32Push puts one trice into trice fifo.
//! This is a trice time critical part.
//! \param v trice id with 2 byte data
TRICE_INLINE void triceInt32Push(uint32_t v) {
    triceInt32Fifo[triceInt32FifoWriteIndex++] = v;
    triceInt32FifoWriteIndex &= TRICE_INT32_FIFO_MASK;
}

//! triceInt8Push puts one byte into trice fifo.
//! This is a trice time critical part.
//! \param v trice id with 2 byte data
TRICE_INLINE void triceInt8Push(uint8_t v) {
    triceInt8Fifo[triceInt8FifoWriteIndex++] = v;
    triceInt8FifoWriteIndex &= TRICE_INT8_FIFO_MASK;
}

//! triceInt32Pop gets one trice from trice fifo.
//! \return trice id with 2 byte data in one uint32_t.
TRICE_INLINE uint32_t triceInt32Pop(void) {
    uint32_t v = triceInt32Fifo[triceInt32FifoReadIndex++];
    triceInt32FifoReadIndex &= TRICE_INT32_FIFO_MASK;
    return v;
}

//! triceInt8Pop gets one trice from trice fifo.
//! \return trice id with 2 byte data in one uint32_t.
TRICE_INLINE uint8_t triceInt8PopFifo(void) {
    uint8_t v = triceInt8Fifo[triceInt8FifoReadIndex++];
    triceInt8FifoReadIndex &= TRICE_INT8_FIFO_MASK;
    return v;
}

int triceInt32UsageFifoDepth(void);
int triceInt8UsageFifoDepth(void);
int triceInt32WriteInt8ReadFifoDepth(void);

////////////////////////////////////// reference


void triceWriteEscP(int count, uint8_t *buf);
void trice_s(uint16_t Id, char *dynString);