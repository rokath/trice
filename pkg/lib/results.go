// Package tst contains no runtime code, only universal helper for tests
package lib

import (
	"fmt"
	"io/ioutil"
	"path/filepath"
	"reflect"
	"runtime"
	"strings"
	"testing"
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// https://github.com/benbjohnson/testing
//

// Assert fails the test if the condition is false.
func Assert(tb testing.TB, condition bool, msg string, v ...interface{}) {
	if !condition {
		_, file, line, _ := runtime.Caller(1)
		fmt.Printf("\033[31m%s:%d: "+msg+"\033[39m\n\n", append([]interface{}{filepath.Base(file), line}, v...)...)
		tb.FailNow()
	}
}

// Ok fails the test if an err is not nil.
func Ok(tb testing.TB, err error) {
	if err != nil {
		_, file, line, _ := runtime.Caller(1)
		fmt.Printf("\033[31m%s:%d: unexpected error: %s\033[39m\n\n", filepath.Base(file), line, err.Error())
		tb.FailNow()
	}
}

// Equals fails the test if exp is not equal to act.
func Equals(tb testing.TB, exp, act interface{}) {
	if !reflect.DeepEqual(exp, act) {
		_, file, line, _ := runtime.Caller(1)
		fmt.Printf("\033[31m%s:%d:\n\n\texp: %#v\n\n\tgot: %#v\033[39m\n\n", filepath.Base(file), line, exp, act)
		tb.FailNow()
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

func equalTestfiles(fn0, fn1 string) bool {
	s0, _ := ioutil.ReadFile(fn0)
	s1, _ := ioutil.ReadFile(fn1)
	var x = strings.NewReplacer("\r\n", "\n")
	x0 := x.Replace(string(s0))
	x1 := x.Replace(string(s1))
	if strings.Compare(x0, x1) == 0 {
		return true
	}
	return false
}

// EqualTextFiles compares 2 test files ignoring different line endings
func EqualTextFiles(t *testing.T, fn0, fn1 string) {
	if !equalTestfiles(fn0, fn1) {
		t.Fail()
	}
}

var replacement = "<br>\n"

var replacer = strings.NewReplacer(
	"\r\n", replacement,
	"\r", replacement,
	"\n", replacement,
	"\v", replacement,
	"\f", replacement,
	"\u0085", replacement,
	"\u2028", replacement,
	"\u2029", replacement,
)

func replaceReplacer(s string) string {
	return replacer.Replace(s)
}

// EqualFiles2 fails test if contence is NOT equal
// Different line endings are ignored
func EqualFiles2(t *testing.T, fn0, fn1 string) {
	b0, err := ioutil.ReadFile(fn0)
	Ok(t, err)
	b1, err := ioutil.ReadFile(fn1)
	Ok(t, err)
	s0 := replaceReplacer(string(b0))
	s1 := replaceReplacer(string(b1))
	Equals(t, s0, s1)
}

// EqualFile fails test if s is NOT equal to contence of fn
// Different lineendings are ignored
func EqualFile(t *testing.T, fn, s string) {
	b, err := ioutil.ReadFile(fn)
	Ok(t, err)
	s0 := replaceReplacer(string(b))
	s1 := replaceReplacer(s)
	Equals(t, s0, s1)
}
