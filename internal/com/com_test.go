// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package com_test is a black-box test.
package com_test

import (
	"os"
	"testing"

	"github.com/rokath/trice/internal/com"
	"github.com/tj/assert"
)

func TestSerial(t *testing.T) {
	com.DataBits = 8
	com.Parity = "none"
	com.StopBits = "1"
	var verbose bool

	ss, _ := com.GetSerialPorts(os.Stdout)
	if len(ss) == 0 {
		return
	}

	pS := com.NewPort(os.Stdout, "noCOM", verbose)
	assert.False(t, pS.Open())

	verbose = true
	pS = com.NewPort(os.Stdout, "noCOM", verbose)
	assert.False(t, pS.Open())

	for i := range ss {
		port := ss[i]
		verbose = false
		pS = com.NewPort(os.Stdout, port, verbose)
		if pS.Open() {
			assert.Nil(t, pS.Close())
		}
		verbose = true
		pS = com.NewPort(os.Stdout, port, verbose)
		if pS.Open() {
			assert.Nil(t, pS.Close())
		}
	}
}
