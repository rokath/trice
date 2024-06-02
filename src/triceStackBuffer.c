//! \file triceStackBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_BUFFER == TRICE_STACK_BUFFER

#ifdef TRICE_CGO
void TriceTransfer( void ){}
#endif

#endif // #if TRICE_BUFFER == TRICE_STACK_BUFFER
