/*! \file triceFifo.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include <stdint.h>
#include "./intern/triceConfigCompiler.h"
#include "./intern/triceFifo.h"

#define TRICE_FILENAME TRICE0( Id(13515), "rd_:triceFifo.c" );

//! trice fifo instance, here are the trices buffered.
ALIGN4 uint32_t
triceInt32Fifo[ TRICE_FIFO_BYTE_SIZE>>2 ]
ALIGN4_END;

uint8_t* const triceInt8Fifo = (uint8_t*)triceInt32Fifo;

int triceInt32FifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
int triceInt32FifoReadIndex = 0; //!< trice fifo read index for 32 bit values

int triceInt8FifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
int triceInt8FifoReadIndex = 0; //!< trice fifo read index

int triceFifoMaxDepth = 0; //!< diagnostics

//! triceInt32FifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int32 access for push and pop.
//! \return count of buffered bytes
int triceInt32UsageFifoDepth(void) {
    int triceCount = (triceInt32FifoWriteIndex - triceInt32FifoReadIndex) & TRICE_INT32_FIFO_MASK;
    int depth = triceCount*sizeof(uint32_t);
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}

//! triceInt8UsageFifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int8 access for push and pop.
//! \return count of buffered bytes
int triceInt8UsageFifoDepth(void) {
    int depth = (triceInt8FifoWriteIndex - triceInt8FifoReadIndex) & TRICE_INT8_FIFO_MASK;
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceMaxDepth; // diagnostics
    return depth;
}

//! triceInt32WriteInt8ReadFifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int32 for push and only int8 for pop.
//! \return count of buffered bytes
int triceInt32WriteInt8ReadFifoDepth(void) {
    int depth = ((triceInt32FifoWriteIndex<<2) - triceInt8FifoReadIndex) & TRICE_INT8_FIFO_MASK;
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}

////////////////////////////////////// reference

static void triceWritePaddingBytes(int count) {
    while (count--) {
        TRICE_PUSH_BYTE(0);
    }
}

static void triceWriteEsc(int count, uint8_t *buf) {
    while (count--) {
        uint8_t c = *buf++;
        TRICE_PUSH_BYTE(c);
        if (TRICE_ESC == c) {
            TRICE_PUSH_BYTE(TRICE_DEL);
        }
    }
}

//! Start with TRICE_ESC and then comes buf
void triceWriteEscP(int count, uint8_t *buf) {
    TRICE_ENTER_CRITICAL_SECTION
    TRICE_PUSH_BYTE(TRICE_ESC);
    triceWriteEsc(count, buf);
    TRICE_LEAVE_CRITICAL_SECTION
}

void trice_s(uint16_t Id, char *dynString) {
    int n = 1 + strlen(dynString);
    int h = -1; // h is the smallest number with 2^h = k && k >= n
    int k = 0;       // n is at least 1 here, so h cannot get -1
    while (k < n) {  // n:  0 | 1 2 3 4 5
        k = 1 << h++;// h: -1 | 1 2 3 3 4
    }                // k:  0 | 1 2 4 4 8
    uint8_t msg[] = {TRICE_P0 + h, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id)};
    TRICE_ENTER_CRITICAL_SECTION
    TRICE_PUSH_BYTE(TRICE_ESC);
    triceWriteEsc(sizeof(msg), msg);
    triceWriteEsc(n, (uint8_t *) dynString);
    triceWritePaddingBytes(k - n);
    TRICE_LEAVE_CRITICAL_SECTION
    // example: ""         =                                   0 -> bufLen=1 -> n=0, (1<<0)= 1, padding=0
    // example: "a"        = 'a'                               0 -> bufLen=2 -> n=1, (1<<1)= 2, padding=0
    // example: "ab"       = 'a' 'b'                           0 -> bufLen=3 -> n=2, (1<<2)= 4, padding=1
    // example: "abc"      = 'a' 'b' 'c'                       0 -> bufLen=4 -> n=2, (1<<2)= 4, padding=0
    // example: "abcd"     = 'a' 'b' 'c' 'd'                   0 -> bufLen=5 -> n=3, (1<<8)= 8, padding=3
    // example: "abcdef"   = 'a' 'b' 'c' 'd' 'e'               0 -> bufLen=6 -> n=3, (1<<8)= 8, padding=2
    // example: "abcdef"   = 'a' 'b' 'c' 'd' 'e' 'f'           0 -> bufLen=7 -> n=3, (1<<8)= 8, padding=1
    // example: "abcdefg"  = 'a' 'b' 'c' 'd' 'e' 'f' 'g'       0 -> bufLen=8 -> n=3, (1<<8)= 8, padding=0
    // example: "abcdefgh" = 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h'   0 -> bufLen=9 -> n=4, (1<<4)=16, padding=7
}
