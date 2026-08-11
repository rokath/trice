// SPDX-License-Identifier: MIT

//! \file dev.c
//! \brief Deep source with ordinary bound logging sites.

#include "dev.h"
#include "trice_dev_c_K8A60DC1F2D42070D.h" // trice-bind: keep as last include before this file's Trice calls

// secondaryDevRun repeats the shared format and adds a deep-specific record.
void secondaryDevRun(int value)
{
    trice("msg:source shared value=%d\n", value);
    trice("msg:source secondary value=%d\n", value);
}
