// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"io/ioutil"
	"math/rand"
	"os"
	"testing"
)

func doUpdate(t *testing.T, path string) {
	pwd, err := os.Getwd()
	assertNil(t, err)
	path = pwd + path
	a := path + "act/"
	e := path + "exp/"

	err = os.RemoveAll(a)
	assertNil(t, err)
	err = copyDir(path+"ori/", a)
	assertNil(t, err)

	fa := a + "til.json"
	fe := e + "til.json"

	var lp, lq, lw ListT
	var p, q, w = &lp, &lq, &lw

	rand.Seed(0)
	err = w.Update(a, fa, true, true)
	assertNil(t, err)
	w.zeroTimestampCreated()
	err = w.write(fa)
	assertNil(t, err)

	err = p.Read(fa)
	assertNil(t, err)
	err = q.Read(fe)
	assertNil(t, err)

	act, err := ioutil.ReadFile(a + "Data.c")
	assertNil(t, err)
	exp, err := ioutil.ReadFile(e + "Data.c")
	assertNil(t, err)
	assertEqual(t, exp, act)

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
