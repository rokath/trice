// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"fmt"
	"io"
	"path/filepath"
	"strings"
	"testing"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// bindRenameCountingFs observes atomic replacements without changing filesystem behavior.
type bindRenameCountingFs struct {
	afero.Fs
	renames int
}

// Rename records one attempted destination replacement and delegates it unchanged.
func (f *bindRenameCountingFs) Rename(oldName, newName string) error {
	f.renames++
	return f.Fs.Rename(oldName, newName)
}

// bindFailOnceRenameFs injects one commit-time failure for a specific destination.
type bindFailOnceRenameFs struct {
	afero.Fs
	destination string
	failed      bool
}

// Rename fails the first replacement of destination and delegates every other rename.
func (f *bindFailOnceRenameFs) Rename(oldName, newName string) error {
	if newName == f.destination && !f.failed {
		f.failed = true
		return fmt.Errorf("injected rename failure for %s", newName)
	}
	return f.Fs.Rename(oldName, newName)
}

// prepareBindTest configures one isolated memory-filesystem project with deterministic IDs and keys.
func prepareBindTest(t *testing.T, sources map[string]string) func() {
	t.Helper()
	teardown := Setup(t)
	BindDir = filepath.Join(Proj, t.Name(), "build", "triceIDs")
	Min = 100
	Max = 199
	SearchMethod = "upward"
	DefaultStampSize = 16
	Srcs = nil
	ExcludeSrcs = nil
	TriceAliases = nil
	TriceSAliases = nil
	ProcessAliases()
	require.NoError(t, FSys.MkdirAll(filepath.Join(Proj, t.Name()), 0o755))
	paths := make([]string, 0, len(sources))
	for name, source := range sources {
		path := filepath.Join(Proj, t.Name(), name)
		require.NoError(t, FSys.MkdirAll(filepath.Dir(path), 0o755))
		require.NoError(t, FSys.WriteFile(path, []byte(source), 0o644))
		paths = append(paths, path)
	}
	Srcs = paths
	oldReader := bindRandomReader
	bindRandomReader = bytes.NewReader(bytes.Repeat([]byte{0x11}, 8*len(sources)+8))
	return func() {
		bindRandomReader = oldReader
		require.NoError(t, FSys.RemoveAll(filepath.Join(Proj, t.Name())))
		teardown()
	}
}

// TestBindGeneratesStableSidecarAndKeepsCallsIDFree covers the ordinary repeated bind workflow.
func TestBindGeneratesStableSidecarAndKeepsCallsIDFree(t *testing.T) {
	source := "#include \"trice.h\"\n\nstatic inline void log_value(int value) {\n    trice(\"msg:value=%d\\n\", value);\n    TRICE8_1(id(0), \"msg:byte=%d\\n\", value);\n}\n"
	defer prepareBindTest(t, map[string]string{"module.c": source})()

	path := Srcs[0]
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	bound, err := FSys.ReadFile(path)
	require.NoError(t, err)
	assert.Contains(t, string(bound), "#include \"trice_module_c_K1111111111111111.h\" // trice-bind:")
	assert.Contains(t, string(bound), `trice("msg:value=%d\n", value);`)
	assert.Contains(t, string(bound), `TRICE8_1(id(0), "msg:byte=%d\n", value);`)
	assert.NotContains(t, string(bound), "iD(100)")

	sidecarPath := filepath.Join(BindDir, "trice_module_c_K1111111111111111.h")
	sidecar, err := FSys.ReadFile(sidecarPath)
	require.NoError(t, err)
	assert.Contains(t, string(sidecar), "#define TRICE_BIND_SITE_K1111111111111111_L5 TRICE_BIND_AUTO,    iD(100u)")
	assert.Contains(t, string(sidecar), "#define TRICE_BIND_SITE_K1111111111111111_L6 TRICE_BIND_REPLACE, id(101u)")

	firstSource := append([]byte(nil), bound...)
	firstSidecar := append([]byte(nil), sidecar...)
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	bound, err = FSys.ReadFile(path)
	require.NoError(t, err)
	sidecar, err = FSys.ReadFile(sidecarPath)
	require.NoError(t, err)
	assert.Equal(t, firstSource, bound)
	assert.Equal(t, firstSidecar, sidecar)
}

// TestBindPreservesUppercaseStampSemantics checks default and explicit stamp wrappers independently.
func TestBindPreservesUppercaseStampSemantics(t *testing.T) {
	source := "TRICE(\"zero\");\nTRICE(id(0), \"id\");\nTRICE(Id(0), \"Id\");\nTRICE(ID(0), \"ID\");\ntrice(Id(0), \"lower Id\");\ntrice(ID(0), \"lower ID\");\n"
	defer prepareBindTest(t, map[string]string{"stamp.c": source})()
	DefaultStampSize = 32

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	sidecar, err := FSys.ReadFile(filepath.Join(BindDir, "trice_stamp_c_K1111111111111111.h"))
	require.NoError(t, err)
	assert.Contains(t, string(sidecar), "TRICE_BIND_AUTO,    ID(100u)")
	assert.Contains(t, string(sidecar), "TRICE_BIND_REPLACE, id(101u)")
	assert.Contains(t, string(sidecar), "TRICE_BIND_REPLACE, Id(102u)")
	assert.Contains(t, string(sidecar), "TRICE_BIND_REPLACE, ID(103u)")
	assert.Contains(t, string(sidecar), "TRICE_BIND_REPLACE, Id(104u)")
	assert.Contains(t, string(sidecar), "TRICE_BIND_REPLACE, ID(105u)")
}

// TestBindDefaultWrapperSelection covers every configured uppercase default and the lowercase invariant.
func TestBindDefaultWrapperSelection(t *testing.T) {
	defer Setup(t)()
	tests := []struct {
		stamp int
		macro string
		want  string
	}{
		{stamp: 0, macro: "TRICE", want: "id"},
		{stamp: 16, macro: "TRICE", want: "Id"},
		{stamp: 32, macro: "TRICE", want: "ID"},
		{stamp: 0, macro: "trice", want: "iD"},
		{stamp: 32, macro: "TRice", want: "iD"},
	}
	for _, test := range tests {
		DefaultStampSize = test.stamp
		assert.Equal(t, test.want, bindDefaultWrapper(test.macro))
	}
}

// TestBindRecognizesWhitespaceIncludeAndReactivation accepts comment-free repeated owner includes.
func TestBindRecognizesWhitespaceIncludeAndReactivation(t *testing.T) {
	key := "K0123456789ABCDEF"
	name := "trice_renamed_c_" + key + ".h"
	source := fmt.Sprintf("# include \"%s\"\ntrice(\"first\");\n#include \"late.h\"\n# include \"%s\" // no identity in this comment\ntrice(\"second\");\n", name, name)
	defer prepareBindTest(t, map[string]string{"renamed-again.c": source})()
	require.NoError(t, FSys.MkdirAll(BindDir, 0o755))
	require.NoError(t, FSys.WriteFile(filepath.Join(BindDir, name), []byte("#define TRICE_BIND_FILE_KEY "+key+"\n#define TRICE_BIND_ROUTE_"+key+" BIND\n"), 0o644))

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, source, string(bound))
	sidecar, err := FSys.ReadFile(filepath.Join(BindDir, name))
	require.NoError(t, err)
	assert.Contains(t, string(sidecar), "_L2")
	assert.Contains(t, string(sidecar), "_L5")
}

// TestBindIgnoresCommentedIncludes keeps non-directive text out of placement and identity analysis.
func TestBindIgnoresCommentedIncludes(t *testing.T) {
	source := "/*\n#include \"trice_foreign_c_K0123456789ABCDEF.h\"\n*/\ntrice(\"bound\");\n"
	defer prepareBindTest(t, map[string]string{"commented.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	after, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Contains(t, string(after), "#include \"trice_commented_c_K1111111111111111.h\"")
	assert.Contains(t, string(after), "#include \"trice_foreign_c_K0123456789ABCDEF.h\"")
}

// TestBindIgnoresCommentedCalls keeps parser-like text in comments out of bind diagnostics and sidecars.
func TestBindIgnoresCommentedCalls(t *testing.T) {
	source := "// trice(\"line comment\");\n/* TRICE(ID(0), \"block comment\"); */\ntrice(\"active\");\n"
	defer prepareBindTest(t, map[string]string{"commented-calls.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	sidecar, err := FSys.ReadFile(filepath.Join(BindDir, "trice_commented_calls_c_K1111111111111111.h"))
	require.NoError(t, err)
	assert.Equal(t, 1, strings.Count(string(sidecar), "#define TRICE_BIND_SITE_"))
	assert.Contains(t, string(sidecar), "active")
	assert.NotContains(t, string(sidecar), "line comment")
	assert.NotContains(t, string(sidecar), "block comment")
	assert.Len(t, IDData.idToTrice, 1)
	assert.Len(t, IDData.idToLocNew, 1)
}

// TestBindRejectsUnsafePlacementWithoutWriting protects all regular outputs after analysis failure.
func TestBindRejectsUnsafePlacementWithoutWriting(t *testing.T) {
	source := "void f(void) { trice(\"first\"); }\n#include \"later.h\"\n"
	defer prepareBindTest(t, map[string]string{"unsafe.c": source})()
	tilBefore, err := FSys.ReadFile(FnJSON)
	require.NoError(t, err)

	var output bytes.Buffer
	err = SubCmdIdBind(&output, FSys)
	require.Error(t, err)
	assert.Contains(t, output.String(), "cannot place bind include safely")
	after, readErr := FSys.ReadFile(Srcs[0])
	require.NoError(t, readErr)
	assert.Equal(t, source, string(after))
	_, statErr := FSys.Stat(BindDir)
	assert.Error(t, statErr)
	tilAfter, readErr := FSys.ReadFile(FnJSON)
	require.NoError(t, readErr)
	assert.Equal(t, tilBefore, tilAfter)
}

// TestBindRejectsMixedSites verifies deterministic ownership diagnostics.
func TestBindRejectsMixedSites(t *testing.T) {
	sources := map[string]string{
		"z.c": "trice(iD(124), \"insert\");\ntrice(\"bind\");\n",
		"a.c": "trice(iD(123), \"insert\");\ntrice(\"bind\");\n",
	}
	defer prepareBindTest(t, sources)()

	var output bytes.Buffer
	err := SubCmdIdBind(&output, FSys)
	require.Error(t, err)
	text := output.String()
	assert.Contains(t, text, "file mixes explicit non-zero IDs")
	assert.Less(t, strings.Index(text, "a.c"), strings.Index(text, "z.c"))
	for _, path := range Srcs {
		content, readErr := FSys.ReadFile(path)
		require.NoError(t, readErr)
		assert.Equal(t, sources[filepath.Base(path)], string(content))
	}
}

// TestBindMarkersAliasesAndMacroDefinitionSupport combines parser sharing with an unused wrapper definition.
func TestBindMarkersAliasesAndMacroDefinitionSupport(t *testing.T) {
	source := "// TRICE_INSERT_OFF\ntrice(iD(777), \"ignored\");\n// TRICE_INSERT_ON\nLOG(\"alias=%d\", 1);\n#define BAD() trice(\"bad\")\n"
	defer prepareBindTest(t, map[string]string{"alias.c": source})()
	TriceAliases = ArrayFlag{"LOG"}
	ProcessAliases()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	after, readErr := FSys.ReadFile(Srcs[0])
	require.NoError(t, readErr)
	assert.Contains(t, string(after), "trice-bind: keep as last include")
	sidecar, readErr := FSys.ReadFile(filepath.Join(BindDir, "trice_alias_c_K1111111111111111.h"))
	require.NoError(t, readErr)
	assert.Contains(t, string(sidecar), "TRICE_BIND_DEFINITION_K1111111111111111")
	assert.NotContains(t, string(sidecar), "777u")
}

// TestBindMultipleDirectSitesGenerateLocalRebase covers two and three direct sites on one line.
func TestBindMultipleDirectSitesGenerateLocalRebase(t *testing.T) {
	source := "#include \"trice.h\"\nvoid f(void) { trice(\"one\"); trice(\"two\"); trice(\"three\"); }\n"
	defer prepareBindTest(t, map[string]string{"multi.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Contains(t, string(bound), "trice-bind: generated rebase begin K1111111111111111_R0")
	assert.Contains(t, string(bound), "trice-bind: generated rebase end K1111111111111111_R0")
	assert.Equal(t, 1, strings.Count(string(bound), bindRebaseIncludeMarker+"begin"))
	assert.NotContains(t, string(bound), "#define TRICE_BIND_REBASE_BEGIN")
	assert.NotContains(t, string(bound), "iD(100")
	assert.Equal(t, strings.Count(source, "\n")+3, strings.Count(string(bound), "\n"), "one owner include and two rebase boundary includes are added")

	sidecarPath := filepath.Join(BindDir, "trice_multi_c_K1111111111111111.h")
	sidecar, err := FSys.ReadFile(sidecarPath)
	require.NoError(t, err)
	assert.Contains(t, string(sidecar), "#define TRICE_BIND_REBASE_COUNT_K1111111111111111_R0 3")
	assert.Contains(t, string(sidecar), "constructor(TRICE_BIND_ID_LOCATION_K1111111111111111_L4_O0)")
	assert.Contains(t, string(sidecar), "constructor(TRICE_BIND_ID_LOCATION_K1111111111111111_L4_O1)")
	assert.Contains(t, string(sidecar), "constructor(TRICE_BIND_ID_LOCATION_K1111111111111111_L4_O2)")
	assert.Equal(t, 3, strings.Count(string(sidecar), "TRICE_BIND_LOCATION_"))
	beginHelper, err := FSys.ReadFile(filepath.Join(BindDir, "trice_multi_c_K1111111111111111_R0_begin.h"))
	require.NoError(t, err)
	assert.Contains(t, string(beginHelper), "#define TRICE_BIND_REBASE_SCOPE K1111111111111111_R0")
	assert.Contains(t, string(beginHelper), "#include \"trice_multi_c_K1111111111111111.h\"")
	endHelper, err := FSys.ReadFile(filepath.Join(BindDir, "trice_multi_c_K1111111111111111_R0_end.h"))
	require.NoError(t, err)
	assert.Contains(t, string(endHelper), "#define TRICE_BIND_REBASE_END")

	firstSource := append([]byte(nil), bound...)
	firstSidecar := append([]byte(nil), sidecar...)
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	bound, err = FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	sidecar, err = FSys.ReadFile(sidecarPath)
	require.NoError(t, err)
	assert.Equal(t, firstSource, bound)
	assert.Equal(t, firstSidecar, sidecar)
}

// TestBindWrapperDefinitionsReuseIDsAtAllInvocations covers normal and same-line calls.
func TestBindWrapperDefinitionsReuseIDsAtAllInvocations(t *testing.T) {
	source := "#define LOG_ERROR(value) do { trice(\"first\"); trice(\"second=%d\", value); } while (0)\n" +
		"void a(void) { LOG_ERROR(1); }\n" +
		"void b(void) { LOG_ERROR(2); LOG_ERROR(3); }\n"
	defer prepareBindTest(t, map[string]string{"wrapper.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	sidecar, err := FSys.ReadFile(filepath.Join(BindDir, "trice_wrapper_c_K1111111111111111.h"))
	require.NoError(t, err)
	text := string(sidecar)
	assert.Equal(t, 2, strings.Count(text, "#define TRICE_BIND_DEFINITION_"))
	assert.Contains(t, text, "TRICE_BIND_ID_DEFINITION_K1111111111111111_L")
	assert.Contains(t, text, " 100u // numeric definition descriptor")
	assert.Contains(t, text, " 101u // numeric definition descriptor")
	assert.Contains(t, text, "TRICE_BIND_REBASE_COUNT_K1111111111111111_R0 2")
	assert.Contains(t, text, "TRICE_BIND_REBASE_COUNT_K1111111111111111_R1 4")
	assert.Len(t, IDData.idToTrice, 2)
	assert.Len(t, IDData.idToLocNew, 2)
	for _, location := range IDData.idToLocNew {
		assert.Equal(t, ToLIFile(Srcs[0]), normalizeLocationPath(location.File))
	}
}

// TestBindSingleSiteWrapperUsesNormalLineDescriptors avoids an unnecessary counter dependency.
func TestBindSingleSiteWrapperUsesNormalLineDescriptors(t *testing.T) {
	source := "#define LOG_VALUE(value) trice(\"value=%d\", value)\n" +
		"void f(void) {\n    LOG_VALUE(1);\n    LOG_VALUE(2);\n}\n"
	defer prepareBindTest(t, map[string]string{"single-wrapper.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.NotContains(t, string(bound), "TRICE_BIND_REBASE_BEGIN")
	sidecar, err := FSys.ReadFile(filepath.Join(BindDir, "trice_single_wrapper_c_K1111111111111111.h"))
	require.NoError(t, err)
	text := string(sidecar)
	assert.NotContains(t, text, "__COUNTER__")
	assert.NotContains(t, text, "TRICE_BIND_REBASE_")
	assert.Equal(t, 1, strings.Count(text, "#define TRICE_BIND_DEFINITION_"))
	assert.Equal(t, 2, strings.Count(text, "#define TRICE_BIND_SITE_"))
	assert.Equal(t, 1, strings.Count(text, " 100u // numeric definition descriptor"), "the wrapper definition has one numeric source of truth")
	assert.Contains(t, text, "iD(TRICE_BIND_ID_DEFINITION_")
}

// TestBindDetectsDuplicateFileKey rejects copied owner identity across physical files.
func TestBindDetectsDuplicateFileKey(t *testing.T) {
	key := "K0123456789ABCDEF"
	name := "trice_owner_c_" + key + ".h"
	source := fmt.Sprintf("#include \"%s\"\ntrice(\"bound\");\n", name)
	defer prepareBindTest(t, map[string]string{"a.c": source, "b.c": source})()
	require.NoError(t, FSys.MkdirAll(BindDir, 0o755))
	require.NoError(t, FSys.WriteFile(filepath.Join(BindDir, name), []byte("#define TRICE_BIND_FILE_KEY "+key+"\n#define TRICE_BIND_ROUTE_"+key+" BIND\n"), 0o644))

	var output bytes.Buffer
	err := SubCmdIdBind(&output, FSys)
	require.Error(t, err)
	assert.Contains(t, output.String(), "belongs to multiple owner files")
}

// TestBindRejectsConflictingOwnerIncludes detects two distinct identities in one physical file.
func TestBindRejectsConflictingOwnerIncludes(t *testing.T) {
	first := "trice_owner_c_K0123456789ABCDEF.h"
	second := "trice_owner_c_K1111111111111111.h"
	source := fmt.Sprintf("#include \"%s\"\n#include \"%s\"\ntrice(\"bound\");\n", first, second)
	defer prepareBindTest(t, map[string]string{"owner.c": source})()

	var output bytes.Buffer
	err := SubCmdIdBind(&output, FSys)
	require.Error(t, err)
	assert.Contains(t, output.String(), "includes conflicting Trice bind sidecars")
	after, readErr := FSys.ReadFile(Srcs[0])
	require.NoError(t, readErr)
	assert.Equal(t, source, string(after))
}

// TestBindDryRunCalculatesButWritesNothing checks source, list, directory, and sidecar suppression.
func TestBindDryRunCalculatesButWritesNothing(t *testing.T) {
	source := "trice(\"dry\");\n"
	defer prepareBindTest(t, map[string]string{"dry.c": source})()
	DryRun = true
	Verbose = true

	var output bytes.Buffer
	require.NoError(t, SubCmdIdBind(&output, FSys))
	assert.Contains(t, output.String(), "dry-run:")
	after, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, source, string(after))
	_, err = FSys.Stat(BindDir)
	assert.Error(t, err)
}

// TestBindAssignmentsAreDeterministicAcrossParallelReadOrder fixes assignment order to sorted paths.
func TestBindAssignmentsAreDeterministicAcrossParallelReadOrder(t *testing.T) {
	sources := map[string]string{
		"z.c": "trice(\"z\");\n",
		"a.c": "trice(\"a\");\n",
	}
	defer prepareBindTest(t, sources)()
	bindRandomReader = bytes.NewReader(append(bytes.Repeat([]byte{0xAA}, 8), bytes.Repeat([]byte{0xBB}, 8)...))

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	aSidecar, err := FSys.ReadFile(filepath.Join(BindDir, "trice_a_c_KAAAAAAAAAAAAAAAA.h"))
	require.NoError(t, err)
	zSidecar, err := FSys.ReadFile(filepath.Join(BindDir, "trice_z_c_KBBBBBBBBBBBBBBBB.h"))
	require.NoError(t, err)
	assert.Contains(t, string(aSidecar), "iD(100u)")
	assert.Contains(t, string(zSidecar), "iD(101u)")
}

// TestBindUpdatesTriceCleanOnlyAfterSuccessfulAnalysis re-enables an existing clean configuration.
func TestBindUpdatesTriceCleanOnlyAfterSuccessfulAnalysis(t *testing.T) {
	source := "#define TRICE_CLEAN 1\n"
	defer prepareBindTest(t, map[string]string{"triceConfig.h": source})()
	tilBefore, err := FSys.ReadFile(FnJSON)
	require.NoError(t, err)
	liBefore, err := FSys.ReadFile(LIFnJSON)
	require.NoError(t, err)

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	after, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, "#define TRICE_CLEAN 0\n", string(after))
	tilAfter, err := FSys.ReadFile(FnJSON)
	require.NoError(t, err)
	liAfter, err := FSys.ReadFile(LIFnJSON)
	require.NoError(t, err)
	assert.Equal(t, tilBefore, tilAfter)
	assert.Equal(t, liBefore, liAfter)
}

// TestBindFileKeyAndSidecarName validates the stable key and readable basename formats directly.
func TestBindFileKeyAndSidecarName(t *testing.T) {
	defer Setup(t)()
	oldReader := bindRandomReader
	bindRandomReader = bytes.NewReader([]byte{0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF})
	defer func() { bindRandomReader = oldReader }()

	key, err := newBindFileKey(map[string][]string{})
	require.NoError(t, err)
	assert.Equal(t, "K0123456789ABCDEF", key)
	assert.Equal(t, "trice_module_name_c___K0123456789ABCDEF.h", bindSidecarFilename("dir/module.name-c++", key))
}

// TestBindClassifiesPhysicalFiles exercises none, insert-owned, bind-owned, and mixed inputs.
func TestBindClassifiesPhysicalFiles(t *testing.T) {
	defer Setup(t)()
	ProcessAliases()
	tests := []struct {
		name   string
		source string
		class  bindFileClass
	}{
		{name: "none", source: "int value;\n", class: bindFileNone},
		{name: "insert", source: "trice(iD(123), \"insert\");\n", class: bindFileInsert},
		{name: "bound", source: "trice(\"bound\");\n", class: bindFileBound},
		{name: "mixed", source: "trice(iD(123), \"insert\");\ntrice(\"bound\");\n", class: bindFileMixed},
	}
	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			plan := analyzeBindFile(bindFileInput{path: test.name + ".c", data: []byte(test.source)})
			assert.Equal(t, test.class, plan.class)
		})
	}
}

// TestBindKeepsRepeatedFormatsDistinctAndStable verifies location-based reuse for identical log text.
func TestBindKeepsRepeatedFormatsDistinctAndStable(t *testing.T) {
	source := "trice(\"msg:same\");\ntrice(\"msg:same\");\n"
	defer prepareBindTest(t, map[string]string{"same.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	sidecarPath := filepath.Join(BindDir, "trice_same_c_K1111111111111111.h")
	first, err := FSys.ReadFile(sidecarPath)
	require.NoError(t, err)
	assert.Contains(t, string(first), "iD(100u)")
	assert.Contains(t, string(first), "iD(101u)")
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	second, err := FSys.ReadFile(sidecarPath)
	require.NoError(t, err)
	assert.Equal(t, first, second)
}

// TestBindDoesNotReplaceUnchangedFiles observes that a second successful run performs no rename.
func TestBindDoesNotReplaceUnchangedFiles(t *testing.T) {
	source := "trice(\"msg:stable\");\n"
	defer prepareBindTest(t, map[string]string{"stable.c": source})()
	counting := &bindRenameCountingFs{Fs: FSys.Fs}
	countedFileSystem := &afero.Afero{Fs: counting}

	require.NoError(t, SubCmdIdBind(io.Discard, countedFileSystem))
	assert.Positive(t, counting.renames)
	counting.renames = 0
	require.NoError(t, SubCmdIdBind(io.Discard, countedFileSystem))
	assert.Zero(t, counting.renames)
}

// TestBindRollsBackCommitFailure prevents a late metadata error from leaving
// earlier sidecar or rebase-helper writes behind.
func TestBindRollsBackCommitFailure(t *testing.T) {
	source := "trice(\"msg:rollback-a\"); trice(\"msg:rollback-b\");\n"
	defer prepareBindTest(t, map[string]string{"rollback.c": source})()
	tilBefore, err := FSys.ReadFile(FnJSON)
	require.NoError(t, err)
	failing := &bindFailOnceRenameFs{Fs: FSys.Fs, destination: FnJSON}
	failingFileSystem := &afero.Afero{Fs: failing}

	err = SubCmdIdBind(io.Discard, failingFileSystem)
	require.Error(t, err)
	after, readErr := FSys.ReadFile(Srcs[0])
	require.NoError(t, readErr)
	assert.Equal(t, source, string(after))
	tilAfter, readErr := FSys.ReadFile(FnJSON)
	require.NoError(t, readErr)
	assert.Equal(t, tilBefore, tilAfter)
	entries, readErr := FSys.ReadDir(BindDir)
	require.NoError(t, readErr)
	assert.Empty(t, entries)
}

// TestBindRejectsCorruptSidecarRoute checks every key and route definition before replacing artifacts.
func TestBindRejectsCorruptSidecarRoute(t *testing.T) {
	key := "K0123456789ABCDEF"
	name := "trice_owner_c_" + key + ".h"
	source := fmt.Sprintf("#include \"%s\"\ntrice(\"bound\");\n", name)
	defer prepareBindTest(t, map[string]string{"owner.c": source})()
	require.NoError(t, FSys.MkdirAll(BindDir, 0o755))
	corrupt := "#define TRICE_BIND_FILE_KEY " + key + "\n#define TRICE_BIND_FILE_KEY K2222222222222222\n#define TRICE_BIND_ROUTE_" + key + " BIND\n#define TRICE_BIND_ROUTE_K1111111111111111 BIND\n"
	require.NoError(t, FSys.WriteFile(filepath.Join(BindDir, name), []byte(corrupt), 0o644))

	var output bytes.Buffer
	err := SubCmdIdBind(&output, FSys)
	require.Error(t, err)
	assert.Contains(t, output.String(), "declares key K2222222222222222")
	assert.Contains(t, output.String(), "declares route for key K1111111111111111")
	after, readErr := FSys.ReadFile(filepath.Join(BindDir, name))
	require.NoError(t, readErr)
	assert.Equal(t, corrupt, string(after))
}

// TestBindPreservesEmptyOwnerIdentity removes stale sites without dropping the include or key.
func TestBindPreservesEmptyOwnerIdentity(t *testing.T) {
	key := "K0123456789ABCDEF"
	name := "trice_empty_c_" + key + ".h"
	source := "#include \"" + name + "\"\nint value;\n"
	defer prepareBindTest(t, map[string]string{"empty.c": source})()
	require.NoError(t, FSys.MkdirAll(BindDir, 0o755))
	stale := "#define TRICE_BIND_FILE_KEY " + key + "\n#define TRICE_BIND_ROUTE_" + key + " BIND\n#define TRICE_BIND_SITE_" + key + "_L2 TRICE_BIND_AUTO, iD(123u)\n"
	require.NoError(t, FSys.WriteFile(filepath.Join(BindDir, name), []byte(stale), 0o644))

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	after, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, source, string(after))
	sidecar, err := FSys.ReadFile(filepath.Join(BindDir, name))
	require.NoError(t, err)
	assert.Contains(t, string(sidecar), "#define TRICE_BIND_FILE_KEY "+key)
	assert.NotContains(t, string(sidecar), "#define TRICE_BIND_SITE_")
}

// TestBindAutomaticallyExcludesBindDirectory ensures hostile generated-tree input is never classified.
func TestBindAutomaticallyExcludesBindDirectory(t *testing.T) {
	sources := map[string]string{
		"module.c":                 "trice(\"managed\");\n",
		"build/triceIDs/hostile.c": "trice(iD(123), \"insert\");\ntrice(\"mixed\");\n",
	}
	defer prepareBindTest(t, sources)()
	project := filepath.Join(Proj, t.Name())
	Srcs = ArrayFlag{project}

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	hostile, err := FSys.ReadFile(filepath.Join(BindDir, "hostile.c"))
	require.NoError(t, err)
	assert.Equal(t, sources["build/triceIDs/hostile.c"], string(hostile))
}

// TestInsertSkipsRecognizedBindOwner prevents accidental source instrumentation by the legacy command.
func TestInsertSkipsRecognizedBindOwner(t *testing.T) {
	key := "K0123456789ABCDEF"
	source := "#include \"trice_owner_c_" + key + ".h\"\ntrice(\"bound\");\n"
	defer prepareBindTest(t, map[string]string{"owner.c": source})()

	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	after, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, source, string(after))
}

// TestBindInvalidOptionsAndOtherErrorsLeaveConfigUntouched checks both early and aggregated failures.
func TestBindInvalidOptionsAndOtherErrorsLeaveConfigUntouched(t *testing.T) {
	t.Run("invalid stamp", func(t *testing.T) {
		source := "trice(\"bound\");\n"
		defer prepareBindTest(t, map[string]string{"invalid.c": source})()
		DefaultStampSize = 8
		require.Error(t, SubCmdIdBind(io.Discard, FSys))
		after, err := FSys.ReadFile(Srcs[0])
		require.NoError(t, err)
		assert.Equal(t, source, string(after))
	})

	t.Run("aggregated placement error", func(t *testing.T) {
		sources := map[string]string{
			"triceConfig.h": "#define TRICE_CLEAN 1\n",
			"unsafe.c":      "trice(\"first\");\n#include \"later.h\"\n",
		}
		defer prepareBindTest(t, sources)()
		require.Error(t, SubCmdIdBind(io.Discard, FSys))
		config, err := FSys.ReadFile(filepath.Join(Proj, t.Name(), "triceConfig.h"))
		require.NoError(t, err)
		assert.Equal(t, sources["triceConfig.h"], string(config))
	})
}

// TestBindMultilineCommentStaysOnOnePhysicalLine guards generated preprocessing boundaries.
func TestBindMultilineCommentStaysOnOnePhysicalLine(t *testing.T) {
	source := "trice( \\\n    \"msg:multiline\"\n);\n"
	defer prepareBindTest(t, map[string]string{"multiline.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	sidecar, err := FSys.ReadFile(filepath.Join(BindDir, "trice_multiline_c_K1111111111111111.h"))
	require.NoError(t, err)
	assert.Contains(t, string(sidecar), "// trice( \\ ...")
	for _, line := range strings.Split(string(sidecar), "\n") {
		assert.False(t, strings.HasSuffix(line, "\\"))
	}
}

// TestBindRejectsUnresolvedRecognizedSites reports shared-parser failures instead of silently skipping them.
func TestBindRejectsUnresolvedRecognizedSites(t *testing.T) {
	sources := map[string]string{
		"a.c": "trice(value);\n",
		"b.c": "trice(\"unterminated\";\n",
	}
	defer prepareBindTest(t, sources)()

	var output bytes.Buffer
	err := SubCmdIdBind(&output, FSys)
	require.Error(t, err)
	assert.Contains(t, output.String(), "no supported format string")
	assert.Contains(t, output.String(), "no matching closing parenthesis")
	for _, path := range Srcs {
		after, readErr := FSys.ReadFile(path)
		require.NoError(t, readErr)
		assert.Equal(t, sources[filepath.Base(path)], string(after))
	}
}

// TestBindRejectsCompositeFormatString enforces the shared parser's non-leading literal boundary.
func TestBindRejectsCompositeFormatString(t *testing.T) {
	source := "#define PREFIX \"msg:\"\ntrice(PREFIX \"value=%d\\n\", 1);\n"
	defer prepareBindTest(t, map[string]string{"composite.c": source})()

	var output bytes.Buffer
	err := SubCmdIdBind(&output, FSys)
	require.Error(t, err)
	assert.Contains(t, output.String(), "unsupported composite or non-leading format string")
	after, readErr := FSys.ReadFile(Srcs[0])
	require.NoError(t, readErr)
	assert.Equal(t, source, string(after))
}
