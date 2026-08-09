// SPDX-License-Identifier: MIT
//! \file TargetActivity.c

#include "trice.h"
#include "trice_TargetActivity_c_K0FEB4E60E1DCE1D8.h" // trice-bind: keep as last include before this file's Trice calls

char* TargetActivity(void) {
	uint8_t buf[3] = {0xaa, 0xbb, 0xcc};
	TRice8B("%3x\n", buf, 3);
	TRice8B("%3x\n", buf, 3);
	return "time:feed3322default:  aa bb cc\ntime:feed3322default:  aa bb cc";
}
