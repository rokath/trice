// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"io"
	"os"
	"path/filepath"
	"slices"
	"testing"
	"time"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// orderedReadFs changes parallel read completion order without changing bytes.
// The delay is small and affects only the selected source's Open calls.
type orderedReadFs struct {
	afero.Fs
	slow string
}

// Open deliberately finishes one file later to expose worker-order dependencies.
func (fs orderedReadFs) Open(name string) (afero.File, error) {
	if filepath.Clean(name) == fs.slow {
		time.Sleep(2 * time.Millisecond)
	}
	return fs.Fs.Open(name)
}

// orderedTestIDs reads actual source IDs or generated Bind descriptors in textual
// order. Checking only LI would miss a source/sidecar that retained the wrong ID.
func orderedTestIDs(t *testing.T, fs *afero.Afero, source string, bind bool) []TriceID {
	t.Helper()
	data, err := fs.ReadFile(source)
	require.NoError(t, err)
	var ids []TriceID
	if !bind {
		sites, diagnostics := scanSourceSites(source, string(data), false)
		require.Empty(t, diagnostics)
		for _, site := range sites {
			ids = append(ids, site.id)
		}
		return ids
	}
	for _, include := range scanBindIncludes(string(data)) {
		if !include.isSidecar {
			continue
		}
		data, err := fs.ReadFile(filepath.Join(BindDir, include.name))
		require.NoError(t, err)
		for _, site := range parseBindHistoricalSites(data, include.key) {
			ids = append(ids, site.id)
		}
		return ids
	}
	t.Fatalf("no owner sidecar in %s", source)
	return nil
}

// seedOrderedTest writes matching primary catalogs with deliberately shuffled
// source ownership. Historical entries remain available for older firmware.
func seedOrderedTest(t *testing.T, ids TriceIDLookUp, locations TriceIDLookUpLI) {
	t.Helper()
	data, err := ids.toJSON()
	require.NoError(t, err)
	require.NoError(t, FSys.WriteFile(FnJSON, data, 0o644))
	writeBindTestLI(t, LIFnJSON, locations)
}

// TestOrderedIDsThirdIdenticalSiteMayChangeBothExistingIDs documents the accepted
// 100/200/300 example for both public workflows, not just the sorting helper.
func TestOrderedIDsThirdIdenticalSiteMayChangeBothExistingIDs(t *testing.T) {
	for _, bind := range []bool{false, true} {
		name := "insert"
		if bind {
			name = "bind"
		}
		t.Run(name, func(t *testing.T) {
			b, d := "trice(iD(100), \"same\");\n", "trice(iD(200), \"same\");\n"
			if bind {
				b, d = "trice(\"same\");\n", "trice(\"same\");\n"
			}
			defer prepareBindTest(t, map[string]string{"b.c": b, "d.c": d})()
			// Each physical owner requires its own persistent file key.
			var keys []byte
			for value := byte(1); value <= 8; value++ {
				keys = append(keys, bytes.Repeat([]byte{value}, 8)...)
			}
			bindRandomReader = bytes.NewReader(keys)
			Max = 399
			project := filepath.Join(Proj, t.Name())
			LIRoot = project
			FieldsDir = filepath.Join(project, "generated")
			aPath, bPath, dPath := filepath.Join(project, "a.c"), filepath.Join(project, "b.c"), filepath.Join(project, "d.c")
			format := TriceFmt{Type: "trice", Strg: "same"}
			seedOrderedTest(t, TriceIDLookUp{100: format, 200: format, 300: format, 399: {Type: "trice", Strg: "historical"}}, TriceIDLookUpLI{
				100: {File: "b.c", Line: 1}, 200: {File: "d.c", Line: 1}, 300: {File: "a.c", Line: 1},
			})
			run := func() {
				t.Helper()
				if bind {
					require.NoError(t, SubCmdIdBind(io.Discard, FSys))
				} else {
					require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
				}
			}
			run()
			assert.Equal(t, []TriceID{100}, orderedTestIDs(t, FSys, bPath, bind))
			assert.Equal(t, []TriceID{200}, orderedTestIDs(t, FSys, dPath, bind))
			a := "trice(iD(300), \"same\");\n"
			if bind {
				a = "trice(\"same\");\n"
			}
			require.NoError(t, FSys.WriteFile(aPath, []byte(a), 0o644))
			// Overlapping roots and reversed CLI order must not duplicate sites.
			Srcs = ArrayFlag{dPath, project, bPath, aPath}
			run()
			for index, source := range []string{aPath, bPath, dPath} {
				want := TriceID((index + 1) * 100)
				assert.Equal(t, []TriceID{want}, orderedTestIDs(t, FSys, source, bind))
				location := NewLutLI(io.Discard, FSys, LIFnJSON)[want]
				assert.Equal(t, filepath.Base(source), location.File)
			}
			before, err := FSys.ReadFile(LIFnJSON)
			require.NoError(t, err)
			for _, slow := range []string{aPath, dPath} {
				Srcs = ArrayFlag{dPath, bPath, aPath}
				fs := &afero.Afero{Fs: orderedReadFs{Fs: FSys.Fs, slow: slow}}
				if bind {
					require.NoError(t, SubCmdIdBind(io.Discard, fs))
				} else {
					require.NoError(t, SubCmdIdInsert(io.Discard, fs))
				}
				after, err := FSys.ReadFile(LIFnJSON)
				require.NoError(t, err)
				assert.Equal(t, before, after, "worker completion must not affect location data")
			}
			assert.Equal(t, format, NewLut(io.Discard, FSys, FnJSON)[100])
			assert.Contains(t, NewLut(io.Discard, FSys, FnJSON), TriceID(399), "historical entries are not deleted")
			if !bind {
				require.NoError(t, SubCmdIdClean(io.Discard, FSys))
				run()
				assert.Equal(t, []TriceID{100}, orderedTestIDs(t, FSys, aPath, false))
				assert.Equal(t, []TriceID{200}, orderedTestIDs(t, FSys, bPath, false))
				assert.Equal(t, []TriceID{300}, orderedTestIDs(t, FSys, dPath, false))
			}
		})
	}
}

// TestOrderedIDsPartialScanReservesExcludedFiles checks both a copied explicit
// ID and an identical unselected source. Neither may move the outside mapping.
func TestOrderedIDsPartialScanReservesExcludedFiles(t *testing.T) {
	for _, bind := range []bool{false, true} {
		name := "insert"
		if bind {
			name = "bind"
		}
		t.Run(name, func(t *testing.T) {
			source := "trice(iD(100), \"same\");\n"
			if bind {
				source = "trice(\"same\");\n"
			}
			defer prepareBindTest(t, map[string]string{"a.c": source, "outside.c": "trice(iD(100), \"same\");\n"})()
			project := filepath.Join(Proj, t.Name())
			LIRoot = project
			FieldsDir = filepath.Join(project, "generated")
			a, outside := filepath.Join(project, "a.c"), filepath.Join(project, "outside.c")
			Srcs = ArrayFlag{project}
			ExcludeSrcs = ArrayFlag{outside}
			format := TriceFmt{Type: "trice", Strg: "same"}
			seedOrderedTest(t, TriceIDLookUp{100: format, 150: format}, TriceIDLookUpLI{100: {File: "outside.c", Line: 1}, 150: {File: "a.c", Line: 1}})
			original, err := FSys.ReadFile(outside)
			require.NoError(t, err)
			if bind {
				require.NoError(t, SubCmdIdBind(io.Discard, FSys))
			} else {
				require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
			}
			assert.Equal(t, []TriceID{150}, orderedTestIDs(t, FSys, a, bind))
			after, err := FSys.ReadFile(outside)
			require.NoError(t, err)
			assert.Equal(t, original, after)
			assert.Equal(t, TriceLI{File: "outside.c", Line: 1}, NewLutLI(io.Discard, FSys, LIFnJSON)[100])
		})
	}
}

// TestOrderedIDsSourcePositionAndIdentity keeps differing types and schemas in
// separate groups while sorting equal sites by line and left-to-right position.
func TestOrderedIDsSourcePositionAndIdentity(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"z.c": "trice(iD(103), \"same\"); trice(iD(101), \"same\");\ntrice(iD(102), \"same\");\nTRICE32_1(Id(150), \"same=%d\", 1);\ntrice(iD(151), \"other\");\n"})()
	LIRoot = filepath.Dir(Srcs[0])
	FieldsDir = filepath.Join(LIRoot, "generated")
	seedOrderedTest(t, TriceIDLookUp{101: {Type: "trice", Strg: "same"}, 102: {Type: "trice", Strg: "same"}, 103: {Type: "trice", Strg: "same"}, 150: {Type: "TRICE32_1", Strg: "same=%d"}, 151: {Type: "trice", Strg: "other"}}, TriceIDLookUpLI{})
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	assert.Equal(t, []TriceID{101, 102, 103, 150, 151}, orderedTestIDs(t, FSys, Srcs[0], false))
	first, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	second, err := FSys.ReadFile(Srcs[0])
	require.NoError(t, err)
	assert.Equal(t, first, second)
}

// TestOrderedIDsOrdinalPathsAndArrivalOrder simulates every relevant ordering
// input without depending on the operating system's directory enumeration.
func TestOrderedIDsOrdinalPathsAndArrivalOrder(t *testing.T) {
	defer Setup(t)()
	LIRoot = Proj
	Min = 100
	Max = 200
	format := TriceFmt{Type: "trice", Strg: "same"}
	for _, reversed := range []bool{false, true} {
		p := idData{idToTrice: TriceIDLookUp{100: format, 101: format, 102: format}, idToLocNew: make(TriceIDLookUpLI)}
		sites := []orderedIDSite{{path: filepath.Join(Proj, "z.c"), line: 1, column: 1, id: 100}, {path: filepath.Join(Proj, "sub", "..", "a.c"), line: 5, column: 2, id: 101}, {path: filepath.Join(Proj, "A.c"), line: 90, column: 1, id: 102}}
		if reversed {
			slices.Reverse(sites)
		}
		require.NoError(t, p.orderIdenticalIDs(sites))
		assert.Equal(t, "A.c", p.idToLocNew[100].File, "case-sensitive ordinal order comes before lowercase a")
		assert.Equal(t, "a.c", p.idToLocNew[101].File, "dot components are normalized")
		assert.Equal(t, "z.c", p.idToLocNew[102].File)
	}
}

// TestOrderedIDsCheckoutPrefixDoesNotDetermineOrder checks that an unchanged
// project gets the same comparison key after moving to another checkout root.
func TestOrderedIDsCheckoutPrefixDoesNotDetermineOrder(t *testing.T) {
	defer Setup(t)()
	for _, checkout := range []string{"checkout-z", "checkout-a"} {
		LIRoot = filepath.Join(Proj, checkout)
		assert.Equal(t, "sub/a.c", orderedIDPath(filepath.Join(LIRoot, "sub", "a.c")))
		assert.Equal(t, "sub/a.c", orderedIDPath(filepath.Join(LIRoot, `sub\a.c`)), "stored Windows separators normalize on every host")
	}
}

// TestOrderedIDsCacheCannotRestoreOldPermutation reproduces a cached unchanged
// file whose final IDs must change because a new earlier site was added.
func TestOrderedIDsCacheCannotRestoreOldPermutation(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"b.c": "trice(iD(100), \"same\");\n"})()
	project := filepath.Join(Proj, t.Name())
	LIRoot = project
	FieldsDir = filepath.Join(project, "generated")
	TriceCacheEnabled = true
	b := Srcs[0]
	a := filepath.Join(project, "a.c")
	seedOrderedTest(t, TriceIDLookUp{100: {Type: "trice", Strg: "same"}, 150: {Type: "trice", Strg: "same"}}, TriceIDLookUpLI{100: {File: "b.c", Line: 1}, 150: {File: "a.c", Line: 1}})
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	// The earlier new site takes 100; unchanged b.c must move from 100 to 150.
	// Its timestamp still agrees with the cached source containing ID 100.
	require.NoError(t, FSys.WriteFile(a, []byte("trice(iD(150), \"same\");\n"), 0o644))
	Srcs = ArrayFlag{b, a}
	// Existing mappings take precedence during allocation; normalization then
	// checks the entire group even when the cached b.c timestamp is unchanged.
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	assert.Equal(t, []TriceID{100}, orderedTestIDs(t, FSys, a, false))
	assert.Equal(t, []TriceID{150}, orderedTestIDs(t, FSys, b, false))
	require.NoError(t, SubCmdIdClean(io.Discard, FSys))
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	assert.Equal(t, []TriceID{100}, orderedTestIDs(t, FSys, a, false))
	assert.Equal(t, []TriceID{150}, orderedTestIDs(t, FSys, b, false))
}

// TestOrderedIDsCacheChecksSourceDespitePreservedTimestamp reproduces a source
// edit with unchanged mtime. A valid cached permutation must still be published.
func TestOrderedIDsCacheChecksSourceDespitePreservedTimestamp(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"main.c": "trice(iD(100), \"same\");\ntrice(iD(150), \"same\");\n"})()
	LIRoot = filepath.Dir(Srcs[0])
	FieldsDir = filepath.Join(LIRoot, "generated")
	TriceCacheEnabled = true
	source := Srcs[0]
	format := TriceFmt{Type: "trice", Strg: "same"}
	seedOrderedTest(t, TriceIDLookUp{100: format, 150: format}, TriceIDLookUpLI{})
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	before, err := FSys.Stat(source)
	require.NoError(t, err)
	require.NoError(t, FSys.WriteFile(source, []byte("trice(iD(150), \"same\");\ntrice(iD(100), \"same\");\n"), 0o644))
	require.NoError(t, FSys.Chtimes(source, before.ModTime(), before.ModTime()))
	require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
	assert.Equal(t, []TriceID{100, 150}, orderedTestIDs(t, FSys, source, false))
	locations := NewLutLI(io.Discard, FSys, LIFnJSON)
	assert.Equal(t, 1, locations[100].Line)
	assert.Equal(t, 2, locations[150].Line)
}

// TestOrderedIDsValidationPrecedesSourceWrites keeps malformed later files
// from leaving the first valid file inserted after deterministic planning fails.
func TestOrderedIDsValidationPrecedesSourceWrites(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"a.c": "trice(\"ok\");\n", "z.c": "TRICE32_1(Id(0), \"bad=%d\");\n"})()
	FieldsDir = filepath.Join(filepath.Dir(Srcs[0]), "generated")
	originals := make(map[string][]byte)
	for _, source := range Srcs {
		data, err := FSys.ReadFile(source)
		require.NoError(t, err)
		originals[source] = bytes.Clone(data)
	}
	require.Error(t, SubCmdIdInsert(io.Discard, FSys))
	for source, original := range originals {
		data, err := FSys.ReadFile(source)
		require.NoError(t, err)
		assert.Equal(t, original, data)
	}
	_, err := FSys.Stat(filepath.Join(FieldsDir, "trice-fields.txt"))
	assert.True(t, os.IsNotExist(err))
}

// TestOrderedIDsMovedAndRemovedSiteReordersRemainingPool checks that movement
// changes ordering while removal neither deletes history nor steals its old ID.
func TestOrderedIDsMovedAndRemovedSiteReordersRemainingPool(t *testing.T) {
	for _, bind := range []bool{false, true} {
		name := "insert"
		if bind {
			name = "bind"
		}
		t.Run(name, func(t *testing.T) {
			b, d := "trice(iD(100), \"same\");\n", "trice(iD(150), \"same\");\n"
			if bind {
				b, d = "trice(\"same\");\n", "trice(\"same\");\n"
			}
			defer prepareBindTest(t, map[string]string{"b.c": b, "d.c": d})()
			bindRandomReader = bytes.NewReader(append(bytes.Repeat([]byte{1}, 8), bytes.Repeat([]byte{2}, 8)...))
			project := filepath.Join(Proj, t.Name())
			LIRoot, FieldsDir = project, filepath.Join(project, "generated")
			bPath, dPath, zPath := filepath.Join(project, "b.c"), filepath.Join(project, "d.c"), filepath.Join(project, "z.c")
			format := TriceFmt{Type: "trice", Strg: "same"}
			seedOrderedTest(t, TriceIDLookUp{100: format, 150: format}, TriceIDLookUpLI{100: {File: "b.c", Line: 1}, 150: {File: "d.c", Line: 1}})
			run := func() {
				t.Helper()
				if bind {
					require.NoError(t, SubCmdIdBind(io.Discard, FSys))
				} else {
					require.NoError(t, SubCmdIdInsert(io.Discard, FSys))
				}
			}
			run()
			// A rename changes path order. A physical Bind owner keeps its file
			// key, but stale primary LI ownership still reserves the former ID.
			require.NoError(t, FSys.Rename(bPath, zPath))
			Srcs = ArrayFlag{dPath, zPath}
			// LI still names b.c. It is outside this scan and must stay reserved.
			run()
			assert.Equal(t, []TriceID{101}, orderedTestIDs(t, FSys, dPath, bind))
			assert.Equal(t, []TriceID{150}, orderedTestIDs(t, FSys, zPath, bind))
			// A removed source is not silently processed through its stale LI entry.
			require.NoError(t, FSys.Remove(zPath))
			Srcs = ArrayFlag{dPath}
			run()
			assert.Equal(t, []TriceID{101}, orderedTestIDs(t, FSys, dPath, bind))
			assert.Contains(t, NewLut(io.Discard, FSys, FnJSON), TriceID(100))
			assert.Contains(t, NewLut(io.Discard, FSys, FnJSON), TriceID(150))
		})
	}
}

// TestOrderedIDsBindLeavesInsertOwnedIdenticalSitesAlone protects the ownership
// boundary even when both kinds of files share one canonical Trice format.
func TestOrderedIDsBindLeavesInsertOwnedIdenticalSitesAlone(t *testing.T) {
	defer prepareBindTest(t, map[string]string{"bound.c": "trice(\"same\");\n", "insert.c": "trice(iD(100), \"same\");\n"})()
	project := filepath.Join(Proj, t.Name())
	LIRoot = project
	insert := filepath.Join(project, "insert.c")
	seedOrderedTest(t, TriceIDLookUp{100: {Type: "trice", Strg: "same"}, 150: {Type: "trice", Strg: "same"}}, TriceIDLookUpLI{100: {File: "insert.c", Line: 1}, 150: {File: "bound.c", Line: 1}})
	before, err := FSys.ReadFile(insert)
	require.NoError(t, err)
	require.NoError(t, SubCmdIdBind(io.Discard, FSys))
	assert.Equal(t, []TriceID{150}, orderedTestIDs(t, FSys, filepath.Join(project, "bound.c"), true))
	after, err := FSys.ReadFile(insert)
	require.NoError(t, err)
	assert.Equal(t, before, after)
	assert.Equal(t, "insert.c", NewLutLI(io.Discard, FSys, LIFnJSON)[100].File)
}

// TestOrderedIDsPolicyAndSchemaBoundaries refuses an unusable pool and keeps
// distinct field schemas separate even when their displayed text looks alike.
func TestOrderedIDsPolicyAndSchemaBoundaries(t *testing.T) {
	defer Setup(t)()
	LIRoot = Proj
	for _, tc := range []struct {
		name    string
		id      TriceID
		reserve bool
	}{
		{"zero has no dictionary identity", 0, false},
		{"outside location is reserved", 150, true},
		{"historical ID violates tag policy", 250, false},
	} {
		t.Run(tc.name, func(t *testing.T) {
			format := TriceFmt{Type: "trice", Strg: "err:same"}
			p := idData{idToTrice: TriceIDLookUp{tc.id: format}, idToLocNew: make(TriceIDLookUpLI), reservedIDs: map[TriceID]bool{tc.id: tc.reserve}, TagList: []TagEntry{{tagName: "ERROR", min: 100, max: 199}}}
			require.Error(t, p.orderIdenticalIDs([]orderedIDSite{{path: filepath.Join(Proj, "x.c"), line: 1, id: tc.id}}))
			assert.Empty(t, p.idToLocNew)
		})
	}
	Min, Max = 100, 199
	p := idData{idToTrice: TriceIDLookUp{100: {Type: "trice", Strg: "value={first:%d}"}, 150: {Type: "trice", Strg: "value={second:%d}"}}, idToLocNew: make(TriceIDLookUpLI)}
	sites := []orderedIDSite{{path: filepath.Join(Proj, "a.c"), line: 1, id: 150}, {path: filepath.Join(Proj, "z.c"), line: 1, id: 100}}
	require.NoError(t, p.orderIdenticalIDs(sites))
	assert.Equal(t, TriceID(150), sites[0].id, "field names are part of identity")
	assert.Equal(t, TriceID(100), sites[1].id)
}
