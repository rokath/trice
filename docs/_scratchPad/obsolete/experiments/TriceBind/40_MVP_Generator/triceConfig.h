// SPDX-License-Identifier: MIT

//! \file triceConfig.h
//! \brief Host configuration for the generated Trice bind compiler PoC.

#ifndef POC_BIND_GENERATOR_TRICE_CONFIG_H_
#define POC_BIND_GENERATOR_TRICE_CONFIG_H_

// Bind keeps an existing clean marker active for normal Trice code generation.
#define TRICE_CLEAN 0

// Each record is emitted synchronously from a small stack buffer.
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DEFERRED_OUTPUT 0
#define TRICE_DIRECT_AUXILIARY8 1

// TCOBS framing allows FILEBUFFER to recover individual records from log.bin.
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_TCOBS

// The single-threaded host example does not need critical-section primitives.
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }

// Target diagnostics and a dynamic cycle counter are unnecessary in this PoC.
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0

#endif // POC_BIND_GENERATOR_TRICE_CONFIG_H_
