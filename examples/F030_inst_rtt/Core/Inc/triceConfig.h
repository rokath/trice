// SPDX-License-Identifier: MIT

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#include "SEGGER_RTT.h"

// Write each record directly to RTT; this example logs only from main.
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

// Match the Trice consistency checks to the shared SEGGER fallback defaults.
#define TRICE_BUFFER_SIZE_DOWN BUFFER_SIZE_DOWN
#define TRICE_SEGGER_RTT_PRINTF_BUFFER_SIZE SEGGER_RTT_PRINTF_BUFFER_SIZE

#endif /* TRICE_CONFIG_H_ */
