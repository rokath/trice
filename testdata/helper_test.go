// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// small helper functions for quick copy into packages code
package ...

import (
	"fmt"
	"io/ioutil"
	"log"
	"math/rand"
	"path/filepath"
	"reflect"
	"runtime"
	"strings"
	"testing"
	"time"
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

// notEqualFilesFail fails test if contence is NOT equal
func notEqualFilesFail(t *testing.T, fn0, fn1 string) {
	ok := equalFileContent(fn0, fn1)
	if false == ok {
		t.FailNow()
	}
}

// randomDynIPPort returns a random IP port number for testing.
// use 'rand.Seed(time.Now().UTC().UnixNano())' before to get different numbers on each first call.
func randomDynIPPort() (s string) {
	min := 49152
	max := 65535
	s = fmt.Sprint(rand.Intn(max-min) + min)
	return
}

// checkDisplay is an object used for testing.
// It implements the lineWriter interface.
type checkDisplay struct {
	lines []string
}

// newCheckDisplay creates a Display. It provides a lineWriter.
func newCheckDisplay() *checkDisplay {
	p := &checkDisplay{}
	return p
}

// writeLine is the implemented lineWriter interface for checkDisplay.
// It appends written lines to the internal data.
func (p *checkDisplay) writeLine(line []string) {
	s := strings.Join(line, "")
	p.lines = append(p.lines, s)
}

func (p *checkDisplay) checkLines(t *testing.T, lines []string) {
	notEqualFail(t, p.lines, lines)
}

//
// test helper ///////////////////////////////////////////////////////////////////////

// equalFileContent returns true if contece is equal
func equalFileContent(fn0, fn1 string) bool {
	cmp := equalfile.New(nil, equalfile.Options{}) // compare using single mode
	ok, err := cmp.CompareFile(fn0, fn1)
	if nil != err {
		ok = false
	}
	return ok
}

// readLines reads all lines of a file into lines.
// If file ends with a newline the last line in lines is empty.
// Do not use with big files.
func readLines(filename string) (lines []string, err error) {
	content, err := ioutil.ReadFile(filename)
	stringContent := string(content)
	if runtime.GOOS == "windows" {
		stringContent = strings.ReplaceAll(stringContent, "\r\n", "\n")
	}
	lines = strings.Split(stringContent, "\n")
	return
}
