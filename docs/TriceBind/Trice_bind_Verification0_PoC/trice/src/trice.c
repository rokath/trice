// SPDX-License-Identifier: MIT
// Minimal host-file backend producing an unframed, little-endian Trice record.
#include "trice.h"

#include <stdio.h>

static FILE* triceOutput;

int TriceInit(void) {
    triceOutput = fopen(TRICE_OUTPUT_FILE, "wb");
    return triceOutput == NULL ? -1 : 0;
}

void TriceClose(void) {
    if (triceOutput != NULL) {
        fclose(triceOutput);
        triceOutput = NULL;
    }
}

void trice32fn_0(uint16_t tid) {
    // Equivalent to the current no-stamp/no-value trice32m_0 record:
    // bits 15:14 = 01 (no stamp), bits 13:0 = ID,
    // byte 2 = cycle marker, byte 3 = payload byte count (zero).
    const uint32_t record =
        ((uint32_t)0u << 24) |
        ((uint32_t)TRICE_CYCLE << 16) |
        (uint32_t)(0x4000u | (tid & 0x3FFFu));

    const unsigned char bytes[4] = {
        (unsigned char)(record),
        (unsigned char)(record >> 8),
        (unsigned char)(record >> 16),
        (unsigned char)(record >> 24),
    };

    if (triceOutput != NULL) {
        (void)fwrite(bytes, 1, sizeof bytes, triceOutput);
    }
}
