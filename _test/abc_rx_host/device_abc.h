// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

#ifndef DEVICE_ABC_H_
#define DEVICE_ABC_H_

#include "triceAbcReceive.h"

#ifdef __cplusplus
extern "C" {
#endif

void rx_no_payload(const triceAbcRx_t* rx);
void rx_i16(const triceAbcRx_t* rx);
void rx_i32(const triceAbcRx_t* rx);
void rx_i64(const triceAbcRx_t* rx);
void rx_nested(const triceAbcRx_t* rx);

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_ABC_H_ */
