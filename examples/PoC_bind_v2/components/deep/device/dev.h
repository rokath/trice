// SPDX-License-Identifier: MIT

//! \file dev.h
//! \brief Second header-level static inline logging example at a deeper path.

#ifndef POC_BIND_V2_COMPONENTS_DEEP_DEVICE_DEV_H_
#define POC_BIND_V2_COMPONENTS_DEEP_DEVICE_DEV_H_

#include "trice.h"

#include "trice_dev_h_F3030303030303030.h" // trice-bind

// secondaryDevInline repeats one format and adds a distinct header log record.
static inline void secondaryDevInline(int value)
{
    trice("msg:inline shared value=%d\n", value);
    trice("msg:inline secondary value=%d\n", value);
}

// secondaryDevRun emits records from the matching deep source file.
void secondaryDevRun(int value);

#endif // POC_BIND_V2_COMPONENTS_DEEP_DEVICE_DEV_H_
