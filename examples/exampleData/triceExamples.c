/*! \file triceExamples.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"

//! TriceHeadLine emits a decorated name. The name length should be 18 characters.
void TriceHeadLine(char* name){
    //! This is usable as the very first trice sequence after restart. Adapt it. Use a UTF-8 capable editor like VS-Code or use pure ASCII.
    TriceS(iD(16369), "w: Hello! 👋🙂\n\n        ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨        \n        🎈🎈🎈🎈%s🎈🎈🎈🎈\n        🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃        \n\n\n", name );
}

//! SomeExampleTrices generates a few Trice example logs and a burst of Trices.
void SomeExampleTrices(int burstCount){
    char* aString = "2.71828182845904523536";
    TriceS(iD(16368), "rd:%s <- float number as string\n", aString );
    Trice64(iD(16367), "msg:%.20f (double with more ciphers than precision)\n", aDouble( 2.71828182845904523536 ) );
    Trice(iD(16366), "msg:%.20f (float  with more ciphers than precision)\n",  aFloat( 2.71828182845904523536 ) );
    Trice(iD(16365), "msg:%f (default rounded float)\n",                       aFloat( 2.71828182845904523536 ) );
    Trice(iD(16364), "info:A Buffer:\n");
    Trice8B(iD(16363), "msg:%02x \n", aString, strlen(aString));
    Trice32B(iD(16362), "msg:%08x  \n", aString, strlen(aString)>>2);
    Trice16F(iD(16361), "att:ARemoteFunctionName", aString, strlen(aString)>>1);
    trice(iD(16360), "info:%d times a 16 byte long Trice messages, which not all will be written because of the TRICE_PROTECT:\n", burstCount);
    for( int i = 0; i < burstCount>>1; i++ ){
        Trice(iD(16359), "i=%x %x\n", 0x44444400 + i, 0xaaaaaa00 + i );
        TRice(iD(16358), "i=%x %x\n", 0x44444400 + i, 0xaaaaaa00 + i );
    }
}
