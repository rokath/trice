// SPDX-License-Identifier: MIT

//! \file main.c
//! \brief Minimal PC application using immediate Trice output.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "trice.h"
// trice bind adds the following generated include automatically during its
// first run; users do not need to create or maintain the file name.
#include "trice_main_c_K6B53773E459BC817.h" // trice-bind: keep as last include before this file's Trice calls

static FILE* logFile; // logFile is open only while the synchronous demo emits its Trice records.
static bool logFail;  // logFail preserves callback failures because the callback returns void.

//! writeLogFile writes each framed record immediately through the configured hook.
static void writeLogFile(const uint8_t* data, size_t length) {
	if (logFile == NULL || logFail) {
		return;
	}

	if (fwrite(data, 1, length, logFile) != length) {
		logFail = true;
	}
}

//! main recreates log.bin and emits two records without deferred buffering.
int main(void) {
	logFile = fopen("log.bin", "wb");
	if (logFile == NULL) {
		perror("log.bin");
		return 1;
	}
	UserNonBlockingDirectWrite8AuxiliaryFn = writeLogFile;
	TriceInit();
	trice("msg:Hello from direct mode.\n");
	trice("att:Direct value=%d.\n", 42);

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
