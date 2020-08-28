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
	errorFail(t, err)
	path = pwd + path
	a := path + "act/"
	e := path + "exp/"

	err = os.RemoveAll(a)
	errorFail(t, err)
	err = copyDir(path+"ori/", a)
	errorFail(t, err)

	fa := a + "til.json"
	fe := e + "til.json"

	var lp, lq, lw ListT
	var p, q, w = &lp, &lq, &lw

	rand.Seed(0)
	err = w.Update(a, fa, true, true)
	errorFail(t, err)
	w.zeroTimestampCreated()
	err = w.write(fa)
	errorFail(t, err)

	err = p.Read(fa)
	errorFail(t, err)
	err = q.Read(fe)
	errorFail(t, err)

	act, err := ioutil.ReadFile(a + "Data.c")
	errorFail(t, err)
	exp, err := ioutil.ReadFile(e + "Data.c")
	errorFail(t, err)
	notEqualFail(t, exp, act)

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
