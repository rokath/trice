// SPDX-License-Identifier: MIT

//! \file dev.h
//! \brief Deep header with a second set of bound static-inline sites.

#ifndef POC_BIND_GENERATOR_COMPONENTS_DEEP_DEVICE_DEV_H_
#define POC_BIND_GENERATOR_COMPONENTS_DEEP_DEVICE_DEV_H_

#include "trice.h"
#include "trice_dev_h_KF740E4CA1ED0340D.h" // trice-bind: keep as last include before this file's Trice calls

// secondaryDevInline repeats the shared format and adds a deep-specific record.
static inline void secondaryDevInline(int value) {
	trice("msg:inline shared value=%d\n", value);
	TRice("msg:inline secondary value=%d\n", value);
}

// secondaryDevRun emits records from the matching deep source file.
void secondaryDevRun(int value);

#endif // POC_BIND_GENERATOR_COMPONENTS_DEEP_DEVICE_DEV_H_
