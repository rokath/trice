/*! \file tcobsEncode.c
\author Thomas.Hoehenleitner [at] seerose.net
\details See ./TCOBSv1Specification.md.
*******************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include "tcobs.h"
#include "tcobsInternal.h"

//! ASSERT checks for a true condition, otherwise stop.
//! This macro was used during development to verify the code.
#define ASSERT( condition ) //  do{ if( !(condition) ){ for(;;){} } }while(0);

//! OUTB writes a non-sigil byte to output and increments offset. 
//! If offset reaches 31, a NOP sigil byte is inserted and offset is then set to 0.
#define OUTB( by ) do{ \
    *o++ = by; \
    offset++; \
    ASSERT( offset <= 31 ); \
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
    ASSERT( offset <= 31 ); \
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
    ASSERT( offset <= 31 ); \
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
    ASSERT( offset <= 31 ); \
    if( offset > 7 ){ \
        *o++ = N | offset; \
        offset = 0; \
    } \
    *o++ = ((reptCount-1) << 3) | offset; \
    offset = 0; \
    reptCount = 0; \
    }while(0);

int TCOBSEncode( void * restrict output, const void * restrict input, size_t length){
    uint8_t* o = output; // write pointer
    uint8_t* out = output;
    uint8_t const * i = input; // read pointer
    uint8_t const * limit = (uint8_t*)input + length; // read limit
    uint8_t zeroCount = 0; // counts zero bytes 1-3 for Z1-Z3
    uint8_t fullCount = 0; // counts 0xFF bytes 1-4 for FF and F2-F4
    uint8_t reptCount = 0; // counts repeat bytes 1-4 for !00 and R2-R4,
    uint8_t b_1 = 0; // previous byte
    uint8_t b = 0; // current byte
    uint8_t offset = 0; // link to next sigil or buffer start looking backwards
    // comment syntax:
    //     Sigil bytes chaining is done with offset and not shown explicitly.
    //     All left from comma is already written to o and if, only partially shown.
    //     n is 0...3|4 and m is n+1, representing count number.
    //     zn, fn, rn right from comma is count in variables, if not shown, then 0.
    //     At any moment only one of the 3 counters can be different from 0.
    //     Zn, Fn, Rn, Nn are (written) sigil bytes.
    //     Between comma and dot are the 2 values b_1 and b.
    //     3 dots ... means it is unknown if bytes follow. 
    //     !00 is a byte != 00.
    //     !FF is a byte != FF.
    //     aa is not 00 and not FF and all aa in a row are equal.
    //     xx yy and zz are any bytes.
    //     Invalid b_1 and b are displayed as --.

    if( length >= 2 ){ // , -- --. xx yy ...
        b = *i++; // , -- xx. yy ...
        for(;;){ // , zn|fn|rn -- xx. yy ...
            b_1 = b; // , xx --. yy ...
            b = *i++; // , xx yy. ...

            if( limit - i > 0 ){ // , xx yy. zz ...

                // , z0 00 00. -> , z1 -- 00.
                // , z0 00 00. 00 -> , z2 -- 00.

                if( (b_1 | b) == 0){ // , zn 00 00. zz ...      
                    zeroCount++; // , zm -- 00. zz ...
                    if( zeroCount == 2 ){ // , z2 -- 00. zz ...
                        zeroCount = 3; // , z3 -- --. zz ...
                        OUT_zeroSigil // Z3, -- --. zz ...
                        b = *i++; // , -- zz. ...
                        if( limit - i == 0 ){ // , -- zz.
                            goto lastByte;
                        }
                        // , -- xx. yy ...
                    } 
                    continue; // , zn -- xx. yy ...  
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
                        // , -- xx. yy ...
                    } 
                    continue; // , fn -- xx. yy ...
                }

                // , r0 aa aa. -> , r1 -- aa.
                // , r0 aa aa. aa -> , r2 -- aa.
                // , r0 aa aa. aa aa -> , r3 -- aa.
                // , r0 aa aa. aa aa aa -> , r4 -- aa.
                
                if( b_1 == b  ){ // , rn aa aa. xx ...
                    ASSERT( b_1 != 0 );
                    ASSERT( b_1 != 0xFF );
                    reptCount++; // , rm -- aa. xx ... 
                    if( reptCount == 4 ){ // , r4 -- aa. xx ...
                        OUTB( b ) // aa, r4 -- --. xx ...
                        OUT_reptSigil // aa R4, -- --. xx ...
                        b = *i++; // , -- xx. ...
                        if( limit - i == 0 ){ // , -- xx.
                            goto lastByte;
                        }
                        // , -- xx. yy ...
                    } 
                    continue; // , r1|r2|r3 -- aa. yy ... 
                }       // OR // , -- xx. yy ...

                // , zn|fn|rn xx yy. zz ... (at this point is b_1 != b)

                // handle counts
                if( zeroCount ) { // , z1|z2 00 aa. xx ...
                    ASSERT( 1 <= zeroCount && zeroCount <= 2 )
                    ASSERT( b_1 == 0 )
                    ASSERT( b_1 != b )
                    zeroCount++; // , z2|z3 -- aa. xx ...
                    OUT_zeroSigil // Z2|Z3, -- aa. xx ...
                    continue; 
                }
                if( fullCount ) { // , f1|f2|f3 FF !FF. xx ...
                    ASSERT( 1 <= fullCount && fullCount <= 3 )
                    ASSERT( b_1 == 0xFF )
                    ASSERT( b_1 != b )
                    fullCount++; // , f2|f3|f4 -- !FF. xx ...
                    OUT_fullSigil // Fn, -- !FF. xx ...
                    continue;
                }
                if( reptCount ) { // , r1|r2|r3 aa !aa. xx ...
                    ASSERT( 1 <= reptCount && reptCount <= 3 )
                    ASSERT( b_1 != 0 )
                    ASSERT( b_1 != 0xFF )
                    ASSERT( b_1 != b )
                    if( reptCount == 1 ){ // , r1 aa !aa. xx ...
                        reptCount = 0; // clear
                        OUTB( b_1 ) // aa, r0 aa !aa. xx ...
                        OUTB( b_1 ) // aa aa, -- !aa. xx ...
                        continue;
                    }
                    *o++ = b_1; // aa, r2|r3 -- !aa. xx ...
                    offset++;
                    OUT_reptSigil // aa R1|R2|R3, -- !aa. xx ...
                    continue;
                }

                // at this point all counts are 0, b_1 != b and b_1 = xx, b == yy
                ASSERT( zeroCount == 0 )
                ASSERT( fullCount == 0 )
                ASSERT( reptCount == 0 )
                ASSERT( b_1 != b )

                // , xx yy. zz ...
                if( b_1 == 0 ){ // , 00 !00. xx ...
                    ASSERT( b != 0 )
                    zeroCount++; // , z1 -- !00. xx ...
                    OUT_zeroSigil
                    continue;  // Z1, -- !00. xx ...
                }
                if( b_1 == 0xFF ){ // , FF !FF. xx ...
                    ASSERT( b != 0xFF )
                    OUTB( 0xFF ); // FF, -- !FF. xx ...
                    continue; 
                }
                
                // , aa xx. yy ...
                ASSERT( 1 <= b_1 && b_1 <= 0xFE )
                OUTB( b_1 ) // aa, -- xx. yy ...
                continue; 

            }else{ // last 2 bytes
                // , zn|fn|rn xx yy.
                if( (zeroCount | fullCount | reptCount) == 0){ // , xx yy.
                    if( b_1 == 0 && b == 0 ){ // , 00 00.
                        *o++ = Z2 | offset; // Z2, -- --.
                        return o - out;
                    }
                    if( b_1 == 0xFF && b == 0xFF ){ // , FF FF.
                        *o++ = F2 | offset; // F2, -- --.
                        return o - out;
                    }
                    if( b_1 == 0 ){ // , 00 xx.
                        zeroCount = 1; // , z1 -- xx.
                        OUT_zeroSigil // Z1, -- xx.
                        goto lastByte;
                    }
                    // , aa xx.
                    ASSERT( b_1 != 0 )
                    OUTB( b_1 ) // aa, -- xx.
                    goto lastByte;
                }

                // At this point exactly one count was incremented.
                // If a count is >0 it is necessarily related to b_1.
                
                if( zeroCount == 1 ) { // , z1 00 yy
                    ASSERT( fullCount == 0 )
                    ASSERT( reptCount == 0 )
                    ASSERT( b_1 == 0 )
                    if( b != 0 ){ // , z1 00 !00.
                        zeroCount++; // , z2 -- !00.
                        OUT_zeroSigil // Z2, -- !00.
                        goto lastByte;
                    }
                    if( b == 0 ){ // , z1 00 00.
                        *o++ = Z3 | offset; // Z3, -- --.
                        return o - out;
                    }
                    ASSERT( 0 )
                }

                if( zeroCount == 2 ) { // , z2 00 !00.
                    ASSERT( fullCount == 0 )
                    ASSERT( reptCount == 0 )
                    ASSERT( b_1 == 0 )
                    ASSERT( b != 0 )
                    zeroCount = 3; // , z3 -- aa.
                    OUT_zeroSigil // Z3, -- aa.
                    goto lastByte;
                }

                if( fullCount == 1 ) { // , f1 FF yy.
                    ASSERT( zeroCount == 0 )
                    ASSERT( reptCount == 0 )
                    ASSERT( b_1 == 0xFF )
                    if( b == 0xFF ){ // , f1 FF FF.
                        ASSERT( offset <= 31 );
                        *o++ = F3 | offset; // F3, -- --.
                        return o - out;
                    }
                    fullCount = 2; // , f2 -- yy.
                    OUT_fullSigil  // F2, -- yy.
                    goto lastByte;
                }

                if( fullCount == 2 ) { // , f2 FF yy.
                    ASSERT( zeroCount == 0 )
                    ASSERT( reptCount == 0 )
                    ASSERT( b_1 == 0xFF )
                    if( b == 0xFF ){ // , f2 FF FF.
                        ASSERT( offset <= 31 );
                        *o++ = F4 | offset; // F4, -- --.
                        return o - out;
                    }
                    // , f2 FF !FF
                    fullCount++; // , f3 -- !FF.
                    OUT_fullSigil  // F3, -- !FF.
                    goto lastByte;
                }

                if( fullCount == 3 ) { // , f3 FF yy.
                    ASSERT( zeroCount == 0 )
                    ASSERT( reptCount == 0 )
                    ASSERT( b_1 == 0xFF )
                    if( b == 0xFF ){ // , f3 FF FF.
                        OUT_fullSigil  // F3, FF FF.
                        ASSERT( offset <= 31 );
                        *o++ = F2 | offset; // F3 F2, -- --.
                        return o - out; // option: F4 FF, -- --. is also right
                    }
                    // , f3 FF !FF.
                    fullCount = 4; // , f4 -- xx.
                    OUT_fullSigil  // F4, -- xx.
                    goto lastByte;
                }

                if( reptCount == 1 ) { // , r1 aa yy. 
                    ASSERT( zeroCount == 0 )
                    ASSERT( fullCount == 0 )
                    ASSERT( b_1 != 0 )
                    ASSERT( b_1 != 0xFF )
                    if( b_1 == b ){ // , r1 aa aa.
                        OUTB( b_1 ) // aa, r1 -- aa.
                        ASSERT( offset <= 31 );
                        if( offset > 7 ){ 
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = R2 | offset; // aa R2, -- --.
                        return o - out;
                    }
                    OUTB( b_1 ) // aa, r0 aa -- yy.
                    OUTB( b_1 ) // aa aa, -- yy.
                    goto lastByte;
                }
                if( reptCount == 2 ) { // , r2 aa yy.
                    ASSERT( zeroCount == 0 )
                    ASSERT( fullCount == 0 )
                    ASSERT( b_1 != 0 )
                    ASSERT( b_1 != 0xFF )
                    if( b_1 == b ){ // , r2 aa aa.
                        OUTB( b_1 ) // aa, r2 -- aa.
                        ASSERT( offset <= 31 );
                        if( offset > 7 ){ 
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = R3 | offset; // aa R3, -- --.
                        return o - out;
                    }                    
                    OUTB( b_1 ) // aa, r2 -- yy.
                    OUT_reptSigil  // aa R2, -- xx.
                    goto lastByte;
                }
                if( reptCount == 3 ) { // , r3 aa yy.
                    ASSERT( zeroCount == 0 )
                    ASSERT( fullCount == 0 )
                    ASSERT( b_1 != 0 )
                    ASSERT( b_1 != 0xFF )
                    OUTB( b_1 ) // aa, r3 -- yy. 
                    if( b_1 == b ){ // , r3 aa aa.
                        ASSERT( offset <= 31 );
                        if( offset > 7 ){
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = R4 | offset; // aa R4, -- --.
                        return o - out;
                    }
                    // aa, r3 -- yy.
                    OUT_reptSigil  // aa R3, -- xx.
                    goto lastByte;
                }

                ASSERT( 0 ) // will not be reached 
            }
        }
    }
    if( length == 0 ){ // , -- --.
        return 0;
    }
    if( length == 1 ){ // , . xx
        b = *i; // , -- xx.
        goto lastByte;
    }

lastByte: // , -- xx.
    if( b == 0 ){ // , -- 00.
        ASSERT( offset <= 31 );
        *o++ = Z1 | offset; // Z1, -- --.
        return o - out;
    }else{ // , -- aa.
        *o++ = b; // aa|ff, -- --.
        offset++;
        ASSERT( offset <= 31 );
        *o++ = N | offset; // aa Nn, -- --.
        return o - out;
    }
}
