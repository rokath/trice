/*! \file triceWriteSeggerRTT.c
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/



#include "trice.h"


#define TRICE_RTT_CHANNEL 1
#define TRICE_RTT_BUFFER_SIZE 200 //!< must be a multiple of 4

static SEGGER_RTT_BUFFER_UP* const pRing = &_SEGGER_RTT.aUp[TRICE_RTT_CHANNEL];

void triceToRTT( uint32_t v ){
    uint32_t* pDest = (uint32_t*)(pRing->pBuffer);
    *pDest = v;

    if( (TRICE_RTT_BUFFER_SIZE - sizeof( uint32_t )) == pRing->WrOff ){
        pRing->WrOff = 0u;
    } else {
        pRing->WrOff += sizeof( uint32_t );
    }
}


//! \param buf address to read from
//! \param nbytes count to write
//! \return count of written bytes
//! triceWrite is copying data into a separate buffer
unsigned triceWrite(const void *buf, int nbytes){
    return (int)SEGGER_RTT_Write(0, buf, nbytes);
}

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//
// RTT Control Block and allocate buffers for channel 0
//
SEGGER_RTT_PUT_CB_SECTION(SEGGER_RTT_CB_ALIGN(SEGGER_RTT_CB _SEGGER_RTT));

SEGGER_RTT_PUT_BUFFER_SECTION(SEGGER_RTT_BUFFER_ALIGN(static char _acUpBuffer0  [BUFFER0_SIZE_UP]));
SEGGER_RTT_PUT_BUFFER_SECTION(SEGGER_RTT_BUFFER_ALIGN(static char _acDownBuffer0[BUFFER0_SIZE_DOWN]));

static void _DoInit(void) {
  SEGGER_RTT_CB* p;
  //
  // Initialize control block
  //
  p = &_SEGGER_RTT;
  p->MaxNumUpBuffers    = SEGGER_RTT_MAX_NUM_UP_BUFFERS;
  p->MaxNumDownBuffers  = SEGGER_RTT_MAX_NUM_DOWN_BUFFERS;
  //
  // Initialize up buffer 0
  //
  p->aUp[0].sName         = "Terminal";
  p->aUp[0].pBuffer       = _acUpBuffer;
  p->aUp[0].SizeOfBuffer  = sizeof(_acUpBuffer);
  p->aUp[0].RdOff         = 0u;
  p->aUp[0].WrOff         = 0u;
  p->aUp[0].Flags         = SEGGER_RTT_MODE_DEFAULT;  //
  // Initialize up buffer 1
  //
  p->aUp[1].sName         = "Trice";
  p->aUp[1].pBuffer       = _acUpBuffer;
  p->aUp[1].SizeOfBuffer  = sizeof(_acUpBuffer);
  p->aUp[1].RdOff         = 0u;
  p->aUp[1].WrOff         = 0u;
  p->aUp[1].Flags         = SEGGER_RTT_MODE_DEFAULT;
  //
  // Initialize down buffer 0
  //
  p->aDown[0].sName         = "Terminal";
  p->aDown[0].pBuffer       = _acDownBuffer;
  p->aDown[0].SizeOfBuffer  = sizeof(_acDownBuffer);
  p->aDown[0].RdOff         = 0u;
  p->aDown[0].WrOff         = 0u;
  p->aDown[0].Flags         = SEGGER_RTT_MODE_DEFAULT;
  //
  // Finish initialization of the control block.
  // Copy Id string in three steps to make sure "SEGGER RTT" is not found
  // in initializer memory (usually flash) by J-Link
  //
  STRCPY(&p->acID[7], "RTT", 9);
  STRCPY(&p->acID[0], "SEGGER", 7);
  p->acID[6] = ' ';
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
