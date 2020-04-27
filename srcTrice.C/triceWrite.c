/*! \file triceWrite.c
\author thomas.hoehenleitner [at] seerose.net
\brief 
\details 
*******************************************************************************/
#define TRICE_FILENAME TRICE0( Id(25337), "rd_:triceWrite.c" );

#include "trice.h"

#if TRICE_WRITE_FUNCTION == TRICE_IMPLEMENTATION

#define TRICE_WRITE_BUFFER_SIZE 1000
static uint8_t writeBuffer[TRICE_WRITE_BUFFER_SIZE]; 
static int writeCount = 0; //!< current transfer todo
static uint8_t* pNextByte = writeBuffer; //! location of next byte to transfer
static uint8_t* pNextBuffer = writeBuffer; //! location of next free buffer space
static uint8_t* const pStart = writeBuffer;
static uint8_t* const pLimit = writeBuffer + TRICE_WRITE_BUFFER_SIZE;



//! \param buf address to read from
//! \param nbytes count to write
//! \return count of written bytes
//! triceWrite is copying data into a separate buffer
static int triceWrite_(const void *buf, int nbytes){
    if( 0 == writeCount ){ // all was done
        memcpy( pStart, buf, nbytes );
        pNextByte = pStart;
        pNextBuffer = pStart + nbytes;
        writeCount = nbytes;
        return nbytes;
    } else { // ongoing transfer
        int cnt;
        if( pNextBuffer < pLimit ){ // some space until end of buffer
            cnt = pNextBuffer + nbytes - pLimit;
            if( cnt <= 0 ) { // will fit
                memcpy( pNextBuffer, buf, nbytes );
                pNextBuffer += nbytes;
                writeCount += nbytes;
                return nbytes;
            } 
            // will not fit, cnt contains the not fitting count at this point
            cnt = nbytes - cnt;
            memcpy( pNextBuffer, buf, cnt );
            pNextBuffer = pLimit;
            writeCount += cnt;
            return cnt;
        }
        // no space until end of buffer
        cnt = pNextByte - pStart; // space right now
        cnt = cnt > nbytes ? nbytes : cnt; // limitate
        memcpy( pStart, buf, cnt );
        pNextBuffer = pStart + cnt;
        writeCount += cnt;
        return cnt;
    }
}

//! \param buf address to read from
//! \param nbytes count to write
//! \return count of written bytes
//! triceWrite is copying data into a separate buffer
int triceWrite(const void *buf, int nbytes){
    int cnt = triceWrite_(buf, nbytes);
    if( cnt == nbytes ){
        return cnt; // done
    }
    // ok, it could have been the internal buffer limit, so trying again
    uint8_t* p = (uint8_t*)buf;
    cnt = triceWrite_(p+cnt, nbytes-cnt);
    return cnt;
}

//! triceWriteServer() must be called cyclically to proceed ongoing write out
//! best place: sysTick ISR and UART ISR (both together)
void triceWriteServer( void ){
    uint8_t v;
    if( !triceTxDataRegisterEmpty() ){ 
        return;
    }
    if( 0 == writeCount ){
        extern int txState; txState = 0; // noTx; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        triceDisableTxEmptyInterrupt();
        return;
    }
    pNextByte = pNextByte < pLimit ? pNextByte : pStart;
    v = *pNextByte++;
    triceTransmitData8( v );
    writeCount--;
    triceEnableTxEmptyInterrupt();
}


#endif // #if TRICE_WRITE_FUNCTION == TRICE_IMPLEMENTATION

