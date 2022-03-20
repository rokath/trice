/*! \file TCOBS.c
\author Thomas.Hoehenleitner [at] seerose.net
\details See ./TCOBSSpecification.md.
*******************************************************************************/

#include <stdint.h>
#include "TCOBS.h"

#define ASSERT( condition )do{ if( !condition ){ for(;;){}}}while(0); //! Check for a true condition, otherwise stop.

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
    int zeroCount = 0; // counts zero bytes 1-3 for Z1-Z3
    int fullCount = 0; // counts 0xFF bytes 1-4 for 0xFF and F2-F4
    int reptCount = 0; // counts repeat bytes 1-8 for xx and R2-R4,
                       //                     1-7 for xx and R5
    uint8_t b_1; // previous byte
    uint8_t b; // current byte

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
        b_1 = b; // keep last value byte
        b = *input--; // get next byte
        if( length == 1 ){
            if( b == 0 ){
                zeroCount++; // Z1=001ooooo, Z2=010ooooo, Z3=011ooooo
                ASSERT( zeroCount <= 3 )
                *p++ = (zeroCount<<5)|(0x1F & offset);
                return p - output;
            }else if( b == 0xFF ){ // a FF and ...
                if( fullCount == 0 ){ // ... no previous FF
                    *p++ = 0xFF;
                    offset++;
                    ASSERT( offset <= 32 ) 
                    *p++ = N | offset; // N=0x101ooooo
                    return p - output;
                }else{ // ... 1-3 previous 0xFF
                    fullCount++; // 2 or 3 or 4
                    ASSERT( fullCount <= 4 )
                    ASSERT( offset <= 32 )
                    *p++ = 0x80 | (fullCount<<5) | (0x1F & offset); // F2=110ooooo, F3=111ooooo, F4=100ooooo
                    return p - output;
                }
            }else{ // b = 01...FE
                if( reptCount == 0 ){
                    *p++ = b; 
                    offset++;
                    ASSERT( offset <= 32 ) 
                    *p++ = N | offset; // N=0x101ooooo
                    return p - output;
                }else{ // b_1 has a valid value (start length > 1)
                    if( b_1 != b ){ // now a different byte: xx, Rx yy
                        if( reptCount == 1 ){ // xx, R1 yy
                            *p++ = b_1; // xx xx, yy
                            offset++;
                            if( offset == 32 ){
                                *p++ = N; // N=0x101ooooo
                                offset = 0;
                            }
                            *p++ = b;  // xx xx yy,
                            offset++;
                            ASSERT( offset <= 32 )
                            *p++ = N | offset;
                            return p - output;
                        }else{ // xx, Rn yy
                            reptCount++;
                            ASSERT( 2 <= reptCount && reptCount <= 5 )
                            switch( reptCount ){
                                case 2:
                                    ASSERT( 1 <= offset && offset <= 8 )
                                    *p++ = 0x08 | offset; // R2 -> 00001ooo
                                    *p++ = b;
                                    return p - output;
                                case 3:
                                    ASSERT( 1 <= offset && offset <= 8 )
                                    *p++ = 0x10 | (7 & offset); // R3 -> 00010ooo
                                    *p++ = b;
                                    return p - output;
                                case 4:
                                    ASSERT( 1 <= offset && offset <= 8 )
                                    *p++ = 0x18 | offset; // R4 -> 00011ooo
                                    *p++ = b;
                                    return p - output;
                                case 5:
                                    ASSERT( 1 <= offset && offset <= 7 )
                                    *p++ = 0x00 | offset; // R5 -> 00000ooo
                                    *p++ = b;
                                    *p++ = N | 2;
                                    return p - output;
                            }
                        }
                    }else{ // b_1 == b ){ // now a repeated byte: xx, Rx xx

                    }
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

