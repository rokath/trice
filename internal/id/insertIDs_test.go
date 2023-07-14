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

func TestInsert(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	// create src file
	sFn := "file.c"
	src := `break; case __LINE__: TRice( iD(0), "msg:value=%d\n", -1  );`

	assert.Nil(t, fSys.WriteFile(sFn, []byte(src), 0777))

	// create empty til.json
	jFn := "til.json"
	JSONFile := ``
	assert.Nil(t, fSys.WriteFile(jFn, []byte(JSONFile), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "insert", "-IDMin", "100", "-IDMax", "999", "-IDMethod", "downward"}))

	// check modified src file
	expSrc := `break; case __LINE__: TRice(iD(999), "msg:value=%d\n", -1  );`
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
