// SPDX-License-Identifier: MIT

// white-box test
package id

import (
	"fmt"
	"path/filepath"
	"testing"

	"github.com/rokath/trice/pkg/tst"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestLutFileTransfer checks lut file transfer.
func TestLutFileTransfer(t *testing.T) { // Anti-Virus issue
	wr := sampleLut0()
	exp := sampleLutMap0

	fn := "TestWriteLutToFile.JSON"
	assert.Nil(t, wr.toFile(FSys, fn))
	rd := make(TriceIDLookUp)
	assert.Nil(t, rd.fromFile(FSys, fn))
	act := fmt.Sprint(rd)
	act = tst.NormalizeMapString(act)
	assert.Equal(t, exp, act)
}

// TestBasePath verifies the expected behavior.
func TestBasePath(t *testing.T) { // Anti-Virus issue
	baseFs := &afero.MemMapFs{}
	baseFs.MkdirAll("/base/path/tmp", 0777)
	bp := afero.NewBasePathFs(baseFs, "/base/path")

	if _, err := bp.Create("/tmp/foo"); err != nil {
		t.Errorf("Failed to set real path")
	}

	if fh, err := bp.Create("../tmp/bar"); err == nil {
		t.Errorf("succeeded in creating %s ...", fh.Name())
	}
}

// TestAddReadsSourceWithoutChangingIt exercises the current add entry point,
// retaining historical metadata while ignoring unassigned IDs and non-source files.
func TestAddReadsSourceWithoutChangingIt(t *testing.T) {
	defer Setup(t)()

	sourceDir := filepath.Join(Proj, t.Name())
	sourceFile := filepath.Join(sourceDir, "nested", "demo.c")
	require.NoError(t, FSys.MkdirAll(filepath.Dir(sourceFile), 0o700))
	t.Cleanup(func() { _ = FSys.RemoveAll(sourceDir) })
	Srcs = ArrayFlag{sourceDir}
	LIRoot = sourceDir

	// All four ID spellings must reach the same dictionary/location workflow.
	const source = `trice8(id(17), "msg:first=%u", 7);
Trice16(Id(18), "msg:second=%u", 8);
TRice32(ID(19), "msg:third=%u", 9);
trice(iD(20), "msg:fourth=%u", 10);
trice(iD(0), "msg:unassigned=%u", 0);
trice("msg:without ID");
`
	require.NoError(t, FSys.WriteFile(sourceFile, []byte(source), 0o600))
	require.NoError(t, FSys.WriteFile(filepath.Join(sourceDir, "notes.txt"), []byte(`trice(iD(999), "not C");`), 0o600))

	// Metadata for earlier builds must survive this partial source scan.
	historical := TriceFmt{Type: "TRICE32_1", Strg: "msg:historical=%u"}
	require.NoError(t, (TriceIDLookUp{700: historical}).toFile(FSys, FnJSON))
	require.NoError(t, (TriceIDLookUpLI{700: {File: "old.c", Line: 9}}).toFile(FSys, LIFnJSON))
	require.NoError(t, SubCmdIdAdd(W, FSys))

	actualSource, err := FSys.ReadFile(sourceFile)
	require.NoError(t, err)
	assert.Equal(t, source, string(actualSource))

	lut := make(TriceIDLookUp)
	require.NoError(t, lut.fromFile(FSys, FnJSON))
	assert.Equal(t, TriceIDLookUp{
		17:  {Type: "trice8", Strg: "msg:first=%u"},
		18:  {Type: "Trice16", Strg: "msg:second=%u"},
		19:  {Type: "TRice32", Strg: "msg:third=%u"},
		20:  {Type: "trice", Strg: "msg:fourth=%u"},
		700: historical,
	}, lut)
	locations := make(TriceIDLookUpLI)
	require.NoError(t, locations.fromFile(FSys, LIFnJSON))
	assert.Equal(t, TriceIDLookUpLI{
		17:  {File: "nested/demo.c", Line: 1},
		18:  {File: "nested/demo.c", Line: 2},
		19:  {File: "nested/demo.c", Line: 3},
		20:  {File: "nested/demo.c", Line: 4},
		700: {File: "old.c", Line: 9},
	}, locations)
}
