package id_test

// The tests in this file cannot run concurrently!

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

type srcFile struct {
	fn          string
	clean       string
	insertedIDs string
}

// TestInsertIDsFromSingleFileIntoNonEmptyJSONWithDoubledIDinsideLi ...
//
// IDs 1200 & 1201 are exist, so they are expected to be restored on the old position.
// The IDs 100 & 101 are newly assigned.
func TestInsertIDsFromSingleFileIntoNonEmptyJSONWithNoIDinsideLi(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

	testSet := []srcFile{
		// fn: start:                                                insertedIDs:
		{fn0, `TRice( iD(1444), "Hi!" ); TRice( iD(122), "Lo!" );`, `TRice( iD(1444), "Hi!" ); TRice( iD(122), "Lo!" );`},
	}

	// create src file
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// trice ID list
	tilJSON := `{
	"222": {
		"Type": "TRice",
		"Strg": "XXX"
	},
	"223": {
		"Type": "TRice",
		"Strg": "YYY"
	}
}`

	// location information
	liJSON := `{
	"222": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"223": {
		"File": "` + fn1 + `",
		"Line": 1
	}
}`

	// re-create til.json
	assert.Nil(t, fSys.WriteFile(id.FnJSON, []byte(tilJSON), 0777))

	// re-create li.json
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(liJSON), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-v", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", ".", "-IDMin", "70", "-IDMax", "80", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		result := k.insertedIDs == string(actSrc)
		if !result {
			fmt.Println("ACTUAL SRC:", string(actSrc))
		}
		assert.True(t, result)
	}

	// location information
	expLiJSON := `{
	"122": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"1444": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"222": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"223": {
		"File": "` + fn1 + `",
		"Line": 1
	}
}`
	actLiJSON, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	resultLI := expLiJSON == string(actLiJSON)
	if !resultLI {
		fmt.Println("ACTUAL LI:", string(actLiJSON))
		fmt.Println("EXPECT LI:", string(expLiJSON))
	}
	assert.True(t, resultLI)

	// trice ID list
	expTilJSON := `{
	"122": {
		"Type": "TRice",
		"Strg": "Lo!"
	},
	"1444": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"222": {
		"Type": "TRice",
		"Strg": "XXX"
	},
	"223": {
		"Type": "TRice",
		"Strg": "YYY"
	}
}`

	actTilJSON, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	resultTil := expTilJSON == string(actTilJSON)
	if !resultTil {
		fmt.Println("ACTUAL TIL:", string(actTilJSON))
		fmt.Println("EXPECT TIL:", string(expTilJSON))
	}
	assert.True(t, resultTil)

}

// TestInsertIDsFromSingleFileIntoNonEmptyJSONWithDoubledIDinsideLi ...
//
// IDs 1200 & 1201 are exist, so they are expected to be restored on the old position.
// The IDs 100 & 101 are newly assigned.
func TestInsertIDsFromSingleFileIntoNonEmptyJSONWithSingleIDinsideLi(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

	testSet := []srcFile{
		// fn: start:                                                insertedIDs:
		{fn0, `TRice( iD(1444), "Hi!" ); TRice( iD(122), "Lo!" );`, `TRice( iD(1444), "Hi!" ); TRice( iD(122), "Lo!" );`},
	}

	// create src file
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// trice ID list
	tilJSON := `{
	"222": {
		"Type": "TRice",
		"Strg": "XXX"
	},
	"223": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`

	// location information
	liJSON := `{
	"222": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"223": {
		"File": "` + fn1 + `",
		"Line": 1
	}
}`

	// re-create til.json
	assert.Nil(t, fSys.WriteFile(id.FnJSON, []byte(tilJSON), 0777))

	// re-create li.json
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(liJSON), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-v", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", ".", "-IDMin", "70", "-IDMax", "80", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		result := k.insertedIDs == string(actSrc)
		if !result {
			fmt.Println("ACTUAL SRC:", string(actSrc))
		}
		assert.True(t, result)
	}

	// location information
	expLiJSON := `{
	"122": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"1444": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"222": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"223": {
		"File": "` + fn1 + `",
		"Line": 1
	}
}`
	actLiJSON, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	resultLI := expLiJSON == string(actLiJSON)
	if !resultLI {
		fmt.Println("ACTUAL LI:", string(actLiJSON))
		fmt.Println("EXPECT LI:", string(expLiJSON))
	}
	assert.True(t, resultLI)

	// trice ID list
	expTilJSON := `{
	"122": {
		"Type": "TRice",
		"Strg": "Lo!"
	},
	"1444": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"222": {
		"Type": "TRice",
		"Strg": "XXX"
	},
	"223": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`

	actTilJSON, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	resultTil := expTilJSON == string(actTilJSON)
	if !resultTil {
		fmt.Println("ACTUAL TIL:", string(actTilJSON))
		fmt.Println("EXPECT TIL:", string(expTilJSON))
	}
	assert.True(t, resultTil)

}

// TestInsertIDsFromSingleFileIntoNonEmptyJSONWithDoubledIDinsideLi ...
//
// IDs 1200 & 1201 are exist, so they are expected to be restored on the old position.
// The IDs 100 & 101 are newly assigned.
func TestInsertIDsFromSingleFileIntoNonEmptyJSONWithDoubledIDinsideLi(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

	testSet := []srcFile{
		// fn: start:                                                insertedIDs:
		{fn0, `TRice( iD(1444), "Hi!" ); TRice( iD(122), "Lo!" );`, `TRice( iD(1444), "Hi!" ); TRice( iD(122), "Lo!" );`},
	}

	// create src file
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// trice ID list
	tilJSON := `{
	"222": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"223": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`

	// location information
	liJSON := `{
	"222": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"223": {
		"File": "` + fn1 + `",
		"Line": 1
	}
}`

	// re-create til.json
	assert.Nil(t, fSys.WriteFile(id.FnJSON, []byte(tilJSON), 0777))

	// re-create li.json
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(liJSON), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-v", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", ".", "-IDMin", "70", "-IDMax", "80", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		result := k.insertedIDs == string(actSrc)
		if !result {
			fmt.Println("ACTUAL SRC:", string(actSrc))
		}
		assert.True(t, result)
	}

	// location information
	expLiJSON := `{
	"122": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"1444": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"222": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"223": {
		"File": "` + fn1 + `",
		"Line": 1
	}
}`

	actLiJSON, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	resultLI := expLiJSON == string(actLiJSON)
	if !resultLI {
		fmt.Println("ACTUAL LI:", string(actLiJSON))
		fmt.Println("EXPECT LI:", string(expLiJSON))
	}
	assert.True(t, resultLI)

	// trice ID list
	expTilJSON := `{
	"122": {
		"Type": "TRice",
		"Strg": "Lo!"
	},
	"1444": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"222": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"223": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`

	actTilJSON, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	resultTil := expTilJSON == string(actTilJSON)
	if !resultTil {
		fmt.Println("ACTUAL TIL:", string(actTilJSON))
		fmt.Println("EXPECT TIL:", string(expTilJSON))
	}
	assert.True(t, resultTil)

}

// TestInsertIDsIntoCleanFilesWithTilJSON ...
//
// IDs 1200 & 1201 are exist, so they are expected to be restored on the old position.
// The IDs 100 & 101 are newly assigned.
func TestInsertIDsFromFilesIntoNonEmptyJSON(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

	testSet := []srcFile{
		// fn: start:                                                insertedIDs:
		{fn0, `TRice( iD(1200), "Hi!" ); TRice( iD(101), "Lo!" );`, `TRice( iD(1200), "Hi!" ); TRice( iD(101), "Lo!" );`},
		{fn1, `TRice( iD(1201), "hi!" ); TRice( iD(100), "lo!" );`, `TRice( iD(1201), "hi!" ); TRice( iD(100), "lo!" );`},
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// trice ID list
	tilJSON := `{
	"222": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"223": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`

	// location information
	liJSON := `{
	"222": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"223": {
		"File": "` + fn1 + `",
		"Line": 1
	}
}`

	// re-create til.json
	assert.Nil(t, fSys.WriteFile(id.FnJSON, []byte(tilJSON), 0777))

	// re-create li.json
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(liJSON), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", ".", "-IDMin", "70", "-IDMax", "80", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		result := k.insertedIDs == string(actSrc)
		if !result {
			fmt.Println("ACTUAL SRC:", string(actSrc))
		}
		assert.True(t, result)
	}

	// location information
	expLiJSON := `{
	"100": {
		"File": "` + fn1 + `",
		"Line": 1
	},
	"101": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"1200": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"1201": {
		"File": "` + fn1 + `",
		"Line": 1
	},
	"222": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"223": {
		"File": "` + fn1 + `",
		"Line": 1
	}
}`

	actLiJSON, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	resultLI := expLiJSON == string(actLiJSON)
	if !resultLI {
		fmt.Println("ACTUAL LI:", string(actLiJSON))
		fmt.Println("EXPECT LI:", string(expLiJSON))
	}
	assert.True(t, resultLI)

	// trice ID list
	expTilJSON := `{
	"100": {
		"Type": "TRice",
		"Strg": "lo!"
	},
	"101": {
		"Type": "TRice",
		"Strg": "Lo!"
	},
	"1200": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"1201": {
		"Type": "TRice",
		"Strg": "hi!"
	},
	"222": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"223": {
		"Type": "TRice",
		"Strg": "Hi!"
	}
}`
	actTilJSON, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	resultTil := expTilJSON == string(actTilJSON)
	if !resultTil {
		fmt.Println("ACTUAL TIL:", string(actTilJSON))
		fmt.Println("EXPECT TIL:", expTilJSON)
	}
	assert.True(t, resultTil)

}

// TestInsertIDsIntoCleanFilesWithTilJSON ...
//
// IDs 1200 & 1201 are exist, so they are expected to be restored on the old position.
// The IDs 100 & 101 are newly assigned.
func TestInsertIDsFromFilesInEmptyJSON(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

	testSet := []srcFile{
		// fn: start:                                                insertedIDs:
		{fn0, `TRice( iD(1200), "Hi!" ); TRice( iD(101), "Lo!" );`, `TRice( iD(1200), "Hi!" ); TRice( iD(101), "Lo!" );`},
		{fn1, `TRice( iD(1201), "hi!" ); TRice( iD(100), "lo!" );`, `TRice( iD(1201), "hi!" ); TRice( iD(100), "lo!" );`},
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", ".", "-IDMin", "70", "-IDMax", "80", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		result := k.insertedIDs == string(actSrc)
		if !result {
			fmt.Println("ACTUAL SRC:", string(actSrc))
		}
		assert.True(t, result)
	}

	// location information
	expLiJSON := `{
	"100": {
		"File": "` + fn1 + `",
		"Line": 1
	},
	"101": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"1200": {
		"File": "` + fn0 + `",
		"Line": 1
	},
	"1201": {
		"File": "` + fn1 + `",
		"Line": 1
	}
}`

	actLiJSON, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	resultLI := expLiJSON == string(actLiJSON)
	if !resultLI {
		fmt.Println("ACTUAL LI:", string(actLiJSON))
		fmt.Println("EXPECT LI:", string(expLiJSON))
	}
	assert.True(t, resultLI)

	// trice ID list
	expTilJSON := `{
	"100": {
		"Type": "TRice",
		"Strg": "lo!"
	},
	"101": {
		"Type": "TRice",
		"Strg": "Lo!"
	},
	"1200": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"1201": {
		"Type": "TRice",
		"Strg": "hi!"
	}
}`

	actTilJSON, e := fSys.ReadFile(id.FnJSON)
	assert.Nil(t, e)
	resultTil := expTilJSON == string(actTilJSON)
	if !resultTil {
		fmt.Println("ACTUAL TIL:", string(actTilJSON))
		fmt.Println("EXPECT TIL:", string(expTilJSON))
	}
	assert.True(t, resultTil)
}

// TestInsertIDsIntoCleanFilesWithTilJSON ...
//
// IDs 1200 & 1201 are exist, so they are expected to be restored on the old position.
// The IDs 100 & 101 are newly assigned.
func TestInsertIDsIntoCleanFilesWithTilJSON(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

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
		"File": "` + fn0 + `",
		"Line": 1
	},
	"1201": {
		"File": "` + fn1 + `",
		"Line": 1
	}
}`
	testSet := []srcFile{
		// fn: clean:                             insertedIDs:
		{fn0, `TRice( "Hi!" ); TRice( "Hi!" );`, `TRice( iD(1200), "Hi!" ); TRice( iD(101), "Hi!" );`},
		{fn1, `TRice( "Hi!" ); TRice( "Hi!" );`, `TRice( iD(1201), "Hi!" ); TRice( iD(100), "Hi!" );`},
	}

	alternativeResultSet := []string{
		`TRice( iD(1200), "Hi!" ); TRice( iD(100), "Hi!" );`,
		`TRice( iD(1201), "Hi!" ); TRice( iD(101), "Hi!" );`,
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// re-create til.json
	assert.Nil(t, fSys.WriteFile(id.FnJSON, []byte(tilJSON), 0777))

	// re-create li.json
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(liJSON), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-v", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", ".", "-IDMin", "100", "-IDMethod", "upward"}))

	// check source files
	for i, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		result := testSet[i].insertedIDs == string(actSrc) || alternativeResultSet[i] == string(actSrc)
		if !result {
			fmt.Println("ACTUAL SRC:", string(actSrc))
			fmt.Println("EXPECT SRC:", testSet[i].insertedIDs)
			fmt.Println("ALTERN SRC:", alternativeResultSet[i])
		}
		assert.True(t, result)
	} // We do not know, which file is processed first.
}

func TestInsertIDsIntoCleanFilesWithEmptyTilJSON(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

	testSet := []srcFile{
		{fn0, `TRice( "Hi!" );`, `TRice( iD(1001), "Hi!" );`},
		{fn1, `TRice( "Hi!" );`, `TRice( iD(1000), "Hi!" );`},
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, fSys.WriteFile(k.fn, []byte(k.clean), 0777))
	}

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-til", id.FnJSON, "-li", id.LIFnJSON, "-src", ".", "-IDMin", "1000", "-IDMethod", "upward", "-v"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := fSys.ReadFile(k.fn)
		assert.Nil(t, e)
		result := testSet[0].insertedIDs == string(actSrc) || testSet[1].insertedIDs == string(actSrc)
		if !result {
			fmt.Println("ACTUAL SRC:", string(actSrc))
		}
		assert.True(t, result)
	} // We do not know, which file is processed first.
}
