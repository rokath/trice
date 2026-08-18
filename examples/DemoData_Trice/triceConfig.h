// SPDX-License-Identifier: MIT

//! \file triceConfig.h
//! \brief Host-demo configuration for a TCOBS-framed TRice byte stream.

#ifndef TRICE_CONFIG_DEMO_PLOT_DATA_H_
#define TRICE_CONFIG_DEMO_PLOT_DATA_H_

#include <stdint.h>

/*
 * Keep normal TRice code active. This demo already contains a fixed iD(1000)
 * and a matching private til.json, so an insertion step is not required.
 */
#define TRICE_CLEAN 0

/*
 * A stack buffer is the simplest choice for this single-threaded host program.
 * Each TRice call is framed and written immediately through the auxiliary
 * byte-output function implemented in main.c.
 */
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DEFERRED_OUTPUT 0
#define TRICE_DIRECT_AUXILIARY8 1

/*
 * TCOBS provides unambiguous package boundaries. The trice host decoder must
 * use its matching TCOBS package-framing setting.
 */
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_TCOBS

/*
 * The example is single-threaded, so no interrupt or scheduler protection is
 * necessary around a TRice call.
 */
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }

/*
 * The target timestamp counts 10 ms units. At the default 50 Hz sample rate it
 * advances by two ticks per generated record.
 */
extern uint32_t demo_plot_timestamp_10ms;
#define TriceStamp32 demo_plot_timestamp_10ms

/* Diagnostics are unnecessary for this small direct-output demonstration. */
#define TRICE_DIAGNOSTICS 0

#endif /* TRICE_CONFIG_DEMO_PLOT_DATA_H_ */
