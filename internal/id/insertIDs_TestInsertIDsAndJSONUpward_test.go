package id_test

// The tests in this file cannot run concurrently!

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func TestInsertIDsAndJSONUpwardXXX(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create empty til.json
	id.FnJSON = t.Name() + "til.json"
	assert.Nil(t, fSys.WriteFile(id.FnJSON, []byte(``), 0777))

	// create empty li.json
	id.LIFnJSON = t.Name() + "li.json"
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(``), 0777))

	id.Min = 10 Diese Teile werden nicht richtig initialisiert!
	id.Max = 20

	id.IDData.PreProcessing(os.Stdout, fSys)

	fmt.Println(id.IDData)
	assert.Fail(t, "view")
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
