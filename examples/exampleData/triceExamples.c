// SPDX-License-Identifier: MIT

//! \file triceExamples.c

#include "trice.h"
#include "trice_sidecar_triceExamples_c_2A55E9986F0B1C24.h" // trice-bind: keep as last include before this file's Trice calls

#if !TRICE_OFF

//! TriceHeadLine emits a decorated name. The name length should be 18 characters.
void TriceHeadLine(char* name) {
	//! This is usable as the very first trice sequence after restart. Adapt it. Use a UTF-8 capable editor like VS-Code or use pure ASCII.
	triceS("w: Hello! 👋🙂 \n\n        ✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨✨        \n        🎈🎈🎈🎈%s🎈🎈🎈🎈\n        🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃🍃        \n\n\n", name);
}

//! SomeExampleTrices generates a few Trice example logs and a burst of Trices.
void SomeExampleTrices(int burstCount) {
	TRICE("att:🐁 Speedy Gonzales 0 TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales 1 TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales 2 TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales 3 TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales 4 TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales 5 TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales 6 TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales 7 TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales 8 TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales 9 TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales A TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales B TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales E TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales F TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales G TRICE(... insert macro (uppercase-only macro) without target stamp\n");
	TRICE("att:🐁 Speedy Gonzales H TRICE(... insert macro (uppercase-only macro) without target stamp\n");

	TRICE(ID(0), "att:🐁 Speedy Gonzales 0 TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales 1 TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales 2 TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales 3 TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales 4 TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales 5 TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales 6 TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales 7 TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales 8 TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales 9 TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales A TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales B TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales E TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales F TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales G TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");
	TRICE(ID(0), "att:🐁 Speedy Gonzales H TRICE(ID(0)... insert macro (uppercase-only macro) with 32-bit time stamp\n");

	TRICE(Id(0), "att:🐁 Speedy Gonzales 0 TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales 1 TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales 2 TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales 3 TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales 4 TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales 5 TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales 6 TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales 7 TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales 8 TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales 9 TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales A TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales B TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales E TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales F TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales G TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");
	TRICE(Id(0), "att:🐁 Speedy Gonzales H TRICE(Id(0)... insert macro (uppercase-only macro) with 16-bit time stamp\n");

	TRice("att:🐁 Speedy Gonzales 0 TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales 1 TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales 2 TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales 3 TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales 4 TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales 5 TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales 6 TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales 7 TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales 8 TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales 9 TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales A TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales B TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales E TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales F TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales G TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");
	TRice("att:🐁 Speedy Gonzales H TRice(... function call macro (mixed-case macro) with 32-bit time stamp\n");

	Trice("att:🐁 Speedy Gonzales 0 TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales 1 TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales 2 TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales 3 TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales 4 TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales 5 TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales 6 TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales 7 TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales 8 TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales 9 TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales A TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales B TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales E TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales F TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales G TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");
	Trice("att:🐁 Speedy Gonzales H TRice(... function call macro (mixed-case macro) with 16-bit time stamp\n");

	char* aString = "2.71828182845904523536";
	TriceS("rd:%s <- float number as string\n", aString);
	Trice64("msg:%.20f (double with more ciphers than precision)\n", aDouble(2.71828182845904523536));
	Trice("msg:%.20f (float  with more ciphers than precision)\n", aFloat(2.71828182845904523536f));
	Trice("msg:%f (default rounded float)\n", aFloat(2.71828182845904523536f));
	Trice("info:A Buffer:\n");
	Trice8B("msg:%02x \n", aString, strlen(aString));
	Trice32B("msg:%08x  \n", aString, strlen(aString) >> 2);
	Trice16F("att:ARemoteFunctionName", aString, strlen(aString) >> 1); // Deprecated legacy RPC example; Trice16C requires an explicit 16-bit stamp.
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
	trice("deb:_SINGLE_MAX_SIZE=%d, _BUFFER_SIZE=%d, _DEFERRED_BUFFER_SIZE=%d\n", TRICE_SINGLE_MAX_SIZE, TRICE_BUFFER_SIZE, TRICE_DEFERRED_BUFFER_SIZE);
}

#endif // #if !TRICE_OFF
