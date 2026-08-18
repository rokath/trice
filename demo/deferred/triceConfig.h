// SPDX-License-Identifier: MIT

//! \file triceConfig.h
//! \brief Small host configuration for deferred Trice output.

#ifndef TRICE_DEMO_DEFERRED_CONFIG_H_
#define TRICE_DEMO_DEFERRED_CONFIG_H_

// Trice calls append to a ring buffer; TriceTransfer drains it later.
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_OUTPUT 1

// Enable the byte-output hook. main.c assigns its file writer to the library's
// UserNonBlockingDeferredWrite8AuxiliaryFn function pointer.
#define TRICE_DEFERRED_AUXILIARY8 1

#endif // TRICE_DEMO_DEFERRED_CONFIG_H_
