// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
//                basti@blackoutcloud.de
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package com_test is a blackbox test.
package com_test

import (
	"testing"

	"github.com/rokath/trice/internal/com"
	"github.com/rokath/trice/internal/global"
	"github.com/rokath/trice/pkg/lib"
)

func Test1(t *testing.T) {
	ss, err := com.GetSerialPorts()
	lib.Equals(t, nil, err)
	//	PS C:\repos\trice> trice s
	//	Could not enumerate serial ports
	//	PS C:\repos\trice> trice s
	//	Found port:  COM4
	//	PS C:\repos\trice> trice s
	//	No serial ports found!
	for i := range ss {
		global.Source = ss[i]
		p := com.New(global.Source)
		if p.Open() {
			err = p.Close()
			lib.Equals(t, nil, err)
		}
	}
}
