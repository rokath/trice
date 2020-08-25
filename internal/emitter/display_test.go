// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"os"
	"testing"
	"time"

	"github.com/rokath/trice/pkg/lib"
)

func ExampleLocalDisplay() {
	p := newLocalDisplay()
	l1 := []string{"This is ", "the 1st ", "line"}
	l2 := []string{"This is ", "the 2nd ", "line"}
	p.writeLine(l1)
	p.writeLine(l2)
	// Output:
	// This is the 1st line
	// This is the 2nd line
}

func TestRemoteDisplay(t *testing.T) {
	// prepare
	afn := "testdata/actRemote.log"
	efn := "testdata/expRemote.log"
	os.Remove(afn)
	p := newRemoteDisplay("localhost", "65497", "trice.exe", "-logfile "+afn)
	l1 := []string{"This is ", "the 1st ", "line"}
	l2 := []string{"This is ", "the 2nd ", "line"}
	p.writeLine(l1)
	p.writeLine(l2)
	p.stopServer(0)
	time.Sleep(100 * time.Millisecond)
	lib.EqualTextFiles(t, afn, efn)
	os.Remove(afn)
}
