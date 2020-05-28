/*! \file triceWriteSeggerRTT.c
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/


#include <string.h> // memcpy
#include "triceConfig.h"
#include "trice.h"
#include "SEGGER_RTT.h"


//! \param buf address to read from
//! \param nbytes count to write
//! \return count of written bytes
//! triceWrite is copying data into a separate buffer
unsigned triceWrite(const void *buf, int nbytes){
    return (int)SEGGER_RTT_Write(0, buf, nbytes);
}

#if TRICE_FIFO_SIZE

unsigned minWriteSpace = 0xFFFFFFFF; //!< for min write space diagnostics

//! get available free space for writing
//!\return writable space
unsigned triceWriteSpace( void ){
    unsigned writeSpace = SEGGER_RTT_GetAvailWriteSpace(0);
    minWriteSpace = writeSpace < minWriteSpace ? minWriteSpace : minWriteSpace; // diagnostics
    return writeSpace;
}


//! This function should be called cyclically to trigger transmission start, for example in the sysTick interrupt.
//! If not interrup is used it should be called cyclically. With each call max 1 byte is transmitted.
//! \param tick current tick, timebase is users choice
//! \param transferPeriod (in ticks) is the max allowe trace transfer rate from trice fifo to trice write buffer.
//! A (basic) trice allocates inside trice fifo 4 byte but in the write buffer 8 bytes. Therefore in case of heavy trice bursts
//! inside trice fifo it is recommended to transfer them not quicker as the trice write out bandwidth for trices allowas to save RAM buffer.
//! For example with 115200 baud about 10 bytes per ms are transferrable, if no other writes go over the same physical channel.
//! So 1 ms for the transfer period is a reasonabe value in that case.
//! \param servePeriod (in ticks) is the time period an internal check for writing out the 

void triceServe( void ){
    triceToWriteBuffer();
}

#endif // #if TRICE_FIFO_SIZE
