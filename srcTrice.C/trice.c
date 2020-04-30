/*! \file trice.c
\details The format strings are ignored - they go not into the target binary. See trice tool doc for details.
The trices (macros) are dumped as 32bit values into a 32 bit fifo. That is the time critical part. 
\li A basic trice (subtrace) consists always of 4 byte: a 16 bit ID with a 16 bit data value.
\li Trices with more data are split into several 32bit basic trice values with IDs = 0 in front.
\li TRICE0, TRICE8_1, TRICE8_2 and TRICE16_1 have 4 bytes size, all others have 8 - 32 bytes size.
\li For trice transmission each basic trice gets a header of additional 4 bytes.
\li The header contains a startbyte, client and server address and a crc8 exOr checksum.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"
#ifdef ENCRYPT
#include "xteaCrypto.h"
#endif

#if TRICE_CODE

//! trice fifo instance, here are the trices buffered. used in TRICE macro expansion
ALIGN4 uint32_t triceFifo[ TRICE_FIFO_SIZE>>2 ] ALIGN4_END;

uint32_t wrIndexTriceFifo = 0; //!< trice fifo write index, used inside macros, so must be visible

static uint32_t rdIndexTriceFifo = 0; //!< trice fifo read index

//! get one trice from trice fifo
//! am p address for trice id with 2 byte data
TRICE_INLINE void triceFifoPop( uint32_t* p ){
    *p = triceFifo[rdIndexTriceFifo++];
    rdIndexTriceFifo &= TRICE_FIFO_MASK;
}

//! trice item count inside trice fifo
//! \return count of buffered trices
TRICE_INLINE size_t triceFifoDepth( void ){
    return (wrIndexTriceFifo - rdIndexTriceFifo) & TRICE_FIFO_MASK;
}

//! partial prefilled trice message transmit buffer 
ALIGN4 static triceMsg_t triceMsg ALIGN4_END = {
    { TRICE_START_BYTE,  TRICE_LOCAL_ADDR,  TRICE_DISPL_ADDR, 0 }, // crc8
    { 0, 0 } // 16bit ID, 16bit data
};

//! pull next trice from fifo and prepare triceMsg buffer
static void prepareNextTriceTransmission(void){
    triceFifoPop( (uint32_t*)(&(triceMsg.ld)) );
    triceMsg.hd.crc8  = TRICE_START_BYTE ^ TRICE_LOCAL_ADDR ^ TRICE_DISPL_ADDR
                         ^ triceMsg.ld.load[0]
                         ^ triceMsg.ld.load[1]
                         ^ triceMsg.ld.load[2]
                         ^ triceMsg.ld.load[3];
    #ifdef ENCRYPT
        triceMsg.hd.start = TRICE_START_BYTE;
        triceMsg.hd.cad  = TRICE_LOCAL_ADDR;
        triceMsg.hd.sad  = TRICE_DISPL_ADDR;
        encrypt( (uint8_t*)&triceMsg );
    #endif
}

uint32_t maxTrices = 0; //!< for max trices depth diagnostics

static void triceTX( void ){
    uint32_t depth = triceFifoDepth();
    if( depth ){
        #ifdef USE_OWN_TRICE_WRITE_FUNCTION
        if( writeCount >= TRICE_WRITE_COUNT_LIMIT){
            return; // don't transfer trices if too buffer is too full
        }
        #endif
        prepareNextTriceTransmission();
        triceWrite( &triceMsg, sizeof(triceMsg) );
        maxTrices = depth < maxTrices ? maxTrices : depth;
    }
}


//! This function should be called inside the transmit done device interrupt.
//! Also it should be called cyclically to trigger transmission start, for example in the sysTick interrupt.
//! If not interrup is used it should be called cyclically. With each call max 1 byte is transmitted.
void TriceServeTransmission( void ){
    #ifdef USE_OWN_TRICE_WRITE_FUNCTION
    triceWriteServer();
    #endif
    triceTX();
}

#endif // #if TRICE_CODE
