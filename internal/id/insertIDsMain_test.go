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

type srcFile struct {
	fn          string
	clean       string
	insertedIDs string
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

// TestInsertIDsIntoCleanFilesWithTilJSON ...
//
// IDs 1200 & 1201 are exist, so they are expected to be restored on the old position.
// The IDs 1000 & 1001 are newly assigned.
func _TestInsertIDsIntoCleanFilesWithTilJSON(t *testing.T) {
	// trice ID list
	tilJSON := `{
	"1200": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"1201": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`

	// location information
	liJSON := `{
		"1200": {
			"File": "file0.c",
			"Line": 1
		},
		"1201": {
			"File": "file1.c",
			"Line": 1
		}
	}`
	testSet := []srcFile{
		// fn:       clean:                            insertedIDs:
		{"file0.c", `TRice( "Hi!" ); TRice( "Hi!" );`, `TRice( iD(1200), "Hi!" ); TRice( iD(1001), "Hi!" );`},
		{"file1.c", `TRice( "Hi!" ); TRice( "Hi!" );`, `TRice( iD(1201), "Hi!" ); TRice( iD(1000), "Hi!" );`},
	}
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// create til.json
	assert.Nil(t, fSys.WriteFile("til.json", []byte(tilJSON), 0777))

	// create li.json
	assert.Nil(t, fSys.WriteFile("li.json", []byte(liJSON), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", ".", "-IDMin", "1000", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		assert.Equal(t, k.insertedIDs, string(actSrc))
	}

}

func _TestInsertIDsIntoCleanFilesWithEmptyTilJSON(t *testing.T) {
	testSet := []srcFile{
		{"file0.c", `TRice( "Hi!" );`, `TRice( iD(1001), "Hi!" );`},
		{"file1.c", `TRice( "Hi!" );`, `TRice( iD(1000), "Hi!" );`},
	}
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", ".", "-IDMin", "1000", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		assert.Equal(t, k.insertedIDs, string(actSrc))
	}
}
