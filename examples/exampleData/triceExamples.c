/*! \file triceExamples.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"

#if !TRICE_OFF

//! TriceHeadLine emits a decorated name. The name length should be 18 characters.
void TriceHeadLine(char * name) {
	//! This is usable as the very first trice sequence after restart. Adapt it. Use a UTF-8 capable editor like VS-Code or use pure ASCII.
	TriceS("w: Hello! 👋🙂 \n\n        ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨        \n        🎈🎈🎈🎈%s🎈🎈🎈🎈\n        🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃        \n\n\n", name);
}

//! SomeExampleTrices generates a few Trice example logs and a burst of Trices.
void SomeExampleTrices(int burstCount) {
	// TRICE32_0(ID(0), "att:🐁 Speedy Gonzales A  32-bit time stamp\n");
	// TRICE32_0(ID(0), "att:🐁 Speedy Gonzales B  32-bit time stamp\n");
	// TRICE32_0(ID(0), "att:🐁 Speedy Gonzales C  32-bit time stamp\n");
	// TRICE32_0(ID(0), "att:🐁 Speedy Gonzales D  32-bit time stamp\n");
	// TRICE32_0(Id(0), "att:🐁 Speedy Gonzales E  16-bit time stamp\n");
	// TRICE32_0(Id(0), "att:🐁 Speedy Gonzales F  16-bit time stamp\n");
	// TRICE32_0(Id(0), "att:🐁 Speedy Gonzales G  16-bit time stamp\n");
	// TRICE32_0(Id(0), "att:🐁 Speedy Gonzales H  16-bit time stamp\n");
	// TRICE32_0(id(0), "att:🐁 Speedy Gonzales I without time stamp\n");
	// TRICE32_0(id(0), "att:🐁 Speedy Gonzales J without time stamp\n");
	// TRICE32_0(id(0), "att:🐁 Speedy Gonzales K without time stamp\n");
	// TRICE32_0(id(0), "att:🐁 Speedy Gonzales L without time stamp\n");
	TRice("att:🐁 Speedy Gonzales a  32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales b  32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales c  32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales d  32-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales e  16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales f  16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales g  16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales h  16-bit time stamp\n");
	// trice("att:🐁 Speedy Gonzales i without time stamp\n");
	// trice("att:🐁 Speedy Gonzales j without time stamp\n");
	// trice("att:🐁 Speedy Gonzales k without time stamp\n");
	// trice("att:🐁 Speedy Gonzales l without time stamp\n");
	char* aString = "2.71828182845904523536";
	TriceS("rd:%s <- float number as string\n", aString);
	Trice64("msg:%.20f (double with more ciphers than precision)\n", aDouble(2.71828182845904523536));
	Trice("msg:%.20f (float  with more ciphers than precision)\n", aFloat(2.71828182845904523536f));
	Trice("msg:%f (default rounded float)\n", aFloat(2.71828182845904523536f));
	Trice("info:A Buffer:\n");
	Trice8B("msg:%02x \n", aString, strlen(aString));
	Trice32B("msg:%08x  \n", aString, strlen(aString) >> 2);
	Trice16F("att:ARemoteFunctionName", aString, strlen(aString) >> 1);
	trice("info:%d times a 16 byte long Trice messages, which may not be written all if the buffer is too small:\n", burstCount);
	for (int i = 0; i < burstCount; i++) {
		Trice("i=%x %x\n", 0x44444400 + i, 0xaaaaaa00 + i);
	}
}

//! LogTriceConfiguration shows a few configuration settings.
void LogTriceConfiguration(void) {
#ifdef LogConfigInfo
	LogConfigInfo();
#endif
	trice("deb:TRICE_DIRECT_OUTPUT == %d, TRICE_DEFERRED_OUTPUT == %d\n", TRICE_DIRECT_OUTPUT, TRICE_DEFERRED_OUTPUT);
#if TRICE_BUFFER == TRICE_STACK_BUFFER
	trice("deb:TRICE_STACK_BUFFER, ");
#elif TRICE_BUFFER == TRICE_STATIC_BUFFER
	trice("deb:TRICE_STATIC_BUFFER, ");
#elif TRICE_BUFFER == TRICE_DOUBLE_BUFFER
	trice("deb:TRICE_DOUBLE_BUFFER, ");
#elif TRICE_BUFFER == TRICE_RING_BUFFER
	trice("deb:TRICE_RING_BUFFER, ");
#endif
#if TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE
	trice("deb:TRICE_SINGLE_PACK_MODE\n");
#else
	trice("deb:TRICE_MULTI_PACK_MODE\n");
#endif
	trice("deb:_CYCLE == %d, _PROTECT == %d, _DIAG == %d, XTEA == %d\n", TRICE_CYCLE_COUNTER, TRICE_PROTECT, TRICE_DIAGNOSTICS, TRICE_DEFERRED_XTEA_ENCRYPT);
	trice("d:_SINGLE_MAX_SIZE=%d, _BUFFER_SIZE=%d, _DEFERRED_BUFFER_SIZE=%d\n", TRICE_SINGLE_MAX_SIZE, TRICE_BUFFER_SIZE, TRICE_DEFERRED_BUFFER_SIZE);
}

#endif // #if !TRICE_OFF
