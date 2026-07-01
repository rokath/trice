// SPDX-License-Identifier: MIT
// TRICE_INSERT_OFF - Trice parser exclusion marker
//! \file triceRxDefaultConfig.h
//! \brief Default configuration values for the Trice receive side.

#ifndef TRICE_RX_DEFAULT_CONFIG_H_
#define TRICE_RX_DEFAULT_CONFIG_H_

#if defined TRICE_RX_SUPPORT && TRICE_RX_SUPPORT == 0
#define TRICE_RX_LOG_SUPPORT 0               // The device canot receive Trice log data
#define TRICE_RX_ABC_SUPPORT 0               // Disables Trice ABC command receive.
#define TRICE_RX_X0_COUNTED_BUFFER_SUPPORT 0 // The device cannot handle counted type X0 packages.
#endif

#if defined TRICE_RX_SUPPORT && TRICE_RX_SUPPORT == 1
#define TRICE_RX_LOG_SUPPORT 1               // The device can receive Trice log data
#define TRICE_RX_ABC_SUPPORT 1               // Enables Trice ABC command receive.
#define TRICE_RX_X0_COUNTED_BUFFER_SUPPORT 1 // The device can handle counted type X0 packages.
#endif

// TRICE_RX_ABC_SUPPORT enables generated ABC receive metadata and dispatch.
#ifndef TRICE_RX_ABC_SUPPORT
#define TRICE_RX_ABC_SUPPORT 0
#endif

// TRICE_RX_LOG_SUPPORT enables log metadata fields and resolver declarations.
#ifndef TRICE_RX_LOG_SUPPORT
#define TRICE_RX_LOG_SUPPORT 0
#endif

// TRICE_RX_X0_COUNTED_BUFFER_SUPPORT enables parsing counted selector-0 records.
#ifndef TRICE_RX_X0_COUNTED_BUFFER_SUPPORT
#define TRICE_RX_X0_COUNTED_BUFFER_SUPPORT 0
#endif

#ifndef TRICE_RX_SUPPORT
#define TRICE_RX_SUPPORT (TRICE_RX_ABC_SUPPORT | TRICE_RX_LOG_SUPPORT | TRICE_RX_X0_COUNTED_BUFFER_SUPPORT)
#endif

// TRICE_LOCATION_SUPPORT enables optional source file and line metadata.
#ifndef TRICE_LOCATION_SUPPORT
#define TRICE_LOCATION_SUPPORT 0
#endif

// // TRICE_RX_FRAMING describes the receive-side byte stream framing policy.
// #ifndef TRICE_RX_FRAMING
// #define TRICE_RX_FRAMING TRICE_FRAMING_TCOBS
// #endif

// TRICE_DOUBLED_16BIT_ID skips an extra duplicated ID word before 16-bit stamps.
#ifndef TRICE_DOUBLED_16BIT_ID
#define TRICE_DOUBLED_16BIT_ID 0
#endif

// TRICE_DEFAULT_PARAMETER_BIT_WIDTH is duplicated here for RX-only builds.
// Including triceDefaultConfig.h would also pull in transmit-buffer checks.
#ifndef TRICE_DEFAULT_PARAMETER_BIT_WIDTH
#define TRICE_DEFAULT_PARAMETER_BIT_WIDTH 32
#endif

// TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN selects the byte order used on the wire.
#ifndef TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN
#define TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN 0
#endif

#endif // TRICE_RX_DEFAULT_CONFIG_H_
