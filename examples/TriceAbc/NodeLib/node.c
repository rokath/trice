// SPDX-License-Identifier: MIT

/*
 * node.c
 *
 * Shared host-side runtime for the TriceAbc demo nodes.
 *
 * This file intentionally stays demo-specific. It is not a generic transport
 * abstraction and not a generic Trice receiver. Its job is to make the current
 * `triceRx` API, the generated ABC table, the generated TIL-C output, and the
 * protocol-neutral `BcSim` transport visible together with small and readable
 * code.
 */

#include "node.h"

#include "cobs.h"

#if TRICE_RX_ABC_SUPPORT == 1
#include "nodeAbc.h"
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* The demo uses one process per node, so one process-global current node is enough. */
static node_t* gNode;

void nodeSetCurrent(node_t* node) {
    gNode = node;
}

node_t* nodeCurrent(void) {
    return gNode;
}

/* Build a compact [**  *   ] style LED bar. */
static void nodeMakeLedBar(char* dst, size_t dstSize, uint8_t leds) {
    unsigned i;

    if (dstSize < 11u) {
        return;
    }
    dst[0] = '[';
    for (i = 0u; i < 8u; ++i) {
        dst[1u + i] = ((leds >> i) & 1u) ? '*' : ' ';
    }
    dst[9] = ']';
    dst[10] = 0;
}

#if TRICE_RX_SUPPORT == 1

/* Map one packed little/big-endian payload element to an unsigned host value. */
static uint64_t nodeReadUnsigned(const uint8_t* p, uint8_t widthBytes) {
    uint64_t v = 0u;
    unsigned i;

#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
    for (i = 0u; i < widthBytes; ++i) {
        v = (v << 8) | (uint64_t)p[i];
    }
#else
    for (i = 0u; i < widthBytes; ++i) {
        v |= (uint64_t)p[i] << (8u * i);
    }
#endif
    return v;
}

/* Map one 32-bit payload value back to the float bit pattern used by aFloat(). */
static float nodeReadFloat32(const uint8_t* p) {
    union {
        uint32_t u32;
        float f32;
    } x;
    x.u32 = (uint32_t)nodeReadUnsigned(p, 4u);
    return x.f32;
}

/* Print bytes in a compact demo-oriented hexadecimal style. */
static void nodePrintHexBytes(const uint8_t* p, size_t n) {
    size_t i;
    for (i = 0u; i < n; ++i) {
        printf("%s%02x", (i == 0u) ? "" : " ", (unsigned)p[i]);
    }
}

#endif /* TRICE_RX_SUPPORT == 1 */

void nodeSetLeds(node_t* node, uint8_t mask) {
    char bar[11];

    if (node == 0) {
        return;
    }
    node->leds = mask;
    nodeMakeLedBar(bar, sizeof(bar), node->leds);
    printf("%s: leds=%s\n", node->name, bar);
    fflush(stdout);
}

void nodeSetKey(node_t* node, const uint8_t* data, size_t len) {
    size_t i;
    size_t n;

    if (node == 0) {
        return;
    }
    n = len;
    if (n > NODE_KEY_MAX) {
        n = NODE_KEY_MAX;
    }
    for (i = 0u; i < n; ++i) {
        /* Keep control bytes visible but compact in the local state print. */
        node->key[i] = (char)(isprint((unsigned char)data[i]) ? data[i] : '.');
    }
    node->key[n] = 0;
    node->keyLen = (uint8_t)n;
}

void nodePrintLeds(const node_t* node, const char* reason) {
    char bar[11];

    if (node == 0) {
        return;
    }
    nodeMakeLedBar(bar, sizeof(bar), node->leds);
    printf("%s: %s%s\n", node->name, (reason != 0) ? reason : "", bar);
    fflush(stdout);
}

void nodePrintState(const node_t* node, const char* reason) {
    char bar[11];

    if (node == 0) {
        return;
    }
    nodeMakeLedBar(bar, sizeof(bar), node->leds);
    printf("%s: %skey=%s leds=%s\n",
           node->name,
           (reason != 0) ? reason : "",
           node->key,
           bar);
    fflush(stdout);
}

int nodeOpen(node_t* node,
             const char* name,
             int canSend,
             int canReceive,
             int rxLogEnabled) {
    int e;

    if (node == 0 || name == 0) {
        return BCSIM_ERR_ARG;
    }
    memset(node, 0, sizeof(*node));
    node->name = name;
    node->canSend = (uint8_t)(canSend != 0);
    node->canReceive = (uint8_t)(canReceive != 0);
    node->rxLogEnabled = (uint8_t)(rxLogEnabled != 0);

    e = bcSimOpen(&node->bus, "abc.bus", "abc.log", name);
    if (e < 0) {
        return e;
    }
    nodeSetCurrent(node);
    printf("%s: joined abc.bus\n", node->name);
    fflush(stdout);
    return BCSIM_OK;
}

void nodeClose(node_t* node) {
    if (node == 0) {
        return;
    }
    bcSimClose(&node->bus);
    if (gNode == node) {
        gNode = 0;
    }
}

#if TRICE_TX_SUPPORT == 1

void nodeSendLedsState(node_t* node) {
    if (node == 0 || node->canSend == 0u) {
        return;
    }
    trice8C("abc:LedsState", &node->leds, 1);
}

void nodeSendDivideResult(node_t* node, float value) {
    uint32_t bits = aFloat(value);

    if (node == 0 || node->canSend == 0u) {
        return;
    }
    trice32C("abc:DivideResult", &bits, 1);
}

/* Auxiliary direct output is the host bridge for every send-capable demo node. */
void TriceNonBlockingDirectWrite8Auxiliary(const uint8_t* enc, size_t encLen) {
    if (gNode == 0 || gNode->canSend == 0u) {
        return;
    }
    (void)bcSimWrite(&gNode->bus, enc, encLen, "trice");
}

/* The 32-bit auxiliary path is intentionally unused in this demo build. */
void TriceNonBlockingDirectWrite32Auxiliary(const uint32_t* enc, unsigned count) {
    (void)enc;
    (void)count;
}

#endif /* TRICE_TX_SUPPORT == 1 */

#if TRICE_RX_SUPPORT == 1

#if TRICE_RX_LOG_SUPPORT == 1

/* The generated TIL strings can contain "\n" text or a literal newline. */
static int nodeFmtEquals(const char* a, const char* b0, const char* b1) {
    return (a != 0 && ((b0 != 0 && strcmp(a, b0) == 0) || (b1 != 0 && strcmp(a, b1) == 0)));
}

/* The demo log printer intentionally recognizes only the message shapes used below. */
static void nodePrintResolvedLog(const node_t* node, const triceRx_t* rx) {
    if (node == 0 || rx == 0 || rx->pFmt == 0) {
        return;
    }

    if (nodeFmtEquals(rx->pFmt, "log:tick=%u\\n", "log:tick=%u\n") && rx->payloadBytes == 4u) {
        printf("%s: log:tick=%u\n",
               node->name,
               (unsigned)nodeReadUnsigned(rx->payload, 4u));
    } else if (nodeFmtEquals(rx->pFmt, "log:from=%u phase=%u\\n", "log:from=%u phase=%u\n") &&
               rx->payloadBytes == 8u) {
        printf("%s: log:from=%u phase=%u\n",
               node->name,
               (unsigned)nodeReadUnsigned(rx->payload + 0u, 4u),
               (unsigned)nodeReadUnsigned(rx->payload + 4u, 4u));
    } else if (nodeFmtEquals(rx->pFmt, "log:text=%s\\n", "log:text=%s\n")) {
        char text[NODE_KEY_MAX + 1u];
        size_t n = rx->payloadBytes;
        if (n > NODE_KEY_MAX) {
            n = NODE_KEY_MAX;
        }
        memcpy(text, rx->payload, n);
        text[n] = 0;
        printf("%s: log:text=%s\n", node->name, text);
    } else {
        printf("%s: log:fmt=\"%s\" payload=", node->name, rx->pFmt);
        nodePrintHexBytes(rx->payload, rx->payloadBytes);
        printf("\n");
    }
    fflush(stdout);
}

#endif /* TRICE_RX_LOG_SUPPORT == 1 */

/* Selector-0 frames are printed separately because they intentionally carry no ID. */
static void nodePrintX0(const node_t* node, const triceRx_t* rx) {
    printf("%s: x0 %u bytes: ", node->name, (unsigned)rx->payloadBytes);
    if (rx->payloadBytes != 0u && rx->payload != 0) {
        nodePrintHexBytes(rx->payload, rx->payloadBytes);
    }
    printf("\n");
    fflush(stdout);
}

/* Parse, classify, and dispatch one fully decoded Trice record. */
static void nodeHandleRecord(node_t* node, const uint8_t* record, size_t len) {
    triceRx_t rx;
    int used;

    used = triceParseNextRecord(&rx, record, len);
    if (used <= 0) {
        printf("%s: rx parse error=%d\n", node->name, used);
        fflush(stdout);
        return;
    }

#if TRICE_RX_ABC_SUPPORT == 1
    if (triceResolveAbc(&rx, triceAbc, (size_t)triceAbcElements) == TRICE_RX_OK) {
        int e = triceDispatchAbc(&rx);
        if (e != TRICE_RX_OK) {
            printf("%s: abc dispatch error=%d id=%u\n", node->name, e, (unsigned)rx.id);
            fflush(stdout);
        }
        return;
    }
#endif

#if TRICE_RX_LOG_SUPPORT == 1
    if (node->rxLogEnabled != 0u &&
        triceResolveLog(&rx, triceLog, (size_t)triceLogElements) == TRICE_RX_OK &&
        triceDispatchLog(&rx) == TRICE_RX_E_UNSUPPORTED) {
        nodePrintResolvedLog(node, &rx);
        return;
    }
#endif

    if (rx.stampBits == TRICE_STAMP_BITS_UNKNOWN) {
        nodePrintX0(node, &rx);
        return;
    }

    printf("%s: ignored id=%u payloadBytes=%u\n",
           node->name,
           (unsigned)rx.id,
           (unsigned)rx.payloadBytes);
    fflush(stdout);
}

/* Decode one complete COBS frame and feed exactly one Trice record into the classifier. */
static void nodeFlushFrame(node_t* node) {
    size_t decodedLen;

    if (node->streamLen == 0u) {
        return;
    }
    decodedLen = COBSDecode(node->frame, node->stream, node->streamLen);
    nodeHandleRecord(node, node->frame, decodedLen);
    node->streamLen = 0u;
}

/* Append one raw bus byte to the current receive frame or end the frame on delimiter 0. */
static void nodePushBusByte(node_t* node, uint8_t b) {
    if (b == 0u) {
        nodeFlushFrame(node);
        return;
    }
    if (node->streamLen >= sizeof(node->stream)) {
        printf("%s: rx frame overflow, dropping partial frame\n", node->name);
        fflush(stdout);
        node->streamLen = 0u;
        return;
    }
    node->stream[node->streamLen++] = b;
}

int nodePoll(node_t* node) {
    uint8_t chunk[128];

    if (node == 0 || node->canReceive == 0u) {
        return 0;
    }
    for (;;) {
        int n = bcSimRead(&node->bus, chunk, sizeof(chunk), "poll");
        int i;

        if (n <= 0) {
            return n;
        }
        for (i = 0; i < n; ++i) {
            nodePushBusByte(node, chunk[i]);
        }
    }
}

#if TRICE_RX_ABC_SUPPORT == 1

uint8_t nodePayloadU8(const triceRx_t* rx, size_t index) {
    if (rx == 0 || rx->payload == 0 || index >= rx->payloadBytes) {
        return 0u;
    }
    return rx->payload[index];
}

float nodePayloadFloat32(const triceRx_t* rx, size_t index) {
    size_t offset = index * 4u;

    if (rx == 0 || rx->payload == 0 || offset + 4u > rx->payloadBytes) {
        return 0.0f;
    }
    return nodeReadFloat32(rx->payload + offset);
}

void nodeHandleSetLeds(const triceRx_t* rx) {
    node_t* node = nodeCurrent();
    if (node == 0) {
        return;
    }
    nodeSetLeds(node, nodePayloadU8(rx, 0u));
}

void nodeHandleGetLeds(const triceRx_t* rx, int answer) {
    node_t* node = nodeCurrent();
    (void)rx;
    if (node == 0) {
        return;
    }
#if TRICE_TX_SUPPORT == 1
    if (answer != 0) {
        nodeSendLedsState(node);
    }
#else
    (void)answer;
#endif
}

void nodeHandleSetKey(const triceRx_t* rx) {
    node_t* node = nodeCurrent();
    if (node == 0) {
        return;
    }
    nodeSetKey(node, rx->payload, rx->payloadBytes);
    nodePrintState(node, "");
}

void nodeHandleLogState(const triceRx_t* rx) {
    node_t* node = nodeCurrent();
    (void)rx;
    if (node == 0) {
        return;
    }
    nodePrintState(node, "");
}

void nodeHandleDivide(const triceRx_t* rx, int answer) {
    node_t* node = nodeCurrent();
    float a;
    float b;

    if (node == 0) {
        return;
    }
    a = nodePayloadFloat32(rx, 0u);
    b = nodePayloadFloat32(rx, 1u);
#if TRICE_TX_SUPPORT == 1
    if (answer != 0 && b != 0.0f) {
        nodeSendDivideResult(node, a / b);
    }
#else
    (void)answer;
    (void)a;
    (void)b;
#endif
}

void nodeHandleLedsState(const triceRx_t* rx) {
    node_t* node = nodeCurrent();
    uint8_t mask;
    char bar[11];

    if (node == 0) {
        return;
    }
    mask = nodePayloadU8(rx, 0u);
    nodeMakeLedBar(bar, sizeof(bar), mask);
    printf("%s: abc:LedsState=%s\n", node->name, bar);
    fflush(stdout);
}

void nodeHandleDivideResult(const triceRx_t* rx) {
    node_t* node = nodeCurrent();
    if (node == 0) {
        return;
    }
    printf("%s: abc:DivideResult=%f\n", node->name, (double)nodePayloadFloat32(rx, 0u));
    fflush(stdout);
}

#endif /* TRICE_RX_ABC_SUPPORT == 1 */

#else

int nodePoll(node_t* node) {
    (void)node;
    return 0;
}

#endif /* TRICE_RX_SUPPORT == 1 */
