// SPDX-License-Identifier: MIT

//! \file main.c
//! \brief Minimal PC application using immediate Trice output.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "trice.h"
#include "trice_main_c_K6B53773E459BC817.h" // trice-bind: keep as last include before this file's Trice calls

// logFile is open only while the synchronous demo emits its Trice records.
static FILE* logFile;
// logWriteFailed preserves callback failures because the callback returns void.
static bool logWriteFailed;

//! writeLogFile writes each framed record immediately through the configured hook.
static void writeLogFile(const uint8_t* data, size_t length) {
	if (logFile == NULL || logWriteFailed) {
		return;
	}

	if (fwrite(data, 1, length, logFile) != length) {
		logWriteFailed = true;
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

	if (logWriteFailed) {
		fputs("Could not write all Trice records to log.bin.\n", stderr);
		return 1;
	}

	return 0;
}
