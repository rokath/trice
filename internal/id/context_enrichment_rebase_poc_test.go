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
	"regexp"
	"runtime"
	"slices"
	"sort"
	"strconv"
	"strings"
	"testing"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// ceRebasePoCWrappers deliberately uses caller-local context in PAIR and
// wrapper-local context in CHOICE. The two CHOICE branches have disjoint scopes.
const ceRebasePoCWrappers = `// SPDX-License-Identifier: MIT
#ifndef CE_POC_WRAPPERS_H
#define CE_POC_WRAPPERS_H
#include "trice.h"
#define SIMPLE(value) do { trice32("msg:single:single %d\n", value); } while (0)
#define PAIR(first, second) do { \
    trice32("msg:left:left %d\n", first); \
    trice32_1("msg:right:right %d\n", second); \
} while (0)
#define CHOICE(value) do { \
    if (value) { \
        int branchLeft = 33; (void)branchLeft; \
        trice32_0("msg:branchleft:choice left\n"); \
    } else { \
        int branchRight = 44; (void)branchRight; \
        trice32("msg:branchright:choice right\n"); \
    } \
} while (0)
#endif
`

// ceRebasePoCSource contains ordinary Bind-supported constructs. Experimental
// routing enters through a forced include, never by changing these call sites.
const ceRebasePoCSource = `// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <stdio.h>
#include "trice.h"
#include "triceRx.h"
#include "wrappers.h"
#ifndef CE_POC_SIMPLE_ONLY
#define CE_POC_SIMPLE_ONLY 0
#endif
#ifndef CE_POC_PAD
#define CE_POC_PAD 0
#endif
#if CE_POC_PAD && defined(__COUNTER__)
enum { unrelated0 = __COUNTER__ };
#if CE_POC_PAD >= 7
enum { unrelated1 = __COUNTER__, unrelated2 = __COUNTER__,
       unrelated3 = __COUNTER__, unrelated4 = __COUNTER__, unrelated5 = __COUNTER__, unrelated6 = __COUNTER__ };
#endif
#endif
static unsigned originalEvaluations;
static unsigned contextEvaluations;
static unsigned records;
static unsigned failures;

// Verify the real producer against the final CE dictionary before printing
// ordered payload values. All records in this scope proof use 32-bit values.
static void captureRecord(const uint8_t *data, size_t size) {
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
        const uint8_t *value = rx.payload + 4u * i;
        uint32_t decoded = (uint32_t)value[0] | ((uint32_t)value[1] << 8) |
            ((uint32_t)value[2] << 16) | ((uint32_t)value[3] << 24);
        printf(":%u", (unsigned)decoded);
    }
    putchar('\n');
    records++;
}

int main(void) {
    UserNonBlockingDirectWrite8AuxiliaryFn = captureRecord;
    int singleLocal = 5; (void)singleLocal;
    SIMPLE(3);
#if !CE_POC_SIMPLE_ONLY
    {
        int leftLocal = 11, rightLocal = 22;
        (void)leftLocal; (void)rightLocal;
        PAIR(++originalEvaluations, 8);
    }
    {
        int leftLocal = 111, rightLocal = 222;
        (void)leftLocal; (void)rightLocal;
        PAIR(++originalEvaluations, 9);
    }
    CHOICE(1);
    CHOICE(0);
    { int onlyLeft = 55; (void)onlyLeft; trice32("msg:blockleft:local left\n"); } { int onlyRight = 66; (void)onlyRight; trice32("msg:blockright:local right\n"); }
    trice32("msg:unselected %d\n", 7); trice32("msg:also unselected\n");
    if (0) {
        int leftLocal = 1111, rightLocal = 2222;
        (void)leftLocal; (void)rightLocal;
        PAIR(++originalEvaluations, 10);
    }
#endif
#if TRICE_OFF || TRICE_CLEAN
    return failures != 0 || records != 0 || originalEvaluations != 0 || contextEvaluations != 0;
#elif CE_POC_SIMPLE_ONLY
    return failures != 0 || records != 1 || originalEvaluations != 0 || contextEvaluations != 0;
#else
    return failures != 0 || records != 11 || originalEvaluations != 2 || contextEvaluations != 2;
#endif
}
`

// ceRebasePoCProbe consumes the same one counter per expansion as the product
// route. Leaving a marker in -E output needs no C parser or target execution.
const ceRebasePoCProbe = `// SPDX-License-Identifier: MIT
#include "trice.h"
#undef TRICE_BIND_REBASE_AUTO_CAPTURE
#define TRICE_BIND_REBASE_AUTO_CAPTURE(constructor, implementation, counter, ...) \
    CE_POC_HIT(TRICE_BIND_REBASE_SCOPE, counter, __VA_ARGS__)
`

// ceRebasePoCRoute selects one adapter as preprocessing tokens. No C branch
// contains another site's expressions; existing Bind BEGIN/END guards remain.
const ceRebasePoCRoute = `// SPDX-License-Identifier: MIT
#include "trice.h"
#define CE_POC_KEY_I(scope, counter) CE_POC_CASE_##scope##_##counter
#define CE_POC_KEY(scope, counter) CE_POC_KEY_I(scope, counter)
#undef TRICE_BIND_REBASE_AUTO_CAPTURE
#define TRICE_BIND_REBASE_AUTO_CAPTURE(constructor, implementation, counter, ...) \
    CE_POC_KEY(TRICE_BIND_REBASE_SCOPE, counter)(constructor, implementation, __VA_ARGS__)
`

// ceRebasePoCProject holds numeric authorities from real Bind output. IDs are
// never guessed from matching text, even when wrappers expand several times.
type ceRebasePoCProject struct {
	til      TriceIDLookUp
	sites    map[TriceID]bindSite
	regions  map[string][]TriceID
	sources  map[string][]byte
	baseline map[string][]byte
}

// prepareCERebasePoC enriches a private source view through the existing schema
// validator and ID allocator. Only temporary dictionaries and sidecars change;
// the ordinary Bind-owned target source and wrapper definitions stay byte-equal.
func prepareCERebasePoC(t *testing.T, project string, fs *afero.Afero) ceRebasePoCProject {
	t.Helper()
	result := ceRebasePoCProject{sites: make(map[TriceID]bindSite), regions: make(map[string][]TriceID), sources: make(map[string][]byte), baseline: make(map[string][]byte)}
	require.NoError(t, SubCmdIdBind(io.Discard, fs))
	memory := &afero.Afero{Fs: afero.NewMemMapFs()}
	require.NoError(t, filepath.Walk(project, func(path string, info os.FileInfo, err error) error {
		if err != nil || info.IsDir() {
			return err
		}
		content, err := os.ReadFile(path)
		if err != nil {
			return err
		}
		result.baseline[path] = content
		return memory.WriteFile(path, content, 0o644)
	}))
	rules, err := parseContextRules([]string{
		`single:", context={context}", singleLocal`,
		`left:", context={context}, count={count}", leftLocal, ++contextEvaluations`,
		`right:", context={context}", rightLocal`,
		`branchleft:", context={context}", branchLeft`,
		`branchright:", context={context}", branchRight`,
		`blockleft:", context={context}", onlyLeft`,
		`blockright:", context={context}", onlyRight`,
	})
	require.NoError(t, err)
	logicalSites := make(map[string][]bindSite)
	for _, path := range Srcs {
		source := result.baseline[path]
		result.sources[path] = source
		sites, diagnostics := scanBindSites(path, string(source))
		require.Empty(t, diagnostics)
		masked := stripCComments(string(source))
		var edits []sourceEdit
		for index := range sites {
			site := &sites[index]
			format, selected, _ := selectContextRules(site.format, rules)
			if len(selected) == 0 {
				continue
			}
			// Deliberately bypass only A10's direct-site capability gate in this
			// private experiment; schema/arity/float validation stays unchanged.
			require.NoError(t, enrichBindSite(string(source), masked, site, format, selected))
			edits = append(edits, contextInsertEdit(string(source), *site, 0))
		}
		logicalSites[path] = sites
		virtual, editErr := applySourceEdits(string(source), edits)
		require.NoError(t, editErr)
		require.NoError(t, memory.WriteFile(path, []byte(virtual), 0o644))
	}
	require.NoError(t, SubCmdIdBind(io.Discard, memory))
	content, err := memory.ReadFile(FnJSON)
	require.NoError(t, err)
	require.NoError(t, json.Unmarshal(content, &result.til))

	// Gather numeric definition/location descriptors before replacing direct
	// modes. Generated rebase maps reference exactly these authoritative IDs.
	numbers := make(map[string]TriceID)
	sidecars := make(map[string][]byte)
	for _, path := range Srcs {
		plan := analyzeBindFile(bindFileInput{path: path, data: result.sources[path]})
		pathSidecar := filepath.Join(BindDir, plan.sidecarName)
		sidecar, readErr := memory.ReadFile(pathSidecar)
		require.NoError(t, readErr)
		history := parseBindHistoricalSites(sidecar, plan.key)
		require.Len(t, history, len(logicalSites[path]))
		for i, item := range history {
			site := logicalSites[path][i]
			site.id = item.id
			require.Equal(t, bindSiteFormat(site), result.til[item.id])
			result.sites[item.id] = site
		}
		for _, match := range regexp.MustCompile(`(?m)^#define (TRICE_BIND_ID_\w+) (\d+)u`).FindAllStringSubmatch(string(sidecar), -1) {
			id, parseErr := strconv.Atoi(match[2])
			require.NoError(t, parseErr)
			numbers[match[1]] = TriceID(id)
		}
		sidecars[pathSidecar] = sidecar
	}
	for path, sidecar := range sidecars {
		for _, match := range regexp.MustCompile(`(?m)^#define TRICE_BIND_REBASE_AUTO_APPLY_(\w+)[^\n]+`).FindAllStringSubmatch(string(sidecar), -1) {
			for _, ref := range regexp.MustCompile(`constructor\((TRICE_BIND_ID_\w+)\)`).FindAllStringSubmatch(match[0], -1) {
				require.NotZero(t, numbers[ref[1]])
				result.regions[match[1]] = append(result.regions[match[1]], numbers[ref[1]])
			}
		}
		// A single-site wrapper already has a line descriptor. This independent
		// subcase can use a normal CE adapter even without __COUNTER__.
		ordinary := regexp.MustCompile(`(?m)^(#define TRICE_BIND_SITE_\w+\s+)TRICE_BIND_AUTO,(\s+)(TRICE_BIND_DEFINITION_\w+|(?:iD|id|Id|ID)\(\d+u\))`)
		sidecar = ordinary.ReplaceAllFunc(sidecar, func(line []byte) []byte {
			parts := ordinary.FindStringSubmatch(string(line))
			id := numbers[strings.Replace(parts[3], "TRICE_BIND_DEFINITION_", "TRICE_BIND_ID_DEFINITION_", 1)]
			if id == 0 {
				literal := regexp.MustCompile(`\d+`).FindString(parts[3])
				value, _ := strconv.Atoi(literal)
				id = TriceID(value)
			}
			if result.sites[id].ce == nil {
				return line
			}
			return []byte(parts[1] + fmt.Sprintf("CE_POC_ADAPTER_%d,", id) + parts[2] + parts[3])
		})
		require.NoError(t, os.WriteFile(path, sidecar, 0o644))
	}
	for _, path := range []string{FnJSON, LIFnJSON, filepath.Join(BindDir, "trice-fields.txt")} {
		data, readErr := memory.ReadFile(path)
		require.NoError(t, readErr)
		require.NoError(t, os.WriteFile(path, data, 0o644))
	}
	return result
}

// ceRebasePoCAdapters uses a separate macro for each logical site. This is test
// plumbing, not a product renderer or an alternative implementation of -ce.
func ceRebasePoCAdapters(model ceRebasePoCProject) string {
	var output strings.Builder
	ids := make([]int, 0, len(model.sites))
	for id := range model.sites {
		ids = append(ids, int(id))
	}
	sort.Ints(ids)
	for _, number := range ids {
		site := model.sites[TriceID(number)]
		if site.ce == nil {
			continue
		}
		parameters := []string{"pocImplementation", "pocTid", "pocFormat"}
		arguments := []string{"pocTid", "pocFormat"}
		for i := 0; i < site.ce.originalArguments; i++ {
			parameter := fmt.Sprintf("pocArg%d", i)
			parameters = append(parameters, parameter)
			arguments = append(arguments, parameter)
		}
		for _, expression := range site.ce.expressions {
			arguments = append(arguments, "("+expression+")")
		}
		fmt.Fprintf(&output, "#define CE_POC_ADAPTER_%d(%s) TRICE_INSERT_%s(%s)\n", number, strings.Join(parameters, ", "), site.macro, strings.Join(arguments, ", "))
	}
	return output.String()
}

// ceRebasePoCMap builds exact absolute-counter tokens from one compiler's -E
// result. A base assertion prevents a stale map from selecting a neighboring
// valid case silently after unrelated counter consumption changes.
func ceRebasePoCMap(t *testing.T, model ceRebasePoCProject, preprocessed []byte) string {
	t.Helper()
	bases := make(map[string]int)
	for _, match := range regexp.MustCompile(`triceBindRebaseBase_(\w+)\s*=\s*(\d+)`).FindAllSubmatch(preprocessed, -1) {
		value, err := strconv.Atoi(string(match[2]))
		require.NoError(t, err)
		bases[string(match[1])] = value
	}
	ordinals := make(map[string]int)
	var output strings.Builder
	output.WriteString(ceRebasePoCRoute)
	output.WriteString(ceRebasePoCAdapters(model))
	for _, match := range regexp.MustCompile(`CE_POC_HIT\(\s*(\w+)\s*,\s*(\d+)\s*,`).FindAllSubmatch(preprocessed, -1) {
		scope := string(match[1])
		counter, err := strconv.Atoi(string(match[2]))
		require.NoError(t, err)
		base, exists := bases[scope]
		require.True(t, exists, "each hit needs an actual compiler-expanded rebase base")
		ordinal := ordinals[scope]
		require.Equal(t, ordinal, counter-base-1, "extra counter use inside a region is not supported")
		require.Less(t, ordinal, len(model.regions[scope]))
		id := model.regions[scope][ordinal]
		adapter := "TRICE_BIND_AUTO"
		if model.sites[id].ce != nil {
			adapter = fmt.Sprintf("CE_POC_ADAPTER_%d", id)
		}
		fmt.Fprintf(&output, "#define CE_POC_CASE_%s_%d(constructor, implementation, ...) do { TRICE_BIND_REBASE_STATIC_ASSERT(TRICE_BIND_REBASE_ACTIVE_BASE == %d, cePocStaleCounterMap); %s(implementation, constructor(%du), __VA_ARGS__); } while (0)\n", scope, counter, base, adapter, id)
		ordinals[scope]++
	}
	for scope, count := range ordinals {
		require.Len(t, model.regions[scope], count, "the probe must see every expansion in each active region")
	}
	require.NotEmpty(t, ordinals, "the full proof must exercise rebase, not only ordinary descriptors")
	return output.String()
}

// cePoCCompiler separates frontend identity, language and target configuration.
// macOS's gcc command can be Clang; version-based deduplication avoids counting
// that alias as evidence for GCC. Cross targets are compile-only evidence.
type cePoCCompiler struct {
	name, cc, cxx string
	flags         []string
	runnable      bool
}

// ceRebasePoCCompilers discovers installed tools without installing dependencies.
func ceRebasePoCCompilers(t *testing.T) []cePoCCompiler {
	t.Helper()
	var result []cePoCCompiler
	seen := make(map[string]bool)
	for _, candidate := range []cePoCCompiler{
		{name: "clang-host", cc: "clang", cxx: "clang++", runnable: true},
		{name: "gcc-host", cc: "gcc", cxx: "g++", runnable: true},
		{name: "gcc-15-host", cc: "gcc-15", cxx: "g++-15", runnable: true},
		{name: "gcc-14-host", cc: "gcc-14", cxx: "g++-14", runnable: true},
		{name: "gcc-arm-m0", cc: "arm-none-eabi-gcc", cxx: "arm-none-eabi-g++", flags: []string{"-mcpu=cortex-m0", "-mthumb"}},
		{name: "gcc-arm-m4", cc: "arm-none-eabi-gcc", cxx: "arm-none-eabi-g++", flags: []string{"-mcpu=cortex-m4", "-mthumb"}},
	} {
		cc, err := exec.LookPath(candidate.cc)
		if err != nil {
			t.Logf("unavailable: %s", candidate.cc)
			continue
		}
		cxx, err := exec.LookPath(candidate.cxx)
		if err != nil {
			t.Logf("unavailable: %s", candidate.cxx)
			continue
		}
		version, err := exec.Command(cc, "--version").CombinedOutput()
		require.NoError(t, err)
		identity := string(version) + strings.Join(candidate.flags, " ")
		if seen[identity] {
			t.Logf("alias, not another compiler: %s", candidate.cc)
			continue
		}
		seen[identity] = true
		candidate.cc, candidate.cxx = cc, cxx
		t.Logf("%s: %s; runnable=%v", candidate.name, strings.Split(string(version), "\n")[0], candidate.runnable)
		result = append(result, candidate)
	}
	require.NotEmpty(t, result, "this opt-in PoC requires at least one C/C++ toolchain")
	return result
}

// ceRebasePoCRun keeps compiler failures readable and uses a temporary working
// directory for object files; no build/test artifact is written to the repo.
func ceRebasePoCRun(project, compiler string, args []string) ([]byte, error) {
	command := exec.Command(compiler, args...)
	command.Dir = project
	return command.CombinedOutput()
}

// ceRebasePoCExpected checks semantic output independently of routing tokens.
// Format lookup is only an assertion helper, never an ID-assignment mechanism.
func ceRebasePoCExpected(t *testing.T, model ceRebasePoCProject, simple bool) string {
	t.Helper()
	want := []struct{ format, values string }{
		{`msg:single %d, context={context}\n`, ":3:5"},
		{`msg:left %d, context={context}, count={count}\n`, ":1:11:1"},
		{`msg:right %d, context={context}\n`, ":8:22"},
		{`msg:left %d, context={context}, count={count}\n`, ":2:111:2"},
		{`msg:right %d, context={context}\n`, ":9:222"},
		{`msg:choice left, context={context}\n`, ":33"},
		{`msg:choice right, context={context}\n`, ":44"},
		{`msg:local left, context={context}\n`, ":55"},
		{`msg:local right, context={context}\n`, ":66"},
		{`msg:unselected %d\n`, ":7"},
		{`msg:also unselected\n`, ""},
	}
	if simple {
		want = want[:1]
	}
	var output strings.Builder
	for _, item := range want {
		var ids []TriceID
		for id, site := range model.sites {
			if site.format == item.format {
				ids = append(ids, id)
			}
		}
		require.Len(t, ids, 1, item.format)
		fmt.Fprintf(&output, "%d%s\n", ids[0], item.values)
	}
	return output.String()
}

// TestContextEnrichmentRebasePoC evaluates a two-pass alternative without
// enabling any previously rejected production -ce construct. Each available
// toolchain probes and compiles its own expansion map in six language modes.
func TestContextEnrichmentRebasePoC(t *testing.T) {
	bindIntegrationEnabled(t)
	compilers := ceRebasePoCCompilers(t)
	project := t.TempDir()
	fs, teardown := prepareOSBindProject(t, project)
	defer teardown()
	writeBindIntegrationFile(t, project, "triceConfig.h", cePoCConfig)
	source := writeBindIntegrationFile(t, project, "main.c", ceRebasePoCSource)
	header := writeBindIntegrationFile(t, project, "wrappers.h", ceRebasePoCWrappers)
	Srcs = ArrayFlag{source, header}
	model := prepareCERebasePoC(t, project, fs)
	repeated := prepareCERebasePoC(t, project, fs)
	assert.Equal(t, model.til, repeated.til, "repeating private CE binding preserves schemas and IDs")
	assert.Equal(t, model.regions, repeated.regions, "region identities and expansion order remain stable")
	assert.Equal(t, model.sources, repeated.sources, "repeat generation never edits the user's bound source")
	table, err := model.til.toListTilC("til.c")
	require.NoError(t, err)
	writeBindIntegrationFile(t, project, "til.c", string(table))
	probe := writeBindIntegrationFile(t, project, "ce-probe.h", ceRebasePoCProbe+ceRebasePoCAdapters(model))
	route := filepath.Join(project, "ce-route.h")
	sourceDir := filepath.Join(bindRepositoryRoot(t), "src")
	want := ceRebasePoCExpected(t, model, false)

	for _, compiler := range compilers {
		t.Run(compiler.name, func(t *testing.T) {
			common := append([]string{"-Wall", "-Wextra", "-Werror", "-I", project, "-I", BindDir, "-I", sourceDir, "-I", filepath.Join(sourceDir, "default_conf")}, compiler.flags...)
			var objects []string
			if compiler.runnable {
				library, globErr := filepath.Glob(filepath.Join(sourceDir, "[a-z]*.c"))
				require.NoError(t, globErr)
				for index, path := range append(library, filepath.Join(project, "til.c")) {
					object := filepath.Join(project, fmt.Sprintf("library-%s-%d.o", compiler.name, index))
					args := append(append([]string{}, common...), "-std=c11", "-c", path, "-o", object)
					output, buildErr := ceRebasePoCRun(project, compiler.cc, args)
					require.NoError(t, buildErr, "%s", output)
					objects = append(objects, object)
				}
			}
			for _, standard := range []string{"c99", "c11", "c17", "c++11", "c++17", "c++20"} {
				t.Run(standard, func(t *testing.T) {
					frontend, language := compiler.cc, "c"
					if strings.HasPrefix(standard, "c++") {
						frontend, language = compiler.cxx, "c++"
					}
					base := append(append([]string{}, common...), "-x", language, "-std="+standard)
					// Existing rebase guards subtract distinct enum types. C++20
					// deprecates that operation even before experimental CE routing.
					// Record the strict limitation, then isolate it with one narrowly
					// scoped warning downgrade for the actual C++20 CE experiment.
					if standard == "c++20" {
						baseline := filepath.Join(project, "baseline")
						for path, data := range model.baseline {
							relative, relErr := filepath.Rel(project, path)
							require.NoError(t, relErr)
							writeBindIntegrationFile(t, baseline, relative, string(data))
						}
						strict := append([]string{"-Wall", "-Wextra", "-Werror", "-x", language, "-std=" + standard, "-I", baseline, "-I", filepath.Join(baseline, "build", "triceIDs"), "-I", sourceDir, "-I", filepath.Join(sourceDir, "default_conf")}, compiler.flags...)
						strict = append(strict, "-fsyntax-only", filepath.Join(baseline, "main.c"))
						output, strictErr := ceRebasePoCRun(project, frontend, strict)
						if strictErr != nil {
							assert.Contains(t, string(output), "enum-enum-conversion", "the baseline limitation must be identified, not assumed")
							t.Log("ordinary Bind already rejects strict C++20: deprecated arithmetic between distinct enum types")
						}
						warning := "-Wno-error=deprecated-enum-enum-conversion"
						if strings.Contains(compiler.name, "clang") {
							warning = "-Wno-error=deprecated-anon-enum-enum-conversion"
						}
						base = append(base, warning)
					}
					var previous string
					for _, padding := range []string{"0", "1", "7"} {
						flags := append(append([]string{}, base...), "-DCE_POC_PAD="+padding)
						probeArgs := append(append([]string{}, flags...), "-include", probe, "-E", "-P", source)
						preprocessed, probeErr := ceRebasePoCRun(project, frontend, probeArgs)
						require.NoError(t, probeErr, "%s", preprocessed)
						mapping := ceRebasePoCMap(t, model, preprocessed)
						if padding == "0" {
							repeatOutput, repeatErr := ceRebasePoCRun(project, frontend, probeArgs)
							require.NoError(t, repeatErr, "%s", repeatOutput)
							assert.Equal(t, mapping, ceRebasePoCMap(t, model, repeatOutput), "same compiler and flags reproduce the exact dispatch header")
						}
						if previous != "" {
							assert.NotEqual(t, previous, mapping, "unrelated counter use changes the per-build map, not the schema IDs")
							staleArgs := append(append([]string{}, flags...), "-include", route, "-fsyntax-only", source)
							output, staleErr := ceRebasePoCRun(project, frontend, staleArgs)
							require.Error(t, staleErr, "a stale map must never silently select another adapter")
							if padding == "1" {
								assert.Regexp(t, `(?i)(division by zero|not an integer constant|not a constant expression|not an integer constant expression)`, string(output), "a one-step shift still finds a token, so the base guard must reject it")
							}
						}
						previous = mapping
						require.NoError(t, os.WriteFile(route, []byte(mapping), 0o644))
						object := filepath.Join(project, "application.o")
						compile := append(append([]string{}, flags...), "-include", route, "-c", source, "-o", object)
						output, buildErr := ceRebasePoCRun(project, frontend, compile)
						require.NoError(t, buildErr, "%s", output)
						if compiler.runnable {
							ceRebasePoCExecute(t, project, frontend, object, objects, want)
						}
					}
					if compiler.name == "clang-host" && (standard == "c11" || standard == "c++17") {
						args := append(append([]string{}, base...), "-DCE_POC_PAD=7", "-include", route, "-c", source, "-o", filepath.Join(project, "editor.o"))
						ceRebasePoCEditor(t, project, source, frontend, args, "")
					}
					ceRebasePoCVariants(t, project, source, route, frontend, base, objects, model, compiler.runnable)
					t.Logf("%s %s: three counter offsets, scopes, stale-map rejection and no-counter variants passed; runtime evidence=%v", compiler.name, standard, compiler.runnable)
				})
			}
		})
	}
	for path, before := range model.sources {
		after, readErr := os.ReadFile(path)
		require.NoError(t, readErr)
		assert.Equal(t, before, after, "the experiment must never change the bound user's source")
	}
}

// ceRebasePoCExecute validates complete wire-derived IDs and values and the
// fixture's own once-only counters. Cross-compiled objects never enter here.
func ceRebasePoCExecute(t *testing.T, project, compiler, object string, library []string, want string) {
	t.Helper()
	executable := filepath.Join(project, "ce-poc")
	if runtime.GOOS == "windows" {
		executable += ".exe"
	}
	args := append([]string{object}, library...)
	args = append(args, "-o", executable)
	output, err := ceRebasePoCRun(project, compiler, args)
	require.NoError(t, err, "%s", output)
	output, err = exec.Command(executable).CombinedOutput()
	require.NoError(t, err, "runtime payload or once-only assertion failed: %s", output)
	assert.Equal(t, want, strings.ReplaceAll(string(output), "\r\n", "\n"))
}

// ceRebasePoCVariants distinguishes missing-counter capability from language
// support: active rebase must fail, a single-site wrapper and disabled logging
// must work. Missing CE symbols must remain genuine compiler errors.
func ceRebasePoCVariants(t *testing.T, project, source, route, compiler string, base, library []string, model ceRebasePoCProject, runnable bool) {
	t.Helper()
	object := filepath.Join(project, "variant.o")
	for _, variant := range []struct {
		name, define string
		want         string
	}{
		{"single wrapper without counter", "CE_POC_SIMPLE_ONLY=1", ceRebasePoCExpected(t, model, true)},
		{"off without counter", "TRICE_OFF=1", ""},
		{"clean without counter", "TRICE_CLEAN=1", ""},
	} {
		t.Run(variant.name, func(t *testing.T) {
			// GCC reports the deliberate removal of a builtin as an unclassified
			// warning. Do not let that warning replace the capability test.
			args := append(append([]string{}, base...), "-Wno-error", "-U__COUNTER__", "-D"+variant.define, "-include", route, "-c", source, "-o", object)
			output, err := ceRebasePoCRun(project, compiler, args)
			require.NoError(t, err, "%s", output)
			if runnable {
				ceRebasePoCExecute(t, project, compiler, object, library, variant.want)
			}
		})
	}
	args := append(append([]string{}, base...), "-Wno-error", "-U__COUNTER__", "-include", route, "-fsyntax-only", source)
	output, err := ceRebasePoCRun(project, compiler, args)
	require.Error(t, err)
	assert.Contains(t, string(output), "requires __COUNTER__")
	assert.Contains(t, string(output), bindLimitsHint)

	valid, err := os.ReadFile(route)
	require.NoError(t, err)
	invalid := bytes.ReplaceAll(valid, []byte("(onlyLeft)"), []byte("(cePocMissingLocal)"))
	require.NotEqual(t, valid, invalid)
	require.NoError(t, os.WriteFile(route, invalid, 0o644))
	args = append(append([]string{}, base...), "-DCE_POC_PAD=7", "-include", route, "-fsyntax-only", source)
	output, err = ceRebasePoCRun(project, compiler, args)
	require.Error(t, err)
	assert.Contains(t, string(output), "cePocMissingLocal")
	if strings.Contains(filepath.Base(compiler), "clang") && (slices.Contains(base, "-std=c11") || slices.Contains(base, "-std=c++17")) {
		ceRebasePoCEditor(t, project, source, compiler, args, "cePocMissingLocal")
	}
	// A CE expression which itself consumes __COUNTER__ invalidates the probe's
	// one-counter-per-record assumption. It must fail, never mislabel payloads.
	extraCounter := bytes.ReplaceAll(valid, []byte("(onlyLeft)"), []byte("(onlyLeft + 0 * __COUNTER__)"))
	require.NoError(t, os.WriteFile(route, extraCounter, 0o644))
	output, err = ceRebasePoCRun(project, compiler, args)
	require.Error(t, err, "extra counter use in CE needs a different architecture contract: %s", output)
	require.NoError(t, os.WriteFile(route, valid, 0o644))
}

// ceRebasePoCEditor uses the actual forced include and compiler flags so editor
// acceptance cannot be attributed to ignoring the experimental macro route.
func ceRebasePoCEditor(t *testing.T, project, source, compiler string, args []string, missing string) {
	t.Helper()
	clangd, err := exec.LookPath("clangd")
	if err != nil {
		t.Log("clangd unavailable: editor evidence not collected")
		return
	}
	commands := []map[string]any{{"directory": project, "file": source, "arguments": append([]string{compiler}, args...)}}
	content, err := json.MarshalIndent(commands, "", "  ")
	require.NoError(t, err)
	writeBindIntegrationFile(t, project, "compile_commands.json", string(content))
	output, err := exec.Command(clangd, "--check="+source, "--compile-commands-dir="+project, "--log=info").CombinedOutput()
	assert.Contains(t, string(output), "Loaded compilation database")
	if missing != "" {
		require.Error(t, err)
		assert.Contains(t, string(output), missing)
		return
	}
	require.NoError(t, err, "%s", output)
	assert.Contains(t, string(output), "0 errors")
}
