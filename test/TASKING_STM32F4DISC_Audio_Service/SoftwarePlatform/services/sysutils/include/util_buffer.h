/* ------------------------------------------------------------
**
**  Copyright (c) 2013-2015 Altium Limited
**
**  This software is the proprietary, copyrighted property of
**  Altium Ltd. All Right Reserved.
**
**  SVN revision information:
**  $Rev: 14907 $:
**  $Date: 2015-01-19 13:30:51 +0100 (Mon, 19 Jan 2015) $:
**
**  Buffer convenience functions.
**
** ------------------------------------------------------------
*/

/*
 * how ring buffers work:
 * - write to head, read from tail, wrap if pointer reaches the end
 * - head == tail : buffer empty
 * - head + 1 == tail : buffer full
 *   NOTE: this means we can only store size - 1 bytes,
 *         because otherwise we can't distinguish between full and empty buffers
 * free/used space calculations:
 * - available = head - tail ; if (available < 0) available += size;
 * - free = tail - head - 1 ; if (free < 0) free += size;
 */

static inline int buffer_get_next(int pointer, int size)
{
    int result = pointer + 1;
    if (result == size) result = 0; // wrap
    return result;
}

static inline int buffer_get_available(int head, int tail, int size)
{
    int result = head - tail;
    if (result < 0) result += size;
    return result;
}

static inline int buffer_get_free(int head, int tail, int size)
{
    int result = tail - head - 1;
    if (result < 0) result += size;
    return result;
}

