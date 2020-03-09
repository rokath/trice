/*! \file Fifo.c
\brief dynamic data buffering
\details
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
//lint -e534 warning 534: ignoring return value of memcpy

#include "config.h" // should be first include file, at least befor other project specific files

#if 1 // def TRACELOG
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
//#include <CriticalSection.h>
//#include "Tr16.h"
//#include "TraceUtilities.h"
#include "Fifo.h"
//#include "LeftRightFifo.h"
#endif // #ifdef TRACELOG

#include "trice.h"

//#include "Os.h"
#include <string.h> // memcpy
//#include "DefaultMacros.h"
#include "msTicks.h"
//#include "CommonTypes.h"
//#include "TickedExecution.h"


//ALIGN4 static uint8_t fifoHeap[FIFO_HEAP_SIZE] ALIGN4_END; //!< manual heap
//static uint8_t* fifoAllocPtr = fifoHeap; //!< internal helper

/*! allocate memory
\param siz
\return pointer
*/
/*
static uint8_t* fifoMalloc( size_t siz ){
    uint8_t* p = fifoAllocPtr;
#if 0 // debug
    {
        static uint8_t n = 0;
        siz = ((siz + 3)>>2)<<2; // fn called only wit multiple of 4
        memset( p, 0xCC + n++, siz );
        fifoAllocPtr += siz;
        if( fifoAllocPtr > &fifoHeap[RCTX_FIFO_SIZE + RCRX_FIFO_SIZE]){
            for(;;);
        }
    }
#else
    fifoAllocPtr += siz;
#endif

    return p;
}
*/




/*! initialize fifo struct, call once at startup
\param f pointer to fifo struct
\param size fifo capacity
*/
/*
void FifoInit( Fifo_t * const f, size_t size ){
    f->size = size;
    f->pBuff = fifoMalloc(size);
    f->pWr = f->pRd = f->pBuff;
    f->pLimit = f->pBuff + size;
}
*/

/*! make sure write pointer stays inside fifo range
\param f pointer to fifo struct
*/
static void fifoLimitateWrPtr_Unprotected( Fifo_t* f ){
    f->pWr -= f->pWr < f->pLimit ? 0 : f->size;
}


/*! make sure read pointer stays inside fifo range
\param f pointer to fifo struct
*/
static void fifoLimitateRdPtr_Unprotected( Fifo_t* f ){
    f->pRd -= f->pRd < f->pLimit ? 0 : f->size;
}


/*!
\param f pointer to fifo struct
\param count a computed count (could be negative)
\return count if count was positive or with size corrected value as correct count
*/
static inline size_t adjustCount( Fifo_t const * const f, int count ){
    count = count < 0 ? count + (unsigned)(f->size) : count; //lint !e713 !e737
    return (size_t)count;
}


/*! fifo depth
\details a fifo of size n (n>1!) can hold only n-1 values to be able to distinct between empty and full
\param f pointer to fifo struct
\retval Count of bytes actually in fifo
*/
size_t FifoReadableCount_Unprotected( Fifo_t const * const f ){
    size_t n = adjustCount( f, (int)(f->pWr - f->pRd) );
    return n;
}

/*! fifo depth
\details a fifo of size n (n>1!) can hold only n-1 values to be able to distinct between empty and full
\param f pointer to fifo struct
\retval Count of bytes actually in fifo
*/
size_t FifoReadableCount_Protected( Fifo_t const * const f ){
    size_t n;
    TRICE_ENTER_CRITICAL_SECTION
    n = adjustCount( f, (int)(f->pWr - f->pRd) );
    TRICE_LEAVE_CRITICAL_SECTION
    return n;
}


/*! space left in fifo
\details a fifo of size n (n>1!) can hold only n-1 values to be able to distinct between empty and full
\param f pointer to fifo struct
\retval Count of bytes actually can be written into fifo
*/
size_t FifoWritableCount_Unprotected( Fifo_t const * const f ){
    return f->size - FifoReadableCount_Unprotected( f ) - 1; 
}


/*! space left in fifo
\details a fifo of size n (n>1!) can hold only n-1 values to be able to distinct between empty and full
\param f pointer to fifo struct
\retval Count of bytes actually can be written into fifo
*/
size_t FifoWritableCount_Protected( Fifo_t const * const f ){
    return f->size - FifoReadableCount_Protected( f ) - 1; 
}


/*! put value in fifo. fifo must have space for at least 1 byte.
\param f pointer to fifo struct
\param value uint8_t value to store
*/
void FifoPushUint8_Unprotected( Fifo_t* f, uint8_t value ){
    *(f->pWr++) = value;
    fifoLimitateWrPtr_Unprotected(f);
}


/*! put value in fifo. fifo must have space for at least 1 byte.
\param f pointer to fifo struct
\param value uint8_t value to store
*/
void FifoPushUint8_Protected( Fifo_t* f, uint8_t value ){
    TRICE_ENTER_CRITICAL_SECTION
    *(f->pWr++) = value;
    fifoLimitateWrPtr_Unprotected(f);
    TRICE_LEAVE_CRITICAL_SECTION
}


/*! Get an uint8_t value from fifo
\param f pointer to fifo struct
\param pValue address where to store value
*/
void FifoPopUint8_Unprotected( Fifo_t* f, uint8_t* pValue ){
    *pValue = *(f->pRd);
    f->pRd++;
    fifoLimitateRdPtr_Unprotected(f);
}

/*! Get an uint8_t value from fifo
\param f pointer to fifo struct
\param pValue address where to store value
*/
void FifoPopUint8_Protected( Fifo_t* f, uint8_t* pValue ){
    TRICE_ENTER_CRITICAL_SECTION
    *pValue = *(f->pRd);
    f->pRd++;
    fifoLimitateRdPtr_Unprotected(f);
    TRICE_LEAVE_CRITICAL_SECTION
}


#if 1
void FifoPushBuffer_Unprotected( Fifo_t* f, size_t count, const uint8_t* pBuff ){
    while(count-->0){
        FifoPushUint8_Unprotected( f, *pBuff++ );
    }
}

void FifoPushBuffer_Protected( Fifo_t* f, size_t count, const uint8_t* pBuff ){
    TRICE_ENTER_CRITICAL_SECTION
    while(count-->0){
        FifoPushUint8_Unprotected( f, *pBuff++ );
    }
    TRICE_LEAVE_CRITICAL_SECTION
}
#else
/*! Push count bytes into fifo. The value count must not be bigger than FifoWritableCount() and is not checked!
\param f pointer to fifo struct
\param count buffer size
\param pBuff address of buffer to be pushed into fifo
*/
void FifoPushBuffer_Unprotected( Fifo_t* f, size_t count, const uint8_t* pBuff ){
    size_t size = FifoWritableBlockSpace(f);
    if( size >= count ){ // 
        TRICE0( Id(17644), "dbg:all can be done in one step\n" );
        memcpy( f->pWr, pBuff, count );
    }else{ // 
        TRICE0( Id(10766), "dbg:need 2 steps\n" );
        memcpy( f->pWr, pBuff, size );
        memcpy( f->pBuff, pBuff+size, count-size );
    }
    f->pWr += count;
    fifoLimitateWrPtr_Unprotected(f);
}

/*! Push count bytes into fifo. The value count must not be bigger than FifoWritableCount() and is not checked!
This function is protected with a critical section
\param f pointer to fifo struct
\param count buffer size
\param pBuff address of buffer to be pushed into fifo
*/
void FifoPushBuffer_Protected( Fifo_t* f, size_t count, const uint8_t* pBuff ){
    TRICE_ENTER_CRITICAL_SECTION
    size_t size = FifoWritableBlockSpace(f);
    if( size >= count ){ // all can be done in one step
        memcpy( f->pWr, pBuff, count );
    }else{ // need 2 steps
        memcpy( f->pWr, pBuff, size );
        memcpy( f->pBuff, pBuff+size, count-size );
    }
    f->pWr += count;
    fifoLimitateWrPtr_Unprotected(f);
    TRICE_LEAVE_CRITICAL_SECTION
}
#endif


/*! Push uint32_t value into fifo. The fifo space is not checked!
\param f pointer to fifo struct
\param v value to be pushed into fifo
*/
void FifoPushUint32_Protected( Fifo_t* f, uint32_t v ){
    TRICE_ENTER_CRITICAL_SECTION
    FifoPushBuffer_Unprotected( f, sizeof(uint32_t), (uint8_t*)&v );
    TRICE_LEAVE_CRITICAL_SECTION
}


/*! Pop count bytes from fifo. The value count must not be bigger than FifoReadableCount() and is not checked!
\param f pointer to fifo struct
\param count buffer size
\param pBuff address of buffer to be popped from fifo
*/
void FifoPopBuffer_Unprotected( Fifo_t* f, size_t count, uint8_t* pBuff ){
    size_t size = FifoReadableBlockSpace(f);
    if( size >= count ){ // all can be done in one step
        memcpy( pBuff, f->pRd, count );
    }else{ // need 2 steps
        memcpy( pBuff, f->pRd,       size );
        memcpy( pBuff+size, f->pBuff,       count-size );
    }
    f->pRd += count;
    fifoLimitateRdPtr_Unprotected(f);
}


/*! Pop count bytes from fifo. The value count must not be bigger than FifoReadableCount() and is not checked!
\param f pointer to fifo struct
\param count buffer size
\param pBuff address of buffer to be popped from fifo
*/
void FifoPopBuffer_Protected( Fifo_t* f, size_t count, uint8_t* pBuff ){
    TRICE_ENTER_CRITICAL_SECTION
    size_t size = FifoReadableBlockSpace(f);
    if( size >= count ){ // all can be done in one step
        memcpy( pBuff, f->pRd, count );
    }else{ // need 2 steps
        memcpy( pBuff, f->pRd,       size );
        memcpy( pBuff+size, f->pBuff,       count-size );
    }
    f->pRd += count;
    fifoLimitateRdPtr_Unprotected(f);
    TRICE_LEAVE_CRITICAL_SECTION
}
