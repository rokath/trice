// SPDX-License-Identifier: MIT

//! \file dev.h
//! \brief First header-level static inline logging example.

#ifndef POC_BIND_V2_DEVICE_DEV_H_
#define POC_BIND_V2_DEVICE_DEV_H_

#include "trice.h"

#include "trice_device_dev_h_F1010101010101010.h" // trice-bind

// primaryDevInline emits one shared and one file-specific header log record.
static inline void primaryDevInline(int value)
{
    trice("msg:inline shared value=%d\n", value);
    trice("msg:inline primary value=%d\n", value);
}

// primaryDevRun emits records from the matching source file.
void primaryDevRun(int value);

#endif // POC_BIND_V2_DEVICE_DEV_H_
