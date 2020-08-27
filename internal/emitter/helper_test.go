// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"fmt"
	"io/ioutil"
	"log"
	"math/rand"
	"reflect"
	"runtime"
	"strings"
	"testing"
	"time"
)

// test helper ///////////////////////////////////////////////////////////////////////
//

func readLines(filename string) (lines []string, err error) {
	content, err := ioutil.ReadFile(filename)
	stringContent := string(content)
	if runtime.GOOS == "windows" {
		stringContent = strings.ReplaceAll(stringContent, "\r\n", "\n")
	}
	lines = strings.Split(stringContent, "\n")
	return
}

// equals fails the test if exp is not equal to act.
func equals(tb testing.TB, exp, act interface{}) {
	if !reflect.DeepEqual(exp, act) {
		// notice that we're using 1, so it will actually log the where
		// the error happened, 0 = this function, we don't want that.
		pc, fn, line, _ := runtime.Caller(1)
		log.Printf("[delta] in %s[%s:%d]", runtime.FuncForPC(pc).Name(), fn, line)
		log.Println("expect:", exp)
		log.Println("actual:", act)
		tb.FailNow()
	}
}

func randomDynIPPort() (s string) {
	rand.Seed(time.Now().UTC().UnixNano())
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

// equalStringSlices returns true when a == b
func equalStringSlices(a, b []string) bool {
	if len(a) != len(b) {
		fmt.Println(len(a), len(b))
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			fmt.Println(a[i])
			fmt.Println(b[i])
			return false
		}
	}
	return true
}

func (p *checkDisplay) checkLines(t *testing.T, lines []string) {
	//if false == reflect.DeepEqual(p.s, lines) {
	if false == equalStringSlices(p.lines, lines) {
		t.Fail()
	}
}

//
// test helper ///////////////////////////////////////////////////////////////////////
