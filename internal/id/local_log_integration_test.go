// SPDX-License-Identifier: MIT

package id

import (
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"sort"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestLocalLogCompilesAndRunsWithBothDeferredBuffers exercises the public
// target API against the real Trice producer, parser, resolver, formatter, and
// both zero-copy buffer adapters. It intentionally uses no generated sidecar;
// generator selection is tested independently above.
func TestLocalLogCompilesAndRunsWithBothDeferredBuffers(t *testing.T) {
	compiler := firstAvailableCompiler("cc", "gcc", "clang")
	if compiler == "" {
		t.Skip("no C compiler available")
	}
	repositoryRoot, err := filepath.Abs(filepath.Join("..", ".."))
	require.NoError(t, err)
	sourceDirectory := filepath.Join(repositoryRoot, "src")
	librarySources, err := filepath.Glob(filepath.Join(sourceDirectory, "[a-z]*.c"))
	require.NoError(t, err)
	sort.Strings(librarySources)

	for _, buffer := range []struct {
		name  string
		value string
	}{{name: "ring", value: "TRICE_RING_BUFFER"}, {name: "double", value: "TRICE_DOUBLE_BUFFER"}} {
		buffer := buffer
		t.Run(buffer.name, func(t *testing.T) {
			project := t.TempDir()
			config := `// SPDX-License-Identifier: MIT
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_BUFFER ` + buffer.value + `
#define TRICE_DEFERRED_BUFFER_SIZE 1024
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_LOCAL_LOG 1
#define TRICE_DIRECT_OUTPUT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_CONFIG_WARNINGS 0
#endif
`
			table := `// SPDX-License-Identifier: MIT
#include "triceRx.h"
const triceLog_t triceLog[] = {
    { 1000u, 32u, 2u, "minimal=%d/%x\n" },
    { 1001u, 32u, 1u, "hook=%04x\n" },
    { 1002u, 32u, 0u, "after-invalid\n" },
    { 1003u, 32u, 0u, "too-small\n" },
    { 1004u, 32u, 1u, "minimal-width=%04x\n" },
    { 1005u, 32u, 0u, "after-format\n" },
    { 1006u, 32u, 0u, "trailing=%" },
    { 1007u, 64u, 2u, "wide=%016x signed=%d\n" },
    { 1008u, 32u, 2u, "stale=%d\n" },
};
const unsigned triceLogElements = sizeof(triceLog) / sizeof(triceLog[0]);
`
			mainSource := `// SPDX-License-Identifier: MIT
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "trice.h"

static int check(int condition) { return condition ? 0 : 1; }
static int hookCalls;
// countingSnprintf proves that TriceLog invokes its hook once per scalar while
// retaining ordinary snprintf return and truncation semantics.
static int countingSnprintf(char *dst, size_t size, const char *format, ...) {
    va_list arguments;
    va_start(arguments, format);
    int result = vsnprintf(dst, size, format, arguments);
    va_end(arguments);
    hookCalls++;
    return result;
}

int main(void) {
    char text[64];
    char one[1];
    int result;

    trice(iD(1000), "minimal=%d/%x\n", -7, 0x2a);
    result = TriceLog(text, sizeof(text));
    if (check(result == 14 && strcmp(text, "minimal=-7/2a\n") == 0)) return 10;

    UserTriceLogPrintfFn = countingSnprintf;
    trice(iD(1001), "hook=%04x\n", 0x2a);
    result = TriceLog(text, sizeof(text));
    if (check(result == 10 && strcmp(text, "hook=002a\n") == 0 && hookCalls == 1)) return 11;

    trice(iD(1002), "after-invalid\n");
    if (check(TriceLog(NULL, sizeof(text)) == TRICE_LOG_ERR_ARGUMENT)) return 12;
    result = TriceLog(text, sizeof(text));
    if (check(result == 14 && strcmp(text, "after-invalid\n") == 0)) return 13;

    trice(iD(1003), "too-small\n");
    if (check(TriceLog(one, sizeof(one)) == TRICE_LOG_ERR_OUTPUT_TOO_SMALL && one[0] == 0)) return 14;
    if (check(TriceLog(text, sizeof(text)) == 0 && text[0] == 0)) return 15;

    UserTriceLogPrintfFn = NULL;
    trice(iD(1004), "minimal-width=%04x\n", 0x2a);
    trice(iD(1005), "after-format\n");
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_FORMAT && text[0] == 0)) return 16;
    result = TriceLog(text, sizeof(text));
    if (check(result == 13 && strcmp(text, "after-format\n") == 0)) return 17;

    trice(iD(1006), "trailing=%");
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_FORMAT && text[0] == 0)) return 18;

    trice(iD(1999), "missing-id\n");
    trice(iD(1005), "after-format\n");
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_ID && text[0] == 0)) return 19;
    result = TriceLog(text, sizeof(text));
    if (check(result == 13 && strcmp(text, "after-format\n") == 0)) return 20;

    trice(iD(1008), "stale=%d\n", 7);
    trice(iD(1005), "after-format\n");
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_RECORD && text[0] == 0)) return 21;
    if (check(TriceLog(text, sizeof(text)) == 0 && text[0] == 0)) return 22;

    UserTriceLogPrintfFn = countingSnprintf;
    trice64(iD(1007), "wide=%016x signed=%d\n", UINT64_C(0x1234), -INT64_C(9));
    result = TriceLog(text, sizeof(text));
    if (check(result == 32 && strcmp(text, "wide=0000000000001234 signed=-9\n") == 0 && hookCalls == 3)) return 23;
    return 0;
}
`
			require.NoError(t, os.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(config), 0o644))
			require.NoError(t, os.WriteFile(filepath.Join(project, "til.c"), []byte(table), 0o644))
			require.NoError(t, os.WriteFile(filepath.Join(project, "main.c"), []byte(mainSource), 0o644))

			executable := filepath.Join(project, "local_log")
			if runtime.GOOS == "windows" {
				executable += ".exe"
			}
			arguments := []string{"-std=c11", "-Wall", "-Wextra", "-Werror", "-I", project, "-I", sourceDirectory, filepath.Join(project, "main.c"), filepath.Join(project, "til.c")}
			arguments = append(arguments, librarySources...)
			arguments = append(arguments, "-o", executable)
			output, compileErr := exec.Command(compiler, arguments...).CombinedOutput()
			require.NoError(t, compileErr, "%s", output)

			output, runErr := exec.Command(executable).CombinedOutput()
			assert.NoError(t, runErr, "%s", output)
		})
	}
}

// TestLocalLogRejectsDisabledRxSupport verifies that a contradictory legacy
// RX override fails at the configuration boundary with an actionable message,
// rather than much later as a collection of missing parser declarations.
func TestLocalLogRejectsDisabledRxSupport(t *testing.T) {
	compiler := firstAvailableCompiler("cc", "gcc", "clang")
	if compiler == "" {
		t.Skip("no C compiler available")
	}
	repositoryRoot, err := filepath.Abs(filepath.Join("..", ".."))
	require.NoError(t, err)
	project := t.TempDir()
	config := `// SPDX-License-Identifier: MIT
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_LOCAL_LOG 1
#define TRICE_RX_SUPPORT 0
#endif
`
	mainSource := `// SPDX-License-Identifier: MIT
#include "trice.h"
int main(void) { return 0; }
`
	require.NoError(t, os.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(config), 0o644))
	require.NoError(t, os.WriteFile(filepath.Join(project, "main.c"), []byte(mainSource), 0o644))
	object := filepath.Join(project, "main.o")
	output, compileErr := exec.Command(
		compiler,
		"-std=c11",
		"-I", project,
		"-I", filepath.Join(repositoryRoot, "src"),
		"-c", filepath.Join(project, "main.c"),
		"-o", object,
	).CombinedOutput()
	require.Error(t, compileErr)
	assert.Contains(t, string(output), "TRICE_LOCAL_LOG == 1 needs TRICE_RX_LOG_SUPPORT == 1")
}

// TestLocalLogMinimalFormatterCompilesOut checks the no-LTO size contract of
// the separate fallback translation unit. With the switch disabled, even a
// direct object-file build must expose no formatter function or data symbol.
func TestLocalLogMinimalFormatterCompilesOut(t *testing.T) {
	compiler := firstAvailableCompiler("cc", "gcc", "clang")
	if compiler == "" {
		t.Skip("no C compiler available")
	}
	nm, err := exec.LookPath("nm")
	if err != nil {
		t.Skip("no object-file symbol reader named nm available")
	}
	repositoryRoot, err := filepath.Abs(filepath.Join("..", ".."))
	require.NoError(t, err)
	project := t.TempDir()
	config := `// SPDX-License-Identifier: MIT
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_LOCAL_LOG 1
#define TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER 0
#define TRICE_DIRECT_OUTPUT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_CONFIG_WARNINGS 0
#endif
`
	require.NoError(t, os.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(config), 0o644))
	object := filepath.Join(project, "triceLogMinimal.o")
	output, compileErr := exec.Command(
		compiler,
		"-std=c11", "-Wall", "-Wextra", "-Werror",
		"-I", project,
		"-I", filepath.Join(repositoryRoot, "src"),
		"-c", filepath.Join(repositoryRoot, "src", "triceLogMinimal.c"),
		"-o", object,
	).CombinedOutput()
	require.NoError(t, compileErr, "%s", output)

	symbols, symbolErr := exec.Command(nm, object).CombinedOutput()
	require.NoError(t, symbolErr, "%s", symbols)
	assert.NotContains(t, string(symbols), "triceLogMinimal")
}
