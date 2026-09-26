// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"testing"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// cePoCKey seeds the ordinary Bind include so CE cannot hide a source edit
// behind the first-time installation of that existing Bind prerequisite.
const cePoCKey = "K0123456789ABCDEF"
const cePoCSidecar = "trice_main_c_" + cePoCKey + ".h"

// cePoCConfig enables an unframed output hook and the real binary record
// resolver. Neither a simulated logging macro nor a printf stub produces data.
const cePoCConfig = `// SPDX-License-Identifier: MIT
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_CLEAN 0
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_AUXILIARY8 1
#define TRICE_DIRECT_OUT_FRAMING TRICE_FRAMING_NONE
#define TRICE_DEFERRED_OUTPUT 0
#define TRICE_RX_LOG_SUPPORT 1
#define TRICE_RX_ABC_SUPPORT 0
#define TRICE_TRANSFER_ORDER_IS_BIG_ENDIAN 0
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0
#define TRICE_CONFIG_WARNINGS 0
#endif
`

// cePoCSource is the unchanged target program. The output hook resolves real
// payloads against a table generated from the CE TIL, including its final arity.
const cePoCSource = `// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <stdio.h>
#include "trice.h"
#include "triceRx.h"
#include "trice_main_c_K0123456789ABCDEF.h"

static unsigned originalEvaluations;
static unsigned contextEvaluations;
static unsigned records;
static unsigned failures;

// Decode the actual transmitted record, then expose its ID and ordered values.
static void captureRecord(const uint8_t* data, size_t size) {
    triceRx_t rx;
    int consumed = TriceParseRecord(&rx, data, size);
    if (consumed <= 0 || (size_t)consumed != size ||
        TriceResolveLog(&rx, triceLog, triceLogElements) != TRICE_RX_RESULT_OK ||
        rx.bitWidth != 32 || rx.payloadBytes != 4u * rx.paramCount) {
        failures++;
        return;
    }
    printf("%u", (unsigned)rx.id);
    for (unsigned i = 0; i < rx.paramCount; ++i) {
        const uint8_t* value = rx.payload + 4u * i;
        uint32_t decoded = (uint32_t)value[0] | ((uint32_t)value[1] << 8) |
            ((uint32_t)value[2] << 16) | ((uint32_t)value[3] << 24);
        printf(":%u", (unsigned)decoded);
    }
    putchar('\n');
    records++;
}

int main(void) {
    UserNonBlockingDirectWrite8AuxiliaryFn = captureRecord;
    int x = 7;
    trice("msg:ctxlocal:no arguments\n");
    trice("msg:ctxonce:existing {original}\n", ++originalEvaluations);
    trice("msg:ctxexpr:expression\n");
    trice_0("msg:ctxlocal:fixed zero\n");
    trice_1("msg:ctxlocal:fixed one {original}\n", 22);
    {
        int blockValue = 11;
        trice("msg:ctxblock:block scope\n");
    }
    trice("msg:untouched\n");
    if (0) {
        trice("msg:ctxonce:not executed\n");
    }
    return failures != 0 || records != 7 || originalEvaluations != 1 || contextEvaluations != 1;
}
`

// cePoCRule is deliberately a fixture, not an implementation of the -ce CLI.
// The PoC isolates callsite injection from selector parsing and policy in A10.
type cePoCRule struct {
	selector, extension, expression string
}

// cePoCInjection records the source signature and its final target macro. Fixed
// arity macros need a new implementation name; generic macros count normally.
type cePoCInjection struct {
	line, originalArguments int
	macro, expression       string
}

// generateCEPoC runs the real Bind/ID engine on an enriched in-memory source
// view, then publishes only dictionaries and the experimental sidecar. This
// models the required pre-ID transformation without exposing a production flag.
func generateCEPoC(t *testing.T, project string) TriceIDLookUp {
	t.Helper()
	sourcePath := filepath.Join(project, "main.c")
	sourceBytes, err := os.ReadFile(sourcePath)
	require.NoError(t, err)
	source := string(sourceBytes)
	sites, diagnostics := scanBindSites(sourcePath, source)
	require.Empty(t, diagnostics)
	rules := []cePoCRule{
		{"ctxlocal", ", x={}", "x"},
		{"ctxonce", ", context={context}", "++contextEvaluations"},
		{"ctxexpr", ", next={next}", "x + 1"},
		{"ctxblock", ", block={}", "blockValue"},
	}
	var injections []cePoCInjection
	virtualSource := source
	// Reverse replacement preserves all original byte offsets and physical lines.
	for index := len(sites) - 1; index >= 0; index-- {
		site := sites[index]
		for _, rule := range rules {
			prefix := "msg:" + rule.selector + ":"
			if !strings.HasPrefix(site.format, prefix) {
				continue
			}
			args, splitErr := splitTriceParametersUntilClosingBracket(source[site.loc[6]:])
			require.NoError(t, splitErr)
			macro := site.macro
			if strings.HasPrefix(macro, "trice_") {
				macro = fmt.Sprintf("trice_%d", len(args)+1)
			}
			injections = append(injections, cePoCInjection{site.line, len(args), macro, rule.expression})
			format := "msg:" + strings.TrimPrefix(site.format, prefix)
			suffix := ""
			if strings.HasSuffix(format, `\n`) {
				format = strings.TrimSuffix(format, `\n`)
				suffix = `\n`
			}
			format += rule.extension + suffix
			args = append(args, rule.expression)
			call := macro + `("` + format + `", ` + strings.Join(args, ", ") + ")"
			end := findClosingParentis(source, site.loc[2]) + 1
			require.Greater(t, end, site.loc[6])
			virtualSource = virtualSource[:site.loc[0]] + call + virtualSource[end:]
			break
		}
	}
	require.Equal(t, strings.Count(source, "\n"), strings.Count(virtualSource, "\n"))

	// Only this private filesystem contains the enriched source. Repeated runs
	// use the previously published TIL and sidecar, just like an incremental bind.
	memory := &afero.Afero{Fs: afero.NewMemMapFs()}
	require.NoError(t, memory.MkdirAll(BindDir, 0o755))
	for _, path := range []string{FnJSON, LIFnJSON, filepath.Join(project, "triceConfig.h"), filepath.Join(BindDir, cePoCSidecar)} {
		content, readErr := os.ReadFile(path)
		if os.IsNotExist(readErr) {
			continue
		}
		require.NoError(t, readErr)
		require.NoError(t, memory.WriteFile(path, content, 0o644))
	}
	require.NoError(t, memory.WriteFile(sourcePath, []byte(virtualSource), 0o644))
	require.NoError(t, SubCmdIdBind(io.Discard, memory))
	afterBind, err := memory.ReadFile(sourcePath)
	require.NoError(t, err)
	require.Equal(t, virtualSource, string(afterBind), "seeded Bind infrastructure must already be sufficient")

	liBytes, err := memory.ReadFile(LIFnJSON)
	require.NoError(t, err)
	var locations TriceIDLookUpLI
	require.NoError(t, json.Unmarshal(liBytes, &locations))
	sidecar, err := memory.ReadFile(filepath.Join(BindDir, cePoCSidecar))
	require.NoError(t, err)
	var extension strings.Builder
	extension.WriteString("\n// Experimental A9 adapters: each expression occurs once in the target call.\n")
	for _, injection := range injections {
		var id TriceID
		for candidate, location := range locations {
			if location.Line == injection.line {
				id = candidate
				break
			}
		}
		require.NotZero(t, id)
		mode := fmt.Sprintf("TRICE_CE_POC_L%d", injection.line)
		siteName := bindSiteMacroName(cePoCKey, injection.line)
		parameters := []string{"ignoredImplementation", "tid", "format"}
		arguments := []string{"tid", "format"}
		for index := 0; index < injection.originalArguments; index++ {
			name := fmt.Sprintf("original%d", index)
			parameters = append(parameters, name)
			arguments = append(arguments, name)
		}
		arguments = append(arguments, "("+injection.expression+")")
		fmt.Fprintf(&extension, "#undef %s\n#define %s %s, iD(%d)\n", siteName, siteName, mode, id)
		fmt.Fprintf(&extension, "#define %s(%s) TRICE_INSERT_%s(%s)\n", mode, strings.Join(parameters, ", "), injection.macro, strings.Join(arguments, ", "))
	}
	sidecar = append(sidecar, extension.String()...)
	require.NoError(t, os.MkdirAll(BindDir, 0o755))
	require.NoError(t, os.WriteFile(filepath.Join(BindDir, cePoCSidecar), sidecar, 0o644))
	for _, path := range []string{FnJSON, LIFnJSON, filepath.Join(BindDir, "trice-fields.txt")} {
		content, readErr := memory.ReadFile(path)
		require.NoError(t, readErr)
		require.NoError(t, os.WriteFile(path, content, 0o644))
	}
	tilBytes, err := memory.ReadFile(FnJSON)
	require.NoError(t, err)
	var til TriceIDLookUp
	require.NoError(t, json.Unmarshal(tilBytes, &til))
	return til
}

// TestContextEnrichmentPoC proves direct-site CE with the product dispatcher,
// ID allocator, structured template parser, target producer, and RX resolver.
// It is opt-in with the other Bind compiler checks and creates no repo artifacts.
func TestContextEnrichmentPoC(t *testing.T) {
	bindIntegrationEnabled(t)
	compiler := firstAvailableCompiler("clang", "cc", "gcc")
	cppCompiler := firstAvailableCompiler("clang++", "c++", "g++")
	clangd := firstAvailableCompiler("clangd")
	require.NotEmpty(t, compiler, "A9 requires a C compiler")
	require.NotEmpty(t, cppCompiler, "A9 requires a C++ compiler")
	require.NotEmpty(t, clangd, "A9 requires clangd for the language-server proof")
	project := t.TempDir()
	_, teardown := prepareOSBindProject(t, project)
	defer teardown()
	sourcePath := writeBindIntegrationFile(t, project, "main.c", cePoCSource)
	writeBindIntegrationFile(t, project, "triceConfig.h", cePoCConfig)
	Srcs = ArrayFlag{sourcePath}
	til := generateCEPoC(t, project)

	// The metadata assertions are independent of macro generation: each logical
	// source case must get the exact final template and the actual emitted values.
	expected := []struct {
		format, values string
	}{
		{`msg:no arguments, x={x}\n`, ":7"},
		{`msg:existing {original}, context={context}\n`, ":1:1"},
		{`msg:expression, next={next}\n`, ":8"},
		{`msg:fixed zero, x={x}\n`, ":7"},
		{`msg:fixed one {original}, x={x}\n`, ":22:7"},
		{`msg:block scope, block={blockValue}\n`, ":11"},
		{`msg:untouched\n`, ""},
	}
	var expectedOutput strings.Builder
	for _, item := range expected {
		var matchedID TriceID
		for id, entry := range til {
			if entry.Strg == item.format {
				matchedID = id
				_, _, count := computeValues(entry, 32)
				assert.Equal(t, strings.Count(item.values, ":"), count, item.format)
				break
			}
		}
		require.NotZero(t, matchedID, item.format)
		fmt.Fprintf(&expectedOutput, "%d%s\n", matchedID, item.values)
	}
	require.Len(t, til, 8, "the non-executed site still has its own schema")

	// Capture every persistent PoC artifact; a second bind must reproduce all
	// bytes, including IDs, source locations, field counts, and adapter macros.
	paths := []string{sourcePath, filepath.Join(project, "triceConfig.h"), FnJSON, LIFnJSON, filepath.Join(BindDir, cePoCSidecar), filepath.Join(BindDir, "trice-fields.txt")}
	before := make(map[string][]byte)
	for _, path := range paths {
		content, err := os.ReadFile(path)
		require.NoError(t, err)
		before[path] = content
	}
	assert.Equal(t, cePoCSource, string(before[sourcePath]))
	assert.Equal(t, til, generateCEPoC(t, project))
	for _, path := range paths {
		content, err := os.ReadFile(path)
		require.NoError(t, err)
		assert.Equal(t, before[path], content, path)
	}
	table, err := til.toListTilC("til.c")
	require.NoError(t, err)
	writeBindIntegrationFile(t, project, "til.c", string(table))
	sourceDir := filepath.Join(bindRepositoryRoot(t), "src")
	library, err := filepath.Glob(filepath.Join(sourceDir, "[a-z]*.c"))
	require.NoError(t, err)
	common := []string{"-Wall", "-Wextra", "-Werror", "-I", project, "-I", BindDir, "-I", sourceDir}
	args := append([]string{"-std=c11", "-c"}, common...)
	args = append(args, library...)
	args = append(args, "til.c")
	command := exec.Command(compiler, args...)
	command.Dir = project
	output, err := command.CombinedOutput()
	require.NoError(t, err, "%s", output)
	objects, err := filepath.Glob(filepath.Join(project, "*.o"))
	require.NoError(t, err)

	for _, language := range []struct{ name, standard, compiler string }{
		{"c", "c11", compiler}, {"c++", "c++17", cppCompiler},
	} {
		t.Run(language.name, func(t *testing.T) {
			object := filepath.Join(project, "main-"+language.name+".o")
			args := append([]string{"-x", language.name, "-std=" + language.standard}, common...)
			args = append(args, "-c", sourcePath, "-o", object)
			compile := exec.Command(language.compiler, args...)
			output, err := compile.CombinedOutput()
			require.NoError(t, err, "%s", output)
			executable := filepath.Join(project, "poc-"+language.name)
			if runtime.GOOS == "windows" {
				executable += ".exe"
			}
			linkArgs := append([]string{object}, objects...)
			linkArgs = append(linkArgs, "-o", executable)
			output, err = exec.Command(language.compiler, linkArgs...).CombinedOutput()
			require.NoError(t, err, "%s", output)
			output, err = exec.Command(executable).CombinedOutput()
			require.NoError(t, err, "payload resolver or evaluation count failed: %s", output)
			// Windows text streams use CRLF; the record contents must still match.
			assert.Equal(t, expectedOutput.String(), strings.ReplaceAll(string(output), "\r\n", "\n"))

			// clangd consumes the same compiler flags as the successful real build.
			commands := []map[string]any{{"directory": project, "file": sourcePath, "arguments": append([]string{language.compiler}, args...)}}
			compileDB, err := json.MarshalIndent(commands, "", "  ")
			require.NoError(t, err)
			writeBindIntegrationFile(t, project, "compile_commands.json", string(compileDB))
			output, err = exec.Command(clangd, "--check="+sourcePath, "--compile-commands-dir="+project, "--log=info").CombinedOutput()
			require.NoError(t, err, "%s", output)
			assert.Contains(t, string(output), "Loaded compilation database", "the editor check must use the real build configuration")
			assert.Contains(t, string(output), "0 errors", "clangd must accept the expanded callsites")
			t.Logf("%s: seven binary records match TIL; evaluation counts and clangd checks passed", language.name)
		})
	}
	// The adapter retains ordinary compiler diagnostics for unavailable context.
	sidecarPath := filepath.Join(BindDir, cePoCSidecar)
	validSidecar := before[sidecarPath]
	invalidSidecar := bytes.ReplaceAll(validSidecar, []byte("(x + 1)"), []byte("(ceMissingLocal + 1)"))
	require.NotEqual(t, validSidecar, invalidSidecar)
	require.NoError(t, os.WriteFile(sidecarPath, invalidSidecar, 0o644))
	args = append([]string{"-std=c11", "-fsyntax-only"}, common...)
	args = append(args, sourcePath)
	output, err = exec.Command(compiler, args...).CombinedOutput()
	require.Error(t, err, "an out-of-scope CE expression must remain a compiler error")
	assert.Contains(t, string(output), "ceMissingLocal")
	output, err = exec.Command(clangd, "--check="+sourcePath, "--compile-commands-dir="+project, "--log=info").CombinedOutput()
	require.Error(t, err, "the language server must also diagnose unavailable context")
	assert.Contains(t, string(output), "ceMissingLocal")
	require.NoError(t, os.WriteFile(sidecarPath, validSidecar, 0o644))
	currentSource, err := os.ReadFile(sourcePath)
	require.NoError(t, err)
	assert.Equal(t, cePoCSource, string(currentSource))
}
