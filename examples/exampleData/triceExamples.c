/*! \file triceExamples.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"

//! TriceHeadLine emits a decorated name. The name length should be 18 characters.
void TriceHeadLine(char* name){
    //! This is usable as the very first trice sequence after restart. Adapt it. Use a UTF-8 capable editor like VS-Code or use pure ASCII.
    TriceS("w: Hello! 👋🙂\n\n        ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨        \n        🎈🎈🎈🎈%s🎈🎈🎈🎈\n        🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃        \n\n\n", name );
}

//! SomeExampleTrices generates a few Trice example logs and a burst of Trices.
void SomeExampleTrices(int burstCount){
    TRICE(Id(0), "att:🐁 Speedy Gonzales \n");
    TRICE(Id(0), "att:🐁 Speedy Gonzales \n");
    TRICE(Id(0), "att:🐁 Speedy Gonzales \n");
    TRICE(Id(0), "att:🐁 Speedy Gonzales \n");
    char* aString = "2.71828182845904523536";
    TriceS("rd:%s <- float number as string\n", aString );
    Trice64("msg:%.20f (double with more ciphers than precision)\n", aDouble( 2.71828182845904523536 ) );
    Trice("msg:%.20f (float  with more ciphers than precision)\n",  aFloat( 2.71828182845904523536 ) );
    Trice("msg:%f (default rounded float)\n",                       aFloat( 2.71828182845904523536 ) );
    Trice("info:A Buffer:\n");
    Trice8B("msg:%02x \n", aString, strlen(aString));
    Trice32B("msg:%08x  \n", aString, strlen(aString)>>2);
    Trice16F("att:ARemoteFunctionName", aString, strlen(aString)>>1);
    trice("info:%d times a 16 byte long Trice messages, which not all will be written because of the TRICE_PROTECT:\n", burstCount);
    for( int i = 0; i < burstCount; i++ ){
        Trice("i=%x %x\n", 0x44444400 + i, 0xaaaaaa00 + i );
    }
}

//! LogTriceConfiguration shows a few configuration settings.
void LogTriceConfiguration( void ){
    trice("msg:CONFIGURATION == %d: ", CONFIGURATION ); 
    triceS("msg:%s\n", CONFIG_TEXT );
    trice("deb:TRICE_DIRECT_OUTPUT == %d, TRICE_DEFERRED_OUTPUT == %d, ", TRICE_DIRECT_OUTPUT, TRICE_DEFERRED_OUTPUT);
    #if TRICE_BUFFER == TRICE_STACK_BUFFER
    trice("deb:TRICE_STACK_BUFFER\n");
    #elif TRICE_BUFFER == TRICE_STATIC_BUFFER
    trice("deb:TRICE_STATIC_BUFFER\n");
    #elif TRICE_BUFFER == TRICE_DOUBLE_BUFFER
    trice("deb:TRICE_STATIC_BUFFER\n");
    #elif TRICE_BUFFER == TRICE_RING_BUFFER
    trice("deb:TRICE_STATIC_BUFFER\n");
    #endif    
    #if TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE
    trice("deb:TRICE_SINGLE_PACK_MODE, ");
    #else
    trice("deb:TRICE_MULTI_PACK_MODE, ");
    #endif
    trice("deb:TRICE_PROTECT == %d, TRICE_DIAGNOSTICS == %d, XTEA == %d\n", TRICE_PROTECT, TRICE_DIAGNOSTICS, TRICE_DEFERRED_XTEA_ENCRYPT );
}