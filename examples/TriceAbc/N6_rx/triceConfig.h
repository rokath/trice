// SPDX-License-Identifier: MIT

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
// Compile all library sources, but keep the unused logging backend disabled.
// TRICE_OFF affects production of logs, not the receive-side decoder.
#define TRICE_OFF 1
#define TRICE_RX_SUPPORT 1 // ABC, LOG & X0

/* No output-buffer or output-framing macro is needed for a pure receive node. */

#endif /* TRICE_CONFIG_N6_RX_H_ */
