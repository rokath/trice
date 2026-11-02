// SPDX-License-Identifier: MIT

// Hardware-free alias/CE sender. The Go fixture supplies triceConfig.h, IDs
// and the local decoding table; the project macros still execute normally.
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
#include "trice.h"
#include "triceRx.h"
#include "trice_sidecar_aliasContext_c_0BF14DF4796F231C.h" // trice-bind: keep as last include before this file's Trice calls

static unsigned originalEvaluations, contextEvaluations, scalarCalls, printfCalls;
static unsigned records, failures;

// TRICE_INSERT_OFF
#define MyWrn(...)          \
	do {                    \
		scalarCalls++;      \
		trice(__VA_ARGS__); \
	} while (0)
#define PRINTF_WITH_ID(id, ...)                    \
	do {                                           \
		char text[96];                             \
		snprintf(text, sizeof(text), __VA_ARGS__); \
		printfCalls++;                             \
		TRICE_INSERT_triceS(id, "%s", text);       \
	} while (0)
#define PrintfS(...) TRICE_BIND_DISPATCH(TRICE_BIND_ROUTE(TRICE_BIND_FILE_KEY), PRINTF_WITH_ID, __VA_ARGS__)
#define DirectS(...) triceS(__VA_ARGS__)
// TRICE_INSERT_ON

// originalValue and the CE counter prove single evaluation at the call site.
static unsigned originalValue(void) {
	originalEvaluations++;
	return 7;
}

#if TRICE_LOCAL_LOG == 0
// Validate generated payload metadata before forwarding the actual wire bytes.
static void capture(const uint8_t* data, size_t size) {
	triceRx_t rx;
	int consumed = TriceParseRecord(&rx, data, size);
	if (consumed <= 0 || (((size_t)consumed + 3u) & ~(size_t)3u) != size ||
	    TriceResolveLog(&rx, triceLog, triceLogElements) != TRICE_RX_RESULT_OK) {
		failures++;
		return;
	}
	fwrite(data, 1, size, stdout);
	records++;
}
#endif

// The local variant checks complete output independently of the host formatter.
static void checkLocal(const char* expected) {
#if TRICE_LOCAL_LOG == 1
	char text[160];
	if (TriceLog(text, sizeof(text)) <= 0 || strcmp(text, expected) != 0) {
		fprintf(stderr, "local output differs: got <%s>, expected <%s>\n", text, expected);
		failures++;
	}
	records++;
#else
	(void)expected;
#endif
}

int main(void) {
#ifdef _WIN32
	_setmode(_fileno(stdout), _O_BINARY);
#endif
#if TRICE_LOCAL_LOG == 0
	UserNonBlockingDirectWrite8AuxiliaryFn = capture;
#endif
	MyWrn("err:value={value}\n", originalValue());
	checkLocal("err:value=7 context=1\n");
	triceS("msg:native:{device:%s}\n", "pump");
	checkLocal("msg:pump [checked]\n");
	PrintfS("info:formatted=%d/%s\n", 9, "ok");
	checkLocal("info:formatted=9/ok\n");
	DirectS("%s", "wrn:payload\n");
	checkLocal("wrn:payload\n");
	return failures != 0 || records != 4 || originalEvaluations != 1 ||
	       contextEvaluations != 1 || scalarCalls != 1 || printfCalls != 1;
}
