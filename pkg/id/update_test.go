// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

package id

import (
	"io/ioutil"
	"math/rand"
	"os"
	"testing"
)

func doUpdate(t *testing.T, path string) {
	pwd, err := os.Getwd()
	ok(t, err)
	path = pwd + path
	a := path + "act/"
	e := path + "exp/"

	err = os.RemoveAll(a)
	ok(t, err)
	err = CopyDir(path+"ori/", a)
	ok(t, err)

	fa := a + "til.json"
	fe := e + "til.json"

	var lp, lq, lw List
	var p, q, w = &lp, &lq, &lw

	rand.Seed(0)
	err = w.Update(a, fa, true, true)
	ok(t, err)
	w.zeroTimestampCreated()
	err = w.write(fa)
	ok(t, err)

	err = p.Read(fa)
	ok(t, err)
	err = q.Read(fe)
	ok(t, err)

	act, err := ioutil.ReadFile(a + "Data.c")
	ok(t, err)
	exp, err := ioutil.ReadFile(e + "Data.c")
	ok(t, err)
	equals(t, exp, act)

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
