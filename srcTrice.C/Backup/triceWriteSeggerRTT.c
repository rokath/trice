/*! \file triceWriteSeggerRTT.c
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"
#if 0 // WIP
///////////////////////////////////////////////////////////////////////////////////////////////////
// unchanged copied from SEGGER_RTT.c lines 104-230 (some values maybe not used)
#ifndef SEGGER_RTT_BUFFER_SECTION
  #if defined(SEGGER_RTT_SECTION)
    #define SEGGER_RTT_BUFFER_SECTION SEGGER_RTT_SECTION
  #endif
#endif

#ifndef   SEGGER_RTT_ALIGNMENT
  #define SEGGER_RTT_ALIGNMENT                            0
#endif

#ifndef   SEGGER_RTT_BUFFER_ALIGNMENT
  #define SEGGER_RTT_BUFFER_ALIGNMENT                     0
#endif

#ifndef   SEGGER_RTT_MODE_DEFAULT
  #define SEGGER_RTT_MODE_DEFAULT                         SEGGER_RTT_MODE_NO_BLOCK_SKIP
#endif

#ifndef   SEGGER_RTT_LOCK
  #define SEGGER_RTT_LOCK()
#endif

#ifndef   SEGGER_RTT_UNLOCK
  #define SEGGER_RTT_UNLOCK()
#endif

#ifndef   STRLEN
  #define STRLEN(a)                                       strlen((a))
#endif

#ifndef   STRCPY
  #define STRCPY(pDest, pSrc, NumBytes)                   strcpy((pDest), (pSrc))
#endif

#ifndef   SEGGER_RTT_MEMCPY_USE_BYTELOOP
  #define SEGGER_RTT_MEMCPY_USE_BYTELOOP                  0
#endif

#ifndef   SEGGER_RTT_MEMCPY
  #ifdef  MEMCPY
    #define SEGGER_RTT_MEMCPY(pDest, pSrc, NumBytes)      MEMCPY((pDest), (pSrc), (NumBytes))
  #else
    #define SEGGER_RTT_MEMCPY(pDest, pSrc, NumBytes)      memcpy((pDest), (pSrc), (NumBytes))
  #endif
#endif

#ifndef   MIN
  #define MIN(a, b)         (((a) < (b)) ? (a) : (b))
#endif

#ifndef   MAX
  #define MAX(a, b)         (((a) > (b)) ? (a) : (b))
#endif
//
// For some environments, NULL may not be defined until certain headers are included
//
#ifndef NULL
  #define NULL 0
#endif
*/
/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/
#if (defined __ICCARM__) || (defined __ICCRX__)
  #define RTT_PRAGMA(P) _Pragma(#P)
#endif

#if SEGGER_RTT_ALIGNMENT || SEGGER_RTT_BUFFER_ALIGNMENT
  #if (defined __GNUC__)
    #define SEGGER_RTT_ALIGN(Var, Alignment) Var __attribute__ ((aligned (Alignment)))
  #elif (defined __ICCARM__) || (defined __ICCRX__)
    #define PRAGMA(A) _Pragma(#A)
#define SEGGER_RTT_ALIGN(Var, Alignment) RTT_PRAGMA(data_alignment=Alignment) \
                                  Var
  #elif (defined __CC_ARM)
    #define SEGGER_RTT_ALIGN(Var, Alignment) Var __attribute__ ((aligned (Alignment)))
  #else
    #error "Alignment not supported for this compiler."
  #endif
#else
  #define SEGGER_RTT_ALIGN(Var, Alignment) Var
#endif

#if defined(SEGGER_RTT_SECTION) || defined (SEGGER_RTT_BUFFER_SECTION)
  #if (defined __GNUC__)
    #define SEGGER_RTT_PUT_SECTION(Var, Section) __attribute__ ((section (Section))) Var
  #elif (defined __ICCARM__) || (defined __ICCRX__)
#define SEGGER_RTT_PUT_SECTION(Var, Section) RTT_PRAGMA(location=Section) \
                                        Var
  #elif (defined __CC_ARM)
    #define SEGGER_RTT_PUT_SECTION(Var, Section) __attribute__ ((section (Section), zero_init))  Var
  #else
    #error "Section placement not supported for this compiler."
  #endif
#else
  #define SEGGER_RTT_PUT_SECTION(Var, Section) Var
#endif


#if SEGGER_RTT_ALIGNMENT
  #define SEGGER_RTT_CB_ALIGN(Var)  SEGGER_RTT_ALIGN(Var, SEGGER_RTT_ALIGNMENT)
#else
  #define SEGGER_RTT_CB_ALIGN(Var)  Var
#endif

#if SEGGER_RTT_BUFFER_ALIGNMENT
  #define SEGGER_RTT_BUFFER_ALIGN(Var)  SEGGER_RTT_ALIGN(Var, SEGGER_RTT_BUFFER_ALIGNMENT)
#else
  #define SEGGER_RTT_BUFFER_ALIGN(Var)  Var
#endif


#if defined(SEGGER_RTT_SECTION)
  #define SEGGER_RTT_PUT_CB_SECTION(Var) SEGGER_RTT_PUT_SECTION(Var, SEGGER_RTT_SECTION)
#else
  #define SEGGER_RTT_PUT_CB_SECTION(Var) Var
#endif

#if defined(SEGGER_RTT_BUFFER_SECTION)
  #define SEGGER_RTT_PUT_BUFFER_SECTION(Var) SEGGER_RTT_PUT_SECTION(Var, SEGGER_RTT_BUFFER_SECTION)
#else
  #define SEGGER_RTT_PUT_BUFFER_SECTION(Var) Var
#endif
//
///////////////////////////////////////////////////////////////////////////////////////////////////


//! compile-time selected pointer for faster access
static SEGGER_RTT_BUFFER_UP* const pRingTrice = &_SEGGER_RTT.aUp[TRICE_RTT_CHANNEL];


//! time critical part
void triceToRTT( uint32_t v ){
    uint32_t* pDest = (uint32_t*)(pRingTrice->pBuffer + pRingTrice->WrOff);
    *pDest = v;

    if( (TRICE_RTT_BUFFER_SIZE - sizeof( uint32_t )) == pRingTrice->WrOff ){
        pRingTrice->WrOff = 0u;
    } else {
        pRingTrice->WrOff += sizeof( uint32_t );
    }
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
SEGGER_RTT_PUT_BUFFER_SECTION(SEGGER_RTT_BUFFER_ALIGN(static char _acUpBuffer1  [BUFFER0_SIZE_UP]));
SEGGER_RTT_PUT_BUFFER_SECTION(SEGGER_RTT_BUFFER_ALIGN(static char _acDownBuffer1[BUFFER0_SIZE_DOWN]));
SEGGER_RTT_PUT_BUFFER_SECTION(SEGGER_RTT_BUFFER_ALIGN(static char _acUpBuffer2  [BUFFER0_SIZE_UP]));
SEGGER_RTT_PUT_BUFFER_SECTION(SEGGER_RTT_BUFFER_ALIGN(static char _acDownBuffer2[BUFFER0_SIZE_DOWN]));

void triceSeggerRTT_DoInit(void) {
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
  p->aUp[0].pBuffer       = _acUpBuffer0;
  p->aUp[0].SizeOfBuffer  = sizeof(_acUpBuffer0);
  p->aUp[0].RdOff         = 0u;
  p->aUp[0].WrOff         = 0u;
  p->aUp[0].Flags         = SEGGER_RTT_MODE_DEFAULT;
  //
  // Initialize down buffer 0
  //
  p->aDown[0].sName         = "Terminal";
  p->aDown[0].pBuffer       = _acDownBuffer0;
  p->aDown[0].SizeOfBuffer  = sizeof(_acDownBuffer0);
  p->aDown[0].RdOff         = 0u;
  p->aDown[0].WrOff         = 0u;
  p->aDown[0].Flags         = SEGGER_RTT_MODE_DEFAULT;  //
  // Initialize up buffer 1
  //
  p->aUp[1].sName         = "Data";
  p->aUp[1].pBuffer       = _acUpBuffer1;
  p->aUp[1].SizeOfBuffer  = sizeof(_acUpBuffer1);
  p->aUp[1].RdOff         = 0u;
  p->aUp[1].WrOff         = 0u;
  p->aUp[1].Flags         = SEGGER_RTT_MODE_DEFAULT;
  //
  // Initialize down buffer 1
  //
  p->aDown[1].sName         = "Data";
  p->aDown[1].pBuffer       = _acDownBuffer1;
  p->aDown[1].SizeOfBuffer  = sizeof(_acDownBuffer1);
  p->aDown[1].RdOff         = 0u;
  p->aDown[1].WrOff         = 0u;
  p->aDown[1].Flags         = SEGGER_RTT_MODE_DEFAULT;  //
  // Initialize up buffer 2
  //
  p->aUp[2].sName         = "Trice";
  p->aUp[2].pBuffer       = _acUpBuffer2;
  p->aUp[2].SizeOfBuffer  = sizeof(_acUpBuffer2);
  p->aUp[2].RdOff         = 0u;
  p->aUp[2].WrOff         = 0u;
  p->aUp[2].Flags         = SEGGER_RTT_MODE_DEFAULT;
  //
  // Initialize down buffer 1
  //
  p->aDown[2].sName         = "Trice";
  p->aDown[2].pBuffer       = _acDownBuffer2;
  p->aDown[2].SizeOfBuffer  = sizeof(_acDownBuffer2);
  p->aDown[2].RdOff         = 0u;
  p->aDown[2].WrOff         = 0u;
  p->aDown[2].Flags         = SEGGER_RTT_MODE_DEFAULT;
  //
  // Finish initialization of the control block.
  // Copy Id string in three steps to make sure "SEGGER RTT" is not found
  // in initializer memory (usually flash) by J-Link
  //
  STRCPY(&p->acID[7], "RTT", 9);
  STRCPY(&p->acID[0], "SEGGER", 7);
  p->acID[6] = ' ';
}
#endif




//! \param buf address to read from
//! \param nbytes count to write
//! \return count of written bytes
//! triceWrite is copying data into a separate buffer
unsigned triceWrite(const void *buf, int nbytes){
    return (int)SEGGER_RTT_Write(0, buf, nbytes);
}



//! put one trice into RTT0 buffer
//! \param v trice
//! trice time critical part
void tricePushRTT( uint32_t v ){
    SEGGER_RTT_Write(TRICE_RTT_CHANNEL, &v, sizeof(v));
}




#if defined( TRICE_FIFO_SIZE ) && TRICE_FIFO_SIZE > 0

unsigned minWriteSpace = 0xFFFFFFFF; //!< for min write space diagnostics

//! get available free space for writing
//!\return writable space
unsigned triceWriteSpace( void ){
    unsigned writeSpace = SEGGER_RTT_GetAvailWriteSpace(0);
    minWriteSpace = writeSpace < minWriteSpace ? minWriteSpace : minWriteSpace; // diagnostics
    return writeSpace;
}


//! This function should be called cyclically to trigger transmission start, for example in the sysTick interrupt.
//! If no interrupt is used it should be called cyclically. With each call max 1 byte is transmitted.
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

#endif // #if defined( TRICE_FIFO_SIZE ) && TRICE_FIFO_SIZE > 0
