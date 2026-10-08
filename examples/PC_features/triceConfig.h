// SPDX-License-Identifier: MIT

#ifndef TRICE_CONFIG_PC_FEATURES_H_
#define TRICE_CONFIG_PC_FEATURES_H_

#include <stdint.h>

// The PC example sends framed binary records to a file for the normal host decoder.
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DEFERRED_OUTPUT 0
#define TRICE_DIRECT_AUXILIARY8 1
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_TCOBS
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }
#define TRICE_DIAGNOSTICS 0

// Separate clocks make the two target timestamp columns easy to compare.
extern uint16_t pc_sample_phase;
extern uint32_t pc_sample_milliseconds;
#define TriceStamp16 pc_sample_phase
#define TriceStamp32 pc_sample_milliseconds

#endif // TRICE_CONFIG_PC_FEATURES_H_
