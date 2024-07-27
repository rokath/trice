// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// white-box test
package id_test

import (
	"bytes"
	"io"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func TestInsertIDsAndJSONDownward(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	// create src files
	src0 := `
	TRice( "Lo!" );
	TRice( "Hi!" );
	`
	fn0 := t.Name() + "file0.c"
	assert.Nil(t, fSys.WriteFile(fn0, []byte(src0), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", ".", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "10", "-IDMax", "20", "-IDMethod", "downward"}))

	// check source files
	expSrc0 := `
	TRice( iD(20), "Lo!" );
	TRice( iD(19), "Hi!" );
	`
	actSrc0, e := fSys.ReadFile(fn0)
	assert.Nil(t, e)
	assert.Equal(t, expSrc0, string(actSrc0))

	// check til.json
	expJSON := `{
	"19": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"20": {
		"Type": "TRice",
		"Strg": "Lo!"
	}
}`
	actJSON, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expJSON, string(actJSON))

	// check location information
	expLI := `{
	"19": {
		"File": "` + fn0 + `",
		"Line": 3
	},
	"20": {
		"File": "` + fn0 + `",
		"Line": 2
	}
}`
	actLI, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))
}

func TestInsertIDsAndJSONUpward(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	// create src files
	src0 := `
	TRice( "Hi!" );
	TRice( "Hi!" );
	`
	fn0 := t.Name() + "file0.c"
	assert.Nil(t, fSys.WriteFile(fn0, []byte(src0), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", "./", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "10", "-IDMax", "20", "-IDMethod", "upward"}))

	// check source files
	expSrc0 := `
	TRice( iD(10), "Hi!" );
	TRice( iD(11), "Hi!" );
	`
	actSrc0, e := fSys.ReadFile(fn0)
	assert.Nil(t, e)
	assert.Equal(t, expSrc0, string(actSrc0))

	// check til.json
	expJSON := `{
	"10": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"11": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`
	actJSON, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expJSON, string(actJSON))

	// check location information
	expLI := `{
	"10": {
		"File": "` + fn0 + `",
		"Line": 2
	},
	"11": {
		"File": "` + fn0 + `",
		"Line": 3
	}
}`
	actLI, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))
}

func TestInsertIDsIntoTilJSONFromFileWithEmptyLi(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	fn0 := t.Name() + "file0.c"
	fn1 := t.Name() + "file1.c"

	testSet := []srcFile{
		// fn:       existing IDs:                unchanged IDs:
		{fn0, `TRice( iD(1200), "Hi!" );`, `TRice( iD(1200), "Hi!" );`},
		{fn1, `TRice( iD(1201), "Lo!" );`, `TRice( iD(1201), "Lo!" );`},
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "i", "-src", ".", "-til", id.FnJSON, "-li", id.LIFnJSON}))

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
	actTil, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expTil, string(actTil))
}

func TestInsertLineDuplicates(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	// create src files
	src0 := `
	TRice( iD(10), "Hi!" );
	TRice( iD(10), "Hi!" );
	`
	fn0 := t.Name() + "file0.c"
	assert.Nil(t, fSys.WriteFile(fn0, []byte(src0), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", "./", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "10", "-IDMax", "20", "-IDMethod", "upward"}))

	// check source files
	expSrc0 := `
	TRice( iD(10), "Hi!" );
	TRice( iD(11), "Hi!" );
	`
	actSrc0, e := fSys.ReadFile(fn0)
	assert.Nil(t, e)
	assert.Equal(t, expSrc0, string(actSrc0))

	// check til.json
	expJSON := `{
	"10": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"11": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`
	actJSON, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expJSON, string(actJSON))

	// check location information
	expLI := `{
	"10": {
		"File": "` + fn0 + `",
		"Line": 2
	},
	"11": {
		"File": "` + fn0 + `",
		"Line": 3
	}
}`
	actLI, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))
}

func TestChangeIDAfterStringModification(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	// create src files
	src0 := `
	TRice( iD(10), "Hi!" );
	TRice( iD(10), "Hi!" );
	`
	fn0 := t.Name() + "file0.c"
	assert.Nil(t, fSys.WriteFile(fn0, []byte(src0), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", "./", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "10", "-IDMax", "20", "-IDMethod", "upward"}))

	// check source files
	expSrc0 := `
	TRice( iD(10), "Hi!" );
	TRice( iD(11), "Hi!" );
	`
	actSrc0, e := fSys.ReadFile(fn0)
	assert.Nil(t, e)
	assert.Equal(t, expSrc0, string(actSrc0))

	// check til.json
	expJSON := `{
	"10": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"11": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`
	actJSON, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expJSON, string(actJSON))

	// check location information
	expLI := `{
	"10": {
		"File": "` + fn0 + `",
		"Line": 2
	},
	"11": {
		"File": "` + fn0 + `",
		"Line": 3
	}
}`
	actLI, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))

	// part 2

	id.SetupTest(t, fSys)()

	assert.Nil(t, fSys.WriteFile(id.FnJSON, []byte(expJSON), 0777))
	// check til
	actJSON, e = fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expJSON, string(actJSON))

	// check li
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(expLI), 0777))
	actLI, e = fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))

	// modify src file
	src0 = `
	TRice( iD(10), "Hi!" );
	TRice( iD(11), "Lo!" );
	`
	assert.Equal(t, fn0, t.Name()+"file0.c")
	assert.Nil(t, fSys.WriteFile(fn0, []byte(src0), 0777))

	// check src
	actSrc0, e = fSys.ReadFile(fn0)
	assert.Nil(t, e)
	assert.Equal(t, src0, string(actSrc0))

	// action 2
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", "./", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "10", "-IDMax", "20", "-IDMethod", "upward"}))

	// check source files
	expSrc0 = `
	TRice( iD(10), "Hi!" );
	TRice( iD(12), "Lo!" );
	`
	actSrc0, e = fSys.ReadFile(fn0)
	assert.Nil(t, e)
	assert.Equal(t, expSrc0, string(actSrc0))

	// check til.json
	expJSON = `{
	"10": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"11": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"12": {
		"Type": "TRice",
		"Strg": "Lo!"
	}
}`
	actJSON, e = fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expJSON, string(actJSON))

	// check location information
	expLI = `{
	"10": {
		"File": "` + fn0 + `",
		"Line": 2
	},
	"12": {
		"File": "` + fn0 + `",
		"Line": 3
	}
}`
	actLI, e = fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))
}

func TestChangeIDAfterStringModification2(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	// create src file
	fn0 := t.Name() + "file0.c"
	src0 := `
	TRice( iD(10), "Lo!" );
	`
	assert.Nil(t, fSys.WriteFile(fn0, []byte(src0), 0777))
	// check src
	actSrc0, e := fSys.ReadFile(fn0)
	assert.Nil(t, e)
	assert.Equal(t, src0, string(actSrc0))

	// create til.json
	tilJSON := `{
	"10": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`

	assert.Nil(t, fSys.WriteFile(id.FnJSON, []byte(tilJSON), 0777))
	// check til
	actJSON, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, tilJSON, string(actJSON))

	LI := `{
	"10": {
		"File": "` + fn0 + `",
		"Line": 2
	}
}`
	// check li
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(LI), 0777))
	actLI, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, LI, string(actLI))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", "./", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "10", "-IDMax", "20", "-IDMethod", "upward"}))

	// check source files
	expSrc0 := `
	TRice( iD(11), "Lo!" );
	`
	actSrc0, e = fSys.ReadFile(fn0)
	assert.Nil(t, e)
	assert.Equal(t, expSrc0, string(actSrc0))

	// check til.json
	expJSON := `{
	"10": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"11": {
		"Type": "TRice",
		"Strg": "Lo!"
	}
}`
	actJSON, e = fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expJSON, string(actJSON))

	// check location information
	expLI := `{
	"11": {
		"File": "` + fn0 + `",
		"Line": 2
	}
}`
	actLI, e = fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))
}

// TestAddIDToTilJSON tests to not change src file if not needed.
func TestAddIDToTilJSON(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	// create src file
	fn0 := t.Name() + "file0.c"
	src0 := `
	TRice( iD(88), "Hi!" );
	`
	assert.Nil(t, fSys.WriteFile(fn0, []byte(src0), 0777))
	// check src
	actSrc0, e := fSys.ReadFile(fn0)
	assert.Nil(t, e)
	assert.Equal(t, src0, string(actSrc0))

	// create til.json
	tilJSON := `{
	"10": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`

	assert.Nil(t, fSys.WriteFile(id.FnJSON, []byte(tilJSON), 0777))
	// check til
	actJSON, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, tilJSON, string(actJSON))

	LI := `{
	"10": {
		"File": "` + fn0 + `",
		"Line": 2
	}
}`
	// check li
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(LI), 0777))
	actLI, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, LI, string(actLI))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", "./", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "10", "-IDMax", "20", "-IDMethod", "upward"}))

	// check source files
	expSrc0 := `
	TRice( iD(88), "Hi!" );
	`
	actSrc0, e = fSys.ReadFile(fn0)
	assert.Nil(t, e)
	assert.Equal(t, expSrc0, string(actSrc0))

	// check til.json
	expJSON := `{
	"10": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"88": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`
	actJSON, e = fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expJSON, string(actJSON))

	// check location information
	expLI := `{
	"88": {
		"File": "` + fn0 + `",
		"Line": 2
	}
}`
	actLI, e = fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))
}

// TestGenerateNewIDIfUsedToTilJSON tests to change src file if ID used differently.
func TestGenerateNewIDIfUsedToTilJSON(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	// create src file
	fn0 := t.Name() + "file0.c"
	src0 := `
	TRice( iD(88), "Hi!" );
	`
	assert.Nil(t, fSys.WriteFile(fn0, []byte(src0), 0777))
	// check src
	actSrc0, e := fSys.ReadFile(fn0)
	assert.Nil(t, e)
	assert.Equal(t, src0, string(actSrc0))

	// create til.json
	tilJSON := `{
	"10": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"88": {
		"Type": "TRice",
		"Strg": "XX!"
	}
}`

	assert.Nil(t, fSys.WriteFile(id.FnJSON, []byte(tilJSON), 0777))
	// check til
	actJSON, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, tilJSON, string(actJSON))

	LI := `{
	"10": {
		"File": "` + fn0 + `",
		"Line": 2
	}
}`
	// check li
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(LI), 0777))
	actLI, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, LI, string(actLI))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", "./", "-til", id.FnJSON, "-li", id.LIFnJSON, "-IDMin", "10", "-IDMax", "20", "-IDMethod", "upward"}))

	// check source files
	expSrc0 := `
	TRice( iD(10), "Hi!" );
	`
	actSrc0, e = fSys.ReadFile(fn0)
	assert.Nil(t, e)
	assert.Equal(t, expSrc0, string(actSrc0))

	// check til.json
	expJSON := `{
	"10": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"88": {
		"Type": "TRice",
		"Strg": "XX!"
	}
}`
	actJSON, e = fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expJSON, string(actJSON))

	// check location information
	expLI := `{
	"10": {
		"File": "` + fn0 + `",
		"Line": 2
	}
}`
	actLI, e = fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))
}
