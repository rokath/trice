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
	assert.False(t, bindRebaseLineIsIndependent(source, start, end, occurrence))

	safe := "static inline void direct(void) { trice(\"first\"); trice(\"second\"); }\nstatic inline void wrapper(int value) { LOG_ERROR(value); }\n"
	occurrence = strings.Index(safe, "LOG_ERROR")
	require.NotEqual(t, -1, occurrence)
	start = lineStart(safe, occurrence)
	end = lineEndIncludingNewline(safe, occurrence)
	assert.True(t, bindRebaseLineIsIndependent(safe, start, end, occurrence))

	safe = "#define LOG_ERROR(value) \\\n    do { trice(\"inside\"); } while (0)\nstatic inline void direct(void) { trice(\"first\"); trice(\"second\"); }\n"
	occurrence = strings.Index(safe, "trice(\"first\")")
	require.NotEqual(t, -1, occurrence)
	start = lineStart(safe, occurrence)
	end = lineEndIncludingNewline(safe, occurrence)
	assert.True(t, bindRebaseLineIsIndependent(safe, start, end, occurrence))
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
	shifted := strings.Replace(string(bound), bindRebaseBeginMarker, "// user line shift\n"+bindRebaseBeginMarker, 1)
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
	assert.Equal(t, 1, strings.Count(string(regenerated), bindRebaseBeginMarker))
	assert.Equal(t, 1, strings.Count(string(regenerated), bindRebaseEndMarker))
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
		{name: "stringification", source: "#define LOG(value) do { trice(\"fixed\"); trice(#value); } while (0)\nvoid f(void) { LOG(v); }\n", expected: "unsupported stringification"},
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
	corrupt := bytes.Replace(bound, []byte("#undef TRICE_BIND_REBASE_BEGIN"), []byte("#undef TRICE_BIND_REBASE_BEGIN_BROKEN"), 1)
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
	assert.Contains(t, output.String(), "does not match the generated directive structure")
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
