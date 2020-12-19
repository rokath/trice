/*! \file triceEscFifo.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_ESC_FIFO_H_
#define TRICE_ESC_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "triceConfig.h"

void triceWriteEscP( int count, uint8_t * buf );
void trice_s(uint16_t Id, char * dynString);

void tricePushByteEscFifo(uint8_t v);
uint8_t tricePopByteEscFifo(void);

int triceEscFifoDepth(void);
extern int triceEscFifoMaxDepth; //!< usabble for diagnostics

#ifdef TRICE_WRITE_ESC_FIFO ///////////////////////////////////////////////////


#define TRICE_ESC  0xEC //!< Escape char is control char to start a package.
#define TRICE_DEL  0xDE //!< Delete char, if follower of TRICE_ESC, deletes the meaning os TRICE_ESC making it an ordinary TRICE_ESC char.
#define TRICE_P0   0xDF //!< No param char = If follower of TRICE_ESC only a 16 bit ID is inside the payload.
#define TRICE_P1   0xE0 //!< 1 byte param char = If follower of TRICE_ESC a 16 bit ID and 1 byte are inside the payload.
#define TRICE_P2   0xE1 //!< 2 byte param char = If follower of TRICE_ESC a 16 bit ID and 2 byte are inside the payload.
#define TRICE_P4   0xE2 //!< 4 byte param char = If follower of TRICE_ESC a 16 bit ID and 4 byte are inside the payload.
#define TRICE_P8   0xE3 //!< 8 byte param char = If follower of TRICE_ESC a 16 bit ID and 8 byte are inside the payload.
#define TRICE_P16  0xE4 //!< 16 byte param char = If follower of TRICE_ESC a 16 bit ID and 8 byte are inside the payload.
//                 0xE5 // dynamically used for runtime strings with size 17-32
//                 0xE6 // dynamically used for runtime strings with size 33-64
//                 0xE7 // dynamically used for runtime strings with size 63-128
//                 0xE8 // dynamically used for runtime strings with size 127-256
//                 0xE9 // reserved
//                 0xEA // reserved
//                 0xEB // reserved


#define TRICE_BYTE(d) ((uint8_t)(d))

#define TRICE_HI_BYTE(v) TRICE_BYTE(((uint16_t)(v))>>8)
#define TRICE_LO_BYTE(v) TRICE_BYTE(v)

#define TRICE_HH_BYTE(v) TRICE_BYTE(((uint32_t)(v))>>24)
#define TRICE_HL_BYTE(v) TRICE_BYTE(((uint32_t)(v))>>16)
#define TRICE_LH_BYTE(v) TRICE_BYTE(((uint32_t)(v))>> 8)
#define TRICE_LL_BYTE(v) TRICE_BYTE(v)

#define TRICE_HHH_BYTE(v) TRICE_BYTE(((uint64_t)(v))>>56)
#define TRICE_HHL_BYTE(v) TRICE_BYTE(((uint64_t)(v))>>48)
#define TRICE_HLH_BYTE(v) TRICE_BYTE(((uint64_t)(v))>>40)
#define TRICE_HLL_BYTE(v) TRICE_BYTE(((uint64_t)(v))>>32)
#define TRICE_LHH_BYTE(v) TRICE_BYTE(((uint64_t)(v))>>24)
#define TRICE_LHL_BYTE(v) TRICE_BYTE(((uint64_t)(v))>>16)
#define TRICE_LLH_BYTE(v) TRICE_BYTE(((uint64_t)(v))>> 8)
#define TRICE_LLL_BYTE(v) TRICE_BYTE(v)


//! Write Id and dynString protected (outside critical section).
//! \param Id trice identifier
//! \param pFmt formatstring for trice (ignored here but used by the trice tool)
//! \param dynString 0-terminated runtime generated string
//! After the 4 byte trice message header are following 2^n bytes 
#define TRICE_S(Id, pFmt, dynString) do{ trice_s(Id, dynString); }while(0)


//! trace Id protected (outside critical section) within 3 bytes
//! \param Id trice identifier
//! \param pFmt formatstring for trice
#define TRICE0(Id, pFmt) do{ \
    uint8_t msg[] = {TRICE_P0, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

//! trace Id protected and a 8-bit value protected (outside critical section) within 4 bytes
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param 8-bit payload
#define TRICE8_1(Id, pFmt, d0) do{ \
    uint8_t msg[] = {TRICE_P1, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_BYTE(d0)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

//! trace Id and two 8-bit values protected (outside critical section) within 5 bytes
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE8_2(Id, pFmt, d0, d1) do{ \
    uint8_t msg[] = {TRICE_P2, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_BYTE(d0), TRICE_BYTE(d1)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

#define TRICE8_3(Id, pFmt, d0, d1, d2) do{ \
    uint8_t msg[] = {TRICE_P4, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_BYTE(d0), TRICE_BYTE(d1), TRICE_BYTE(d2), TRICE_BYTE(0)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

#define TRICE8_4(Id, pFmt, d0, d1, d2, d3) do{ \
    uint8_t msg[] = {TRICE_P4, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_BYTE(d0), TRICE_BYTE(d1), TRICE_BYTE(d2), TRICE_BYTE(d3)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

#define TRICE8_5(Id, pFmt, d0, d1, d2, d3, d4) do{ \
    uint8_t msg[] = {TRICE_P8, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_BYTE(d0), TRICE_BYTE(d1), TRICE_BYTE(d2), TRICE_BYTE(d3), \
        TRICE_BYTE(d4), TRICE_BYTE(0),  TRICE_BYTE(0),  TRICE_BYTE(0)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

#define TRICE8_6(Id, pFmt, d0, d1, d2, d3, d4, d5) do{ \
    uint8_t msg[] = {TRICE_P8, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_BYTE(d0), TRICE_BYTE(d1), TRICE_BYTE(d2), TRICE_BYTE(d3), \
        TRICE_BYTE(d4), TRICE_BYTE(d5), TRICE_BYTE(0),  TRICE_BYTE(0)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

#define TRICE8_7(Id, pFmt, d0, d1, d2, d3, d4, d5, d6) do{ \
    uint8_t msg[] = {TRICE_P8, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_BYTE(d0), TRICE_BYTE(d1), TRICE_BYTE(d2), TRICE_BYTE(d3), \
        TRICE_BYTE(d4), TRICE_BYTE(d5), TRICE_BYTE(d6), TRICE_BYTE(0)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

#define TRICE8_8(Id, pFmt, d0, d1, d2, d3, d4, d5, d6, d7) do{ \
    uint8_t msg[] = {TRICE_P8, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_BYTE(d0), TRICE_BYTE(d1), TRICE_BYTE(d2), TRICE_BYTE(d3), \
        TRICE_BYTE(d4), TRICE_BYTE(d5), TRICE_BYTE(d6), TRICE_BYTE(d7)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

//! trace Id and 16-bit value protected (outside critical section) within 5 bytes
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE16_1(Id, pFmt, d0) do{ \
    uint8_t msg[] = {TRICE_P2, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_HI_BYTE(d0), TRICE_LO_BYTE(d0)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

//! trace Id and two 16-bit values protected (outside critical section) within 7 bytes
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE16_2(Id, pFmt, d0, d1) do{ \
    uint8_t msg[] = {TRICE_P4, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_HI_BYTE(d0), TRICE_LO_BYTE(d0), \
        TRICE_HI_BYTE(d1), TRICE_LO_BYTE(d1)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

#define TRICE16_3(Id, pFmt, d0, d1, d2) do{ \
    uint8_t msg[] = {TRICE_P8, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_HI_BYTE(d0), TRICE_LO_BYTE(d0), \
        TRICE_HI_BYTE(d1), TRICE_LO_BYTE(d1), \
        TRICE_HI_BYTE(d2), TRICE_LO_BYTE(d2), \
        TRICE_BYTE(0),     TRICE_BYTE(0)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

#define TRICE16_4(Id, pFmt, d0, d1, d2, d3) do{ \
    uint8_t msg[] = {TRICE_P8, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_HI_BYTE(d0), TRICE_LO_BYTE(d0), \
        TRICE_HI_BYTE(d1), TRICE_LO_BYTE(d1), \
        TRICE_HI_BYTE(d2), TRICE_LO_BYTE(d2), \
        TRICE_HI_BYTE(d3), TRICE_LO_BYTE(d3)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE32_1(Id, pFmt, d0) do{ \
    uint8_t msg[] = {TRICE_P4, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_HH_BYTE(d0), TRICE_HL_BYTE(d0), TRICE_LH_BYTE(d0), TRICE_LL_BYTE(d0)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE32_2(Id, pFmt, d0, d1) do{ \
    uint8_t msg[] = {TRICE_P8, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_HH_BYTE(d0), TRICE_HL_BYTE(d0), TRICE_LH_BYTE(d0), TRICE_LL_BYTE(d0), \
        TRICE_HH_BYTE(d1), TRICE_HL_BYTE(d1), TRICE_LH_BYTE(d1), TRICE_LL_BYTE(d1)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
#define TRICE32_3(Id, pFmt, d0, d1, d2) do{ \
    uint8_t msg[] = {TRICE_P16, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_HH_BYTE(d0), TRICE_HL_BYTE(d0), TRICE_LH_BYTE(d0), TRICE_LL_BYTE(d0), \
        TRICE_HH_BYTE(d1), TRICE_HL_BYTE(d1), TRICE_LH_BYTE(d1), TRICE_LL_BYTE(d1), \
        TRICE_HH_BYTE(d2), TRICE_HL_BYTE(d2), TRICE_LH_BYTE(d2), TRICE_LL_BYTE(d2), \
        TRICE_BYTE(0),     TRICE_BYTE(0),     TRICE_BYTE(0),     TRICE_BYTE(0)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! \param d2 payload
//! \param d3 payload
#define TRICE32_4(Id, pFmt, d0, d1, d2, d3) do{ \
    uint8_t msg[] = {TRICE_P16, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_HH_BYTE(d0), TRICE_HL_BYTE(d0), TRICE_LH_BYTE(d0), TRICE_LL_BYTE(d0), \
        TRICE_HH_BYTE(d1), TRICE_HL_BYTE(d1), TRICE_LH_BYTE(d1), TRICE_LL_BYTE(d1), \
        TRICE_HH_BYTE(d2), TRICE_HL_BYTE(d2), TRICE_LH_BYTE(d2), TRICE_LL_BYTE(d2), \
        TRICE_HH_BYTE(d3), TRICE_HL_BYTE(d3), TRICE_LH_BYTE(d3), TRICE_LL_BYTE(d3)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)


//! trace Id and 64-bit value protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE64_1(Id, pFmt, d0) do{ \
    uint8_t msg[] = {TRICE_P8, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_HHH_BYTE(d0), TRICE_HHL_BYTE(d0), TRICE_HLH_BYTE(d0), TRICE_HLL_BYTE(d0), \
        TRICE_LHH_BYTE(d0), TRICE_LHL_BYTE(d0), TRICE_LLH_BYTE(d0), TRICE_LLL_BYTE(d0)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

//! trace Id and 64-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
#define TRICE64_2(Id, pFmt, d0, d1) do{ \
    uint8_t msg[] = {TRICE_P16, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_HHH_BYTE(d0), TRICE_HHL_BYTE(d0), TRICE_HLH_BYTE(d0), TRICE_HLL_BYTE(d0), \
        TRICE_LHH_BYTE(d0), TRICE_LHL_BYTE(d0), TRICE_LLH_BYTE(d0), TRICE_LLL_BYTE(d0), \
        TRICE_HHH_BYTE(d1), TRICE_HHL_BYTE(d1), TRICE_HLH_BYTE(d1), TRICE_HLL_BYTE(d1), \
        TRICE_LHH_BYTE(d1), TRICE_LHL_BYTE(d1), TRICE_LLH_BYTE(d1), TRICE_LLL_BYTE(d1)}; \
    triceWriteEscP( sizeof(msg), msg ); \
} while(0)

#endif // #if TO_TRICE_WRITE == TRICE_CODE 

#ifdef __cplusplus
}
#endif

#endif /* TRICE_ESC_FIFO_H_ */
