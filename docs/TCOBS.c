/*! \file TCOBS.c
\author Thomas.Hoehenleitner [at] seerose.net
\details See ./TCOBSSpecification.md.
*******************************************************************************/

#include <stdint.h>
#include "TCOBS.h"

#define ASSERT( condition )do{ if( !(condition) ){ for(;;){}}}while(0); //! ASSERT checks for a true condition, otherwise stop.
#define BCOUNT ( limit - i ) //!< BCOUNT ist the remaining input byte count.
#define OUTB( b ) do{ *o++ = b; offset++; if( offset == 31 ){ *o++ = N | 31; offset = 0; } } while( 0 ); //!< OUTB writes a non-sigil byte to output.

#define N  0xA0 //!< sigil byte 0x101ooooo, offset 0-31
#define Z1 0x20 //!< sigil byte 0x001ooooo, offset 0-31
#define Z2 0x40 //!< sigil byte 0x010ooooo, offset 0-31
#define Z3 0x60 //!< sigil byte 0x011ooooo, offset 0-31
#define F2 0xC0 //!< sigil byte 0x110ooooo, offset 0-31
#define F3 0xE0 //!< sigil byte 0x111ooooo, offset 0-31
#define F4 0x80 //!< sigil byte 0x100ooooo, offset 0-31
#define R2 0x08 //!< sigil byte 0x00001ooo, offset 0-7
#define R3 0x10 //!< sigil byte 0x00010ooo, offset 0-7
#define R4 0x18 //!< sigil byte 0x00011ooo, offset 0-7
#define R5 0x00 //!< sigil byte 0x00000ooo, offset 0-6 stored as 1-7

unsigned TCOBSEncode( uint8_t* restrict output, const uint8_t* restrict input, unsigned length){
    uint8_t* o = output;
    uint8_t* i = (uint8_t*)input;
    uint8_t* limit = (uint8_t*)input + length;
    int offset = 0; // sigil chain link
    int zeroCount = 0; // counts zero bytes 1-3 for Z1-Z3
    int fullCount = 0; // counts 0xFF bytes 1-4 for 0xFF and F2-F4
    int reptCount = 0; // counts repeat bytes 1-8 for xx and R2-R4,
                       //                     1-7 for xx and R5
    uint8_t b_1; // previous byte
    uint8_t b; // current byte
    if( BCOUNT == 0 ){ // nothing to do
        return 0;
    }
    if( BCOUNT == 1 ){ // simple special case
        b = *i;
        if( b == 0 ){
            *o = Z1;
            return 1;
        }else{
            *o++ = b;
            *o = N | 1;
            return 2;
        }
    }
    for(;;){
        b_1 = b; // keep last byte for compare
        b = *i++; // get next byte
        if( BCOUNT > 1 ){ // most likely
            if( b == 0 ){
                zeroCount++; 
                if( zeroCount < 3 ){
                    continue;
                }else{ // 3rd zero
                    ASSERT( offset < 32 )
                    *o++ = Z3 | offset; // Z3=011ooooo
                    offset = 0;
                    zeroCount = 0;
                    continue;
                }
            }else if( b == 0xFF ){
                fullCount++;
                if( fullCount < 4 ){
                    continue;
                }else{ // 4th FF
                    ASSERT( offset < 32 )
                    *o++ = F4 | offset; // F4=0x100ooooo
                    offset = 0;
                    fullCount = 0;
                    continue;
                }
            }else{ // b = 01...FE
                if( zeroCount ){ // Z1=001ooooo, Z2=010ooooo, Z3=011ooooo
                    ASSERT( offset < 32 && fullCount == 0 && reptCount == 0 )
                    *o++ = (zeroCount << 5) | offset;
                    offset = 1;
                    zeroCount = 0;
                    *o++ = b; 
                    continue;
                } else if( fullCount == 1 ){ // a single FF
                    ASSERT( offset < 32 && zeroCount == 0 && reptCount == 0 )
                    OUTB( 0xFF )
                    fullCount = 0;
                    OUTB( b )
                    continue;
                }else if( 2 <= fullCount && fullCount <= 4 ){ // 2-4 FF
                    ASSERT( offset < 32 && zeroCount == 0 && reptCount == 0 )
                    *o++ = 0x80 | (fullCount << 5) | offset;
                    offset = 1;
                    fullCount = 0;
                    *o++ = b;
                    continue;
                }else if( b_1 != b ){
                    ASSERT( offset < 32 && zeroCount == 0 && fullCount == 0 )
                    switch( reptCount ){
                    case 0: // no repetition
                        OUTB( b )
                        continue;
                    case 1: // one repetition
                        OUTB( b_1 )
                        OUTB( b )
                        continue;
                    case 2: // two repetitions
                        ASSERT( offset < 32 )
                        if( offset > 7 ){
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = R2 | offset; // R2=0x00001ooo
                        offset = 1;
                        *o++ = b; 
                        continue; 
                    case 3: // 3 repetitions
                        ASSERT( offset < 32 )
                        if( offset > 7 ){
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = R3 | offset; // R3=0x00010ooo
                        offset = 1;
                        *o++ = b;
                        continue; 
                    case 4: // 4 repetitions
                        ASSERT( offset < 32 )
                        if( offset > 7 ){
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = R4 | offset; // R4=0x00011ooo
                        offset = 1;
                        *o++ = b;
                        continue; 
                    case 5: // 5 repetitions
                        ASSERT( offset < 32 )
                        if( offset > 6 ){
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = R5 | (offset+1); // R5=0x00000ooo
                        offset = 1;
                        *o++ = b;
                        continue; 
                    default:
                       ASSERT( 0 )
                    }
                }else{ // b_1 == b
                    reptCount++;
                    if( reptCount < 5 ){
                        continue;
                    }
                    ASSERT( offset < 7 )
                    *o++ = R5 | (offset+1); // R5=0x00000ooo
                    offset = 0;
                    reptCount = 0;
                    continue;
                }
            }
        }else{ // BCOUNT == 1, finish
            if( b == 0 ){
                zeroCount++; // Z1=001ooooo, Z2=010ooooo, Z3=011ooooo
                ASSERT( zeroCount <= 3 )
                *o++ = (zeroCount<<5)| offset;
                return o - output;
            }else if( b == 0xFF ){ // a FF and ...
                if( fullCount == 0 ){ // ... no previous FF
                    *o++ = 0xFF;
                    offset++;
                    ASSERT( offset < 32 ) 
                    *o++ = N | offset; // N=0x101ooooo
                    return o - output;
                }else{ // ... 1-3 previous 0xFF
                    fullCount++; // 2 or 3 or 4
                    ASSERT( fullCount <= 4 )
                    ASSERT( offset < 32 )
                    *o++ = 0x80 | (fullCount<<5) | offset; // F2=110ooooo, F3=111ooooo, F4=100ooooo
                    return o - output;
                }
            }else{ // b = 01...FE
                if( reptCount == 0 ){
                    *o++ = b; 
                    offset++;
                    ASSERT( offset < 32 ) 
                    *o++ = N | offset; // N=0x101ooooo
                    return o - output;
                }else if( b_1 != b ){ // now a different byte: xx, Rx yy, b_1 has a valid value here
                    if( reptCount == 1 ){ // xx, R1 yy
                        *o++ = b_1; // xx xx, yy
                        offset++;
                        if( offset == 31 ){
                            *o++ = N | 31; // N=0x101ooooo
                            offset = 0;
                        }
                        *o++ = b;  // xx xx yy,
                        offset++;
                        ASSERT( offset < 32 )
                        *o++ = N | offset;
                        return o - output;
                    }else{ // xx, Rn yy
                        reptCount++;
                        ASSERT( 2 <= reptCount && reptCount <= 5 )
                        switch( reptCount ){
                        case 2:
                            ASSERT( offset < 32 )
                            if( offset > 7 ){
                                *o++ = N | offset;
                                offset = 0;
                            }
                            *o++ = 0x08 | offset; // R2 -> 00001ooo
                            *o++ = b;
                            *o++ = N | 1;
                            return o - output;
                        case 3:
                            ASSERT( offset < 32 )
                            if( offset > 7 ){
                                *o++ = N | offset;
                                offset = 0;
                            }
                            *o++ = 0x10 | (7 & offset); // R3 -> 00010ooo
                            *o++ = b;
                            *o++ = N | 1;
                            return o - output;
                        case 4:
                            ASSERT( offset < 32 )
                            if( offset > 7 ){
                                *o++ = N | offset;
                                offset = 0;
                            }
                            *o++ = 0x18 | offset; // R4 -> 00011ooo
                            *o++ = b;
                            *o++ = N | 1;
                            return o - output;
                        case 5:
                            ASSERT( offset < 32 )
                            if( offset > 6 ){
                                *o++ = N | offset;
                                offset = 0;
                            }
                            *o++ = 0x00 | (offset+1); // R5 -> 00000ooo
                            *o++ = b;
                            *o++ = N | 1;
                            return o - output;
                        default:
                            ASSERT( 0 )
                        }
                    }
                }else{ // b_1 == b ){ // now a repeated byte: xx, Rx xx
                    reptCount++;
                    ASSERT( 2 <= reptCount && reptCount <= 5 )
                    switch( reptCount ){
                    case 2:
                        ASSERT( offset < 32 )
                        if( offset > 7 ){
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = 0x08 | offset; // R2 -> 00001ooo
                        return o - output;
                    case 3:
                        ASSERT( offset < 32 )
                        if( offset > 7 ){
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = 0x10 | (7 & offset); // R3 -> 00010ooo
                        return o - output;
                    case 4:
                        ASSERT( offset < 32 )
                        if( offset > 7 ){
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = 0x18 | offset; // R4 -> 00011ooo
                        return o - output;
                    case 5:
                        ASSERT( offset < 32 )
                        if( offset > 6 ){
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = 0x00 | offset; // R5 -> 00000ooo
                        return o - output;
                    default:
                        ASSERT( 0 )
                    }                       
                }
            }
        }
    }
}
