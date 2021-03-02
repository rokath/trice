/*! \file triceFifoToBytesBuffer.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"
#include "intern/triceFifo.h"
#include "intern/triceFifoToBytesBuffer.h"

#define TRICE_FILENAME TRICE0( Id(65118), "rd_:triceFifoToBytesBuffer.c" );

//! syncLevelLimit is the max amount of trices bulk transmitted without a sync trice injected.
//! Assuming triceServeBareFifoToBytesBuffer() is called every ms this is also the max ms abount without a sync trice.
//! Bigger numbers decrease the bandwidth needs but increase the possible loss of trices in case of
//! re-sync need. A number of 10-100 is recommended: every 10-100 ms or 10-100 trices a sync trice is transmitted.
//! It increases the bandwidth need by 10-1%.
int const syncLevelLimit = 100;

#ifndef TRICE_WRAP_START_BYTE
#define TRICE_WRAP_START_BYTE 0
#endif

#ifndef TRICE_WRAP_LOCAL_ADDR
#define TRICE_WRAP_LOCAL_ADDR 0
#endif

#ifndef TRICE_WRAP_DEST_ADDR
#define TRICE_WRAP_DEST_ADDR 0
#endif



uint8_t triceBytesBuffer[8] = { TRICE_WRAP_START_BYTE, TRICE_WRAP_LOCAL_ADDR, TRICE_WRAP_DEST_ADDR, 0, 0,0,0,0 }; //!< bytes transmit buffer (prefilled for wrap
int const triceBytesBufferIndexLimit = 8; // sizeof(triceBytesBuffer[8]);
int triceBytesBufferIndex = triceBytesBufferIndexLimit;

TRICE_INLINE void triceTransfer(uint32_t t0, uint32_t t1) {
    *(uint32_t*)&triceBytesBuffer[0] = t0;
    *(uint32_t*)&triceBytesBuffer[4] = t1;
#ifdef ENCRYPT
    encrypt( triceBytesBuffer );
#endif
}

int triceBytesByfferDepth( void ){
    return triceBytesBufferIndexLimit - triceBytesBufferIndex;
}

void triceServeFifoSyncedToBytesBuffer(void) {
    // 89 ab cd ef <- on serial port
    // ih il dh dl
    uint32_t const syncTrice = TRICE_HTON(0x89abcdef);
    static int syncLevel = syncLevelLimit; // start with a sync trice
    if (triceBytesBufferIndexLimit == triceBytesBufferIndex) { // bytes buffer empty and tx finished
        // next trice
        int n = triceU32FifoDepth();
        if (syncLevel < syncLevelLimit) { // no need for a sync trice
            if (0 == n) { // no trices to transmit
                syncLevel++;
                return;
            } else if (4 == n) { // one trice to transmit
                triceTransfer(triceU32Pop(), syncTrice);
                syncLevel = 0;
            } else { // at least 2 trices to transmit
                uint32_t t0 = triceU32Pop();
                uint32_t t1 = triceU32Pop();
                triceTransfer(t0, t1);
                syncLevel += 2;
            }
        } else { // need for a sync trice
            if (4 <= n) { // at least one trice, so transmit it and one sync trice
                triceTransfer(triceU32Pop(), syncTrice);
            } else { // nothing to transmit so transmit 2 sync trices
                triceTransfer(syncTrice, syncTrice);
            }
            syncLevel = 0;
        }
        triceBytesBufferIndex = 0;
    }
}

void triceServeFifoWrappedToBytesBuffer(void) {
    if (triceBytesBufferIndexLimit == triceBytesBufferIndex) { // bytes buffer empty and tx finished
        // next trice
        int n = triceU32FifoDepth();
        if ( n >= 4 ) { // a trice to transmit
            uint32_t x = triceU32Pop();
#ifdef ENCRYPT // need to re-init buffer
            triceBytesBuffer[0] = TRICE_WRAP_START_BYTE;
            triceBytesBuffer[1] = TRICE_WRAP_LOCAL_ADDR;
            triceBytesBuffer[2] = TRICE_WRAP_DEST_ADDR;
#endif
            triceBytesBuffer[3] = (uint8_t)( TRICE_WRAP_START_BYTE ^ TRICE_WRAP_LOCAL_ADDR ^ TRICE_WRAP_DEST_ADDR ^ x ^ (x>>8) ^ (x>>16) ^ (x>>24) ); // crc8
            
            *(uint32_t*)&triceBytesBuffer[4] = x; //triceLoadInNetworkOrder(&triceBytesBuffer[4], x);
#ifdef ENCRYPT
            encrypt( triceBytesBuffer );
#endif
            triceBytesBufferIndex = 0;
        }
    }
}
