// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"sort"
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

// maskCanonicalMacroDefinitions excludes the two intentionally unsupported wrapper macros in the temporary copy.
func maskCanonicalMacroDefinitions(t *testing.T, source string) string {
	t.Helper()
	start := strings.Index(source, "//! SCOPY is a helper macro")
	end := strings.Index(source[start:], "typedef struct")
	require.GreaterOrEqual(t, start, 0)
	require.GreaterOrEqual(t, end, 0)
	end += start
	macroDefinitions := source[start:end]
	// The temporary compile keeps these explicitly unsupported wrapper bodies on
	// the inserted implementation path while every direct user site stays bound.
	macroDefinitions = strings.ReplaceAll(macroDefinitions, "TRICE_S(", "TRICE_INSERT_TRICE_S(")
	macroDefinitions = strings.ReplaceAll(macroDefinitions, "TRICE(", "TRICE_INSERT_TRICE(")
	return source[:start] + "// TRICE_INSERT_OFF\n" + macroDefinitions + "// TRICE_INSERT_ON\n" + source[end:]
}

// splitCanonicalBindLines moves only second and later managed sites to their own physical lines.
func splitCanonicalBindLines(t *testing.T, source string) string {
	t.Helper()
	sites, diagnostics := scanBindSites("triceCheck.c", source)
	for _, diagnostic := range diagnostics {
		require.Contains(t, diagnostic.message, "multiple bindable Trice sites")
	}
	seenLine := make(map[int]bool)
	positions := make([]int, 0)
	for _, site := range sites {
		if seenLine[site.line] {
			positions = append(positions, site.loc[0])
		}
		seenLine[site.line] = true
	}
	sort.Sort(sort.Reverse(sort.IntSlice(positions)))
	for _, position := range positions {
		source = source[:position] + "\n" + source[position:]
	}
	return source
}

// TestBindCanonicalTriceCheckGeneratesCompleteSidecar uses the canonical macro matrix in a temporary bind tree.
func TestBindCanonicalTriceCheckGeneratesCompleteSidecar(t *testing.T) {
	bindIntegrationEnabled(t)
	root := bindRepositoryRoot(t)
	canonical, err := os.ReadFile(filepath.Join(root, "_test", "testdata", "triceCheck.c"))
	require.NoError(t, err)

	// The repository fixture may intentionally be committed Bound. Normalize
	// only this temporary copy so the test always exercises include insertion.
	canonicalSource := string(canonical)
	canonicalSource = string(removeBindSidecarIncludes(canonical, scanBindIncludes(canonicalSource)))
	source := maskCanonicalMacroDefinitions(t, canonicalSource)
	source = splitCanonicalBindLines(t, source)
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
	require.Len(t, entries, 1)
	sidecar, err := os.ReadFile(filepath.Join(BindDir, entries[0].Name()))
	require.NoError(t, err)
	assert.Equal(t, len(sites), bytes.Count(sidecar, []byte("#define TRICE_BIND_SITE_")))
	for _, line := range strings.Split(string(sidecar), "\n") {
		assert.Falsef(t, strings.HasSuffix(line, "\\"), "sidecar line must not continue physically: %s", line)
	}
	for _, site := range sites {
		assert.Contains(t, string(sidecar), fmt.Sprintf("_L%d ", site.line+1), "auto-inserted include shifts every canonical site by one line")
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
