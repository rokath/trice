// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

import (
	"io/ioutil"
	"os"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/msg"

	"github.com/rokath/trice/pkg/tst"
	"github.com/stretchr/testify/assert"
)

var (
	m *sync.RWMutex // var verbose is used differently in tests, so there parallel execution must be avoided
	h *sync.RWMutex // help vars are used differently in tests, so there parallel execution must be avoided
)

func init() {
	m = new(sync.RWMutex)
	h = new(sync.RWMutex)
}

func TestMain(m *testing.M) {
	id.FnJSON = getTemporaryFileName("til-*.JSON")
	code := m.Run()
	os.Remove(id.FnJSON) // // os.Exit() does not respect defer statements
	os.Exit(code)
}

func testVersion(t *testing.T, v []string) {
	fi, err := os.Stat(os.Args[0])
	assert.Nil(t, err)
	buildTime := fi.ModTime().String()
	exp := v[0] + "version=devel, built " + buildTime + "\n" + v[1]

	fn := func() {
		msg.OnErr(Handler([]string{"trice", "ver"}))
	}
	act := tst.CaptureStdOut(fn)
	assert.Equal(t, exp, act)

	fn = func() {
		msg.OnErr(Handler([]string{"trice", "version"}))
	}
	act = tst.CaptureStdOut(fn)
	assert.Equal(t, exp, act)
}

func getTemporaryFileName(pattern string) string {
	tempFileHandle, e := ioutil.TempFile(os.TempDir(), pattern) // opens for read and write
	msg.OnErr(e)
	tempFileName := tempFileHandle.Name()
	msg.OnErr(tempFileHandle.Close())
	return tempFileName
}
