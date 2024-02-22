//! \file triceAuxiliary.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdlib.h>
#include "trice.h"

#if TRICE_DIRECT_AUXILIARY == 1

//! UserNonBlockingDirectWriteAuxiliaryFn can get a user function address for writing to an auxiliary interface.
WriteAuxiliaryFn_t UserNonBlockingDirectWriteAuxiliaryFn = (void*)0;

//! TriceNonBlockingDirectWriteAuxiliary writes to a user defined interface.
void TriceNonBlockingDirectWriteAuxiliary( uint8_t const * enc, size_t encLen ){
    if( UserNonBlockingDirectWriteAuxiliaryFn != (void*)0 ){
        UserNonBlockingDirectWriteAuxiliaryFn( enc, encLen );
    }
}

#endif // #if TRICE_DIRECT_AUXILIARY == 1

#if TRICE_DEFERRED_AUXILIARY == 1

//! UserNonBlockingDeferredWriteAuxiliaryFn can get a user function address for writing to an auxiliary interface.
WriteAuxiliaryFn_t UserNonBlockingDeferredWriteAuxiliaryFn = (void*)0;

//! TriceNonBlockingDeferredWriteAuxiliary writes to a user defined interface.
void TriceNonBlockingDeferredWriteAuxiliary( uint8_t const * enc, size_t encLen ){
    if( UserNonBlockingDeferredWriteAuxiliaryFn != (void*)0 ){
        UserNonBlockingDeferredWriteAuxiliaryFn( enc, encLen );
    }
}

#endif // #if TRICE_DEFERRED_AUXILIARY == 1
