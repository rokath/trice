// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"encoding/json"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"runtime"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// runContextCLI isolates repeatable flags between public command invocations.
func runContextCLI(t *testing.T, fs *afero.Afero, options ...string) (string, error) {
	t.Helper()
	FlagsInit()
	id.Srcs, id.ExcludeSrcs, id.IDRange = nil, nil, nil
	id.TriceAliases, id.TriceSAliases = nil, nil
	t.Cleanup(func() { id.Srcs, id.ExcludeSrcs = nil, nil; FlagsInit() })
	var output bytes.Buffer
	err := Handler(&output, fs, append([]string{"trice"}, options...))
	return output.String(), err
}

// TestContextEnrichmentCLI checks public parsing, late user-label registration,
// repeated options, command-local reset, and bind-only flag ownership.
func TestContextEnrichmentCLI(t *testing.T) {
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	require.NoError(t, fs.WriteFile("main.c", []byte("#include \"trice.h\"\ntrice(\"motor:ready\");\n"), 0o644))
	require.NoError(t, fs.WriteFile("til.json", []byte("{}"), 0o644))
	require.NoError(t, fs.WriteFile("li.json", []byte("{}"), 0o644))
	options := []string{"bind", "-src", "main.c", "-til", "til.json", "-li", "li.json", "-buildDir", "build", "-IDMin", "1000", "-IDMax", "1999", "-IDMethod", "upward"}
	_, err := runContextCLI(t, fs, append(append([]string{}, options...), "-ce", `MOTOR:", x={}", pos.x`, "-ce", `motor:", y={}", pos.y`, "-ulabel", "motor")...)
	require.NoError(t, err)
	content, err := fs.ReadFile("til.json")
	require.NoError(t, err)
	var til id.TriceIDLookUp
	require.NoError(t, json.Unmarshal(content, &til))
	assert.Equal(t, "motor:ready, x={pos.x}, y={pos.y}", til[1000].Strg)
	assert.Nil(t, fsScInsert.Lookup("ce"))
	assert.Nil(t, fsScClean.Lookup("ce"))
	assert.Nil(t, fsScGenerate.Lookup("ce"))

	_, err = runContextCLI(t, fs, options...)
	require.NoError(t, err)
	content, err = fs.ReadFile("til.json")
	require.NoError(t, err)
	require.NoError(t, json.Unmarshal(content, &til))
	assert.Equal(t, "motor:ready", til[1001].Strg, "omitting -ce disables previous enrichment")
	help, err := RenderHelpText("-bind")
	require.NoError(t, err)
	assert.Contains(t, help, "-ce")
	assert.Contains(t, help, `Search UM for "bind-limits".`)
}

// contextTargetConfig enables actual unframed target records and the generated
// C resolver. Disable switches remain overridable for the no-evaluation tests.
const contextTargetConfig = `// SPDX-License-Identifier: MIT
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#ifndef TRICE_CLEAN
#define TRICE_CLEAN 0
#endif
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

// contextTargetSource exercises local scope and real record transport, including
// names that would collide with naive adapter parameters. Canonical examples
// are inserted from triceCheck.c so the documented fixture is actually tested.
const contextTargetSource = `// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <stdio.h>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
#include "trice.h"
#include "triceRx.h"
#ifdef __COUNTER__
#error "This test must compile without __COUNTER__."
#endif

// CE_GLOBALS
static unsigned originalEvaluations;
static unsigned contextEvaluations;
static unsigned records;
static unsigned failures;

// An inline helper sees its own parameter, never a caller-local variable.
static inline void logLocal(int local) {
    (void)local;
    trice32("info:inlinectx:Inline\n");
}

// Resolve every actual record against public generate -logC output before
// forwarding its original bytes to the Go decoder through stdout.
static void capture(const uint8_t *data, size_t size) {
    triceRx_t rx;
    int consumed = TriceParseRecord(&rx, data, size);
    int resolved = consumed > 0 ? (int)TriceResolveLog(&rx, triceLog, triceLogElements) : -1;
    // Stack-buffer transport rounds records up to a 32-bit boundary; the
    // record parser deliberately returns the unpadded logical length.
    if (consumed <= 0 || (((size_t)consumed + 3u) & ~(size_t)3u) != size ||
        resolved != TRICE_RX_RESULT_OK) {
        fprintf(stderr, "record validation: consumed=%d size=%zu resolved=%d\n", consumed, size, resolved);
        failures++;
        return;
    }
    fwrite(data, 1, size, stdout);
    records++;
}

int main(void) {
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_BINARY);
#endif
    UserNonBlockingDirectWrite8AuxiliaryFn = capture;
    // CE_EXAMPLES
    trice8("info:tiny:Small\n");
    TRICE16_0(Id(0), "info:small:Short\n");
    trice64("info:wide:Energy\n");
    trice32("info:once:Count {original}\n", ++originalEvaluations);
    if (0) {
        trice32("info:once:Unexecuted {original}\n", ++originalEvaluations);
    }
    {
        int privateValue = 11;
        (void)privateValue;
        trice32("info:left:Left\n");
    }
    {
        int otherValue = 22;
        (void)otherValue;
        trice32("info:right:Right\n");
    }
    {
        int tid = 1, format = 2, arg0 = 3, trice_ce_tid = 4;
        (void)tid; (void)format; (void)arg0; (void)trice_ce_tid;
        trice32("info:hygiene:Names\n");
    }
    triceS("info:label:Device {device:%s}\n", "pump");
    trice32_1(
        "info:pos:Multiline {sample}\n",
        5
    );
    logLocal(99);
#if TRICE_OFF || TRICE_CLEAN
    return failures != 0 || records != 0 || originalEvaluations != 0 || contextEvaluations != 0;
#else
    if (failures != 0 || records != 14 || originalEvaluations != 1 || contextEvaluations != 1) {
        fprintf(stderr, "failures=%u records=%u original=%u context=%u\n", failures, records, originalEvaluations, contextEvaluations);
    }
    return failures != 0 || records != 14 || originalEvaluations != 1 || contextEvaluations != 1;
#endif
}
`

// TestContextEnrichmentTargetToDecoder proves the complete public workflow:
// bind, generated C metadata, compiler/clangd, actual payload, text/JSON/KV.
func TestContextEnrichmentTargetToDecoder(t *testing.T) {
	if os.Getenv("TRICE_BIND_INTEGRATION") != "1" {
		t.Skip("set TRICE_BIND_INTEGRATION=1 for the CE compiler and decoder integration")
	}
	cc, err := exec.LookPath("clang")
	require.NoError(t, err, "the CE integration requires Clang")
	cxx, err := exec.LookPath("clang++")
	require.NoError(t, err, "the CE integration requires Clang++")
	clangd, err := exec.LookPath("clangd")
	require.NoError(t, err, "the CE integration requires clangd")
	_, filename, _, ok := runtime.Caller(0)
	require.True(t, ok)
	root := filepath.Clean(filepath.Join(filepath.Dir(filename), "..", ".."))
	canonical, err := os.ReadFile(filepath.Join(root, "_test", "testdata", "triceCheck.c"))
	require.NoError(t, err)
	fixture := string(canonical)
	globalStart := strings.Index(fixture, "// Context Enrichment globals")
	require.NotEqual(t, -1, globalStart)
	globals := strings.SplitN(fixture[globalStart:], "\n\n", 2)[0]
	start := strings.Index(fixture, "// Context Enrichment examples:")
	end := strings.Index(fixture, "// End Context Enrichment examples;")
	require.True(t, start >= 0 && end > start)
	pattern := regexp.MustCompile(`(?m)^\s*break; case __LINE__: (.+?);\s*//exp:`)
	var calls []string
	for _, match := range pattern.FindAllStringSubmatch(fixture[start:end], -1) {
		calls = append(calls, match[1]+";")
	}
	require.Len(t, calls, 4, "all canonical CE examples must participate in the real target test")
	source := strings.ReplaceAll(contextTargetSource, "// CE_GLOBALS", globals)
	source = strings.ReplaceAll(source, "// CE_EXAMPLES", strings.Join(calls, "\n    "))
	project := t.TempDir()
	sourcePath := filepath.Join(project, "main.c")
	tilPath, liPath := filepath.Join(project, "til.json"), filepath.Join(project, "li.json")
	buildDir := filepath.Join(project, "build")
	fs := &afero.Afero{Fs: afero.NewOsFs()}
	require.NoError(t, os.WriteFile(sourcePath, []byte(source), 0o644))
	require.NoError(t, fs.WriteFile(tilPath, []byte("{}"), 0o644))
	require.NoError(t, fs.WriteFile(liPath, []byte("{}"), 0o644))
	require.NoError(t, os.WriteFile(filepath.Join(project, "triceConfig.h"), []byte(contextTargetConfig), 0o644))
	bind := []string{"bind", "-src", sourcePath, "-til", tilPath, "-li", liPath, "-buildDir", buildDir, "-IDMin", "1000", "-IDMax", "1999", "-IDMethod", "upward"}
	rules := []string{
		`pos:", x={}, y={}", pos.x, pos.y`,
		`speed:", m/s=%f", aFloat(velocity)`,
		`tiny:", b={byte}", -7`,
		`small:", n={number}", -123`,
		`wide:", J={energy:%.2f}", aDouble(12.25)`,
		`once:", context={context}", ++contextEvaluations`,
		`left:", value={value}", privateValue`,
		`right:", value={value}", otherValue`,
		`hygiene:", total={total}", tid + format + arg0 + trice_ce_tid`,
		`label:" online"`,
		`inlinectx:", local={local}", local`,
	}
	for _, rule := range rules {
		bind = append(bind, "-ce", rule)
	}
	output, err := runContextCLI(t, fs, bind...)
	require.NoError(t, err, output)
	bound, err := os.ReadFile(sourcePath)
	require.NoError(t, err)
	for _, call := range calls {
		assert.Contains(t, string(bound), call)
	}
	output, err = runContextCLI(t, fs, bind...)
	require.NoError(t, err, output)
	repeated, err := os.ReadFile(sourcePath)
	require.NoError(t, err)
	assert.Equal(t, bound, repeated)
	tablePath := filepath.Join(project, "triceLog.c")
	output, err = runContextCLI(t, fs, "generate", "-logC", tablePath, "-src", sourcePath, "-til", tilPath, "-li", liPath, "-buildDir", buildDir)
	require.NoError(t, err, output)

	library, err := filepath.Glob(filepath.Join(root, "src", "[a-z]*.c"))
	require.NoError(t, err)
	common := []string{"-Wall", "-Wextra", "-Werror", "-Wno-builtin-macro-redefined", "-U__COUNTER__", "-I", project, "-I", buildDir, "-I", filepath.Join(root, "src")}
	args := append([]string{"-std=c11", "-c"}, common...)
	args = append(args, library...)
	args = append(args, tablePath)
	compile := exec.Command(cc, args...)
	compile.Dir = project
	buildOutput, err := compile.CombinedOutput()
	require.NoError(t, err, "%s", buildOutput)
	objects, err := filepath.Glob(filepath.Join(project, "*.o"))
	require.NoError(t, err)
	for _, language := range []struct{ name, standard, compiler string }{{"c", "c11", cc}, {"c++", "c++17", cxx}} {
		t.Run(language.name, func(t *testing.T) {
			object := filepath.Join(project, "main-"+language.name+".o")
			args := append([]string{"-x", language.name, "-std=" + language.standard}, common...)
			args = append(args, "-c", sourcePath, "-o", object)
			buildOutput, err := exec.Command(language.compiler, args...).CombinedOutput()
			require.NoError(t, err, "%s", buildOutput)
			commands := []map[string]any{{"directory": project, "file": sourcePath, "arguments": append([]string{language.compiler}, args...)}}
			database, err := json.Marshal(commands)
			require.NoError(t, err)
			require.NoError(t, os.WriteFile(filepath.Join(project, "compile_commands.json"), database, 0o644))
			buildOutput, err = exec.Command(clangd, "--check="+sourcePath, "--compile-commands-dir="+project).CombinedOutput()
			require.NoError(t, err, "%s", buildOutput)
			assert.Contains(t, string(buildOutput), "Loaded compilation database")
			assert.Contains(t, string(buildOutput), "0 errors")
			for _, mode := range []string{"active", "TRICE_OFF", "TRICE_CLEAN"} {
				t.Run(mode, func(t *testing.T) {
					if mode != "active" {
						disabledArgs := append(append([]string{}, args...), "-D"+mode+"=1")
						buildOutput, err := exec.Command(language.compiler, disabledArgs...).CombinedOutput()
						require.NoError(t, err, "%s", buildOutput)
					}
					executable := filepath.Join(project, "ce-target")
					if runtime.GOOS == "windows" {
						executable += ".exe"
					}
					linkArgs := append([]string{object}, objects...)
					linkArgs = append(linkArgs, "-o", executable)
					buildOutput, err := exec.Command(language.compiler, linkArgs...).CombinedOutput()
					require.NoError(t, err, "%s", buildOutput)
					var diagnostics bytes.Buffer
					run := exec.Command(executable)
					run.Stderr = &diagnostics
					wire, err := run.Output()
					require.NoError(t, err, "record validation or evaluation count failed: %s", diagnostics.String())
					if mode != "active" {
						assert.Empty(t, wire, "disabled logging emits no data and evaluates neither original nor CE expressions")
						return
					}
					assertContextDecodedOutput(t, fs, project, tilPath, wire)
				})
			}
		})
	}

	// Invalid local context remains a real compiler and editor error. Bind does
	// not invent a C symbol table or impose scope from any other log location.
	badRules := append([]string{}, bind...)
	for i, option := range badRules {
		if strings.HasPrefix(option, "left:") {
			badRules[i] = `left:", value={value}", ceMissingLocal`
		}
	}
	output, err = runContextCLI(t, fs, badRules...)
	require.NoError(t, err, output)
	args = append(append([]string{"-std=c11", "-fsyntax-only"}, common...), sourcePath)
	buildOutput, err = exec.Command(cc, args...).CombinedOutput()
	require.Error(t, err)
	assert.Contains(t, string(buildOutput), "ceMissingLocal")
	buildOutput, err = exec.Command(clangd, "--check="+sourcePath, "--compile-commands-dir="+project).CombinedOutput()
	require.Error(t, err)
	assert.Contains(t, string(buildOutput), "ceMissingLocal")
}

// assertContextDecodedOutput compares complete messages across all renderers
// and checks independently typed fields from actual C/C++ target bytes.
func assertContextDecodedOutput(t *testing.T, fs *afero.Afero, project, tilPath string, wire []byte) {
	t.Helper()
	capture := filepath.Join(project, "capture.bin")
	require.NoError(t, fs.WriteFile(capture, wire, 0o644))
	wantMessages := []string{
		"Position sample, x=-444, y=77", "Speed sample, m/s=33.330002",
		"Moving sample=3, x=-444, y=77, m/s=33.330002", "Fixed sample=4, x=-444, y=77",
		"Small, b=-7", "Short, n=-123", "Energy, J=12.25", "Count 1, context=1",
		"Left, value=11", "Right, value=22", "Names, total=10", "Device pump online",
		"Multiline 5, x=-444, y=77", "Inline, local=99",
	}
	for _, format := range []string{"text", "json", "kv"} {
		// Unrouted auxiliary output retains the doubled ID of 16-bit-stamped
		// records; the public decoder option describes this actual transport.
		output, err := runContextCLI(t, fs, "log", "-p", "FILEBUFFER", "-args", capture, "-pf", "none", "-d16", "-til", tilPath, "-li", "off", "-hs", "off", "-ts", "off", "-prefix", "", "-suffix", "", "-color", "none", "-logFormat", format)
		require.NoError(t, err)
		lines := strings.Split(strings.TrimSuffix(output, "\n"), "\n")
		require.Len(t, lines, len(wantMessages), "%s output: %s", format, output)
		switch format {
		case "text":
			assert.Equal(t, wantMessages, lines)
		case "json":
			for i, line := range lines {
				var event struct {
					Tag     string         `json:"tag"`
					Message string         `json:"message"`
					Fields  map[string]any `json:"fields"`
				}
				require.NoError(t, json.Unmarshal([]byte(line), &event))
				assert.Equal(t, "INFO", event.Tag)
				// Structured output preserves the message's final LF inside its
				// string value; the surrounding record has its own line ending.
				assert.Equal(t, wantMessages[i]+"\n", event.Message)
				switch i {
				case 0, 2, 3, 12:
					assert.Equal(t, float64(-444), event.Fields["pos.x"])
					assert.Equal(t, float64(77), event.Fields["pos.y"])
				case 4:
					assert.Equal(t, float64(-7), event.Fields["byte"])
				case 5:
					assert.Equal(t, float64(-123), event.Fields["number"])
				case 6:
					assert.Equal(t, 12.25, event.Fields["energy"])
				case 11:
					assert.Equal(t, "pump", event.Fields["device"])
				}
			}
		case "kv":
			for i, line := range lines {
				assert.Contains(t, line, `message="`+wantMessages[i]+`\n"`)
			}
			assert.Contains(t, lines[0], "field.pos.x=-444")
			assert.Contains(t, lines[0], "field.pos.y=77")
		}
	}

	// Turning color handling off keeps known source tags in the message, while
	// configured free CE selectors have already been removed at bind time.
	output, err := runContextCLI(t, fs, "log", "-p", "FILEBUFFER", "-args", capture, "-pf", "none", "-d16", "-til", tilPath, "-li", "off", "-hs", "off", "-ts", "off", "-color", "off", "-logFormat", "json")
	require.NoError(t, err)
	lines := strings.Split(strings.TrimSuffix(output, "\n"), "\n")
	require.Len(t, lines, len(wantMessages))
	for i, line := range lines {
		var event struct {
			Tag     string `json:"tag"`
			Message string `json:"message"`
		}
		require.NoError(t, json.Unmarshal([]byte(line), &event))
		assert.Equal(t, "INFO", event.Tag)
		assert.Equal(t, "info:"+wantMessages[i]+"\n", event.Message)
	}
}
