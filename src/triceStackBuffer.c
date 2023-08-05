//! \file triceStackBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_BUFFER == TRICE_STACK_BUFFER

#if TRICE_DIAGNOSTICS == 1

//! TriceSingleMaxWordCount is a diagnostics value usable to optimize buffer size.
unsigned TriceSingleMaxWordCount = 0;

#endif

#ifdef TRICE_CGO
void TriceTransfer( void ){}
#endif

#endif // #if TRICE_BUFFER == TRICE_STACK_BUFFER
