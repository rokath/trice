// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"bytes"
	"fmt"
	"io"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

// TestUpdateSet1 allows to test many test vectors by extending the testVector slice.
func TestUpdateSet1(t *testing.T) { // Anti-Virus issue
	type testVector struct {
		srcSrc0, srcSrc1 string
		expSrc0, expSrc1 string
	}
	triceMacro := []string{"TRICE8", "TRICE8_1", " TRICE8", " TRICE8_1", "TRICE8 ", "TRICE8_1 ", " TRICE8 ", " TRICE8_1 "}

	expLI := `{
	"1000": {
		"file": "file.c",
		"Line": 1
	}
}`

	set := []testVector{
		{
			`/* this is line 1*/ break; case __LINE__:`, `( "msg:value=%d\n", -1  ); // user written code with 16-bit stamp`,
			`/* this is line 1*/ break; case __LINE__:`, `( id( 1000), "msg:value=%d\n", -1  ); // user written code with 16-bit stamp`,
		},
	}

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	for i := range set {
		for k := range triceMacro {
			src := set[i].srcSrc0 + triceMacro[k] + set[i].srcSrc1
			expSrc := set[i].expSrc0 + triceMacro[k] + set[i].expSrc1

			// create empty til.json
			jFn := "til.json"
			JSONFile := ``
			assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

			// create src file
			sFn := "file.c"
			assert.Nil(t, fSys.WriteFile(sFn, []byte(src), 0777))

			// action
			var b bytes.Buffer
			assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "u", "-src", ".", "-v", "-IDMin", "1000", "-IDMax", "2000", "-IDMethod", "upward", "-defaultStampSize", "0"}))

			// check modified src file
			actSrc, e := fSys.ReadFile(sFn)
			assert.Nil(t, e)
			assert.Equal(t, expSrc, string(actSrc))

			// check generated li.json file
			actLI, e := fSys.ReadFile("li.json")
			assert.Nil(t, e)
			assert.Equal(t, expLI, string(actLI))
		}
	}
}

// TestUpdateSet allows to test many test vectors by extending the testVector slice.
func TestUpdateSet0(t *testing.T) { // Anti-Virus issue
	type testVector struct {
		srcSrc string
		expSrc string
	}

	expLI := `{
	"1000": {
		"file": "file.c",
		"Line": 1
	}
}`

	set := []testVector{
		{
			`/* this is line 1*/ break; case __LINE__: TRICE8( "msg:value=%d\n", -1  ); // user written code with 16-bit stamp`,
			`/* this is line 1*/ break; case __LINE__: TRICE8( id( 1000), "msg:value=%d\n", -1  ); // user written code with 16-bit stamp`,
		},
		{
			`/* this is line 1*/ break; case __LINE__: TRICE8     ( Id(0),      "msg:value=%d\n", -1  ); // user written code with 16-bit stamp`,
			`/* this is line 1*/ break; case __LINE__: TRICE8     ( Id( 1000),      "msg:value=%d\n", -1  ); // user written code with 16-bit stamp`,
		},
		{
			`/* this is line 1*/ break; case __LINE__: TRICE8_1   ( Id(0),      "msg:value=%d\n", -1  ); // user written code with 16-bit stamp`,
			`/* this is line 1*/ break; case __LINE__: TRICE8_1   ( Id( 1000),      "msg:value=%d\n", -1  ); // user written code with 16-bit stamp`,
		},
	}

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	for i := range set {
		src := set[i].srcSrc
		expSrc := set[i].expSrc

		// create empty til.json
		jFn := "til.json"
		JSONFile := ``
		assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

		// create src file
		sFn := "file.c"
		assert.Nil(t, fSys.WriteFile(sFn, []byte(src), 0777))

		// action
		var b bytes.Buffer
		assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "u", "-src", ".", "-v", "-IDMin", "1000", "-IDMax", "2000", "-IDMethod", "upward", "-defaultStampSize", "0"}))

		// check modified src file
		actSrc, e := fSys.ReadFile(sFn)
		assert.Nil(t, e)
		assert.Equal(t, expSrc, string(actSrc))

		// check generated li.json file
		actLI, e := fSys.ReadFile("li.json")
		assert.Nil(t, e)
		assert.Equal(t, expLI, string(actLI))
	}
}

func TestUpdateLegacy0(t *testing.T) { // Anti-Virus issue
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create src file
	sFn := "file.c"
	src := `/* this is line 1*/ break; case __LINE__: TRICE8     ( Id(0),      "msg:value=%d\n", -1  ); // user written code with 16-bit stamp`

	assert.Nil(t, fSys.WriteFile(sFn, []byte(src), 0777))

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "u", "-src", ".", "-v", "-IDMin", "1000", "-IDMax", "2000", "-IDMethod", "upward", "-defaultStampSize", "0"}))

	// check modified src file
	expSrc := `/* this is line 1*/ break; case __LINE__: TRICE8     ( Id( 1000),      "msg:value=%d\n", -1  ); // user written code with 16-bit stamp`

	actSrc, e := fSys.ReadFile(sFn)
	assert.Nil(t, e)

	//fmt.Println(string(actSrc))

	assert.Equal(t, expSrc, string(actSrc))

	// check modified til.json file
	expJSON := `{
	"1000": {
		"Type": "TRICE8",
		"Strg": "msg:value=%d\\n"
	}
}`
	actJSON, e := fSys.ReadFile(jFn)
	assert.Nil(t, e)

	//fmt.Println(actOut)

	// check generated li.json file
	expLI := `{
	"1000": {
		"file": "file.c",
		"Line": 1
	}
}`
	//fmt.Println(string(actJSON))

	assert.Equal(t, expJSON, string(actJSON))
	actLI, e := fSys.ReadFile("li.json")
	assert.Nil(t, e)

	fmt.Println(string(actLI))

	assert.Equal(t, expLI, string(actLI))
}

func TestUpdateLegaacy1(t *testing.T) { // Anti-Virus issue
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create src file
	sFn := "file.c"
	src := ` // this is line 1
  //break; case __LINE__: TRICE8     ( Id(0),      "msg:value=%d\n", -1  ); // user written code with 16-bit stamp
  //break; case __LINE__: TRICE8_1   ( Id(0),      "msg:value=%d\n", -1  ); // user written code with 16-bit stamp
    break; case __LINE__: TRICE8     ( Id( 7009),  "msg:value=%d\n", -14 ); // trice tool modified code for compilation, inlined code, with 16-bit stamp
    break; case __LINE__: TRICE8_1   ( Id( 7009),  "msg:value=%d\n", -15 ); // trice tool modified code for compilation, inlined code, with 16-bit stamp

  //break; case __LINE__: TRICE8     (             "msg:value=%d\n", -1  ); // legacy default with default stamp
  //break; case __LINE__: TRICE8_1   (             "msg:value=%d\n", -1  ); // legacy default with default stamp
  //break; case __LINE__: TRICE8     ( ID(0),      "msg:value=%d\n", -1  ); // user written code with 32-bit stamp
  //break; case __LINE__: TRICE8_1   ( ID(0),      "msg:value=%d\n", -1  ); // user written code with 32-bit stamp
    break; case __LINE__: TRICE8     ( ID( 7009),  "msg:value=%d\n", -14 ); // trice tool modified code for compilation, inlined code, with 32-bit stamp
    break; case __LINE__: TRICE8_1   ( ID( 7009),  "msg:value=%d\n", -15 ); // trice tool modified code for compilation, inlined code, with 32-bit stamp
`
	assert.Nil(t, fSys.WriteFile(sFn, []byte(src), 0777))

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "u", "-src", ".", "-v", "-IDMin", "1000", "-IDMax", "2000", "-IDMethod", "upward", "-defaultStampSize", "0"}))

	// check modified src file
	expSrc := ` // this is line 1
  //break; case __LINE__: TRICE8     ( Id( 1000),      "msg:value=%d\n", -1  ); // user written code with 16-bit stamp
  //break; case __LINE__: TRICE8_1   ( Id( 1001),      "msg:value=%d\n", -1  ); // user written code with 16-bit stamp
    break; case __LINE__: TRICE8     ( Id( 7009),  "msg:value=%d\n", -14 ); // trice tool modified code for compilation, inlined code, with 16-bit stamp
    break; case __LINE__: TRICE8_1   ( Id( 1002),  "msg:value=%d\n", -15 ); // trice tool modified code for compilation, inlined code, with 16-bit stamp

  //break; case __LINE__: TRICE8     ( id( 1003),             "msg:value=%d\n", -1  ); // legacy default with default stamp
  //break; case __LINE__: TRICE8_1   ( id( 1004),             "msg:value=%d\n", -1  ); // legacy default with default stamp
  //break; case __LINE__: TRICE8     ( ID( 1005),      "msg:value=%d\n", -1  ); // user written code with 32-bit stamp
  //break; case __LINE__: TRICE8_1   ( ID( 1006),      "msg:value=%d\n", -1  ); // user written code with 32-bit stamp
    break; case __LINE__: TRICE8     ( ID( 7009),  "msg:value=%d\n", -14 ); // trice tool modified code for compilation, inlined code, with 32-bit stamp
    break; case __LINE__: TRICE8_1   ( ID( 1007),  "msg:value=%d\n", -15 ); // trice tool modified code for compilation, inlined code, with 32-bit stamp
`
	actSrc, e := fSys.ReadFile(sFn)
	assert.Nil(t, e)

	//fmt.Println(string(actSrc))

	assert.Equal(t, expSrc, string(actSrc))

	// check modified til.json file
	expJSON := `{
	"1000": {
		"Type": "TRICE8",
		"Strg": "msg:value=%d\\n"
	},
	"1001": {
		"Type": "TRICE8_1",
		"Strg": "msg:value=%d\\n"
	},
	"1002": {
		"Type": "TRICE8_1",
		"Strg": "msg:value=%d\\n"
	},
	"1003": {
		"Type": "TRICE8",
		"Strg": "msg:value=%d\\n"
	},
	"1004": {
		"Type": "TRICE8_1",
		"Strg": "msg:value=%d\\n"
	},
	"1005": {
		"Type": "TRICE8",
		"Strg": "msg:value=%d\\n"
	},
	"1006": {
		"Type": "TRICE8_1",
		"Strg": "msg:value=%d\\n"
	},
	"1007": {
		"Type": "TRICE8_1",
		"Strg": "msg:value=%d\\n"
	},
	"7009": {
		"Type": "TRICE8",
		"Strg": "msg:value=%d\\n"
	}
}`
	actJSON, e := fSys.ReadFile(jFn)
	assert.Nil(t, e)

	//fmt.Println(string(actJSON))

	assert.Equal(t, expJSON, string(actJSON))

	// check console output
	expOut := `Read ID List file til.json with 0 items.
dir= .
List= til.json
file.c
TRICE8     ( -> TRICE8     ( id(0),
TRICE8_1   ( -> TRICE8_1   ( id(0),
IDMin= 1000 IDMax= 2000 IDMethod= upward
Id(0) -> Id( 1000)
IDMin= 1000 IDMax= 2000 IDMethod= upward
Id(0) -> Id( 1001)
IDMin= 1000 IDMax= 2000 IDMethod= upward
Id( 7009) -> Id( 1002)
IDMin= 1000 IDMax= 2000 IDMethod= upward
id(0) -> id( 1003)
IDMin= 1000 IDMax= 2000 IDMethod= upward
id(0) -> id( 1004)
IDMin= 1000 IDMax= 2000 IDMethod= upward
ID(0) -> ID( 1005)
IDMin= 1000 IDMax= 2000 IDMethod= upward
ID(0) -> ID( 1006)
IDMin= 1000 IDMax= 2000 IDMethod= upward
ID( 7009) -> ID( 1007)
Changed:  file.c
9 ID's in List til.json listModified= true
`
	actOut := b.String()

	//fmt.Println(actOut)

	assert.Equal(t, expOut, actOut)

	// check generated li.json file
	expLI := `{
	"1000": {
		"file": "file.c",
		"Line": 2
	},
	"1001": {
		"file": "file.c",
		"Line": 3
	},
	"1002": {
		"file": "file.c",
		"Line": 5
	},
	"1003": {
		"file": "file.c",
		"Line": 7
	},
	"1004": {
		"file": "file.c",
		"Line": 8
	},
	"1005": {
		"file": "file.c",
		"Line": 9
	},
	"1006": {
		"file": "file.c",
		"Line": 10
	},
	"1007": {
		"file": "file.c",
		"Line": 12
	},
	"7009": {
		"file": "file.c",
		"Line": 11
	}
}`
	actLI, e := fSys.ReadFile("li.json")
	assert.Nil(t, e)

	fmt.Println(string(actLI))

	assert.Equal(t, expLI, string(actLI))
}
