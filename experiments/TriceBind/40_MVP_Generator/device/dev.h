// SPDX-License-Identifier: MIT

//! \file dev.h
//! \brief Shallow header with bound static-inline logging sites.

#ifndef POC_BIND_GENERATOR_DEVICE_DEV_H_
#define POC_BIND_GENERATOR_DEVICE_DEV_H_

#include "trice.h"
#include "trice_dev_h_KB8F7A5E5F865D5AF.h" // trice-bind: keep as last include before this file's Trice calls

// primaryDevInline emits one shared and one shallow-header-specific record.
static inline void primaryDevInline(int value)
{
    trice("msg:inline shared value=%d\n", value);
    Trice("msg:inline primary value=%d\n", value);
}

// primaryDevRun emits records from the matching shallow source file.
void primaryDevRun(int value);

#endif // POC_BIND_GENERATOR_DEVICE_DEV_H_
