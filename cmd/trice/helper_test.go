// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"path/filepath"
	"reflect"
	"runtime"
	"strings"
	"testing"
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

// notEqualFail fails the test if exp is not equal to act.
func notEqualFail(tb testing.TB, exp, act interface{}) {
	if !reflect.DeepEqual(exp, act) {
		_, file, line, _ := runtime.Caller(1)
		log.Println("expect:", exp)
		log.Println("actual:", act)
		fmt.Println(filepath.Base(file), line)
		tb.FailNow()
	}
}

// notEqualTextFilesFail compares 2 test files ignoring different line endings
func notEqualTextFilesFail(t *testing.T, fn0, fn1 string) {
	if !equalTextfiles(fn0, fn1) {
		t.Fail()
	}
}

//
// test helper ///////////////////////////////////////////////////////////////////////

// equalTextfiles returns true if file content is the same.
// Different line endings are ignored.
func equalTextfiles(fn0, fn1 string) bool {
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
