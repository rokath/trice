// SPDX-License-Identifier: MIT

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

// Write each record directly to RTT; this example logs only from main.
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

#endif /* TRICE_CONFIG_H_ */
