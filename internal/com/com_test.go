// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package com_test is a blackbox test.
package com_test

import (
	"testing"

	"github.com/rokath/trice/internal/com"
	"github.com/tj/assert"
)

// todo: handle special cases: (may be with a kind of COM-port simulator)
//
//	trice s
//	Could not enumerate serial ports
//
//	trice s
//	No serial ports found!
//
// A normal case:
//	PS C:\repos\trice> trice s
//	Found port:  COM4
func Test1(t *testing.T) {
	ss, err := com.GetSerialPorts()
	assert.Nil(t, err)

	for i := range ss {
		port := ss[i]
		p := com.NewCOMPortGoBugSt(port)
		if p.Open() {
			assert.Nil(t, p.Close())
		}
	}
}
