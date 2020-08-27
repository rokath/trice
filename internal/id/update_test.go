// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"io/ioutil"
	"math/rand"
	"os"
	"testing"

	"github.com/rokath/trice/pkg/lib"
)

func doUpdate(t *testing.T, path string) {
	pwd, err := os.Getwd()
	lib.Ok(t, err)
	path = pwd + path
	a := path + "act/"
	e := path + "exp/"

	err = os.RemoveAll(a)
	lib.Ok(t, err)
	err = copyDir(path+"ori/", a)
	lib.Ok(t, err)

	fa := a + "til.json"
	fe := e + "til.json"

	var lp, lq, lw ListT
	var p, q, w = &lp, &lq, &lw

	rand.Seed(0)
	err = w.Update(a, fa, true, true)
	lib.Ok(t, err)
	w.zeroTimestampCreated()
	err = w.write(fa)
	lib.Ok(t, err)

	err = p.Read(fa)
	lib.Ok(t, err)
	err = q.Read(fe)
	lib.Ok(t, err)

	act, err := ioutil.ReadFile(a + "Data.c")
	lib.Ok(t, err)
	exp, err := ioutil.ReadFile(e + "Data.c")
	lib.Ok(t, err)
	lib.Equals(t, exp, act)

	os.RemoveAll(a)
}

func TestUpdateZeroId(t *testing.T) {
	doUpdate(t, "/testdata/UpdateZeroId/")
}

func TestUpdateAllEqual(t *testing.T) {
	doUpdate(t, "/testdata/UpdateAllEqual/")
}

func TestUpdateSameId(t *testing.T) {
	doUpdate(t, "/testdata/UpdateSameId/")
}

func TestUpdatePrintfAdapter(t *testing.T) {
	doUpdate(t, "/testdata/AutoParamCount/")
}
