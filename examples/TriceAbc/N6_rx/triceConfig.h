#ifndef TRICE_CONFIG_N6_RX_H_
#define TRICE_CONFIG_N6_RX_H_

/*
 * triceConfig.h for N6_rx.
 *
 * This node is receive-only: it processes incoming bus commands locally and
 * never writes to the bus. No transmit-stack settings are enabled here.
 *
 * Only node-specific overrides belong here. Bus-wide settings are included from
 * ../triceBusConfig.h. General defaults stay in the normal Trice default config.
 */

#include "../triceBusConfig.h"

/* Always make the clean/non-clean build mode explicit in demo node configs. */
#define TRICE_CLEAN 0

/* This node excludes the normal transmit stack and includes receive support. */
#define TRICE_TRANSMIT_SUPPORT 0
#define TRICE_RECEIVE_SUPPORT  1

/* No output-buffer or output-framing macro is needed for a pure receive node. */

#endif /* TRICE_CONFIG_N6_RX_H_ */
