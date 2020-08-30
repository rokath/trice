// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"os"
	"testing"
	"time"
)

func TestRemoteDisplay(t *testing.T) {
	// prepare
	afn := "testdata/actRemote.log"
	efn := "testdata/expRemote.log"
	os.Remove(afn)
	exec := "trice.exe" // todo: os.Args[0] does not work!
	ipp := randomDynIPPort()
	p := NewRemoteDisplay(exec, "-logfile "+afn, "localhost", ipp)
	l1 := []string{"This is ", "the 1st ", "line"}
	l2 := []string{"This is ", "the 2nd ", "line"}
	p.writeLine(l1)
	p.writeLine(l2)
	p.stopServer(0)
	time.Sleep(100 * time.Millisecond)
	expLines, expErr := readLines(efn)
	actLines, actErr := readLines(afn)
	assertEqual(t, nil, expErr)
	assertEqual(t, nil, actErr)
	assertEqual(t, 9, len(expLines))
	assertEqual(t, 9, len(actLines))
	assertEqual(t, expLines[1:], actLines[1:])
	assertNil(t, os.Remove(afn))
}
