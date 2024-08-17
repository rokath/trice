// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
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

func TestZeroIDs(t *testing.T) {

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer id.SetupTest(t, fSys)()

	// create src file
	sFn := t.Name() + "file.c"
	src := `break; case __LINE__: trice( iD(999), "msg:value=%d\n", -1  );`

	assert.Nil(t, fSys.WriteFile(sFn, []byte(src), 0777))

	// action
	var b bytes.Buffer
	assert.Nil(t, args.Handler(io.Writer(&b), fSys, []string{"trice", "zero", "-til", id.FnJSON, "-li", id.LIFnJSON}))

	// check modified src file
	expSrc := `break; case __LINE__: trice( iD(0), "msg:value=%d\n", -1  );`

	actSrc, e := fSys.ReadFile(sFn)
	assert.Nil(t, e)

	assert.Equal(t, expSrc, string(actSrc))
}
