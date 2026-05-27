// SPDX-License-Identifier: MIT

//! \file triceX0.h
//! \brief trice selector X0 package writer

#ifndef TRICEX0_H_
#define TRICEX0_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Write a typeX0 counted user packet.
 *
 * Packet format:
 *
 *     first uint16:
 *         bits 15..14 = 00
 *         bits 13..0  = payload byte count
 *
 *     following bytes:
 *         payload[0 .. len-1]
 *
 * len is limited to the supported X0/buffer range. If len exceeds that range,
 * the implementation truncates the payload and increments the Trice truncation
 * diagnostic counter.
 *
 * The function uses the normal Trice critical-section mechanism and is safe
 * against interleaving with other Trice writes.
 */
void triceX0(const void* buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif // #ifndef TRICEX0_H_
