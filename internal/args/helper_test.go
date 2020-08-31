// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package args

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

// assertNil fails the test if an err is not nil.
func assertNil(tb testing.TB, err error) {
	if err != nil {
		_, file, line, _ := runtime.Caller(1)
		fmt.Println(err.Error(), filepath.Base(file), line)
		tb.FailNow()
	}
}

// assertEqual fails the test if exp is not equal to act.
func assertEqual(tb testing.TB, exp, act interface{}) {
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

// assertEqualLines compares exp and act line by line ignoring lineendings and leading/trailing spaces.
func assertEqualLines(tb testing.TB, exp, act string) {

	// remove windows line endings
	exp0 := strings.ReplaceAll(exp, "\r\n", "\n")
	act0 := strings.ReplaceAll(act, "\r\n", "\n")

	exp1 := strings.Split(exp0, "\r")
	act1 := strings.Split(act0, "\r")

	if len(exp1) != len(act1) {
		fmt.Println(len(exp1), len(act1))
		tb.Fail()
	}
	for i, se := range exp1 {
		expLine := strings.TrimSpace(se)
		actLine := strings.TrimSpace(act1[i])
		assertEqual(tb, expLine, actLine)
	}
}

// assertEqualTextFiles compares 2 test files ignoring different line endings
func assertEqualTextFiles(t *testing.T, fn0, fn1 string) {
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
