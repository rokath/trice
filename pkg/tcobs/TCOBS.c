/*! \file TCOBS.c
\author Thomas.Hoehenleitner [at] seerose.net
\details See ./TCOBSSpecification.md.
*******************************************************************************/

#include <stdint.h>
#include "TCOBS.h"

#define ASSERT( condition )do{ if( !(condition) ){ for(;;){}}}while(0); //! ASSERT checks for a true condition, otherwise stop.
#define OUTB( b ) do{ *o++ = b; offset++; if( offset == 31 ){ *o++ = N | 31; offset = 0; } } while( 0 ); //!< OUTB writes a non-sigil byte to output.


#define OUT_zeroSigil do{ ASSERT( b_1 == 0    ); ASSERT( fullCount|reptCount == 0 ); ASSERT(                  zeroCount < 4 );                                                    *o++ =        (zeroCount    << 5) | offset; offset = 0; zeroCount = 0; }while(0)
#define OUT_fullSigil do{ ASSERT( b_1 == 0xFF ); ASSERT( zeroCount|reptCount == 0 ); ASSERT( 1 < fullCount && fullCount < 5 );                                                    *o++ = 0x80 | (fullCount    << 5) | offset; offset = 0; fullCount = 0; }while(0)
#define OUT_reptSigil do{                        ASSERT( zeroCount|fullCount == 0 ); ASSERT( 1 < reptCount && reptCount < 5 ); if( offset > 7 ){ *o++ = N | offset; offset = 0; } *o++ =        (reptCount-1) << 3) | offset; offset = 0; reptCount = 0; }while(0)



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

int TCOBSEncode( uint8_t* restrict output,  uint8_t const * restrict input, unsigned length){
    uint8_t* o = output;
    uint8_t const * i = input;
    uint8_t const * limit = input + length;
    int offset = 0; // sigil chain link
    int zeroCount = 0; // counts zero bytes 1-3 for Z1-Z3
    int fullCount = 0; // counts 0xFF bytes 1-4 for 0xFF and F2-F4
    int reptCount = 0; // counts repeat bytes 1-8 for xx and R2-R4,
                       //                     1-7 for xx and R5
    uint8_t b_1 = 0; // previous byte
    uint8_t b = 0; // current byte

    if( length < 3 ){ // special cases
        if( length == 0 ){ 
            return 0;
        }
        if( length == 1 ){
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
        if( length == 2 ){ 
            b_1 = *i++;
            b = *i++;
            if( (b_1 | b) == 0 ){
                *o++ = Z2;
                return 1;
            } else if( b_1 == 0){
                *o++ = Z1;
                *o++ = b;
                *o++ = N | 1;
                return 3;
            } else if( b == 0 ){
                *o++ = b_1;
                *o++ = Z1 | 1;
                return 2;
            } else {
                *o++ = b_1;
                *o++ = b;
                *o++ = N | 2;
                return 3;
            }
        }
    }

    // length >= 3
    b = *i++; // get first byte
    for(;;){
        b_1 = b; // keep byte for compare
        b = *i++; // get next byte

        if( limit - i > 0 ){ // most likely case, loop continuation
            if( b_1|b == 0){ // both 0         
                zeroCount++;
                if( zeroCount == 3 ){ // , 00 00 00 00
                    OUT_zeroSigil 
                } // Z3, 00
                continue;
            }
            if( b_1 & b == 0xFF ){ // both FF    
                fullCount++;
                if( fullCount == 4 ){ // , FF FF FF FF FF
                    OUT_fullSigil
                } // F4, FF
                continue;                
            }
            if( b_1 == b  ){ // both equal      
                reptCount++;
                if( reptCount == 5 ){ // , xx xx xx xx xx xx
                    *o++ = b_1;
                    offset++: // xx, xx xx xx xx xx
                    OUT_reptSigil
                } // xx R4, xx
                continue;                
            }

            // at this point b_1 != b

            // handle counts
            if( zeroCount ) // 00
                zeroCount++;
                OUT_zeroSigil // Zn,
                continue;
            if( fullCount ) // FF
                fullCount++;
                OUT_fullSigil // Fn,
                continue;
            if( reptCount ) // xx
                *o++ = xx;
                offset++;
                OUT_reptSigil // xx Rn,
                continue;

            // at this point all counts 0, b_1 != b and b_1 == 00 || FF || xx, b == 00 || FF || yy

            if( b_1 == 0 ){ // , 00 xx
                OUT_zeroSigil // Z1, xx
                continue;
            }
            if( b_1 == 0xFF ){ // , FF xx
                OUTB( 0xFF ); // FF, xx
                continue;
            }
            // , b_1 = 01...FE 
            OUTB( b_1 ) // b_1,
            continue;

        }else{ // last 2 bytes

        counts sind nicht ganz klar

            if( b_1 | b == 0 ) { // Zn 00 00
                switch( zeroCount ){
                case 0: // , 00 00
                    zeroCount = 2;
                    OUT_zeroSigil // Z2,
                    return o - output;
                case 1: // , Z1 00 00
                    zeroCount = 3;
                    OUT_zeroSigil // Z3, 
                    return o - output;
                case 2: // , Z1 00 00
                    zeroCount = 3;
                    OUT_zeroSigil // Z3, 00
                    *o++ = Z1; // Z3 Z1,
                    return o - output;
                default:
                    ASSERT(0)
                    return 0;
                }
            }

            if( b_1 == 0 ){ // , Zn 00 xx
                switch( zeroCount ){
                case 0: // , 00 xx
                    zeroCount = 2;
                    OUT_zeroSigil // Z2,
                    return o - output;
                case 1: // , Z1 00 00
                    zeroCount = 3;
                    OUT_zeroSigil // Z3, 
                    return o - output;
                case 2: // , Z1 00 00
                    zeroCount = 3;
                    OUT_zeroSigil // Z3, 00
                    *o++ = Z1; // Z3 Z1,
                    return o - output;
                default:
                    ASSERT(0)
                    return 0;
                }

            }


            }
            if( zeroCount && ((b_1|b)==0) ){ // , Zn 00 00
                zeroCount++;
                if( )



            if( zeroCount ) // 00 b_1 b
                zeroCount++;
                OUT_zeroSigil // Zn,
                continue;
            if( fullCount ) // FF
                fullCount++;
                OUT_fullSigil // Fn,
                continue;
            if( reptCount ) // xx
                *o++ = xx;
                offset++;
                OUT_reptSigil // xx Rn,
                continue;













        // zeroCount > 0 means 0 occurred zeroCount times but not written yet
        // fullCount > 0 means 0xFF occurred fullCount times but not written yet
        // reptCount > 0 means b_1 occurred reptCount times but not written yet
        ASSERT( (zeroCount>0) + (fullCount>0) + (reptCount>0) <= 1 )
        ASSERT( zeroCount < 3 )
        ASSERT( fullCount < 4 )
        ASSERT( reptCount < 4 )
            if( (b_1 == 0 ){
                if( fullCount > 0 ){
                    hier weiter
                } 
                zeroCount++; // Z1=001ooooo, Z2=010ooooo, Z3=011ooooo
                if( zeroCount == 3 ){
                    *o++ == Z3 | offset;
                    offset = 0;
                }
                if( b == 0 ){
                    zeroCount++;
                    *o++ = (zeroCount<<5)| offset;
                    return o - output;
                }else{
                    *o++ = b;
                    offset++;
                    *o++ = N | offset;
                    return o - output;
                }
            }else if( b_1 == 0xFF ){
                fullCount++;
                if( fullCount == 4 ){
                    *o++ = F4 | offset;
                    offset = 0;
                }
                if
            }
            }else{ // b_1 != 0
                *o++ = b_1;
                offset++;
                if( b == 0 ){
                    zeroCount++;
                    *o++ = (zeroCount<<5)| offset;
                    return o - output;
                }else{ // b != 0
                    if( offset == 31 ){
                        *o++ = N | offset;
                        offset = 0;
                    }
                    *o++ = b;
                    offset++;
                    *o++ = N | offset;
                    return o - output;
                }
            }
                
                

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
