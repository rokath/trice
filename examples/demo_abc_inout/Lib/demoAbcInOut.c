#if defined(__APPLE__) && !defined(_DARWIN_C_SOURCE)
#define _DARWIN_C_SOURCE
#elif !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

/*
 * demoAbcInOut.c
 *
 * File-backed byte-stream transport for Trice ABC demonstrations.
 *
 * The design goal is maximum clarity and minimum demo-specific code in the
 * device applications. Several PC processes share one append-only binary file,
 * normally named abc.bus. Each process writes byte chunks to the end of the
 * file and polls the file for bytes appended by other processes.
 *
 * This module deliberately does not know about Trice packet formats. It does
 * not frame records, deframe COBS/TCOBS, decrypt XTEA, decode Trice IDs, or
 * call ABC handlers. It only provides a visible byte-stream medium and a small
 * amount of demo convenience:
 *
 *   - append-only writes to abc.bus,
 *   - writer serialization by atomic lock directory,
 *   - self-echo filtering by remembered file-offset ranges,
 *   - optional human-readable TX/RX hex logging to abc.log.
 *
 * The binary bus file remains clean: every byte in abc.bus came from a caller's
 * demoAbcWrite() buffer. This is the reason why external tools can inspect the
 * same file as the running demo processes.
 */

#include "demoAbcInOut.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#include <direct.h>
#include <windows.h>
#define DEMO_ABC_MKDIR(path) _mkdir(path)
#define DEMO_ABC_RMDIR(path) _rmdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define DEMO_ABC_MKDIR(path) mkdir(path, 0777)
#define DEMO_ABC_RMDIR(path) rmdir(path)
#endif

/* Sleep helper used while waiting for the writer lock directory. */
static void demoAbcSleepMs(unsigned ms) {
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
static int demoAbcCopyString(char* dst, size_t dstSize, const char* src) {
    size_t n;
    if (dst == 0 || dstSize == 0u) {
        return DEMO_ABC_ERR_ARG;
    }
    if (src == 0) {
        src = "";
    }
    n = strlen(src);
    if (n >= dstSize) {
        return DEMO_ABC_ERR_PATH_TOO_LONG;
    }
    memcpy(dst, src, n + 1u);
    return DEMO_ABC_OK;
}

/* Derive "abc.bus.lock" from "abc.bus". */
static int demoAbcMakeLockPath(char* dst, size_t dstSize, const char* busPath) {
    int n;
    if (dst == 0 || dstSize == 0u || busPath == 0 || busPath[0] == 0) {
        return DEMO_ABC_ERR_ARG;
    }
    n = snprintf(dst, dstSize, "%s.lock", busPath);
    if (n < 0 || (size_t)n >= dstSize) {
        return DEMO_ABC_ERR_PATH_TOO_LONG;
    }
    return DEMO_ABC_OK;
}


/* Return the current file size. Missing files are treated as size zero. */
static int demoAbcFileSize(const char* path, uint64_t* size) {
    FILE* f;
    long pos;

    if (path == 0 || path[0] == 0 || size == 0) {
        return DEMO_ABC_ERR_ARG;
    }

    f = fopen(path, "rb");
    if (f == 0) {
        if (errno == ENOENT) {
            *size = 0u;
            return DEMO_ABC_OK;
        }
        return DEMO_ABC_ERR_IO;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        (void)fclose(f);
        return DEMO_ABC_ERR_IO;
    }
    pos = ftell(f);
    (void)fclose(f);
    if (pos < 0) {
        return DEMO_ABC_ERR_IO;
    }
    *size = (uint64_t)pos;
    return DEMO_ABC_OK;
}

/* Create a file if it does not exist. Existing content is preserved. */
static int demoAbcTouchFile(const char* path) {
    FILE* f;
    if (path == 0 || path[0] == 0) {
        return DEMO_ABC_OK;
    }
    f = fopen(path, "ab");
    if (f == 0) {
        return DEMO_ABC_ERR_IO;
    }
    if (fclose(f) != 0) {
        return DEMO_ABC_ERR_IO;
    }
    return DEMO_ABC_OK;
}

/*
 * Acquire the writer lock.
 *
 * Creating a directory is atomic on local filesystems: if several processes try
 * to create the same directory at the same time, exactly one succeeds. That is
 * enough for this demonstration transport and avoids OS-specific flock/fcntl or
 * Windows locking code in the public design.
 */
static int demoAbcLock(const demoAbcInOut_t* io) {
    unsigned waited = 0u;

    if (io == 0 || io->lockPath[0] == 0) {
        return DEMO_ABC_ERR_ARG;
    }

    for (;;) {
        if (DEMO_ABC_MKDIR(io->lockPath) == 0) {
            return DEMO_ABC_OK;
        }
        if (errno != EEXIST) {
            return DEMO_ABC_ERR_LOCK;
        }
        if (waited >= DEMO_ABC_LOCK_WAIT_MS) {
            return DEMO_ABC_ERR_LOCK_TIMEOUT;
        }
        demoAbcSleepMs(DEMO_ABC_LOCK_POLL_MS);
        waited += DEMO_ABC_LOCK_POLL_MS;
    }
}

/* Release the writer lock directory. */
static int demoAbcUnlock(const demoAbcInOut_t* io) {
    if (io == 0 || io->lockPath[0] == 0) {
        return DEMO_ABC_ERR_ARG;
    }
    if (DEMO_ABC_RMDIR(io->lockPath) != 0) {
        return DEMO_ABC_ERR_LOCK;
    }
    return DEMO_ABC_OK;
}

/* Remove self-write ranges that are completely before the current read offset. */
static void demoAbcPruneOwnRanges(demoAbcInOut_t* io) {
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
static int demoAbcFindOwnRangeAt(const demoAbcInOut_t* io, uint64_t offset) {
    unsigned i;
    for (i = 0u; i < io->ownCount; ++i) {
        if (io->own[i].from <= offset && offset < io->own[i].to) {
            return (int)i;
        }
    }
    return -1;
}

/* Return the next own-range start after offset, or fileSize if there is none. */
static uint64_t demoAbcNextOwnStart(const demoAbcInOut_t* io, uint64_t offset, uint64_t fileSize) {
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
static int demoAbcRememberOwnRange(demoAbcInOut_t* io, uint64_t from, uint64_t to) {
    demoAbcPruneOwnRanges(io);
    if (from == to) {
        return DEMO_ABC_OK;
    }
    if (io->ownCount >= DEMO_ABC_MAX_OWN_RANGES) {
        return DEMO_ABC_ERR_RANGE_OVERFLOW;
    }
    io->own[io->ownCount].from = from;
    io->own[io->ownCount].to = to;
    ++io->ownCount;
    return DEMO_ABC_OK;
}

/* Write the abc.log header once for a fresh log file. */
static int demoAbcEnsureLogHeader(const demoAbcInOut_t* io) {
    uint64_t size = 0u;
    FILE* f;
    int e;

    if (io == 0 || io->logPath[0] == 0) {
        return DEMO_ABC_OK;
    }

    e = demoAbcFileSize(io->logPath, &size);
    if (e != DEMO_ABC_OK) {
        return e;
    }
    if (size != 0u) {
        return DEMO_ABC_OK;
    }

    f = fopen(io->logPath, "ab");
    if (f == 0) {
        return DEMO_ABC_ERR_IO;
    }
    fprintf(f, "# demoAbcInOut traffic log\n");
    fprintf(f, "# abc.bus is a pure binary byte stream. This text log is diagnostic only.\n");
    fprintf(f, "# offset and len are decimal values. Bytes are hexadecimal %%02x values.\n");
    fprintf(f, "# %8s %4s %-12s %-3s %-20s %s\n", "offset", "len", "device", "dir", "status", "bytes");
    fprintf(f, "# %8s %4s %-12s %-3s %-20s %s\n", "--------", "----", "------------", "---", "--------------------", "--------------------------------");
    if (fclose(f) != 0) {
        return DEMO_ABC_ERR_IO;
    }
    return DEMO_ABC_OK;
}

/* Append one human-readable TX or RX line to abc.log. abc.bus is not touched. */
static int demoAbcAppendLog(const demoAbcInOut_t* io,
                            uint64_t offset,
                            size_t len,
                            const char* dir,
                            const char* status,
                            const uint8_t* p) {
    FILE* f;
    size_t i;

    if (io == 0 || io->logPath[0] == 0) {
        return DEMO_ABC_OK;
    }
    if (status == 0 || status[0] == 0) {
        status = "-";
    }
    if (dir == 0 || dir[0] == 0) {
        dir = "?";
    }

    f = fopen(io->logPath, "ab");
    if (f == 0) {
        return DEMO_ABC_ERR_IO;
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
        return DEMO_ABC_ERR_IO;
    }
    return DEMO_ABC_OK;
}

const char* demoAbcErrorString(int err) {
    switch (err) {
    case DEMO_ABC_OK: return "ok";
    case DEMO_ABC_ERR_ARG: return "invalid argument";
    case DEMO_ABC_ERR_IO: return "I/O error";
    case DEMO_ABC_ERR_LOCK: return "lock error";
    case DEMO_ABC_ERR_LOCK_TIMEOUT: return "lock timeout";
    case DEMO_ABC_ERR_RANGE_OVERFLOW: return "own-range table overflow";
    case DEMO_ABC_ERR_PATH_TOO_LONG: return "path or name too long";
    case DEMO_ABC_ERR_NOT_OPEN: return "handle is not open";
    default: return "unknown error";
    }
}

int demoAbcOpen(demoAbcInOut_t* io,
                const char* busPath,
                const char* logPath,
                const char* deviceName) {
    uint64_t size = 0u;
    int e;

    if (io == 0 || busPath == 0 || busPath[0] == 0) {
        return DEMO_ABC_ERR_ARG;
    }

    memset(io, 0, sizeof(*io));

    e = demoAbcCopyString(io->busPath, sizeof(io->busPath), busPath);
    if (e != DEMO_ABC_OK) {
        return e;
    }
    e = demoAbcCopyString(io->logPath, sizeof(io->logPath), logPath != 0 ? logPath : "");
    if (e != DEMO_ABC_OK) {
        return e;
    }
    e = demoAbcCopyString(io->deviceName, sizeof(io->deviceName),
                          deviceName != 0 && deviceName[0] != 0 ? deviceName : "?");
    if (e != DEMO_ABC_OK) {
        return e;
    }
    e = demoAbcMakeLockPath(io->lockPath, sizeof(io->lockPath), io->busPath);
    if (e != DEMO_ABC_OK) {
        return e;
    }

    e = demoAbcTouchFile(io->busPath);
    if (e != DEMO_ABC_OK) {
        return e;
    }

    if (io->logPath[0] != 0) {
        e = demoAbcTouchFile(io->logPath);
        if (e != DEMO_ABC_OK) {
            return e;
        }
        /* The lock avoids multiple concurrent processes creating duplicate headers. */
        e = demoAbcLock(io);
        if (e != DEMO_ABC_OK) {
            return e;
        }
        e = demoAbcEnsureLogHeader(io);
        (void)demoAbcUnlock(io);
        if (e != DEMO_ABC_OK) {
            return e;
        }
    }

    e = demoAbcFileSize(io->busPath, &size);
    if (e != DEMO_ABC_OK) {
        return e;
    }

    io->readOffset = size;
    io->isOpen = 1;
    return DEMO_ABC_OK;
}

int demoAbcRead(demoAbcInOut_t* io,
                uint8_t* p,
                size_t max,
                const char* status) {
    FILE* f;
    uint64_t fileSize = 0u;
    size_t out = 0u;
    int e;

    if (io == 0 || p == 0) {
        return DEMO_ABC_ERR_ARG;
    }
    if (!io->isOpen) {
        return DEMO_ABC_ERR_NOT_OPEN;
    }
    if (max == 0u) {
        return 0;
    }

#if DEMO_ABC_READ_USES_LOCK == 1
    e = demoAbcLock(io);
    if (e != DEMO_ABC_OK) {
        return e;
    }
#endif

    e = demoAbcFileSize(io->busPath, &fileSize);
    if (e != DEMO_ABC_OK) {
#if DEMO_ABC_READ_USES_LOCK == 1
        (void)demoAbcUnlock(io);
#endif
        return e;
    }

    if (io->readOffset >= fileSize) {
#if DEMO_ABC_READ_USES_LOCK == 1
        (void)demoAbcUnlock(io);
#endif
        return 0;
    }

    f = fopen(io->busPath, "rb");
    if (f == 0) {
#if DEMO_ABC_READ_USES_LOCK == 1
        (void)demoAbcUnlock(io);
#endif
        return DEMO_ABC_ERR_IO;
    }

    while (io->readOffset < fileSize && out < max) {
        int ownIndex;
        uint64_t foreignEnd;
        uint64_t want64;
        size_t want;
        size_t got;
        uint64_t chunkOffset;
        size_t chunkOutStart;

        demoAbcPruneOwnRanges(io);
        ownIndex = demoAbcFindOwnRangeAt(io, io->readOffset);
        if (ownIndex >= 0) {
            uint64_t skipTo = io->own[ownIndex].to;
            if (skipTo > fileSize) {
                skipTo = fileSize;
            }
            io->readOffset = skipTo;
            continue;
        }

        foreignEnd = demoAbcNextOwnStart(io, io->readOffset, fileSize);
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
#if DEMO_ABC_READ_USES_LOCK == 1
            (void)demoAbcUnlock(io);
#endif
            return DEMO_ABC_ERR_IO;
        }

        chunkOffset = io->readOffset;
        chunkOutStart = out;
        got = fread(p + out, 1u, want, f);
        if (got == 0u) {
            if (ferror(f)) {
                (void)fclose(f);
#if DEMO_ABC_READ_USES_LOCK == 1
                (void)demoAbcUnlock(io);
#endif
                return DEMO_ABC_ERR_IO;
            }
            break;
        }

        out += got;
        io->readOffset += (uint64_t)got;
        (void)demoAbcAppendLog(io, chunkOffset, got, "RX", status, p + chunkOutStart);
    }

    if (fclose(f) != 0) {
#if DEMO_ABC_READ_USES_LOCK == 1
        (void)demoAbcUnlock(io);
#endif
        return DEMO_ABC_ERR_IO;
    }

#if DEMO_ABC_READ_USES_LOCK == 1
    e = demoAbcUnlock(io);
    if (e != DEMO_ABC_OK) {
        return e;
    }
#endif

    return (int)out;
}

int demoAbcWrite(demoAbcInOut_t* io,
                 const uint8_t* p,
                 size_t n,
                 const char* status) {
    FILE* f;
    uint64_t start = 0u;
    int e;

    if (io == 0 || (p == 0 && n != 0u)) {
        return DEMO_ABC_ERR_ARG;
    }
    if (!io->isOpen) {
        return DEMO_ABC_ERR_NOT_OPEN;
    }
    if (n == 0u) {
        return 0;
    }

    e = demoAbcLock(io);
    if (e != DEMO_ABC_OK) {
        return e;
    }

    e = demoAbcFileSize(io->busPath, &start);
    if (e != DEMO_ABC_OK) {
        (void)demoAbcUnlock(io);
        return e;
    }

    e = demoAbcRememberOwnRange(io, start, start + (uint64_t)n);
    if (e != DEMO_ABC_OK) {
        (void)demoAbcUnlock(io);
        return e;
    }

    f = fopen(io->busPath, "ab");
    if (f == 0) {
        (void)demoAbcUnlock(io);
        return DEMO_ABC_ERR_IO;
    }
    if (fwrite(p, 1u, n, f) != n) {
        (void)fclose(f);
        (void)demoAbcUnlock(io);
        return DEMO_ABC_ERR_IO;
    }
    if (fclose(f) != 0) {
        (void)demoAbcUnlock(io);
        return DEMO_ABC_ERR_IO;
    }

    (void)demoAbcAppendLog(io, start, n, "TX", status, p);

    e = demoAbcUnlock(io);
    if (e != DEMO_ABC_OK) {
        return e;
    }

    return (int)n;
}

void demoAbcClose(demoAbcInOut_t* io) {
    if (io != 0) {
        memset(io, 0, sizeof(*io));
    }
}
