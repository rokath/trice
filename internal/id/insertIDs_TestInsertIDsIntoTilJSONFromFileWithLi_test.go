package id_test

// The tests in this file cannot run concurrently!

import (
	"bytes"
	"io"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

// TestInsertIDsIntoTilJSONFromFileWithLi ...
//
// IDs 1200 & 1201 are exist, so they are expected to go into til.json.
func ___TestInsertIDsIntoTilJSONFromFileWithLi(t *testing.T) {

	fn0 := t.Name() + "file0.c"
	fn1 := t.Name() + "file1.c"
	tilFn := t.Name() + "til.json"
	liFn := t.Name() + "li.json"

	// trice ID list
	tilJSON := `{
}`
	// location information
	liJSON := `{
		"1200": {
			"File": "` + fn0 + `",
			"Line": 1
		},
		"1201": {
			"File": "` + fn1 + `",
			"Line": 1
		}
	}`
	testSet := []srcFile{
		// fn:       existing IDs:                unchanged IDs:
		{fn0, `TRice( iD(1200), "Hi!" );`, `TRice( iD(1200), "Hi!" );`},
		{fn1, `TRice( iD(1201), "Lo!" );`, `TRice( iD(1201), "Lo!" );`},
	}
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// create til.json
	assert.Nil(t, fSys.WriteFile(tilFn, []byte(tilJSON), 0777))

	// create li.json
	assert.Nil(t, fSys.WriteFile(liFn, []byte(liJSON), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", ".", "-til", tilFn, "-li", liFn}))

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		assert.Equal(t, k.insertedIDs, string(actSrc))
	}

	// check til.json
	expTil := `{
	"1200": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"1201": {
		"Type": "TRice",
		"Strg": "Lo!"
	}
}`
	actTil, e := fSys.ReadFile(tilFn)
	assert.Nil(t, e)
	assert.Equal(t, expTil, string(actTil))

}
