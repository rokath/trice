// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 0
#define TRICE_CYCLE_COUNTER 0
#define TRICE_CONFIG_WARNINGS 0
#define TRICE_MCU_IS_BIG_ENDIAN 0
#define TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN 0
#define TRICE_ABC_TRANSMIT_SUPPORT 1
#define TRICE_ABC_RECEIVE_SUPPORT 0
#define TRICE_LEGACY_RPC_SUPPORT 0

extern uint32_t abcTxWords[];
extern uint32_t abcTxLength;

#define TRICE_ENTER do { uint32_t* TriceBufferWritePosition = abcTxWords;
#define TRICE_LEAVE abcTxLength = (uint32_t)((uint8_t*)TriceBufferWritePosition - (uint8_t*)abcTxWords); } while (0);

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
