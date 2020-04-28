/*! \file triceWrite.c
\author thomas.hoehenleitner [at] seerose.net
\brief 
\details 
*******************************************************************************/
#define TRICE_FILENAME TRICE0( Id(60411), "rd_:triceWrite.c" );

#include <string.h> // memcpy
#include "triceConfig_template.h"
#include "triceWrite.h"
#include "trice.h"

ALIGN4 static uint8_t writeBuffer[TRICE_WRITE_BUFFER_SIZE]                  ALIGN4_END; 
ALIGN4 static uint8_t* pNextByte = writeBuffer                              ALIGN4_END; //!< pNextByte is pointing to the next buffer space in usage
ALIGN4 static uint8_t* const pStart = writeBuffer                           ALIGN4_END; //!< begin of write buffer
ALIGN4 static uint8_t* const pLimit = writeBuffer + TRICE_WRITE_BUFFER_SIZE ALIGN4_END; //!< first address after write buffer

uint16_t writeCount = 0; //!< writeCount is the size of the used buffer space
uint16_t writeCountMax = 0;

//! triceWrite is copying data into a separate buffer fot transmission
//! \param buf address to read from
//! \param nbytes count to write
//! \return count of written bytes
static int triceWrite_(const void *buf, int nbytes){
    int cnt = nbytes < TRICE_WRITE_BUFFER_SIZE ? nbytes : TRICE_WRITE_BUFFER_SIZE; // limitate;
    uint8_t* pNextSpace = pNextByte + writeCount; // pNextSpace is pointing to the beginning of the next free buffer space.
    int space; // usable continuous space
    if( pNextSpace < pLimit ){ // usable space goes from pNextSpace to pLimit
        // |________#########_____________|
        //                                ^---pLimit
        //                   ^----------------pNextSpace
        //          ^-------------------------pNextByte
        // ^----------------------------------pStart
        // Only the right (upper) free space is usble right now.
        // The left (lower) free space gets used with the 2nd call of triceWrite_().
        space = pLimit - pNextSpace;
    } else { // wraping
        pNextSpace -= TRICE_WRITE_BUFFER_SIZE; // usable space goes from pNextSpace to pNextByte
        // |########_________#############|
        //                                ^---pLimit
        //                   ^----------------pNextByte
        //          ^-------------------------pNextSpace
        // ^----------------------------------pStart
        space = pNextByte - pNextSpace; // same as TRICE_WRITE_BUFFER_SIZE - writeCount
    }
    cnt = cnt < space ? cnt : space;
    memcpy( pNextSpace, buf, cnt );
    writeCount += cnt;
    if(writeCount > writeCountMax){
        writeCountMax = writeCount;
    }
    return cnt;
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
        triceDisableTxEmptyInterrupt();
        return;
    }
    pNextByte = pNextByte < pLimit ? pNextByte : pStart; // handle wrap
    v = *pNextByte++;
    writeCount--;
    triceTransmitData8( v );
    triceEnableTxEmptyInterrupt();
}
