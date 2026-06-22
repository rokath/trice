// SPDX-License-Identifier: MIT

#ifndef TRICE_ABC_NODE_H_
#define TRICE_ABC_NODE_H_

/*
 * node.h
 *
 * Shared host-side helper API for the TriceAbc demo nodes.
 *
 * The demo uses one process per node. Therefore one process-global "current
 * node" pointer is acceptable and keeps the generated ABC handler signature
 * simple. The generated handler API only passes a `const triceRx_t*`, so the
 * surrounding application state must come from elsewhere.
 */

#include "../BcSim/BcSim.h"
#include "triceRx.h"

#if TRICE_TX_SUPPORT == 1
#include "trice.h"
#endif

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Small fixed limits keep the demo deterministic and malloc-free. */
#define NODE_STREAM_MAX 512u
#define NODE_FRAME_MAX 512u
#define NODE_KEY_MAX 31u

/* One process-local node state used by the shared demo runtime. */
typedef struct {
    const char* name;     /* Short node label such as "N3_bi". */
    BcSim_t bus;          /* File-backed broadcast medium handle. */
    uint8_t leds;         /* Eight demo LED states packed into one byte. */
    char key[NODE_KEY_MAX + 1u]; /* Stored printable key, always zero-terminated. */
    uint8_t keyLen;       /* Number of valid key bytes before the terminating zero. */
    uint8_t canSend;      /* 1 if this node may emit to the bus. */
    uint8_t canReceive;   /* 1 if this node polls and decodes incoming frames. */
    uint8_t rxLogEnabled; /* 1 if normal Trice log records should be printed. */

#if TRICE_RX_SUPPORT == 1
    uint8_t stream[NODE_STREAM_MAX]; /* Raw framed bytes collected until delimiter 0. */
    size_t streamLen;                /* Number of valid bytes in stream[]. */
    uint8_t frame[NODE_FRAME_MAX];   /* Decoded single-record buffer. */
#endif
} node_t;

/* Install the process-local node that the TX bridge and ABC handlers should use. */
void nodeSetCurrent(node_t* node);

/* Return the process-local node or NULL before nodeSetCurrent() was called. */
node_t* nodeCurrent(void);

/* Open one node on the shared abc.bus / abc.log files. */
int nodeOpen(node_t* node,
             const char* name,
             int canSend,
             int canReceive,
             int rxLogEnabled);

/* Clear the local handle state and release the BcSim view. */
void nodeClose(node_t* node);

/* Poll, frame, decode, and dispatch all currently available foreign traffic. */
int nodePoll(node_t* node);

/* Store one LED bit mask and print the simulated bar immediately. */
void nodeSetLeds(node_t* node, uint8_t mask);

/* Store a key buffer in printable form and keep it zero-terminated. */
void nodeSetKey(node_t* node, const uint8_t* data, size_t len);

/* Print the current key and LED state without sending another Trice. */
void nodePrintState(const node_t* node, const char* reason);

/* Print a short LED-only line. */
void nodePrintLeds(const node_t* node, const char* reason);

#if TRICE_TX_SUPPORT == 1
/* Emit one ABC response carrying the current LED mask. */
void nodeSendLedsState(node_t* node);

/* Emit one ABC response carrying a 32-bit float result value. */
void nodeSendDivideResult(node_t* node, float value);
#endif

#if TRICE_RX_ABC_SUPPORT == 1
/* Read one 8-bit payload element from a validated ABC record. */
uint8_t nodePayloadU8(const triceRx_t* rx, size_t index);

/* Read one 32-bit float payload element from a validated ABC record. */
float nodePayloadFloat32(const triceRx_t* rx, size_t index);

/* Shared demo-side command effects used by the tiny node wrapper functions. */
void nodeHandleSetLeds(const triceRx_t* rx);
void nodeHandleGetLeds(const triceRx_t* rx, int answer);
void nodeHandleSetKey(const triceRx_t* rx);
void nodeHandleLogState(const triceRx_t* rx);
void nodeHandleDivide(const triceRx_t* rx, int answer);
void nodeHandleLedsState(const triceRx_t* rx);
void nodeHandleDivideResult(const triceRx_t* rx);
#endif

#ifdef __cplusplus
}
#endif

#endif /* TRICE_ABC_NODE_H_ */
