/*! \file triceEsc.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_ESC_H_
#define TRICE_ESC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include "triceConfig.h"

#if TRICE_ESC_ENCODING == TRICE_ENCODING 


#define Id(n) (n) //!< Macro for improved trice readability and better source code parsing.


#define TRICE_ESC  0xEC //!< Escape char is control char to start a package.
#define TRICE_DEL  0xDE //!< Delete char, if follower of TRICE_ESC, deletes the meaning os TRICE_ESC making it an ordinary TRICE_ESC char.
#define TRICE_P0   0xDF //!< 223: 0 - 0 param char = If follower of TRICE_ESC only a 16 bit ID is inside the payload.
#define TRICE_P1   0xE0 //!< 224: 1 - 1 byte param char = If follower of TRICE_ESC a 16 bit ID and 1 byte are inside the payload.
#define TRICE_P2   0xE1 //!< 225: 2 - 2 byte param char = If follower of TRICE_ESC a 16 bit ID and 2 byte are inside the payload.
#define TRICE_P4   0xE2 //!< 226: 3 - 4 byte param char = If follower of TRICE_ESC a 16 bit ID and 4 byte are inside the payload.
#define TRICE_P8   0xE3 //!< 227: 4 - 8 byte param char = If follower of TRICE_ESC a 16 bit ID and 8 byte are inside the payload.
#define TRICE_P16  0xE4 //!< 228: 5 -16 byte param char = If follower of TRICE_ESC a 16 bit ID and 8 byte are inside the payload.
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
#ifdef TRICE_ESC_LEGACY
#define TRICE_S(Id, pFmt, dynString) do{ trice_s_legacy(Id, dynString); }while(0)
#else
#define TRICE_S(Id, pFmt, dynString) do{ trice_s(Id, dynString); }while(0)
#endif

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

/*
//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE32_1v1(Id, pFmt, d0) do{ \
    uint8_t msg[] = {TRICE_ESC, TRICE_P4, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id), \
        TRICE_HH_BYTE(d0), TRICE_HL_BYTE(d0), TRICE_LH_BYTE(d0), TRICE_LL_BYTE(d0)}; \
    triceWriteEscE( sizeof(msg), msg ); \
} while(0)


//! trace Id and 32-bit values protected (outside critical section)
//! \param Id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define TRICE32_1v2(Id, pFmt, d0) do{ \
    uint32_t msg[2] = { TRICE_HTON( TRICE_ESC<<24 | TRICE_P4<<16 | id) , \
                       TRICE_HTON(d0) }; \
    triceWriteEscE( 8, (uint8_t*)msg ); \
} while(0)
*/


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


TRICE_INLINE void triceWritePaddingBytes(int count) {
    while (count--) {
        TRICE_U8PUSH(0);
    }
}

TRICE_INLINE void triceWriteEsc(int count, uint8_t *buf) {
    while (count--) {
        uint8_t c = *buf++;
        TRICE_U8PUSH(c);
        if (TRICE_ESC == c) {
            TRICE_U8PUSH(TRICE_DEL);
        }
    }
}
/*
// triceWriteEscE writes first byte in buffer without check because it is a wanted TRICE_ESC.
TRICE_INLINE void triceWriteEscE(int count, uint8_t *buf) {
    TRICE_U8PUSH(*buf);
    while (--count) {
        uint8_t c = *++buf;
        TRICE_U8PUSH(c);
        if (TRICE_ESC == c) {
            TRICE_U8PUSH(TRICE_DEL);
        }
    }
}
*/
//! Start with TRICE_ESC and then comes buf
TRICE_INLINE void triceWriteEscP(int count, uint8_t *buf) {
    TRICE_ENTER_CRITICAL_SECTION
    TRICE_U8PUSH(TRICE_ESC);
    triceWriteEsc(count, buf);
    TRICE_LEAVE_CRITICAL_SECTION
}

#define TRICE_BYTE(d) ((uint8_t)(d))
#define TRICE_HI_BYTE(v) TRICE_BYTE(((uint16_t)(v))>>8)
#define TRICE_LO_BYTE(v) TRICE_BYTE(v)

TRICE_INLINE void trice_s_legacy(uint16_t Id, char *dynString) {
    int n = 1 + strlen(dynString);
    int h = -1; // h is the smallest number with 2^h = k && k >= n
    int k = 0;       // n is at least 1 here, so h cannot get -1
    while (k < n) {  // n:  0 | 1 2 3 4 5
        k = 1 << h++;// h: -1 | 1 2 3 3 4
    }                // k:  0 | 1 2 4 4 8
    uint8_t msg[] = {TRICE_P0 + h, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id)};
    TRICE_ENTER_CRITICAL_SECTION
    TRICE_U8PUSH(TRICE_ESC);
    triceWriteEsc(sizeof(msg), msg);
    triceWriteEsc(n, (uint8_t *) dynString);
    triceWritePaddingBytes(k - n);
    TRICE_LEAVE_CRITICAL_SECTION
    // example: ""         =                                   0 -> bufLen=1 -> n=0, (1<<0)= 1, padding=0
    // example: "a"        = 'a'                               0 -> bufLen=2 -> n=1, (1<<1)= 2, padding=0
    // example: "ab"       = 'a' 'b'                           0 -> bufLen=3 -> n=2, (1<<2)= 4, padding=1
    // example: "abc"      = 'a' 'b' 'c'                       0 -> bufLen=4 -> n=2, (1<<2)= 4, padding=0
    // example: "abcd"     = 'a' 'b' 'c' 'd'                   0 -> bufLen=5 -> n=3, (1<<8)= 8, padding=3
    // example: "abcdef"   = 'a' 'b' 'c' 'd' 'e'               0 -> bufLen=6 -> n=3, (1<<8)= 8, padding=2
    // example: "abcdef"   = 'a' 'b' 'c' 'd' 'e' 'f'           0 -> bufLen=7 -> n=3, (1<<8)= 8, padding=1
    // example: "abcdefg"  = 'a' 'b' 'c' 'd' 'e' 'f' 'g'       0 -> bufLen=8 -> n=3, (1<<8)= 8, padding=0
    // example: "abcdefgh" = 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h'   0 -> bufLen=9 -> n=4, (1<<4)=16, padding=7
}

#if 0
//! see https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
TRICE_INLINE unsigned long upper_power_of_two(unsigned long v){
    if( 0 == v ){
        return 0;
    }
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}
#else
//! see https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
//!\return 1 + needed shift for 1 to be bigger than x
//!\li  0:     0
//!\li  1:     1
//!\li  2:     2
//!\li  3:     3
//!\li  4:     3
//!\li  5:     4
//!\li  6:     4
//!\li  7:     4
//!\li  8:     4
//!\li  9:     5
TRICE_INLINE uint32_t upperOneShift(uint32_t x){
    uint32_t power = 1;
    uint32_t shift = 0;
    if( 0 == x ){
        return 0;
    }
    while(power < x){
        power<<=1;
        shift++;
    }
    return shift+1;
}
#endif

TRICE_INLINE void upo2check( void ){
    for( uint32_t i = 0; i < 10; i++ ){
        TRICE32_2( Id(65023), "sig:%2d:%6d\n", i, upperOneShift(i) );
    }
}

//! trice_s does not transmit the string terminating 0 but adds optionally padding bytes with content 0
TRICE_INLINE void trice_s(uint16_t Id, char *dynString) {
    int n = strlen(dynString);
    int h = upperOneShift(n);
    uint8_t msg[] = {TRICE_P0 + h, TRICE_HI_BYTE(Id), TRICE_LO_BYTE(Id)};
    TRICE_ENTER_CRITICAL_SECTION
    TRICE_U8PUSH(TRICE_ESC);
    triceWriteEsc(sizeof(msg), msg);
    if( n > 0 ){
        triceWriteEsc(n, (uint8_t *) dynString);
        while( n++ < (1<<(h-1)) ){
            TRICE_U8PUSH(0);
        }
    }
    TRICE_LEAVE_CRITICAL_SECTION
}



#endif // #if TRICE_ESC_ENCODING == TRICE_ENCODING

#ifdef __cplusplus
}
#endif

#endif /* TRICE_ESC_H_ */
