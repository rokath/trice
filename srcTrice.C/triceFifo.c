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
int triceU32UsageFifoDepth(void) {
    int triceCount = (triceU32FifoWriteIndex - triceU32FifoReadIndex) & TRICE_U32_FIFO_MASK;
    int depth = triceCount*sizeof(uint32_t);
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}

//! triceU8UsageFifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int8 access for push and pop.
//! \return count of buffered bytes
int triceU8UsageFifoDepth(void) {
    int depth = (triceU8FifoWriteIndex - triceU8FifoReadIndex) & TRICE_U8_FIFO_MASK;
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}

//! triceU32WriteInt8ReadFifoDepth determines bytes count inside trice fifo.
//! Assumption: Only int32 for push and only int8 for pop.
//! \return count of buffered bytes
int triceU32WriteInt8ReadFifoDepth(void) {
    int depth = ((triceU32FifoWriteIndex<<2) - triceU8FifoReadIndex) & TRICE_U8_FIFO_MASK;
    triceFifoMaxDepth = triceFifoMaxDepth < depth ? depth : triceFifoMaxDepth; // diagnostics
    return depth;
}

////////////////////////////////////// reference

static void triceWritePaddingBytes(int count) {
    while (count--) {
        TRICE_U8PUSH(0);
    }
}

static void triceWriteEsc(int count, uint8_t *buf) {
    while (count--) {
        uint8_t c = *buf++;
        TRICE_U8PUSH(c);
        if (TRICE_ESC == c) {
            TRICE_U8PUSH(TRICE_DEL);
        }
    }
}

//! Start with TRICE_ESC and then comes buf
void triceWriteEscP(int count, uint8_t *buf) {
    TRICE_ENTER_CRITICAL_SECTION
    TRICE_U8PUSH(TRICE_ESC);
    triceWriteEsc(count, buf);
    TRICE_LEAVE_CRITICAL_SECTION
}

#define TRICE_BYTE(d) ((uint8_t)(d))
#define TRICE_HI_BYTE(v) TRICE_BYTE(((uint16_t)(v))>>8)
#define TRICE_LO_BYTE(v) TRICE_BYTE(v)

void trice_s(uint16_t Id, char *dynString) {
    int n = 1 + strlen(dynString);
    int h = -1; // h is the smallest number with 2^h = k && k >= n
    int k = 0;       // n is at least 1 here, so h cannot get -1
    while (k < n) {  // n:  0 | 1 2 3 4 5
        k = 1 << h++;// h: -1 | 1 2 3 3 4
    }                // k:  0 | 1 2 4 4 8
    uint8_t msg[] = {TRICE_P0 + h, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id)};
    TRICE_ENTER_CRITICAL_SECTION
    TRICE_U8PUSH(TRICE_ESC);
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
