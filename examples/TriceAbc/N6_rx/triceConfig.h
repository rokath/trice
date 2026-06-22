#ifndef TRICE_CONFIG_N6_RX_H_
#define TRICE_CONFIG_N6_RX_H_

/*
 * triceConfig.h for N6_rx.
 *
 * This node is receive-only: it processes incoming bus commands locally and
 * never writes to the bus. No transmit-stack settings are enabled here.
 *
 * Only node-specific overrides belong here. Bus-wide settings are included from
 * ../triceRxConfig.h. General defaults stay in the normal Trice default config.
 */

/* This node excludes the normal transmit stack and includes receive support. */
#define TRICE_TX_SUPPORT 0
#define TRICE_RX_ABC_SUPPORT 1
#define TRICE_RX_LOG_SUPPORT 1

#include "../triceRxConfig.h"

/* Always make the clean/non-clean build mode explicit in demo node configs. */
#define TRICE_CLEAN 1

/* No output-buffer or output-framing macro is needed for a pure receive node. */

#endif /* TRICE_CONFIG_N6_RX_H_ */
