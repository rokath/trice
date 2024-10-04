// Copyright 2020 Thomas.Hoehenleitner [at] seerose.netid_test
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import (
	"bytes"
	"io"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

func TestClean(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	// create src file
	sFn := t.Name() + "file.c"
	src := `break; case __LINE__: trice(iD(999), "msg:value=%d\n", -1  );`
	assert.Nil(t, fSys.WriteFile(sFn, []byte(src), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-til", id.FnJSON, "-li", id.LIFnJSON}))

	// check modified src file
	expSrc := `break; case __LINE__: trice("msg:value=%d\n", -1  );`

	actSrc, e := fSys.ReadFile(sFn)
	assert.Nil(t, e)

	assert.Equal(t, expSrc, string(actSrc))
}

func TestCleanWithLIExtension(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	// create non-empty li.json
	li := `{
	"16200": {
		"File": "stm32f0xx_it.c",
		"Line": 160
	},
	"16202": {
		"File": "main.c",
		"Line": 71
	}
}`
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, []byte(li), 0777))

	// create src file
	sFn := t.Name() + "file.c"
	src := `break; case __LINE__: trice(iD(999), "msg:value=%d\n", -1  );`
	assert.Nil(t, fSys.WriteFile(sFn, []byte(src), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "clean", "-til", id.FnJSON, "-li", id.LIFnJSON}))

	// check modified src file
	expSrc := `break; case __LINE__: trice("msg:value=%d\n", -1  );`

	actSrc, e := fSys.ReadFile(sFn)
	assert.Nil(t, e)
	assert.Equal(t, expSrc, string(actSrc))

	// check extended location indormation file
	expLI := `{
	"16200": {
		"File": "stm32f0xx_it.c",
		"Line": 160
	},
	"16202": {
		"File": "main.c",
		"Line": 71
	},
	"999": {
		"File": "TestCleanWithLIExtensionfile.c",
		"Line": 1
	}
}`
	actLI, e := fSys.ReadFile(id.LIFnJSON)
	assert.Nil(t, e)
	assert.Equal(t, expLI, string(actLI))
}
