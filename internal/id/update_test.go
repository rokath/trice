// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package id_test

import (
	"github.com/stretchr/testify/assert"
	"io/ioutil"
	"math/rand"
	"os"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/assert2"
	"github.com/rokath/trice/pkg/file"
)

func doUpdate(t *testing.T, sOri, sExp []string, listExp string) {

	rand.Seed(0)
	p := id.NewList(file.Random("{}", "", "til*.json"))

	dir0, err := ioutil.TempDir("", "*")
	assert.Nil(t, err)
	dir1, err := ioutil.TempDir(dir0, "*")
	assert.Nil(t, err)
	dir2, err := ioutil.TempDir(dir0, "*")
	assert.Nil(t, err)
	dir3, err := ioutil.TempDir(dir1, "*")
	assert.Nil(t, err)

	n0 := file.Random(sOri[0], dir1, "*.c")
	err = p.Update(dir0, true, true)
	assert.Nil(t, err)

	n1 := file.Random(sOri[1], dir2, "*.cpp")
	err = p.Update(dir0, true, true)
	assert.Nil(t, err)

	n2 := file.Random(sOri[2], dir2, "*.cc")
	err = p.Update(dir0, true, true)
	assert.Nil(t, err)

	n3 := file.Random(sOri[3], dir3, "*.c")
	err = p.Update(dir0, true, true)
	assert.Nil(t, err)

	p.ZeroTimestampCreated()
	p.WriteListFile()

	listAct := file.ReadString(p.FnJSON)
	assert2.EqualLines(t, listExp, listAct)

	sAct := make([]string, 0, 4)
	sAct = append(sAct, file.ReadString(n0))
	sAct = append(sAct, file.ReadString(n1))
	sAct = append(sAct, file.ReadString(n2))
	sAct = append(sAct, file.ReadString(n3))

	for i := range sExp {
		assert2.EqualLines(t, sExp[i], sAct[i])
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
