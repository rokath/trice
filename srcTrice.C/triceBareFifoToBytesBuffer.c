/*! \file triceBareFifoToBytesBuffer.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"
#include "triceBareFifoToEscFifo.h"
#include "triceBareFifo.h"
#include "triceEscFifo.h"

#define TRICE_FILENAME TRICE0( Id( 1716), "rd_:triceBareFifoToBytesBuffer.c" );

//! syncLevelLimit is the max amount of trices bulk transmitted without a sync trice injected.
//! Assuming triceServeBareFifoToBytesBuffer() is called every ms this is also the max ms abount without a sync trice.
//! Bigger numbers decrease the bandwidth needs but increase the possible loss of trices in case of
//! re-sync need. A number of 10-100 is recommended: every 10-100 ms or 10-100 trices a sync trice is transmitted
//! increasing the bandwidth need by 10-1% ans loosing 10 trices in the worst case.
int const syncLevelLimit = 10;


uint8_t triceBytesBuffer[8]; //!< bytes transmit buffer
int const triceBytesBufferIndexLimit = 8; // sizeof(triceBytesBuffer[8]);
int triceBytesBufferIndex = triceBytesBufferIndexLimit;


//! TODO: endianess with compiler macros.
TRICE_INLINE void triceLoadInNetworkOrder(uint8_t *p, uint32_t t) {
    // ID arrives in upper 16 bit in machine endianess
    // DA arrives in lower 16 bit in machine endianess (8 bit values are in trice ordering)

    // transmit in big endian
    uint8_t idHi = p[0] = (uint8_t)(t >> 24); // IDHi big endian
    uint8_t idLo = p[1] = (uint8_t)(t >> 16); // IDLo big endian

    // transmit in big endian   
    uint8_t daLo = p[2] = (uint8_t)(t >> 8); // DaLo big endian -> should be changed to >>24 (needs many changes in trice.h but does not influence trice go code)
    uint8_t daHi = p[3] = (uint8_t)(t >> 0); // DaHi big endian -> should be changed to >>16 (needs many changes in trice.h but does not influence trice go code)
}

TRICE_INLINE void triceTransfer(uint32_t t0, uint32_t t1) {
    triceLoadInNetworkOrder(&triceBytesBuffer[0], t0);
    triceLoadInNetworkOrder(&triceBytesBuffer[4], t1);
}

int triceBytesByfferDepth( void ){
    return triceBytesBufferIndexLimit - triceBytesBufferIndex;
}

//! triceServeBareFifoToBytesBuffer must be called cyclically to proceed ongoing write out.
//! It schould be called at least every ms.
//! A possibe place is main loop.
void triceServeBareFifoToBytesBuffer(void) {
    // 89 ab cd ef <- on serial port
    // ih il dh dl
    uint32_t const syncTrice = 0x89abcdef;
    static int syncLevel = syncLevelLimit; // start with a sync trice
    if (triceBytesBufferIndexLimit == triceBytesBufferIndex) { // bytes buffer empty and tx finished
        // next trice
        int n = triceBareFifoDepth();
        if (syncLevel < syncLevelLimit) { // no need for a sync trice
            if (0 == n) { // no trices to transmit
                syncLevel++;
                return;
            } else if (1 == n) { // one trice to transmit
                triceTransfer(tricePopBareFifo(), syncTrice);
                syncLevel = 0;
            } else { // at least 2 trices to transmit
                uint32_t t0 = tricePopBareFifo();
                uint32_t t1 = tricePopBareFifo();
                triceTransfer(t0, t1);
                syncLevel += 2;
            }
        } else { // need for a sync trice
            if (1 <= n) { // at least one trice, so transmit it and one sync trice
                triceTransfer(tricePopBareFifo(), syncTrice);
            } else { // nothing to transmit so transmit 2 sync trices
                triceTransfer(syncTrice, syncTrice);
            }
            syncLevel = 0;
        }
        triceBytesBufferIndex = 0;
    }
}
