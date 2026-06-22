// SPDX-License-Identifier: MIT

//! \file triceX0.c
//! \brief trice X0 selector package implementation

#include "trice.h"
#include "triceX0.h"

#if TRICE_TX_X0_COUNTED_BUFFER_SUPPORT == 1

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
 * The first word therefore contains only the payload length. Because len is
 * limited to 0x3fff, the selector bits are guaranteed to stay 00.
 *
 * This function deliberately does not use TRICE_PUT_BUFFER() after TRICE_PUT16().
 * TRICE_PUT_BUFFER() assumes that the destination pointer is already aligned
 * as expected for the normal Trice record layout. Here, only a 16-bit X0 header
 * is written before the payload, so the total record size must be rounded as
 * align4(2 + len), not as align4(len).
 *
 * Interrupt safety:
 *
 *     TRICE_ENTER / TRICE_LEAVE are used exactly around the buffer modification.
 *     This follows the normal Trice critical-section model and prevents
 *     interleaving with other Trice writes.
 */
void triceX0(const void* buf, uint16_t len) {
    /*
     * Worst-case padding is 3 bytes. The physical buffered record size is:
     *
     *     2 + len + pad
     *
     * where the leading 2 bytes are the X0 header and pad is chosen so that
     * the total size is 32-bit aligned.
     */
    uint16_t limit = TRICE_SINGLE_MAX_SIZE - 5u;

#if TRICE_SINGLE_MAX_SIZE > 0x4004u
    /*
     * X0 counted stores the payload length in only 14 bits. If the configured
     * single-record buffer size is larger than the X0 count range, clamp the
     * logical payload length to the protocol maximum.
     *
     * The check is only needed when TRICE_SINGLE_MAX_SIZE - 5 can exceed
     * 0x3fff, i.e. when TRICE_SINGLE_MAX_SIZE > 0x4004.
     */
    if (limit > 0x3fffu) {
        limit = 0x3fffu;
    }
#endif

    /*
     * Keep the same truncation behavior style as dynamic Trice buffers:
     * emit diagnostics and send a truncated payload instead of overflowing
     * the record buffer or the 14-bit X0 count field.
     */
    if (len > limit) {
        TRICE_DYN_STRG_BUF_TRUNCATE_COUNT_INCREMENT();
        len = limit;
    }

    /*
     * Enter the Trice critical section before touching the shared write
     * position and output buffer.
     */
    TRICE_ENTER

    /*
     * Write the X0 header. Since len <= 0x3fff, the two selector bits remain
     * zero and the lower 14 bits contain the payload byte count.
     */
    TRICE_PUT16(len);

    /*
     * After TRICE_PUT16(), TriceBufferWritePosition points immediately after
     * the 2-byte X0 header. Copy the payload right there.
     *
     * memcpy(..., ..., 0) is intentionally allowed here; avoiding a branch is
     * preferable on this hot, small helper path.
     */
    uint8_t* p = (uint8_t*)TriceBufferWritePosition;
    memcpy(p, buf, len);

    /*
     * Compute the number of padding bytes needed to align the total physical
     * record size to 4 bytes.
     *
     * At this point, 2 bytes have already been written before p. We need:
     *
     *     (2 + len + pad) % 4 == 0
     *
     * This compact form is equivalent to:
     *
     *     pad = (4 - ((2 + len) & 3)) & 3
     */
    unsigned pad = (2u - (unsigned)len) & 3u;

    /*
     * Only clear padding bytes when padding exists. This avoids an unnecessary
     * memset call for the already aligned case.
     */
    if (pad != 0u) {
        memset(p + len, 0, pad);
    }

    /*
     * Advance the shared write position to the next 32-bit aligned record
     * boundary.
     */
    TriceBufferWritePosition = (uint32_t*)(p + len + pad);

    /*
     * Leave the Trice critical section and let the configured Trice backend
     * continue exactly as for normal Trice records.
     */
    TRICE_LEAVE
}

#endif // #if TRICE_TX_X0_COUNTED_BUFFER_SUPPORT == 1
