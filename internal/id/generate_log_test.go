// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"path/filepath"
	"testing"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestSelectCurrentLogEntriesFromInsertedSource verifies that -logC trusts an
// explicit source ID only after its complete type/string identity matches TIL.
func TestSelectCurrentLogEntriesFromInsertedSource(t *testing.T) {
	defer Setup(t)()
	source := filepath.Join(Proj, "inserted.c")
	require.NoError(t, FSys.WriteFile(source, []byte("void f(void) { trice(iD(1000), \"value=%d\", 7); }\n// trice(iD(1001), \"commented=%x\", 8);\n"), 0o644))
	Srcs = ArrayFlag{source}

	til := TriceIDLookUp{
		1000: {Type: "trice", Strg: "value=%d"},
		1001: {Type: "trice", Strg: "commented=%x"},
		1999: {Type: "trice", Strg: "old and unused"},
	}
	selected, err := selectCurrentLogEntries(&B, FSys, til)
	require.NoError(t, err)
	assert.Equal(t, TriceIDLookUp{1000: til[1000], 1001: til[1001]}, selected)
}

// TestSelectCurrentLogEntriesFromBoundSource covers multiple sites on one
// physical line. Numeric location descriptors are ordered by their generated
// source ordinals and no format-based ID guess is needed.
func TestSelectCurrentLogEntriesFromBoundSource(t *testing.T) {
	defer Setup(t)()
	const key = "K1111111111111111"
	source := filepath.Join(Proj, "bound.c")
	sidecarName := "trice_bound_c_" + key + ".h"
	sourceText := "#include \"" + sidecarName + "\"\nvoid f(void) { trice(\"first\"); trice(\"second=%x\", 2); }\n"
	require.NoError(t, FSys.WriteFile(source, []byte(sourceText), 0o644))
	BindDir = filepath.Join(Proj, "build", "triceIDs")
	require.NoError(t, FSys.MkdirAll(BindDir, 0o755))
	sidecar := "#define TRICE_BIND_FILE_KEY " + key + "\n" +
		"#define TRICE_BIND_ROUTE_" + key + " BIND\n" +
		"#define TRICE_BIND_ID_LOCATION_" + key + "_L2_O0 1001u\n" +
		"#define TRICE_BIND_ID_LOCATION_" + key + "_L2_O1 1002u\n"
	require.NoError(t, FSys.WriteFile(filepath.Join(BindDir, sidecarName), []byte(sidecar), 0o644))
	Srcs = ArrayFlag{source}

	til := TriceIDLookUp{
		1001: {Type: "trice", Strg: "first"},
		1002: {Type: "trice", Strg: "second=%x"},
	}
	selected, err := selectCurrentLogEntries(&B, FSys, til)
	require.NoError(t, err)
	assert.Equal(t, til, selected)
}

// TestSelectCurrentLogEntriesIgnoresOriginalZeroIDInSidecarComment protects
// replace descriptors generated from insert-style source. Their diagnostic
// comment retains the original Id(0), but the first constructor is the only
// authoritative ID and must be selected for -logC.
func TestSelectCurrentLogEntriesIgnoresOriginalZeroIDInSidecarComment(t *testing.T) {
	defer Setup(t)()
	const key = "K5555555555555555"
	source := filepath.Join(Proj, "replaced.c")
	sidecarName := "trice_replaced_c_" + key + ".h"
	sourceText := "#include \"" + sidecarName + "\"\nvoid f(void) { trice(\"value=%d\", 7); }\n"
	require.NoError(t, FSys.WriteFile(source, []byte(sourceText), 0o644))
	BindDir = filepath.Join(Proj, "build", "triceIDs")
	require.NoError(t, FSys.MkdirAll(BindDir, 0o755))
	sidecar := "#define TRICE_BIND_FILE_KEY " + key + "\n" +
		"#define TRICE_BIND_ROUTE_" + key + " BIND\n" +
		"#define TRICE_BIND_SITE_" + key + "_L2 TRICE_BIND_REPLACE, Id(1001u) // trice(Id(0), \"value=%d\", 7);\n"
	require.NoError(t, FSys.WriteFile(filepath.Join(BindDir, sidecarName), []byte(sidecar), 0o644))
	Srcs = ArrayFlag{source}

	til := TriceIDLookUp{1001: {Type: "trice", Strg: "value=%d"}}
	selected, err := selectCurrentLogEntries(&B, FSys, til)
	require.NoError(t, err)
	assert.Equal(t, til, selected)
}

// TestSelectCurrentLogEntriesRejectsStaleDescriptorCount verifies that -logC
// does not pair current source sites with an incomplete, stale bind sidecar.
// Regenerating the sidecar is safer than assigning the remaining IDs by order.
func TestSelectCurrentLogEntriesRejectsStaleDescriptorCount(t *testing.T) {
	defer Setup(t)()
	const key = "K4444444444444444"
	source := filepath.Join(Proj, "stale-count.c")
	sidecarName := "trice_stale_count_c_" + key + ".h"
	sourceText := "#include \"" + sidecarName + "\"\nvoid f(void) { trice(\"first\"); trice(\"second\"); }\n"
	require.NoError(t, FSys.WriteFile(source, []byte(sourceText), 0o644))
	BindDir = filepath.Join(Proj, "build", "triceIDs")
	require.NoError(t, FSys.MkdirAll(BindDir, 0o755))
	sidecar := "#define TRICE_BIND_FILE_KEY " + key + "\n" +
		"#define TRICE_BIND_ROUTE_" + key + " BIND\n" +
		"#define TRICE_BIND_ID_LOCATION_" + key + "_L2_O0 1001u\n"
	require.NoError(t, FSys.WriteFile(filepath.Join(BindDir, sidecarName), []byte(sidecar), 0o644))
	Srcs = ArrayFlag{source}

	_, err := selectCurrentLogEntries(&B, FSys, TriceIDLookUp{
		1001: {Type: "trice", Strg: "first"},
		1002: {Type: "trice", Strg: "second"},
	})
	require.Error(t, err)
	assert.Contains(t, B.String(), "has 2 active site(s), but sidecar "+sidecarName+" has 1 numeric descriptor(s)")
}

// TestSelectCurrentLogEntriesFromBoundWrapper runs the real bind generator
// before selection. It protects the numeric definition descriptors used by a
// wrapper with multiple inner Trices and repeated invocation sites.
func TestSelectCurrentLogEntriesFromBoundWrapper(t *testing.T) {
	source := "#define LOG_PAIR(value) do { trice(\"first\"); trice(\"second=%d\", value); } while (0)\n" +
		"void f(void) { LOG_PAIR(1); LOG_PAIR(2); }\n" +
		"void g(void) { trice(\"after-generated-rebase\"); }\n"
	defer prepareBindTest(t, map[string]string{"wrapper.c": source})()

	require.NoError(t, SubCmdIdBind(&B, FSys))
	til := make(TriceIDLookUp, len(IDData.idToTrice))
	for id, entry := range IDData.idToTrice {
		til[id] = entry
	}

	selected, err := selectCurrentLogEntries(&B, FSys, til)
	require.NoError(t, err)
	assert.Equal(t, til, selected)
}

// TestSelectCurrentLogEntriesRejectsUnresolvedCommentedBindSite proves that a
// source-visible commented call is never dropped merely because active calls
// in the same file are bind-owned. The user can make its identity explicit or
// use insert when such a commented entry must remain in the local table.
func TestSelectCurrentLogEntriesRejectsUnresolvedCommentedBindSite(t *testing.T) {
	defer Setup(t)()
	const key = "K3333333333333333"
	source := filepath.Join(Proj, "commented.c")
	sidecarName := "trice_commented_c_" + key + ".h"
	sourceText := "#include \"" + sidecarName + "\"\nvoid f(void) { trice(\"active\"); }\n// trice(\"commented\");\n"
	require.NoError(t, FSys.WriteFile(source, []byte(sourceText), 0o644))
	BindDir = filepath.Join(Proj, "build", "triceIDs")
	require.NoError(t, FSys.MkdirAll(BindDir, 0o755))
	sidecar := "#define TRICE_BIND_FILE_KEY " + key + "\n" +
		"#define TRICE_BIND_ROUTE_" + key + " BIND\n" +
		"#define TRICE_BIND_SITE_" + key + "_L2 TRICE_BIND_AUTO, iD(1000)\n"
	require.NoError(t, FSys.WriteFile(filepath.Join(BindDir, sidecarName), []byte(sidecar), 0o644))
	Srcs = ArrayFlag{source}

	_, err := selectCurrentLogEntries(&B, FSys, TriceIDLookUp{1000: {Type: "trice", Strg: "active"}})
	require.Error(t, err)
	assert.Contains(t, B.String(), "commented.c:3: error: Trice site has no resolved non-zero ID")
}

// TestSelectCurrentLogEntriesSkipsUnsupportedCommentedSite limits the comment
// exception to formats that the local target formatter cannot represent. The
// active supported site remains selected while the documentation-only %U call
// needs neither a bind descriptor nor a target table row.
func TestSelectCurrentLogEntriesSkipsUnsupportedCommentedSite(t *testing.T) {
	defer Setup(t)()
	const key = "K6666666666666666"
	source := filepath.Join(Proj, "unsupported-comment.c")
	sidecarName := "trice_unsupported_comment_c_" + key + ".h"
	sourceText := "#include \"" + sidecarName + "\"\nvoid f(void) { trice(\"active\"); }\n// trice(\"unicode=%U\", 'A');\n"
	require.NoError(t, FSys.WriteFile(source, []byte(sourceText), 0o644))
	BindDir = filepath.Join(Proj, "build", "triceIDs")
	require.NoError(t, FSys.MkdirAll(BindDir, 0o755))
	sidecar := "#define TRICE_BIND_FILE_KEY " + key + "\n" +
		"#define TRICE_BIND_ROUTE_" + key + " BIND\n" +
		"#define TRICE_BIND_SITE_" + key + "_L2 TRICE_BIND_AUTO, iD(1000u)\n"
	require.NoError(t, FSys.WriteFile(filepath.Join(BindDir, sidecarName), []byte(sidecar), 0o644))
	Srcs = ArrayFlag{source}

	til := TriceIDLookUp{1000: {Type: "trice", Strg: "active"}}
	selected, err := selectCurrentLogEntries(&B, FSys, til)
	require.NoError(t, err)
	assert.Equal(t, til, selected)
}

// TestAddCurrentLogSiteRejectsInvalidMappings exercises the fail-closed TIL
// identity checks independently of source ownership. Each case must leave the
// selected table unchanged so callers can discard the complete failed result.
func TestAddCurrentLogSiteRejectsInvalidMappings(t *testing.T) {
	defer Setup(t)()
	FnJSON = "current-til.json"
	site := bindSite{line: 17, macro: "trice", format: "value=%d"}

	tests := []struct {
		name     string
		id       TriceID
		selected TriceIDLookUp
		til      TriceIDLookUp
		message  string
	}{
		{
			name:    "zero ID",
			message: "no resolved non-zero ID",
		},
		{
			name:    "ID absent from TIL",
			id:      1001,
			til:     TriceIDLookUp{},
			message: "Trice ID 1001 is absent from current-til.json",
		},
		{
			name: "source and TIL identity differ",
			id:   1001,
			til: TriceIDLookUp{
				1001: {Type: "trice", Strg: "other=%d"},
			},
			message: "Trice ID 1001 does not match current-til.json",
		},
		{
			name: "selected table already conflicts",
			id:   1001,
			selected: TriceIDLookUp{
				1001: {Type: "trice", Strg: "older=%d"},
			},
			til: TriceIDLookUp{
				1001: {Type: "trice", Strg: "value=%d"},
			},
			message: "Trice ID 1001 resolves to conflicting current entries",
		},
	}

	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			selected := test.selected
			if selected == nil {
				selected = make(TriceIDLookUp)
			}
			before := make(TriceIDLookUp, len(selected))
			for id, entry := range selected {
				before[id] = entry
			}

			diagnostics := addCurrentLogSite(selected, test.til, "source.c", site, test.id)
			require.Len(t, diagnostics, 1)
			assert.Contains(t, diagnostics[0].message, test.message)
			assert.Equal(t, before, selected)
		})
	}
}

// TestGenerateLogCRejectsStaleBindStateWithoutTouchingOutput verifies the
// transactional user-visible rule: every diagnostic is collected before the
// regular generated file is opened.
func TestGenerateLogCRejectsStaleBindStateWithoutTouchingOutput(t *testing.T) {
	defer Setup(t)()
	const key = "K2222222222222222"
	source := filepath.Join(Proj, "stale.c")
	require.NoError(t, FSys.WriteFile(source, []byte("#include \"trice_stale_c_"+key+".h\"\nvoid f(void) { trice(\"new\"); }\n"), 0o644))
	Srcs = ArrayFlag{source}
	BindDir = filepath.Join(Proj, "missing", "triceIDs")
	FnJSON = filepath.Join(Proj, "current-til.json")
	require.NoError(t, (TriceIDLookUp{1000: {Type: "trice", Strg: "new"}}).toFile(FSys.Fs, FnJSON))
	output := filepath.Join(Proj, "generated", "til.c")
	require.NoError(t, FSys.MkdirAll(filepath.Dir(output), 0o755))
	require.NoError(t, FSys.WriteFile(output, []byte("keep me\n"), 0o644))
	GenerateLogC = true
	GenerateLogCPath = output
	GenerateABC = ""

	err := SubCmdGenerate(&B, FSys)
	require.Error(t, err)
	assert.Contains(t, B.String(), "run trice bind or pass its directory with -bindDir")
	unchanged, readErr := FSys.ReadFile(output)
	require.NoError(t, readErr)
	assert.Equal(t, "keep me\n", string(unchanged))
}

// TestGenerateLogCAndABCRejectCombination ensures the two generators cannot
// silently combine different table-selection semantics in one command.
func TestGenerateLogCAndABCRejectCombination(t *testing.T) {
	defer Setup(t)()
	GenerateLogC = true
	GenerateABC = "node"
	err := SubCmdGenerate(&bytes.Buffer{}, FSys)
	require.Error(t, err)
	assert.Contains(t, err.Error(), "-logC and -abc")
}

// TestGenerateLogCLeavesIdenticalOutputUntouched protects incremental target
// builds: rerunning the generator must not replace an unchanged C table and
// thereby trigger an unnecessary recompilation.
func TestGenerateLogCLeavesIdenticalOutputUntouched(t *testing.T) {
	defer Setup(t)()
	base := afero.NewMemMapFs()
	counting := &bindRenameCountingFs{Fs: base}
	fs := &afero.Afero{Fs: counting}
	source := filepath.Join(Proj, "stable.c")
	FnJSON = filepath.Join(Proj, "til.json")
	output := filepath.Join(Proj, "build", "til.c")
	require.NoError(t, fs.MkdirAll(filepath.Dir(output), 0o755))
	require.NoError(t, fs.WriteFile(source, []byte("void f(void) { trice(iD(1000), \"stable\"); }\n"), 0o644))
	require.NoError(t, (TriceIDLookUp{1000: {Type: "trice", Strg: "stable"}}).toFile(fs.Fs, FnJSON))
	Srcs = ArrayFlag{source}
	GenerateLogC = true
	GenerateLogCPath = output
	GenerateABC = ""

	require.NoError(t, SubCmdGenerate(&B, fs))
	firstRenames := counting.renames
	require.Positive(t, firstRenames)
	require.NoError(t, SubCmdGenerate(&B, fs))
	assert.Equal(t, firstRenames, counting.renames)
}

// TestGenerateLogCRejectsEmptySelection prevents a missing or mistaken -src
// from replacing a useful table with an empty generated translation unit.
func TestGenerateLogCRejectsEmptySelection(t *testing.T) {
	defer Setup(t)()
	source := filepath.Join(Proj, "empty.c")
	require.NoError(t, FSys.WriteFile(source, []byte("int value;\n"), 0o644))
	Srcs = ArrayFlag{source}

	_, err := selectCurrentLogEntries(&B, FSys, TriceIDLookUp{})
	require.Error(t, err)
	assert.Contains(t, B.String(), "no resolved current Trice sites were found")
}
