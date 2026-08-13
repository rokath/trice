// SPDX-License-Identifier: MIT

//! \file main.c
//! \brief Host entry point that recreates log.bin and exercises bound sites.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "components/deep/device/dev.h"
#include "device/dev.h"
#include "trice.h"
#include "trice_main_c_KAA795C46633CFF91.h" // trice-bind: keep as last include before this file's Trice calls

// logFile owns the binary destination during the synchronous demonstration.
static FILE* logFile;
// logWriteFailed preserves callback errors because the target hook returns void.
static bool logWriteFailed;

// TriceNonBlockingDirectWrite8Auxiliary appends one framed record to log.bin.
void TriceNonBlockingDirectWrite8Auxiliary(const uint8_t* data, size_t length) {
	if (logFile == NULL || logWriteFailed) {
		return;
	}
	if (fwrite(data, 1, length, logFile) != length) {
		logWriteFailed = true;
	}
}

// main recreates the log, calls both directory variants, and reports I/O errors.
int main(void) {
	static const uint8_t bytes[] = {0x11u, 0x22u, 0x33u};
	logFile = fopen("log.bin", "wb");
	if (logFile == NULL) {
		perror("log.bin");
		return 1;
	}

	TriceInit();
	trice("msg:main shared value=%d\n", 10);
	TRICE("msg:main uppercase value=%d\n", 20);
	triceS("msg:main string=%s\n", "bound");
	TRICE8_B(id(0), "msg:main byte=%02x\n", bytes, sizeof bytes);
	primaryDevInline(30);
	secondaryDevInline(40);
	primaryDevRun(50);
	secondaryDevRun(60);

	if (fclose(logFile) != 0) {
		perror("log.bin");
		return 1;
	}
	logFile = NULL;
	if (logWriteFailed) {
		fputs("Could not write all Trice records to log.bin\n", stderr);
		return 1;
	}
	return 0;
}
