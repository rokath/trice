// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// whitebox test for package emitter.
package emitter

import (
	"fmt"
	"github.com/rokath/trice/pkg/msg"
	"math/rand"
	"os"
	"testing"
	"time"

	"github.com/stretchr/testify/assert"
)

func TestRemoteDisplay(t *testing.T) {
	// prepare
	afn := "testdata/actRemote.log"
	efn := "testdata/expRemote.log"
	msg.InfoOnErr("", os.Remove(afn))
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
	assert.Equal(t, nil, expErr)
	assert.Equal(t, nil, actErr)
	assert.Equal(t, 9, len(expLines))
	assert.Equal(t, 9, len(actLines))
	assert.Equal(t, expLines[1:], actLines[1:])
	assert.Nil(t, os.Remove(afn))
}

func randomDynIPPort() (s string) {
	rand.Seed(time.Now().UTC().UnixNano())
	min := 49152
	max := 65535
	s = fmt.Sprint(rand.Intn(max-min) + min)
	return
}
