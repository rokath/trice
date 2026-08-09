// SPDX-License-Identifier: MIT

/*
 * Request POSIX interfaces such as nanosleep() and getaddrinfo() when building
 * on Linux or macOS. Windows uses its native Winsock and Sleep APIs instead.
 */
#ifndef _WIN32
#define _POSIX_C_SOURCE 200112L
#endif

#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
typedef SOCKET SocketHandle;
#define INVALID_SOCKET_HANDLE INVALID_SOCKET
#else
#include <netdb.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
typedef int SocketHandle;
#define INVALID_SOCKET_HANDLE (-1)
#endif

#include "trice.h"
#include "trice_main_c_K59380759EB9D6878.h" // trice-bind: keep as last include before this file's Trice calls

#define DEMO_PI 3.14159265358979323846
#define DEFAULT_BINARY_FILE "DemoData_Trice.bin"

/*
 * This global is read by the TriceStamp32 macro in triceConfig.h. Its unit is
 * exactly 10 ms, not milliseconds and not seconds.
 */
uint32_t demo_plot_timestamp_10ms = 0;

/*
 * The direct TRice writer sends a complete TCOBS-framed record to exactly one
 * destination: a binary file, stdout, or a UDP datagram.
 */
typedef enum {
    OUTPUT_BINARY_FILE,
    OUTPUT_STDOUT,
    OUTPUT_UDP
} OutputKind;

typedef struct {
    double rate_hz;
    unsigned long long sample_count;
    bool delay;
    OutputKind output_kind;
    const char *file_name;
    const char *udp_host;
    const char *udp_port;
} Options;

typedef struct {
    SocketHandle socket;
    struct sockaddr_storage address;
#ifdef _WIN32
    int address_length;
    bool winsock_started;
#else
    socklen_t address_length;
#endif
} UdpSender;

/*
 * The TRice output callback has no return value. Store the first output failure
 * here so the generator loop can stop and main() can return a failure status.
 */
static bool output_failed = false;
static OutputKind active_output_kind = OUTPUT_BINARY_FILE;
static FILE *active_stream = NULL;
static UdpSender active_udp_sender;

static void print_usage(const char *program)
{
    printf(
        "Usage: %s [options]\n"
        "\n"
        "Generate TCOBS-framed TRice records containing x, y, and z.\n"
        "The 32-bit target timestamp counts units of 10 ms.\n"
        "\n"
        "Output options (choose at most one):\n"
        "  -o, --output FILE     Write a fresh binary FILE\n"
        "      --stdout          Write the binary stream to stdout\n"
        "      --udp HOST PORT   Send one framed record per UDP datagram\n"
        "                         Example: --udp 127.0.0.1 9001\n"
        "  Default: overwrite " DEFAULT_BINARY_FILE "\n"
        "\n"
        "Generator options:\n"
        "  -r, --rate HZ         Sample rate (default: 50)\n"
        "  -n, --samples N       Stop after N samples (default: unlimited)\n"
        "      --no-delay        Generate as fast as possible\n"
        "  -h, --help            Show this help\n",
        program
    );
}

/* Strictly parse a floating-point command-line value. */
static bool parse_double(const char *text, double *value)
{
    char *end = NULL;
    errno = 0;
    *value = strtod(text, &end);
    return errno == 0 && end != text && *end == '\0';
}

/* Strictly parse the requested unsigned sample count. */
static bool parse_unsigned_long_long(
    const char *text,
    unsigned long long *value)
{
    char *end = NULL;

    /* strtoull() accepts a leading minus sign, which is not useful here. */
    if (text[0] == '-') {
        return false;
    }

    errno = 0;
    *value = strtoull(text, &end, 10);
    return errno == 0 && end != text && *end == '\0';
}

/* Ensure that an option has enough following values before advancing argv. */
static bool require_values(
    int current_index,
    int argument_count,
    int required_count,
    const char *option)
{
    if (current_index + required_count < argument_count) {
        return true;
    }

    fprintf(stderr, "Missing value for %s\n", option);
    return false;
}

/*
 * Parse the compact command line without adding a platform-specific dependency.
 * The default destination remains the fixed binary file unless the user
 * explicitly selects one other destination.
 */
static bool parse_options(int argc, char **argv, Options *options)
{
    bool destination_selected = false;

    for (int i = 1; i < argc; ++i) {
        const char *argument = argv[i];

        if (strcmp(argument, "-h") == 0 ||
            strcmp(argument, "--help") == 0) {
            print_usage(argv[0]);
            exit(EXIT_SUCCESS);
        } else if (strcmp(argument, "--no-delay") == 0) {
            options->delay = false;
        } else if (strcmp(argument, "-r") == 0 ||
                   strcmp(argument, "--rate") == 0) {
            if (!require_values(i, argc, 1, argument) ||
                !parse_double(argv[++i], &options->rate_hz) ||
                !isfinite(options->rate_hz) ||
                options->rate_hz <= 0.0) {
                fprintf(stderr, "Invalid sample rate\n");
                return false;
            }
        } else if (strcmp(argument, "-n") == 0 ||
                   strcmp(argument, "--samples") == 0) {
            if (!require_values(i, argc, 1, argument) ||
                !parse_unsigned_long_long(
                    argv[++i], &options->sample_count)) {
                fprintf(stderr, "Invalid sample count\n");
                return false;
            }
        } else if (strcmp(argument, "-o") == 0 ||
                   strcmp(argument, "--output") == 0) {
            if (destination_selected) {
                fprintf(stderr, "Select only one output destination\n");
                return false;
            }
            if (!require_values(i, argc, 1, argument)) {
                return false;
            }

            destination_selected = true;
            options->output_kind = OUTPUT_BINARY_FILE;
            options->file_name = argv[++i];
        } else if (strcmp(argument, "--stdout") == 0) {
            if (destination_selected) {
                fprintf(stderr, "Select only one output destination\n");
                return false;
            }

            destination_selected = true;
            options->output_kind = OUTPUT_STDOUT;
        } else if (strcmp(argument, "--udp") == 0) {
            if (destination_selected) {
                fprintf(stderr, "Select only one output destination\n");
                return false;
            }
            if (!require_values(i, argc, 2, argument)) {
                return false;
            }

            destination_selected = true;
            options->output_kind = OUTPUT_UDP;
            options->udp_host = argv[++i];
            options->udp_port = argv[++i];
        } else {
            fprintf(stderr, "Unknown option: %s\n", argument);
            return false;
        }
    }

    return true;
}

/* Hide the Windows/POSIX sleep difference behind a seconds-based function. */
static void sleep_seconds(double seconds)
{
#ifdef _WIN32
    DWORD milliseconds = (DWORD)(seconds * 1000.0 + 0.5);
    Sleep(milliseconds > 0 ? milliseconds : 1);
#else
    struct timespec remaining;
    remaining.tv_sec = (time_t)seconds;
    remaining.tv_nsec =
        (long)((seconds - (double)remaining.tv_sec) * 1.0e9);

    while (nanosleep(&remaining, &remaining) != 0 && errno == EINTR) {
        /* Resume sleeping for the remaining duration after an interrupt. */
    }
#endif
}

/*
 * Generate the same mathematical signals as DemoData_CSV. Calculations use
 * double; each value is converted to IEEE-754 float bits only at the TRice call,
 * because the demonstration uses three 32-bit TRice parameters.
 */
static void generate_values(
    double t_seconds,
    double *x,
    double *y,
    double *z)
{
    /*
     * Modulate y slowly so the Lissajous trace keeps changing visibly even
     * after the plot has collected enough samples to cover the full figure.
     */
    double phase_drift = (DEMO_PI / 3.0)
                       * sin(2.0 * DEMO_PI * 0.04 * t_seconds);

    *x = sin(2.0 * DEMO_PI * 0.70 * t_seconds);
    *y = sin(2.0 * DEMO_PI * 0.91 * t_seconds
           + DEMO_PI / 2.0
           + phase_drift);

    double cycle_position = fmod(t_seconds, 8.0);
    double pulse = cycle_position >= 7.75 ? 0.8 : 0.0;

    *z = 0.6 * sin(2.0 * DEMO_PI * 0.13 * t_seconds)
       + 0.2 * *x * *y
       + pulse;
}

/* Return the platform-specific socket error as a printable integer. */
static int socket_error_code(void)
{
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

/* Close the UDP socket and release Winsock state when applicable. */
static void udp_sender_close(UdpSender *sender)
{
    if (sender->socket != INVALID_SOCKET_HANDLE) {
#ifdef _WIN32
        closesocket(sender->socket);
#else
        close(sender->socket);
#endif
        sender->socket = INVALID_SOCKET_HANDLE;
    }

#ifdef _WIN32
    if (sender->winsock_started) {
        WSACleanup();
        sender->winsock_started = false;
    }
#endif
}

/* Resolve the destination and retain the first usable UDP address. */
static bool udp_sender_open(
    UdpSender *sender,
    const char *host,
    const char *port)
{
    struct addrinfo hints;
    struct addrinfo *addresses = NULL;
    struct addrinfo *candidate = NULL;

    memset(sender, 0, sizeof(*sender));
    sender->socket = INVALID_SOCKET_HANDLE;

#ifdef _WIN32
    WSADATA winsock_data;
    if (WSAStartup(MAKEWORD(2, 2), &winsock_data) != 0) {
        fprintf(stderr, "WSAStartup failed\n");
        return false;
    }
    sender->winsock_started = true;
#endif

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    int result = getaddrinfo(host, port, &hints, &addresses);
    if (result != 0) {
#ifdef _WIN32
        fprintf(stderr, "Cannot resolve UDP destination: %d\n", result);
#else
        fprintf(stderr, "Cannot resolve UDP destination: %s\n",
                gai_strerror(result));
#endif
        udp_sender_close(sender);
        return false;
    }

    for (candidate = addresses;
         candidate != NULL;
         candidate = candidate->ai_next) {
        sender->socket = socket(
            candidate->ai_family,
            candidate->ai_socktype,
            candidate->ai_protocol);

        if (sender->socket == INVALID_SOCKET_HANDLE) {
            continue;
        }

        if (candidate->ai_addrlen <= sizeof(sender->address)) {
            memcpy(
                &sender->address,
                candidate->ai_addr,
                candidate->ai_addrlen);
#ifdef _WIN32
            sender->address_length = (int)candidate->ai_addrlen;
#else
            sender->address_length = (socklen_t)candidate->ai_addrlen;
#endif
            break;
        }

#ifdef _WIN32
        closesocket(sender->socket);
#else
        close(sender->socket);
#endif
        sender->socket = INVALID_SOCKET_HANDLE;
    }

    freeaddrinfo(addresses);

    if (sender->socket == INVALID_SOCKET_HANDLE) {
        fprintf(stderr, "Cannot create UDP socket: %d\n", socket_error_code());
        udp_sender_close(sender);
        return false;
    }

    return true;
}

/* Send one complete TCOBS-framed TRice record in one UDP datagram. */
static bool udp_sender_send(
    const UdpSender *sender,
    const uint8_t *data,
    size_t length)
{
#ifdef _WIN32
    int sent = sendto(
        sender->socket,
        (const char *)data,
        (int)length,
        0,
        (const struct sockaddr *)&sender->address,
        sender->address_length);
    bool complete = sent == (int)length;
#else
    ssize_t sent = sendto(
        sender->socket,
        data,
        length,
        0,
        (const struct sockaddr *)&sender->address,
        sender->address_length);
    bool complete = sent == (ssize_t)length;
#endif

    if (!complete) {
        fprintf(stderr, "UDP send failed: %d\n", socket_error_code());
    }
    return complete;
}

/*
 * TRice calls this function synchronously for every direct-output record.
 *
 * For a stream destination the bytes are appended and flushed. For UDP the
 * complete framed record becomes one datagram, preserving its packet boundary.
 */
void TriceNonBlockingDirectWrite8Auxiliary(
    const uint8_t *encoded_data,
    size_t encoded_length)
{
    if (output_failed) {
        return;
    }

    if (active_output_kind == OUTPUT_UDP) {
        output_failed = !udp_sender_send(
            &active_udp_sender,
            encoded_data,
            encoded_length);
        return;
    }

    if (fwrite(encoded_data, 1, encoded_length, active_stream)
        != encoded_length) {
        fprintf(stderr, "Binary output write failed\n");
        output_failed = true;
        return;
    }

    if (fflush(active_stream) != 0) {
        fprintf(stderr, "Binary output flush failed\n");
        output_failed = true;
    }
}

int main(int argc, char **argv)
{
    Options options = {
        .rate_hz = 50.0,
        .sample_count = 0,
        .delay = true,
        .output_kind = OUTPUT_BINARY_FILE,
        .file_name = DEFAULT_BINARY_FILE,
        .udp_host = NULL,
        .udp_port = NULL
    };

    if (!parse_options(argc, argv, &options)) {
        fprintf(stderr, "Try '%s --help'.\n", argv[0]);
        return EXIT_FAILURE;
    }

    memset(&active_udp_sender, 0, sizeof(active_udp_sender));
    active_udp_sender.socket = INVALID_SOCKET_HANDLE;
    active_output_kind = options.output_kind;

    if (options.output_kind == OUTPUT_BINARY_FILE) {
        /*
         * "wb" always creates a new file or truncates an existing one. Binary
         * mode is essential on Windows because TRice data can contain 0x0A.
         */
        active_stream = fopen(options.file_name, "wb");
        if (active_stream == NULL) {
            fprintf(stderr, "Cannot open '%s': %s\n",
                    options.file_name, strerror(errno));
            return EXIT_FAILURE;
        }
        fprintf(stderr, "Writing %s\n", options.file_name);
    } else if (options.output_kind == OUTPUT_STDOUT) {
        active_stream = stdout;
#ifdef _WIN32
        /*
         * Prevent the Microsoft runtime from translating binary byte 0x0A
         * into the two-byte text sequence 0x0D 0x0A.
         */
        if (_setmode(_fileno(stdout), _O_BINARY) == -1) {
            fprintf(stderr, "Cannot switch stdout to binary mode\n");
            return EXIT_FAILURE;
        }
#endif
    } else {
        if (!udp_sender_open(
                &active_udp_sender,
                options.udp_host,
                options.udp_port)) {
            return EXIT_FAILURE;
        }
    }

    TriceInit();
    trice("msg:Hello LabPlot!\n");
    /*
     * Derive seconds and the 10 ms timestamp from the integer sample index.
     * Rounding to the nearest tick also gives sensible timestamps for sample
     * rates that are not exact multiples of 100 Hz.
     */
    const double period_seconds = 1.0 / options.rate_hz;
    for (unsigned long long sample = 0;
         !output_failed &&
         (options.sample_count == 0 || sample < options.sample_count);
         ++sample) {
        double time_seconds = (double)sample * period_seconds;
        double x;
        double y;
        double z;
        generate_values(time_seconds, &x, &y, &z);

        /*
         * Convert through uint64_t first. The final cast then gives the
         * intentional uint32_t wrap after 2^32 ticks (about 497 days).
         */
        uint64_t absolute_ticks_10ms =
            (uint64_t)(time_seconds * 100.0 + 0.5);
        demo_plot_timestamp_10ms = (uint32_t)absolute_ticks_10ms;

        if(absolute_ticks_10ms % 100 == 0) {
            TRice64("sig:%.1f s\n", aDouble(time_seconds));
        }
        /*#
         * aFloat() transfers the IEEE-754 float bit pattern as a 32-bit Trice
         * value. The decoder uses the three %f specifiers from til.json.
         */
        TRice("vis_demo:%f,%f,%f\n", aFloat((float)x), aFloat((float)y), aFloat((float)z));

        if (!output_failed &&
            options.delay &&
            (options.sample_count == 0 ||
             sample + 1 < options.sample_count)) {
            sleep_seconds(period_seconds);
        }
    }

    udp_sender_close(&active_udp_sender);

    if (active_stream != NULL &&
        active_stream != stdout &&
        fclose(active_stream) != 0) {
        fprintf(stderr, "Error while closing binary output file\n");
        output_failed = true;
    }

    return output_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
