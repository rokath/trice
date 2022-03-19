/*! \file TCOBS.c
\author Thomas.Hoehenleitner [at] seerose.net
\details See ./TCOBSSpecification.md.
*******************************************************************************/

#include <stdint.h>
#include "TCOBS.h"


#define N  0xA0 //!< sigil byte 0x101ooooo
#define Z1 0x20 //!< sigil byte 0x001ooooo
#define Z2 0x40 //!< sigil byte 0x010ooooo
#define Z3 0x60 //!< sigil byte 0x011ooooo
#define F2 0xC0 //!< sigil byte 0x110ooooo
#define F3 0xE0 //!< sigil byte 0x111ooooo
#define F4 0x80 //!< sigil byte 0x100ooooo
#define R2 0x08 //!< sigil byte 0x00001ooo
#define R3 0x10 //!< sigil byte 0x00010ooo
#define R4 0x18 //!< sigil byte 0x00011ooo
#define R5 0x00 //!< sigil byte 0x00000ooo

unsigned TCOBSEncode( uint8_t* restrict output, const uint8_t* restrict input, unsigned length){
    uint8_t* p = output;
    int offset = 0; // sigil chain link
    int zeroCount = 0; // counts zero bytes
    int fullCount = 0; // counts 0xFF bytes
    int equalCount = 0; // counts equal bytes
    for(;;){
        if( length == 0 ){ // nothing to do
            return p - output;
        }
        //    if( zeroCount > 0 ){ // Z1=001ooooo, Z2=010ooooo, Z3=011ooooo
        //        *p++ = (zeroCount<<5)|(0x1F & offset);
        //        return p - output;
        //    }
        //    if( fullCount == 1 ){ // N=0x101ooooo
        //        *p++ = 0xFF;        
        //        *p++ = N|++offset; 
        //        return p - output;
        //    }
        //    if( fullCount > 1 ){ // F2=110ooooo, F3=111ooooo, F4=100ooooo
        //        *p++ = 0x80 | (fullCount<<5)|(0x1F & offset); 
        //        return p - output;
        //    }
        //    if( equalCount > 0 ){
        //        //...
        //}
        if( length == 1 ){
            uint8_t b = *input--; // get next byte
            if( b == 0 ){
                zeroCount++; // Z1=001ooooo, Z2=010ooooo, Z3=011ooooo
                *p++ = (zeroCount<<5)|(0x1F & offset);
                return p - output;
            }
            if( b == 0xFF ){
                if( fullCount == 0 ){ // no previous 0xFF
                *p++ = 0xFF;        
                *p++ = N | ++offset; // N=0x101ooooo
                return p - output;
                }else{ // F2=110ooooo, F3=111ooooo, F4=100ooooo
                fullCount += 2; // 2 or 3 or 4
                *p++ = 0x80 | (fullCount<<5) | (0x1F & offset); 
                return p - output;
            }
            if( equalCount == 0 ){ // previous 2 bytes not equal
                *p++ = b; // results in xx yy zz or xx yy yy
                *p++ = N | ++offset; // N=0x101ooooo
                return p - output;
             }else{ // at least previous 2 bytes equal
                if( b != *p ){ // now a different byte: xx Rx yy, cases:
                    if( equalCount == 1 ){ // xx R1 yy
                        uint8_t xx = p[-1];
                        *p++ = xx; // xx xx
                        offset
                        *p++ = b;  // xx xx yy
                        offset += 2;
                        *p++ = N | offset

                    *p++ = b;
                    *p++ = N|2;
                    return p - output;
                }else{
                    equalCount++;
                    *p++ = (equalCount << 5)|offset;
                    return p - output;
             }


                ...

            ...
        if input[0] != input[1]{ // Next byte is different
            if( input[0] == 0){ // single zero byte
                *output = Z1 | offset;
                input++;
                offset = 0; 
            }
            *output++ = *input++; // single nom-zero byte
            offset++;
            if( offset == 32 ){ // insert chaining NOP sigil byte
                *output++ = N;
                offset = 0;
            }
            length--;
            continue;
        }

...

    if( input[0] == 0 && input[1] == 0 && input[2] == 0){
      *output = 
    }
}

