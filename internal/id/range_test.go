// Copyright 2024 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import (
	"testing"

	"github.com/rokath/trice/internal/args"
	. "github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

// source tree management

func TestRange0(t *testing.T) {
	defer Setup(t)()

	// create src file
	sFn := t.Name() + "file.c"
	src := `trice("msg:Hi\n");trice("err:Alarm!\n");trice("msg:Lo\n");`
	assert.Nil(t, FSys.WriteFile(sFn, []byte(src), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-IDMethod", "upward", "-src", sFn, "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file
	expSrc := `trice(iD(1000), "msg:Hi\n");trice(iD(1001), "err:Alarm!\n");trice(iD(1002), "msg:Lo\n");`

	actSrc, e := FSys.ReadFile(sFn)
	assert.Nil(t, e)

	assert.Equal(t, expSrc, string(actSrc))
}

func TestRange1(t *testing.T) {
	defer Setup(t)()

	// create src file
	sFn := t.Name() + "file.c"
	src := `trice("msg:Hi\n"); ... trice("err:Alarm!\n"); ... trice("msg:Lo\n"); ... trice("d:foo\n");`
	assert.Nil(t, FSys.WriteFile(sFn, []byte(src), 0777))

	// action
	assert.Nil(t, args.Handler(W, FSys, []string{"trice", "insert", "-IDRange", "e:100,102", "-IDRange", "dbg:200,203", "-IDMax=1005", "-IDMethod", "upward", "-src", sFn, "-til", FnJSON, "-li", LIFnJSON}))

	// check modified src file
	expSrc := `trice(iD(1000), "msg:Hi\n"); ... trice(iD(100), "err:Alarm!\n"); ... trice(iD(1001), "msg:Lo\n"); ... trice(iD(200), "d:foo\n");`

	actSrc, e := FSys.ReadFile(sFn)
	assert.Nil(t, e)

	assert.Equal(t, expSrc, string(actSrc))
}
