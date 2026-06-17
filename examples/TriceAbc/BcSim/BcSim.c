#if defined(__APPLE__) && !defined(_DARWIN_C_SOURCE)
/*
 * macOS hides some C/POSIX declarations unless one of the Darwin feature-test
 * macros is enabled before including system headers. This keeps snprintf(),
 * nanosleep(), and related declarations visible for strict C99 builds.
 */
#define _DARWIN_C_SOURCE
#elif !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
/*
 * On POSIX-like systems request POSIX.1-2008 declarations before including
 * headers. Windows uses its own API branches below and must not receive this.
 */
#define _POSIX_C_SOURCE 200809L
#endif

/*
 * BcSim.c
 *
 * File-backed byte-stream transport for local broadcast simulations.
 *
 * The design goal is maximum clarity and minimum demo-specific code in the
 * participant applications. Several PC processes share one append-only binary
 * file, normally named bc.bus. Each process writes byte chunks to the end of the
 * file and polls the file for bytes appended by other processes.
 *
 * This module deliberately does not know about packet formats. It does not
 * frame records, decrypt data, decode message IDs, or call handlers. It only
 * provides a visible byte-stream medium and a small amount of demo convenience:
 *
 *   - append-only writes to bc.bus,
 *   - writer serialization by atomic lock directory,
 *   - self-echo filtering by remembered file-offset ranges,
 *   - optional human-readable TX/RX hex logging to bc.log.
 *
 * The binary bus file remains clean: every byte in bc.bus came from a caller's
 * bcSimWrite() buffer. This is the reason why external tools can inspect the
 * same file as the running demo processes.
 */

#include "BcSim.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#include <direct.h>
#include <windows.h>
/* Platform adapter: create and remove a directory with Windows C runtime calls. */
#define BCSIM_MKDIR(path) _mkdir(path)
#define BCSIM_RMDIR(path) _rmdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/* Platform adapter: create and remove a directory with POSIX calls. */
#define BCSIM_MKDIR(path) mkdir(path, 0777)
#define BCSIM_RMDIR(path) rmdir(path)
#endif

/* Sleep helper used while waiting for the writer lock directory. */
static void bcSimSleepMs(unsigned ms) {
#if defined(_WIN32)
    /* Sleep() takes milliseconds directly. */
    Sleep((DWORD)ms);
#else
    /* nanosleep() takes seconds plus nanoseconds. */
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000u);
    ts.tv_nsec = (long)((ms % 1000u) * 1000000u);
    (void)nanosleep(&ts, 0);
#endif
}

/*
 * Copy a user supplied string into a fixed-size field.
 *
 * The function rejects truncation instead of silently cutting paths or names.
 * This makes path mistakes visible during demo setup and keeps all stored
 * strings guaranteed zero-terminated.
 */
static int bcSimCopyString(char* dst, size_t dstSize, const char* src) {
    size_t n;
    if (dst == 0 || dstSize == 0u) {
        return BCSIM_ERR_ARG;
    }
    if (src == 0) {
        src = "";
    }
    n = strlen(src);
    if (n >= dstSize) {
        return BCSIM_ERR_PATH_TOO_LONG;
    }
    memcpy(dst, src, n + 1u);
    return BCSIM_OK;
}

/*
 * Derive the lock-directory name from the bus-file name.
 *
 * Example:
 *
 *     bc.bus  ->  bc.bus.lock
 *
 * Keeping the lock path derived from the bus path avoids an additional public
 * API parameter and ensures that independent bus files also use independent
 * locks.
 */
static int bcSimMakeLockPath(char* dst, size_t dstSize, const char* busPath) {
    int n;
    if (dst == 0 || dstSize == 0u || busPath == 0 || busPath[0] == 0) {
        return BCSIM_ERR_ARG;
    }
    n = snprintf(dst, dstSize, "%s.lock", busPath);
    if (n < 0 || (size_t)n >= dstSize) {
        return BCSIM_ERR_PATH_TOO_LONG;
    }
    return BCSIM_OK;
}

/*
 * Return the current file size.
 *
 * Missing files are treated as size zero. That lets bcSimOpen() and bcSimRead()
 * work naturally before the first writer has created the bus file.
 */
static int bcSimFileSize(const char* path, uint64_t* size) {
    FILE* f;
    long pos;

    if (path == 0 || path[0] == 0 || size == 0) {
        return BCSIM_ERR_ARG;
    }

    f = fopen(path, "rb");
    if (f == 0) {
        if (errno == ENOENT) {
            *size = 0u;
            return BCSIM_OK;
        }
        return BCSIM_ERR_IO;
    }

    /* Use fseek/ftell to stay simple and portable for local demo files. */
    if (fseek(f, 0, SEEK_END) != 0) {
        (void)fclose(f);
        return BCSIM_ERR_IO;
    }
    pos = ftell(f);
    (void)fclose(f);
    if (pos < 0) {
        return BCSIM_ERR_IO;
    }
    *size = (uint64_t)pos;
    return BCSIM_OK;
}

/*
 * Create a file if it does not exist while preserving existing content.
 *
 * Opening in append-binary mode is enough to create a missing local file. No
 * bytes are written here.
 */
static int bcSimTouchFile(const char* path) {
    FILE* f;
    if (path == 0 || path[0] == 0) {
        return BCSIM_OK;
    }
    f = fopen(path, "ab");
    if (f == 0) {
        return BCSIM_ERR_IO;
    }
    if (fclose(f) != 0) {
        return BCSIM_ERR_IO;
    }
    return BCSIM_OK;
}

/*
 * Acquire the writer lock.
 *
 * Creating a directory is atomic on local filesystems: if several processes try
 * to create the same directory at the same time, exactly one succeeds. That is
 * enough for this demonstration transport and avoids OS-specific flock/fcntl or
 * Windows locking code in the public design.
 */
static int bcSimLock(const BcSim_t* io) {
    unsigned waited = 0u;

    if (io == 0 || io->lockPath[0] == 0) {
        return BCSIM_ERR_ARG;
    }

    for (;;) {
        if (BCSIM_MKDIR(io->lockPath) == 0) {
            return BCSIM_OK;
        }
        if (errno != EEXIST) {
            return BCSIM_ERR_LOCK;
        }
        if (waited >= BCSIM_LOCK_WAIT_MS) {
            return BCSIM_ERR_LOCK_TIMEOUT;
        }
        bcSimSleepMs(BCSIM_LOCK_POLL_MS);
        waited += BCSIM_LOCK_POLL_MS;
    }
}

/*
 * Release the writer lock directory.
 *
 * Only the lock owner should call this function. Failure usually indicates that
 * the lock directory was removed externally or that the filesystem reported an
 * unexpected error.
 */
static int bcSimUnlock(const BcSim_t* io) {
    if (io == 0 || io->lockPath[0] == 0) {
        return BCSIM_ERR_ARG;
    }
    if (BCSIM_RMDIR(io->lockPath) != 0) {
        return BCSIM_ERR_LOCK;
    }
    return BCSIM_OK;
}

/*
 * Remove self-write ranges that are completely before the current read offset.
 *
 * Once readOffset has advanced beyond a range, that range can never be needed
 * again for self-echo filtering and can be removed from the fixed table.
 */
static void bcSimPruneOwnRanges(BcSim_t* io) {
    unsigned r = 0u;
    unsigned w = 0u;

    while (r < io->ownCount) {
        if (io->own[r].to > io->readOffset) {
            if (w != r) {
                io->own[w] = io->own[r];
            }
            ++w;
        }
        ++r;
    }
    io->ownCount = w;
}

/* Find the own range that contains offset, or return -1 if the offset is foreign. */
static int bcSimFindOwnRangeAt(const BcSim_t* io, uint64_t offset) {
    unsigned i;
    for (i = 0u; i < io->ownCount; ++i) {
        if (io->own[i].from <= offset && offset < io->own[i].to) {
            return (int)i;
        }
    }
    return -1;
}

/*
 * Return the next own-range start after offset, or fileSize if there is none.
 *
 * bcSimRead() uses this to copy one contiguous foreign byte span without running
 * into the next remembered self-written span.
 */
static uint64_t bcSimNextOwnStart(const BcSim_t* io, uint64_t offset, uint64_t fileSize) {
    unsigned i;
    uint64_t next = fileSize;
    for (i = 0u; i < io->ownCount; ++i) {
        if (offset < io->own[i].from && io->own[i].from < next) {
            next = io->own[i].from;
        }
    }
    return next;
}

/*
 * Remember one self-written bus-file range for later self-echo filtering.
 *
 * The range is recorded before the actual file append. If the later append
 * fails, the range may remain remembered. This is acceptable for a demo because
 * the file did not grow into that range; pruning and readOffset movement will
 * eventually make it irrelevant. Keeping the sequence simple avoids a second
 * rollback path.
 */
static int bcSimRememberOwnRange(BcSim_t* io, uint64_t from, uint64_t to) {
    bcSimPruneOwnRanges(io);
    if (from == to) {
        return BCSIM_OK;
    }
    if (io->ownCount >= BCSIM_MAX_OWN_RANGES) {
        return BCSIM_ERR_RANGE_OVERFLOW;
    }
    io->own[io->ownCount].from = from;
    io->own[io->ownCount].to = to;
    ++io->ownCount;
    return BCSIM_OK;
}

/*
 * Write the bc.log header once for a fresh log file.
 *
 * The header is emitted only when the log file exists but has size zero. This
 * keeps repeated process starts from producing repeated column descriptions.
 */
static int bcSimEnsureLogHeader(const BcSim_t* io) {
    uint64_t size = 0u;
    FILE* f;
    int e;

    if (io == 0 || io->logPath[0] == 0) {
        return BCSIM_OK;
    }

    e = bcSimFileSize(io->logPath, &size);
    if (e != BCSIM_OK) {
        return e;
    }
    if (size != 0u) {
        return BCSIM_OK;
    }

    f = fopen(io->logPath, "ab");
    if (f == 0) {
        return BCSIM_ERR_IO;
    }
    fprintf(f, "# BcSim traffic log\n");
    fprintf(f, "# bc.bus is a pure binary byte stream. This text log is diagnostic only.\n");
    fprintf(f, "# offset and len are decimal values. Bytes are hexadecimal %%02x values.\n");
    fprintf(f, "# %8s %4s %-12s %-3s %-20s %s\n", "offset", "len", "device", "dir", "status", "bytes");
    fprintf(f, "# %8s %4s %-12s %-3s %-20s %s\n", "--------", "----", "------------", "---", "--------------------", "--------------------------------");
    if (fclose(f) != 0) {
        return BCSIM_ERR_IO;
    }
    return BCSIM_OK;
}

/*
 * Append one human-readable TX or RX line to bc.log.
 *
 * The binary bus file is not touched here. The text log exists only to make the
 * demonstration understandable while keeping the binary bus stream clean.
 */
static int bcSimAppendLog(const BcSim_t* io,
                            uint64_t offset,
                            size_t len,
                            const char* dir,
                            const char* status,
                            const uint8_t* p) {
    FILE* f;
    size_t i;

    if (io == 0 || io->logPath[0] == 0) {
        return BCSIM_OK;
    }
    if (status == 0 || status[0] == 0) {
        status = "-";
    }
    if (dir == 0 || dir[0] == 0) {
        dir = "?";
    }

    f = fopen(io->logPath, "ab");
    if (f == 0) {
        return BCSIM_ERR_IO;
    }

    /* Offset and length are right-aligned. Only byte values keep leading zeros. */
    fprintf(f, "%10llu %4llu %-12s %-3s %-20s",
            (unsigned long long)offset,
            (unsigned long long)len,
            io->deviceName[0] != 0 ? io->deviceName : "?",
            dir,
            status);
    for (i = 0u; i < len; ++i) {
        fprintf(f, " %02x", (unsigned)p[i]);
    }
    fprintf(f, "\n");

    if (fclose(f) != 0) {
        return BCSIM_ERR_IO;
    }
    return BCSIM_OK;
}

/* Convert a public error code into a stable diagnostic string. */
const char* bcSimErrorString(int err) {
    switch (err) {
    case BCSIM_OK: return "ok";
    case BCSIM_ERR_ARG: return "invalid argument";
    case BCSIM_ERR_IO: return "I/O error";
    case BCSIM_ERR_LOCK: return "lock error";
    case BCSIM_ERR_LOCK_TIMEOUT: return "lock timeout";
    case BCSIM_ERR_RANGE_OVERFLOW: return "own-range table overflow";
    case BCSIM_ERR_PATH_TOO_LONG: return "path or name too long";
    case BCSIM_ERR_NOT_OPEN: return "handle is not open";
    default: return "unknown error";
    }
}

int bcSimOpen(BcSim_t* io,
                const char* busPath,
                const char* logPath,
                const char* deviceName) {
    uint64_t size = 0u;
    int e;

    if (io == 0 || busPath == 0 || busPath[0] == 0) {
        return BCSIM_ERR_ARG;
    }

    /* Clear all fields first so every later error leaves a defined structure. */
    memset(io, 0, sizeof(*io));

    /* Store all user supplied paths/names in fixed fields owned by the handle. */
    e = bcSimCopyString(io->busPath, sizeof(io->busPath), busPath);
    if (e != BCSIM_OK) {
        return e;
    }
    e = bcSimCopyString(io->logPath, sizeof(io->logPath), logPath != 0 ? logPath : "");
    if (e != BCSIM_OK) {
        return e;
    }
    e = bcSimCopyString(io->deviceName, sizeof(io->deviceName),
                          deviceName != 0 && deviceName[0] != 0 ? deviceName : "?");
    if (e != BCSIM_OK) {
        return e;
    }
    e = bcSimMakeLockPath(io->lockPath, sizeof(io->lockPath), io->busPath);
    if (e != BCSIM_OK) {
        return e;
    }

    /* Ensure the binary stream file exists without truncating old content. */
    e = bcSimTouchFile(io->busPath);
    if (e != BCSIM_OK) {
        return e;
    }

    if (io->logPath[0] != 0) {
        e = bcSimTouchFile(io->logPath);
        if (e != BCSIM_OK) {
            return e;
        }
        /* The lock avoids multiple concurrent processes creating duplicate headers. */
        e = bcSimLock(io);
        if (e != BCSIM_OK) {
            return e;
        }
        e = bcSimEnsureLogHeader(io);
        (void)bcSimUnlock(io);
        if (e != BCSIM_OK) {
            return e;
        }
    }

    e = bcSimFileSize(io->busPath, &size);
    if (e != BCSIM_OK) {
        return e;
    }

    /* Join the live bus at EOF; historical bytes are not replayed by default. */
    io->readOffset = size;
    io->isOpen = 1;
    return BCSIM_OK;
}

int bcSimRead(BcSim_t* io,
                uint8_t* p,
                size_t max,
                const char* status) {
    FILE* f;
    uint64_t fileSize = 0u;
    size_t out = 0u;
    int e;

    if (io == 0 || p == 0) {
        return BCSIM_ERR_ARG;
    }
    if (!io->isOpen) {
        return BCSIM_ERR_NOT_OPEN;
    }
    if (max == 0u) {
        return 0;
    }

#if BCSIM_READ_USES_LOCK == 1
    /* Optional deterministic mode: do not read while a writer is appending. */
    e = bcSimLock(io);
    if (e != 0) {
        return e;
    }
#endif

    e = bcSimFileSize(io->busPath, &fileSize);
    if (e != BCSIM_OK) {
#if BCSIM_READ_USES_LOCK == 1
        (void)bcSimUnlock(io);
#endif
        return e;
    }

    if (io->readOffset >= fileSize) {
#if BCSIM_READ_USES_LOCK == 1
        (void)bcSimUnlock(io);
#endif
        return 0;
    }

    f = fopen(io->busPath, "rb");
    if (f == 0) {
#if BCSIM_READ_USES_LOCK == 1
        (void)bcSimUnlock(io);
#endif
        return BCSIM_ERR_IO;
    }

    while (io->readOffset < fileSize && out < max) {
        int ownIndex;
        uint64_t foreignEnd;
        uint64_t want64;
        size_t want;
        size_t got;
        uint64_t chunkOffset;
        size_t chunkOutStart;

        /* Keep the own-range table small before checking the current offset. */
        bcSimPruneOwnRanges(io);

        /* Consume self-written bytes silently. They stay in bc.bus but are not returned. */
        ownIndex = bcSimFindOwnRangeAt(io, io->readOffset);
        if (ownIndex >= 0) {
            uint64_t skipTo = io->own[ownIndex].to;
            if (skipTo > fileSize) {
                skipTo = fileSize;
            }
            io->readOffset = skipTo;
            continue;
        }

        /* Copy only up to the next own range, EOF snapshot, or caller buffer limit. */
        foreignEnd = bcSimNextOwnStart(io, io->readOffset, fileSize);
        want64 = foreignEnd - io->readOffset;
        if (want64 > (uint64_t)(max - out)) {
            want64 = (uint64_t)(max - out);
        }
        if (want64 > (uint64_t)SIZE_MAX) {
            want64 = (uint64_t)SIZE_MAX;
        }
        want = (size_t)want64;
        if (want == 0u) {
            break;
        }

        if (fseek(f, (long)io->readOffset, SEEK_SET) != 0) {
            (void)fclose(f);
#if BCSIM_READ_USES_LOCK == 1
            (void)bcSimUnlock(io);
#endif
            return BCSIM_ERR_IO;
        }

        chunkOffset = io->readOffset;
        chunkOutStart = out;
        got = fread(p + out, 1u, want, f);
        if (got == 0u) {
            if (ferror(f)) {
                (void)fclose(f);
#if BCSIM_READ_USES_LOCK == 1
                (void)bcSimUnlock(io);
#endif
                return BCSIM_ERR_IO;
            }
            break;
        }

        out += got;
        io->readOffset += (uint64_t)got;
        (void)bcSimAppendLog(io, chunkOffset, got, "RX", status, p + chunkOutStart);
    }

    if (fclose(f) != 0) {
#if BCSIM_READ_USES_LOCK == 1
        (void)bcSimUnlock(io);
#endif
        return BCSIM_ERR_IO;
    }

#if BCSIM_READ_USES_LOCK == 1
    e = bcSimUnlock(io);
    if (e != BCSIM_OK) {
        return e;
    }
#endif

    return (int)out;
}

int bcSimWrite(BcSim_t* io,
                 const uint8_t* p,
                 size_t n,
                 const char* status) {
    FILE* f;
    uint64_t start = 0u;
    int e;

    if (io == 0 || (p == 0 && n != 0u)) {
        return BCSIM_ERR_ARG;
    }
    if (!io->isOpen) {
        return BCSIM_ERR_NOT_OPEN;
    }
    if (n == 0u) {
        return 0;
    }

    /* Only writers always lock, so one byte chunk is appended as one contiguous span. */
    e = bcSimLock(io);
    if (e != BCSIM_OK) {
        return e;
    }

    /* The current EOF becomes the start offset for self-echo filtering and logging. */
    e = bcSimFileSize(io->busPath, &start);
    if (e != BCSIM_OK) {
        (void)bcSimUnlock(io);
        return e;
    }

    e = bcSimRememberOwnRange(io, start, start + (uint64_t)n);
    if (e != BCSIM_OK) {
        (void)bcSimUnlock(io);
        return e;
    }

    f = fopen(io->busPath, "ab");
    if (f == 0) {
        (void)bcSimUnlock(io);
        return BCSIM_ERR_IO;
    }
    if (fwrite(p, 1u, n, f) != n) {
        (void)fclose(f);
        (void)bcSimUnlock(io);
        return BCSIM_ERR_IO;
    }
    if (fclose(f) != 0) {
        (void)bcSimUnlock(io);
        return BCSIM_ERR_IO;
    }

    (void)bcSimAppendLog(io, start, n, "TX", status, p);

    e = bcSimUnlock(io);
    if (e != BCSIM_OK) {
        return e;
    }

    return (int)n;
}

void bcSimClose(BcSim_t* io) {
    if (io != 0) {
        /* No persistent file handles exist; clearing the state prevents reuse bugs. */
        memset(io, 0, sizeof(*io));
    }
}
