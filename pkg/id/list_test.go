// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id

import (
	"io/ioutil"
	"os"
	"testing"
)

func (p *ListT) zeroTimestampCreated() {
	for i := range *p {
		(*p)[i].Created = 0
	}
}

func Test_write(t *testing.T) {
	wd, err := os.Getwd()
	ok(t, err)
	fa := wd + "/testdata/BasicFunctions/writeAct.json"
	fe := wd + "/testdata/BasicFunctions/writeExp.json"
	var la, le ListT
	p, q := &la, &le
	p.extendIdList(12345, "TRICE0", "Hi", true)
	p.zeroTimestampCreated()
	p.write(fa)
	q.Read(fe)
	equals(t, p, q)
	os.Remove(fa)
}

func TestZeroSourceTreeIds(t *testing.T) {
	pwd, err := os.Getwd()
	ok(t, err)
	path := pwd + "/testdata/Zero/"
	a := path + "act/"
	e := path + "exp/"

	err = os.RemoveAll(a)
	ok(t, err)
	err = CopyDir(path+"ori/", a)
	ok(t, err)

	zeroSourceTreeIds(a, true)

	act, err := ioutil.ReadFile(a + "Data.c")
	ok(t, err)
	exp, err := ioutil.ReadFile(e + "Data.c")
	ok(t, err)
	equals(t, exp, act)
	os.RemoveAll(a)
}

func Test_appendItem(t *testing.T) {
	pwd, err := os.Getwd()
	ok(t, err)
	path := pwd + "/testdata/appendID/"
	da := path + "act/"
	fa := path + "act/til.json"
	fe := path + "exp/til.json"

	os.RemoveAll(da)
	os.Mkdir(da, 0644)

	var lp, lq, ls, lr ListT
	var p, q, s, r = &lp, &lq, &ls, &lr

	s.extendIdList(123, "TRICE0", "some logstring", true)
	s.extendIdList(4444, "TRICE32_1", "some other %d logstring", true)
	err = s.write(fa)
	ok(t, err)

	err = r.Read(fa)

	ok(t, err)
	r.extendIdList(55, "TRICE8_7", "some more %d %d %d %d %d %d %d logstring", true)
	r.zeroTimestampCreated()
	err = r.write(fa)
	ok(t, err)

	err = p.Read(fa)
	ok(t, err)
	err = q.Read(fe)
	ok(t, err)

	equals(t, p, q)
	os.RemoveAll(da)
}
