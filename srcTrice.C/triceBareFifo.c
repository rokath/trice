/*! \file triceBareFifo.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include <stdint.h>
#include "triceConfigCompiler.h"
#include "triceBareFifo.h"

#define TRICE_FILENAME TRICE0( Id( 1603), "rd_:triceBareFifo.c" );

//! TRICE_FIFO_BYTE_SIZE must be a power of 2, one trice needs typically 4 or 8 bytes, max 32 bytes.
//! The fifo has to hold trice bursts until they are transmitted.
//! It is transmitted with lower priority in the background for example with the UART tx interrupt.
#define TRICE_FIFO_BYTE_SIZE 256

#define TRICE_FIFO_MASK (((TRICE_FIFO_BYTE_SIZE)>>2)-1) //!< max possible trices count in fifo

//! trice fifo instance, here are the trices buffered.
ALIGN4 uint32_t
triceFifo[ TRICE_FIFO_BYTE_SIZE>>2 ]
ALIGN4_END;

int triceFifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
int triceFifoReadIndex = 0; //!< trice fifo read index
int triceBareFifoMaxDepth = 0; //!< diagnostics

//! tricePushBareFifo puts one trice into trice fifo.
//! This is a trice time critical part.
//! \param v trice id with 2 byte data
void tricePushBareFifo(uint32_t v) {
    triceFifo[triceFifoWriteIndex++] = v;
    triceFifoWriteIndex &= TRICE_FIFO_MASK;
}

//! tricePopBareFifo gets one trice from trice fifo.
//! \return trice id with 2 byte data in one uint32_t.
uint32_t tricePopBareFifo(void) {
    uint32_t v = triceFifo[triceFifoReadIndex++];
    triceFifoReadIndex &= TRICE_FIFO_MASK;
    return v;
}

//! triceBareFifoDepth determines bytes count inside trice fifo.
//! \return count of buffered bytes
int triceBareFifoDepth(void) {
    int triceCount = (triceFifoWriteIndex - triceFifoReadIndex) & TRICE_FIFO_MASK;
    int depth = triceCount*sizeof(uint32_t);
    triceBareFifoMaxDepth = depth < triceBareFifoMaxDepth ? triceBareFifoMaxDepth : depth; // diagnostics
    return depth;
}
