#ifndef TRICE_BUS_CONFIG_H_
#define TRICE_BUS_CONFIG_H_

// triceRxConfig.h
//
// Shared bus-wide settings for the host-native Trice ABC demo.
//
// Every node includes this file before its node-local overrides. That keeps
// framing and receive-side stream assumptions identical across the whole bus.

// The demo uses one explicit framing choice for every participant on abc.bus.
#define TRICE_BUS_FRAMING TRICE_FRAMING_COBS
#include "cobs.h"

// Receive-side stream processing must match the transmit-side framing choice.
// #define TRICE_RX_FRAMING TRICE_BUS_FRAMING

//  // The demo shows counted selector-0 traffic on the same bus.
//  //
//  // TX-only nodes may force TRICE_RX_SUPPORT to 0 before they include this file.
//  // In that case the shared receive-only selector-0 support should not be forced
//  // back on.
//  #ifndef TRICE_RX_SUPPORT
//  #define TRICE_RX_X0_COUNTED_BUFFER_SUPPORT 1
//  #endif

///#include "triceRxDefaultConfig.h"

#endif // TRICE_BUS_CONFIG_H_
