#include "triceParseNextRecord.h"

#ifndef TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN
#define TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN 0
#endif

#ifndef TRICE_DOUBLED_16BIT_ID
#define TRICE_DOUBLED_16BIT_ID 0
#endif

static uint16_t triceReadU16(const uint8_t* p) {
#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 1
    return (uint16_t)(((uint16_t)p[0] << 8) | (uint16_t)p[1]);
#else
    return (uint16_t)((uint16_t)p[0] | ((uint16_t)p[1] << 8));
#endif
}

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
    triceRxInit(rx);
    if (len < 4u) {
        return TRICE_RX_E_SHORT;
    }

    w = triceReadU16(buf);
    rx->id = (uint16_t)(w & 0x3fffu);

    switch ((uint8_t)(w >> 14)) {
    case 1u: /* no stamp */
        rx->stampBits = 0u;
        break;

    case 2u: /* 16-bit stamp */
        rx->stampBits = 16u;
#if TRICE_DOUBLED_16BIT_ID == 1
        offset += 2u;
#endif
        if (len < offset + 2u) {
            return TRICE_RX_E_SHORT;
        }
        rx->stamp = (uint32_t)triceReadU16(buf + offset);
        offset += 2u;
        break;

    case 3u: /* 32-bit stamp */
        rx->stampBits = 32u;
        if (len < offset + 4u) {
            return TRICE_RX_E_SHORT;
        }
        rx->stamp = triceReadU32(buf + offset);
        offset += 4u;
        break;

    default:
        return TRICE_RX_E_UNSUPPORTED;
    }

    if (len < offset + 2u) {
        return TRICE_RX_E_SHORT;
    }

    w = triceReadU16(buf + offset);
    offset += 2u;

    rx->payloadBytes = (w & 0x8000u) ? (uint16_t)(w & 0x7fffu) : (uint16_t)(w >> 8);
    if (len < offset + (size_t)rx->payloadBytes) {
        return TRICE_RX_E_SHORT;
    }

    rx->payload = (rx->payloadBytes == 0u) ? 0 : (buf + offset);
    return (int)(offset + (size_t)rx->payloadBytes);
}
