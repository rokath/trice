/*! \file triceEscFifo.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include <string.h> // strlen
#include "triceConfigCompiler.h"
#include "trice.h"
#include "triceEscFifo.h"

#define TRICE_FILENAME TRICE0( Id(44860), "rd_:triceEscFifo.c" );

//! TRICE_FIFO_BYTE_SIZE must be a power of 2, one trice needs 4 to 32 or one day more bytes.
//! The fifo has to hold trice bursts until they are transmitted.
//! It is transmitted with lower priority in the background for example with the UART tx interrupt.
#define TRICE_FIFO_BYTE_SIZE 1024

#define TRICE_FIFO_MASK (TRICE_FIFO_BYTE_SIZE-1) //!< max possible byte count in fifo

//! trice fifo instance, here are the trices buffered.
static uint8_t triceFifo[TRICE_FIFO_BYTE_SIZE];

static int triceFifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
static int triceFifoReadIndex = 0; //!< trice fifo read index
int triceEscFifoMaxDepth = 0; //!< diagnostics

//! tricePushByteEscFifo puts one byte into trice fifo.
//! This is a trice time critical part.
//! \param v byte date
void tricePushByteEscFifo(uint8_t v) {
    triceFifo[triceFifoWriteIndex++] = v;
    triceFifoWriteIndex &= TRICE_FIFO_MASK;
}

//! tricePopByteEscFifo gets one byte from trice fifo.
//! \return byte date
uint8_t tricePopByteEscFifo(void) {
    uint8_t v = triceFifo[triceFifoReadIndex++];
    triceFifoReadIndex &= TRICE_FIFO_MASK;
    return v;
}

//! triceEscFifoDepth determines trices count inside trice fifo.
//! \return count of buffered trices
int triceEscFifoDepth(void) {
    int triceDepth = (triceFifoWriteIndex - triceFifoReadIndex) & TRICE_FIFO_MASK;
    triceEscFifoMaxDepth = triceDepth < triceEscFifoMaxDepth ? triceEscFifoMaxDepth : triceDepth; // diagnostics
    return triceDepth;
}

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
