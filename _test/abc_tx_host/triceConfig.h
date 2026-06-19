// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_CYCLE_COUNTER 0 // No Trice library linked, this way we avoid missing TriceCycle.

extern uint32_t abcTxWords[];
extern uint32_t abcTxLength;

#define TRICE_ENTER do { uint32_t* TriceBufferWritePosition = abcTxWords;
#define TRICE_LEAVE abcTxLength = (uint32_t)((uint8_t*)TriceBufferWritePosition - (uint8_t*)abcTxWords); } while (0);

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
