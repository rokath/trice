// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

//blackbox test
package id_test

import (
	"io/ioutil"
	"os"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/assert"
	"github.com/rokath/trice/pkg/file"
)

func TestWrite(t *testing.T) {
	fd, err := ioutil.TempFile("", "*.json")
	assert.ErrorNil(t, err)
	fa := fd.Name()
	fd.Close()
	p := id.NewList(fa)
	p.WriteListFile()
	p.ExtendIDList(12345, "TRICE0", "Hi", true)
	p.ZeroTimestampCreated()
	p.WriteListFile()
	assert.EqualTextFiles(t, "testdata/BasicFunctions/writeExp.json", fa)
}

func TestZeroSourceTreeIds(t *testing.T) {
	pwd, err := os.Getwd()
	assert.ErrorNil(t, err)
	path := pwd + "/testdata/Zero/"
	a := path + "act/"
	e := path + "exp/"

	assert.ErrorNil(t, os.RemoveAll(a))
	assert.ErrorNil(t, file.CopyDir(path+"ori/", a))

	id.ZeroSourceTreeIds(a, true)

	act, err := ioutil.ReadFile(a + "Data.c")
	assert.ErrorNil(t, err)
	exp, err := ioutil.ReadFile(e + "Data.c")
	assert.ErrorNil(t, err)
	assert.Equal(t, exp, act)
	os.RemoveAll(a)
}

func TestAppendItem(t *testing.T) {
	fd, err := ioutil.TempFile("", "*.json")
	assert.ErrorNil(t, err)
	fa := fd.Name()
	fd.Close()

	// create file
	p := id.NewList(fa)
	p.ExtendIDList(123, "TRICE0", "some logstring", true)
	p.ExtendIDList(4444, "TRICE32_1", "some other %d logstring", true)
	p.WriteListFile()

	// extend file
	p = id.NewList(fa)
	p.ReadListFile()
	p.ExtendIDList(55, "TRICE8_7", "some more %d %d %d %d %d %d %d logstring", true)
	p.ZeroTimestampCreated()
	p.WriteListFile()

	assert.EqualTextFiles(t, "testdata/appendID/exp/til.json", fa)
}
