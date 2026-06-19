// SPDX-License-Identifier: MIT
//! \file triceConfig.h

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_LEGACY_RPC_SUPPORT 1
#define TRICE_TX_ABC_SUPPORT 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_CGO 1
#define TRICE_TX_X0_COUNTED_BUFFER_SUPPORT 1
#define TRICE_CYCLE_COUNTER 0

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
