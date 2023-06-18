package main

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

func TestInsertIDsIntoCleanFilesWithTilJSON(t *testing.T) {
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
			"file": "file0.c",
			"Line": 1
		},
		"1201": {
			"file": "file1.c",
			"Line": 1
		}
	}`
	testSet := []srcFile{
		// fn:       clean:                            insertedIDs:
		{"file0.c", `TRice( "Hi!" );TRice( "Hi!" );`, `TRice( iD( 1200), "Hi!" ); TRice( iD( 1000), "Hi!" );`},
		{"file1.c", `TRice( "Hi!" );TRice( "Hi!" );`, `TRice( iD( 1201), "Hi!" ); TRice( iD( 1001), "Hi!" );`},
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
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-src", ".", "-IDMin", "1000", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		assert.Equal(t, k.insertedIDs, string(actSrc))
	}

}

func TestInsertIDsIntoCleanFilesWithEmptyTilJSON(t *testing.T) {
	testSet := []srcFile{
		{"file0.c", `TRice( "Hi!" );`, `TRice( iD( 1000), "Hi!" );`},
		{"file1.c", `TRice( "Hi!" );`, `TRice( iD( 1001), "Hi!" );`},
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
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-src", ".", "-IDMin", "1000", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		assert.Equal(t, k.insertedIDs, string(actSrc))
	}
}

func TestInsertIDsAndJSON(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	src0 := `TRice( "Hi!" );`
	src1 := `TRice( "Hi!" );`

	// create src files
	sFn0 := "file0.c"
	assert.Nil(t, fSys.WriteFile(sFn0, []byte(src0), 0777))
	sFn1 := "file1.c"
	assert.Nil(t, fSys.WriteFile(sFn1, []byte(src1), 0777))

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-src", ".", "-IDMin", "1000", "-IDMethod", "upward"}))

	// check source files
	expSrc0 := `TRice( iD( 1000), "Hi!" );`
	actSrc0, e := fSys.ReadFile(sFn0)
	assert.Nil(t, e)
	//fmt.Println(string(actSrc))
	assert.Equal(t, expSrc0, string(actSrc0))
	expSrc1 := `TRice( iD( 1001), "Hi!" );`
	actSrc1, e := fSys.ReadFile(sFn1)
	assert.Nil(t, e)
	//fmt.Println(string(actSrc))
	assert.Equal(t, expSrc1, string(actSrc1))

	// check til.json
	expJSON := `{
	"1000": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"1001": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`
	actJSON, e := fSys.ReadFile(jFn)
	assert.Nil(t, e)
	//fmt.Println(string(actJSON))
	assert.Equal(t, expJSON, string(actJSON))

	// check location information
	expLI := `{
	"1000": {
		"file": "file0.c",
		"Line": 1
	},
	"1001": {
		"file": "file1.c",
		"Line": 1
	}
}`
	actLI, e := fSys.ReadFile("li.json")
	assert.Nil(t, e)
	//fmt.Println(string(actLI))
	assert.Equal(t, expLI, string(actLI))
}
