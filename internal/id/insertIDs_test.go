// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import (
	"bytes"
	"io"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func TestInsertKnownID(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

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
	assert.Nil(t, fSys.WriteFile("li.json", []byte(exsLI), 0777))

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
	assert.Nil(t, fSys.WriteFile("til.json", []byte(exsTIL), 0777))

	// create src file1
	src1 := `
	TRice( id(77),"%x" );
	`
	assert.Nil(t, fSys.WriteFile("file1.c", []byte(src1), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward"}))

	// check modified src file1
	expSrc1 := `
	TRice( id(77),"%x" );
	`
	actSrc1, e := fSys.ReadFile("file1.c")
	assert.Nil(t, e)
	assert.Equal(t, expSrc1, string(actSrc1))
}

func TestInsertExistingID_A(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// create empty li.json
	assert.Nil(t, fSys.WriteFile("li.json", []byte(``), 0777))

	// create src file1
	sFn1 := "folder1/file1.c"
	src1 := `
	TRice( id(77),"x" );
	`
	assert.Nil(t, fSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward"}))

	// check untouched src file1
	actSrc1, e := fSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, src1, string(actSrc1))

	// create expected til.json file
	expTil := `{
	"77": {
		"Type": "TRice",
		"Strg": "x"
	}
}`
	actTil, e := fSys.ReadFile("til.json")
	assert.Nil(t, e)
	assert.Equal(t, expTil, string(actTil))

	expLi := `{
	"77": {
		"File": "file1.c",
		"Line": 2
	}
}`
	actLi, e := fSys.ReadFile("li.json")
	assert.Nil(t, e)
	assert.Equal(t, expLi, string(actLi))
}

func TestInsertExistingID_B(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// create empty li.json
	assert.Nil(t, fSys.WriteFile("li.json", []byte(``), 0777))

	// create src file1
	sFn1 := "folder1/file1.c"
	src1 := `
	TRice( id(77),"x" );
	`
	assert.Nil(t, fSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward", "-liPathIsRelative"}))

	// check untouched src file1
	actSrc1, e := fSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, src1, string(actSrc1))

	// create expected til.json file
	expTil := `{
	"77": {
		"Type": "TRice",
		"Strg": "x"
	}
}`
	actTil, e := fSys.ReadFile("til.json")
	assert.Nil(t, e)
	assert.Equal(t, expTil, string(actTil))

	expLi := `{
	"77": {
		"File": "folder1/file1.c",
		"Line": 2
	}
}`
	actLi, e := fSys.ReadFile("li.json")
	assert.Nil(t, e)
	assert.Equal(t, expLi, string(actLi))
}

func TestInsert99(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// create empty li.json
	assert.Nil(t, fSys.WriteFile("li.json", []byte(``), 0777))

	// create src file1
	sFn1 := "file1.c"
	src1 := `
	TRice("x" ); 
	TRice( "x" );
	TRice(  "x" );
	TRice(id(0),"x" );
	TRice( id(0),"x" );
	TRice(  id(0),"x" );
	TRice(id(0), "x" );
	TRice(   id(0),    "x" );
	TRICE( "%x", 1)
	TRICE(   Id(0)  , "%x", 1)
	`
	assert.Nil(t, fSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward"}))

	// check modified src file1
	expSrc1 := `
	TRice( iD(999), "x" ); 
	TRice( iD(998), "x" );
	TRice( iD(997), "x" );
	TRice( iD(996), "x" );
	TRice( iD(995), "x" );
	TRice( iD(994), "x" );
	TRice( iD(993), "x" );
	TRice( iD(992), "x" );
	TRICE( ID(991), "%x", 1)
	TRICE( Id(990), "%x", 1)
	`
	actSrc1, e := fSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc1, string(actSrc1))
}

func TestInsert11(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// create empty li.json
	assert.Nil(t, fSys.WriteFile("li.json", []byte(``), 0777))

	// create src file1
	sFn1 := "file1.c"
	src1 := `
	TRice(    id(0), "x" );TRice( id(0)   , "x" );
	`
	assert.Nil(t, fSys.WriteFile(sFn1, []byte(src1), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward"}))

	// check modified src file1
	expSrc1 := `
	TRice( iD(999), "x" );TRice( iD(998), "x" );
	`
	actSrc1, e := fSys.ReadFile(sFn1)
	assert.Nil(t, e)
	assert.Equal(t, expSrc1, string(actSrc1))
}

func TestInsert2(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create src file
	sFn := "file.c"
	src := `
	break; case __LINE__: TRice(   iD(0), "msg:value=%d\n", -1  );
	break; case __LINE__: TRice( "msg:value=%d\n", -1  );
	`

	assert.Nil(t, fSys.WriteFile(sFn, []byte(src), 0777))

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// create empty li.json
	assert.Nil(t, fSys.WriteFile("li.json", []byte(``), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward"}))

	// check modified src file
	expSrc := `
	break; case __LINE__: TRice( iD(999), "msg:value=%d\n", -1  );
	break; case __LINE__: TRice( iD(998), "msg:value=%d\n", -1  );
	`
	actSrc, e := fSys.ReadFile(sFn)
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
	actLI, e := fSys.ReadFile("li.json")
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
	actTIL, e := fSys.ReadFile(jFn)
	assert.Nil(t, e)
	assert.Equal(t, expTIL, string(actTIL))
}

func TestInsert1(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create src file
	sFn := "file.c"
	src := `break; case __LINE__: TRice( iD(0), "msg:value=%d\n", -1  );`

	assert.Nil(t, fSys.WriteFile(sFn, []byte(src), 0777))

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// create empty li.json
	assert.Nil(t, fSys.WriteFile("li.json", []byte(``), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward"}))

	// check modified src file
	expSrc := `break; case __LINE__: TRice( iD(999), "msg:value=%d\n", -1  );`
	actSrc, e := fSys.ReadFile(sFn)
	assert.Nil(t, e)
	assert.Equal(t, expSrc, string(actSrc))

	// check modified li.json file
	expLI := `{
	"999": {
		"File": "file.c",
		"Line": 1
	}
}`
	actLI, e := fSys.ReadFile("li.json")
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))

	// check modified til.json file
	expTIL := `{
	"999": {
		"Type": "TRice",
		"Strg": "msg:value=%d\\n"
	}
}`
	actTIL, e := fSys.ReadFile(jFn)
	assert.Nil(t, e)
	assert.Equal(t, expTIL, string(actTIL))
}

func TestInsert0(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create src file
	sFn := "file.c"
	src := `break; case __LINE__: TRice( "msg:value=%d\n", -1  );`

	assert.Nil(t, fSys.WriteFile(sFn, []byte(src), 0777))

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// create empty li.json
	assert.Nil(t, fSys.WriteFile("li.json", []byte(``), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward"}))

	// check modified src file
	expSrc := `break; case __LINE__: TRice( iD(999), "msg:value=%d\n", -1  );`
	actSrc, e := fSys.ReadFile(sFn)
	assert.Nil(t, e)
	assert.Equal(t, expSrc, string(actSrc))

	// check modified li.json file
	expLI := `{
	"999": {
		"File": "file.c",
		"Line": 1
	}
}`
	actLI, e := fSys.ReadFile("li.json")
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))

	// check modified til.json file
	expTIL := `{
	"999": {
		"Type": "TRice",
		"Strg": "msg:value=%d\\n"
	}
}`
	actTIL, e := fSys.ReadFile(jFn)
	assert.Nil(t, e)
	assert.Equal(t, expTIL, string(actTIL))
}
