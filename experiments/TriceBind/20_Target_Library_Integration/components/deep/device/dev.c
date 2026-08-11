// SPDX-License-Identifier: MIT

//! \file dev.c
//! \brief Second source-level logging example at a deeper path.

#include "dev.h"

#include "trice_dev_c_F4040404040404040.h" // trice-bind

// secondaryDevRun repeats one format and adds a distinct deep-source record.
void secondaryDevRun(int value)
{
    trice("msg:source shared value=%d\n", value);
    trice("msg:source secondary value=%d\n", value);
}
