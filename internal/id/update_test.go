// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import (
	"io/ioutil"
	"math/rand"
	"os"
	"testing"

	"github.com/stretchr/testify/assert"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/tst"
)

func doUpdate1(t *testing.T, sOri, sExp []string, listExp string) {
	rand.Seed(0)
	p := id.NewList(randomFile("{}", "", "til*.json"))

	dir0, err := ioutil.TempDir("", "*")
	assert.Nil(t, err)

	n0 := randomFile(sOri[0], dir0, "*.c")
	err = p.Update(dir0, true, true)
	assert.Nil(t, err)

	p.ZeroTimestampCreated()
	p.WriteListFile()

	listAct := readFileAsString(p.FnJSON)
	tst.EqualLines(t, listExp, listAct)

	sAct := make([]string, 0, 1)
	sAct = append(sAct, readFileAsString(n0))

	for i := range sExp {
		tst.EqualLines(t, sExp[i], sAct[i])
	}

	assert.Nil(t, os.RemoveAll(dir0))
	assert.Nil(t, os.RemoveAll(p.FnJSON))
}

func doUpdate(t *testing.T, sOri, sExp []string, listExp string) {

	rand.Seed(0)
	p := id.NewList(randomFile("{}", "", "til*.json"))

	dir0, err := ioutil.TempDir("", "*")
	assert.Nil(t, err)
	dir1, err := ioutil.TempDir(dir0, "*")
	assert.Nil(t, err)
	dir2, err := ioutil.TempDir(dir0, "*")
	assert.Nil(t, err)
	dir3, err := ioutil.TempDir(dir1, "*")
	assert.Nil(t, err)

	n0 := randomFile(sOri[0], dir1, "*.c")
	err = p.Update(dir0, true, true)
	assert.Nil(t, err)

	n1 := randomFile(sOri[1], dir2, "*.cpp")
	err = p.Update(dir0, true, true)
	assert.Nil(t, err)

	n2 := randomFile(sOri[2], dir2, "*.cc")
	err = p.Update(dir0, true, true)
	assert.Nil(t, err)

	n3 := randomFile(sOri[3], dir3, "*.c")
	err = p.Update(dir0, true, true)
	assert.Nil(t, err)

	p.ZeroTimestampCreated()
	p.WriteListFile()

	listAct := readFileAsString(p.FnJSON)
	tst.EqualLines(t, listExp, listAct)

	sAct := make([]string, 0, 4)
	sAct = append(sAct, readFileAsString(n0))
	sAct = append(sAct, readFileAsString(n1))
	sAct = append(sAct, readFileAsString(n2))
	sAct = append(sAct, readFileAsString(n3))

	for i := range sExp {
		tst.EqualLines(t, sExp[i], sAct[i])
	}

	assert.Nil(t, os.RemoveAll(dir0))
	assert.Nil(t, os.RemoveAll(p.FnJSON))
}

// todo
func TestUpdatePrintfAdapter(t *testing.T) {

	sOri := []string{`

		`, `

		`, `

		`, `

		`}

	sExp := []string{`

		`, `

		`, `

		`, `

		`}

	listExp := `[]`
	doUpdate(t, sOri, sExp, listExp)
}
