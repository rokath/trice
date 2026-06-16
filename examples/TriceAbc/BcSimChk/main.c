#if defined(__APPLE__) && !defined(_DARWIN_C_SOURCE)
#define _DARWIN_C_SOURCE
#elif !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200809L
#endif

/*
 * main.c
 *
 * Small check/demo program for ../BcSim/BcSim.c.
 *
 * Build one binary and start it several times with different -name values. All
 * instances share the same bc.bus file. Each instance periodically reads bytes
 * appended by the other instances and writes a few own demo messages. Its own
 * messages are filtered by bcSimRead() using the file-offset ranges remembered
 * by bcSimWrite().
 *
 * This check program deliberately writes random byte blocks with random lengths.
 * A real Trice broadcast-command simulation would replace buildRandomPayload() with a tiny
 * BC/TCOBS/COBS/XTEA transmit layer and would feed received bytes into the
 * corresponding decoder before calling the Trice receive runtime. The BcSim module
 * itself remains unchanged because it transports only bytes.
 */

#include "BcSim.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

#define DEMO_RX_BUFFER_SIZE 1024u
#define DEMO_PAYLOAD_MIN 4u
#define DEMO_PAYLOAD_MAX 32u
#define DEMO_STATUS_SIZE 64u

static void sleepMs(unsigned ms) {
#if defined(_WIN32)
    Sleep((DWORD)ms);
#else
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000u);
    ts.tv_nsec = (long)((ms % 1000u) * 1000000u);
    (void)nanosleep(&ts, 0);
#endif
}

static unsigned processIdForSeed(void) {
#if defined(_WIN32)
    return (unsigned)GetCurrentProcessId();
#else
    return (unsigned)getpid();
#endif
}

static unsigned nameHash(const char* s) {
    unsigned h = 2166136261u;
    while (s != 0 && *s != 0) {
        h ^= (unsigned char)*s++;
        h *= 16777619u;
    }
    return h;
}

static void usage(const char* exe) {
    printf("Usage: %s -name NAME [options]\n", exe);
    printf("\nOptions:\n");
    printf("  -bus FILE       Shared binary bus file, default bc.bus\n");
    printf("  -log FILE       Human-readable log file, default bc.log\n");
    printf("  -auto N         Send N random demo payloads, default 5\n");
    printf("  -interval MS    Poll/send interval, default 250\n");
    printf("  -h, --help      Show this help\n");
    printf("\nExample:\n");
    printf("  %s -name A -auto 4\n", exe);
    printf("\nNotes:\n");
    printf("  A newly opened device starts reading at the current end of bc.bus.\n");
    printf("  There is intentionally no -from-start option for normal devices.\n");
}

static int parseInt(const char* s, int def) {
    char* end = 0;
    long v;
    if (s == 0) {
        return def;
    }
    v = strtol(s, &end, 10);
    if (end == s || *end != 0) {
        return def;
    }
    if (v < 0) {
        return 0;
    }
    if (v > 1000000L) {
        return 1000000;
    }
    return (int)v;
}

static void printBytesAsHex(const uint8_t* p, int n) {
    int i;
    for (i = 0; i < n; ++i) {
        printf(" %02x", (unsigned)p[i]);
    }
}

/* Build one random byte block with a random length in the inclusive range 4..32. */
static int buildRandomPayload(uint8_t* dst, size_t dstSize) {
    size_t len;
    size_t i;

    if (dst == 0 || dstSize < DEMO_PAYLOAD_MAX) {
        return -1;
    }

    len = DEMO_PAYLOAD_MIN + (size_t)(rand() % (int)(DEMO_PAYLOAD_MAX - DEMO_PAYLOAD_MIN + 1u));
    for (i = 0u; i < len; ++i) {
        dst[i] = (uint8_t)(rand() & 0xffu);
    }
    return (int)len;
}

/* Drain all currently available foreign bytes and print them. */
static int drainInput(BcSim_t* io, const char* name, const char* status) {
    for (;;) {
        uint8_t rx[DEMO_RX_BUFFER_SIZE];
        int n = bcSimRead(io, rx, sizeof(rx), status);
        if (n < 0) {
            fprintf(stderr, "[%s] bcSimRead failed: %s\n", name, bcSimErrorString(n));
            return n;
        }
        if (n == 0) {
            return 0;
        }
        printf("[%s] RX %d bytes", name, n);
        printBytesAsHex(rx, n);
        printf("\n");
    }
}

int main(int argc, char** argv) {
    const char* name = 0;
    const char* busPath = "bc.bus";
    const char* logPath = "bc.log";
    int autoCount = 5;
    int intervalMs = 250;
    int i;
    BcSim_t io;
    int e;

    for (i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-name") == 0 && i + 1 < argc) {
            name = argv[++i];
        } else if (strcmp(argv[i], "-bus") == 0 && i + 1 < argc) {
            busPath = argv[++i];
        } else if (strcmp(argv[i], "-log") == 0 && i + 1 < argc) {
            logPath = argv[++i];
        } else if (strcmp(argv[i], "-auto") == 0 && i + 1 < argc) {
            autoCount = parseInt(argv[++i], autoCount);
        } else if (strcmp(argv[i], "-interval") == 0 && i + 1 < argc) {
            intervalMs = parseInt(argv[++i], intervalMs);
        } else {
            fprintf(stderr, "Unknown or incomplete option: %s\n", argv[i]);
            usage(argv[0]);
            return 2;
        }
    }

    if (name == 0 || name[0] == 0) {
        fprintf(stderr, "Missing -name NAME.\n");
        usage(argv[0]);
        return 2;
    }

    srand((unsigned)time(0) ^ processIdForSeed() ^ nameHash(name));

    e = bcSimOpen(&io, busPath, logPath, name);
    if (e < 0) {
        fprintf(stderr, "[%s] bcSimOpen failed: %s\n", name, bcSimErrorString(e));
        return 1;
    }

    printf("[%s] opened bus=%s log=%s readOffset=%llu\n",
           name,
           busPath,
           logPath,
           (unsigned long long)io.readOffset);

    for (i = 0; i < autoCount; ++i) {
        char status[DEMO_STATUS_SIZE];
        uint8_t payload[DEMO_PAYLOAD_MAX];
        int payloadLen;

        snprintf(status, sizeof(status), "poll-%d", i);
        e = drainInput(&io, name, status);
        if (e < 0) {
            bcSimClose(&io);
            return 1;
        }

        payloadLen = buildRandomPayload(payload, sizeof(payload));
        if (payloadLen < 0) {
            fprintf(stderr, "[%s] failed to build random payload\n", name);
            bcSimClose(&io);
            return 1;
        }

        snprintf(status, sizeof(status), "tx-%d", i);
        e = bcSimWrite(&io, payload, (size_t)payloadLen, status);
        if (e < 0) {
            fprintf(stderr, "[%s] bcSimWrite failed: %s\n", name, bcSimErrorString(e));
            bcSimClose(&io);
            return 1;
        }
        printf("[%s] TX %d bytes", name, e);
        printBytesAsHex(payload, e);
        printf("\n");
        sleepMs((unsigned)intervalMs);
    }

    for (i = 0; i < 6; ++i) {
        e = drainInput(&io, name, "final-drain");
        if (e < 0) {
            bcSimClose(&io);
            return 1;
        }
        sleepMs((unsigned)intervalMs);
    }

    printf("[%s] done\n", name);
    bcSimClose(&io);
    return 0;
}
