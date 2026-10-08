// SPDX-License-Identifier: MIT

//! \file triceConfig.h
//! \brief Independent host configuration for the live deferred-output demo.

#ifndef TRICE_DEMO_LIVE_CONFIG_H_
#define TRICE_DEMO_LIVE_CONFIG_H_

// Log calls buffer records; the loop in main.c transfers them to the file.
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_OUTPUT 1

// main.c connects its file writer to this deferred-output hook.
#define TRICE_DEFERRED_AUXILIARY8 1

#endif // TRICE_DEMO_LIVE_CONFIG_H_
