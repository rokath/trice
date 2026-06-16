#if defined(__APPLE__) && !defined(_DARWIN_C_SOURCE)
#define _DARWIN_C_SOURCE
#elif !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

/*
 * BcSim.c
 *
 * File-backed byte-stream transport for Trice broadcast simulations.
 *
 * The design goal is maximum clarity and minimum demo-specific code in the
 * device applications. Several PC processes share one append-only binary file,
 * normally named bc.bus. Each process writes byte chunks to the end of the
 * file and polls the file for bytes appended by other processes.
 *
 * This module deliberately does not know about Trice packet formats. It does
 * not frame records, deframe COBS/TCOBS, decrypt XTEA, decode Trice IDs, or
 * call BC handlers. It only provides a visible byte-stream medium and a small
 * amount of demo convenience:
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
#define BCSIM_MKDIR(path) _mkdir(path)
#define BCSIM_RMDIR(path) _rmdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define BCSIM_MKDIR(path) mkdir(path, 0777)
#define BCSIM_RMDIR(path) rmdir(path)
#endif

/* Sleep helper used while waiting for the writer lock directory. */
static void bcSimSleepMs(unsigned ms) {
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000u);
    ts.tv_nsec = (long)((ms % 1000u) * 1000000u);
    (void)nanosleep(&ts, 0);
#endif
}

/* Copy a user string into a fixed-size field without silent truncation. */
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

/* Derive "bc.bus.lock" from "bc.bus". */
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


/* Return the current file size. Missing files are treated as size zero. */
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

/* Create a file if it does not exist. Existing content is preserved. */
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

/* Release the writer lock directory. */
static int bcSimUnlock(const BcSim_t* io) {
    if (io == 0 || io->lockPath[0] == 0) {
        return BCSIM_ERR_ARG;
    }
    if (BCSIM_RMDIR(io->lockPath) != 0) {
        return BCSIM_ERR_LOCK;
    }
    return BCSIM_OK;
}

/* Remove self-write ranges that are completely before the current read offset. */
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

/* Return the next own-range start after offset, or fileSize if there is none. */
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

/* Remember one self-written bus-file range for later self-echo filtering. */
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

/* Write the bc.log header once for a fresh log file. */
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

/* Append one human-readable TX or RX line to bc.log. bc.bus is not touched. */
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

    memset(io, 0, sizeof(*io));

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
    e = bcSimLock(io);
    if (e != BCSIM_OK) {
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

        bcSimPruneOwnRanges(io);
        ownIndex = bcSimFindOwnRangeAt(io, io->readOffset);
        if (ownIndex >= 0) {
            uint64_t skipTo = io->own[ownIndex].to;
            if (skipTo > fileSize) {
                skipTo = fileSize;
            }
            io->readOffset = skipTo;
            continue;
        }

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

    e = bcSimLock(io);
    if (e != BCSIM_OK) {
        return e;
    }

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
        memset(io, 0, sizeof(*io));
    }
}
