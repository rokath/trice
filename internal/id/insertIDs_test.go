// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import (
	"fmt"
	"os"
	"testing"

	"github.com/rokath/trice/internal/args"
	. "github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

func TestInsertKnownID(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create existing li.json file
	exsLI := `{
	"66": {
		"File": "file1.c",
		"Line": 3
	},
	"77": {
		"File": "file1.c",
		"Line": 3
	},
	"999": {
		"File": "fileX.c",
		"Line": 2
	}
}`
	assert.Nil(t, FSys.WriteFile(LIFnJSON, []byte(exsLI), 0777))

	// create existing til.json file
	exsTIL := `{
	"55": {
		"Type": "TRice",
		"Strg": "msg:value=%d\\n"
	},
	"77": {
		"Type": "TRice",
		"Strg": "%x"
	}
}`
	assert.Nil(t, FSys.WriteFile(FnJSON, []byte(exsTIL), 0777))

	// create src file1
	src1 := `
	TRice(id(77),"%x", 123 );
	`
	assert.Nil(t, FSys.WriteFile("file1.c", []byte(src1), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-src", "file1.c", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc1 := `
	TRice(id(77),"%x", 123 );
	`
	actSrc1, e := FSys.ReadFile("file1.c")
	assert.Nil(t, e)
	assert.Equal(t, expSrc1, string(actSrc1))

	// cleanup
	FSys.Remove(FnJSON)
	FSys.Remove(LIFnJSON)
	FSys.RemoveAll(UserHomeDir)
}

func TestInsertExistingID_A(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file1
	sFn1 := "folder1/file1.c"
	src1 := `
	TRice(id(77),"x" );
	`
	assert.Nil(t, FSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-src", "folder1/file1.c", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward", "-til", FnJSON, "-li", LIFnJSON}))

	// check untouched src file1
	actSrc1, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, src1, string(actSrc1))

	// create expected til.json file
	expTil := `{
	"77": {
		"Type": "TRice",
		"Strg": "x"
	}
}`
	actTil, e := FSys.ReadFile(FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expTil, string(actTil))

	expLi := `{
	"77": {
		"File": "file1.c",
		"Line": 2
	}
}`
	actLi, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLi, string(actLi))
}

func TestInsertExistingID_B(t *testing.T) {

	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file1
	sFn1 := "folder1/file1.c"
	src1 := `
	TRice(id(77),"x" );
	`
	assert.Nil(t, FSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-src", "folder1/file1.c", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward", "-liPath=relative", "-til", FnJSON, "-li", LIFnJSON}))

	// check untouched src file1
	actSrc1, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, src1, string(actSrc1))

	// create expected til.json file
	expTil := `{
	"77": {
		"Type": "TRice",
		"Strg": "x"
	}
}`
	actTil, e := FSys.ReadFile(FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expTil, string(actTil))

	expLi := `{
	"77": {
		"File": "folder1/file1.c",
		"Line": 2
	}
}`
	actLi, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLi, string(actLi))
}

func TestInsert99(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.
	DefaultStampSize = 32
	// create src file1
	sFn1 := "file1.c"
	src1 := `
	TRice("x" ); 
	TRice("x" );
	TRice( "x" );
	TRice(id(0),"x" );
	TRice(id(0),"x" );
	TRice( id(0),"x" );
	TRice(id(0), "x" );
	TRice(  id(0),    "x" );
	TRICE("%x", 1)
	TRICE(  Id(0)  , "%x", 1)
	`
	assert.Nil(t, FSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-src", "file1.c", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc1 := `
	TRice(iD(999), "x" ); 
	TRice(iD(998), "x" );
	TRice(iD(997), "x" );
	TRice(iD(996), "x" );
	TRice(iD(995), "x" );
	TRice(iD(994), "x" );
	TRice(iD(993), "x" );
	TRice(iD(992), "x" );
	TRICE(ID(991), "%x", 1)
	TRICE(Id(990), "%x", 1)
	`
	actSrc1, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc1, string(actSrc1))
}

func TestInsert99w(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.
	DefaultStampSize = 32
	// create src file1
	sFn1 := "file1.c"
	src1 := `
	TRice("x" ); 
	TRice("x" );
	TRice( "x" );
	TRice(id(0),"x" );
	TRice(id(0),"x" );
	TRice( id(0),"x" );
	TRice(id(0), "x" );
	TRice(  id(0),    "x" );
	TRICE("%x", 1)
	TRICE(  Id(0)  , "%x", 1)
	`
	assert.Nil(t, FSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-w", "-src", "file1.c", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc1 := `
	TRice( iD( 999 ), "x" ); 
	TRice( iD( 998 ), "x" );
	TRice( iD( 997 ), "x" );
	TRice( iD( 996 ), "x" );
	TRice( iD( 995 ), "x" );
	TRice( iD( 994 ), "x" );
	TRice( iD( 993 ), "x" );
	TRice( iD( 992 ), "x" );
	TRICE( ID( 991 ), "%x", 1)
	TRICE( Id( 990 ), "%x", 1)
	`
	actSrc1, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc1, string(actSrc1))
}

func TestInsert11(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file1
	sFn1 := "file1.c"
	src1 := `
	TRice(   id(0), "x" );TRice(id(0)   , "x" );
	`
	assert.Nil(t, FSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-src", "file1.c", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc1 := `
	TRice(iD(999), "x" );TRice(iD(998), "x" );
	`
	actSrc1, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc1, string(actSrc1))
}

func TestInsert2(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file
	sFn := "file.c"
	src := `
	break; case __LINE__: TRice(  iD(0), "msg:value=%d\n", -1  );
	break; case __LINE__: TRice("msg:value=%d\n", -1  );
	`

	assert.Nil(t, FSys.WriteFile(sFn, []byte(src), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-src", "file.c", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file
	expSrc := `
	break; case __LINE__: TRice(iD(999), "msg:value=%d\n", -1  );
	break; case __LINE__: TRice(iD(998), "msg:value=%d\n", -1  );
	`
	actSrc, e := FSys.ReadFile(sFn)
	assert.Nil(t, e)
	assert.Equal(t, expSrc, string(actSrc))

	// check modified li.json file
	expLI := `{
	"998": {
		"File": "file.c",
		"Line": 3
	},
	"999": {
		"File": "file.c",
		"Line": 2
	}
}`
	actLI, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))

	// check modified til.json file
	expTIL := `{
	"998": {
		"Type": "TRice",
		"Strg": "msg:value=%d\\n"
	},
	"999": {
		"Type": "TRice",
		"Strg": "msg:value=%d\\n"
	}
}`
	actTIL, e := FSys.ReadFile(FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expTIL, string(actTIL))
}

func TestInsert1(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file
	sFn := "file.c"
	src := `break; case __LINE__: TRice(iD(0), "msg:value=%d\n", -1  );`

	assert.Nil(t, FSys.WriteFile(sFn, []byte(src), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-src", "file.c", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file
	expSrc := `break; case __LINE__: TRice(iD(999), "msg:value=%d\n", -1  );`
	actSrc, e := FSys.ReadFile(sFn)
	assert.Nil(t, e)
	assert.Equal(t, expSrc, string(actSrc))

	// check modified li.json file
	expLI := `{
	"999": {
		"File": "file.c",
		"Line": 1
	}
}`
	actLI, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))

	// check modified til.json file
	expTIL := `{
	"999": {
		"Type": "TRice",
		"Strg": "msg:value=%d\\n"
	}
}`
	actTIL, e := FSys.ReadFile(FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expTIL, string(actTIL))
}

func TestInsert0(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file
	sFn := "file.c"
	src := `break; case __LINE__: TRice("msg:value=%d\n", -1  );`

	assert.Nil(t, FSys.WriteFile(sFn, []byte(src), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-src", "file.c", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file
	expSrc := `break; case __LINE__: TRice(iD(999), "msg:value=%d\n", -1  );`
	actSrc, e := FSys.ReadFile(sFn)
	assert.Nil(t, e)
	assert.Equal(t, expSrc, string(actSrc))

	// check modified li.json file
	expLI := `{
	"999": {
		"File": "file.c",
		"Line": 1
	}
}`
	actLI, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))

	// check modified til.json file
	expTIL := `{
	"999": {
		"Type": "TRice",
		"Strg": "msg:value=%d\\n"
	}
}`
	actTIL, e := FSys.ReadFile(FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expTIL, string(actTIL))
}

func TestInsertWithTickInComment(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	// create src file1
	sFn1 := "file1.c"
	src1 := `
	//""'
	//"
	TRice("x" );
	//"
	TRice("x" );
	triceAssertTrue("x", flag );
	TriceAssertTrue("x", flag );
	TRiceAssertTrue("x", flag );
	triceAssertFalse("x", flag );
	TriceAssertFalse("x", flag );
	TRiceAssertFalse("x", flag );
	`
	assert.Nil(t, FSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-src", "file1.c", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward", "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file1
	expSrc1 := `
	//""'
	//"
	TRice("x" );
	//"
	TRice(iD(999), "x" );
	triceAssertTrue(iD(998), "x", flag );
	TriceAssertTrue(iD(997), "x", flag );
	TRiceAssertTrue(iD(996), "x", flag );
	triceAssertFalse(iD(995), "x", flag );
	TriceAssertFalse(iD(994), "x", flag );
	TRiceAssertFalse(iD(993), "x", flag );
	`
	actSrc1, e := FSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc1, string(actSrc1))
}

// TestInsertIDsIntoTilJSONFromFileWithLi ...
//
// IDs 1200 & 1201 are exist, so they are expected to go into til.json.
func TestInsertIDsIntoTilJSONFromFileWithLi(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

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
	testSet := []SrcFile{
		// fn:       existing IDs:                unchanged IDs:
		{fn0, `TRice(iD(1200), "Hi!" );`, `TRice(iD(1200), "Hi!" );`},
		{fn1, `TRice(iD(1201), "Lo!" );`, `TRice(iD(1201), "Lo!" );`},
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
	}

	// re-create li.json
	assert.Nil(t, FSys.WriteFile(LIFnJSON, []byte(liJSON), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "insert", "-src", fn0, "-src", fn1, "-til", FnJSON, "-li", LIFnJSON}))

	// check source files
	for _, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		assert.Equal(t, k.InsertedIDs, string(actSrc))
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
	actTil, e := FSys.ReadFile(FnJSON)
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
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	fn0 := t.Name() + "file0.c"
	fn1 := t.Name() + "file1.c"

	testSet := []SrcFile{
		// fn: in:                                                 expected:
		{fn0, `trice("new" ); trice("Lo!" );` /*           */, `trice(iD(18), "new" ); trice(iD(17), "Lo!" );`},
		{fn1, `trice(iD(1200), "Hi!" ); trice(iD(19), "old" );`, `trice(iD(1200), "Hi!" ); trice(iD(19), "old" );`},
	}
	// create src files
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
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
	assert.Nil(t, FSys.WriteFile(LIFnJSON, []byte(liJSON), 0777))

	// When fn1 is processed first, how to know that ID 19 is used already without location information? -> Create first if empty

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "insert", "-src", fn0, "-src", fn1, "-IDMin=10", "-IDMax=19", "-IDMethod=downward", "-til", FnJSON, "-li", LIFnJSON}))

	// check source files
	for i, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		actS := string(actSrc)
		r := k.InsertedIDs == actS
		if !r {
			fmt.Println(i, " ACTUAL SRC:", actS)
			fmt.Println(i, " EXPECT SRC:", k.InsertedIDs)
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
	actTil, e := FSys.ReadFile(FnJSON)
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
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	fn0 := t.Name() + "file0.c"
	fn1 := t.Name() + "file1.c"

	testSet := []SrcFile{
		// fn: in:                                              expected:
		{fn0, `trice("new %d", 1 ); /* ' */ trice("Lo!" );`, `trice(iD(18), "new %d", 1 ); /* ' */ trice(iD(17), "Lo!" );`},
		{fn1, `trice(iD(1200), "Hi!" ); trice(iD(19), "old" );`, `trice(iD(1200), "Hi!" ); trice(iD(19), "old" );`},
	}
	// create src files
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
	}

	// When fn0 is processed first, how to know that ID 19 is used already in fn0 without location information? -> Create first if empty

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "clean", "-src", fn0, "-src", fn1, "-v", "-til", FnJSON, "-li", LIFnJSON}))

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
	actLIJSONs, e := FSys.ReadFile(LIFnJSON)
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
	actTil0, e := FSys.ReadFile(FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expTil0, string(actTil0))

	// 2nd action
	IDData.TagList = IDData.TagList[:0] // reset ID space
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "insert", "-src", fn0, "-src", fn1, "-v", "-IDMin=10", "-IDMax=19", "-IDMethod=downward", "-til", FnJSON, "-li", LIFnJSON}))

	// Do noit apply "-src=." here! It would set Srcs to be {".", "."} what results in a double parsing of the root folder causing the test to fail.
	// This is, because we called "trice clean" before in this test.

	// check source files
	for _, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		actS := string(actSrc)
		assert.Equal(t, k.InsertedIDs, actS)
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
		"Strg": "new %d"
	},
	"19": {
		"Type": "trice",
		"Strg": "old"
	}
}`
	actTil, e := FSys.ReadFile(FnJSON)
	assert.Nil(t, e)
	result := expTil == string(actTil)
	if !result {
		fmt.Println("ACTUAL TIL:", string(actTil))
		fmt.Println("EXPECT TIL:", expTil)
	}
	assert.True(t, result)
}

// TestInsertIDsForNewTrice2WithLI ...
func TestInsertIDsForNewTrice2WithoutLIAndTickInComment(t *testing.T) {

	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	fn0 := t.Name() + "file0.c"
	fn1 := t.Name() + "file1.c"

	testSet := []SrcFile{
		// fn: in:                                                 expected:
		{fn0, `trice("new %d", 1 ); /*'*/ trice("Lo!" );` /**/, `trice(iD(18), "new %d", 1 ); /*'*/ trice(iD(17), "Lo!" );`},
		{fn1, `trice(iD(1200), "Hi!" ); trice(iD(19), "old" );`, `trice(iD(1200), "Hi!" ); trice(iD(19), "old" );`},
	}
	// create src files
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
	}

	// When fn0 is processed first, how to know that ID 19 is used already in fn0 without location information? -> Create first if empty

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "clean", "-src", fn0, "-src", fn1, "-v", "-til", FnJSON, "-li", LIFnJSON}))

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
	actLIJSONs, e := FSys.ReadFile(LIFnJSON)
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
	actTil0, e := FSys.ReadFile(FnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expTil0, string(actTil0))

	// 2nd action
	IDData.TagList = IDData.TagList[:0] // reset ID space
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "insert", "-src", fn0, "-src", fn1, "-v", "-IDMin=10", "-IDMax=19", "-IDMethod=downward", "-til", FnJSON, "-li", LIFnJSON}))

	// Do noit apply "-src=." here! It would set Srcs to be {".", "."} what results in a double parsing of the root folder causing the test to fail.
	// This is, because we called "trice clean" before in this test.

	// check source files
	for _, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		actS := string(actSrc)
		assert.Equal(t, k.InsertedIDs, actS)
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
		"Strg": "new %d"
	},
	"19": {
		"Type": "trice",
		"Strg": "old"
	}
}`
	actTil, e := FSys.ReadFile(FnJSON)
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
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	fn0 := t.Name() + "file0.c"

	// location information
	liJSON := `{
}`
	testSet := []SrcFile{
		// fn: in:                                         expected:
		{fn0, `TRice(iD(1200), "Hi!" ); trice("new" );`, `TRice(iD(1200), "Hi!" ); trice(iD(1201), "new" );`},
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
	}

	// re-create li.json
	assert.Nil(t, FSys.WriteFile(LIFnJSON, []byte(liJSON), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"TRICE", "insert", "-src", fn0, "-IDMin=1200", "-IDMax=1201", "-IDMethod=upward", "-til", FnJSON, "-li", LIFnJSON}))

	// check source files
	for _, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		actS := string(actSrc)
		r := actS == k.InsertedIDs
		if !r {
			fmt.Println(" ACTUAL SRC:", actS)
			fmt.Println(" EXPECT SRC:", k.InsertedIDs)
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
	actTil, e := FSys.ReadFile(FnJSON)
	assert.Nil(t, e)
	result := expTil == string(actTil)
	if !result {
		fmt.Println("ACTUAL TIL:", string(actTil))
		fmt.Println("EXPECT TIL:", expTil)
	}
	assert.True(t, result)
}

// TestInsertIDsForNewTrice2 ...
func TestInsertIDsForNewTrice2(t *testing.T) {
	defer Setup(t)() // This executes Setup(t) and puts the returned function into the defer list.

	fn0 := t.Name() + "file0.c"
	fn1 := t.Name() + "file1.c"

	testSet := []SrcFile{
		// fn: in:                                                   expected:
		{fn0, `TRice(iD(1200), "Hi!" ); trice(iD(1201), "Lo!" );`, `TRice(iD(1200), "Hi!" ); trice(iD(1201), "Lo!" );`},
		{fn1, // in
			`	break; case __LINE__: triceS("msg:With triceS:%s\n", sABCDE );
				TRice("hi %d", 5 );
				// don` + "'" + `t forget
				trice("lo!" );
				break; case __LINE__: triceS("msg:With triceS:%s\n", sABCDE );
				trice32("msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );`,

			// expected
			`	break; case __LINE__: triceS(iD(1202), "msg:With triceS:%s\n", sABCDE );
				TRice(iD(1203), "hi %d", 5 );
				// don` + "'" + `t forget
				trice(iD(1204), "lo!" );
				break; case __LINE__: triceS(iD(1205), "msg:With triceS:%s\n", sABCDE );
				trice32(iD(1206), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );`},
	}

	// create src files
	for _, k := range testSet {
		assert.Nil(t, FSys.WriteFile(k.Fn, []byte(k.Clean), 0777))
	}

	// action 0
	assert.Nil(t, args.Handler(os.Stdout, FSys, []string{"trice", "add", "-src", fn0, "-src", fn1, "-til", FnJSON, "-li", LIFnJSON}))

	// check til.json
	expTil := `{
	"1200": {
		"Type": "TRice",
		"Strg": "Hi!"
	},
	"1201": {
		"Type": "trice",
		"Strg": "Lo!"
	}
}`
	actTil, e := FSys.ReadFile(FnJSON)
	assert.Nil(t, e)
	result := expTil == string(actTil)
	if !result {
		fmt.Println("ACTUAL TIL:", string(actTil))
		fmt.Println("EXPECT TIL:", expTil)
	}
	assert.True(t, result)

	// check li.json
	expLI := `{
	"1200": {
		"File": "TestInsertIDsForNewTrice2file0.c",
		"Line": 1
	},
	"1201": {
		"File": "TestInsertIDsForNewTrice2file0.c",
		"Line": 1
	}
}`
	actLI, e := FSys.ReadFile(LIFnJSON)
	assert.Nil(t, e)
	result = expLI == string(actLI)
	if !result {
		fmt.Println("ACTUAL LI:", string(actLI))
		fmt.Println("EXPECT LI:", expLI)
	}
	assert.True(t, result)

	// action 1 (2nd action)
	IDData.TagList = IDData.TagList[:0] // reset ID space
	e = args.Handler(os.Stdout, FSys, []string{"trice", "insert", "-src", fn0, "-src", fn1, "-IDMin=1200", "-IDMax=1299", "-IDMethod=upward", "-til", FnJSON, "-li", LIFnJSON})
	assert.Nil(t, e)

	// check source files
	for _, k := range testSet {
		actSrc, e := FSys.ReadFile(k.Fn)
		assert.Nil(t, e)
		actS := string(actSrc)

		assert.Equal(t, k.InsertedIDs, actS)

		r := actS == k.InsertedIDs
		if !r {
			fmt.Println(" ACTUAL SRC:", actS)
			fmt.Println(" EXPECT SRC:", k.InsertedIDs)
		}
		assert.True(t, r)
	}
}
