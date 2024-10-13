package id_test

// The tests in this file cannot run concurrently!

import (
	"fmt"
	"testing"

	"github.com/rokath/trice/internal/args"
	. "github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

// TestInsertIDsFromSingleFileIntoNonEmptyJSONWithDoubledIDinsideLi ...
//
// IDs 1200 & 1201 are exist, so they are expected to be restored on the old position.
// The IDs 100 & 101 are newly assigned.
func TestInsertIDsFromSingleFileIntoNonEmptyJSONWithNoIDinsideLi(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

	testSet := []SrcFile{
		// fn: start:                                                insertedIDs:
		{fn0, `TRice(iD(1444), "Hi!" ); TRice(iD(122), "Lo!" );`, `TRice(iD(1444), "Hi!" ); TRice(iD(122), "Lo!" );`},
	}

	// create src file
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
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
	assert.Nil(t, FSys.WriteFile(FnJSON, []byte(tilJSON), 0777))

	// re-create li.json
	assert.Nil(t, FSys.WriteFile(LIFnJSON, []byte(liJSON), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "insert", "-v", "-til", FnJSON, "-li", LIFnJSON, "-src", fn0, "-src", fn1, "-IDMin", "70", "-IDMax", "80", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		result := k.InsertedIDs == string(actSrc)
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
	actLiJSON, e := FSys.ReadFile(LIFnJSON)
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

	actTilJSON, e := FSys.ReadFile(FnJSON)
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
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

	testSet := []SrcFile{
		// fn: start:                                                insertedIDs:
		{fn0, `TRice(iD(1444), "Hi!" ); TRice(iD(122), "Lo!" );`, `TRice(iD(1444), "Hi!" ); TRice(iD(122), "Lo!" );`},
	}

	// create src file
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
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
	assert.Nil(t, FSys.WriteFile(FnJSON, []byte(tilJSON), 0777))

	// re-create li.json
	assert.Nil(t, FSys.WriteFile(LIFnJSON, []byte(liJSON), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "insert", "-v", "-til", FnJSON, "-li", LIFnJSON, "-src", fn0, "-src", fn1, "-IDMin", "70", "-IDMax", "80", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		result := k.InsertedIDs == string(actSrc)
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
	actLiJSON, e := FSys.ReadFile(LIFnJSON)
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

	actTilJSON, e := FSys.ReadFile(FnJSON)
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
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

	testSet := []SrcFile{
		// fn: start:                                                insertedIDs:
		{fn0, `TRice(iD(1444), "Hi!" ); TRice(iD(122), "Lo!" );`, `TRice(iD(1444), "Hi!" ); TRice(iD(122), "Lo!" );`},
	}

	// create src file
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
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
	assert.Nil(t, FSys.WriteFile(FnJSON, []byte(tilJSON), 0777))

	// re-create li.json
	assert.Nil(t, FSys.WriteFile(LIFnJSON, []byte(liJSON), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "insert", "-v", "-til", FnJSON, "-li", LIFnJSON, "-src", fn0, "-src", fn1, "-IDMin", "70", "-IDMax", "80", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		result := k.InsertedIDs == string(actSrc)
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

	actLiJSON, e := FSys.ReadFile(LIFnJSON)
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

	actTilJSON, e := FSys.ReadFile(FnJSON)
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
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

	testSet := []SrcFile{
		// fn: start:                                                insertedIDs:
		{fn0, `TRice(iD(1200), "Hi!" ); TRice(iD(101), "Lo!" );`, `TRice(iD(1200), "Hi!" ); TRice(iD(101), "Lo!" );`},
		{fn1, `TRice(iD(1201), "hi!" ); TRice(iD(100), "lo!" );`, `TRice(iD(1201), "hi!" ); TRice(iD(100), "lo!" );`},
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
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
	assert.Nil(t, FSys.WriteFile(FnJSON, []byte(tilJSON), 0777))

	// re-create li.json
	assert.Nil(t, FSys.WriteFile(LIFnJSON, []byte(liJSON), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "insert", "-til", FnJSON, "-li", LIFnJSON, "-src", fn0, "-src", fn1, "-IDMin", "70", "-IDMax", "80", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		result := k.InsertedIDs == string(actSrc)
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

	actLiJSON, e := FSys.ReadFile(LIFnJSON)
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
	actTilJSON, e := FSys.ReadFile(FnJSON)
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
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

	testSet := []SrcFile{
		// fn: start:                                                insertedIDs:
		{fn0, `TRice(iD(1200), "Hi!" ); TRice(iD(101), "Lo!" );`, `TRice(iD(1200), "Hi!" ); TRice(iD(101), "Lo!" );`},
		{fn1, `TRice(iD(1201), "hi!" ); TRice(iD(100), "lo!" );`, `TRice(iD(1201), "hi!" ); TRice(iD(100), "lo!" );`},
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
	}

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "insert", "-til", FnJSON, "-li", LIFnJSON, "-src", fn0, "-src", fn1, "-IDMin", "70", "-IDMax", "80", "-IDMethod", "upward"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		result := k.InsertedIDs == string(actSrc)
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

	actLiJSON, e := FSys.ReadFile(LIFnJSON)
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

	actTilJSON, e := FSys.ReadFile(FnJSON)
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
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

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
	testSet := []SrcFile{
		// fn: clean:                             insertedIDs:
		{fn0, `TRice("Hi!" ); TRice("Hi!" );`, `TRice(iD(1200), "Hi!" ); TRice(iD(101), "Hi!" );`},
		{fn1, `TRice("Hi!" ); TRice("Hi!" );`, `TRice(iD(1201), "Hi!" ); TRice(iD(100), "Hi!" );`},
	}

	alternativeResultSet := []string{
		`TRice(iD(1200), "Hi!" ); TRice(iD(100), "Hi!" );`,
		`TRice(iD(1201), "Hi!" ); TRice(iD(101), "Hi!" );`,
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
	}

	// re-create til.json
	assert.Nil(t, FSys.WriteFile(FnJSON, []byte(tilJSON), 0777))

	// re-create li.json
	assert.Nil(t, FSys.WriteFile(LIFnJSON, []byte(liJSON), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "insert", "-v", "-til", FnJSON, "-li", LIFnJSON, "-src", fn0, "-src", fn1, "-IDMin", "100", "-IDMethod", "upward"}))

	// check source files
	for i, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		result := testSet[i].InsertedIDs == string(actSrc) || alternativeResultSet[i] == string(actSrc)
		if !result {
			fmt.Println("ACTUAL SRC:", string(actSrc))
			fmt.Println("EXPECT SRC:", testSet[i].InsertedIDs)
			fmt.Println("ALTERN SRC:", alternativeResultSet[i])
		}
		assert.True(t, result)
	} // We do not know, which file is processed first.
}

func TestInsertIDsIntoCleanFilesWithEmptyTilJSON(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	fn0 := t.Name() + "_file0.c"
	fn1 := t.Name() + "_file1.c"

	testSet := []SrcFile{
		{fn0, `TRice("Hi!" );`, `TRice(iD(1001), "Hi!" );`},
		{fn1, `TRice("Hi!" );`, `TRice(iD(1000), "Hi!" );`},
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
	}

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "insert", "-til", FnJSON, "-li", LIFnJSON, "-src", fn0, "-src", fn1, "-IDMin", "1000", "-IDMethod", "upward", "-v"}))

	// check source files
	for _, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		result := testSet[0].InsertedIDs == string(actSrc) || testSet[1].InsertedIDs == string(actSrc)
		if !result {
			fmt.Println("ACTUAL SRC:", string(actSrc))
		}
		assert.True(t, result)
	} // We do not know, which file is processed first.
}
