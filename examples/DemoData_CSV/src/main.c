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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
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

#define DEMO_PI 3.14159265358979323846
#define CSV_LINE_CAPACITY 160

/*
 * Exactly one output destination is active. stdout is the convenient default,
 * a file is useful for playback, and UDP is useful for a live plot.
 */
typedef enum {
	OUTPUT_STDOUT,
	OUTPUT_FILE,
	OUTPUT_UDP
} OutputKind;

typedef struct {
	/* Number of generated records per second. */
	double rate_hz;

	/* Zero means "continue until interrupted with Ctrl+C". */
	unsigned long long sample_count;

	/* Skip real-time waiting when false, for fast file generation and tests. */
	bool delay;

	/* A CSV header is optional because live receivers usually expect numbers. */
	bool header;

	/* Selected output and its optional command-line parameters. */
	OutputKind output_kind;
	const char* file_name;
	const char* udp_host;
	const char* udp_port;
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

static void print_usage(const char* program) {
	printf(
	    "Usage: %s [options]\n"
	    "\n"
	    "Generate time_s,x,y,z records as newline-terminated CSV.\n"
	    "\n"
	    "Output options (choose at most one):\n"
	    "  -o, --output FILE     Write CSV to FILE\n"
	    "      --udp HOST PORT   Send one CSV record per UDP datagram\n"
	    "                         Example: --udp 127.0.0.1 9000\n"
	    "  No output option writes CSV to stdout.\n"
	    "\n"
	    "Generator options:\n"
	    "  -r, --rate HZ         Sample rate (default: 50)\n"
	    "  -n, --samples N       Stop after N samples (default: unlimited)\n"
	    "      --header          Write/send the CSV column names first\n"
	    "      --no-delay        Generate as fast as possible\n"
	    "  -h, --help            Show this help\n",
	    program);
}

/*
 * Convert a complete string to double. Checking both errno and the final
 * character rejects range errors as well as partial input such as "50Hz".
 */
static bool parse_double(const char* text, double* value) {
	char* end = NULL;
	errno = 0;
	*value = strtod(text, &end);
	return errno == 0 && end != text && *end == '\0';
}

/* Apply the same strict parsing rule to the unsigned sample count. */
static bool parse_unsigned_long_long(
    const char* text,
    unsigned long long* value) {
	char* end = NULL;

	/* strtoull() accepts a leading minus sign, which is not useful here. */
	if (text[0] == '-') {
		return false;
	}

	errno = 0;
	*value = strtoull(text, &end, 10);
	return errno == 0 && end != text && *end == '\0';
}

/*
 * Verify that an option has the requested number of following arguments before
 * parse_options() advances through argv.
 */
static bool require_values(
    int current_index,
    int argument_count,
    int required_count,
    const char* option) {
	if (current_index + required_count < argument_count) {
		return true;
	}

	fprintf(stderr, "Missing value for %s\n", option);
	return false;
}

/*
 * The program deliberately uses a small hand-written parser. This keeps the
 * executable independent of platform-specific option-parsing libraries.
 */
static bool parse_options(int argc, char** argv, Options* options) {
	bool destination_selected = false;

	for (int i = 1; i < argc; ++i) {
		const char* argument = argv[i];

		if (strcmp(argument, "-h") == 0 ||
		    strcmp(argument, "--help") == 0) {
			print_usage(argv[0]);
			exit(EXIT_SUCCESS);
		} else if (strcmp(argument, "--header") == 0) {
			options->header = true;
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
			options->output_kind = OUTPUT_FILE;
			options->file_name = argv[++i];
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

/*
 * Pause between samples. The rest of the program works in seconds, so both
 * operating-system-specific APIs are hidden behind this common interface.
 */
static void sleep_seconds(double seconds) {
#ifdef _WIN32
	DWORD milliseconds = (DWORD)(seconds * 1000.0 + 0.5);
	Sleep(milliseconds > 0 ? milliseconds : 1);
#else
	struct timespec remaining;
	remaining.tv_sec = (time_t)seconds;
	remaining.tv_nsec =
	    (long)((seconds - (double)remaining.tv_sec) * 1.0e9);

	while (nanosleep(&remaining, &remaining) != 0 && errno == EINTR) {
		/* Resume sleeping for the time left after an interrupt. */
	}
#endif
}

/*
 * Calculate the three synthetic sensor values.
 *
 * t_seconds is intentionally a double in seconds. Frequencies can therefore be
 * written directly in hertz. x and y form a slowly changing Lissajous figure
 * in an XY plot. z combines a slower wave with x*y and a short periodic pulse,
 * which makes it visually distinct in a time plot.
 */
static void generate_values(
    double t_seconds,
    double* x,
    double* y,
    double* z) {
	/*
	 * Modulate y slowly so the Lissajous trace keeps changing visibly even
	 * after the plot has collected enough samples to cover the full figure.
	 */
	double phase_drift = (DEMO_PI / 3.0) * sin(2.0 * DEMO_PI * 0.04 * t_seconds);

	*x = sin(2.0 * DEMO_PI * 0.70 * t_seconds);
	*y = sin(2.0 * DEMO_PI * 0.91 * t_seconds + DEMO_PI / 2.0 + phase_drift);

	/* Add a 250 ms pulse at the end of every eight-second interval. */
	double cycle_position = fmod(t_seconds, 8.0);
	double pulse = cycle_position >= 7.75 ? 0.8 : 0.0;

	*z = 0.6 * sin(2.0 * DEMO_PI * 0.13 * t_seconds) + 0.2 * *x * *y + pulse;
}

/* Return the platform-specific socket error as a printable integer. */
static int socket_error_code(void) {
#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}

/* Close the socket and release Winsock state when necessary. */
static void udp_sender_close(UdpSender* sender) {
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

/*
 * Resolve HOST and PORT, then retain the first usable UDP address. getaddrinfo
 * supports names such as "localhost" as well as numeric IPv4/IPv6 addresses.
 */
static bool udp_sender_open(
    UdpSender* sender,
    const char* host,
    const char* port) {
	struct addrinfo hints;
	struct addrinfo* addresses = NULL;
	struct addrinfo* candidate = NULL;

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

/* Send one complete newline-terminated CSV record as one UDP datagram. */
static bool udp_sender_send(
    const UdpSender* sender,
    const char* data,
    size_t length) {
#ifdef _WIN32
	int sent = sendto(
	    sender->socket,
	    data,
	    (int)length,
	    0,
	    (const struct sockaddr*)&sender->address,
	    sender->address_length);
	bool complete = sent == (int)length;
#else
	ssize_t sent = sendto(
	    sender->socket,
	    data,
	    length,
	    0,
	    (const struct sockaddr*)&sender->address,
	    sender->address_length);
	bool complete = sent == (ssize_t)length;
#endif

	if (!complete) {
		fprintf(stderr, "UDP send failed: %d\n", socket_error_code());
	}
	return complete;
}

/*
 * Send text to the selected destination. Files and stdout are flushed after
 * every record so a following process or live viewer sees data immediately.
 */
static bool write_text(
    const Options* options,
    FILE* stream,
    const UdpSender* udp_sender,
    const char* text) {
	size_t length = strlen(text);

	if (options->output_kind == OUTPUT_UDP) {
		return udp_sender_send(udp_sender, text, length);
	}

	if (fwrite(text, 1, length, stream) != length) {
		fprintf(stderr, "Output write failed\n");
		return false;
	}

	if (fflush(stream) != 0) {
		fprintf(stderr, "Output flush failed\n");
		return false;
	}
	return true;
}

int main(int argc, char** argv) {
	/*
	 * These defaults provide a smooth live plot without placing a noticeable
	 * load on the host computer.
	 */
	Options options = {
	    .rate_hz = 50.0,
	    .sample_count = 0,
	    .delay = true,
	    .header = false,
	    .output_kind = OUTPUT_STDOUT,
	    .file_name = NULL,
	    .udp_host = NULL,
	    .udp_port = NULL};

	if (!parse_options(argc, argv, &options)) {
		fprintf(stderr, "Try '%s --help'.\n", argv[0]);
		return EXIT_FAILURE;
	}

	FILE* stream = stdout;
	UdpSender udp_sender;
	memset(&udp_sender, 0, sizeof(udp_sender));
	udp_sender.socket = INVALID_SOCKET_HANDLE;

	if (options.output_kind == OUTPUT_FILE) {
		/*
		 * "w" starts a fresh CSV file on each invocation. Text mode provides
		 * the operating system's conventional newline representation.
		 */
		stream = fopen(options.file_name, "w");
		if (stream == NULL) {
			fprintf(stderr, "Cannot open '%s': %s\n",
			        options.file_name, strerror(errno));
			return EXIT_FAILURE;
		}
	} else if (options.output_kind == OUTPUT_UDP) {
		if (!udp_sender_open(
		        &udp_sender,
		        options.udp_host,
		        options.udp_port)) {
			return EXIT_FAILURE;
		}
	}

	bool success = true;
	if (options.header) {
		success = write_text(
		    &options,
		    stream,
		    &udp_sender,
		    "time_s,x,y,z\n");
	}

	/*
	 * Derive time from the integer sample index rather than repeatedly adding
	 * the period. This avoids an accumulating floating-point rounding error.
	 */
	const double period_seconds = 1.0 / options.rate_hz;
	for (unsigned long long sample = 0;
	     success &&
	     (options.sample_count == 0 || sample < options.sample_count);
	     ++sample) {
		double time_seconds = (double)sample * period_seconds;
		double x;
		double y;
		double z;
		generate_values(time_seconds, &x, &y, &z);

		char line[CSV_LINE_CAPACITY];
		int length = snprintf(
		    line,
		    sizeof(line),
		    "%.6f,%.6f,%.6f,%.6f\n",
		    time_seconds,
		    x,
		    y,
		    z);

		if (length < 0 || (size_t)length >= sizeof(line)) {
			fprintf(stderr, "CSV line buffer is too small\n");
			success = false;
			break;
		}

		success = write_text(&options, stream, &udp_sender, line);

		if (success &&
		    options.delay &&
		    (options.sample_count == 0 ||
		     sample + 1 < options.sample_count)) {
			sleep_seconds(period_seconds);
		}
	}

	udp_sender_close(&udp_sender);

	if (stream != stdout && fclose(stream) != 0) {
		fprintf(stderr, "Error while closing output file\n");
		success = false;
	}

	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
