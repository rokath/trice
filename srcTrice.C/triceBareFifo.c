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

uint32_t triceFifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
uint32_t triceFifoReadIndex = 0; //!< trice fifo read index
int triceBareFifoMaxDepthTrices = 0; //!< diagnostics

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

//! triceBareFifoDepth determines trices count inside trice fifo.
//! \return count of buffered trices
unsigned triceBareFifoDepth(void) {
    unsigned triceDepth = (triceFifoWriteIndex - triceFifoReadIndex) & TRICE_FIFO_MASK;
    triceBareFifoMaxDepthTrices = triceDepth < triceBareFifoMaxDepthTrices ? triceBareFifoMaxDepthTrices : triceDepth; // diagnostics
    return triceDepth;
}
