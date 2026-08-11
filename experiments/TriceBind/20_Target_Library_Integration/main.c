// SPDX-License-Identifier: MIT

//! \file main.c
//! \brief Host entry point writing bound Trice records into log.bin.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "components/deep/device/dev.h"
#include "device/dev.h"
#include "trice.h"

#include "trice_main_c_F5050505050505050.h" // trice-bind

// logFile owns the binary output stream while the synchronous demo is active.
static FILE* logFile;
// logWriteFailed remembers callback failures because the Trice hook returns void.
static bool logWriteFailed;

// TriceNonBlockingDirectWrite8Auxiliary appends one framed record to log.bin.
void TriceNonBlockingDirectWrite8Auxiliary(const uint8_t* data, size_t length)
{
    if (logFile == NULL || logWriteFailed) {
        return;
    }
    if (fwrite(data, 1, length, logFile) != length) {
        logWriteFailed = true;
    }
}

// main recreates log.bin, exercises all translation units, and reports I/O errors.
int main(void)
{
    logFile = fopen("log.bin", "wb");
    if (logFile == NULL) {
        perror("log.bin");
        return 1;
    }

    TriceInit();
    trice("msg:main started\n");
    primaryDevInline(11);
    secondaryDevInline(22);
    primaryDevRun(33);
    secondaryDevRun(44);
    trice("msg:main completed value=%d\n", 55);

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
