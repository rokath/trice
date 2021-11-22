// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package com_test is a blackbox test.
package com_test

import (
	"os"
	"testing"

	"github.com/rokath/trice/internal/com"
	"github.com/tj/assert"
)

// To do: handle special cases:
//
//	PS C:\repos\trice> trice s
//	Could not enumerate serial ports
//	PS C:\repos\trice> trice s
//	No serial ports found!
//
// A normal case:
//	PS C:\repos\trice> trice s
//	Found port:  COM4
func TestSerial(t *testing.T) {
	ss, err := com.GetSerialPorts(os.Stdout)
	assert.Nil(t, err)
	var verbose bool

	verbose = false
	pS := com.NewCOMPortGoBugSt(os.Stdout, verbose, "noCOM")
	assert.False(t, pS.Open())

	verbose = true
	pS = com.NewCOMPortGoBugSt(os.Stdout, verbose, "noCOM")
	assert.False(t, pS.Open())

	for i := range ss {
		port := ss[i]
		verbose = false
		pS = com.NewCOMPortGoBugSt(os.Stdout, verbose, port)
		if pS.Open() {
			//b := make([]byte, 1)
			//_, _ = p.Read(b)
			assert.Nil(t, pS.Close())
		}
		verbose = true
		pS = com.NewCOMPortGoBugSt(os.Stdout, verbose, port)
		if pS.Open() {
			//b := make([]byte, 1)
			//_, _ = p.Read(b)
			assert.Nil(t, pS.Close())
		}
	}

	verbose = false
	pT := com.NewCOMPortTarm(os.Stdout, verbose, "noCOM")
	assert.False(t, pT.Open())

	verbose = true
	pT = com.NewCOMPortTarm(os.Stdout, verbose, "noCOM")
	assert.False(t, pT.Open())

	for i := range ss {
		port := ss[i]
		verbose = false
		pT = com.NewCOMPortTarm(os.Stdout, verbose, port)
		if pT.Open() {
			//b := make([]byte, 1)
			//_, _ = p.Read(b)
			assert.Nil(t, pT.Close())
		}
		verbose = true
		pT = com.NewCOMPortTarm(os.Stdout, verbose, port)
		if pT.Open() {
			//b := make([]byte, 1)
			//_, _ = p.Read(b)
			assert.Nil(t, pT.Close())
		}
	}
}
