// SPDX-License-Identifier: MIT

//! \file dev.c
//! \brief First source-level logging example.

#include "dev.h"

#include "trice_device_dev_c_F2020202020202020.h" // trice-bind

// primaryDevRun demonstrates ordinary function logging in the shallow folder.
void primaryDevRun(int value)
{
    trice("msg:source shared value=%d\n", value);
    trice("msg:source primary value=%d\n", value);
}
