#ifndef BCSIM_CONFIG_H_
#define BCSIM_CONFIG_H_

/*
 * BcSim_config.h
 *
 * Compile-time configuration for the standalone BcSim byte-stream transport.
 *
 * This header is intentionally protocol-neutral. It configures only the file
 * based broadcast simulation itself: path storage sizes, diagnostic name sizes,
 * self-echo bookkeeping capacity, and optional read-side locking behavior.
 *
 * Each macro is guarded with #ifndef so that a build system can override values
 * from the compiler command line, for example:
 *
 *     CFLAGS="-DBCSIM_READ_USES_LOCK=1" ./build.sh
 */

/*
 * Maximum number of bytes reserved for a path string, including the terminating
 * zero byte. This storage is used for the binary bus file, the optional text log
 * file, and the derived lock-directory path.
 */
#ifndef BCSIM_PATH_MAX
#define BCSIM_PATH_MAX 512u
#endif

/*
 * Maximum number of bytes reserved for the participant name printed in the
 * human-readable log, including the terminating zero byte.
 */
#ifndef BCSIM_NAME_MAX
#define BCSIM_NAME_MAX 64u
#endif

/*
 * Maximum number of own-write byte ranges remembered by one process handle.
 *
 * BcSim filters self-echo by file offsets. After bcSimWrite() appended bytes to
 * the bus file, the written [from,to) range is stored here. Later bcSimRead()
 * consumes this range internally but does not return those bytes to the caller.
 *
 * A finite table keeps the module malloc-free and deterministic. Very slow
 * readers that write many times without reading may exhaust this table.
 */
#ifndef BCSIM_MAX_OWN_RANGES
#define BCSIM_MAX_OWN_RANGES 128u
#endif

/*
 * Poll interval, in milliseconds, while a writer waits for the lock directory
 * to disappear. A small value keeps the demonstration responsive without busy
 * spinning aggressively.
 */
#ifndef BCSIM_LOCK_POLL_MS
#define BCSIM_LOCK_POLL_MS 10u
#endif

/*
 * Maximum time, in milliseconds, spent waiting for the writer lock directory.
 * A timeout prevents a process from blocking forever if another process was
 * aborted while holding the demonstration lock.
 */
#ifndef BCSIM_LOCK_WAIT_MS
#define BCSIM_LOCK_WAIT_MS 5000u
#endif

/*
 * Select whether bcSimRead() also acquires the writer lock.
 *
 * 0: Default. Readers do not lock. They may observe an incomplete write if the
 *    operating system exposes newly appended bytes before the writer is done.
 *    This is useful for stream-like demonstrations because upper layers must be
 *    able to buffer incomplete input anyway.
 *
 * 1: Readers acquire the same lock as writers. This makes the check program
 *    more deterministic but less representative of streaming transports where
 *    partial input may become visible.
 */
#ifndef BCSIM_READ_USES_LOCK
#define BCSIM_READ_USES_LOCK 0
#endif

#endif /* BCSIM_CONFIG_H_ */
