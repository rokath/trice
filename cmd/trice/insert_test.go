package main

import (
	"bytes"
	"io"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func TestInsertIDs(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	src := `
    TRice( "Hi!" );  // line 2
`
	// create src file
	sFn := "file.c"
	assert.Nil(t, fSys.WriteFile(sFn, []byte(src), 0777))

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-src", ".", "-IDMin", "1000", "-IDMethod", "upward"}))

	// check source file
	expSrc := `
    TRice( iD( 1000), "Hi!" );  // line 2
`
	actSrc, e := fSys.ReadFile(sFn)
	assert.Nil(t, e)
	//fmt.Println(string(actSrc))
	assert.Equal(t, expSrc, string(actSrc))

	// check til.json
	expJSON := `{
	"1000": {
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
		"file": "file.c",
		"Line": 2
	}
}`
	actLI, e := fSys.ReadFile("li.json")
	assert.Nil(t, e)
	//fmt.Println(string(actLI))
	assert.Equal(t, expLI, string(actLI))
}
