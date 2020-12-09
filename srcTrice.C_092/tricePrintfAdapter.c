/*! \file tricePrintfAdapter.c
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include <stdarg.h> // va_list
#include <stdio.h>  // vsnprintf

#include "triceConfig.h"

#ifdef TRICE_PRINTF_ADAPTER

void triceRuntimeGeneratedString( int rightBound, const char* s );

//! trice replacement helper for printf() with %s 
//! use only for dynamic generatd strings
//! \param pFmt formatstring
int tricePrintfAdapter( const char* pFmt, ... ){
    va_list va;
    int done;
    static char buffer[TRICE_PRINTF_ADAPTER_BUFFERSIZE];
    
    va_start (va, pFmt );
    done = vsnprintf( buffer, sizeof(buffer), pFmt, va);
    va_end (va);

    triceRuntimeGeneratedString( 0, buffer );
    return done;
}

#endif
