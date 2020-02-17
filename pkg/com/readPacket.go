// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// All rights reserved.
// Use of this source code is governed by a
// license that can be found in the LICENSE file.

package com

import (
	"fmt"

	"github.com/rokath/trice/pkg/emit"
	"github.com/rokath/trice/pkg/id"
	"github.com/tarm/serial"
)

func readHeader(s *serial.Port) []byte {
	b := readAtLeastBytes(s, 8)
	for b[1] != 0x60 || // only one receiver now
		b[2] != 0x61 || // only one transmitter now
		b[0]^b[1]^b[2]^b[4]^b[5]^b[6]^b[7] != b[3] { // crc8 check
		fmt.Println("discarding", b[0])
		b = append(b[1:], readAtLeastBytes(s, 1)...) // try to sync
	}
	//fmt.Print(b)
	return b
}

// ReadEndless expects a pointer to a filled COM port configuration
func ReadEndless(s *serial.Port, l id.List, color string) {
	for {
		b := readHeader(s)
		if 0xeb == b[0] { // trice startbyte, no further data
			err := emit.Trace(b, l, color)
			if nil != err {
				fmt.Println("trace.Log error", err, b)
			}
		} else {
			fmt.Println("unknown header", b)
		}
		continue
	}
}
