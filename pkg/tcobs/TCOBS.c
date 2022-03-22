/*! \file TCOBS.c
\author Thomas.Hoehenleitner [at] seerose.net
\details See ./TCOBSSpecification.md.
*******************************************************************************/

#include <stdint.h>
#include "TCOBS.h"

//! ASSERT checks for a true condition, otherwise stop.
#define ASSERT( condition )do{ if( !(condition) ){ for(;;){}}}while(0);

//! OUTB writes a non-sigil byte to output and increments offset. 
//! If offset reaches 31 a NOP sigil byte is inserted and offset is then set to 0.
#define OUTB( b ) do{ \
    *o++ = b; offset++; \
    if( offset == 31 ){ \
        *o++ = N | 31; \
        offset = 0; \
    } \
    } while( 0 ); 

//! OUT_zeroSigil writes one of the sigil bytes Z1, Z3, Z3 
//! according to zeroCount and sets zeroCount=0 and offset=0.
#define OUT_zeroSigil do{ \
    ASSERT( b_1 == 0  ); \
    ASSERT( (fullCount|reptCount) == 0 ); \
    ASSERT( zeroCount < 4 ); \
    *o++ = (zeroCount << 5) | offset; \
    offset = 0; \
    zeroCount = 0; \
    }while(0);

//! OUT_fullSigil writes one of the sigil bytes F2, F3, F4 
//! according to fullCount and sets fullCount=0 and offset=0.
#define OUT_fullSigil do{ \
    ASSERT( b_1 == 0xFF ); \
    ASSERT( (zeroCount|reptCount) == 0 ); \
    ASSERT( 2 <= fullCount && fullCount <= 4 ); \
    *o++ = 0x80 | (fullCount << 5) | offset; \
    offset = 0; \
    fullCount = 0; \
    }while(0);

//! OUT_reptSigil writes one of the sigil bytes R2, R3, R4 
//! according to reptCount and sets reptCount=0 and offset=0.
#define OUT_reptSigil do{ \
    ASSERT( (zeroCount|fullCount) == 0 ); \
    ASSERT( 2 <= reptCount && reptCount <= 4 ); \
    if( offset > 7 ){ \
        *o++ = N | offset; \
        offset = 0; \
    } \
    *o++ = ((reptCount-1) << 3) | offset; \
    offset = 0; \
    reptCount = 0; \
    }while(0);

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
    int reptCount = 0; // counts repeat bytes 1-4 for xx and R2-R4,
    uint8_t b_1 = 0; // previous byte
    uint8_t b = 0; // current byte

    if( length < 3 ){ // special cases
        if( length == 0 ){ 
            return 0;
        }
        if( length == 1 ){
            b = *i;
            goto lastByte;
        }
        if( length == 2 ){
            b_1 = *i++;
            b = *i++;
            goto lastTwoBytes;
        }
    }

    // length is >= 3

    b = *i++; // get first byte
    for(;;){
        b_1 = b; // keep byte for compare
        b = *i++; // get next byte

        if( limit - i > 0 ){ // most likely case, loop continuation
            if( (b_1 | b) == 0){ // both 0         
                zeroCount++;
                if( zeroCount == 3 ){ // , 00 00 00 00
                    OUT_zeroSigil 
                } // Z3, 00
                continue;
            }
            if( (b_1 & b) == 0xFF ){ // both FF    
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
                    offset++; // xx, xx xx xx xx xx
                    OUT_reptSigil
                } // xx R4, xx
                continue;                
            }

            // at this point is b_1 != b

            // handle counts
            if( zeroCount ) { // 00
                zeroCount++;
                OUT_zeroSigil // Zn,
                continue;
            }
            if( fullCount ) { // FF
                fullCount++;
                OUT_fullSigil // Fn,
                continue;
            }
            if( reptCount ) { // xx
                *o++ = b_1;
                offset++;
                OUT_reptSigil // xx Rn,
                continue;
            }

            // at this point all counts are 0, b_1 != b and b_1 = xx, b == yy

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
            // Used  for examples: 
            //     All left from comma is written to o
            //     zn, fn, rn right from comma is count, if not shown, then 0
            //     between comma and dot are b_1 and b
            //     when bytes right from dot still inside loop 
            //     AA is a byte !=0, xx and yy are any bytes
            // example: ... , z0 00 00. AA xx
            //      ->: ... , z1 00 AA. xx yy
            //      ->: ... Z2, z0 AA xx. yy    == Z2 was written because of AA!=00
            //      ->: ... Z2 AA, z0 xx yy.    <- here
            // example: ... , z1 00 00. AA xx
            //      ->: ... , z2 00 AA. xx yy
            //      ->: ... Z3, z0 AA xx. yy    == Z3 was written because of AA!=00
            //      ->: ... Z3 AA, z0 xx yy.    <- here
            // example: ... , z0 00 00. 00 xx 
            //      ->: ... , z1 00 00. xx
            //      ->: ... , z2 00 xx.         == here
            // example: ... , r2 AA 00. xx yy
            //      ->: ... AA R2, z0 r0 00 xx. yy
            //      ->: ... AA R2, z1 xx yy.

            if( (zeroCount | fullCount | reptCount) == 0){ // no counts at all
lastTwoBytes:
                if( b_1 == 0 && b == 0 ){
                    zeroCount = 2;
                    OUT_zeroSigil
                    return o - output;
                }
                if( b_1 == 0xFF && b == 0xFF ){
                    fullCount = 2;
                    OUT_fullSigil
                    return o - output;
                }
                if( b_1 == 0 ){ // b != 0
                    zeroCount = 1;
                    OUT_zeroSigil
                    goto lastByte;
                }
                OUTB( b_1 )
                goto lastByte;
lastByte:
                if( b == 0 ){
                    *o++ = Z1 | offset;
                    return o - output;
                }else{
                    *o++ = b;
                    *o++ = N | (offset+1);
                    return o - output;
                }
            }

            // at this point exactly one count was incremented and is maybe related to b_1
            
            if( zeroCount == 1 ) { 
                if( b_1 != 0 && b != 0 ){
                    OUT_zeroSigil
                    goto lastTwoBytes;
                }
                if( b_1 == 0 && b == 0 ){
                    zeroCount = 3;
                    OUT_zeroSigil
                    return o - output;
                }
                if( b_1 == 0 ){ // b !=0
                    zeroCount = 2;
                    OUT_zeroSigil
                    goto lastByte;
                }
                OUTB( b_1 )
                goto lastByte;
            }
            if( zeroCount == 2 ) { 
                if( b_1 != 0 && b != 0 ){
                    OUT_zeroSigil
                    goto lastTwoBytes;
                }
                if( b_1 == 0 && b == 0 ){
                    OUT_zeroSigil // Z2
                    zeroCount = 2;
                    OUT_zeroSigil // Z2
                    return o - output;
                }
                if( b_1 == 0 ){ // b !=0
                    zeroCount = 3;
                    OUT_zeroSigil
                    goto lastByte;
                }
                OUTB( b_1 )
                goto lastByte;
            }
            if( fullCount == 1 ) { 
            }
            if( fullCount == 2 ) { 
            }
            if( fullCount == 3 ) { 
            }
            if( fullCount == 4 ) { 
            }
            if( reptCount == 1 ) { 
            }
            if( reptCount == 2 ) { 
            }
            if( reptCount == 3 ) { 
            }
            if( reptCount == 4 ) { 
            }
        }
    }
}
