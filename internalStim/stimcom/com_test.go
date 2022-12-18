// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package com_test is a black-box test.
package stimcom_test

import (
	"os"
	"testing"

	"github.com/rokath/trice/internalStim/com"
	"github.com/tj/assert"
)

func init() {
	com.Parity = "e"
}

// todo: handle special cases: (may be with a kind of COM-port simulator)
//
//	stim s
//	Could not enumerate serial ports
//
//	stim s
//	No serial ports found!
//
// A normal case:
//
//	stim s
//	Found port:  COM4
//	Found port:  COM7
func TestSerial(t *testing.T) {
	ss, _ := com.GetSerialPorts(os.Stdout)
	if len(ss) == 0 {
		return
	}

	var verbose bool

	verbose = false
	pS := com.NewPort(os.Stdout, "noCOM", verbose)
	assert.False(t, pS.Open())

	verbose = true
	pS = com.NewPort(os.Stdout, "noCOM", verbose)
	assert.False(t, pS.Open())

	for i := range ss {
		verbose = false
		pS = com.NewPort(os.Stdout, ss[i], verbose)
		if pS.Open() {
			assert.Nil(t, pS.Close())
		}
		verbose = true
		pS = com.NewPort(os.Stdout, ss[i], verbose)
		if pS.Open() {
			assert.Nil(t, pS.Close())
		}
	}
}
