// SPDX-License-Identifier: MIT
// Reduced PoC extract following the structure of rokath/trice/src/trice.h.
#ifndef TRICE_H_
#define TRICE_H_

#include <stdint.h>
#include "triceConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

// Upstream Trice uses iD(n) as a parser helper around an explicit ID.
#define iD(n) (n)

int TriceInit(void);
void TriceClose(void);
void trice32fn_0(uint16_t tid);

#include "triceOn.h"

#ifdef __cplusplus
}
#endif

#endif
