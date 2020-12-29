/*! \file triceFifo.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include <stdint.h>
#include <string.h> // strlen
#include "triceConfig.h"
#include "intern/triceConfigCompiler.h"
#include "intern/triceFifo.h"

#define TRICE_FILENAME TRICE0( Id(13515), "rd_:triceFifo.c" );

//! trice fifo instance, here are the trices buffered.
ALIGN4 uint32_t
triceU32Fifo[ TRICE_FIFO_BYTE_SIZE>>2 ]
ALIGN4_END;

uint8_t* const triceU8Fifo = (uint8_t*)triceU32Fifo;

int triceU32FifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
int triceU32FifoReadIndex = 0; //!< trice fifo read index for 32 bit values

int triceU8FifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
int triceU8FifoReadIndex = 0; //!< trice fifo read index

int triceFifoMaxDepth = 0; //!< diagnostics

//! triceU32FifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int32 access for push and pop.
//! \return count of buffered bytes
int triceU32FifoDepth(void) {
    int triceCount = (triceU32FifoWriteIndex - triceU32FifoReadIndex) & TRICE_U32_FIFO_MASK;
    int depth = triceCount*sizeof(uint32_t);
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}

//! triceU8UsageFifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int8 access for push and pop.
//! \return count of buffered bytes
int triceU8FifoDepth(void) {
    int depth = (triceU8FifoWriteIndex - triceU8FifoReadIndex) & TRICE_U8_FIFO_MASK;
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}

//! triceU32WriteU8ReadFifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int32 for push and only int8 for pop.
//! \return count of buffered bytes
int triceU32WriteU8ReadFifoDepth(void) {
    int depth = ((triceU32FifoWriteIndex<<2) - triceU8FifoReadIndex) & TRICE_U8_FIFO_MASK;
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}

