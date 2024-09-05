//! \file triceStaticBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_BUFFER == TRICE_STATIC_BUFFER && TRICE_OFF == 0

//! triceSingleBuffer holds a single trice during direct trice macro execution.
uint32_t triceSingleBuffer[TRICE_BUFFER_SIZE >> 2];

//! triceSingleBufferStartWritePosition points to trice data start.
uint32_t* const triceSingleBufferStartWritePosition = &triceSingleBuffer[TRICE_DATA_OFFSET >> 2];

void TriceTransfer(void) {}

#endif // #if TRICE_BUFFER == TRICE_STATIC_BUFFER && TRICE_OFF == 0
