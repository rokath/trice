#ifndef TRICE_CONFIG_N3_BI_H_
#define TRICE_CONFIG_N3_BI_H_

/*
 * triceConfig.h for N3_bi.
 *
 * This node is bidirectional: it can emit and receive bus commands. The suffix
 * describes bus access only. It does not define request/response semantics.
 *
 * Only node-specific overrides belong here. Bus-wide settings are included from
 * ../triceRxConfig.h. General defaults stay in the normal Trice default config.
 */

/* This node includes both normal transmit support and receive support. */
#define TRICE_TX_SUPPORT 1
#define TRICE_RX_ABC_SUPPORT 1

#include "../triceRxConfig.h"

/* Always make the clean/non-clean build mode explicit in demo node configs. */
#define TRICE_CLEAN 1

/* The host demo uses the CGO bridge instead of a physical UART or RTT probe. */
#define TRICE_CYCLE_COUNTER 0

/* Direct stack-buffer output is sufficient for the prepared PC demonstration. */
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_AUXILIARY8 1
#define TRICE_BUFFER TRICE_STACK_BUFFER

/* Use the bus-wide framing choice so all nodes write compatible bytes. */
#define TRICE_DIRECT_OUT_FRAMING TRICE_BUS_FRAMING

#endif /* TRICE_CONFIG_N3_BI_H_ */
