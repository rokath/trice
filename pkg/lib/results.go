// Package tst contains no runtime code, only universal helper for tests
package lib

import (
	"fmt"
	"path/filepath"
	"reflect"
	"runtime"
	"testing"

	"github.com/udhos/equalfile"
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
		//_, file, line, _ := runtime.Caller(1)
		//fmt.Printf("\033[31m%s:%d:\n\n\texp: %#v\n\n\tgot: %#v\033[39m\n\n", filepath.Base(file), line, exp, act)
		tb.FailNow()
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// EqualFileContent returns true if contece is equal
func EqualFileContent(fn0, fn1 string) bool {
	cmp := equalfile.New(nil, equalfile.Options{}) // compare using single mode
	ok, err := cmp.CompareFile(fn0, fn1)
	if nil != err {
		ok = false
	}
	return ok
}

// EqualFiles fails test if contence is NOT equal
func EqualFiles(t *testing.T, fn0, fn1 string) {
	ok := EqualFileContent(fn0, fn1)
	if false == ok {
		t.FailNow()
	}
}
