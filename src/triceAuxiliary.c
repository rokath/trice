//! \file triceAuxiliary.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdlib.h>
#include "trice.h"

#if TRICE_DIRECT_AUXILIARY == 1

void TriceNonBlockingDirectWriteAuxiliary( uint8_t const * enc, size_t encLen ){
    // user code here
}

#endif // #if TRICE_DIRECT_AUXILIARY == 1

#if TRICE_DEFERRED_AUXILIARY == 1

void TriceNonBlockingDeferredWriteAuxiliary( uint8_t const * enc, size_t encLen ){
    // user code here
}

#endif // #if TRICE_DEFERRED_AUXILIARY == 1
