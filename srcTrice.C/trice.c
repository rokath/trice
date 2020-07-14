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

//! partial prefilled trice message transmit buffer
static ALIGN4 triceMsg_t triceMsg ALIGN4_END = {
    { TRICE_START_BYTE,  TRICE_LOCAL_ADDR,  TRICE_DISPL_ADDR, 0 }, // crc8
    {{ 0, 0 }} // 16bit ID, 16bit data
}; // https://stackoverflow.com/questions/13746033/how-to-repair-warning-missing-braces-around-initializer



#if defined( TRICE_FIFO_SIZE ) && TRICE_FIFO_SIZE > 0

//! trice fifo instance, here are the trices buffered. used in TRICE macro expansion
ALIGN4 uint32_t triceFifo[ TRICE_FIFO_SIZE>>2 ] ALIGN4_END;

uint32_t wrIndexTriceFifo = 0; //!< trice fifo write index, used inside macros, so must be visible

static uint32_t rdIndexTriceFifo = 0; //!< trice fifo read index

//! get one trice from trice fifo
//! am p address for trice id with 2 byte data
TRICE_INLINE void triceFifoPop( void* p ){
    uint32_t* up = p;
    *up = triceFifo[rdIndexTriceFifo++];
    rdIndexTriceFifo &= TRICE_FIFO_MASK;
}

//uint32_t maxTrices = 0; //!< for max trices depth diagnostics
uint32_t maxTriceDepth = 0; //!< for max trices depth diagnostics

//! trice item count inside trice fifo
//! \return count of buffered trices
static unsigned triceFifoDepth( void ){
    unsigned triceDepth = (wrIndexTriceFifo - rdIndexTriceFifo) & TRICE_FIFO_MASK;
    maxTriceDepth = triceDepth < maxTriceDepth ? maxTriceDepth : triceDepth; // diagnostics
    return triceDepth;
}

static void prepareNextTriceTransmission(void);

void triceToWriteBuffer( void ){
    uint32_t depth = triceFifoDepth();
    if( depth ){ // trice for transfer available
        if( triceWriteSpace() < TRICE_WRITE_SPACE_MIN){ // no space in write buffer
            return; // don't transfer trices if buffer is too full
        }
        triceFifoPop( &(triceMsg.ld) );
        prepareNextTriceTransmission();
        triceWrite( &triceMsg, sizeof(triceMsg) );
    }
}


void triceBareToWriteBuffer( void ){
    uint32_t depth = triceFifoDepth();
    if( depth ){ // trice for transfer available
        if( triceWriteSpace() < TRICE_WRITE_SPACE_MIN){ // no space in write buffer
            return; // don't transfer trices if buffer is too full
        }
        {
            uint32_t trice;
            triceFifoPop( &trice );
            triceWrite( &trice, sizeof(trice) );
        }
    }
}

#endif

//! pull next trice from fifo and prepare triceMsg buffer
static void prepareNextTriceTransmission(void){
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

//! put one trice into trice fifo
//! \param v trice id with 2 byte data
//! trice time critical part
void tricePush( uint32_t v ){
    triceMsg.ld.atomicTrice = v;
    prepareNextTriceTransmission();
    triceWrite( &triceMsg, sizeof(triceMsg) );
}

#endif // #if TRICE_CODE
