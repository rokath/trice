// SPDX-License-Identifier: MIT

//! \file triceConfig.h
//! \brief Small host configuration for immediate Trice output.

#ifndef TRICE_DEMO_DIRECT_CONFIG_H_
#define TRICE_DEMO_DIRECT_CONFIG_H_

// A stack buffer is sufficient because every record is written immediately.
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1

// Enable the byte-output hook. main.c assigns its file writer to the library's
// UserNonBlockingDirectWrite8AuxiliaryFn function pointer.
#define TRICE_DIRECT_AUXILIARY8 1

// TCOBS gives the file decoder an unambiguous boundary for every record.
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_TCOBS

#endif // TRICE_DEMO_DIRECT_CONFIG_H_
