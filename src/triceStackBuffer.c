// SPDX-License-Identifier: MIT

#include "trice.h"

#if TRICE_BUFFER == TRICE_STACK_BUFFER && TRICE_OFF == 0

void TriceTransfer(void) {}

uint32_t* triceSingleBufferStartWritePosition = (void*)0;

#endif // #if TRICE_BUFFER == TRICE_STACK_BUFFER && TRICE_OFF == 0
