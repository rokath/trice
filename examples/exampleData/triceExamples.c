/*! \file triceExamples.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"

#if !TRICE_OFF

//! TriceHeadLine emits a decorated name. The name length should be 18 characters.
void TriceHeadLine(char* name) {
	//! This is usable as the very first trice sequence after restart. Adapt it. Use a UTF-8 capable editor like VS-Code or use pure ASCII.
	TriceS(iD(16369), "w: Hello! 👋🙂\n\n        ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨        \n        🎈🎈🎈🎈%s🎈🎈🎈🎈\n        🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃        \n\n\n", name);
}

//! SomeExampleTrices generates a few Trice example logs and a burst of Trices.
void SomeExampleTrices(int burstCount) {
	// TRICE32_0(ID(16368), "att:🐁 Speedy Gonzales A  32-bit time stamp\n");
	// TRICE32_0(ID(16367), "att:🐁 Speedy Gonzales B  32-bit time stamp\n");
	// TRICE32_0(ID(16366), "att:🐁 Speedy Gonzales C  32-bit time stamp\n");
	// TRICE32_0(ID(16365), "att:🐁 Speedy Gonzales D  32-bit time stamp\n");
	// TRICE32_0(Id(16364), "att:🐁 Speedy Gonzales E  16-bit time stamp\n");
	// TRICE32_0(Id(16363), "att:🐁 Speedy Gonzales F  16-bit time stamp\n");
	// TRICE32_0(Id(16362), "att:🐁 Speedy Gonzales G  16-bit time stamp\n");
	// TRICE32_0(Id(16361), "att:🐁 Speedy Gonzales H  16-bit time stamp\n");
	// TRICE32_0(id(16360), "att:🐁 Speedy Gonzales I without time stamp\n");
	// TRICE32_0(id(16359), "att:🐁 Speedy Gonzales J without time stamp\n");
	// TRICE32_0(id(16358), "att:🐁 Speedy Gonzales K without time stamp\n");
	// TRICE32_0(id(16357), "att:🐁 Speedy Gonzales L without time stamp\n");
	TRice(iD(16356), "att:🐁 Speedy Gonzales a  32-bit time stamp\n");
	TRice(iD(16355), "att:🐁 Speedy Gonzales b  32-bit time stamp\n");
	TRice(iD(16354), "att:🐁 Speedy Gonzales c  32-bit time stamp\n");
	TRice(iD(16353), "att:🐁 Speedy Gonzales d  32-bit time stamp\n");
	Trice(iD(16352), "att:🐁 Speedy Gonzales e  16-bit time stamp\n");
	Trice(iD(16351), "att:🐁 Speedy Gonzales f  16-bit time stamp\n");
	Trice(iD(16350), "att:🐁 Speedy Gonzales g  16-bit time stamp\n");
	Trice(iD(16349), "att:🐁 Speedy Gonzales h  16-bit time stamp\n");
	// trice(iD(16348), "att:🐁 Speedy Gonzales i without time stamp\n");
	// trice(iD(16347), "att:🐁 Speedy Gonzales j without time stamp\n");
	// trice(iD(16346), "att:🐁 Speedy Gonzales k without time stamp\n");
	// trice(iD(16345), "att:🐁 Speedy Gonzales l without time stamp\n");
	char* aString = "2.71828182845904523536";
	TriceS(iD(16344), "rd:%s <- float number as string\n", aString);
	Trice64(iD(16343), "msg:%.20f (double with more ciphers than precision)\n", aDouble(2.71828182845904523536));
	Trice(iD(16342), "msg:%.20f (float  with more ciphers than precision)\n", aFloat(2.71828182845904523536));
	Trice(iD(16341), "msg:%f (default rounded float)\n", aFloat(2.71828182845904523536));
	Trice(iD(16340), "info:A Buffer:\n");
	Trice8B(iD(16339), "msg:%02x \n", aString, strlen(aString));
	Trice32B(iD(16338), "msg:%08x  \n", aString, strlen(aString) >> 2);
	Trice16F(iD(16337), "att:ARemoteFunctionName", aString, strlen(aString) >> 1);
	trice(iD(16336), "info:%d times a 16 byte long Trice messages, which may not be written all if the buffer is too small:\n", burstCount);
	for (int i = 0; i < burstCount; i++) {
		Trice(iD(16335), "i=%x %x\n", 0x44444400 + i, 0xaaaaaa00 + i);
	}
}

//! LogTriceConfiguration shows a few configuration settings.
void LogTriceConfiguration(void) {
#ifdef LogConfigInfo
	LogConfigInfo();
#endif
	trice(iD(16334), "deb:TRICE_DIRECT_OUTPUT == %d, TRICE_DEFERRED_OUTPUT == %d\n", TRICE_DIRECT_OUTPUT, TRICE_DEFERRED_OUTPUT);
#if TRICE_BUFFER == TRICE_STACK_BUFFER
	trice(iD(16333), "deb:TRICE_STACK_BUFFER, ");
#elif TRICE_BUFFER == TRICE_STATIC_BUFFER
	trice(iD(16332), "deb:TRICE_STATIC_BUFFER, ");
#elif TRICE_BUFFER == TRICE_DOUBLE_BUFFER
	trice(iD(16331), "deb:TRICE_DOUBLE_BUFFER, ");
#elif TRICE_BUFFER == TRICE_RING_BUFFER
	trice(iD(16330), "deb:TRICE_RING_BUFFER, ");
#endif
#if TRICE_DEFERRED_TRANSFER_MODE == TRICE_SINGLE_PACK_MODE
	trice(iD(16329), "deb:TRICE_SINGLE_PACK_MODE\n");
#else
	trice(iD(16328), "deb:TRICE_MULTI_PACK_MODE\n");
#endif
	trice(iD(16327), "deb:_CYCLE == %d, _PROTECT == %d, _DIAG == %d, XTEA == %d\n", TRICE_CYCLE_COUNTER, TRICE_PROTECT, TRICE_DIAGNOSTICS, TRICE_DEFERRED_XTEA_ENCRYPT);
	trice(iD(16326), "d:_SINGLE_MAX_SIZE=%d, _BUFFER_SIZE=%d, _DEFERRED_BUFFER_SIZE=%d\n", TRICE_SINGLE_MAX_SIZE, TRICE_BUFFER_SIZE, TRICE_DEFERRED_BUFFER_SIZE);
}

#endif // #if !TRICE_OFF
