// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package assert_test contains blackbox tests.
package tst_test

import (
	"io/ioutil"
	"os"
	"path/filepath"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"

	"github.com/rokath/trice/pkg/tst"
)

func TestTempFileName(t *testing.T) {
	s := tst.TempFileName("AB_*.CD")
	b := filepath.Base(s)
	tst.AssertTrue(t, strings.HasPrefix(b, "AB_"))
	tst.AssertTrue(t, strings.HasSuffix(b, ".CD"))
	tst.AssertTrue(t, len(s) > 10)
}

func TestEqual(t *testing.T) {
	tst.Equal(t, 33, 33)
}

func TestEqualLines(t *testing.T) {
	exp := "Hello\r\nWorld\r\n"
	act := "Hello\nWorld\n"
	tst.EqualLines(t, exp, act)
}

func TestEqualTextFiles(t *testing.T) {
	fd0, e0 := ioutil.TempFile("", "*.txt")
	assert.Nil(t, e0)
	defer func() {
		assert.Nil(t, fd0.Close())
		assert.Nil(t, os.Remove(fd0.Name()))
	}()

	fd1, e1 := ioutil.TempFile("", "*.txt")
	assert.Nil(t, e1)
	defer func() {
		assert.Nil(t, fd1.Close())
		assert.Nil(t, os.Remove(fd1.Name()))
	}()

	_, e2 := fd0.WriteString("Hello\r\nWorld\r\n")
	assert.Nil(t, e2)
	_, e3 := fd1.WriteString("Hello\nWorld\n")
	assert.Nil(t, e3)
	tst.EqualTextFiles(t, fd0.Name(), fd1.Name())
}

func TestEqualFiles(t *testing.T) {
	tst.EqualLines(t, os.Args[0], os.Args[0])
}
