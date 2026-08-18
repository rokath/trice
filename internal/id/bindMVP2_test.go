// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"io"
	"path/filepath"
	"sort"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// useDistinctBindTestKeys supplies one deterministic, collision-free key per
// sorted physical file in a multi-file memory project.
func useDistinctBindTestKeys(fileCount int) {
	data := make([]byte, 0, fileCount*8)
	for index := 0; index < fileCount; index++ {
		data = append(data, bytes.Repeat([]byte{byte(index + 1)}, 8)...)
	}
	bindRandomReader = bytes.NewReader(data)
}

// readOwnedBindSidecar resolves the generated identity from one bound source
// instead of coupling tests to the key-assignment order.
func readOwnedBindSidecar(t *testing.T, sourcePath string) (bindInclude, []byte) {
	t.Helper()
	source, err := FSys.ReadFile(sourcePath)
	require.NoError(t, err)
	for _, include := range scanBindIncludes(string(source)) {
		if include.isSidecar {
			content, readErr := FSys.ReadFile(filepath.Join(BindDir, include.name))
			require.NoError(t, readErr)
			return include, content
		}
	}
	t.Fatalf("%s has no generated bind sidecar include", sourcePath)
	return bindInclude{}, nil
}

// writeBindTestTIL writes one deterministic lookup and returns the exact bytes
// so tests can prove that read-only metadata remains untouched.
func writeBindTestTIL(t *testing.T, path string, lookup TriceIDLookUp) []byte {
	t.Helper()
	content, err := lookup.toJSON()
	require.NoError(t, err)
	require.NoError(t, FSys.WriteFile(path, content, 0o644))
	return content
}

// writeBindTestLI is the location-list counterpart of writeBindTestTIL.
func writeBindTestLI(t *testing.T, path string, lookup TriceIDLookUpLI) []byte {
	t.Helper()
	content, err := lookup.toJSON()
	require.NoError(t, err)
	require.NoError(t, FSys.WriteFile(path, content, 0o644))
	return content
}

// TestBindRebaseLineContextRejectsGeneratedIncludeAfterIf verifies that the
// safety check looks through the automatically inserted owner include and
// still sees the control-flow dependency in the original source.
func TestBindRebaseLineContextRejectsGeneratedIncludeAfterIf(t *testing.T) {
	source := "void f(int value) {\n    if (value)\n#include \"trice_invalid_c_K1111111111111111.h\" // trice-bind: keep as last include\n        trice(\"first\"); trice(\"second\");\n}\n"
	occurrence := strings.Index(source, "trice(\"first\")")
	require.NotEqual(t, -1, occurrence)
	start := lineStart(source, occurrence)
	end := lineEndIncludingNewline(source, occurrence)
	assert.False(t, bindRebaseLineIsIndependent(source, start, end, occurrence, nil))

	safe := "static inline void direct(void) { trice(\"first\"); trice(\"second\"); }\nstatic inline void wrapper(int value) { LOG_ERROR(value); }\n"
	occurrence = strings.Index(safe, "LOG_ERROR")
	require.NotEqual(t, -1, occurrence)
	start = lineStart(safe, occurrence)
	end = lineEndIncludingNewline(safe, occurrence)
	assert.True(t, bindRebaseLineIsIndependent(safe, start, end, occurrence, nil))

	safe = "#define LOG_ERROR(value) \\\n    do { trice(\"inside\"); } while (0)\nstatic inline void direct(void) { trice(\"first\"); trice(\"second\"); }\n"
	occurrence = strings.Index(safe, "trice(\"first\")")
	require.NotEqual(t, -1, occurrence)
	start = lineStart(safe, occurrence)
	end = lineEndIncludingNewline(safe, occurrence)
	assert.True(t, bindRebaseLineIsIndependent(safe, start, end, occurrence, nil))
}

// TestBindCounterIdentifierDetection prevents application identifiers that
// merely contain the built-in name from being diagnosed as counter use.
func TestBindCounterIdentifierDetection(t *testing.T) {
	assert.True(t, bindContainsIdentifier("enum { value = __COUNTER__ };", "__COUNTER__"))
	assert.False(t, bindContainsIdentifier("unsigned MY__COUNTER__VALUE;", "__COUNTER__"))
}

// TestBindMVP2HeaderWrapperIdentityAndIdempotence proves that two source
// owners reuse the two definition IDs from one header and that a second bind
// writes no semantic changes.
func TestBindMVP2HeaderWrapperIdentityAndIdempotence(t *testing.T) {
	sources := map[string]string{
		"logging.h": "#ifndef LOGGING_H\n#define LOGGING_H\n#define LOG_ERROR(value) do { trice(\"first\"); TRICE(\"second=%d\", value); } while (0)\n#endif\n",
		"a.c":       "#include \"logging.h\"\nvoid a(int value) { LOG_ERROR(value); }\n",
		"b.c":       "#include \"logging.h\"\nvoid b(void) { LOG_ERROR(1); LOG_ERROR(2); }\n",
	}
	defer prepareBindTest(t, sources)()
	useDistinctBindTestKeys(len(sources))

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	headerPath := filepath.Join(Proj, t.Name(), "logging.h")
	headerInclude, headerSidecar := readOwnedBindSidecar(t, headerPath)
	assert.Equal(t, 2, strings.Count(string(headerSidecar), "#define TRICE_BIND_DEFINITION_"))
	assert.Equal(t, 2, strings.Count(string(headerSidecar), "#define TRICE_BIND_ID_DEFINITION_"))
	assert.Len(t, IDData.idToTrice, 2)

	for _, name := range []string{"a.c", "b.c"} {
		path := filepath.Join(Proj, t.Name(), name)
		_, sidecar := readOwnedBindSidecar(t, path)
		assert.Contains(t, string(sidecar), "TRICE_BIND_ID_DEFINITION_"+headerInclude.key)
		assert.Contains(t, string(sidecar), "TRICE_BIND_REBASE_COUNT_")
	}

	locations, _ := readBindRemigrationLocations(t)
	require.Len(t, locations, 2)
	for _, location := range locations {
		assert.Equal(t, ToLIFile(headerPath), normalizeLocationPath(location.File))
	}

	paths := []string{FnJSON, LIFnJSON}
	paths = append(paths, Srcs...)
	entries, err := FSys.ReadDir(BindDir)
	require.NoError(t, err)
	for _, entry := range entries {
		paths = append(paths, filepath.Join(BindDir, entry.Name()))
	}
	snapshots := make(map[string][]byte, len(paths))
	for _, path := range paths {
		content, readErr := FSys.ReadFile(path)
		require.NoError(t, readErr)
		snapshots[path] = content
	}

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	for path, expected := range snapshots {
		actual, readErr := FSys.ReadFile(path)
		require.NoError(t, readErr)
		assert.Equal(t, expected, actual, path)
	}
}

// TestBindMVP2SourceMoveUpdatesDescriptorsWithoutChangingIDs checks that a
// user edit before a generated region relocates LI data while preserving TIL
// identity and exactly one regenerated marker pair.
func TestBindMVP2SourceMoveUpdatesDescriptorsWithoutChangingIDs(t *testing.T) {
	source := "#include \"trice.h\"\nvoid f(void) { trice(\"first\"); trice(\"second\"); }\n"
	defer prepareBindTest(t, map[string]string{"move.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	_, firstSidecar := readOwnedBindSidecar(t, Srcs[0])
	firstLines, err := parseBindRemigrationSiteLines(firstSidecar, "K1111111111111111")
	require.NoError(t, err)
	require.Len(t, firstLines, 2)
	firstTIL, err := FSys.ReadFile(FnJSON)
	require.NoError(t, err)
	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	beginMarker := bindRebaseIncludeMarker + "begin"
	markerPosition := strings.Index(string(bound), beginMarker)
	require.GreaterOrEqual(t, markerPosition, 0)
	markerLineStart := lineStart(string(bound), markerPosition)
	shifted := string(bound[:markerLineStart]) + "// user line shift\n" + string(bound[markerLineStart:])
	require.NoError(t, FSys.WriteFile(Srcs[0], []byte(shifted), 0o644))

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	_, secondSidecar := readOwnedBindSidecar(t, Srcs[0])
	secondLines, err := parseBindRemigrationSiteLines(secondSidecar, "K1111111111111111")
	require.NoError(t, err)
	assert.Equal(t, len(firstLines), len(secondLines))
	ids := make([]int, 0, len(firstLines))
	for id := range firstLines {
		ids = append(ids, int(id))
	}
	sort.Ints(ids)
	for _, value := range ids {
		id := TriceID(value)
		assert.Equal(t, firstLines[id]+1, secondLines[id])
	}
	secondTIL, err := FSys.ReadFile(FnJSON)
	require.NoError(t, err)
	assert.Equal(t, firstTIL, secondTIL)
	regenerated, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, 1, strings.Count(string(regenerated), bindRebaseIncludeMarker+"begin"))
	assert.Equal(t, 1, strings.Count(string(regenerated), bindRebaseIncludeMarker+"end"))
}

// TestBindMVP2KeepsAdjacentPhysicalLinesInSeparateScopes verifies that source
// compactness never widens a counter scope across independent statements.
func TestBindMVP2KeepsAdjacentPhysicalLinesInSeparateScopes(t *testing.T) {
	source := "void f(void) {\n" +
		"    trice(\"first\"); trice(\"second\");\n" +
		"    trice(\"third\"); trice(\"fourth\");\n" +
		"}\n"
	defer prepareBindTest(t, map[string]string{"adjacent.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	text := string(bound)
	assert.Equal(t, 2, strings.Count(text, bindRebaseIncludeMarker+"begin"))
	assert.Equal(t, 2, strings.Count(text, bindRebaseIncludeMarker+"end"))
	firstEnd := strings.Index(text, "generated rebase end K1111111111111111_R0")
	secondBegin := strings.Index(text, "generated rebase begin K1111111111111111_R1")
	require.GreaterOrEqual(t, firstEnd, 0)
	require.GreaterOrEqual(t, secondBegin, 0)
	assert.Less(t, firstEnd, secondBegin)

	entries, err := FSys.ReadDir(BindDir)
	require.NoError(t, err)
	assert.Len(t, entries, 5, "one owner sidecar plus two explicit helpers per independent source line")
}

// TestBindMVP2WrapsOneMultilineInvocationAsItsMinimalRegion proves that a
// physical line restriction does not force users to reformat one macro call.
func TestBindMVP2WrapsOneMultilineInvocationAsItsMinimalRegion(t *testing.T) {
	source := "#define LOG_ERROR(value) do { trice(\"first\"); trice(\"second=%d\", value); } while (0)\n" +
		"void f(int value) {\n" +
		"    LOG_ERROR(\n" +
		"        value\n" +
		"    );\n" +
		"}\n"
	defer prepareBindTest(t, map[string]string{"multiline.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	expected := "#include \"trice_multiline_c_K1111111111111111_R0_begin.h\" // trice-bind: generated rebase begin K1111111111111111_R0\n" +
		"    LOG_ERROR(\n" +
		"        value\n" +
		"    );\n" +
		"#include \"trice_multiline_c_K1111111111111111_R0_end.h\" // trice-bind: generated rebase end K1111111111111111_R0\n"
	assert.Contains(t, string(bound), expected)
	assert.Equal(t, 1, strings.Count(string(bound), bindRebaseIncludeMarker+"begin"))

	_, sidecar := readOwnedBindSidecar(t, Srcs[0])
	assert.Contains(t, string(sidecar), "#define TRICE_BIND_REBASE_COUNT_K1111111111111111_R0 2")
}

// TestBindMVP2AllowsDataStringificationAndMacroOwnedSemicolon verifies that
// preprocessing unrelated to a static Trice format does not exclude an
// otherwise transparent statement wrapper. The replacement-owned semicolon
// makes each source invocation a complete statement without an extra token.
func TestBindMVP2AllowsDataStringificationAndMacroOwnedSemicolon(t *testing.T) {
	source := "#define COPY(element) do { const char *name = #element; triceS(\"name=%s\", name); trice(\"size=%d\", 4); } while (0);\n" +
		"void f(void) {\n" +
		"    COPY(first)\n" +
		"    COPY(second)\n" +
		"}\n"
	defer prepareBindTest(t, map[string]string{"copy.c": source})()

	var output bytes.Buffer
	require.NoErrorf(t, SubCmdIdBind(&output, FSys), "%s", output.String())
	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	text := string(bound)
	assert.Contains(t, text, "    COPY(first)\n")
	assert.Contains(t, text, "    COPY(second)\n")
	assert.Equal(t, 2, strings.Count(text, bindRebaseIncludeMarker+"begin"))
	assert.Equal(t, 2, strings.Count(text, bindRebaseIncludeMarker+"end"))

	_, sidecar := readOwnedBindSidecar(t, Srcs[0])
	assert.Equal(t, 2, strings.Count(string(sidecar), "#define TRICE_BIND_DEFINITION_"))
	assert.Len(t, IDData.idToTrice, 2)
}

// TestBindMVP2AcceptsFormattedGeneratedIncludes proves that formatter-owned
// horizontal spacing remains byte-for-byte intact across a normal bind run.
func TestBindMVP2AcceptsFormattedGeneratedIncludes(t *testing.T) {
	source := "void f(void) { trice(\"first\"); trice(\"second\"); }\n"
	defer prepareBindTest(t, map[string]string{"formatted.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	formatted := bytes.ReplaceAll(bound, []byte("\" // trice-bind: generated rebase"), []byte("\"   // trice-bind: generated rebase"))
	require.NotEqual(t, bound, formatted)
	require.NoError(t, FSys.WriteFile(Srcs[0], formatted, 0o644))

	var output bytes.Buffer
	require.NoErrorf(t, SubCmdIdBind(&output, FSys), "%s", output.String())
	regenerated, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, formatted, regenerated)
	assert.Equal(t, 1, strings.Count(string(regenerated), bindRebaseIncludeMarker+"begin"))
	assert.Equal(t, 1, strings.Count(string(regenerated), bindRebaseIncludeMarker+"end"))
}

// TestBindMVP2RemovesStaleHelperHeaders verifies that reducing a source region
// to the ordinary line path leaves no obsolete generated phase files behind.
func TestBindMVP2RemovesStaleHelperHeaders(t *testing.T) {
	source := "void f(void) { trice(\"first\"); trice(\"second\"); }\n"
	defer prepareBindTest(t, map[string]string{"stale-helper.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	beginPath := filepath.Join(BindDir, "trice_stale_helper_c_K1111111111111111_R0_begin.h")
	endPath := filepath.Join(BindDir, "trice_stale_helper_c_K1111111111111111_R0_end.h")
	_, err := FSys.Stat(beginPath)
	require.NoError(t, err)
	_, err = FSys.Stat(endPath)
	require.NoError(t, err)

	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	changed := bytes.Replace(bound, []byte("trice(\"first\"); trice(\"second\");"), []byte("trice(\"first\");"), 1)
	require.NotEqual(t, bound, changed)
	require.NoError(t, FSys.WriteFile(Srcs[0], changed, 0o644))
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	_, err = FSys.Stat(beginPath)
	assert.Error(t, err)
	_, err = FSys.Stat(endPath)
	assert.Error(t, err)
}

// TestBindMVP2ConvertsLegacyVerboseBlocks verifies that one normal bind run
// migrates the previous source representation without losing owner identity.
func TestBindMVP2ConvertsLegacyVerboseBlocks(t *testing.T) {
	key := "K0123456789ABCDEF"
	name := "trice_legacy_c_" + key + ".h"
	scope := key + "_R0"
	source := "#include \"" + name + "\" // trice-bind: keep as last include before this file's Trice calls\n" +
		"/* trice-bind: generated rebase begin " + scope + " */\n" +
		"#define TRICE_BIND_REBASE_SCOPE " + scope + "\n" +
		"#define TRICE_BIND_REBASE_BEGIN\n" +
		"#include \"" + name + "\" // trice-bind: generated rebase begin\n" +
		"#undef TRICE_BIND_REBASE_BEGIN\n" +
		bindRebaseBlockEnd + "\n" +
		"void f(void) { trice(\"first\"); trice(\"second\"); }\n" +
		"/* trice-bind: generated rebase end " + scope + " */\n" +
		"#define TRICE_BIND_REBASE_END\n" +
		"#include \"" + name + "\" // trice-bind: generated rebase end\n" +
		"#undef TRICE_BIND_REBASE_END\n" +
		bindRebaseBlockEnd + "\n"
	defer prepareBindTest(t, map[string]string{"legacy.c": source})()
	writeValidBindSidecar(t, name, key)

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.NotContains(t, string(bound), bindRebaseBeginMarker)
	assert.NotContains(t, string(bound), bindRebaseBlockEnd)
	assert.Equal(t, 1, strings.Count(string(bound), bindRebaseIncludeMarker+"begin"))
	_, err = FSys.Stat(filepath.Join(BindDir, "trice_legacy_c_"+key+"_R0_begin.h"))
	require.NoError(t, err)
}

// TestBindMVP2RejectsUnsupportedConstructsTransactionally exercises generator
// boundaries and verifies that every fatal analysis leaves regular files and
// generated lists byte-for-byte unchanged.
func TestBindMVP2RejectsUnsupportedConstructsTransactionally(t *testing.T) {
	tests := []struct {
		name     string
		source   string
		expected string
	}{
		{name: "zero placeholder", source: "void f(void) { trice(id(0), \"first\"); trice(\"second\"); }\n", expected: "id(0)"},
		{name: "nested wrapper", source: "#define INNER() trice(\"inner\")\n#define OUTER() do { INNER(); } while (0)\nvoid f(void) { OUTER(); }\n", expected: "nested logging wrapper"},
		{name: "recursive wrapper", source: "#define LOOP() do { trice(\"loop\"); LOOP(); } while (0)\nvoid f(void) { LOOP(); }\n", expected: "recursive logging wrapper"},
		{name: "token paste", source: "#define LOG(value) do { trice(\"value\"); int value##Copy = 0; (void)value##Copy; } while (0)\nvoid f(void) { LOG(v); }\n", expected: "unsupported token pasting"},
		{name: "stringified format", source: "#define LOG(value) do { trice(\"fixed\"); trice(#value); } while (0)\nvoid f(void) { LOG(v); }\n", expected: "no supported format string"},
		{name: "stringification changes literal format", source: "#define LOG(value) do { trice(\"prefix=\" #value); trice(\"fixed\"); } while (0)\nvoid f(void) { LOG(v); }\n", expected: "format"},
		{name: "dynamic format", source: "#define LOG(format) trice(format)\nvoid f(void) { LOG(\"dynamic\"); }\n", expected: "format"},
		{name: "indirect redefinition", source: "#define trice(...) backend(__VA_ARGS__)\nvoid f(void) { trice(\"redefined\"); }\n", expected: "indirect redefinition"},
		{name: "explicit definition ID", source: "#define LOG() trice(iD(123), \"explicit\")\nvoid f(void) { LOG(); }\n", expected: "explicit non-zero"},
		{name: "unsafe expression", source: "int f(void) { return (trice(\"first\"), trice(\"second\"), 0); }\n", expected: "standalone"},
		{name: "continued control flow", source: "void f(int value) {\n    if (value)\n        trice(\"first\"); trice(\"second\");\n}\n", expected: "independent block item"},
		{name: "closing outer scope", source: "void f(void) {\n    trice(\"first\"); trice(\"second\"); }\n", expected: "independent block item"},
		{name: "foreign counter", source: "void f(void) { trice(\"first\"); enum { foreign = __COUNTER__ }; trice(\"second\"); }\n", expected: "additional explicit __COUNTER__"},
	}

	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			defer prepareBindTest(t, map[string]string{"invalid.c": test.source})()
			tilBefore, err := FSys.ReadFile(FnJSON)
			require.NoError(t, err)
			liBefore, err := FSys.ReadFile(LIFnJSON)
			require.NoError(t, err)

			var output bytes.Buffer
			err = SubCmdIdBind(&output, FSys)
			require.Error(t, err)
			assert.Contains(t, output.String(), test.expected)
			after, readErr := FSys.ReadFile(Srcs[0])
			require.NoError(t, readErr)
			assert.Equal(t, test.source, string(after))
			tilAfter, readErr := FSys.ReadFile(FnJSON)
			require.NoError(t, readErr)
			assert.Equal(t, tilBefore, tilAfter)
			liAfter, readErr := FSys.ReadFile(LIFnJSON)
			require.NoError(t, readErr)
			assert.Equal(t, liBefore, liAfter)
		})
	}
}

// TestBindMVP2RejectsMalformedGeneratedArtifacts ensures a marker-like block
// cannot authorize partial deletion or regeneration when one owned directive
// was edited independently.
func TestBindMVP2RejectsMalformedGeneratedArtifacts(t *testing.T) {
	source := "void f(void) { trice(\"first\"); trice(\"second\"); }\n"
	defer prepareBindTest(t, map[string]string{"markers.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	corrupt := bytes.Replace(
		bound,
		[]byte("generated rebase begin K1111111111111111_R0"),
		[]byte("generated rebase begin K1111111111111111_R9"),
		1,
	)
	require.NotEqual(t, bound, corrupt)
	require.NoError(t, FSys.WriteFile(Srcs[0], corrupt, 0o644))
	_, sidecarBefore := readOwnedBindSidecar(t, Srcs[0])
	tilBefore, err := FSys.ReadFile(FnJSON)
	require.NoError(t, err)
	liBefore, err := FSys.ReadFile(LIFnJSON)
	require.NoError(t, err)

	var output bytes.Buffer
	err = SubCmdIdBind(&output, FSys)
	require.Error(t, err)
	assert.Contains(t, output.String(), "rebase include does not match the generated directive structure")
	after, readErr := FSys.ReadFile(Srcs[0])
	require.NoError(t, readErr)
	assert.Equal(t, corrupt, after)
	_, sidecarAfter := readOwnedBindSidecar(t, Srcs[0])
	assert.Equal(t, sidecarBefore, sidecarAfter)
	tilAfter, readErr := FSys.ReadFile(FnJSON)
	require.NoError(t, readErr)
	assert.Equal(t, tilBefore, tilAfter)
	liAfter, readErr := FSys.ReadFile(LIFnJSON)
	require.NoError(t, readErr)
	assert.Equal(t, liBefore, liAfter)
}

// TestBindMVP2RejectsModifiedHelperTransactionally keeps generated source and
// regular outputs untouched when a referenced phase header was edited.
func TestBindMVP2RejectsModifiedHelperTransactionally(t *testing.T) {
	source := "void f(void) { trice(\"first\"); trice(\"second\"); }\n"
	defer prepareBindTest(t, map[string]string{"helper.c": source})()

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	bound, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	helperPath := filepath.Join(BindDir, "trice_helper_c_K1111111111111111_R0_begin.h")
	helper, err := FSys.ReadFile(helperPath)
	require.NoError(t, err)
	modified := append(append([]byte(nil), helper...), []byte("// local edit\n")...)
	require.NoError(t, FSys.WriteFile(helperPath, modified, 0o644))

	var output bytes.Buffer
	err = SubCmdIdBind(&output, FSys)
	require.Error(t, err)
	assert.Contains(t, output.String(), "existing generated Trice bind rebase helper was modified")
	after, readErr := FSys.ReadFile(Srcs[0])
	require.NoError(t, readErr)
	assert.Equal(t, bound, after)
	helperAfter, readErr := FSys.ReadFile(helperPath)
	require.NoError(t, readErr)
	assert.Equal(t, modified, helperAfter)
}

// TestBindReusesReadOnlyMetadataAndRegeneratesMovedSidecar verifies the normal
// nested-project migration path: a custom-named TIL and LI plus an old sidecar
// preserve the ID, while the current BindDir receives freshly rendered bytes.
func TestBindReusesReadOnlyMetadataAndRegeneratesMovedSidecar(t *testing.T) {
	key := "K2222222222222222"
	sidecarName := "trice_module_c_" + key + ".h"
	source := "#include \"" + sidecarName + "\" // trice-bind: keep as last include before this file's Trice calls\n" +
		"\n// Lines added after the historical sidecar was generated.\n\n" +
		"void f(void) { trice(\"nested message\"); }\n"
	defer prepareBindTest(t, map[string]string{"sub/deep/module.c": source})()

	project := filepath.Join(Proj, t.Name())
	metadataRoot := filepath.Join(project, "sub")
	secondaryTILPath := filepath.Join(metadataRoot, "demoIDs.json")
	secondaryLIPath := filepath.Join(metadataRoot, "sourceLocations.json")
	secondaryTIL := writeBindTestTIL(t, secondaryTILPath, TriceIDLookUp{151: {Type: "trice", Strg: "nested message"}})
	secondaryLI := writeBindTestLI(t, secondaryLIPath, TriceIDLookUpLI{151: {File: "deep/module.c", Line: 2}})

	historicalDir := filepath.Join(metadataRoot, "build", "triceIDs")
	require.NoError(t, FSys.MkdirAll(historicalDir, 0o755))
	historicalSidecar := []byte("#define TRICE_BIND_FILE_KEY " + key + "\n" +
		"#define TRICE_BIND_ROUTE_" + key + " BIND\n" +
		"#define TRICE_BIND_SITE_" + key + "_L2 TRICE_BIND_AUTO, iD(151u)\n")
	require.NoError(t, FSys.WriteFile(filepath.Join(historicalDir, sidecarName), historicalSidecar, 0o644))

	// A different output tree proves that historical sidecars are evidence, not
	// files copied into the current build.
	BindDir = filepath.Join(project, "parent-build", "triceIDs")
	beforeSource, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	Verbose = true
	var output bytes.Buffer
	require.NoErrorf(t, SubCmdIdBind(&output, FSys), "%s", output.String())

	afterSource, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, beforeSource, afterSource)
	assert.Contains(t, output.String(), "reuse ID 151 from read-only metadata")
	assert.Equal(t, TriceFmt{Type: "trice", Strg: "nested message"}, IDData.idToTrice[151])

	generated, err := FSys.ReadFile(filepath.Join(BindDir, sidecarName))
	require.NoError(t, err)
	assert.Contains(t, string(generated), "iD(151u)")
	assert.NotEqual(t, historicalSidecar, generated)
	historicalAfter, err := FSys.ReadFile(filepath.Join(historicalDir, sidecarName))
	require.NoError(t, err)
	assert.Equal(t, historicalSidecar, historicalAfter)
	secondaryTILAfter, err := FSys.ReadFile(secondaryTILPath)
	require.NoError(t, err)
	assert.Equal(t, secondaryTIL, secondaryTILAfter)
	secondaryLIAfter, err := FSys.ReadFile(secondaryLIPath)
	require.NoError(t, err)
	assert.Equal(t, secondaryLI, secondaryLIAfter)
}

// TestBindReusesContentDetectedTILWithoutHistoricalSidecar proves that build
// artifacts are optional and that a source without a File Key receives one
// while retaining an ID from arbitrarily named JSON one level above -src.
func TestBindReusesContentDetectedTILWithoutHistoricalSidecar(t *testing.T) {
	source := "void f(void) { trice(\"metadata only\"); }\n"
	defer prepareBindTest(t, map[string]string{"firmware/src/module.c": source})()

	project := filepath.Join(Proj, t.Name())
	Srcs = ArrayFlag{filepath.Join(project, "firmware", "src")}
	metadataRoot := filepath.Join(project, "firmware")
	secondaryTILPath := filepath.Join(metadataRoot, "productMessageDatabase.json")
	secondaryLIPath := filepath.Join(metadataRoot, "anything.json")
	secondaryTIL := writeBindTestTIL(t, secondaryTILPath, TriceIDLookUp{160: {Type: "trice", Strg: "metadata only"}})
	secondaryLI := writeBindTestLI(t, secondaryLIPath, TriceIDLookUpLI{160: {File: "src/module.c", Line: 1}})

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	sourcePath := filepath.Join(project, "firmware", "src", "module.c")
	bound, err := FSys.ReadFile(sourcePath)
	require.NoError(t, err)
	assert.Contains(t, string(bound), "// trice-bind: keep as last include")
	assert.Equal(t, TriceFmt{Type: "trice", Strg: "metadata only"}, IDData.idToTrice[160])
	_, sidecar := readOwnedBindSidecar(t, sourcePath)
	assert.Contains(t, string(sidecar), "iD(160u)")

	secondaryTILAfter, err := FSys.ReadFile(secondaryTILPath)
	require.NoError(t, err)
	assert.Equal(t, secondaryTIL, secondaryTILAfter)
	secondaryLIAfter, err := FSys.ReadFile(secondaryLIPath)
	require.NoError(t, err)
	assert.Equal(t, secondaryLI, secondaryLIAfter)
}

// TestBindSecondaryIDCollisionYieldsToPrimary verifies that a nested project's
// conflicting numeric ID is a recoverable condition rather than a fatal merge.
func TestBindSecondaryIDCollisionYieldsToPrimary(t *testing.T) {
	key := "K3333333333333333"
	sidecarName := "trice_collision_c_" + key + ".h"
	source := "#include \"" + sidecarName + "\" // trice-bind: keep as last include before this file's Trice calls\n" +
		"void f(void) { trice(\"secondary format\"); }\n"
	defer prepareBindTest(t, map[string]string{"nested/collision.c": source})()

	project := filepath.Join(Proj, t.Name())
	writeBindTestTIL(t, FnJSON, TriceIDLookUp{151: {Type: "trice", Strg: "primary format"}})
	secondaryTILPath := filepath.Join(project, "nested", "demoIDs.json")
	secondaryBefore := writeBindTestTIL(t, secondaryTILPath, TriceIDLookUp{151: {Type: "trice", Strg: "secondary format"}})
	historicalDir := filepath.Join(project, "nested", "build", "triceIDs")
	require.NoError(t, FSys.MkdirAll(historicalDir, 0o755))
	require.NoError(t, FSys.WriteFile(filepath.Join(historicalDir, sidecarName), []byte(
		"#define TRICE_BIND_FILE_KEY "+key+"\n"+
			"#define TRICE_BIND_ROUTE_"+key+" BIND\n"+
			"#define TRICE_BIND_SITE_"+key+"_L2 TRICE_BIND_AUTO, iD(151u)\n"), 0o644))
	BindDir = filepath.Join(project, "root-build", "triceIDs")

	Verbose = true
	var output bytes.Buffer
	require.NoErrorf(t, SubCmdIdBind(&output, FSys), "%s", output.String())
	assert.Contains(t, output.String(), "read-only ID 151")
	assert.Contains(t, output.String(), "yields to the primary TIL")
	assert.Equal(t, TriceFmt{Type: "trice", Strg: "primary format"}, IDData.idToTrice[151])
	assert.Equal(t, TriceFmt{Type: "trice", Strg: "secondary format"}, IDData.idToTrice[100])
	generated, err := FSys.ReadFile(filepath.Join(BindDir, sidecarName))
	require.NoError(t, err)
	assert.Contains(t, string(generated), "iD(100u)")
	secondaryAfter, err := FSys.ReadFile(secondaryTILPath)
	require.NoError(t, err)
	assert.Equal(t, secondaryBefore, secondaryAfter)
}

// TestBindConflictingSecondaryProjectsResolveDeterministically verifies that
// two read-only projects may reuse one local number differently. Sorted source
// order claims the free primary number once; the later project receives a new
// primary ID without either secondary TIL being rewritten.
func TestBindConflictingSecondaryProjectsResolveDeterministically(t *testing.T) {
	keyA := "K4444444444444444"
	keyB := "K5555555555555555"
	sourceA := "#include \"trice_module_c_" + keyA + ".h\" // trice-bind: keep as last include before this file's Trice calls\n" +
		"void a(void) { trice(\"project a\"); }\n"
	sourceB := "#include \"trice_module_c_" + keyB + ".h\" // trice-bind: keep as last include before this file's Trice calls\n" +
		"void b(void) { trice(\"project b\"); }\n"
	defer prepareBindTest(t, map[string]string{"a/module.c": sourceA, "b/module.c": sourceB})()

	project := filepath.Join(Proj, t.Name())
	Srcs = ArrayFlag{project}
	aTILPath := filepath.Join(project, "a", "local.json")
	bTILPath := filepath.Join(project, "b", "local.json")
	aBefore := writeBindTestTIL(t, aTILPath, TriceIDLookUp{150: {Type: "trice", Strg: "project a"}})
	bBefore := writeBindTestTIL(t, bTILPath, TriceIDLookUp{150: {Type: "trice", Strg: "project b"}})
	writeBindTestLI(t, filepath.Join(project, "a", "where.json"), TriceIDLookUpLI{150: {File: "module.c", Line: 2}})
	writeBindTestLI(t, filepath.Join(project, "b", "where.json"), TriceIDLookUpLI{150: {File: "module.c", Line: 2}})

	Verbose = true
	var output bytes.Buffer
	require.NoErrorf(t, SubCmdIdBind(&output, FSys), "%s", output.String())
	assert.Equal(t, TriceFmt{Type: "trice", Strg: "project a"}, IDData.idToTrice[150])
	assert.Equal(t, TriceFmt{Type: "trice", Strg: "project b"}, IDData.idToTrice[100])
	assert.Contains(t, output.String(), "read-only ID 150")
	assert.Contains(t, output.String(), "yields to the primary TIL")

	aAfter, err := FSys.ReadFile(aTILPath)
	require.NoError(t, err)
	assert.Equal(t, aBefore, aAfter)
	bAfter, err := FSys.ReadFile(bTILPath)
	require.NoError(t, err)
	assert.Equal(t, bBefore, bAfter)
}

// TestBindDirectorySourceIgnoresHiddenFolders ensures neither source files nor
// tempting metadata below .git-like or .trice-like trees influence bind.
func TestBindDirectorySourceIgnoresHiddenFolders(t *testing.T) {
	defer prepareBindTest(t, map[string]string{
		"visible.c":                "void visible(void) { trice(\"visible\"); }\n",
		".trice/cache/hidden.c":    "void hidden(void) { trice(\"hidden\"); }\n",
		".git/generated/ignored.c": "void ignored(void) { trice(\"ignored\"); }\n",
	})()

	project := filepath.Join(Proj, t.Name())
	Srcs = ArrayFlag{project}
	hiddenTILPath := filepath.Join(project, ".trice", "cache", "oldIDs.json")
	writeBindTestTIL(t, hiddenTILPath, TriceIDLookUp{155: {Type: "trice", Strg: "visible"}})
	hiddenBefore, err := FSys.ReadFile(filepath.Join(project, ".trice", "cache", "hidden.c"))
	require.NoError(t, err)
	gitBefore, err := FSys.ReadFile(filepath.Join(project, ".git", "generated", "ignored.c"))
	require.NoError(t, err)

	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	visible, err := FSys.ReadFile(filepath.Join(project, "visible.c"))
	require.NoError(t, err)
	assert.Contains(t, string(visible), "// trice-bind: keep as last include")
	assert.Equal(t, TriceFmt{Type: "trice", Strg: "visible"}, IDData.idToTrice[100])
	_, importedHiddenID := IDData.idToTrice[155]
	assert.False(t, importedHiddenID)
	hiddenAfter, err := FSys.ReadFile(filepath.Join(project, ".trice", "cache", "hidden.c"))
	require.NoError(t, err)
	assert.Equal(t, hiddenBefore, hiddenAfter)
	gitAfter, err := FSys.ReadFile(filepath.Join(project, ".git", "generated", "ignored.c"))
	require.NoError(t, err)
	assert.Equal(t, gitBefore, gitAfter)
}

// TestClassifyBindMetadataJSONByContent documents the filename-independent
// schema discriminator and its deliberate treatment of empty optional maps.
func TestClassifyBindMetadataJSONByContent(t *testing.T) {
	tilKind, til, _, err := classifyBindMetadataJSON([]byte(`{"123":{"Type":"trice","Strg":"x"}}`))
	require.NoError(t, err)
	assert.Equal(t, bindMetadataTIL, tilKind)
	assert.Equal(t, TriceFmt{Type: "trice", Strg: "x"}, til[123])

	liKind, _, li, err := classifyBindMetadataJSON([]byte(`{"123":{"File":"src/a.c","Line":7}}`))
	require.NoError(t, err)
	assert.Equal(t, bindMetadataLI, liKind)
	assert.Equal(t, TriceLI{File: "src/a.c", Line: 7}, li[123])

	emptyKind, _, _, err := classifyBindMetadataJSON([]byte(`{}`))
	require.NoError(t, err)
	assert.Equal(t, bindMetadataUnknown, emptyKind)
	_, _, _, err = classifyBindMetadataJSON([]byte(`{"123":{"Type":"trice","Line":7}}`))
	assert.Error(t, err)
}
