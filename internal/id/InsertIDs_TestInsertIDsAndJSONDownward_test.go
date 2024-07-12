package id_test

// The tests in this file cannot run concurrently!

import (
	"bytes"
	"fmt"
	"io"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func _TestInsertIDsAndJSONDownward(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create src files
	src0 := `
	TRice( "Lo!" );
	TRice( "Hi!" );
	`
	fn0 := t.Name() + "file0.c"
	assert.Nil(t, fSys.WriteFile(fn0, []byte(src0), 0777))

	// create empty til.json
	tilFn := t.Name() + "til.json"
	assert.Nil(t, fSys.WriteFile(tilFn, []byte(``), 0777))

	// create empty li.json
	liFn := t.Name() + "li.json"
	assert.Nil(t, fSys.WriteFile(liFn, []byte(``), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"TRICE", "insert", "-src", ".", "-til", tilFn, "-li", liFn, "-IDMin", "10", "-IDMax", "20", "-IDMethod", "downward"}))

	// check source files
	expSrc0 := `
	TRice( iD(20), "Lo!" );
	TRice( iD(19), "Hi!" );
	`
	actSrc0, e := fSys.ReadFile(fn0)
	assert.Nil(t, e)
	fmt.Println(string(actSrc0))
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
	actJSON, e := fSys.ReadFile(tilFn)
	assert.Nil(t, e)
	fmt.Println(string(actJSON))
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
	actLI, e := fSys.ReadFile(liFn)
	assert.Nil(t, e)
	fmt.Println(string(actLI))
	assert.Equal(t, expLI, string(actLI))

}
