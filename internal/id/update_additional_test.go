// SPDX-License-Identifier: MIT

package id

import (
	"bytes"
	"os"
	"path/filepath"
	"testing"

	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestTriceIDParseVariants verifies all supported ID spellings are parsed into their respective idType.
func TestTriceIDParseVariants(t *testing.T) {
	tests := []struct {
		input  string
		id     TriceID
		kind   idType
		prefix string
	}{
		{`TRICE(id(17), "x")`, 17, idTypeS0, "id(17)"},
		{`TRICE(Id(18), "x")`, 18, idTypeS2, "Id(18)"},
		{`TRICE(ID(19), "x")`, 19, idTypeS4, "ID(19)"},
		{`TRICE(iD(20), "x")`, 20, idTypeS8, "iD(20)"},
	}
	for _, tt := range tests {
		nbID, id, found := triceIDParse(tt.input)
		assert.Equal(t, tt.prefix, nbID)
		assert.Equal(t, tt.id, id)
		assert.Equal(t, tt.kind, found)
	}
}

// TestTriceFmtParseAndTriceParse verifies format extraction and combined parsing.
func TestTriceFmtParseAndTriceParse(t *testing.T) {
	tf, ok := triceFmtParse(`TRICE16_2( Id(123), "value=%d" )`)
	require.True(t, ok)
	assert.Equal(t, "TRICE16_2", tf.Type)
	assert.Equal(t, "value=%d", tf.Strg)

	nbID, id, parsed, found := triceParse(`TRICE16_2( Id(123), "value=%d" )`)
	assert.Equal(t, "Id(123)", nbID)
	assert.Equal(t, TriceID(123), id)
	assert.Equal(t, tf, parsed)
	assert.EqualValues(t, idTypeS2, found)
}

// TestReadFileAndRefreshIDs verifies source scanning and conflicting ID handling.
func TestReadFileAndRefreshIDs(t *testing.T) {
	defer Setup(t)()

	fs := &afero.Afero{Fs: afero.NewMemMapFs()}
	require.NoError(t, fs.WriteFile("demo.c", []byte("TRICE8( Id(123), \"value=%d\" );\n"), 0o644))
	require.NoError(t, fs.WriteFile("note.txt", []byte("ignore"), 0o644))

	fi, err := fs.Stat("demo.c")
	require.NoError(t, err)
	text, err := readFile(ioDiscard{}, fs, "demo.c", fi, nil)
	require.NoError(t, err)
	assert.Contains(t, text, "TRICE8")

	txtInfo, err := fs.Stat("note.txt")
	require.NoError(t, err)
	text, err = readFile(ioDiscard{}, fs, "note.txt", txtInfo, nil)
	require.NoError(t, err)
	assert.Equal(t, "", text)

	ilu := TriceIDLookUp{
		123: {Type: "TRICE8_1", Strg: "other=%d"},
	}
	flu := triceFmtLookUp{}
	lim := TriceIDLookUpLI{}
	var out bytes.Buffer
	refreshIDs(&out, "demo.c", "break; case __LINE__: TRICE8_1( Id(123), \"value=%d\\n\", -1 );\nbreak; case __LINE__: TRICE8_1( Id(124), \"next=%d\\n\", -2 );\n", ilu, flu, lim)

	assert.Contains(t, out.String(), "already used differently")
	_, exists := lim[123]
	assert.False(t, exists)
	_, exists = ilu[124]
	assert.False(t, exists)
	_, exists = lim[124]
	assert.False(t, exists)
}

// TestRefreshListCollectsSourceFiles verifies recursive source refresh ignores non-source files.
func TestRefreshListCollectsSourceFiles(t *testing.T) {
	dir := t.TempDir()
	srcDir := filepath.Join(dir, "src")
	require.NoError(t, os.MkdirAll(srcDir, 0o755))
	require.NoError(t, os.WriteFile(filepath.Join(srcDir, "demo.c"), []byte("break; case __LINE__: TRICE8_1( Id(321), \"demo=%d\\n\", -7 );"), 0o644))
	require.NoError(t, os.WriteFile(filepath.Join(srcDir, "readme.txt"), []byte("skip"), 0o644))

	oldVerbose := Verbose
	oldFnJSON := FnJSON
	Verbose = false
	FnJSON = "til.json"
	t.Cleanup(func() {
		Verbose = oldVerbose
		FnJSON = oldFnJSON
	})

	ilu := TriceIDLookUp{}
	flu := triceFmtLookUp{}
	lim := TriceIDLookUpLI{}
	refreshList(ioDiscard{}, &afero.Afero{Fs: afero.NewOsFs()}, srcDir, ilu, flu, lim)

	assert.Equal(t, TriceFmt{Type: "TRICE8_1", Strg: "demo=%d\\n"}, ilu[321])
	assert.Equal(t, "demo.c", lim[321].File)
}

type ioDiscard struct{}

func (ioDiscard) Write(p []byte) (int, error) { return len(p), nil }
