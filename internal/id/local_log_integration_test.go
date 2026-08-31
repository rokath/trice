// SPDX-License-Identifier: MIT

package id

import (
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"sort"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestLocalLogCompilesAndRunsWithBothDeferredBuffers exercises the public
// target API against the real Trice producer, parser, resolver, formatter, and
// both zero-copy buffer adapters. Ring-buffer runs in both transfer byte orders
// also verify integer and floating-point reconstruction. It intentionally uses
// no generated sidecar; generator selection is tested independently above.
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
		name          string
		value         string
		transferOrder string
	}{
		{name: "ring-little-endian", value: "TRICE_RING_BUFFER", transferOrder: "0"},
		{name: "ring-big-endian", value: "TRICE_RING_BUFFER", transferOrder: "1"},
		{name: "double-little-endian", value: "TRICE_DOUBLE_BUFFER", transferOrder: "0"},
	} {
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
#define TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS 1
#define TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS 1
#define TRICE_LOCAL_LOG_USE_BUFFER_TRICES 1
#define TRICE_LOCAL_LOG_USE_PREFIX_HOOK 1
#define TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN ` + buffer.transferOrder + `
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
    { 1009u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "string=[%s] percent=%%\n" },
    { 1010u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "right=[%8.3s]\n" },
    { 1011u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "left=[%-6.3s]\n" },
    { 1012u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "embedded=[%s]\n" },
    { 1013u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "empty=<%s>\n" },
    { 1014u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "twice=%s/%s\n" },
    { 1015u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "dynamic=%02x\n" },
    { 1016u, 32u, 0u, "after-string-error\n" },
    { 1017u, 32u, 2u, "float=%+.2f sci=%.1e\n" },
    { 1018u, 64u, 2u, "double=%.6f compact=%.4g\n" },
    { 1019u, 16u, 1u, "bad-float=%f\n" },
    { 1020u, 64u, 1u, "long-float=%.2Lf\n" },
    { 1021u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "small=[%s]\n" },
    { 1022u, 32u, 1u, "scalar-string=%s\n" },
    { 1023u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "bad-flag=%+s\n" },
    { 1024u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "precision-zero=[%.s]\n" },
    { 1025u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "raw=[%s]\n" },
    { 1026u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "overflow=[%999999999999999999999999999999999999999s]\n" },
    { 1027u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "dynamic-width=[%*s]\n" },
    { 1028u,  8u, 5u, "special=%b/%#b/%O/%t/%p\n" },
    { 1029u,  8u, 1u, "quote=%q\n" },
    { 1030u,  8u, TRICE_LOG_PARAM_COUNT_DYNAMIC_STRING, "quoted=%q\n" },
    { 1031u, 16u, TRICE_LOG_PARAM_COUNT_DYNAMIC_BUFFER, "buffer:%04x \n" },
    { 1032u, 32u, 0u, NULL },
    { 1033u, 32u, 0u, "prefixed\n" },
};
const unsigned triceLogElements = sizeof(triceLog) / sizeof(triceLog[0]);
`
			mainSource := `// SPDX-License-Identifier: MIT
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "trice.h"

static int check(int condition) { return condition ? 0 : 1; }
// checkText keeps individual cases focused on observable snprintf-compatible
// length and content rather than duplicating manually counted byte totals.
static int checkText(int result, const char *actual, const char *expected) {
    return result == (int)strlen(expected) && strcmp(actual, expected) == 0 ? 0 : 1;
}

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
// idPrefix demonstrates that presentation policy is optional and receives
// parsed metadata without changing record decoding or generated strings.
static int idPrefix(char *dst, size_t size, uint16_t id, uint8_t stampBits, uint32_t stamp) {
    (void)stampBits;
    (void)stamp;
    return snprintf(dst, size, "[%u] ", (unsigned)id);
}
// failingPrefix verifies error mapping and the no-partial-output contract.
static int failingPrefix(char *dst, size_t size, uint16_t id, uint8_t stampBits, uint32_t stamp) {
    (void)dst; (void)size; (void)id; (void)stampBits; (void)stamp;
    return -1;
}
// longPrefix reports snprintf's required length to exercise prefix truncation.
static int longPrefix(char *dst, size_t size, uint16_t id, uint8_t stampBits, uint32_t stamp) {
    (void)id; (void)stampBits; (void)stamp;
    return snprintf(dst, size, "prefix-too-long");
}

int main(void) {
    char text[64];
    char one[1];
    char small[8];
    const char noTerminator[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    const char embeddedNul[] = {'A', 0, 'B'};
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

    UserTriceLogPrintfFn = NULL;
    triceS(iD(1009), "string=[%s] percent=%%\n", "runtime");
    result = TriceLog(text, sizeof(text));
    if (check(checkText(result, text, "string=[runtime] percent=%\n") == 0 && hookCalls == 3)) return 24;

    // Queue the following scalar before consuming the four-byte string. Its
    // non-zero ID header begins immediately after the aligned string record,
    // proving that %s cannot depend on finding a convenient padding NUL.
    triceN(iD(1025), "raw=[%s]\n", noTerminator, 4u);
    trice(iD(1016), "after-string-error\n");
    result = TriceLog(text, sizeof(text));
    if (checkText(result, text, "raw=[abcd]\n")) return 25;
    result = TriceLog(text, sizeof(text));
    if (checkText(result, text, "after-string-error\n")) return 26;

    // triceN proves that local string formatting uses the encoded byte count;
    // this six-byte payload deliberately has no accessible terminator.
    triceN(iD(1010), "right=[%8.3s]\n", noTerminator, sizeof(noTerminator));
    result = TriceLog(text, sizeof(text));
    if (checkText(result, text, "right=[     abc]\n")) return 27;

    triceN(iD(1011), "left=[%-6.3s]\n", noTerminator, sizeof(noTerminator));
    result = TriceLog(text, sizeof(text));
    if (checkText(result, text, "left=[abc   ]\n")) return 28;

    triceN(iD(1012), "embedded=[%s]\n", embeddedNul, sizeof(embeddedNul));
    result = TriceLog(text, sizeof(text));
    if (checkText(result, text, "embedded=[A]\n")) return 29;

    triceS(iD(1013), "empty=<%s>\n", "");
    result = TriceLog(text, sizeof(text));
    if (checkText(result, text, "empty=<>\n")) return 30;

    triceS(iD(1024), "precision-zero=[%.s]\n", "not visible");
    result = TriceLog(text, sizeof(text));
    if (checkText(result, text, "precision-zero=[]\n")) return 31;

    triceS(iD(1021), "small=[%s]\n", "too long");
    if (check(TriceLog(small, sizeof(small)) == TRICE_LOG_ERR_OUTPUT_TOO_SMALL && small[0] == 0)) return 32;
    if (check(TriceLog(text, sizeof(text)) == 0)) return 33;

    triceS(iD(1014), "twice=%s/%s\n", "one value");
    trice(iD(1016), "after-string-error\n");
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_FORMAT && text[0] == 0)) return 34;
    result = TriceLog(text, sizeof(text));
    if (checkText(result, text, "after-string-error\n")) return 35;

    triceN(iD(1015), "dynamic=%02x\n", noTerminator, sizeof(noTerminator));
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_FORMAT && text[0] == 0)) return 36;

    triceS(iD(1023), "bad-flag=%+s\n", "value");
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_FORMAT && text[0] == 0)) return 37;

    triceS(iD(1026), "overflow=[%999999999999999999999999999999999999999s]\n", "value");
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_FORMAT && text[0] == 0)) return 38;

    triceS(iD(1027), "dynamic-width=[%*s]\n", "value");
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_FORMAT && text[0] == 0)) return 39;

    trice32(iD(1017), "float=%+.2f sci=%.1e\n", aFloat(1.25f), aFloat(-0.03125f));
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_FORMAT && text[0] == 0)) return 40;

    UserTriceLogPrintfFn = countingSnprintf;
    trice32(iD(1017), "float=%+.2f sci=%.1e\n", aFloat(1.25f), aFloat(-0.03125f));
    result = TriceLog(text, sizeof(text));
    if (check(checkText(result, text, "float=+1.25 sci=-3.1e-02\n") == 0 && hookCalls == 5)) return 41;

    trice64(iD(1018), "double=%.6f compact=%.4g\n", aDouble(0.125), aDouble(1234.0));
    result = TriceLog(text, sizeof(text));
    if (check(checkText(result, text, "double=0.125000 compact=1234\n") == 0 && hookCalls == 7)) return 42;

    trice64(iD(1020), "long-float=%.2Lf\n", aDouble(2.5));
    result = TriceLog(text, sizeof(text));
    if (check(checkText(result, text, "long-float=2.50\n") == 0 && hookCalls == 8)) return 43;

    trice16(iD(1019), "bad-float=%f\n", 0u);
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_FORMAT && text[0] == 0)) return 44;

    trice(iD(1022), "scalar-string=%s\n", 0u);
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_FORMAT && text[0] == 0)) return 45;

    trice8(iD(1028), "special=%b/%#b/%O/%t/%p\n", 5u, 5u, 9u, 1u, 255u);
    result = TriceLog(text, sizeof(text));
    if (checkText(result, text, "special=101/0b101/0o11/true/ff\n")) return 46;

    trice8(iD(1029), "quote=%q\n", 'A');
    result = TriceLog(text, sizeof(text));
    if (checkText(result, text, "quote='A'\n")) return 47;

    triceS(iD(1030), "quoted=%q\n", "a\n\"");
    result = TriceLog(text, sizeof(text));
    if (checkText(result, text, "quoted=\"a\\n\\\"\"\n")) return 48;

#if TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN == 0
    const uint16_t values[] = {1u, 0x2au};
    TRICE16_B(id(1031), "buffer:%04x \n", values, 2u);
    result = TriceLog(text, sizeof(text));
    if (check(checkText(result, text, "buffer:0001 002a \n") == 0 && hookCalls == 10)) return 49;
#endif

    trice(iD(1032), "known-but-disabled\n");
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_FEATURE_DISABLED && text[0] == 0)) return 50;

    UserTriceLogPrefixFn = idPrefix;
    trice(iD(1033), "prefixed\n");
    result = TriceLog(text, sizeof(text));
    if (checkText(result, text, "[1033] prefixed\n")) return 51;

    UserTriceLogPrefixFn = failingPrefix;
    trice(iD(1033), "prefixed\n");
    if (check(TriceLog(text, sizeof(text)) == TRICE_LOG_ERR_PREFIX && text[0] == 0)) return 52;

    UserTriceLogPrefixFn = longPrefix;
    trice(iD(1033), "prefixed\n");
    if (check(TriceLog(small, sizeof(small)) == TRICE_LOG_ERR_OUTPUT_TOO_SMALL && small[0] == 0)) return 53;
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

// TestGeneratedLocalLogTableHonorsFeatureSubsets preprocesses one generated
// table under two target configurations. This verifies the complete path from
// shared format analysis to C preprocessor selection, including the important
// invariant that disabled format strings are absent from the target input.
func TestGeneratedLocalLogTableHonorsFeatureSubsets(t *testing.T) {
	compiler := firstAvailableCompiler("cc", "gcc", "clang")
	if compiler == "" {
		t.Skip("no C compiler available")
	}
	defer Setup(t)()
	repositoryRoot, err := filepath.Abs(filepath.Join("..", ".."))
	require.NoError(t, err)
	project := t.TempDir()
	list := TriceIDLookUp{
		1000: {Type: "trice_0", Strg: "always-ready"},
		1001: {Type: "trice32", Strg: "minimal=%d"},
		1002: {Type: "trice32", Strg: "unsigned=%u"},
		1003: {Type: "triceS", Strg: "dynamic=%s"},
		1004: {Type: "TRICE16_B", Strg: "buffer:%04x"},
		1005: {Type: "trice64", Strg: "double=%.2f"},
		1006: {Type: "trice8", Strg: "custom=%#b/%O/%t/%p/%q"},
		1007: {Type: "TRICE8_C", Strg: "abc-command"},
	}
	table, err := list.toListTilC("til.c")
	require.NoError(t, err)
	require.NoError(t, os.WriteFile(filepath.Join(project, "til.c"), table, 0o644))

	preprocess := func(name, options string) string {
		t.Helper()
		config := "// SPDX-License-Identifier: MIT\n#ifndef TRICE_CONFIG_H_\n#define TRICE_CONFIG_H_\n#define TRICE_LOCAL_LOG 1\n" + options + "#endif\n"
		require.NoError(t, os.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(config), 0o644))
		output, compileErr := exec.Command(
			compiler, "-E", "-P",
			"-I", project,
			"-I", filepath.Join(repositoryRoot, "src"),
			filepath.Join(project, "til.c"),
		).CombinedOutput()
		require.NoError(t, compileErr, "%s configuration:\n%s", name, output)
		return string(output)
	}

	minimal := preprocess("literal only", `#define TRICE_LOCAL_LOG_USE_PRINTF_HOOK 0
#define TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER 0
#define TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_64_BIT_VALUES 0
#define TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG 0
#define TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES 0
#define TRICE_LOCAL_LOG_USE_BUFFER_TRICES 0
#define TRICE_LOCAL_LOG_KEEP_DISABLED_IDS 0
`)
	assert.Contains(t, minimal, "always-ready")
	for _, omitted := range []string{"minimal=%d", "unsigned=%u", "dynamic=%s", "buffer:%04x", "double=%.2f", "custom=%#b", "abc-command"} {
		assert.False(t, strings.Contains(minimal, omitted), "disabled preprocessor output retained %q", omitted)
	}

	full := preprocess("full", `#define TRICE_LOCAL_LOG_USE_PRINTF_HOOK 1
#define TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER 0
#define TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS 1
#define TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS 1
#define TRICE_LOCAL_LOG_USE_64_BIT_VALUES 1
#define TRICE_LOCAL_LOG_USE_BINARY_FORMAT_SPECIFIERS 1
#define TRICE_LOCAL_LOG_USE_ALT_FORM_FLAG 1
#define TRICE_LOCAL_LOG_USE_EXTENDED_FORMAT_SPECIFIERS 1
#define TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES 1
#define TRICE_LOCAL_LOG_USE_BUFFER_TRICES 1
`)
	for _, retained := range []string{"always-ready", "minimal=%d", "unsigned=%u", "dynamic=%s", "buffer:%04x", "double=%.2f", "custom=%#b"} {
		assert.True(t, strings.Contains(full, retained), "full preprocessor output omitted %q", retained)
	}
	assert.NotContains(t, full, "abc-command")
}

// TestLocalLogDynamicStringNeedsNoPrintfImplementation verifies that bounded
// %s handling is part of the record formatter rather than either printf path.
// This keeps string-only applications usable while the separately compiled
// minimal formatter is disabled and no external hook is installed.
func TestLocalLogDynamicStringNeedsNoPrintfImplementation(t *testing.T) {
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

	project := t.TempDir()
	config := `// SPDX-License-Identifier: MIT
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 256
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_LOCAL_LOG 1
#define TRICE_LOCAL_LOG_USE_PRINTF_HOOK 0
#define TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER 0
#define TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_64_BIT_VALUES 0
#define TRICE_DIRECT_OUTPUT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_CONFIG_WARNINGS 0
#endif
`
	table := `// SPDX-License-Identifier: MIT
#include "triceRx.h"
const triceLog_t triceLog[] = {
    { 1000u, 8u, TRICE_LOG_PARAM_COUNT_DYNAMIC, "text=[%s]\n" },
    { 1001u, 8u, TRICE_LOG_PARAM_COUNT_DYNAMIC_STRING, "wide=[%4s]\n" },
    { 1002u, 8u, TRICE_LOG_PARAM_COUNT_DYNAMIC_STRING, "precise=[%.2s]\n" },
    { 1003u, 8u, 1u, "binary=%b\n" },
    { 1004u, 8u, 1u, "bool=%t\n" },
    { 1005u, 16u, TRICE_LOG_PARAM_COUNT_DYNAMIC_BUFFER, "buffer:%x\n" },
    { 1006u, 32u, 1u, "float=%f\n" },
    { 1007u, 64u, 1u, "wide=%x\n" },
    { 1008u, 32u, 0u, 0 },
};
const unsigned triceLogElements = sizeof(triceLog) / sizeof(triceLog[0]);
`
	mainSource := `// SPDX-License-Identifier: MIT
#include <string.h>
#include "trice.h"

int main(void) {
    char text[32];
    uint16_t value = 1u;
    triceS(iD(1000), "text=[%s]\n", "standalone");
    int result = TriceLog(text, sizeof(text));
    if (result != 18 || strcmp(text, "text=[standalone]\n") != 0) return 1;
    triceS(iD(1001), "wide=[%4s]\n", "x");
    if (TriceLog(text, sizeof(text)) != TRICE_LOG_ERR_FEATURE_DISABLED) return 2;
    triceS(iD(1002), "precise=[%.2s]\n", "abc");
    if (TriceLog(text, sizeof(text)) != TRICE_LOG_ERR_FEATURE_DISABLED) return 3;
    trice8(iD(1003), "binary=%b\n", 1u);
    if (TriceLog(text, sizeof(text)) != TRICE_LOG_ERR_FEATURE_DISABLED) return 4;
    trice8(iD(1004), "bool=%t\n", 1u);
    if (TriceLog(text, sizeof(text)) != TRICE_LOG_ERR_FEATURE_DISABLED) return 5;
    TRICE16_B(id(1005), "buffer:%x\n", &value, 1u);
    if (TriceLog(text, sizeof(text)) != TRICE_LOG_ERR_FEATURE_DISABLED) return 6;
    trice32(iD(1006), "float=%f\n", aFloat(1.0f));
    if (TriceLog(text, sizeof(text)) != TRICE_LOG_ERR_FEATURE_DISABLED) return 7;
    trice64(iD(1007), "wide=%x\n", UINT64_C(1));
    if (TriceLog(text, sizeof(text)) != TRICE_LOG_ERR_FEATURE_DISABLED) return 8;
    trice(iD(1008), "known-disabled\n");
    if (TriceLog(text, sizeof(text)) != TRICE_LOG_ERR_FEATURE_DISABLED) return 9;
    return text[0] == 0 ? 0 : 10;
}
`
	require.NoError(t, os.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(config), 0o644))
	require.NoError(t, os.WriteFile(filepath.Join(project, "til.c"), []byte(table), 0o644))
	require.NoError(t, os.WriteFile(filepath.Join(project, "main.c"), []byte(mainSource), 0o644))

	executable := filepath.Join(project, "local_log_string")
	if runtime.GOOS == "windows" {
		executable += ".exe"
	}
	arguments := []string{
		"-std=c11", "-Wall", "-Wextra", "-Werror",
		"-I", project,
		"-I", sourceDirectory,
		filepath.Join(project, "main.c"),
		filepath.Join(project, "til.c"),
	}
	arguments = append(arguments, librarySources...)
	arguments = append(arguments, "-o", executable)
	output, compileErr := exec.Command(compiler, arguments...).CombinedOutput()
	require.NoError(t, compileErr, "%s", output)

	output, runErr := exec.Command(executable).CombinedOutput()
	assert.NoError(t, runErr, "%s", output)
}

// TestLocalLogMinimalFormatterWithout64BitValues verifies the smallest numeric
// path as a complete producer-to-text flow. In particular, its strict build
// catches accidental 64-bit signatures or helpers in a 32-bit-only setup.
func TestLocalLogMinimalFormatterWithout64BitValues(t *testing.T) {
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
	project := t.TempDir()
	config := `// SPDX-License-Identifier: MIT
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 256
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_LOCAL_LOG 1
#define TRICE_LOCAL_LOG_USE_PRINTF_HOOK 0
#define TRICE_LOCAL_LOG_USE_MINIMAL_FORMATTER 1
#define TRICE_LOCAL_LOG_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_PRECISION_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_FLOAT_FORMAT_SPECIFIERS 0
#define TRICE_LOCAL_LOG_USE_64_BIT_VALUES 0
#define TRICE_LOCAL_LOG_USE_DYNAMIC_STRING_TRICES 0
#define TRICE_DIRECT_OUTPUT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_CONFIG_WARNINGS 0
#endif
`
	table := `// SPDX-License-Identifier: MIT
#include "triceRx.h"
const triceLog_t triceLog[] = {
    { 1000u, 32u, 2u, "values=%d/%x\n" },
};
const unsigned triceLogElements = sizeof(triceLog) / sizeof(triceLog[0]);
`
	mainSource := `// SPDX-License-Identifier: MIT
#include <string.h>
#include "trice.h"

int main(void) {
    char text[40];
    trice(iD(1000), "values=%d/%x\n", -2147483647, 0xabcdefu);
    int result = TriceLog(text, sizeof(text));
    return result == 26 && strcmp(text, "values=-2147483647/abcdef\n") == 0 ? 0 : 1;
}
`
	require.NoError(t, os.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(config), 0o644))
	require.NoError(t, os.WriteFile(filepath.Join(project, "til.c"), []byte(table), 0o644))
	require.NoError(t, os.WriteFile(filepath.Join(project, "main.c"), []byte(mainSource), 0o644))
	executable := filepath.Join(project, "local_log_minimal32")
	if runtime.GOOS == "windows" {
		executable += ".exe"
	}
	arguments := []string{
		"-std=c11", "-Wall", "-Wextra", "-Werror",
		"-I", project,
		"-I", sourceDirectory,
		filepath.Join(project, "main.c"),
		filepath.Join(project, "til.c"),
	}
	arguments = append(arguments, librarySources...)
	arguments = append(arguments, "-o", executable)
	output, compileErr := exec.Command(compiler, arguments...).CombinedOutput()
	require.NoError(t, compileErr, "%s", output)
	output, runErr := exec.Command(executable).CombinedOutput()
	assert.NoError(t, runErr, "%s", output)
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

// TestLocalLogRejectsNonBooleanFeatureConfiguration verifies that local-log
// mistakes fail at the configuration boundary with the switch name, rather
// than selecting inconsistent preprocessor paths across translation units.
func TestLocalLogRejectsNonBooleanFeatureConfiguration(t *testing.T) {
	compiler := firstAvailableCompiler("cc", "gcc", "clang")
	if compiler == "" {
		t.Skip("no C compiler available")
	}
	repositoryRoot, err := filepath.Abs(filepath.Join("..", ".."))
	require.NoError(t, err)
	project := t.TempDir()
	config := `// SPDX-License-Identifier: MIT
#define TRICE_LOCAL_LOG_USE_BUFFER_TRICES 2
#include "triceLogDefaultConfig.h"
`
	require.NoError(t, os.WriteFile(filepath.Join(project, "invalid.c"), []byte(config), 0o644))
	object := filepath.Join(project, "invalid.o")
	output, compileErr := exec.Command(
		compiler,
		"-std=c11",
		"-I", filepath.Join(repositoryRoot, "src"),
		"-c", filepath.Join(project, "invalid.c"),
		"-o", object,
	).CombinedOutput()
	require.Error(t, compileErr)
	assert.Contains(t, string(output), "TRICE_LOCAL_LOG_USE_BUFFER_TRICES must be 0 or 1")
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
