/*! \file tcobsDecode.c
\author Thomas.Hoehenleitner [at] seerose.net
\details See ./TCOBSv2Specification.md
*******************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include "tcobs.h"
#include "tcobsInternal.h"

//! ASSERT checks for a true condition, otherwise stop.
//! For the decoding this macro was used just to verify the code during development.
#define ASSERT( condition ) // do{ if( !(condition) ){ return -__LINE__; } }while(0);

//! MAX_CIPHERS is max expected sigil bytes of one kind in a row. 
//! 3^24 = 282.429.536.481, we do not expect so much equal bytes.
#define MAX_CIPHERS 24 

static int writeZn( uint8_t ** out, uint8_t ** ss, int * ciphersCount);
static int writeFn( uint8_t ** out, uint8_t ** ss, int * ciphersCount);
static int writeRn( uint8_t ** out, uint8_t ** ss, int * ciphersCount, uint8_t repeatByte);

static int CCQNZtoCCQN( uint8_t* ciphers, int count );
static int CCQNFtoCCQN( uint8_t* ciphers, int count );
static int CCTNRtoCCTN( uint8_t* ciphers, int count );

static int CCQNZtoN( uint8_t* ciphers, int count );
static int CCQNFtoN( uint8_t* ciphers, int count );
static int CCTNRtoN( uint8_t* ciphers, int count );

int TCOBSDecode( void * restrict output, size_t max, const void * restrict input, size_t length ){
    if( length == 0 ){
        return 0;
    }else{
        uint8_t sigil;
        int dc = 0; // distance counter
        uint8_t cc[MAX_CIPHERS];
        uint8_t * ss = cc + MAX_CIPHERS; // sigilSequence (without distance bits) = counted ciphers
        uint8_t * o = (uint8_t*)output + max; // output write pointer behind next value
        uint8_t const * i = (uint8_t*)input + length; // input read pointer behind next value
        int zc = 0; // Z sigils in a row
        int fc = 0; // F sigils in a row
        int rc = 0; // R Z sigils in a row
        uint8_t repeatByte;
        int err;
        for(;;){
            if( i == input ){ // done
                ASSERT( (zc|fc|rc|dc) == 0 )
                return (uint8_t*)output + max - o;
            }
            ASSERT(zc<MAX_CIPHERS)
            ASSERT(fc<MAX_CIPHERS)
            ASSERT(rc<MAX_CIPHERS)
            sigil = *--i;
            if( sigil >> 7 ==  0 ){               // 0xxxxxxx    //  N | Z0 | Z1 | Z2 | R1
                if( sigil >> 6 ==  0 ){           // 00xddddd    //  N | Z0
                    dc = sigil & 0x1F;
                    if( sigil >> 5 ==  0 ){       // 000ddddd    //  N |
                        goto Nsigil;
                    }else{                        // 001ddddd    //      Z0
                        goto Z0sigil;
                    }
                }else{                            // 01xxxxxx    //           Z1 | Z2 | R1
                    if( sigil >> 5 == 2 ){        // 010xxxxx    //              | Z2 | R1
                        if( sigil >> 4 == 4 ){    // 0100dddd    //                     R1
                            dc = sigil & 0xF;
                            goto R1sigil;
                        }else{                    // 0101dddd    //                Z2
                            dc = sigil & 0xF;
                            goto Z2sigil;
                        }
                    }else{                        // 011ddddd    //           Z1
                        dc = sigil & 0x1F;
                        goto Z1sigil;
                    }
                }
            }else{                                // 1xxxxxxx    // Z3 | F0 |F1 | F2 | F3 | R0 | R2
                if( sigil >> 6 ==  2 ){           // 10xxxxxx    // Z3                    | R0 | R2
                    if( sigil >> 5 ==  4 ){       // 100ddddd    //                         R0
                        dc = sigil & 0x1F;
                        goto R0sigil;
                    }else{                        // 101xdddd    // Z3                         | R2
                        dc = sigil & 0xF;
                        if( sigil >> 4 == 0xA ){  // 1010dddd    //                              R2
                        dc = sigil & 0xF;
                            goto R2sigil;
                        }else{                    // 1011dddd    // Z3
                        dc = sigil & 0xF;
                            goto Z3sigil;
                        }                     }   
                }else{                            // 11xxxxxx    //      F0 | F1 | F2 | F3
                    if( sigil >> 5 ==  6 ){       // 110ddddd    //           F1
                        dc = sigil & 0x1F;
                        goto F1sigil;
                    }else{                        // 111xxxxx    //               F2 | F3 | F0
                        if( sigil >> 4 ==  0xE ){ // 1110dddd    //               F2 
                            dc = sigil & 0xF;
                            goto F2sigil;
                        }else{                    // 1111xxxx    //      F0 |          F3
                            if( sigil == 0xFF ){  // 11111111    //      F0
                                dc = 0;
                                goto F0sigil;
                            }else{                // 1111dddd    //                    F3
                                dc = sigil & 0xF;
                                goto F3sigil;
                            }
                        }
                    }
                }
            }
// The following jump points are code to be executed inside the upper binary search, but "outsourced" here for better readability.
// This example is for better decode understanding:
// # state          #                      in buffer #                                   out buffer # state derived action to follow
// # sg zc fc rc dc # ------------------------------ # -------------------------------------------- # ----------------------------------------------------------
// #     0  0  0  0 # 22 33 Z0|2 Z0 77 88 99 Z0|3 F3 #                                              # start
// # F3  0  0  0  0 # 22 33 Z0|2 Z0 77 88 99 Z0|3    #                                              # F3 stored && continue
// # Z0  0  1  0  3 # 22 33 Z0|2 Z0 77 88 99         #                                              # Z0 && fc -> a change -> writeFn( &o, ss, fc );
// |     0  0  0  3 # 22 33 Z0|2 Z0 77 88 99         #                                  FF FF FF FF # *--o = 0;
// |     0  0  0  3 # 22 33 Z0|2 Z0 77 88 99         #                               00 FF FF FF FF # COPY_BYTES
// |     0  0  0  0 # 22 33 Z0|2 Z0                  #                      77 88 99 00 FF FF FF FF # continue
// # Z0  0  0  0  0 # 22 33 Z0|2                     #                      77 88 99 00 FF FF FF FF # all 0 -> Z0 stored && continue
// # Z0  0  1  0  2 # 22 33                          #                      77 88 99 00 FF FF FF FF # Z0 & zc & dc -> a change -> writeZn( &o, ss, zc ); Z0Z0==5
// |     0  0  0  2 # 22 33                          #       00 00 00 00 00 77 88 99 00 FF FF FF FF # COPY_BYTES
// |     0  0  0  0 # 22 33                          # 22 33 00 00 00 00 00 77 88 99 00 FF FF FF FF # i == input -> done
            Z0sigil:
                if( fc ){
                    ASSERT( rc == 0 )
                    ASSERT( fc == cc + MAX_CIPHERS - ss )
                    err = writeFn( &o, &ss, &fc );
                    if( err ){
                        return err;
                    }
                }
                if( rc ){
                    ASSERT( fc == 0 )
                    ASSERT( rc == cc + MAX_CIPHERS - ss )
                    repeatByte = *--i;
                    i++;
                    err = writeRn( &o, &ss, &rc, repeatByte );
                    if( err ){
                        return err;
                    }
                }
                if( dc == 0 && i != input ){
                    *--ss = Z0;
                    zc++;
                    continue;
                }
                *--ss = Z0;
                zc++;
                err = writeZn( &o, &ss, &zc );
                if( err ){
                    return err;
                }
                if( (uint8_t*)input > i - dc ){
                    return INPUT_DATA_CORRUPTED - __LINE__;
                }
                if( (uint8_t*)output + dc > o ){ 
                    return OUT_BUFFER_TOO_SMALL - __LINE__;
                }
                while( dc-- ){ 
                    *--o = *--i; 
                } 
                dc = 0;
                continue;
            Z1sigil:
                if( fc ){
                    ASSERT( rc == 0 )
                    ASSERT( fc == cc + MAX_CIPHERS - ss )
                    err = writeFn( &o, &ss, &fc );
                    if( err ){
                        return err;
                    }
                }
                if( rc ){
                    ASSERT( fc == 0 )
                    ASSERT( rc == cc + MAX_CIPHERS - ss )
                    repeatByte = *--i;
                    i++;
                    err = writeRn( &o, &ss, &rc, repeatByte );
                    if( err ){
                        return err;
                    }
                }
                if( dc == 0 && i != input ){
                    *--ss = Z1;
                    zc++;
                    continue;
                }
                *--ss = Z1;
                zc++;
                err = writeZn( &o, &ss, &zc );
                if( err ){
                    return err;
                }
                if( (uint8_t*)input > i - dc ){
                    return INPUT_DATA_CORRUPTED - __LINE__;
                }
                if( (uint8_t*)output + dc > o ){ 
                    return OUT_BUFFER_TOO_SMALL - __LINE__;
                }
                while( dc-- ){ 
                    *--o = *--i; 
                } 
                dc = 0;
                continue;
            Z2sigil:
                if( fc ){
                    ASSERT( rc == 0 )
                    ASSERT( fc == cc + MAX_CIPHERS - ss )
                    err = writeFn( &o, &ss, &fc );
                    if( err ){
                        return err;
                    }
                }
                if( rc ){
                    ASSERT( fc == 0 )
                    ASSERT( rc == cc + MAX_CIPHERS - ss )
                    repeatByte = *--i;
                    i++;
                    err = writeRn( &o, &ss, &rc, repeatByte );
                    if( err ){
                        return err;
                    }
                }
                if( dc == 0 && i != input ){
                    *--ss = Z2;
                    zc++;
                    continue;
                }
                *--ss = Z2;
                zc++;
                err = writeZn( &o, &ss, &zc );
                if( err ){
                    return err;
                }
                if( (uint8_t*)input > i - dc ){
                    return INPUT_DATA_CORRUPTED - __LINE__;
                }
                if( (uint8_t*)output + dc > o ){ 
                    return OUT_BUFFER_TOO_SMALL - __LINE__;
                }
                while( dc-- ){ 
                    *--o = *--i; 
                } 
                dc = 0;
                continue;
            Z3sigil:
                if( fc ){
                    ASSERT( rc == 0 )
                    ASSERT( fc == cc + MAX_CIPHERS - ss )
                    err = writeFn( &o, &ss, &fc );
                    if( err ){
                        return err;
                    }
                }
                if( rc ){
                    ASSERT( fc == 0 )
                    ASSERT( rc == cc + MAX_CIPHERS - ss )
                    repeatByte = *--i;
                    i++;
                    err = writeRn( &o, &ss, &rc, repeatByte );
                    if( err ){
                        return err;
                    }
                }
                if( dc == 0 && i != input ){
                    *--ss = Z3;
                    zc++;
                    continue;
                }
                *--ss = Z3;
                zc++;
                err = writeZn( &o, &ss, &zc );
                if( err ){
                    return err;
                }
                if( (uint8_t*)input > i - dc ){
                    return INPUT_DATA_CORRUPTED - __LINE__;
                }
                if( (uint8_t*)output + dc > o ){ 
                    return OUT_BUFFER_TOO_SMALL - __LINE__;
                }
                while( dc-- ){ 
                    *--o = *--i; 
                } 
                dc = 0;
                continue;
            F0sigil: // This could be a single F0, but it is inside the sigil chain.
                if( zc ){
                    ASSERT( rc == 0 )
                    ASSERT( zc == cc + MAX_CIPHERS - ss )
                    err = writeZn( &o, &ss, &zc );
                    if( err ){
                        return err;
                    }
                }
                if( rc ){
                    ASSERT( zc == 0 )
                    ASSERT( rc == cc + MAX_CIPHERS - ss )
                    repeatByte = *--i;
                    i++;
                    err = writeRn( &o, &ss, &rc, repeatByte );
                    if( err ){
                        return err;
                    }
                }
                if( /*dc == 0 &&*/ i != input ){ // F0 means dc == 0.
                    *--ss = F0; 
                    fc++;
                    continue;
                }
                *--ss = F0;
                fc++;
                err = writeFn( &o, &ss, &fc );
                if( err ){
                    return err;
                }
                if( (uint8_t*)input > i - dc ){
                    return INPUT_DATA_CORRUPTED - __LINE__;
                }
                if( (uint8_t*)output + dc > o ){ 
                    return OUT_BUFFER_TOO_SMALL - __LINE__;
                }
                while( dc-- ){ 
                    *--o = *--i; 
                } 
                dc = 0;
                continue;
            F1sigil:
                if( zc ){
                    ASSERT( rc == 0 )
                    ASSERT( zc == cc + MAX_CIPHERS - ss )
                    err = writeZn( &o, &ss, &zc );
                    if( err ){
                        return err;
                    }
                }
                if( rc ){
                    ASSERT( zc == 0 )
                    ASSERT( rc == cc + MAX_CIPHERS - ss )
                    repeatByte = *--i;
                    i++;
                    err = writeRn( &o, &ss, &rc, repeatByte );
                    if( err ){
                        return err;
                    }
                }
                if( dc == 0 && i != input ){
                    *--ss = F1;
                    fc++;
                    continue;
                }
                *--ss = F1;
                fc++;
                err = writeFn( &o, &ss, &fc );
                if( err ){
                    return err;
                }
                if( (uint8_t*)input > i - dc ){
                    return INPUT_DATA_CORRUPTED - __LINE__;
                }
                if( (uint8_t*)output + dc > o ){ 
                    return OUT_BUFFER_TOO_SMALL - __LINE__;
                }
                while( dc-- ){ 
                    *--o = *--i; 
                } 
                dc = 0;
                continue;
            F2sigil:
                if( zc ){
                    ASSERT( rc == 0 )
                    ASSERT( zc == cc + MAX_CIPHERS - ss )
                    err = writeZn( &o, &ss, &zc );
                    if( err ){
                        return err;
                    }
                }
                if( rc ){
                    ASSERT( zc == 0 )
                    ASSERT( rc == cc + MAX_CIPHERS - ss )
                    repeatByte = *--i;
                    i++;
                    err = writeRn( &o, &ss, &rc, repeatByte );
                    if( err ){
                        return err;
                    }
                }
                if( dc == 0 && i != input ){
                    *--ss = F2;
                    fc++;
                    continue;
                }
                *--ss = F2;
                fc++;
                err = writeFn( &o, &ss, &fc );
                if( err ){
                    return err;
                }
                if( (uint8_t*)input > i - dc ){
                    return INPUT_DATA_CORRUPTED - __LINE__;
                }
                if( (uint8_t*)output + dc > o ){ 
                    return OUT_BUFFER_TOO_SMALL - __LINE__;
                }
                while( dc-- ){ 
                    *--o = *--i; 
                } 
                dc = 0;
                continue;
            F3sigil:
                if( zc ){
                    ASSERT( rc == 0 )
                    ASSERT( zc == cc + MAX_CIPHERS - ss )
                    err = writeZn( &o, &ss, &zc );
                    if( err ){
                        return err;
                    }
                }
                if( rc ){
                    ASSERT( zc == 0 )
                    ASSERT( rc == cc + MAX_CIPHERS - ss )
                    repeatByte = *--i;
                    i++;
                    err = writeRn( &o, &ss, &rc, repeatByte );
                    if( err ){
                        return err;
                    }
                }
                if( dc == 0 && i != input ){
                    *--ss = F3;
                    fc++;
                    continue;
                }
                *--ss = F3;
                fc++;
                err = writeFn( &o, &ss, &fc );
                if( err ){
                    return err;
                }
                if( (uint8_t*)input > i - dc ){
                    return INPUT_DATA_CORRUPTED - __LINE__;
                }
                if( (uint8_t*)output + dc > o ){ 
                    return OUT_BUFFER_TOO_SMALL - __LINE__;
                }
                while( dc-- ){ 
                    *--o = *--i; 
                } 
                dc = 0;
                continue;
            R0sigil:
                if( zc ){
                    ASSERT( fc == 0 )
                    ASSERT( zc == cc + MAX_CIPHERS - ss )
                    err = writeZn( &o, &ss, &zc );
                    if( err ){
                        return err;
                    }
                }
                if( fc ){
                    ASSERT( zc == 0 )
                    ASSERT( fc == cc + MAX_CIPHERS - ss )
                    err = writeFn( &o, &ss, &fc );
                    if( err ){
                        return err;
                    }
                }
                if( dc == 0 && i != input ){
                    *--ss = R0;
                    rc++;
                    continue;
                }
                *--ss = R0;
                rc++;
                repeatByte = *--i;
                i++;
                err = writeRn( &o, &ss, &rc, repeatByte );
                if( err ){
                    return err;
                }
                if( (uint8_t*)input > i - dc ){
                    return INPUT_DATA_CORRUPTED - __LINE__;
                }
                if( (uint8_t*)output + dc > o ){ 
                    return OUT_BUFFER_TOO_SMALL - __LINE__;
                }
                while( dc-- ){ 
                    *--o = *--i; 
                } 
                dc = 0;
                continue;
            R1sigil:
                if( zc ){
                    ASSERT( fc == 0 )
                    ASSERT( zc == cc + MAX_CIPHERS - ss )
                    err = writeZn( &o, &ss, &zc );
                    if( err ){
                        return err;
                    }
                }
                if( fc ){
                    ASSERT( zc == 0 )
                    ASSERT( fc == cc + MAX_CIPHERS - ss )
                    err = writeFn( &o, &ss, &fc );
                    if( err ){
                        return err;
                    }
                }
                if( dc == 0 && i != input ){
                    *--ss = R1;
                    rc++;
                    continue;
                }
                *--ss = R1;
                rc++;
                repeatByte = *--i;
                i++;
                err = writeRn( &o, &ss, &rc, repeatByte );
                if( err ){
                    return err;
                }
                if( (uint8_t*)input > i - dc ){
                    return INPUT_DATA_CORRUPTED - __LINE__;
                }
                if( (uint8_t*)output + dc > o ){ 
                    return OUT_BUFFER_TOO_SMALL - __LINE__;
                }
                while( dc-- ){ 
                    *--o = *--i; 
                }
                dc = 0;
                continue;
            R2sigil:
                if( zc ){
                    ASSERT( fc == 0 )
                    ASSERT( zc == cc + MAX_CIPHERS - ss )
                    err = writeZn( &o, &ss, &zc );
                    if( err ){
                        return err;
                    }
                }
                if( fc ){
                    ASSERT( zc == 0 )
                    ASSERT( fc == cc + MAX_CIPHERS - ss )
                    err = writeFn( &o, &ss, &fc );
                    if( err ){
                        return err;
                    }
                }
                if( dc == 0 && i != input ){
                    *--ss = R2;
                    rc++;
                    continue;
                }
                *--ss = R2;
                rc++;
                repeatByte = *--i;
                i++;
                err = writeRn( &o, &ss, &rc, repeatByte );
                if( err ){
                    return err;
                }
                if( (uint8_t*)input > i - dc ){
                    return INPUT_DATA_CORRUPTED - __LINE__;
                }
                if( (uint8_t*)output + dc > o ){ 
                    return OUT_BUFFER_TOO_SMALL - __LINE__;
                }
                while( dc-- ){ 
                    *--o = *--i; 
                } 
                dc = 0;
                continue;
            Nsigil:
                ASSERT( dc != 0 )
                if( zc ){
                    ASSERT( fc == 0 )
                    ASSERT( zc == cc + MAX_CIPHERS - ss )
                    err = writeZn( &o, &ss, &zc );
                    if( err ){
                        return err;
                    }
                }
                if( fc ){
                    ASSERT( zc == 0 )
                    ASSERT( fc == cc + MAX_CIPHERS - ss )
                    err = writeFn( &o, &ss, &fc );
                    if( err ){
                        return err;
                    }
                }
                if( rc ){
                    ASSERT( zc == 0 )
                    ASSERT( rc == cc + MAX_CIPHERS - ss )
                    repeatByte = *--i;
                    i++;
                    err = writeRn( &o, &ss, &rc, repeatByte );
                    if( err ){
                        return err;
                    }
                }
                if( (uint8_t*)input > i - dc ){
                    return INPUT_DATA_CORRUPTED - __LINE__;
                }
                if( (uint8_t*)output + dc > o ){ 
                    return OUT_BUFFER_TOO_SMALL - __LINE__;
                }
                while( dc-- ){ 
                    *--o = *--i; 
                } 
                dc = 0;
                continue;
        }
    }
}

//! writeZn writes 0x00 to *out n times. n is computed from sigil sequence in ss containing ciphersCount sigil bytes.
static int writeZn( uint8_t ** out, uint8_t ** ss, int * ciphersCount){
    int n = CCQNZtoN( *ss, *ciphersCount );
    if( n < 0 ){
        return n;
    }
    while( n-- ){
        *out -= 1;
        **out = 0;
    }
    *ss += *ciphersCount;
    *ciphersCount = 0;
    return 0;
}

//! writeFn writes 0xFF to *out n times. n is computed from sigil sequence in ss containing ciphersCount sigil bytes.
static int writeFn( uint8_t ** out, uint8_t ** ss, int * ciphersCount){
    int n = CCQNFtoN( *ss, *ciphersCount );
    if( n < 0 ){
        return n;
    }
    while( n-- ){
        *out -= 1;
        **out = 0xFF;
    }
    *ss += *ciphersCount;
    *ciphersCount = 0;
    return 0;
}

//! writeRn writes repeatByte to *out n+1 times. n is computed from sigil sequence in ss containing ciphersCount sigil bytes.
static int writeRn( uint8_t ** out, uint8_t ** ss, int * ciphersCount, uint8_t repeatByte ){
    int n = CCTNRtoN( *ss, *ciphersCount );
    if( n < 0 ){
        return n;
    }
    while( n-- ){
        *out -= 1;
        **out = repeatByte;
    }
    *ss += *ciphersCount;
    *ciphersCount = 0;
    return 0;
}

//! CCQNZtoCCQN converts Z-sigils in ciphers to ciphers.
static int CCQNZtoCCQN( uint8_t* ciphers, int count ){
    for(int i = 0; i < count; i++ ){
        if( ciphers[i] == Z0 ){
            ciphers[i] = 0;
        }else if( ciphers[i] == Z1 ){
            ciphers[i] = 1;
        }else if( ciphers[i] == Z2 ){
            ciphers[i] = 2;
        }else if( ciphers[i] == Z3 ){
            ciphers[i] = 3;
        }else{
            return - __LINE__;
        }
    }
    return 0;
}

//! CCQNFtoCCQN converts F-sigils in ciphers to ciphers.
static int CCQNFtoCCQN( uint8_t* ciphers, int count ){
    for(int i = 0; i < count; i++ ){
        if( ciphers[i] == F0 ){
            ciphers[i] = 0;
        }else if( ciphers[i] == F1 ){
            ciphers[i] = 1;
        }else if( ciphers[i] == F2 ){
            ciphers[i] = 2;
        }else if( ciphers[i] == F3 ){
            ciphers[i] = 3;
        }else{
            return - __LINE__;
        }
    }
    return 0;
}

//! CCTNRtoCCTN converts R-sigils in ciphers to ciphers.
static int CCTNRtoCCTN( uint8_t* ciphers, int count ){
    for(int i = 0; i < count; i++ ){
        if( ciphers[i] == R0 ){
            ciphers[i] = 0;
        }else if( ciphers[i] == R1 ){
            ciphers[i] = 1;
        }else if( ciphers[i] == R2 ){
            ciphers[i] = 2;
        }else{
            return - __LINE__;
        }
    }
    return 0;
}

//! CCQNtoN converts count CCQN ciphers in a number.
static int CCQNtoN( uint8_t* ciphers, int count ){
    int n = 0;
    int pwr = 1; // 4^0
    for(int i = count-1; i >= 0; i-- ){
        n += pwr; // generic start part
        n += ciphers[i] * pwr;
        pwr *= 4;
    }
    return n;
}

//! CCTNtoN converts count CCTN ciphers in a number.
static int CCTNtoN( uint8_t* ciphers, int count ){
    int n = 1;
    int pwr = 1; // 3^0
    for(int i = count-1; i >= 0; i-- ){
        n += pwr; // generic start part 
        n += ciphers[i] * pwr;
        pwr *= 3;
    }
    return n;
}

//! CCQNZtoN converts count CCQNZ ciphers in a number.
static int CCQNZtoN( uint8_t* ciphers, int count ){
    int r = CCQNZtoCCQN( ciphers, count );
    if( r < 0 ){
        return __LINE__;
    }
    return CCQNtoN( ciphers, count );
}

//! CCQNFtoN converts count CCQNF ciphers in a number.
static int CCQNFtoN( uint8_t* ciphers, int count ){
    int r = CCQNFtoCCQN( ciphers, count );
    if( r < 0 ){
        return __LINE__;
    }
    return CCQNtoN( ciphers, count );
}

//! CCTNRtoN converts count CCTNR ciphers in a number.
static int CCTNRtoN( uint8_t* ciphers, int count ){
    int r = CCTNRtoCCTN( ciphers, count );
    if( r < 0 ){
        return __LINE__;
    }
    return CCTNtoN( ciphers, count );
}
