#ifndef TRICE_CONFIG_N1_TX_H_
#define TRICE_CONFIG_N1_TX_H_

/*
 * triceConfig.h for N1_tx (transmit-only node).
 *
 * This file contains node-specific Trice settings only. Bus-wide settings are
 * included from ../triceBusConfig.h. Unrelated defaults remain in
 * triceDefaultConfig.h.
 */

#include "../triceBusConfig.h"

#define TRICE_CLEAN 0

#define TRICE_TRANSMIT_SUPPORT 1
#define TRICE_RECEIVE_SUPPORT  0

/* This node uses the normal Trice transmit stack. */
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUT_FRAMING TRICE_BUS_FRAMING

#endif /* TRICE_CONFIG_N1_TX_H_ */
