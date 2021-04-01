// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package tst provides some helper functions for testing.
package tst

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"strings"
	"testing"

	"github.com/rokath/trice/pkg/msg"

	"github.com/stretchr/testify/assert"

	"github.com/udhos/equalfile"
)

// TempFileName returns a temporary file name based on pattern.
// The file is created and deleted and only the name is delivered.
// The pattern should contain at least an asterics. Example: myFile-*.bin"
func TempFileName(pattern string) (s string) {
	tempFileHandle, e := ioutil.TempFile(os.TempDir(), pattern) // opens for read and write
	msg.OnErr(e)
	s = tempFileHandle.Name()
	msg.OnErr(tempFileHandle.Close())
	msg.InfoOnErr(os.Remove(s), s)
	return
}

// CaptureStdOut captures stdout and returns it as string.
func CaptureStdOut(f func()) string {

	// keep backup of the real stdout
	old := os.Stdout

	// re-direct stdout
	r, w, err := os.Pipe()
	msg.FatalOnErr(err)

	os.Stdout = w
	defer func() {
		// restoring the real stdout
		os.Stdout = old
	}()

	// copy the output in a separate goroutine so printing can't block indefinitely
	outC := make(chan string)
	go func() {
		var buf bytes.Buffer
		_, err = io.Copy(&buf, r)
		msg.OnErr(err)
		outC <- buf.String()
	}()

	// run the function
	f()

	// back to normal state
	msg.FatalOnErr(w.Close())

	// read output
	return <-outC
}

// EqualLines compares exp and act line by line ignoring lineendings and leading/trailing spaces.
func EqualLines(tb testing.TB, exp, act string) {

	// remove windows line endings
	exp0 := strings.ReplaceAll(exp, "\r", "")
	act0 := strings.ReplaceAll(act, "\r", "")
	exp1 := strings.ReplaceAll(exp0, "\t", "")
	act1 := strings.ReplaceAll(act0, "\t", "")
	exp2 := strings.ReplaceAll(exp1, " ", "")
	act2 := strings.ReplaceAll(act1, " ", "")

	expS := strings.Split(exp2, "\n")
	actS := strings.Split(act2, "\n")

	fmt.Println(len(expS), len(actS))
	//assert.True(tb, len(expS) == len(actS))

	for i := range expS {
		if expS[i] != actS[i] {
			fmt.Println(i, "expLine:"+expS[i])
			fmt.Println(i, "actLine:"+actS[i])
			tb.Fail()
			return
		}
	}
}

// EqualTextFiles fails test if lines in fn0 & fn1 NOT equal.
// Line endings are ignored
func EqualTextFiles(t *testing.T, fn0, fn1 string) {

	// Read entire file content, giving us little control but
	// making it very simple. No need to close the file.
	b0, e0 := ioutil.ReadFile(fn0)
	assert.Nil(t, e0)
	b1, e1 := ioutil.ReadFile(fn1)
	assert.Nil(t, e1)

	// Convert []byte to string and print to screen
	s0 := string(b0)
	s1 := string(b1)

	EqualLines(t, s0, s1)
}

// AssertEqualFiles fails test if contence is NOT equal
func AssertEqualFiles(t *testing.T, fn0, fn1 string) {
	cmp := equalfile.New(nil, equalfile.Options{}) // compare using single mode
	ok, err := cmp.CompareFile(fn0, fn1)
	assert.Nil(t, err)
	assert.True(t, ok)
}
