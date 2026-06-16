#ifndef BCSIM_H_
#define BCSIM_H_

/*
 * BcSim.h
 *
 * Small file-backed byte-stream transport for Trice broadcast simulations.
 *
 * Several PC applications share one append-only binary file, usually bc.bus.
 * Each application appends bytes to that file and periodically reads bytes that
 * other applications appended. This models a simple broadcast medium without a
 * network stack, hub process, real embedded hardware, or simulator setup.
 *
 * This module is intentionally independent from Trice. It does not know Trice
 * IDs, BC handler tables, COBS/TCOBS framing, XTEA encryption, or packet
 * boundaries. It only transports bytes. The intended stack is:
 *
 *   bcSimRead/bcSimWrite  ->  shared byte stream over bc.bus
 *   optional decode layer     ->  none, COBS, TCOBS, XTEA, ...
 *   Trice decoder / BC RX    ->  the Trice receive runtime
 *
 * Important rule:
 *
 *   bc.bus contains exactly and only the bytes passed to bcSimWrite().
 *
 * No device name, status text, length prefix, timestamp, or other demo metadata
 * is inserted into bc.bus. This keeps the file usable as input for tools that
 * understand the selected Trice byte stream format, for example a Trice FILE
 * input mode. Human-readable diagnostics are written to bc.log instead.
 */

#include "BcSim_config.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Negative return values used by bcSimOpen/bcSimRead/bcSimWrite. */
enum {
    BCSIM_OK = 0,
    BCSIM_ERR_ARG = -1,
    BCSIM_ERR_IO = -2,
    BCSIM_ERR_LOCK = -3,
    BCSIM_ERR_LOCK_TIMEOUT = -4,
    BCSIM_ERR_RANGE_OVERFLOW = -5,
    BCSIM_ERR_PATH_TOO_LONG = -6,
    BCSIM_ERR_NOT_OPEN = -7
};

/* One byte-offset range in bc.bus. 'from' is inclusive, 'to' is exclusive. */
typedef struct {
    uint64_t from;
    uint64_t to;
} bcSimRange_t;

/*
 * State of one device-side view of the shared bus file.
 *
 * The structure is intentionally public and fixed-size. This avoids dynamic
 * allocation and allows advanced demo code to inspect or adjust fields.
 */
typedef struct {
    char busPath[BCSIM_PATH_MAX];
    char logPath[BCSIM_PATH_MAX];
    char lockPath[BCSIM_PATH_MAX];
    char deviceName[BCSIM_NAME_MAX];

    /*
     * Current read position in bc.bus.
     *
     * bcSimOpen() initializes this to the current bus file size. Therefore a
     * newly started device does not replay historical messages by default. It
     * only receives bytes appended after it joined the demo bus.
     *
     * A special replay/monitor application may set readOffset manually after
     * bcSimOpen(). Normal device programs should not casually expose such a
     * mode because historical own messages cannot be recognized after restart.
     */
    uint64_t readOffset;

    /* Own write ranges used by bcSimRead() for offset-based self-echo filtering. */
    bcSimRange_t own[BCSIM_MAX_OWN_RANGES];
    unsigned ownCount;

    int isOpen;
} BcSim_t;

/*
 * bcSimOpen initializes one device-side bus handle.
 *
 * busPath    Path to the shared binary bus file, for example "bc.bus".
 * logPath    Path to the optional human-readable log file, for example
 *            "bc.log". Pass NULL or an empty string to disable text logging.
 * deviceName Name printed into bc.log. Pass NULL or empty for "?".
 *
 * The function creates busPath and logPath if needed. It does not truncate
 * existing files. A demo script should remove bc.bus/bc.log before starting a
 * fresh demonstration.
 *
 * readOffset is initialized to the current size of busPath. This is deliberate:
 * a newly started device joins the live bus and does not process old messages.
 */
int bcSimOpen(BcSim_t* io,
                const char* busPath,
                const char* logPath,
                const char* deviceName);

/*
 * bcSimRead reads newly appended bytes from the shared bus file.
 *
 * The function returns only bytes written by other processes. Bytes previously
 * appended by this same handle through bcSimWrite() are consumed internally
 * and filtered by bus-file offset. This is the self-echo filter.
 *
 * The function does not interpret packet boundaries. It may return any positive
 * number of bytes up to max. The layer above this module must buffer and decode
 * its own framing format, for example raw Trice records, COBS/TCOBS frames, or
 * XTEA-protected frames.
 *
 * With the default configuration BCSIM_READ_USES_LOCK == 0, bcSimRead()
 * does not acquire the write lock. This means it may read bytes while another
 * process is appending. That is intentional because it exercises the same
 * incomplete-chunk handling that a real stream transport requires.
 *
 * status is optional and appears only in bc.log RX lines. It may be NULL or
 * empty. The returned value is the number of bytes copied into p, 0 if no
 * foreign bytes are currently available, or a negative BCSIM_ERR_* value.
 */
int bcSimRead(BcSim_t* io,
                uint8_t* p,
                size_t max,
                const char* status);

/*
 * bcSimWrite appends exactly n bytes to the shared bus file.
 *
 * Writers are serialized with an atomically created lock directory derived from
 * busPath, for example "bc.bus.lock". The function remembers the written
 * [start,start+n) bus-file range for later self-echo filtering and optionally
 * appends a TX line to bc.log.
 *
 * bc.bus receives only p[0..n-1]. The status string and device name are not
 * written to bc.bus; they are used only in bc.log.
 *
 * The function returns n on success, 0 for n == 0, or a negative
 * BCSIM_ERR_* value.
 */
int bcSimWrite(BcSim_t* io,
                 const uint8_t* p,
                 size_t n,
                 const char* status);

/*
 * bcSimClose releases local state.
 *
 * The current implementation opens files only for individual operations, so
 * there may be no persistent file handle to close. The function still belongs
 * in the API because it gives applications a clear lifetime boundary and keeps
 * the interface compatible with possible future implementations.
 */
void bcSimClose(BcSim_t* io);

/* Optional helper for example applications and diagnostics. */
const char* bcSimErrorString(int err);

#ifdef __cplusplus
}
#endif

#endif /* BCSIM_H_ */
