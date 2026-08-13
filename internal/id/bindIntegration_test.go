// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"runtime"
	"strings"
	"testing"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// bindIntegrationEnabled keeps compiler-dependent checks in the dedicated repository script.
func bindIntegrationEnabled(t *testing.T) {
	t.Helper()
	if os.Getenv("TRICE_BIND_INTEGRATION") != "1" {
		t.Skip("set TRICE_BIND_INTEGRATION=1 to run compiler and canonical-source checks")
	}
}

// bindRepositoryRoot resolves repository fixtures without embedding a machine-specific path.
func bindRepositoryRoot(t *testing.T) string {
	t.Helper()
	_, filename, _, ok := runtime.Caller(0)
	require.True(t, ok)
	return filepath.Clean(filepath.Join(filepath.Dir(filename), "..", ".."))
}

// prepareOSBindProject gives an integration test isolated lists, paths, and global ID state.
func prepareOSBindProject(t *testing.T, project string) (*afero.Afero, func()) {
	t.Helper()
	teardown := Setup(t)
	fileSystem := &afero.Afero{Fs: afero.NewOsFs()}
	FnJSON = filepath.Join(project, "til.json")
	LIFnJSON = filepath.Join(project, "li.json")
	LIRoot = project
	BindDir = filepath.Join(project, "build", "triceIDs")
	Srcs = ArrayFlag{project}
	ExcludeSrcs = nil
	Min = 1000
	Max = 7999
	SearchMethod = "upward"
	DefaultStampSize = 16
	TriceAliases = nil
	TriceSAliases = nil
	IDData.TagList = nil
	ProcessAliases()
	require.NoError(t, os.WriteFile(FnJSON, []byte("{}\n"), 0o644))
	require.NoError(t, os.WriteFile(LIFnJSON, []byte("{}\n"), 0o644))
	return fileSystem, teardown
}

// writeBindIntegrationFile creates one documented test fixture below the temporary project.
func writeBindIntegrationFile(t *testing.T, root, name, content string) string {
	t.Helper()
	path := filepath.Join(root, filepath.FromSlash(name))
	require.NoError(t, os.MkdirAll(filepath.Dir(path), 0o755))
	require.NoError(t, os.WriteFile(path, []byte(content), 0o644))
	return path
}

// firstAvailableCompiler returns the first requested GCC- or Clang-compatible frontend.
func firstAvailableCompiler(candidates ...string) string {
	for _, candidate := range candidates {
		if path, err := exec.LookPath(candidate); err == nil {
			return path
		}
	}
	return ""
}

// availableBindCompilers resolves every installed candidate once so the MVP2
// matrix covers GCC- and Clang-compatible commands whenever both are present.
func availableBindCompilers(candidates ...string) []string {
	seen := make(map[string]bool)
	var result []string
	for _, candidate := range candidates {
		path, err := exec.LookPath(candidate)
		if err == nil && !seen[path] {
			seen[path] = true
			result = append(result, path)
		}
	}
	return result
}

// compileBindFixture compiles without linking so target macro expansion is checked in isolation.
func compileBindFixture(t *testing.T, compiler, standard, source, object string, includeDirs ...string) {
	compileBindFixtureWithPedantry(t, compiler, standard, source, object, true, includeDirs...)
}

// compileBindFixtureWithPedantry optionally tolerates the established pre-C23 empty variadic slot in triceOff.h.
func compileBindFixtureWithPedantry(t *testing.T, compiler, standard, source, object string, pedantic bool, includeDirs ...string) {
	t.Helper()
	args := []string{"-std=" + standard, "-Wall", "-Wextra", "-Werror"}
	if pedantic {
		args = append(args, "-pedantic")
	}
	for _, includeDir := range includeDirs {
		args = append(args, "-I", includeDir)
	}
	args = append(args, "-c", source, "-o", object)
	command := exec.Command(compiler, args...)
	output, err := command.CombinedOutput()
	require.NoErrorf(t, err, "%s %s\n%s", compiler, strings.Join(args, " "), output)
}

// compileBindFixtureOptimized compiles one target fixture at an explicit optimization level.
func compileBindFixtureOptimized(t *testing.T, compiler, standard, optimization, source, object string, includeDirs ...string) {
	t.Helper()
	args := []string{"-std=" + standard, optimization, "-Wall", "-Wextra", "-Werror", "-pedantic"}
	for _, includeDir := range includeDirs {
		args = append(args, "-I", includeDir)
	}
	args = append(args, "-c", source, "-o", object)
	command := exec.Command(compiler, args...)
	output, err := command.CombinedOutput()
	require.NoErrorf(t, err, "%s %s\n%s", compiler, strings.Join(args, " "), output)
}

// runBindFixtureCompiler exposes diagnostics for negative capability and
// generated-invariant tests while retaining the normal warning policy.
func runBindFixtureCompiler(compiler, standard, source, object string, extraFlags []string, includeDirs ...string) ([]byte, error) {
	args := []string{"-std=" + standard, "-O0", "-Wall", "-Wextra", "-Werror", "-pedantic"}
	args = append(args, extraFlags...)
	for _, includeDir := range includeDirs {
		args = append(args, "-I", includeDir)
	}
	args = append(args, "-c", source, "-o", object)
	return exec.Command(compiler, args...).CombinedOutput()
}

// assertBindRebaseConstructorCoverage keeps the constructor metadata in sync
// with every legacy public dispatch macro without changing that public API.
func assertBindRebaseConstructorCoverage(t *testing.T, root string) {
	t.Helper()
	header, err := os.ReadFile(filepath.Join(root, "src", "triceBind.h"))
	require.NoError(t, err)
	publicPattern := regexp.MustCompile(`(?m)^#define ([A-Za-z_0-9]+)\(\.\.\.\) TRICE_BIND_DISPATCH\(TRICE_BIND_ROUTE\(TRICE_BIND_FILE_KEY\), (TRICE_INSERT_[A-Za-z_0-9]+), __VA_ARGS__\)$`)
	mappingPattern := regexp.MustCompile(`(?m)^#define TRICE_BIND_REBASE_CONSTRUCTOR_(TRICE_INSERT_[A-Za-z_0-9]+) (iD|TRICE_BIND_REBASE_DEFAULT_TID)$`)
	mappings := make(map[string]string)
	for _, match := range mappingPattern.FindAllSubmatch(header, -1) {
		mappings[string(match[1])] = string(match[2])
	}
	public := publicPattern.FindAllSubmatch(header, -1)
	require.NotEmpty(t, public)
	assert.Len(t, mappings, len(public))
	for _, match := range public {
		name := string(match[1])
		expected := "TRICE_BIND_REBASE_DEFAULT_TID"
		if len(name) > 2 && name[2] == 'i' {
			expected = "iD"
		}
		assert.Equal(t, expected, mappings[string(match[2])], name)
	}
}

// TestBindGeneratedTargetCompilesCAndCPP exercises real generated sidecars against the target headers.
func TestBindGeneratedTargetCompilesCAndCPP(t *testing.T) {
	bindIntegrationEnabled(t)
	root := bindRepositoryRoot(t)
	project := t.TempDir()
	fileSystem, teardown := prepareOSBindProject(t, project)
	defer teardown()

	config := `// SPDX-License-Identifier: MIT
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_CLEAN 0
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DEFERRED_OUTPUT 0
#define TRICE_DIRECT_AUXILIARY8 1
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_TCOBS
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0
#define BIND_ALIAS trice
#define BIND_SALIAS triceS
#endif
`
	header := `// SPDX-License-Identifier: MIT
#ifndef BIND_TARGET_FIXTURE_H_
#define BIND_TARGET_FIXTURE_H_
#include "trice.h"
static inline int bind_inline(int value) {
    trice("msg:inline=%d\n", value);
    Trice("msg:inline16=%d\n", value);
    return value;
}
#endif
`
	cSource := `// SPDX-License-Identifier: MIT
#include <stdint.h>
#include "bound.h"
#include "trice.h"
int bind_c(int value) {
    static const uint8_t bytes[] = {1u, 2u};
    trice("msg:lower=%d\n", value);
    TRICE("msg:upper=%d\n", value);
    triceS("msg:string=%s\n", "c");
    BIND_ALIAS("msg:alias=%d\n", value);
    BIND_SALIAS("msg:salias=%s\n", "c");
    TRICE_B(id(0), "msg:byte=%02x\n", bytes, sizeof bytes);
    triceAssertOrReturnValue("err:value must be positive\n", value > 0, -1);
    return bind_inline(value);
}
`
	cppSource := `// SPDX-License-Identifier: MIT
#include <cstdint>
#include "bound.h"
#include "trice.h"
int bind_cpp(int value) {
    static const std::uint8_t bytes[] = {3u, 4u};
    TRice("msg:mixed=%d\n", value);
    TRICE8_B(ID(0), "msg:cpp-byte=%02x\n", bytes, sizeof bytes);
    return bind_inline(value);
}
`
	hybridSource := `// SPDX-License-Identifier: MIT
#include "bound.h"
#undef TRICE_BIND_FILE_KEY
void bind_hybrid(void) {
    trice(iD(1500), "msg:insert-owned after bound header\n");
}
`
	writeBindIntegrationFile(t, project, "triceConfig.h", config)
	writeBindIntegrationFile(t, project, "bound.h", header)
	cPath := writeBindIntegrationFile(t, project, "module.c", cSource)
	cppPath := writeBindIntegrationFile(t, project, "module.cpp", cppSource)
	hybridPath := writeBindIntegrationFile(t, project, "hybrid.c", hybridSource)
	TriceAliases = ArrayFlag{"BIND_ALIAS"}
	TriceSAliases = ArrayFlag{"BIND_SALIAS"}
	ProcessAliases()

	require.NoError(t, SubCmdIdBind(os.Stdout, fileSystem))
	hybridAfter, err := os.ReadFile(hybridPath)
	require.NoError(t, err)
	assert.Equal(t, hybridSource, string(hybridAfter), "insert-owned source must not receive a sidecar")
	assert.DirExists(t, BindDir)

	cCompiler := firstAvailableCompiler("cc", "gcc", "clang")
	cppCompiler := firstAvailableCompiler("c++", "g++", "clang++")
	if cCompiler == "" || cppCompiler == "" {
		t.Fatalf("bind target integration requires a GCC- or Clang-compatible C and C++ frontend")
	}
	includes := []string{BindDir, project, filepath.Join(root, "src")}
	compileBindFixture(t, cCompiler, "c11", cPath, filepath.Join(project, "module.o"), includes...)
	compileBindFixture(t, cCompiler, "c11", hybridPath, filepath.Join(project, "hybrid.o"), includes...)
	compileBindFixture(t, cppCompiler, "c++17", cppPath, filepath.Join(project, "module_cpp.o"), includes...)

	// Bound ID-free sources must also stay valid when clean or off selects the existing no-output macros.
	cleanConfigDir := filepath.Join(project, "clean-config")
	offConfigDir := filepath.Join(project, "off-config")
	writeBindIntegrationFile(t, cleanConfigDir, "triceConfig.h", strings.Replace(config, "#define TRICE_CLEAN 0", "#define TRICE_CLEAN 1", 1))
	writeBindIntegrationFile(t, offConfigDir, "triceConfig.h", strings.Replace(config, "#define TRICE_CLEAN 0", "#define TRICE_CLEAN 0\n#define TRICE_OFF 1", 1))
	compileBindFixtureWithPedantry(t, cCompiler, "c11", cPath, filepath.Join(project, "module_clean.o"), false, BindDir, cleanConfigDir, project, filepath.Join(root, "src"))
	compileBindFixtureWithPedantry(t, cCompiler, "c11", cPath, filepath.Join(project, "module_off.o"), false, BindDir, offConfigDir, project, filepath.Join(root, "src"))
}

// TestBindMVP2RebaseCompilesCAndCPP exercises generated direct, wrapper,
// same-line, source, and header regions against the production target macros.
func TestBindMVP2RebaseCompilesCAndCPP(t *testing.T) {
	bindIntegrationEnabled(t)
	root := bindRepositoryRoot(t)
	assertBindRebaseConstructorCoverage(t, root)
	project := t.TempDir()
	fileSystem, teardown := prepareOSBindProject(t, project)
	defer teardown()

	config := `// SPDX-License-Identifier: MIT
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_CLEAN 0
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DEFERRED_OUTPUT 0
#define TRICE_DIRECT_AUXILIARY8 1
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_TCOBS
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0
#endif
`
	header := `// SPDX-License-Identifier: MIT
#ifndef BIND_MVP2_FIXTURE_H_
#define BIND_MVP2_FIXTURE_H_
#include "trice.h"
#define LOG_ERROR(value)                                      \
    do {                                                      \
        switch (value) {                                      \
        case 7:                                               \
            trice("cannot open file\n");                    \
            break;                                            \
        default:                                              \
            TRICE("error=%d", value);                        \
            break;                                            \
        }                                                     \
    } while (0)
#define STRINGIFIED_LOG(value) do { const char *bindName = #value; triceS("name=%s", bindName); TRICE("value=%d", value); } while (0);
static inline void bind_header_direct(void) { trice("header-a"); trice("header-b"); }
static inline void bind_header_wrapper(int value) { LOG_ERROR(value); }
#endif
`
	sharedHeader := `// SPDX-License-Identifier: MIT
#ifndef BIND_MVP2_SHARED_H_
#define BIND_MVP2_SHARED_H_
#include "trice.h"
#define SHARED_LOG(value) do { trice("shared-first"); trice("shared-second=%d", value); } while (0)
#endif
`
	cSource := `// SPDX-License-Identifier: MIT
#include "logging.h"
#include "shared.h"
void bind_mvp2_c(int value) {
    enum { unrelatedBefore = __COUNTER__ };
    (void)unrelatedBefore;
    LOG_ERROR(value);
    STRINGIFIED_LOG(value)
    trice("direct-a"); trice("direct-b"); trice("direct-c");
    LOG_ERROR(7); LOG_ERROR(8);
    bind_header_direct();
    bind_header_wrapper(value);
    SHARED_LOG(value);
}

`
	cppSource := `// SPDX-License-Identifier: MIT
#include "logging.h"
void bind_mvp2_cpp(int value) {
    LOG_ERROR(value);
    STRINGIFIED_LOG(value)
    trice("cpp-a"); trice("cpp-b");
    bind_header_direct();
    bind_header_wrapper(value);
}
`
	secondCSource := `// SPDX-License-Identifier: MIT
#include "logging.h"
#include "shared.h"
void bind_mvp2_second_c(int value) {
    LOG_ERROR(value);
    STRINGIFIED_LOG(value)
    bind_header_direct();
    bind_header_wrapper(value);
    SHARED_LOG(value);
}
`
	writeBindIntegrationFile(t, project, "triceConfig.h", config)
	writeBindIntegrationFile(t, project, "logging.h", header)
	writeBindIntegrationFile(t, project, "shared.h", sharedHeader)
	cPath := writeBindIntegrationFile(t, project, "module.c", cSource)
	secondCPath := writeBindIntegrationFile(t, project, "module_second.c", secondCSource)
	cppPath := writeBindIntegrationFile(t, project, "module.cpp", cppSource)

	require.NoError(t, SubCmdIdBind(os.Stdout, fileSystem))
	boundHeader, err := os.ReadFile(filepath.Join(project, "logging.h"))
	require.NoError(t, err)
	assert.Contains(t, string(boundHeader), "trice-bind: generated rebase begin")
	assert.Equal(t, 2, strings.Count(string(boundHeader), bindRebaseIncludeMarker+"begin"))
	assert.NotContains(t, string(boundHeader), "#define TRICE_BIND_REBASE_BEGIN")
	assert.Len(t, IDData.idToTrice, 13, "wrapper definitions are allocated once across all invocation sites")

	cCompilers := availableBindCompilers("gcc", "clang", "cc")
	cppCompilers := availableBindCompilers("g++", "clang++", "c++")
	if len(cCompilers) == 0 || len(cppCompilers) == 0 {
		t.Fatal("MVP2 bind integration requires GCC- or Clang-compatible C and C++ frontends")
	}
	includes := []string{BindDir, project, filepath.Join(root, "src")}
	for compilerIndex, compiler := range cCompilers {
		compileBindFixtureOptimized(t, compiler, "c99", "-O0", cPath, filepath.Join(project, fmt.Sprintf("module_c%d_o0.o", compilerIndex)), includes...)
		compileBindFixtureOptimized(t, compiler, "c99", "-O2", cPath, filepath.Join(project, fmt.Sprintf("module_c%d_o2.o", compilerIndex)), includes...)
		compileBindFixtureOptimized(t, compiler, "c99", "-O0", secondCPath, filepath.Join(project, fmt.Sprintf("module_second_c%d_o0.o", compilerIndex)), includes...)
		compileBindFixtureOptimized(t, compiler, "c99", "-O2", secondCPath, filepath.Join(project, fmt.Sprintf("module_second_c%d_o2.o", compilerIndex)), includes...)
	}
	for compilerIndex, compiler := range cppCompilers {
		compileBindFixtureOptimized(t, compiler, "c++11", "-O0", cppPath, filepath.Join(project, fmt.Sprintf("module_cpp%d_o0.o", compilerIndex)), includes...)
		compileBindFixtureOptimized(t, compiler, "c++11", "-O2", cppPath, filepath.Join(project, fmt.Sprintf("module_cpp%d_o2.o", compilerIndex)), includes...)
	}
}

// TestBindMVP2CounterGuardsAndGeneratedInvariants checks the local capability
// boundary, counter drift, expected expansion count, and descriptor presence.
func TestBindMVP2CounterGuardsAndGeneratedInvariants(t *testing.T) {
	bindIntegrationEnabled(t)
	root := bindRepositoryRoot(t)
	project := t.TempDir()
	fileSystem, teardown := prepareOSBindProject(t, project)
	defer teardown()

	config := `// SPDX-License-Identifier: MIT
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_CLEAN 0
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DEFERRED_OUTPUT 0
#define TRICE_DIRECT_AUXILIARY8 1
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_TCOBS
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0
#endif
`
	advancedSource := `// SPDX-License-Identifier: MIT
#include "trice.h"
void advanced(void) { trice("first"); trice("second"); }
`
	normalSource := `// SPDX-License-Identifier: MIT
#include "trice.h"
void normal(void) {
    trice("normal");
}
`
	writeBindIntegrationFile(t, project, "triceConfig.h", config)
	advancedPath := writeBindIntegrationFile(t, project, "advanced.c", advancedSource)
	normalPath := writeBindIntegrationFile(t, project, "normal.c", normalSource)
	require.NoError(t, SubCmdIdBind(os.Stdout, fileSystem))

	compiler := firstAvailableCompiler("cc", "gcc", "clang")
	if compiler == "" {
		t.Fatal("MVP2 negative integration requires a GCC- or Clang-compatible C frontend")
	}
	includes := []string{BindDir, project, filepath.Join(root, "src")}
	noCounterFlags := []string{"-U__COUNTER__", "-Wno-builtin-macro-redefined"}
	output, err := runBindFixtureCompiler(compiler, "c99", advancedPath, filepath.Join(project, "advanced_no_counter.o"), noCounterFlags, includes...)
	require.Error(t, err)
	assert.Contains(t, string(output), "this advanced source construct requires __COUNTER__")
	output, err = runBindFixtureCompiler(compiler, "c99", normalPath, filepath.Join(project, "normal_no_counter.o"), noCounterFlags, includes...)
	require.NoErrorf(t, err, "%s", output)

	cleanConfigDir := filepath.Join(project, "clean-config")
	offConfigDir := filepath.Join(project, "off-config")
	writeBindIntegrationFile(t, cleanConfigDir, "triceConfig.h", strings.Replace(config, "#define TRICE_CLEAN 0", "#define TRICE_CLEAN 1", 1))
	writeBindIntegrationFile(t, offConfigDir, "triceConfig.h", strings.Replace(config, "#define TRICE_CLEAN 0", "#define TRICE_CLEAN 0\n#define TRICE_OFF 1", 1))
	for _, disabledConfigDir := range []string{cleanConfigDir, offConfigDir} {
		disabledIncludes := []string{BindDir, disabledConfigDir, project, filepath.Join(root, "src")}
		output, err = runBindFixtureCompiler(compiler, "c99", advancedPath, filepath.Join(disabledConfigDir, "advanced_no_counter.o"), noCounterFlags, disabledIncludes...)
		require.NoErrorf(t, err, "%s", output)
	}

	boundSource, err := os.ReadFile(advancedPath)
	require.NoError(t, err)
	owner := scanBindIncludes(string(boundSource))
	var sidecarPath string
	for _, include := range owner {
		if include.isSidecar {
			sidecarPath = filepath.Join(BindDir, include.name)
			break
		}
	}
	require.NotEmpty(t, sidecarPath)
	sidecar, err := os.ReadFile(sidecarPath)
	require.NoError(t, err)

	t.Run("foreign counter drift", func(t *testing.T) {
		beginPosition := strings.Index(string(boundSource), bindRebaseIncludeMarker+"begin")
		require.GreaterOrEqual(t, beginPosition, 0)
		insertPosition := lineEndIncludingNewline(string(boundSource), beginPosition)
		drifted := string(boundSource[:insertPosition]) + "enum { triceBindForeignCounter = __COUNTER__ };\n" + string(boundSource[insertPosition:])
		require.NoError(t, os.WriteFile(advancedPath, []byte(drifted), 0o644))
		output, compileErr := runBindFixtureCompiler(compiler, "c99", advancedPath, filepath.Join(project, "advanced_drift.o"), nil, includes...)
		require.Error(t, compileErr)
		assert.Contains(t, string(output), "triceBindRebase")
		require.NoError(t, os.WriteFile(advancedPath, boundSource, 0o644))
	})

	for _, replacement := range []string{" 1\n", " 3\n"} {
		t.Run("wrong expansion count"+strings.TrimSpace(replacement), func(t *testing.T) {
			modified := regexp.MustCompile(`(?m)(^#define TRICE_BIND_REBASE_COUNT_[^ ]+) 2$`).ReplaceAll(sidecar, []byte("${1}"+replacement))
			require.NotEqual(t, sidecar, modified)
			require.NoError(t, os.WriteFile(sidecarPath, modified, 0o644))
			output, compileErr := runBindFixtureCompiler(compiler, "c99", advancedPath, filepath.Join(project, "advanced_count_"+strings.TrimSpace(replacement)+".o"), nil, includes...)
			require.Error(t, compileErr)
			assert.Contains(t, string(output), "TRICE_BIND_REBASE_STATIC_ASSERT")
			require.NoError(t, os.WriteFile(sidecarPath, sidecar, 0o644))
		})
	}

	t.Run("missing descriptor", func(t *testing.T) {
		lines := strings.Split(string(sidecar), "\n")
		removed := false
		for index, line := range lines {
			if strings.HasPrefix(line, "#define TRICE_BIND_ID_LOCATION_") {
				lines = append(lines[:index], lines[index+1:]...)
				removed = true
				break
			}
		}
		require.True(t, removed)
		require.NoError(t, os.WriteFile(sidecarPath, []byte(strings.Join(lines, "\n")), 0o644))
		output, compileErr := runBindFixtureCompiler(compiler, "c99", advancedPath, filepath.Join(project, "advanced_missing_descriptor.o"), nil, includes...)
		require.Error(t, compileErr)
		assert.Contains(t, string(output), "TRICE_BIND_ID_LOCATION_")
		require.NoError(t, os.WriteFile(sidecarPath, sidecar, 0o644))
	})
}

// TestBindMVP2RebaseEmitsStableRuntimeIDs executes a small target adapter at
// both optimization levels and checks the ID sequence selected at expansion
// time, including both runtime branches of a two-site logging wrapper.
func TestBindMVP2RebaseEmitsStableRuntimeIDs(t *testing.T) {
	bindIntegrationEnabled(t)
	root := bindRepositoryRoot(t)
	project := t.TempDir()
	fileSystem, teardown := prepareOSBindProject(t, project)
	defer teardown()

	target := `// SPDX-License-Identifier: MIT
#ifndef BIND_RUNTIME_TARGET_H_
#define BIND_RUNTIME_TARGET_H_
void bind_emit(unsigned id);
#define iD(n) (n)
#define id(n) (n)
#define Id(n) (n)
#define ID(n) (n)
#define TRICE_INSERT_trice(tid, ...) bind_emit((unsigned)(tid))
#define TRICE_INSERT_TRICE(tid, ...) bind_emit((unsigned)(tid))
#include "triceBind.h"
#define CUSTOM_WITH_ID(tid, ...) bind_emit((unsigned)(tid))
#define CUSTOM(...) TRICE_BIND_DISPATCH_AUTO(TRICE_BIND_ROUTE(TRICE_BIND_FILE_KEY), iD, CUSTOM_WITH_ID, __VA_ARGS__)
#endif
`
	source := `// SPDX-License-Identifier: MIT
#include <stdio.h>
#include "target.h"
#define LOG_ERROR(value) do { switch (value) { case 7: trice("case-seven"); break; default: TRICE("default=%d", value); break; } } while (0)
static unsigned emitted[6];
static unsigned emittedCount;
void bind_emit(unsigned id) { emitted[emittedCount++] = id; }
int main(void) {
    enum { foreignBefore = __COUNTER__ };
    (void)foreignBefore;
    trice("direct-first"); TRICE("direct-second");
    LOG_ERROR(
        7
    );
    LOG_ERROR(8);
    CUSTOM("custom-first"); CUSTOM("custom-second");
    if (emittedCount != 6u) { return 1; }
    if (emitted[0] != 1002u || emitted[1] != 1003u || emitted[2] != 1000u || emitted[3] != 1001u || emitted[4] != 1004u || emitted[5] != 1005u) {
        printf("%u %u %u %u %u %u\n", emitted[0], emitted[1], emitted[2], emitted[3], emitted[4], emitted[5]);
        return 2;
    }
    return 0;
}
`
	writeBindIntegrationFile(t, project, "target.h", target)
	sourcePath := writeBindIntegrationFile(t, project, "runtime.c", source)
	Srcs = ArrayFlag{sourcePath}
	TriceAliases = ArrayFlag{"CUSTOM"}
	ProcessAliases()
	require.NoError(t, SubCmdIdBind(os.Stdout, fileSystem))
	assert.Len(t, IDData.idToTrice, 6)

	compiler := firstAvailableCompiler("cc", "gcc", "clang")
	if compiler == "" {
		t.Fatal("MVP2 runtime integration requires a GCC- or Clang-compatible C frontend")
	}
	for _, optimization := range []string{"-O0", "-O2"} {
		executable := filepath.Join(project, "runtime_"+strings.TrimPrefix(optimization, "-"))
		if runtime.GOOS == "windows" {
			executable += ".exe"
		}
		args := []string{"-std=c99", optimization, "-Wall", "-Wextra", "-Werror", "-pedantic", "-I", BindDir, "-I", project, "-I", filepath.Join(root, "src"), sourcePath, "-o", executable}
		output, err := exec.Command(compiler, args...).CombinedOutput()
		require.NoErrorf(t, err, "%s %s\n%s", compiler, strings.Join(args, " "), output)
		output, err = exec.Command(executable).CombinedOutput()
		require.NoErrorf(t, err, "%s\n%s", executable, output)
	}
}

// TestBindCanonicalTriceCheckGeneratesCompleteSidecar uses the canonical macro matrix in a temporary bind tree.
func TestBindCanonicalTriceCheckGeneratesCompleteSidecar(t *testing.T) {
	bindIntegrationEnabled(t)
	root := bindRepositoryRoot(t)
	canonical, err := os.ReadFile(filepath.Join(root, "_test", "testdata", "triceCheck.c"))
	require.NoError(t, err)

	// The repository fixture may intentionally be committed Bound. Normalize
	// only this temporary copy so the test always exercises include insertion.
	canonical, _, artifactDiagnostics, _ := stripBindRebaseArtifacts("triceCheck.c", canonical)
	require.Empty(t, artifactDiagnostics)
	canonicalSource := string(canonical)
	canonicalSource = string(removeBindSidecarIncludes(canonical, scanBindIncludes(canonicalSource)))
	source := canonicalSource
	sites, diagnostics := scanBindSites("triceCheck.c", source)
	require.Empty(t, diagnostics)
	require.Greater(t, len(sites), 500, "canonical source should exercise the broad public macro matrix")

	project := t.TempDir()
	fileSystem, teardown := prepareOSBindProject(t, project)
	defer teardown()
	config := `// SPDX-License-Identifier: MIT
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_CLEAN 0
#define TRICE_LEGACY_RPC_SUPPORT 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DEFERRED_OUTPUT 0
#define TRICE_DIRECT_AUXILIARY8 1
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_TCOBS
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0
#define TRICE_CGO 1
#endif
`
	writeBindIntegrationFile(t, project, "triceConfig.h", config)
	sourcePath := writeBindIntegrationFile(t, project, "triceCheck.c", source)
	require.NoError(t, SubCmdIdBind(os.Stdout, fileSystem))

	bound, err := os.ReadFile(sourcePath)
	require.NoError(t, err)
	assert.Contains(t, string(bound), "trice-bind: keep as last include")
	entries, err := os.ReadDir(BindDir)
	require.NoError(t, err)
	rebaseRegionCount := strings.Count(string(bound), bindRebaseIncludeMarker+"begin")
	require.Len(t, entries, 1+2*rebaseRegionCount)
	var ownerName string
	for _, include := range scanBindIncludes(string(bound)) {
		if include.isSidecar {
			ownerName = include.name
			break
		}
	}
	require.NotEmpty(t, ownerName)
	sidecar, err := os.ReadFile(filepath.Join(BindDir, ownerName))
	require.NoError(t, err)
	ordinarySites := bytes.Count(sidecar, []byte("#define TRICE_BIND_SITE_"))
	definitionSites := bytes.Count(sidecar, []byte("#define TRICE_BIND_DEFINITION_"))
	definitionIDs := bytes.Count(sidecar, []byte("#define TRICE_BIND_ID_DEFINITION_"))
	locationSites := bytes.Count(sidecar, []byte("#define TRICE_BIND_LOCATION_"))
	locationIDs := bytes.Count(sidecar, []byte("#define TRICE_BIND_ID_LOCATION_"))
	require.Equal(t, definitionSites, definitionIDs, "each wrapper definition site has one numeric descriptor")
	require.Equal(t, locationSites, locationIDs, "each counter-selected site has one numeric descriptor")
	assert.Equal(t, len(sites), ordinarySites+definitionSites+locationSites)
	assert.Positive(t, definitionSites, "canonical source should exercise logging wrapper definitions")
	assert.Positive(t, locationSites, "canonical source should exercise counter-selected locations")
	for _, line := range strings.Split(string(sidecar), "\n") {
		assert.Falsef(t, strings.HasSuffix(line, "\\"), "sidecar line must not continue physically: %s", line)
	}

	cCompiler := firstAvailableCompiler("cc", "gcc", "clang")
	if cCompiler == "" {
		t.Fatal("canonical bind integration requires a GCC- or Clang-compatible C frontend")
	}
	compileBindFixture(
		t,
		cCompiler,
		"c11",
		sourcePath,
		filepath.Join(project, "triceCheck.o"),
		BindDir,
		project,
		filepath.Join(root, "src"),
	)
}
