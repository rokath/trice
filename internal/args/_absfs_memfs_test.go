// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"fmt"
	"testing"

	"github.com/absfs/memfs"
	"github.com/tj/assert"
)

func Test_absfs(t *testing.T) {
	ramFS, _ := memfs.NewFS()

	e := ramFS.MkdirAll("dir1/dir2", 0777)
	assert.Nil(t, e)

	f, e := ramFS.Create("fn")
	assert.Nil(t, e)

	exp := []byte("incinerating-unsubstantial")

	n, e := f.Write(exp)
	assert.Nil(t, e)
	assert.True(t, n == len(exp))
	e = f.Close()
	assert.Nil(t, e)

	f, e = ramFS.Open("fn")
	assert.Nil(t, e)

	act := make([]byte, 2*len(exp))

	n, e = f.Read(act)
	fmt.Println(n, e)
	assert.Nil(t, e)
	assert.True(t, n == len(exp))
	act = act[:n]

	assert.Equal(t, exp, act)

}
