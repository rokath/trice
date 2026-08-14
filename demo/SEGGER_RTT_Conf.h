// SPDX-License-Identifier: MIT

//! \file SEGGER_RTT_Conf.h
//! \brief Minimal host settings for the optional RTT source compiled by the demos.

#ifndef TRICE_DEMO_SEGGER_RTT_CONF_H_
#define TRICE_DEMO_SEGGER_RTT_CONF_H_

// The demos do not use RTT. These smallest valid table sizes merely allow the
// intentionally simple ../../src/*.c build command to compile SEGGER_RTT.c.
#define SEGGER_RTT_MAX_NUM_UP_BUFFERS 1
#define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS 1

#endif // TRICE_DEMO_SEGGER_RTT_CONF_H_
