//! \file triceAuxiliary.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"
#include <stdint.h>
#include <stdlib.h>

#if TRICE_DIRECT_AUXILIARY8 == 1 && TRICE_OFF == 0

//! UserNonBlockingDirectWrite8AuxiliaryFn can get a user function address for writing to an auxiliary interface.
Write8AuxiliaryFn_t UserNonBlockingDirectWrite8AuxiliaryFn = (void*)0;

//! TriceNonBlockingDirectWrite8Auxiliary writes to a user defined interface.
void TriceNonBlockingDirectWrite8Auxiliary(const uint8_t* enc, size_t encLen) {
#if TRICE_CGO == 1 // automated tests
	TriceWriteDeviceCgo(enc, encLen);
#else
	if (UserNonBlockingDirectWrite8AuxiliaryFn != (void*)0) {
		UserNonBlockingDirectWrite8AuxiliaryFn(enc, encLen);
	}
#endif
}

#endif // #if TRICE_DIRECT_AUXILIARY8 == 1

#if TRICE_DEFERRED_AUXILIARY8 == 1

//! UserNonBlockingDeferredWrite8AuxiliaryFn can get a user function address for writing to an auxiliary interface.
Write8AuxiliaryFn_t UserNonBlockingDeferredWrite8AuxiliaryFn = (void*)0;

//! TriceNonBlockingDeferredWrite8Auxiliary writes to a user defined interface.
void TriceNonBlockingDeferredWrite8Auxiliary(const uint8_t* enc, size_t encLen) {
#if TRICE_CGO == 1 // automated tests
	TriceWriteDeviceCgo(enc, encLen);
#else
	if (UserNonBlockingDeferredWrite8AuxiliaryFn != (void*)0) {
		UserNonBlockingDeferredWrite8AuxiliaryFn(enc, encLen);
	}
#endif
}

#endif // #if TRICE_DEFERRED_AUXILIARY8 == 1

#if TRICE_DIRECT_AUXILIARY32 == 1

//! UserNonBlockingDirectWrite32AuxiliaryFn can get a user function address for writing to an auxiliary interface.
Write32AuxiliaryFn_t UserNonBlockingDirectWrite32AuxiliaryFn = (void*)0;

//! TriceNonBlockingDirectWrite32Auxiliary writes to a user defined interface.
void TriceNonBlockingDirectWrite32Auxiliary(const uint32_t* enc, unsigned count) {
#if TRICE_CGO == 1 // automated tests
	TriceWriteDeviceCgo(enc, count << 2);
#else
	if (UserNonBlockingDirectWrite32AuxiliaryFn != (void*)0) {
		UserNonBlockingDirectWrite32AuxiliaryFn(enc, count);
	}
#endif
}

#endif // #if TRICE_DIRECT_AUXILIARY32 == 1

#if TRICE_DEFERRED_AUXILIARY32 == 1

//! UserNonBlockingDeferredWrite32AuxiliaryFn can get a user function address for writing to an auxiliary interface.
Write32AuxiliaryFn_t UserNonBlockingDeferredWrite32AuxiliaryFn = (void*)0;

//! TriceNonBlockingDeferredWrite32Auxiliary writes to a user defined interface.
void TriceNonBlockingDeferredWrite32Auxiliary(const uint32_t* enc, unsigned count) {
#if TRICE_CGO == 1 // automated tests
	TriceWriteDeviceCgo(enc, count << 2);
#else
	if (UserNonBlockingDeferredWrite32AuxiliaryFn != (void*)0) {
		UserNonBlockingDeferredWrite32AuxiliaryFn(enc, count);
	}
#endif
}

#endif // #if TRICE_DEFERRED_AUXILIARY32 == 1 && TRICE_OFF == 0
