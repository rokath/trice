// SPDX-License-Identifier: MIT

//! \file main.c
//! \brief Hardware-free live logging: emit one deferred record per second.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Only the one-second pause differs between Windows and POSIX systems.
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "trice.h"
// trice bind manages the generated include name below.
#include "trice_main_c_K47FC081B2DF519B8.h" // trice-bind: keep as last include before this file's Trice calls

// The file stays open while the application runs; TriceTransfer uses this writer.
static FILE* logFile;
// The callback cannot return an error, so main checks this flag after transfer.
static bool writeFailed;

//! writeLogFile appends the transferred binary records to the open capture.
static void writeLogFile(const uint8_t* data, size_t length) {
	if (fwrite(data, 1, length, logFile) != length) {
		writeFailed = true;
	}
}

//! main produces a growing capture for a parallel trice log -p FILE process.
int main(void) {
	logFile = fopen("log.bin", "wb");
	if (logFile == NULL) {
		perror("log.bin");
		return 1;
	}

	UserNonBlockingDeferredWrite8AuxiliaryFn = writeLogFile;
	// This demo's own triceConfig.h selects deferred output.
	TriceInit();
	puts("Live demo running. Stop with Ctrl+C.");

	// An unsigned counter wraps naturally if this demo runs long enough.
	uint32_t counter = 0;
	for (;;) {
		trice("msg:Live counter=%u.\n", counter);
		counter++;

		// The log call only buffers its record. Transfer now writes it to the file;
		// omitting this step would leave the decoder waiting for new bytes.
		while (TricesCountRingBuffer > 0) {
			TriceTransfer();
		}

		// Flush the C file buffer so the other process can read each record now.
		if (fflush(logFile) != 0 || writeFailed) {
			fputs("Could not write the live capture.\n", stderr);
			fclose(logFile);
			return 1;
		}

		// Keep the output readable and avoid a busy loop.
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}
}
