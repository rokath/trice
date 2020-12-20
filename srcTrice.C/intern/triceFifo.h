/*! \file triceFifo.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

//! TRICE_FIFO_BYTE_SIZE must be a power of 2, one trice needs typically 4 or 8 bytes, max 32 bytes.
//! The fifo has to hold trice bursts until they are transmitted.
//! It is transmitted with lower priority in the background for example with the UART tx interrupt.
#ifndef TRICE_FIFO_BYTE_SIZE
#define TRICE_FIFO_BYTE_SIZE (256)
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
TRICE_INLINE uint8_t triceU8PopFifo(void) {
    uint8_t v = triceU8Fifo[triceU8FifoReadIndex++];
    triceU8FifoReadIndex &= TRICE_U8_FIFO_MASK;
    return v;
}

int triceU32UsageFifoDepth(void);
int triceU8UsageFifoDepth(void);
int triceU32WriteInt8ReadFifoDepth(void);

////////////////////////////////////// reference

#define TRICE_ESC  0xEC //!< Escape char is control char to start a package.
#define TRICE_DEL  0xDE //!< Delete char, if follower of TRICE_ESC, deletes the meaning os TRICE_ESC making it an ordinary TRICE_ESC char.
#define TRICE_P0   0xDF //!< No param char = If follower of TRICE_ESC only a 16 bit ID is inside the payload.
#define TRICE_P1   0xE0 //!< 1 byte param char = If follower of TRICE_ESC a 16 bit ID and 1 byte are inside the payload.
#define TRICE_P2   0xE1 //!< 2 byte param char = If follower of TRICE_ESC a 16 bit ID and 2 byte are inside the payload.
#define TRICE_P4   0xE2 //!< 4 byte param char = If follower of TRICE_ESC a 16 bit ID and 4 byte are inside the payload.
#define TRICE_P8   0xE3 //!< 8 byte param char = If follower of TRICE_ESC a 16 bit ID and 8 byte are inside the payload.
#define TRICE_P16  0xE4 //!< 16 byte param char = If follower of TRICE_ESC a 16 bit ID and 8 byte are inside the payload.
//                 0xE5 // dynamically used for runtime strings with size 17-32
//                 0xE6 // dynamically used for runtime strings with size 33-64
//                 0xE7 // dynamically used for runtime strings with size 63-128
//                 0xE8 // dynamically used for runtime strings with size 127-256
//                 0xE9 // reserved
//                 0xEA // reserved
//                 0xEB // reserved


void triceWriteEscP(int count, uint8_t *buf);
void trice_s(uint16_t Id, char *dynString);
