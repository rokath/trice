// SPDX-License-Identifier: MIT

/* Exercise portable C99 calls without Microsoft's replacement-API warnings.
 * Keep -Werror active for compiler diagnostics and all other warnings. */
#if defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS 1
#endif

/* Keep the real POSIX adapters available when the Windows branch is simulated. */
#if defined(__APPLE__)
#define _DARWIN_C_SOURCE
#elif !defined(_WIN32) || defined(BCSIM_TEST_WINDOWS_SHIM)
#define _POSIX_C_SOURCE 200809L
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if defined(_WIN32)
#include <direct.h>
#include <windows.h>
#endif
#if !defined(_WIN32) || defined(BCSIM_TEST_WINDOWS_SHIM)
#include <sys/stat.h>
#include <unistd.h>
#endif

/* Faults affect only the requested filesystem operation, never other I/O. */
static int failMkdir;
static int failRmdir;
static int failSeek;
static int contendOnce;
static unsigned mkdirCalls;
/* Inject finite denials separately from permanent errors and ordinary contention. */
static unsigned denialsRemaining;
static int mkdirErrno = EACCES;
static unsigned long mkdirNativeError = 5u;
/* Record the wait budget without real sleeps or timing-sensitive assertions. */
static unsigned sleepCalls, sleptMs, firstSleepMs, lastSleepMs;

#if defined(_WIN32)
int bcSimProbeMkdir(const char* path);
DWORD WINAPI bcSimProbeProcessId(void);
void WINAPI bcSimProbeSleep(DWORD ms);
#define _mkdir bcSimProbeMkdir
#define _rmdir bcSimProbeRmdir
#define GetCurrentProcessId bcSimProbeProcessId
#define Sleep bcSimProbeSleep
#else
int bcSimProbeMkdir(const char* path, mode_t mode);
pid_t bcSimProbeProcessId(void);
int bcSimProbeSleep(const struct timespec* delay, struct timespec* remaining);
#define mkdir bcSimProbeMkdir
#define rmdir bcSimProbeRmdir
#define getpid bcSimProbeProcessId
#define nanosleep bcSimProbeSleep
#endif
int bcSimProbeRmdir(const char* path);
int bcSimProbeSeek(FILE* stream, long offset, int origin);
#define fseek bcSimProbeSeek
/* Exercise complete 100 ms retries and a shorter final wait in one shared budget. */
#define BCSIM_LOCK_POLL_MS 10u
#define BCSIM_LOCK_WAIT_MS 250u
#include "../../examples/TriceAbc/BcSim/BcSim.c"
#undef fseek
#undef mkdir
#undef rmdir
#undef getpid
#undef _mkdir
#undef _rmdir
#undef GetCurrentProcessId
#undef Sleep
#undef nanosleep

#if defined(BCSIM_TEST_WINDOWS_SHIM)
/* These are adapter doubles, not an emulation of Windows filesystem behavior. */
unsigned long bcSimProbeDosError;
static DWORD lastWindowsError;
DWORD GetLastError(void) { return lastWindowsError; }
void SetLastError(DWORD value) { lastWindowsError = value; }
#endif

/* Set both CRT and native error state before returning from an injected fault. */
static void setProbeError(int error, unsigned long nativeError) {
	errno = error;
#if defined(_WIN32)
	_doserrno = nativeError;
	SetLastError((DWORD)nativeError);
#else
	(void)nativeError;
#endif
}

/* Sleep may overwrite error state; only the final failed mkdir may be reported. */
static void recordSleep(unsigned ms) {
	if (sleepCalls == 0u)
		firstSleepMs = ms;
	++sleepCalls;
	sleptMs += ms;
	lastSleepMs = ms;
	setProbeError(EDOM, 999u);
}

#if defined(_WIN32)
void WINAPI bcSimProbeSleep(DWORD ms) {
	recordSleep((unsigned)ms);
}
#else
int bcSimProbeSleep(const struct timespec* delay, struct timespec* remaining) {
	(void)remaining;
	recordSleep((unsigned)delay->tv_sec * 1000u + (unsigned)(delay->tv_nsec / 1000000L));
	return 0;
}
#endif

/* Use the host filesystem behind either the native or simulated adapter. */
static int realMkdir(const char* path) {
#if defined(_WIN32) && !defined(BCSIM_TEST_WINDOWS_SHIM)
	return _mkdir(path);
#else
	return mkdir(path, 0777);
#endif
}

#if defined(_WIN32)
int bcSimProbeMkdir(const char* path) {
#else
int bcSimProbeMkdir(const char* path, mode_t mode) {
	(void)mode;
#endif
	++mkdirCalls;
	if (contendOnce) {
		contendOnce = 0;
		setProbeError(EEXIST, 183u);
		return -1;
	}
	if (failMkdir || denialsRemaining != 0u) {
		if (denialsRemaining != 0u)
			--denialsRemaining;
		setProbeError(mkdirErrno, mkdirNativeError);
		return -1;
	}
	return realMkdir(path);
}

/* An unsuccessful release must not turn into a successful public open. */
int bcSimProbeRmdir(const char* path) {
	if (failRmdir) {
		setProbeError(EBUSY, 32u);
		return -1;
	}
#if defined(_WIN32) && !defined(BCSIM_TEST_WINDOWS_SHIM)
	return _rmdir(path);
#else
	return rmdir(path);
#endif
}

/* Preserve the original I/O failure when releasing that operation's lock fails. */
int bcSimProbeSeek(FILE* stream, long offset, int origin) {
	if (failSeek) {
		errno = EIO;
		return -1;
	}
	return fseek(stream, offset, origin);
}

/* Deliberately clobber error state after capture to verify diagnostic snapshots. */
#if defined(_WIN32)
DWORD WINAPI bcSimProbeProcessId(void) {
#else
pid_t bcSimProbeProcessId(void) {
#endif
	setProbeError(EDOM, 999u);
	return 4242;
}

/* Run one public API scenario; every invocation owns a fresh working directory. */
int main(int argc, char** argv) {
	BcSim_t io;
	const uint8_t payload[] = {0x41, 0x00, 0xff};
	uint8_t received[3];
	int result;
	int expected = BCSIM_OK;
	int expectedErrno = 0;
	int savedErrno;
	/* A transient write failure must recover without writing the payload twice. */
	int payloadWritten = 0;
	int writeScenario;
	int deniedScenario;
#if defined(_WIN32)
	DWORD savedWindowsError;
	unsigned long savedDosError;
	unsigned long expectedNativeError = 0;
#endif
	if (argc != 2)
		return 90;
	writeScenario = strcmp(argv[1], "write_error") == 0 || strcmp(argv[1], "transient_write_denied") == 0;
	deniedScenario = strcmp(argv[1], "transient_mkdir_denied") == 0 || strcmp(argv[1], "denied_until_deadline") == 0;
	failMkdir = strcmp(argv[1], "mkdir_error") == 0 || strcmp(argv[1], "contention_then_denied") == 0 ||
	            strcmp(argv[1], "other_native_error") == 0 || strcmp(argv[1], "other_errno") == 0;
	if (strcmp(argv[1], "other_native_error") == 0)
		mkdirNativeError = 32u;
	if (strcmp(argv[1], "other_errno") == 0)
		mkdirErrno = EIO;
	if (deniedScenario)
		denialsRemaining = strcmp(argv[1], "denied_until_deadline") == 0 ? 3u : 1u;
	failRmdir = strcmp(argv[1], "unlock_error") == 0 || strcmp(argv[1], "header_and_unlock_error") == 0;
	failSeek = strcmp(argv[1], "header_and_unlock_error") == 0;
	contendOnce = strcmp(argv[1], "contention") == 0 || strcmp(argv[1], "contention_then_denied") == 0;
	if (strcmp(argv[1], "timeout") == 0 && realMkdir("bc.bus.lock") != 0)
		return 91;
	result = bcSimOpen(&io, "bc.bus", "bc.log", "probe participant");
	if (writeScenario) {
		if (result != BCSIM_OK)
			return 92;
		failMkdir = strcmp(argv[1], "write_error") == 0;
		denialsRemaining = failMkdir ? 0u : 1u;
		result = bcSimWrite(&io, payload, sizeof(payload), "probe");
		if (result == (int)sizeof(payload)) {
			payloadWritten = 1;
			result = BCSIM_OK;
		}
	}
#if defined(_WIN32)
	savedWindowsError = GetLastError();
	savedDosError = _doserrno;
#endif
	savedErrno = errno;
	if (failMkdir || failRmdir)
		expected = BCSIM_ERR_LOCK;
#if !defined(_WIN32)
	/* POSIX access errors remain fatal, even if a later call would succeed. */
	if (deniedScenario || strcmp(argv[1], "transient_write_denied") == 0)
		expected = BCSIM_ERR_LOCK;
#endif
	if (failSeek)
		expected = BCSIM_ERR_IO;
	if (strcmp(argv[1], "timeout") == 0)
		expected = BCSIM_ERR_LOCK_TIMEOUT;
	if (expected == BCSIM_ERR_LOCK && !failRmdir)
		expectedErrno = mkdirErrno;
	if (failRmdir)
		expectedErrno = EBUSY;
#if defined(_WIN32)
	if (failMkdir)
		expectedNativeError = mkdirNativeError;
	if (failRmdir)
		expectedNativeError = 32u;
	if (expectedNativeError && (savedDosError != expectedNativeError || savedWindowsError != expectedNativeError))
		return 93;
#endif
	printf("result=%d expected=%d is_open=%d errno=%d expected_errno=%d mkdir_calls=%u sleep_calls=%u slept_ms=%u first_sleep_ms=%u last_sleep_ms=%u\n",
	       result, expected, io.isOpen, savedErrno, expectedErrno, mkdirCalls, sleepCalls, sleptMs, firstSleepMs, lastSleepMs);
	if (result != expected || (expectedErrno && savedErrno != expectedErrno))
		return 94;
	if (expected == BCSIM_OK) {
		if (!io.isOpen || (!payloadWritten && bcSimWrite(&io, payload, sizeof(payload), "probe") != 3))
			return 95;
		if (bcSimRead(&io, received, sizeof(received), "probe") != 0)
			return 96;
	} else if (!writeScenario && io.isOpen) {
		return 97;
	}
	bcSimClose(&io);
	return 0;
}
