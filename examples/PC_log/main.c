// SPDX-License-Identifier: MIT

#include <stdio.h>

#include "trice.h"

// This File Key include is normally inserted once by `trice bind`. The build
// script runs bind before compilation and creates the referenced header below
// build/triceIDs; application code contains no numeric Trice IDs.
#include "trice_main_c_K50434C4F4744454D.h" // trice-bind: keep as last include before this file's Trice calls

int main(void) {
	char text[128];
	int length;

	// The standard C library snprintf already has the exact hook contract.
	UserTriceLogPrintfFn = snprintf;

	trice("msg:PC local log is running\n");
	trice("value=%d hex=%08x\n", -42, 0x2a);
	triceS("text:runtime=[%s]\n", "hello from triceS");
	triceS("text:width and precision=[%-12.5s]\n", "abcdefgh");
	trice32("float:single=%+.3f scientific=%.2e\n", aFloat(3.125f), aFloat(-0.03125f));
	trice64("float:double=%.9f compact=%.6g\n", aDouble(3.141592653589793), aDouble(123456.0));

	// A real RTOS application calls the same loop from a low-priority task.
	while ((length = TriceLog(text, sizeof(text))) > 0) {
		(void)fwrite(text, 1u, (size_t)length, stdout);
	}
	if (length < 0) {
		fprintf(stderr, "TriceLog failed: %d\n", length);
		return 1;
	}
	return 0;
}
