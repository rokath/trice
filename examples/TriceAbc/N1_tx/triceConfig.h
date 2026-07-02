#ifndef TRICE_CONFIG_N1_TX_H_
#define TRICE_CONFIG_N1_TX_H_

/*
 * triceConfig.h for N1_tx.
 *
 * This node is transmit-only: it can emit bus commands but does not process
 * incoming bus traffic. Therefore no feedback is possible for this node at the
 * node software-stack level.
 *
 * Only node-specific overrides belong here. Bus-wide settings are included from
 * ../triceRxConfig.h. General defaults stay in the normal Trice default config.
 */

/* TX-only nodes override the shared RX defaults before they are derived. */
#define TRICE_RX_SUPPORT 0

#define TRICE_TX_SUPPORT 1

/* Always make the clean/non-clean build mode explicit in demo node configs. */
#define TRICE_CLEAN 1

/* This node uses the normal transmit stack and does not include receive support. */
#define TRICE_TX_SUPPORT 1

/* The host demo uses the CGO bridge instead of a physical UART or RTT probe. */
#define TRICE_CYCLE_COUNTER 0

/* Direct stack-buffer output is sufficient for the prepared PC demonstration. */
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_AUXILIARY8 1
#define TRICE_BUFFER TRICE_STACK_BUFFER

/* Use the bus-wide framing choice so all nodes write compatible bytes. */
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_COBS

#endif /* TRICE_CONFIG_N1_TX_H_ */
