package id_test

import (
	"bytes"
	"fmt"
	"io"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

// TestInsertIDsIntoTilJSONFromFileWithLi ...
//
// IDs 1200 & 1201 are exist, so they are expected to go into til.json.
func TestInsertIDsIntoTilJSONFromFileWithLi(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	fn0 := t.Name() + "file0.c"
	fn1 := t.Name() + "file1.c"

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

	// create src files
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// re-create li.json
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(liJSON), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", ".", "-til", id.FnJSON, "-li", id.LIFnJSON}))

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
	result := expTil == string(actTil)
	if !result {
		fmt.Println("ACTUAL TIL:", string(actTil))
		fmt.Println("EXPECT TIL:", expTil)
	}
	assert.True(t, result)
}

// TestInsertIDsForNewTrice2WithLI ...
func TestInsertIDsForNewTrice2WithLI(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	fn0 := t.Name() + "file0.c"
	fn1 := t.Name() + "file1.c"

	testSet := []srcFile{
		// fn: in:                                                 expected:
		{fn0, `trice( "new" ); trice( "Lo!" );` /*             */, `trice( iD(18), "new" ); trice( iD(17), "Lo!" );`},
		{fn1, `trice( iD(1200), "Hi!" ); trice( iD(19), "old" );`, `trice( iD(1200), "Hi!" ); trice( iD(19), "old" );`},
	}
	// create src files
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// location information
	liJSON := `{
	"1200": {
		"File": "` + fn1 + `",
		"Line": 1
	},
	"19": {
		"File": "` + fn1 + `",
		"Line": 1
	}
}`
	// re-create li.json
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(liJSON), 0777))

	// When fn1 is processed first, how to know that ID 19 is used already without location information? -> Create first if empty

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src=.", "-IDMin=10", "-IDMax=19", "-IDMethod=downward", "-til", id.FnJSON, "-li", id.LIFnJSON}))

	// check source files
	for i, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		actS := string(actSrc)
		r := k.insertedIDs == actS
		if !r {
			fmt.Println(i, " ACTUAL SRC:", actS)
			fmt.Println(i, " EXPECT SRC:", k.insertedIDs)
		}
		assert.True(t, r)
	}

	// check til.json
	expTil := `{
	"1200": {
		"Type": "trice",
		"Strg": "Hi!"
	},
	"17": {
		"Type": "trice",
		"Strg": "Lo!"
	},
	"18": {
		"Type": "trice",
		"Strg": "new"
	},
	"19": {
		"Type": "trice",
		"Strg": "old"
	}
}`
	actTil, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	result := expTil == string(actTil)
	if !result {
		fmt.Println("ACTUAL TIL:", string(actTil))
		fmt.Println("EXPECT TIL:", expTil)
	}
	assert.True(t, result)
}

// TestInsertIDsForNewTrice2WithLI ...
func TestInsertIDsForNewTrice2WithoutLI(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	fn0 := t.Name() + "file0.c"
	fn1 := t.Name() + "file1.c"

	testSet := []srcFile{
		// fn: in:                                                 expected:
		{fn0, `trice( "new" ); trice( "Lo!" );` /*             */, `trice( iD(18), "new" ); trice( iD(17), "Lo!" );`},
		{fn1, `trice( iD(1200), "Hi!" ); trice( iD(19), "old" );`, `trice( iD(1200), "Hi!" ); trice( iD(19), "old" );`},
	}
	// create src files
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// When fn0 is processed first, how to know that ID 19 is used already in fn0 without location information? -> Create first if empty

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "clean", "-v", "-til", id.FnJSON, "-li", id.LIFnJSON}))

	// expected location information
	expLIJSON := `{
	"1200": {
		"File": "` + fn1 + `",
		"Line": 1
	},
	"19": {
		"File": "` + fn1 + `",
		"Line": 1
	}
}`
	actLIJSONs, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	actLIJSON := string(actLIJSONs)
	assert.Equal(t, actLIJSON, expLIJSON)

	expTil0 := `{
	"1200": {
		"Type": "trice",
		"Strg": "Hi!"
	},
	"19": {
		"Type": "trice",
		"Strg": "old"
	}
}`
	actTil0, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expTil0, string(actTil0))

	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-v", "-IDMin=10", "-IDMax=19", "-IDMethod=downward", "-til", id.FnJSON, "-li", id.LIFnJSON}))

	// Do noit apply "-src=." here! It would set Srcs to be {".", "."} what results in a double parsing of the root folder causing the test to fail.
	// This is, because we called "trice clean" before in this test.

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		actS := string(actSrc)
		assert.Equal(t, k.insertedIDs, actS)
	}

	// check til.json
	expTil := `{
	"1200": {
		"Type": "trice",
		"Strg": "Hi!"
	},
	"17": {
		"Type": "trice",
		"Strg": "Lo!"
	},
	"18": {
		"Type": "trice",
		"Strg": "new"
	},
	"19": {
		"Type": "trice",
		"Strg": "old"
	}
}`
	actTil, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	result := expTil == string(actTil)
	if !result {
		fmt.Println("ACTUAL TIL:", string(actTil))
		fmt.Println("EXPECT TIL:", expTil)
	}
	assert.True(t, result)
}

// TestInsertIDsForNewTrice ...
//
// IDs 1200 & 1201 are exist, so they are expected to go into til.json.
func TestInsertIDsForNewTrice1(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	fn0 := t.Name() + "file0.c"

	// location information
	liJSON := `{
}`
	testSet := []srcFile{
		// fn: in:                                                 expected:
		{fn0, `TRice( iD(1200), "Hi!" ); trice( "new" );`, `TRice( iD(1200), "Hi!" ); trice( iD(1201), "new" );`},
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// re-create li.json
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(liJSON), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", ".", "-IDMin=1200", "-IDMax=1201", "-IDMethod=upward", "-til", id.FnJSON, "-li", id.LIFnJSON}))

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		actS := string(actSrc)
		r := actS == k.insertedIDs
		if !r {
			fmt.Println(" ACTUAL SRC:", actS)
			fmt.Println(" EXPECT SRC:", k.insertedIDs)
		}
		assert.True(t, r)
	}

	// check til.json
	expTil := `{
	"1200": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"1201": {
		"Type": "trice",
		"Strg": "new"
	}
}`
	actTil, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	result := expTil == string(actTil)
	if !result {
		fmt.Println("ACTUAL TIL:", string(actTil))
		fmt.Println("EXPECT TIL:", expTil)
	}
	assert.True(t, result)
}
