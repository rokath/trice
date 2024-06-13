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
void TriceNonBlockingDirectWriteAuxiliary( const uint8_t * enc, size_t encLen ){
    #if defined(TRICE_CGO) // automated tests
        TriceWriteDeviceCgo( enc, encLen );
    #else
        if( UserNonBlockingDirectWriteAuxiliaryFn != (void*)0 ){
            UserNonBlockingDirectWriteAuxiliaryFn( enc, encLen );
        }
    #endif
}

#endif // #if TRICE_DIRECT_AUXILIARY == 1

#if TRICE_DEFERRED_AUXILIARY == 1

//! UserNonBlockingDeferredWriteAuxiliaryFn can get a user function address for writing to an auxiliary interface.
WriteAuxiliaryFn_t UserNonBlockingDeferredWriteAuxiliaryFn = (void*)0;

//! TriceNonBlockingDeferredWriteAuxiliary writes to a user defined interface.
void TriceNonBlockingDeferredWriteAuxiliary( const uint8_t * enc, size_t encLen ){
    #if defined(TRICE_CGO) // automated tests
        TriceWriteDeviceCgo( enc, encLen );
    #else
        if( UserNonBlockingDeferredWriteAuxiliaryFn != (void*)0 ){
            UserNonBlockingDeferredWriteAuxiliaryFn( enc, encLen );
        }
    #endif
}

#endif // #if TRICE_DEFERRED_AUXILIARY == 1
