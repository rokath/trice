// SPDX-License-Identifier: MIT

//! \file dev.c
//! \brief Shallow source with ordinary bound logging sites.

#include "dev.h"
#include "trice_dev_c_KE257CF4E5C9BFE98.h" // trice-bind: keep as last include before this file's Trice calls

// primaryDevRun emits a shared and a shallow-source-specific record.
void primaryDevRun(int value)
{
    trice("msg:source shared value=%d\n", value);
    trice("msg:source primary value=%d\n", value);
}
