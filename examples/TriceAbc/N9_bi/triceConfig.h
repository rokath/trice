// SPDX-License-Identifier: MIT

#ifndef TRICE_CONFIG_N9_BI_H_
#define TRICE_CONFIG_N9_BI_H_

// triceConfig.h for N9_bi.
//
// This node is bidirectional: it can emit and receive bus commands. In the
// stamp demo it acts as a silent responder and therefore enables transmit and
// ABC receive support, but no normal log receive support.

#define TRICE_TX_SUPPORT 1
#define TRICE_RX_ABC_SUPPORT 1

#include "../triceRxConfig.h"

#define TRICE_CLEAN 1
#define TRICE_CYCLE_COUNTER 0
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_AUXILIARY8 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUT_FRAMING TRICE_BUS_FRAMING

#endif /* TRICE_CONFIG_N9_BI_H_ */
