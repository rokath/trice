// SPDX-License-Identifier: MIT

// white-box test
package id

import (
	"bytes"
	"fmt"
	"testing"

	"github.com/rokath/trice/pkg/tst"
	"github.com/spf13/afero"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// sampleLut0 is a test helper function, which returns a lookup map.
func sampleLut0() (lu TriceIDLookUp) {
	lu = make(TriceIDLookUp)
	lu[11] = TriceFmt{Type: "t11", Strg: "s11"}
	lu[12] = TriceFmt{Type: "t12", Strg: "s12"}
	lu[12] = TriceFmt{Type: "t12", Strg: "s12a"}
	return
}

const sampleLutJSON0 = `{
	"11": {
		"Type": "t11",
		"Strg": "s11"
	},
	"12": {
		"Type": "t12",
		"Strg": "s12a"
	}
}`

const sampleLutMap0 = "map[11:{t11 s11} 12:{t12 s12a}]"

// TestLutToJSONFilledByteSlice checks normal initial case.
func TestLutToJSON(t *testing.T) {
	lut := sampleLut0()
	exp := sampleLutJSON0
	b, e := lut.toJSON()
	assert.Nil(t, e)
	act := string(b)
	assert.Equal(t, exp, act)
}

// TestJSONToLutEmptyByteSlice checks empty case.
func TestJSONToLutEmptyByteSlice(t *testing.T) {
	var b []byte
	exp := "map[]"
	lut := make(TriceIDLookUp)
	assert.Nil(t, lut.FromJSON(b))
	act := fmt.Sprint(lut)
	assert.Equal(t, exp, act)
}

// TestJSONToLutMapUpdate checks if a prefilled map is extended and updated.
func TestJSONToLutMapUpdate(t *testing.T) {
	b := []byte(sampleLutJSON0)
	exp := "map[11:{t11 s11} 12:{t12 s12} 33:{t33 s33}]"
	lut := make(TriceIDLookUp)
	assert.Nil(t, lut.FromJSON(b))
	const sampleLutJSON1 = `{
	"33": {
		"Type": "t33",
		"Strg": "s33"
	},
	"12": {
		"Type": "t12",
		"Strg": "s12"
	}
}`
	b1 := []byte(sampleLutJSON1)
	err := lut.FromJSON(b1)
	assert.Nil(t, err)
	act := fmt.Sprint(lut)
	act = tst.NormalizeMapString(act)
	assert.Equal(t, exp, act)
}

// TestJSONToLut checks if JSON to lut works.
func TestJSONToLut(t *testing.T) {
	b := []byte(sampleLutJSON0)
	exp := sampleLutMap0
	lut := make(TriceIDLookUp)
	assert.Nil(t, lut.FromJSON(b))
	act := fmt.Sprint(lut)
	act = tst.NormalizeMapString(act)
	assert.Equal(t, exp, act)
}

const sampleLutJSON1 = `{
	"15": {
		"Type": "t11",
		"Strg": "s11"
	},
	"15": {
		"Type": "t12",
		"Strg": "s12a"
	}
}`

const sampleLutMap1 = "map[15:{t12 s12a}]"

// TestJSONToLut checks if JSON to lut with IDs used twice works.
func TestJSONWithDoubleIDToLut(t *testing.T) {
	b := []byte(sampleLutJSON1)
	exp := sampleLutMap1
	lut := make(TriceIDLookUp)
	assert.Nil(t, lut.FromJSON(b))
	act := fmt.Sprint(lut)
	act = tst.NormalizeMapString(act)
	assert.Equal(t, exp, act)
}

// TestNewLutAndNewLutLIWithEmptyFile verifies the expected behavior.
func TestNewLutAndNewLutLIWithEmptyFile(t *testing.T) {
	defer Setup(t)()

	assert.Empty(t, NewLut(&bytes.Buffer{}, FSys, "emptyFile"))
	assert.Empty(t, NewLutLI(&bytes.Buffer{}, FSys, "emptyFile"))
}

// TestLocationInfoFromFileWhileLogging verifies the expected behavior.
func TestLocationInfoFromFileWhileLogging(t *testing.T) {
	defer Setup(t)()

	oldLogging := Logging
	Logging = true
	t.Cleanup(func() { Logging = oldLogging })

	li := make(TriceIDLookUpLI)
	assert.NoError(t, li.fromFile(FSys, Proj+"missing-li.json"))
	assert.Empty(t, li)
}

// TestAddFmtCount verifies the expected behavior.
func TestAddFmtCount(t *testing.T) {
	defer Setup(t)()

	ilu := TriceIDLookUp{
		10: {Type: "TRICE", Strg: "value=%d"},
		11: {Type: "TRICE", Strg: "plain"},
		12: {Type: "TRICE_B", Strg: "buf=%d"},
		13: {Type: "TRICE_S", Strg: SAliasStrgPrefix + `false, "msg=%d"` + SAliasStrgSuffix},
		14: {Type: "TRICE16_99_1", Strg: "value=%d"},
		15: {Type: "TRICE_F", Strg: "rpc:%d"},
	}

	var out bytes.Buffer
	ilu.AddFmtCount(&out)

	assert.Equal(t, "TRICE_1", ilu[10].Type)
	assert.Equal(t, "TRICE", ilu[11].Type)
	assert.Equal(t, "TRICE_B", ilu[12].Type)
	assert.Equal(t, "TRICE_S", ilu[13].Type)
	assert.Equal(t, "TRICE16_99_1", ilu[14].Type)
	assert.Equal(t, "TRICE_F", ilu[15].Type)
	assert.Contains(t, out.String(), "Expected format specifier count is 0 but got 1")
	assert.Contains(t, out.String(), "Unexpected Trice type - please check")
}

// TestNewIDUnknownMethod verifies the expected behavior.
func TestNewIDUnknownMethod(t *testing.T) {
	defer Setup(t)()

	ilu := TriceIDLookUp{}
	assert.Equal(t, TriceID(0), ilu.newID(&bytes.Buffer{}, 10, 20, "mystery"))
}

// TestLutLocationInfoFromFile verifies the expected behavior.
func TestLutLocationInfoFromFile(t *testing.T) {
	defer Setup(t)()

	const liJSON = `{
	"17": {
		"File": "demo.c",
		"Line": 42
	}
}`
	require.NoError(t, FSys.WriteFile(LIFnJSON, []byte(liJSON), 0o777))

	li := NewLutLI(&bytes.Buffer{}, FSys, LIFnJSON)
	require.Len(t, li, 1)
	assert.Equal(t, "demo.c", li[17].File)
	assert.Equal(t, 42, li[17].Line)
}

// TestLutFileTransferMemMap verifies the expected behavior.
func TestLutFileTransferMemMap(t *testing.T) {
	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	wr := sampleLut0()

	require.NoError(t, wr.toFile(fs.Fs, "lut.json"))
	rd := make(TriceIDLookUp)
	require.NoError(t, rd.fromFile(fs, "lut.json"))
	assert.Equal(t, tst.NormalizeMapString(sampleLutMap0), tst.NormalizeMapString(fmt.Sprint(rd)))
}

// TestResolveTriceAlias verifies the expected behavior.
func TestResolveTriceAlias(t *testing.T) {
	defer Setup(t)()

	TriceAliases = ArrayFlag{"LogX", "MixS"}
	TriceSAliases = ArrayFlag{"PrintS", "MixS"}

	alias := TriceFmt{Type: "LogX"}
	resolveTriceAlias(&alias)
	assert.Equal(t, "trice", alias.Type)
	assert.Equal(t, "LogX", alias.Alias)

	sAlias := TriceFmt{Type: "PrintS"}
	resolveTriceAlias(&sAlias)
	assert.Equal(t, "triceS", sAlias.Type)
	assert.Equal(t, "PrintS", sAlias.Alias)

	shared := TriceFmt{Type: "MixS"}
	resolveTriceAlias(&shared)
	assert.Equal(t, "triceS", shared.Type)
	assert.Equal(t, "MixS", shared.Alias)
}

// TestIDSpaceHelpers verifies the expected behavior.
func TestIDSpaceHelpers(t *testing.T) {
	defer Setup(t)()

	IDData.TagList = []TagEntry{
		{tagName: "", min: 10, max: 12, iDSpace: []TriceID{10, 11, 12}},
		{tagName: "err", min: 100, max: 102, iDSpace: []TriceID{100, 101, 102}},
	}
	assert.True(t, IDData.IDIsPartOfIDSpace(100))
	assert.False(t, IDData.IDIsPartOfIDSpace(999))

	IDData.TagList = nil
	IDRange = ArrayFlag{"e:100,102"}
	Min = 10
	Max = 12
	require.NoError(t, EvaluateIDRangeStrings())
	require.Len(t, IDData.TagList, 1)

	IDData.TagList = nil
	IDRange = ArrayFlag{"e:100,102", "err:101,103"}
	assert.Error(t, EvaluateIDRangeStrings())
}
