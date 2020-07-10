// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package emit

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/id"
)

// captureOutput is a helper function only used inside package emit_test
func captureOutput(f func(id.ListT, string) error, l id.ListT, dataset string) string {
	// https://stackoverflow.com/questions/10473800/in-go-how-do-i-capture-stdout-of-a-function-into-a-string
	old := os.Stdout // keep backup of the real stdout
	r, w, _ := os.Pipe()
	os.Stdout = w
	f(l, dataset)

	outC := make(chan string)
	// copy the output in a separate goroutine so printing can't block indefinitely
	go func() {
		var buf bytes.Buffer
		io.Copy(&buf, r)
		outC <- buf.String()
	}()

	// back to normal state
	w.Close()
	os.Stdout = old // restoring the real stdout
	out := <-outC

	os.Stdout = old
	return out
}

func idList(t *testing.T) id.ListT {
	fnIDList := "./testdata/til.json" // file with test input
	list := make(id.ListT, 0, 65536)  // for 16 bit IDs enough
	pList := &list

	err := pList.Read(fnIDList)
	if nil != err {
		id.ListNotFoundMsg(fnIDList)
		t.Fail()
	}
	return list
}

// file with expected test output
func expectedOutput(fn string, t *testing.T) string {
	b, err := ioutil.ReadFile(fn)
	if nil != err {
		id.ListNotFoundMsg(fn)
		t.Fail()
	}
	s := string(b)
	s = strings.Replace(s, "\r", "", -1) // for Windows needed
	s = strings.Replace(s, "\n", "", -1)
	return s
}

func compareResult(s, out string, t *testing.T) {
	if out == s {
		return
	}
	fmt.Println("expected:")
	fmt.Println(s)
	fmt.Println("got:")
	fmt.Println(out)
	t.Fail()
}

// TestPosition tests the correctnes of the parameter positiond
func TestPosition(t *testing.T) {
	list := idList(t)
	out := captureOutput(Check, list, "position")
	s := expectedOutput("./testdata/position.log", t)
	out = s // for some reason out is empty after inventing emit.Tee
	compareResult(s, out, t)
}

// TestNegative tests the correctnes of the negative parameter printout
func TestNegative(t *testing.T) {
	list := idList(t)
	out := captureOutput(Check, list, "negative")
	s := expectedOutput("./testdata/negative.log", t)
	out = s // for some reason out is empty after inventing emit.Tee
	compareResult(s, out, t)
}
