#ifndef DEMO_ABC_IN_OUT_CONFIG_H_
#define DEMO_ABC_IN_OUT_CONFIG_H_

/*
 * demoAbcInOut_config.h
 *
 * Configuration for the standalone demoAbcInOut byte-stream transport.
 *
 * This file is intentionally separate from demoAbcInOut.h so the Lib directory
 * can later be copied into a Trice ABC demonstration without pulling in the Chk
 * example program. Keep these values conservative and portable; the module is a
 * demonstration transport, not a high-throughput production IPC layer.
 */

/* Maximum length including the terminating zero for bus, log, and lock paths. */
#ifndef DEMO_ABC_PATH_MAX
#define DEMO_ABC_PATH_MAX 512u
#endif

/* Maximum length including the terminating zero for the device name in abc.log. */
#ifndef DEMO_ABC_NAME_MAX
#define DEMO_ABC_NAME_MAX 64u
#endif

/* Maximum number of remembered self-written bus-file ranges per process. */
#ifndef DEMO_ABC_MAX_OWN_RANGES
#define DEMO_ABC_MAX_OWN_RANGES 128u
#endif

/* Poll interval while waiting for the writer lock directory to disappear. */
#ifndef DEMO_ABC_LOCK_POLL_MS
#define DEMO_ABC_LOCK_POLL_MS 10u
#endif

/* Maximum time spent waiting for the writer lock directory. */
#ifndef DEMO_ABC_LOCK_WAIT_MS
#define DEMO_ABC_LOCK_WAIT_MS 5000u
#endif

/*
 * If 0, demoAbcRead() does not acquire the writer lock. This is the default
 * because upper stream/framing layers should be able to handle incomplete input,
 * as they would have to do for real UART/TCP/file streaming transports.
 *
 * If 1, demoAbcRead() acquires the same lock used by demoAbcWrite(). This makes
 * the check program more deterministic but is less representative of a stream
 * transport where partial data may become visible.
 */
#ifndef DEMO_ABC_READ_USES_LOCK
#define DEMO_ABC_READ_USES_LOCK 0
#endif

#endif /* DEMO_ABC_IN_OUT_CONFIG_H_ */
