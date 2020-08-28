package cage_test

import (
	"fmt"
	"path/filepath"
	"runtime"
	"testing"

	"github.com/udhos/equalfile"
)

// test helper ///////////////////////////////////////////////////////////////////////
//

// errorFail fails the test if an err is not nil.
func errorFail(tb testing.TB, err error) {
	if err != nil {
		_, file, line, _ := runtime.Caller(1)
		fmt.Println(err.Error(), filepath.Base(file), line)
		tb.FailNow()
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

// equalFiles fails test if contence is NOT equal
func equalFiles(t *testing.T, fn0, fn1 string) {
	ok := equalFileContent(fn0, fn1)
	if false == ok {
		t.FailNow()
	}
}

//
// test helper ///////////////////////////////////////////////////////////////////////
