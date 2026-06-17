#include "triceParseNextRecord.h"

/*
 * Default transfer order for the prepared demo parser.
 *
 * A node-specific or bus-wide config may override this before compiling the
 * parser. The byte reads below are explicit and alignment-safe for both MCU byte
 * orders; this macro selects only the order used in the byte stream.
 */
#ifndef TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN
#define TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN 0
#endif

/*
 * Prepared support switch for record variants that duplicate 16-bit IDs.
 *
 * The initial demo keeps this disabled, but the parser has the offset hook so
 * the special case can be exercised later without changing the public API.
 */
#ifndef TRICE_DOUBLED_16BIT_ID
#define TRICE_DOUBLED_16BIT_ID 0
#endif

/*
 * Read one 16-bit transfer-order value from p.
 *
 * The function assembles the integer byte by byte. It never casts p to a wider
 * integer pointer, so unaligned input buffers are safe.
 */
static uint16_t triceReadU16(const uint8_t* p) {
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
    return (uint16_t)(((uint16_t)p[0] << 8) | (uint16_t)p[1]);
#else
    return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
#endif
}

/* Read one 32-bit transfer-order value from p using alignment-safe byte access. */
static uint32_t triceReadU32(const uint8_t* p) {
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
    return ((uint32_t)p[0] << 24) |
           ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8) |
           (uint32_t)p[3];
#else
    return ((uint32_t)p[0]) |
           ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) |
           ((uint32_t)p[3] << 24);
#endif
}

int triceParseNextRecord(triceRx_t* rx, const uint8_t* buf, size_t len) {
    uint16_t w;
    size_t offset = 2u;

    if (rx == 0 || buf == 0) {
        return TRICE_RX_E_ARG;
    }

    /* Start with known defaults, especially bitWidth == unknown and fn == NULL. */
    triceRxInit(rx);

    /* Need at least first word plus count/length word for the simplest record. */
    if (len < 4u) {
        return TRICE_RX_E_SHORT;
    }

    /* First word contains selector bits in the upper two bits and a 14-bit ID. */
    w = triceReadU16(buf);
    rx->id = (uint16_t)(w & 0x3fffu);

    switch ((uint8_t)(w >> 14)) {
    case 1u: /* no stamp */
        rx->stampBits = 0u;
        break;

    case 2u: /* 16-bit stamp follows the ID word. */
        rx->stampBits = 16u;
#if TRICE_DOUBLED_16BIT_ID == 1
        /* Skip the duplicated ID word before reading the stamp. */
        offset += 2u;
#endif
        if (len < offset + 2u) {
            return TRICE_RX_E_SHORT;
        }
        rx->stamp = (uint32_t)triceReadU16(buf + offset);
        offset += 2u;
        break;

    case 3u: /* 32-bit stamp follows the ID word. */
        rx->stampBits = 32u;
        if (len < offset + 4u) {
            return TRICE_RX_E_SHORT;
        }
        rx->stamp = triceReadU32(buf + offset);
        offset += 4u;
        break;

    default:
        /* Selector zero is not handled by this small prepared parser. */
        return TRICE_RX_E_UNSUPPORTED;
    }

    /* After optional stamp bytes, one count/length word is expected. */
    if (len < offset + 2u) {
        return TRICE_RX_E_SHORT;
    }

    w = triceReadU16(buf + offset);
    offset += 2u;

    /* High bit set means long byte count; otherwise the high byte carries count. */
    rx->payloadBytes = (w & 0x8000u) ? (uint16_t)(w & 0x7fffu) : (uint16_t)(w >> 8);
    if (len < offset + (size_t)rx->payloadBytes) {
        return TRICE_RX_E_SHORT;
    }

    /* Payload is not copied. It points into the caller-owned input buffer. */
    rx->payload = (rx->payloadBytes == 0u) ? 0 : (buf + offset);
    return (int)(offset + (size_t)rx->payloadBytes);
}
