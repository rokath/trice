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
    *o++ = b; \
    offset++; \
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
    ASSERT( 1 <= zeroCount && zeroCount <= 3 ); \
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
//! If offset is bigger than 7 a NOP sigil byte is inserted.
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

    if( length < 2 ){ // special cases
        if( length == 0 ){ // , -- --.
            return 0;
        }
        if( length == 1 ){ // , . xx
            b = *i; // , -- xx.
            goto lastByte;
        }
        /*if( length == 2 ){ // , -- --. xx yy
            b_1 = *i++; // , xx --. yy
            b = *i++; // , xx yy.
            goto lastTwoBytes;
        }*/
    }

    // , -- --. xx yy ... (length >= 2)

    // comment syntax:
    //     Sigil bytes chaining is not shown explicitly.
    //     All left from comma is already written to o and if, only partially shown.
    //     n is 0...3|4 and m is n+1, representing count number.
    //     zn, fn, rn right from comma is count in variables, if not shown, then 0.
    //     At any moment only one of the 3 counters can be different from 0.
    //     Zn, Fn, Rn, Nn are (written) sigil bytes.
    //     Between comma and dot are the 2 values b_1 and b.
    //     3 dots ... means it is unkwown if bytes follow. 
    //     aa is a byte !=0, xx yy and zz are any bytes.
    //     !FF is any byte but not 0xFF.
    //     Invalid b_1 and b are displayed as --.

    b = *i++; // , -- xx, yy ...
    for(;;){
            b_1 = b; // , xx --. yy ...
            b = *i++; // , xx yy, ...

            if( limit - i > 0 ){ // , xx yy. zz ...

                // , z0 00 00. -> , f1 -- 00.
                // , z0 00 00. 00 -> , f2 -- 00.

                if( (b_1 | b) == 0){ // , zn 00 00. zz ...      
                    zeroCount++; // , zm -- 00. zz ...
                    if( zeroCount == 2 ){ // , z2 -- 00. zz ...
                        zeroCount = 3; // , z3 -- --. zz ...
                        OUT_zeroSigil // Z3, -- --. zz ...
                        b = *i++; // , -- zz. ...
                        if( limit - i == 0 ){ // , -- zz.
                            goto lastByte;
                        }
                        // , -- zz. ...
                } 
                    continue; // , fn -- 00. ...  
                }

                // , f0 FF FF. -> , f1 -- FF.
                // , f0 FF FF. FF -> , f2 -- FF.
                // , f0 FF FF. FF FF -> , f3 -- FF.

                if( (b_1 & b) == 0xFF ){ // , fn FF FF. zz ...
                    fullCount++; // , fm -- FF. zz ...
                    if( fullCount == 3 ){ // , f3 -- FF. zz ...
                        fullCount = 4; // , f4 -- --. zz ...
                        OUT_fullSigil // F4, -- --. zz ...
                        b = *i++; // , -- zz. ...
                        if( limit - i == 0 ){ // , -- zz.
                            goto lastByte;
                        }
                        // , -- zz. ...
                    } 
                    continue; // , fn -- xx. ...            
                }

                // , r0 aa aa. -> , r1 -- aa.
                // , r0 aa aa. aa -> , r2 -- aa.
                // , r0 aa aa. aa aa -> , r3 -- aa.
                // , r0 aa aa. aa aa aa -> , r4 -- aa.
                if( b_1 == b  ){ // , rn aa aa. ...    
                    reptCount++; // , rm -- aa. ... 
                    if( reptCount == 4 ){ // , r4 -- aa. ...
                        OUTB( b ) // aa, r4 -- --. ...
                        OUT_reptSigil // aa R4, -- --. ...
                        b = *i++; // , -- xx. ...
                        if( limit - i == 0 ){ // , -- xx.
                            goto lastByte;
                        }
                        // , -- xx. ...
                    } 
                    continue; // , rn -- xx. ...             
                }

                // , zn fn rn xx yy. ... (at this point is b_1 != b)

                // handle counts
                if( zeroCount ) { // , z1|z2 00 aa. xx yy
                    zeroCount++;
                    OUT_zeroSigil
                    continue; // Z2|Z3, z0 -- aa. ...
                }
                if( fullCount ) { // , f1|f2|f3 xx yy. ...
                    fullCount++; // , f2|f3|f4 -- aa. ...
                    OUT_fullSigil // , 
                    continue; // Fn, f0 -- aa. ...
                }
                if( reptCount ) { // , r1|r2|r3 aa bb. ...
                    *o++ = b_1;
                    offset++;  // aa, r1|r2|r3 -- bb. ...
                    OUT_reptSigil 
                    continue;  // aa R1|R2|R3, r0 -- bb. ...
                }

                // at this point all counts are 0, b_1 != b and b_1 = xx, b == yy

                if( b_1 == 0 ){ // , 00 aa. ...
                    OUT_zeroSigil
                    continue;  // Z1, -- aa. ...
                }
                if( b_1 == 0xFF ){ // , FF aa. ...
                    OUTB( 0xFF );
                    continue; // FF, -- aa. ...
                }
                // , 01|...|FE aa. ... 
                OUTB( b_1 ) 
                continue; // 01|...|FE, -- aa. ...

            }else{ // last 2 bytes

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
lastTwoBytes: // , xx yy.
                    if( b_1 == 0 && b == 0 ){ // , 00 00.
                        zeroCount = 2; // , z2 -- --.
                        OUT_zeroSigil // Z2, -- --
                        return o - output;
                    }
                    if( b_1 == 0xFF && b == 0xFF ){ // , FF FF.
                        fullCount = 2; // , f2 -- --.
                        OUT_fullSigil // F2, -- --.
                        return o - output;
                    }
                    if( b_1 == 0 ){ // , 00 xx.
                        zeroCount = 1; // , z1 -- xx.
                        OUT_zeroSigil // Z1, -- xx.
                        goto lastByte;
                    }
                    // , aa xx.
                    OUTB( b_1 ) // aa, -- xx.
                    goto lastByte;
lastByte: // , -- xx.
                    if( b == 0 ){ // , -- 00.
                        *o++ = Z1 | offset; // Z1, -- --.
                        return o - output;
                    }else{ // , -- aa.
                        *o++ = b; // aa, -- --.
                        offset++;
                        *o++ = N | offset; // aa Nn, -- --.
                        return o - output;
                    }
                }

                // at this point exactly one count was incremented
                
                if( zeroCount == 1 ) { // , z1 xx yy
                    if( b_1 != 0 && b != 0 ){ // , z1 xx yy.
                        OUT_zeroSigil // Z1, xx yy.
                        goto lastTwoBytes;
                    }
                    if( b_1 == 0 && b == 0 ){ // , z1 00 00.
                        zeroCount = 3; // , z3 -- --.
                        OUT_zeroSigil // Z3, -- --.
                        return o - output;
                    }
                    if( b_1 == 0 ){ // , z1 00 aa.
                        zeroCount = 2; // , z2 -- aa.
                        OUT_zeroSigil // Z2, -- aa.
                        goto lastByte;
                    }
                    // , z1 aa xx.
                    OUT_zeroSigil // Z1, aa xx.
                    OUTB( b_1 ) // Z1 aa, -- xx.
                    goto lastByte;
                }
                if( zeroCount == 2 ) { // , z2 xx yy.
                    if( b_1 != 0 && b != 0 ){ // , z2 aa bb.
                        OUT_zeroSigil // Z2, aa bb.
                        goto lastTwoBytes;
                    }
                    if( b_1 == 0 && b == 0 ){ // , z2 00 00.
                        OUT_zeroSigil // Z2, 00 00.
                        zeroCount = 2; // Z2, z2 -- --.
                        OUT_zeroSigil // Z2 Z2, -- --.
                        return o - output;
                    }
                    if( b_1 == 0 ){ // , z2 00 aa.
                        zeroCount = 3; // , z3 -- aa.
                        OUT_zeroSigil // Z3, -- aa.
                        goto lastByte;
                    }
                    // , z2 aa xx.
                    OUT_zeroSigil // Z2, aa xx.
                    OUTB( b_1 ) // Z2 aa, -- xx.
                    goto lastByte;
                }
                if( fullCount == 1 ) { // , f1 xx yy.
                    if( b_1 == 0xFF ){ // , f1 FF yy.
                        if( b == 0xFF ){ // , f1 FF FF.
                            fullCount = 3; // , f3 -- --.
                            OUT_fullSigil  // F3, -- --.
                            return o - output;
                        }
                        fullCount = 2; // , f2 -- yy
                        OUT_fullSigil  // F2, -- yy
                        goto lastByte;
                    }
                    if( b_1 == 00 ){ // , f1 00 xx.
                        OUTB( 0xFF ) // FF, f1 00 xx.
                        fullCount = 0; // FF, 00 xx.
                        zeroCount = 1; // FF, z1 -- xx.
                        OUT_zeroSigil // FF Z1, -- xx. 
                        goto lastByte;
                    }
                    // , f1 aa xx.
                    OUT_zeroSigil // F1, aa xx.
                    OUTB( b_1 ) // F1 aa, -- xx.
                    goto lastByte;
                }
                if( fullCount == 2 ) { // , f2 xx yy.
                    if( b_1 == 0xFF ){ // , f2 FF yy.
                        if( b == 0xFF ){ // , f2 FF FF.
                            fullCount = 4; // , f4 -- --.
                            OUT_zeroSigil // F4, -- --.
                            return o - output;
                        }
                        // , f2 FF xx
                        fullCount = 3; // , f3 -- xx.
                        OUT_fullSigil  // F3, -- yy.
                        goto lastByte;
                    }
                    OUT_fullSigil  // F2, xx yy.
                    goto lastTwoBytes;
                }
                if( fullCount == 3 ) { // , f3 xx yy.
                    if( b_1 == 0xFF ){ // , f3 FF yy.
                        if( b == 0xFF ){ // , f3 FF FF.
                            OUT_fullSigil  // F3, FF FF.
                            fullCount = 2; // F3, f2 -- --.
                            OUT_fullSigil  // F3 F2, -- --.
                            return o - output;
                        }
                        // , f3 FF !FF.
                        fullCount = 4; // , f4 -- xx.
                        OUT_fullSigil  // F4, -- xx.
                        goto lastByte;
                    }
                }
                ASSERT( fullCount == 0 )

                if( reptCount == 1 ) { // , r1 xx yy. 
                }
                if( reptCount == 2 ) { // , r2 xx yy.
                }
                if( reptCount == 3 ) { // , r4 xx yy.
                }

                ASSERT( 0 ) // will not be reached 
            }
        }
    }
}
