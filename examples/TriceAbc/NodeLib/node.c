// SPDX-License-Identifier: MIT

#if !defined(_WIN32)
#define _POSIX_C_SOURCE 199309L
#endif

// node.c
//
// Shared host-side runtime for the TriceAbc demo nodes.
//
// This file intentionally stays demo-specific. It is not a generic transport
// abstraction and not a generic Trice receiver. Its job is to make the current
// `triceRx` API, the generated ABC table, the generated TIL-C output, and the
// protocol-neutral `BcSim` transport visible together with small and readable
// code.

#include "node.h"

#include "cobs.h"

#if TRICE_RX_ABC_SUPPORT == 1
#include "nodeAbc.h"
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#include <direct.h>
#include <windows.h>
#define NODE_MKDIR(path) _mkdir(path)
#define NODE_RMDIR(path) _rmdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define NODE_MKDIR(path) mkdir(path, 0777)
#define NODE_RMDIR(path) rmdir(path)
#endif

// The demo uses one process per node, so one process-global current node is enough.
static node_t* gNode;

// Keep one full console line together even when several demo processes print.
#define NODE_CONSOLE_LOCK_PATH "abc.console.lock"
#define NODE_CONSOLE_LINE_MAX 512u
#define NODE_CONSOLE_LOCK_POLL_MS 10u

// Sleep briefly while another demo process owns the console lock.
void nodeSleepMs(unsigned ms) {
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000u);
    ts.tv_nsec = (long)((ms % 1000u) * 1000000u);
    (void)nanosleep(&ts, 0);
#endif
}

// Serialize terminal writes across all node processes.
//
// Trice critical sections are process-local and protect target-side shared
// state only. The demo needs an additional OS-visible lock because several
// independent processes print to one terminal at the same time.
static int nodeConsoleLock(void) {
    for (;;) {
        if (NODE_MKDIR(NODE_CONSOLE_LOCK_PATH) == 0) {
            return 0;
        }
        if (errno != EEXIST) {
            // Never fall back to unlocked terminal output.
            //
            // Rare lock races are acceptable in a demo, but mixed lines are
            // not. Retrying preserves readable output even if one lock attempt
            // sees a transient filesystem-side error.
            nodeSleepMs(NODE_CONSOLE_LOCK_POLL_MS);
            continue;
        }
        nodeSleepMs(NODE_CONSOLE_LOCK_POLL_MS);
    }
}

// Release the process-shared console lock again after one whole line is printed.
static void nodeConsoleUnlock(void) {
    (void)NODE_RMDIR(NODE_CONSOLE_LOCK_PATH);
}

// Print exactly one already assembled line.
//
// Building the complete text before taking the lock keeps the critical section
// small and avoids interleaving from helper functions that would otherwise call
// printf() several times for one logical output line.
static void nodePrintLine(const char* text) {
    if (text == 0) {
        return;
    }
    nodeConsoleLock();
    fputs(text, stdout);
    fflush(stdout);
    nodeConsoleUnlock();
}

// Format one complete line first and then print it under the console lock.
static void nodePrintLineF(const char* fmt, ...) {
    char line[NODE_CONSOLE_LINE_MAX];
    va_list ap;
    int n;

    if (fmt == 0) {
        return;
    }
    va_start(ap, fmt);
    n = vsnprintf(line, sizeof(line), fmt, ap);
    va_end(ap);
    if (n < 0) {
        return;
    }
    if ((size_t)n >= sizeof(line)) {
        line[sizeof(line) - 2u] = '\n';
        line[sizeof(line) - 1u] = 0;
    }
    nodePrintLine(line);
}

void nodeSetCurrent(node_t* node) {
    gNode = node;
}

node_t* nodeCurrent(void) {
    return gNode;
}

// Build a compact [**  *   ] style LED bar.
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

// Map one packed little/big-endian payload element to an unsigned host value.
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

// Map one 32-bit payload value back to the float bit pattern used by aFloat().
static float nodeReadFloat32(const uint8_t* p) {
    union {
        uint32_t u32;
        float f32;
    } x;
    x.u32 = (uint32_t)nodeReadUnsigned(p, 4u);
    return x.f32;
}

// Format one hexadecimal byte row into the caller supplied line buffer.
static void nodeAppendHexBytes(char* dst, size_t dstSize, size_t* pos, const uint8_t* p, size_t n) {
    size_t i;

    if (dst == 0 || dstSize == 0u || pos == 0) {
        return;
    }
    for (i = 0u; i < n; ++i) {
        int wrote = snprintf(dst + *pos, dstSize - *pos, "%s%02x", (i == 0u) ? "" : " ", (unsigned)p[i]);
        if (wrote < 0) {
            return;
        }
        if ((size_t)wrote >= dstSize - *pos) {
            *pos = dstSize - 1u;
            return;
        }
        *pos += (size_t)wrote;
    }
}

#endif // TRICE_RX_SUPPORT == 1

void nodeSetLeds(node_t* node, uint8_t mask) {
    char bar[11];

    if (node == 0) {
        return;
    }
    node->leds = mask;
    nodeMakeLedBar(bar, sizeof(bar), node->leds);
    nodePrintLineF("%s: leds=%s\n", node->name, bar);
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
        // Keep control bytes visible but compact in the local state print.
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
    nodePrintLineF("%s: %s%s\n", node->name, (reason != 0) ? reason : "", bar);
}

void nodePrintState(const node_t* node, const char* reason) {
    char bar[11];

    if (node == 0) {
        return;
    }
    nodeMakeLedBar(bar, sizeof(bar), node->leds);
    nodePrintLineF("%s: %skey=%s leds=%s\n",
                   node->name,
                   (reason != 0) ? reason : "",
                   node->key,
                   bar);
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
    nodePrintLineF("%s: joined abc.bus\n", node->name);
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

// Auxiliary direct output is the host bridge for every send-capable demo node.
void TriceNonBlockingDirectWrite8Auxiliary(const uint8_t* enc, size_t encLen) {
    if (gNode == 0 || gNode->canSend == 0u) {
        return;
    }
    (void)bcSimWrite(&gNode->bus, enc, encLen, "trice");
}

// The 32-bit auxiliary path is intentionally unused in this demo build.
void TriceNonBlockingDirectWrite32Auxiliary(const uint32_t* enc, unsigned count) {
    (void)enc;
    (void)count;
}

#endif // TRICE_TX_SUPPORT == 1

#if TRICE_RX_SUPPORT == 1

#if TRICE_RX_LOG_SUPPORT == 1

// The generated TIL strings can contain "\n" text or a literal newline.
static int nodeFmtEquals(const char* a, const char* b0, const char* b1) {
    return (a != 0 && ((b0 != 0 && strcmp(a, b0) == 0) || (b1 != 0 && strcmp(a, b1) == 0)));
}

// The demo log printer intentionally recognizes only the message shapes used below.
static void nodePrintResolvedLog(const node_t* node, const triceRx_t* rx) {
    if (node == 0 || rx == 0 || rx->pFmt == 0) {
        return;
    }

    if (nodeFmtEquals(rx->pFmt, "log:tick=%u\\n", "log:tick=%u\n") && rx->payloadBytes == 4u) {
        nodePrintLineF("%s: log:tick=%u\n",
                       node->name,
                       (unsigned)nodeReadUnsigned(rx->payload, 4u));
    } else if (nodeFmtEquals(rx->pFmt, "log:from=%u phase=%u\\n", "log:from=%u phase=%u\n") &&
               rx->payloadBytes == 8u) {
        nodePrintLineF("%s: log:from=%u phase=%u\n",
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
        nodePrintLineF("%s: log:text=%s\n", node->name, text);
    } else {
        char line[NODE_CONSOLE_LINE_MAX];
        size_t pos = 0u;
        int wrote = snprintf(line, sizeof(line), "%s: log:fmt=\"%s\" payload=", node->name, rx->pFmt);
        if (wrote < 0) {
            return;
        }
        if ((size_t)wrote >= sizeof(line)) {
            line[sizeof(line) - 2u] = '\n';
            line[sizeof(line) - 1u] = 0;
            nodePrintLine(line);
            return;
        }
        pos = (size_t)wrote;
        nodeAppendHexBytes(line, sizeof(line), &pos, rx->payload, rx->payloadBytes);
        (void)snprintf(line + pos, sizeof(line) - pos, "\n");
        nodePrintLine(line);
    }
}

#endif // TRICE_RX_LOG_SUPPORT == 1

// Selector-0 frames are printed separately because they intentionally carry no ID.
static void nodePrintX0(const node_t* node, const triceRx_t* rx) {
    char line[NODE_CONSOLE_LINE_MAX];
    size_t pos = 0u;
    int wrote = snprintf(line, sizeof(line), "%s: x0 %u bytes: ", node->name, (unsigned)rx->payloadBytes);

    if (wrote < 0) {
        return;
    }
    if ((size_t)wrote >= sizeof(line)) {
        line[sizeof(line) - 2u] = '\n';
        line[sizeof(line) - 1u] = 0;
        nodePrintLine(line);
        return;
    }
    pos = (size_t)wrote;
    if (rx->payloadBytes != 0u && rx->payload != 0) {
        nodeAppendHexBytes(line, sizeof(line), &pos, rx->payload, rx->payloadBytes);
    }
    (void)snprintf(line + pos, sizeof(line) - pos, "\n");
    nodePrintLine(line);
}

// Parse, classify, and dispatch one fully decoded Trice record.
static int nodeHandleRecord(node_t* node, const uint8_t* record, size_t len) {
    triceRx_t rx;
    int used;

    used = triceParseNextRecord(&rx, record, len);
    if (used <= 0) {
        nodePrintLineF("%s: rx parse error=%d\n", node->name, used);
        return used;
    }

#if TRICE_RX_ABC_SUPPORT == 1
    if (triceResolveAbc(&rx, triceAbc, (size_t)triceAbcElements) == TRICE_RX_OK) {
        int e = triceDispatchAbc(&rx);
        if (e != TRICE_RX_OK) {
            nodePrintLineF("%s: abc dispatch error=%d id=%u\n", node->name, e, (unsigned)rx.id);
        }
        return used;
    }
#endif

#if TRICE_RX_LOG_SUPPORT == 1
    if (node->rxLogEnabled != 0u &&
        triceResolveLog(&rx, triceLog, (size_t)triceLogElements) == TRICE_RX_OK &&
        triceDispatchLog(&rx) == TRICE_RX_E_UNSUPPORTED) {
        nodePrintResolvedLog(node, &rx);
        return used;
    }
#endif

    if (rx.stampBits == TRICE_STAMP_BITS_UNKNOWN) {
        nodePrintX0(node, &rx);
        return used;
    }

    nodePrintLineF("%s: ignored id=%u payloadBytes=%u\n",
                   node->name,
                   (unsigned)rx.id,
                   (unsigned)rx.payloadBytes);
    return used;
}

// Build a short optional stamp suffix for human-readable demo output.
static const char* nodeStampText(const triceRx_t* rx, char* dst, size_t dstSize) {
    if (dst == 0 || dstSize == 0u || rx == 0 || rx->stampBits == 0u || rx->stampBits == TRICE_STAMP_BITS_UNKNOWN) {
        return "";
    }
    if (rx->stampBits == 16u) {
        (void)snprintf(dst, dstSize, " stamp=0x%04x", (unsigned)(rx->stamp & 0xffffu));
        return dst;
    }
    (void)snprintf(dst, dstSize, " stamp=0x%08x", (unsigned)rx->stamp);
    return dst;
}

// The stamp demo uses low-order stamp bits as a responder bitmap.
//
// Unstamped requests keep the original broadcast behavior. Stamped requests are
// answered only by nodes whose `replyStampMask` bit is present in the stamp.
#if TRICE_TX_SUPPORT == 1
static int nodeReplyMatchesStamp(const node_t* node, const triceRx_t* rx) {
    if (node == 0 || rx == 0 || node->canSend == 0u) {
        return 0;
    }
    if (rx->stampBits == 0u) {
        return 1;
    }
    if (rx->stampBits == TRICE_STAMP_BITS_UNKNOWN || node->replyStampMask == 0u) {
        return 0;
    }
    return (rx->stamp & node->replyStampMask) != 0u;
}
#endif

// Advance from one logical record to the next possible record start.
//
// `triceParseNextRecord()` intentionally reports only the logical record size.
// The target-side transport can still append zero padding up to the next
// 32-bit boundary. The demo consumes that padding only when all expected bytes
// are actually zero. Otherwise the following byte is treated as the next record
// start so packed records stay decodable.
static size_t nodeAdvanceAlignedRecord(const uint8_t* decoded, size_t decodedLen, size_t offset, size_t logicalUsed) {
    size_t next = offset + logicalUsed;
    size_t alignedNext = (next + 3u) & ~(size_t)3u;
    size_t i;

    if (alignedNext > decodedLen) {
        return next;
    }
    for (i = next; i < alignedNext; ++i) {
        if (decoded[i] != 0u) {
            return next;
        }
    }
    return alignedNext;
}

// Decode one encoded frame payload and feed all contained Trice records into
// the classifier.
//
// A decoded COBS frame can contain one logical record plus alignment bytes or
// several logical records packed together. The loop therefore parses until the
// frame bytes are exhausted.
static void nodeHandleEncodedFrame(node_t* node, const uint8_t* frame, size_t frameLen) {
    size_t decodedLen;
    size_t offset = 0u;

    if (node == 0 || frame == 0 || frameLen == 0u) {
        return;
    }
    decodedLen = COBSDecode(node->frame, frame, frameLen);
    if (decodedLen == 0u) {
        nodePrintLineF("%s: rx decode error=0\n", node->name);
        return;
    }
    while (offset < decodedLen) {
        int used = nodeHandleRecord(node, node->frame + offset, decodedLen - offset);

        if (used <= 0) {
            return;
        }
        offset = nodeAdvanceAlignedRecord(node->frame, decodedLen, offset, (size_t)used);
    }
}

// Scan the accumulated stream for zero-delimited COBS frames.
//
// `bcSimRead()` may return partial frames or several frames at once. This
// collector therefore uses the local byte buffer as the stream truth, processes
// each complete zero-delimited frame immediately, and keeps only the incomplete
// tail for the next read.
static void nodeProcessStream(node_t* node) {
    size_t frameStart = 0u;
    size_t i;

    if (node == 0) {
        return;
    }
    for (i = 0u; i < node->streamLen; ++i) {
        if (node->stream[i] != 0u) {
            continue;
        }
        if (i > frameStart) {
            nodeHandleEncodedFrame(node, node->stream + frameStart, i - frameStart);
        }
        frameStart = i + 1u;
    }
    if (frameStart != 0u) {
        size_t tailLen = node->streamLen - frameStart;

        if (tailLen != 0u) {
            memmove(node->stream, node->stream + frameStart, tailLen);
        }
        node->streamLen = tailLen;
    }
}

int nodePoll(node_t* node) {
    uint8_t chunk[512];

    if (node == 0 || node->canReceive == 0u) {
        return 0;
    }
    for (;;) {
        int n = bcSimRead(&node->bus, chunk, sizeof(chunk), "poll");

        if (n <= 0) {
            return n;
        }
        if (node->streamLen + (size_t)n > sizeof(node->stream)) {
            nodePrintLineF("%s: rx stream overflow, dropping buffered bytes\n", node->name);
            node->streamLen = 0u;
            if ((size_t)n > sizeof(node->stream)) {
                continue;
            }
        }
        memcpy(node->stream + node->streamLen, chunk, (size_t)n);
        node->streamLen += (size_t)n;
        nodeProcessStream(node);
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

// Send `abc:LedsState` with the same stamp width and stamp value as the request.
#if TRICE_TX_SUPPORT == 1
static void nodeSendLedsStateReply(node_t* node, const triceRx_t* rx) {
    if (node == 0 || rx == 0 || node->canSend == 0u) {
        return;
    }
    if (rx->stampBits == 16u) {
        Trice8C("abc:LedsState", (uint16_t)rx->stamp, &node->leds, 1);
        return;
    }
    if (rx->stampBits == 32u) {
        TRice8C("abc:LedsState", rx->stamp, &node->leds, 1);
        return;
    }
    trice8C("abc:LedsState", &node->leds, 1);
}

// Send `abc:DivideResult` with the same stamp width and stamp value as the request.
static void nodeSendDivideResultReply(node_t* node, const triceRx_t* rx, float value) {
    uint32_t bits = aFloat(value);

    if (node == 0 || rx == 0 || node->canSend == 0u) {
        return;
    }
    if (rx->stampBits == 16u) {
        Trice32C("abc:DivideResult", (uint16_t)rx->stamp, &bits, 1);
        return;
    }
    if (rx->stampBits == 32u) {
        TRice32C("abc:DivideResult", rx->stamp, &bits, 1);
        return;
    }
    trice32C("abc:DivideResult", &bits, 1);
}
#endif

// ABC handlers are implemented here directly under their generated names.
//
// The dispatcher already selects by function name, so another wrapper layer in
// each node `main.c` would add only repetition. Runtime node role flags decide
// whether a handler only updates local state or also answers on the bus.
void setLeds(const triceRx_t* rx) {
    node_t* node = nodeCurrent();
    if (node == 0) {
        return;
    }
    nodeSetLeds(node, nodePayloadU8(rx, 0u));
}

// `getLeds` is broadcast by default and stamped-routed when a stamp is present.
void getLeds(const triceRx_t* rx) {
    node_t* node = nodeCurrent();
    if (node == 0) {
        return;
    }
#if TRICE_TX_SUPPORT == 1
    if (nodeReplyMatchesStamp(node, rx) != 0) {
        nodeSendLedsStateReply(node, rx);
    }
#else
    (void)rx;
#endif
}

// `setKey` stores one counted byte buffer and prints the updated local state.
void setKey(const triceRx_t* rx) {
    node_t* node = nodeCurrent();
    if (node == 0) {
        return;
    }
    nodeSetKey(node, rx->payload, rx->payloadBytes);
    nodePrintState(node, "");
}

// `logState` intentionally triggers only a local host print and no Trice reply.
void logState(const triceRx_t* rx) {
    node_t* node = nodeCurrent();
    (void)rx;
    if (node == 0) {
        return;
    }
    nodePrintState(node, "");
}

// `divide` demonstrates request/response traffic with float payload elements.
//
// Receive-only nodes still consume the command but stop after parsing because
// they cannot answer. Division by zero is ignored so the demo stays compact.
void divide(const triceRx_t* rx) {
    node_t* node = nodeCurrent();

    if (node == 0) {
        return;
    }
#if TRICE_TX_SUPPORT == 1
    if (nodeReplyMatchesStamp(node, rx) != 0) {
        float a = nodePayloadFloat32(rx, 0u);
        float b = nodePayloadFloat32(rx, 1u);

        if (b != 0.0f) {
            nodeSendDivideResultReply(node, rx, a / b);
        }
    }
#else
    (void)rx;
#endif
}

// Response records are printed explicitly so the fan-out stays easy to follow.
void LedsState(const triceRx_t* rx) {
    node_t* node = nodeCurrent();
    uint8_t mask;
    char bar[11];
    char stamp[32];

    if (node == 0) {
        return;
    }
    mask = nodePayloadU8(rx, 0u);
    nodeMakeLedBar(bar, sizeof(bar), mask);
    nodePrintLineF("%s: abc:LedsState%s %s\n", node->name, nodeStampText(rx, stamp, sizeof(stamp)), bar);
}

// Float answers are shown as plain text because readability matters most here.
void DivideResult(const triceRx_t* rx) {
    node_t* node = nodeCurrent();
    char stamp[32];

    if (node == 0) {
        return;
    }
    nodePrintLineF("%s: abc:DivideResult%s value=%f\n",
                   node->name,
                   nodeStampText(rx, stamp, sizeof(stamp)),
                   (double)nodePayloadFloat32(rx, 0u));
}

#endif // TRICE_RX_ABC_SUPPORT == 1

#else

int nodePoll(node_t* node) {
    (void)node;
    return 0;
}

#endif // TRICE_RX_SUPPORT == 1
