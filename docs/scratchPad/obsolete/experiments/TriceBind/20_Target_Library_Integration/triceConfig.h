// SPDX-License-Identifier: MIT

//! \file triceConfig.h
//! \brief Host configuration for the sidecar binding compiler PoC.

#ifndef TRICE_CONFIG_POC_BIND_V2_H_
#define TRICE_CONFIG_POC_BIND_V2_H_

// Keep this marker explicit so trice insert and trice clean can toggle it.
#define TRICE_CLEAN 1

// Enable ID-free lowercase trice(...) calls backed by generated sidecars.
#define TRICE_BIND 1

// Emit each record synchronously from a small stack buffer.
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DEFERRED_OUTPUT 0
#define TRICE_DIRECT_AUXILIARY8 1

// TCOBS framing lets FILEBUFFER recover record boundaries from log.bin.
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_TCOBS

// The single-threaded host PoC needs no critical-section implementation.
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }

// Disable target diagnostics and use the static no-cycle-counter marker.
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0

#endif // TRICE_CONFIG_POC_BIND_V2_H_
