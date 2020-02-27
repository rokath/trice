// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package com

import (
	"fmt"

	"github.com/rokath/trice/pkg/emit"
	"github.com/rokath/trice/pkg/id"
	"github.com/tarm/serial"
)

var locAddr = byte(0x60) // local address
var remAddr = byte(0x60) // remote address
var toMs = 1000

func readHeader(s *serial.Port) ([]byte, error) {
	b, err := readAtLeastBytes(s, 8, toMs)
	if nil != err {
		return b, err
	}
	for b[1] != remAddr ||
		b[2] != locAddr ||
		b[0]^b[1]^b[2]^b[4]^b[5]^b[6]^b[7] != b[3] { // crc8 check
		fmt.Println("discarding", b[0])
		x, err := readAtLeastBytes(s, 1, toMs)
		if nil != err {
			return b, err
		}
		b = append(b[1:], x...) // try to sync
	}
	//fmt.Print(b)
	return b, nil
}

// ReadEndless expects a pointer to a filled COM port configuration
func ReadEndless(s *serial.Port, l id.List, color string) {
	for {
		b, err := readHeader(s)
		if nil != err {
			//fmt.Println(err)
			continue
		}
		if 0xeb == b[0] { // traceLog startbyte, no further data
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
