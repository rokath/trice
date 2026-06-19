// SPDX-License-Identifier: MIT

// TRICE_INSERT_OFF - Trice parser exclusion marker

//! \file triceRxDefaultConfig.h
//! \brief TRICE RX extension. Trice Default Config declarations and macros for receive.
//! \details The Trice library currently focusses on TRICE_TX only and does not reflect that explicit because that is its main task.
//! Going towards Trice ABC RX and thinking in the direction of a C/C++ language-only tlog functionality makes it neccesary to invent additional switches.

#ifndef TRICE_RX_DEFAULT_CONFIG_H_
#define TRICE_RX_DEFAULT_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// TRICE RX extension
//  Cases:
//

#ifndef TRICE_RX_ABC_SUPPORT
#define TRICE_RX_ABC_SUPPORT 0 // The device can receive and handle Trice ABC packages. It enables Trice ABC receive types and direct handler dispatch.
#endif

#ifndef TRICE_RX_LOG_SUPPORT
#define TRICE_RX_LOG_SUPPORT 0 // The device can receive and handle Trice Log packages.
#endif                         // controls `pTrice`, `pFmt` and log resolver/formatter-related code.

#ifndef TRICE_LOCATION_SUPPORT
#define TRICE_LOCATION_SUPPORT 0 // controls `file` and `line` fields and location resolution when TRICE_RX_LOG_SUPPORT==1 
#endif 

#ifndef TRICE_RX_X0_COUNTED_BUFFER_SUPPORT
#define TRICE_RX_X0_COUNTED_BUFFER_SUPPORTT 0 // The device can receive and handle Trice X0 packages.
#endif

// TRICE_TX_SUPPORT is a derived value signaling, that the Trice transmit stack is needed.
// It is currenty unused but planned to be able to exclude the TRICE TX stack for TRICE RX - only devices.
#define TRICE_TX_SUPPORT (TRICE_TX_LOG_SUPPORT | TRICE_TX_ABC_SUPPORT | TRICE_TX_X0_COUNTED_BUFFER_SUPPORT)

// TRICE_RX_SUPPORT is a derived value signaling, that the Trice receive stack is needed.
#define TRICE_RX_SUPPORT (TRICE_RX_LOG_SUPPORT | TRICE_RX_ABC_SUPPORT | TRICE_RX_X0_COUNTED_BUFFER_SUPPORT)

#ifndef TRICE_BIT_WIDTH_UNKNOWN
#define TRICE_BIT_WIDTH_UNKNOWN 0xffu // 0xffu = unresolved. Unknown bitWidth after parsing are signaling typeX0 Trice packages.

#ifndef TRICE_STAMP_BITS_UNKNOWN
#define TRICE_STAMP_BITS_UNKNOWN 0xffu // 0xffu = unresolved. Unknown stamp bits after parsing are signaling typeX0 Trice packages.
#endif

/*
 * Framing mode expected on receive.
 *
 * Individual nodes map this value to their direct or deferred output framing
 * macro. Keeping the framing choice here prevents accidentally building nodes
 * that talk different byte-stream dialects.
 */
#ifndef TRICE_RX_FRAMING
#define TRICE_RX_FRAMING TRICE_FRAMING_TCOBS
#endif

/*
 * Prepared switch for a doubled-16-bit-ID demonstration variant.
 *
 * Usually this is disabled. It is defined here because all bus
 * participants must agree if this special mode is enabled later.
 */
#ifndef TRICE_RX_D16_ENABLE
#define TRICE_RX_D16_ENABLE 0
#endif

//
///////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* TRICE_RX_DEFAULT_CONFIG_H_ */
