#ifndef TRICE_CONFIG_N6_RX_H_
#define TRICE_CONFIG_N6_RX_H_

/*
 * triceConfig.h for N6_rx (receive-only node).
 *
 * This file contains node-specific Trice settings only. Bus-wide settings are
 * included from ../triceBusConfig.h. Unrelated defaults remain in
 * triceDefaultConfig.h.
 */

#include "../triceBusConfig.h"

#define TRICE_CLEAN 0

#define TRICE_TRANSMIT_SUPPORT 0
#define TRICE_RECEIVE_SUPPORT  1

/* This node is prepared for the normal Trice receive stack. */

#endif /* TRICE_CONFIG_N6_RX_H_ */
