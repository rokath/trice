/*! \file triceWrite.c
\author thomas.hoehenleitner [at] seerose.net
\brief 
\details 
*******************************************************************************/
#define TRICE_FILENAME TRICE0( Id(60411), "rd_:triceWrite.c" );

#include <string.h> // memcpy
#include "triceConfig.h"
#include "trice.h"

ALIGN4 static uint8_t pStart[TRICE_WRITE_BUFFER_SIZE] ALIGN4_END; //!< write out buffer
static uint8_t* const pLimit = pStart + TRICE_WRITE_BUFFER_SIZE;  //!< first address after write buffer
static uint8_t*       pNext = pStart;                             //!< pNext is pointing to the next buffer space in usage

uint16_t writeCount = 0; //!< writeCount is the size of the used buffer space, it is used externally to control the write buffer fill state
uint16_t writeCountMax = 0; //!< this value is used to report externally the max used write buffer size to adjust TRICE_WRITE_BUFFER_SIZE properly


unsigned triceWriteSpace( void ){
    return TRICE_WRITE_BUFFER_SIZE - writeCount;
}


//! triceWrite is copying data into a separate buffer fot transmission
//! \param buf address to read from
//! \param nbytes count to write
//! \return count of written bytes
static int triceWrite_(const void *buf, int nbytes){
    int cnt = nbytes < TRICE_WRITE_BUFFER_SIZE ? nbytes : TRICE_WRITE_BUFFER_SIZE; // limitate;
    uint8_t* pNextSpace = pNext + writeCount; // pNextSpace is pointing to the beginning of the next free buffer space.
    int space; // usable continuous space         // pNextSpace can point behind pLimit, what needs to be handled.
    if( pNextSpace < pLimit ){ // usable space goes from pNextSpace to pLimit and from pStart to pNext
        // |________#########_____________|
        //                                ^---pLimit
        //                   ^----------------pNextSpace
        //          ^-------------------------pNext
        // ^----------------------------------pStart
        // Only the right (upper) free space is usble right now.
        // The left (lower) free space gets used with the 2nd call of triceWrite_().
        space = pLimit - pNextSpace;
    } else { // handle wraping
        pNextSpace -= TRICE_WRITE_BUFFER_SIZE; // usable space goes from pNextSpace to pNext
        // |########_________#############|
        //                                ^---pLimit
        //                   ^----------------pNext
        //          ^-------------------------pNextSpace
        // ^----------------------------------pStart
        space = pNext - pNextSpace; // this is same as TRICE_WRITE_BUFFER_SIZE - writeCount
    }
    cnt = cnt < space ? cnt : space; // limitate
    memcpy( pNextSpace, buf, cnt );
    TRICE_ENTER_CRITICAL_SECTION
    writeCount += cnt;
    TRICE_LEAVE_CRITICAL_SECTION
    if(writeCount > writeCountMax){ // watch max filling
        writeCountMax = writeCount;
    }
    return cnt;
}

//! \param buf address to read from
//! \param nbytes count to write
//! \return count of written bytes
//! triceWrite is copying data into a separate buffer
unsigned triceWrite(const void *buf, int nbytes){
    uint8_t* p;
    int cnt = triceWrite_(buf, nbytes);
    if( cnt == nbytes ){
        return cnt; // done
    }
    // ok, it could have been the internal buffer limit, so trying again
    p = (uint8_t*)buf;
    cnt = triceWrite_(p+cnt, nbytes-cnt);
    return cnt;
}

#ifdef LL_INTERFACE_NO_INTERRUPTS
//! triceWriteServer() must be called cyclically to proceed ongoing write out
//! best place: sysTick ISR and UART ISR (both together)
static void triceWriteBufferOut( void ){
    uint8_t v;
    if( !triceTxDataRegisterEmpty() ){ 
        return;
    }
    if( 0 == writeCount ){
        return;
    }
    //TRICE_ENTER_CRITICAL_SECTION
        v = *pNext++;
        pNext = pNext < pLimit ? pNext : pStart; // handle wrap
        writeCount--;
    //TRICE_LEAVE_CRITICAL_SECTION
    triceTransmitData8( v );
}
#endif // #ifdef LL_INTERFACE


#ifdef LL_INTERFACE_WITH_INTERRUPTS
//! triceWriteServer() must be called cyclically to proceed ongoing write out
//! best place: sysTick ISR and UART ISR (both together)
static void triceWriteBufferOut( void ){
    uint8_t v;
    if( !triceTxDataRegisterEmpty() ){ 
        return;
    }
    if( 0 == writeCount ){
        triceDisableTxEmptyInterrupt();
        return;
    }
    //TRICE_ENTER_CRITICAL_SECTION
        v = *pNext++;
        pNext = pNext < pLimit ? pNext : pStart; // handle wrap
        writeCount--;
    //TRICE_LEAVE_CRITICAL_SECTION
    triceTransmitData8( v );
    triceEnableTxEmptyInterrupt();
}
#endif // #ifdef LL_INTERFACE_WITH_INTERRUPTS

#ifdef HAL_INTERFACE_BLOCKING_MODE

//! triceWriteServer() must be called cyclically to proceed ongoing write out
//! best place: sysTick ISR and UART ISR (both together)
static void triceWriteBufferOut( void ){
    if( writeCount ){
    TRICE_ENTER_CRITICAL_SECTION
        int nbytes = pLimit - pNext;
        nbytes = nbytes < writeCount ? nbytes : writeCount;
        HAL_UART_Transmit( TRICE_UART_HANDLE_PTR, pNext, nbytes, 0xffffffff); // blocking
        writeCount -= nbytes;
        pNext += nbytes;
        pNext -= pNext < pLimit ? 0 : TRICE_WRITE_BUFFER_SIZE; // handle wrap
    TRICE_LEAVE_CRITICAL_SECTION
    }
}

#endif // #ifdef HAL_INTERFACE_BLOCKING_MODE

#ifdef HAL_INTERFACE_INTERRUPT_MODE

//! triceWriteServer() must be called cyclically to proceed ongoing write out
//! best place: sysTick ISR and UART ISR (both together)
static void triceWriteBufferOut( void ){
    if( writeCount && (HAL_UART_GetState(TRICE_UART_HANDLE_PTR) != HAL_UART_STATE_BUSY_TX) ){
    TRICE_ENTER_CRITICAL_SECTION
        int nbytes = pLimit - pNext;
        nbytes = nbytes < writeCount ? nbytes : writeCount;
        HAL_UART_Transmit_IT(TRICE_UART_HANDLE_PTR, pNext, nbytes); // ok, needs enabled UART2 interrupt
        writeCount -= nbytes;
        pNext += nbytes;
        pNext -= pNext < pLimit ? 0 : TRICE_WRITE_BUFFER_SIZE; // handle wrap
    TRICE_LEAVE_CRITICAL_SECTION
    }
}

#endif // #ifdef HAL_INTERFACE_INTERRUPT_MODE

#ifdef HAL_INTERFACE_DMA_MODE

//! triceWriteServer() must be called cyclically to proceed ongoing write out
//! best place: sysTick ISR and UART ISR (both together)
static void triceWriteBufferOut( void ){
    if( writeCount && ( HAL_UART_STATE_READY == TRICE_UART_HANDLE_PTR->gState ) ){
    TRICE_ENTER_CRITICAL_SECTION
        int nbytes = pLimit - pNext;
        nbytes = nbytes < writeCount ? nbytes : writeCount;
        HAL_UART_Transmit_DMA(TRICE_UART_HANDLE_PTR, pNext, nbytes); // ok, needs added DNA channel to USART2_TX
        writeCount -= nbytes;
        pNext += nbytes;
        pNext -= pNext < pLimit ? 0 : TRICE_WRITE_BUFFER_SIZE; // handle wrap
    TRICE_LEAVE_CRITICAL_SECTION
    }
}

#endif // #ifdef HAL_INTERFACE_DMA_MODE

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
    unsigned tick = TRICE_SERVER_TICK;
    static int lastTransfer = 0;
    static int lastServe = 0;
#if TRICE_TRANSFER_PERIOD
    if( tick > lastTransfer + TRICE_TRANSFER_PERIOD ){
        triceToWriteBuffer();
        lastTransfer = tick;
    }
#endif
#if TRICE_SERVE_PERIOD
    if( tick > lastServe + TRICE_SERVE_PERIOD ){
        triceWriteBufferOut();
        lastServe = tick;
    }
#endif
}
