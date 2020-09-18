/*! \file triceEscFifoUART.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include <string.h> // memcpy
#include "trice.h"
#include "triceEscFifoUART.h"

#define TRICE_FILENAME TRICE0( Id(27621), "rd_:triceEscFifoUART.c" );

//! TRICE_FIFO_BYTE_SIZE must be a power of 2, one trice needs 4 to 32 or one day more bytes.
//! The fifo has to hold trice bursts until they are transmitted.
//! It is transmitted with lower priority in the background for example with the UART tx interrupt.
#define TRICE_FIFO_BYTE_SIZE 256

//#define TRICE_FIFO_MASK (((TRICE_FIFO_BYTE_SIZE)>>2)-1)

ALIGN4 static uint8_t pStart[TRICE_FIFO_BYTE_SIZE] ALIGN4_END; //!< write out buffer
static uint8_t* const pLimit = pStart + TRICE_FIFO_BYTE_SIZE;  //!< first address after write buffer
static uint8_t*       pNext = pStart;                          //!< pNext is pointing to the next buffer space in usage

uint16_t writeCount = 0; //!< writeCount is the size of the used buffer space, it is used externally to control the write buffer fill state
int triceFifoMaxDepth = 0; //!< this value is used to report externally the max used write buffer size to adjust TRICE_WRITE_BUFFER_SIZE properly

unsigned triceWriteAvailSpace( void ){
    return TRICE_FIFO_BYTE_SIZE - writeCount;
}

int triceEscFifoDepth( void ){
    return writeCount;
}


//! triceWrite_ is a helper for copying data into a separate buffer for transmission.
//! \param buf address to read from
//! \param nbytes count to write
//! \return count of written bytes
static int triceWrite_(const void *buf, int nbytes){
    int cnt = nbytes < TRICE_FIFO_BYTE_SIZE ? nbytes : TRICE_FIFO_BYTE_SIZE; // limitate;
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
        pNextSpace -= TRICE_FIFO_BYTE_SIZE; // usable space goes from pNextSpace to pNext
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
    if(writeCount > triceFifoMaxDepth){ // watch max filling
        triceFifoMaxDepth = writeCount;
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

uint8_t triceReadByte(void){
    writeCount--;
    uint8_t *p = pNext - writeCount;
    p = p < pStart ? p + TRICE_FIFO_BYTE_SIZE : p;
    return *p;
}
#define ESC  0xEC
#define DEL  0xDE
#define P0   0xDF
#define P1   0xE0
#define P2   0xE1
#define P4   0xE2
#define P8   0xE3
#define P16  0xE4
#define P32  0xE5
#define P64  0xE6
#define P128 0xE7
#define P256 0xE8
#define P512 0xE9
#define P1K  0xEA
#define P2K  0xEB

#define BY(d) ((uint8_t)(d))

#define HI(v) BY(((uint16_t)(v))>>8)
#define LO(v) BY(v))

#define HH(v) BY(((uint32_t)(v))>>24)
#define HL(v) BY(((uint32_t)(v))>>16)
#define LH(v) BY(((uint32_t)(v))>> 8)
#define LL(v) BY(v)

#define HHH(v) BY(((uint64_t)(v))>>56)
#define HHL(v) BY(((uint64_t)(v))>>48)
#define HLH(v) BY(((uint64_t)(v))>>40)
#define HLL(v) BY(((uint64_t)(v))>>32)
#define LHH(v) BY(((uint64_t)(v))>>24)
#define LHL(v) BY(((uint64_t)(v))>>16)
#define LLH(v) BY(((uint64_t)(v))>> 8)
#define LLL(v) BY(v)




//! trace Id and string protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param dynString 0-terminated runtime generated string
#define TRICE_S( Id, pFmt, dynString ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    int sLen = strlen(dynString); \
    int bufLen = sLen+1; \
    int n = -1; \
    while( sLen >> ++n ); {\
    uint8_t lc = P0 + n; \
    uint8_t msg[] = {ESC, P0 + n, HI(Id), LO(Id) } \
    triceWrite( &msg, sizeof(msg) ); \
    triceWrite( dynString, bufLen ); \
    triceWrite( padding, (1<<(n-1)) - bufLen); } \
    TRICE_LEAVE_CRITICAL_SECTION \
    } while(0) 
// example: ""         =                                   0 -> bufLen=1 -> n=0, (1<<0)= 1, padding=0
// example: "a"        = 'a'                               0 -> bufLen=2 -> n=1, (1<<1)= 2, padding=0
// example: "ab"       = 'a' 'b'                           0 -> bufLen=3 -> n=2, (1<<2)= 4, padding=1
// example: "abc"      = 'a' 'b' 'c'                       0 -> bufLen=4 -> n=2, (1<<2)= 4, padding=0
// example: "abcd"     = 'a' 'b' 'c' 'd'                   0 -> bufLen=5 -> n=3, (1<<8)= 8, padding=3
// example: "abcdef"   = 'a' 'b' 'c' 'd' 'e'               0 -> bufLen=6 -> n=3, (1<<8)= 8, padding=2
// example: "abcdef"   = 'a' 'b' 'c' 'd' 'e' 'f'           0 -> bufLen=7 -> n=3, (1<<8)= 8, padding=1
// example: "abcdefg"  = 'a' 'b' 'c' 'd' 'e' 'f' 'g'       0 -> bufLen=8 -> n=3, (1<<8)= 8, padding=0
// example: "abcdefgh" = 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h'   0 -> bufLen=9 -> n=4, (1<<4)=16, padding=7



//! trace Id protected (outside critical section), 16 bit data are 0
//! \param Id trice identifier
//! \param pFmt formatstring for trice
#define ETRICE0( Id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    uint8_t msg[] = {ESC, P0, HI(Id), LO(Id)}; \
    triceWrite( &msg, sizeof(msg) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)



//! trace Id protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define ETRICE8_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    uint8_t msg[] = {ESC, P1, HI(Id), LO(Id), \
        BY(d0)}; \
    triceWrite( &msg, sizeof(msg) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


//! trace Id and 8-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define ETRICE8_2( Id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    uint8_t msg[] = {ESC, P2, HI(Id), LO(Id), \
        BY(d0), BY(d1)}; \
    triceWrite( &msg, sizeof(msg) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! trace Id and 16-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define ETRICE16_1( Id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    uint8_t msg[] = {ESC, P2, HI(Id), LO(Id), \
        HI(d0), LO(d0)}; \
    triceWrite( &msg, sizeof(msg) ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define ETRICE32_4( Id, pFmt, d0, d1, d2, d3 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    uint8_t msg[] = {ESC, P16, HI(Id), LO(Id), \
        HH(d0), HL(d0), LH(d0), LL(d0), \
        HH(d1), HL(d1), LH(d1), LL(d1), \
        HH(d2), HL(d2), LH(d2), LL(d2), \
        HH(d3), HL(d3), LH(d3), LL(d3)}; \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)



//! trace Id and 64-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define ETRICE64_2( Id, pFmt, d0, d1 ) do{ \
    uint64_t x0 = (uint64_t)d0, x1 = (uint64_t)d1; \
    TRICE_ENTER_CRITICAL_SECTION \
    uint8_t msg[] = {ESC, P16, HI(Id), LO(Id), \
        HHH(d0), HHL(d0), HLH(d0), HLL(d0), \
        LHH(d0), LHL(d0), LLH(d0), LLL(d0), \
        HHH(d1), HHL(d1), HLH(d1), HLL(d1), \
        LHH(d1), LHL(d1), LLH(d1), LLL(d1)}; \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)


//! trice fifo instance, here are the trices buffered.
ALIGN4 uint32_t triceFifo[ TRICE_FIFO_BYTE_SIZE>>2 ] ALIGN4_END;

uint32_t triceFifoWriteIndex = 0; //!< trice fifo write index, used inside macros, so must be visible
uint32_t triceFifoReadIndex = 0; //!< trice fifo read index
uint32_t triceFifoMaxDepthTrices = 0; //!< diagnostics
uint8_t  triceBytesBuffer[8]; //!< bytes transmit buffer
int const triceBytesBufferIndexLimit = 8; // sizeof(triceBytesBuffer[8]);
int triceBytesBufferIndex = triceBytesBufferIndexLimit;

//! TODO: endianess with compiler macros.
TRICE_INLINE void triceLoadInNetworkOrder( uint8_t* p, uint32_t t ){
    // ID arrives in upper 16 bit in machine endianess
    // DA arrives in lower 16 bit in machine endianess (8 bit values are in trice ordering)

    // transmit in big endian
    uint8_t idHi = p[0] = (uint8_t)(t>>24); // IDHi big endian
    uint8_t idLo = p[1] = (uint8_t)(t>>16); // IDLo big endian
    
    // transmit in big endian   
    uint8_t daLo = p[2] = (uint8_t)(t>> 8); // DaLo big endian -> should be changed to >>24 (needs many changes in trice.h but does not influence trice go code)
    uint8_t daHi = p[3] = (uint8_t)(t>> 0); // DaHi big endian -> should be changed to >>16 (needs many changes in trice.h but does not influence trice go code)
}

TRICE_INLINE void triceTransfer( uint32_t t0, uint32_t t1 ){
    triceLoadInNetworkOrder( &triceBytesBuffer[0], t0 );
    triceLoadInNetworkOrder( &triceBytesBuffer[4], t1 );
}

//! triceFifoDepth determines trices count inside trice fifo.
//! \return count of buffered trices
TRICE_INLINE unsigned triceFifoDepth( void ){
    unsigned triceDepth = (triceFifoWriteIndex - triceFifoReadIndex) & TRICE_FIFO_MASK;
    triceFifoMaxDepthTrices = triceDepth < triceFifoMaxDepthTrices ? triceFifoMaxDepthTrices : triceDepth; // diagnostics
    return triceDepth;
}

//! triceServeOut must be called cyclically to proceed ongoing write out.
//! It schould be called at least every ms.
//! A possibe place is main loop.
void triceServeOut( void ){
    // 89 ab cd ef <- on serial port
    // ih il dh dl
    uint32_t const syncTrice = 0x89abcdef; // endianess!! ID is in low part
    static int syncLevel = TRICE_BARE_SYNC_LEVEL; // start with a sync trice
    if( triceBytesBufferIndexLimit == triceBytesBufferIndex ){ // bytes buffer empty and tx finished
        // next trice
        int n = triceFifoDepth();
        if( syncLevel < TRICE_BARE_SYNC_LEVEL ){ // no need for a sync trice
            if( 0 == n ) { // no trices to transmit
                syncLevel++; 
                return;
            } else if( 1 == n ){ // one trice to transmit
                triceTransfer( tricePop(), syncTrice );
                syncLevel = 0;
            } else { // at least 2 trices to transmit
                uint32_t t0 = tricePop();
                uint32_t t1 = tricePop();
                triceTransfer( t0, t1 );
                syncLevel+=2;
            }
        } else { // need for a sync trice
            if( 1 <= n ){ // at least one trice, so transmit it and one sync trice
                triceTransfer( tricePop(), syncTrice );
            } else { // nothing to transmit so transmit 2 sync trices
                triceTransfer( syncTrice, syncTrice );
            }
            syncLevel = 0;
        }
        triceBytesBufferIndex = 0;
        // next byte
        triceTransmitData8( triceBytesBuffer[triceBytesBufferIndex++] );
        triceEnableTxEmptyInterrupt();
    }
}

//! triceServeTransmit() must be called cyclically to proceed ongoing write out.
//! A good place: sysTick ISR and UART ISR (both together).
//! TODO: endianess with compiler macros.
void triceServeTransmit( void ){
    if( !triceTxDataRegisterEmpty() ){
        for(;;); // unexpected case
    }
    if( triceBytesBufferIndexLimit == triceBytesBufferIndex ){
        for(;;); // unexpected case
    }
    // next byte
    triceTransmitData8( triceBytesBuffer[triceBytesBufferIndex++] );
    if( sizeof(triceBytesBuffer) == triceBytesBufferIndex  ){ // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}
