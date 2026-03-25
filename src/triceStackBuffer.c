// SPDX-License-Identifier: MIT

//! \file triceStackBuffer.c
//! \brief trice Stack Buffer implementation.


#include "trice.h"

#if TRICE_BUFFER == TRICE_STACK_BUFFER && TRICE_OFF == 0

//! \brief No-op transfer function for stack-buffer mode.
void TriceTransfer(void) {}

//! \brief Start pointer used by TRICE_PUT write macros in stack-buffer mode.
uint32_t* triceSingleBufferStartWritePosition = (uint32_t*)0;

#endif // #if TRICE_BUFFER == TRICE_STACK_BUFFER && TRICE_OFF == 0
