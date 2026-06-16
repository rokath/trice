#ifndef BCSIM_CONFIG_H_
#define BCSIM_CONFIG_H_

/*
 * BcSim_config.h
 *
 * Configuration for the standalone BcSim byte-stream transport.
 *
 * This file is intentionally separate from BcSim.h so the BcSim directory
 * can later be copied into a Trice broadcast-command simulation without pulling in the BcSimChk
 * example program. Keep these values conservative and portable; the module is a
 * demonstration transport, not a high-throughput production IPC layer.
 */

/* Maximum length including the terminating zero for bus, log, and lock paths. */
#ifndef BCSIM_PATH_MAX
#define BCSIM_PATH_MAX 512u
#endif

/* Maximum length including the terminating zero for the device name in bc.log. */
#ifndef BCSIM_NAME_MAX
#define BCSIM_NAME_MAX 64u
#endif

/* Maximum number of remembered self-written bus-file ranges per process. */
#ifndef BCSIM_MAX_OWN_RANGES
#define BCSIM_MAX_OWN_RANGES 128u
#endif

/* Poll interval while waiting for the writer lock directory to disappear. */
#ifndef BCSIM_LOCK_POLL_MS
#define BCSIM_LOCK_POLL_MS 10u
#endif

/* Maximum time spent waiting for the writer lock directory. */
#ifndef BCSIM_LOCK_WAIT_MS
#define BCSIM_LOCK_WAIT_MS 5000u
#endif

/*
 * If 0, bcSimRead() does not acquire the writer lock. This is the default
 * because upper stream/framing layers should be able to handle incomplete input,
 * as they would have to do for real UART/TCP/file streaming transports.
 *
 * If 1, bcSimRead() acquires the same lock used by bcSimWrite(). This makes
 * the check program more deterministic but is less representative of a stream
 * transport where partial data may become visible.
 */
#ifndef BCSIM_READ_USES_LOCK
#define BCSIM_READ_USES_LOCK 0
#endif

#endif /* BCSIM_CONFIG_H_ */
