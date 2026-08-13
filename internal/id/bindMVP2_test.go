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
// horizontal spacing cannot make an otherwise intact generated boundary look
// corrupt. A normal bind run may canonicalize that spacing again.
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
