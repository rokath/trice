//! \file triceStaticBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_BUFFER == TRICE_STATIC_BUFFER

//! triceSingleBuffer holds a single trice during direct trice macro execution.
uint32_t triceSingleBuffer[TRICE_BUFFER_SIZE>>2];

//! triceSingleBufferStartWritePosition points to trice data start.
uint32_t* const triceSingleBufferStartWritePosition = &triceSingleBuffer[TRICE_DATA_OFFSET>>2];

#if TRICE_DIAGNOSTICS == 1

//! TriceSingleMaxWordCount is a diagnostics value usable to optimize buffer size.
unsigned TriceSingleMaxWordCount = 0;

#endif

#ifdef TRICE_CGO
void TriceTransfer( void ){}
#endif

#endif // #if TRICE_BUFFER == TRICE_STATIC_BUFFER
