//! \file device_abc.h
//! Trice ABC selection file for target device.
//! Generated once; edit this file to select received ABC commands.

#ifndef DEVICE_ABC_H_
#define DEVICE_ABC_H_

#include "triceRx.h"

#ifdef __cplusplus
extern "C" {
#endif

void rx_no_payload(const triceRx_t* rx);
void rx_i8_bulk(const triceRx_t* rx);
void rx_i16(const triceRx_t* rx);
void rx_i32(const triceRx_t* rx);
void rx_i64(const triceRx_t* rx);
void rx_nested(const triceRx_t* rx);

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_ABC_H_ */
