// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package tst provides some helper functions for testing.
package tst

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"reflect"
	"runtime"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"

	"github.com/udhos/equalfile"
)

// CaptureStdOut captures stdout and returns it as string.
func CaptureStdOut(f func()) string {

	// keep backup of the real stdout
	old := os.Stdout

	// re-direct stdout
	r, w, err := os.Pipe()
	if nil != err {
		return err.Error()
	}
	os.Stdout = w
	defer func() {
		// restoring the real stdout
		os.Stdout = old
	}()

	// copy the output in a separate goroutine so printing can't block indefinitely
	outC := make(chan string)
	go func() {
		var buf bytes.Buffer
		io.Copy(&buf, r)
		outC <- buf.String()
	}()

	// run the function
	f()

	// back to normal state
	err = w.Close()
	if nil != err {
		return err.Error()
	}

	// read output
	return <-outC
}

// EqualStrings fails test if strings not identical.
func EqualStrings(t *testing.T, exp, act string) {
	if strings.Compare(act, exp) != 0 {
		log.Println("")
		log.Println("expect:", exp)
		log.Println("actual:", act)
		t.Fail()
	}
}

// Equal fails the test if exp is not equal to act.
func Equal(tb testing.TB, exp, act interface{}) {
	if !reflect.DeepEqual(exp, act) {
		_, file, line, _ := runtime.Caller(1)
		log.Println("expect:", exp)
		log.Println("actual:", act)
		es := fmt.Sprintf("%v", exp)
		as := fmt.Sprintf("%v", act)
		log.Println("expect:", es)
		log.Println("actual:", as)
		log.Println("expect:", []byte(es))
		log.Println("actual:", []byte(as))
		fmt.Println(filepath.Base(file), line)
		tb.FailNow()
	}
}

// EqualLines compares exp and act line by line ignoring lineendings and leading/trailing spaces.
func EqualLines(tb testing.TB, exp, act string) {

	// remove windows line endings
	exp0 := strings.ReplaceAll(exp, "\r\n", "\n")
	act0 := strings.ReplaceAll(act, "\r\n", "\n")

	exp1 := strings.Split(exp0, "\n")
	act1 := strings.Split(act0, "\n")

	if len(exp1) != len(act1) {
		fmt.Println(len(exp1), len(act1))
		tb.Fail()
	}
	for i, se := range exp1 {
		expLine := strings.TrimSpace(se)
		actLine := strings.TrimSpace(act1[i])
		Equal(tb, expLine, actLine)
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

// EqualFiles fails test if contence is NOT equal
func EqualFiles(t *testing.T, fn0, fn1 string) {
	ok := equalFileContent(fn0, fn1)
	if false == ok {
		t.FailNow()
	}
}

// equalFileContent returns true if contece is equal
func equalFileContent(fn0, fn1 string) bool {
	cmp := equalfile.New(nil, equalfile.Options{}) // compare using single mode
	ok, err := cmp.CompareFile(fn0, fn1)
	if nil != err {
		ok = false
	}
	return ok
}

// AssertNoErr fails the test if err is not nil.
func AssertNoErr(tb testing.TB, err error) {
	if err != nil {
		_, file, line, _ := runtime.Caller(1)
		fmt.Println(err.Error(), filepath.Base(file), line)
		tb.FailNow()
	}
}

// AssertTrue fails the test if flag is false.
func AssertTrue(tb testing.TB, flag bool) {
	if !flag {
		_, file, line, _ := runtime.Caller(1)
		fmt.Println(flag, filepath.Base(file), line)
		tb.FailNow()
	}
}
