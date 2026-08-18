// SPDX-License-Identifier: MIT

//! \file main.c
//! \brief Minimal PC application using deferred Trice output.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "trice.h"
// trice bind adds the following generated include automatically during its
// first run; users do not need to create or maintain the file name.
#include "trice_main_c_K660F15320F4A797D.h" // trice-bind: keep as last include before this file's Trice calls

static FILE* logFile; // logFile receives records only when TriceTransfer drains the ring buffer.
static bool logFail;  // logFail preserves callback failures because the callback returns void.

//! writeLogFile writes each transferred record through the configured hook.
static void writeLogFile(
    const uint8_t* data,
    size_t length) {
	if (logFile == NULL || logFail) {
		return;
	}

	if (fwrite(data, 1, length, logFile) != length) {
		logFail = true;
	}
}

//! main buffers two records and then explicitly transfers both to log.bin.
int main(void) {
	logFile = fopen("log.bin", "wb");
	if (logFile == NULL) {
		perror("log.bin");
		return 1;
	}
	UserNonBlockingDeferredWrite8AuxiliaryFn = writeLogFile;
	TriceInit();
	trice("msg:Hello from deferred mode.\n");
	trice("att:Deferred value=%d.\n", 42);

	// Keep transferring until every buffered record reached the file writer.
	while (TricesCountRingBuffer > 0) {
		TriceTransfer();
	}

	if (fclose(logFile) != 0) {
		perror("log.bin");
		return 1;
	}
	logFile = NULL;

	if (logFail) {
		fputs("Could not write all Trice records to log.bin.\n", stderr);
		return 1;
	}

	return 0;
}
