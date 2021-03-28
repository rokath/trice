// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"fmt"
	"os"
	"testing"

	"github.com/rokath/trice/pkg/tst"
	"github.com/tj/assert"
)

func TestMain(m *testing.M) {
	// do stuff before tests
	exitVal := m.Run()
	// do stuff after tests
	os.Exit(exitVal)
}

func Example_doit() {
	version = "1.2.3"
	commit = "myCommit"
	date = "2006-01-02_1504-05"
	os.Args = []string{"trice", "ver"}
	act := tst.CaptureStdOut(doit)
	fmt.Print(act)
	// Output:
	// version=1.2.3, commit=myCommit, built at 2006-01-02_1504-05
}

func TestDoit(t *testing.T) {
	version = "1.2.3"
	commit = "myCommit"
	date = "2006-01-02_1504-05"
	os.Args = []string{"trice", "ver"}
	act := tst.CaptureStdOut(doit)
	exp := "version=1.2.3, commit=myCommit, built at 2006-01-02_1504-05\n"
	assert.Equal(t, exp, act)
}
