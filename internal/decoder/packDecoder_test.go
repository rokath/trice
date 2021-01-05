// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"fmt"
	"testing"

	"github.com/rokath/trice/internal/receiver"
	"github.com/stretchr/testify/assert"
)

var (
	byteStreamPack0 = string([]byte{
		137, 171, 205, 239, // sync packet
		65, 171, 0, 16, 0,
	})

	byteStreamPack1 = string([]byte{
		137, 171, 205, 239,
		209, 56, 0, 2, 5, 4, 0, 0,
		137, 171, 205, 239,

		9, 223, 0, 4, 1, 127, 128, 255, //  TRICE8  %03x ->  001  07f  -80  -01
		247, 191, 0, 4, 1, 127, 128, 255, // TRICE8   %4d ->    1  127 -128   -1
		156, 127, 0, 4, 1, 127, 128, 255, // TRICE8   %4o ->    1  177 -200   -1
		33, 226, 0, 8, 0, 1, 127, 255, 128, 0, 255, 255, // TRICE16  %05x ->   00001   07fff   -8000   -0001
		233, 221, 0, 8, 0, 1, 127, 255, 128, 0, 255, 255, //  TRICE16   %6d ->       1   32767  -32768      -1
		96, 205, 0, 8, 0, 1, 127, 255, 128, 0, 255, 255, //  TRICE16   %7o ->       1   77777 -100000      -1

	})

	byteStreamPack2 = string([]byte{
		//0, 18, 0, 0, // ""
		0, 18, 0, 1, 10, 0, 0, 0, // "\n"
		0, 18, 0, 2, 49, 10, 0, 0, // "1\n"
		0, 18, 0, 3, 49, 50, 10, 0, // "12\n"
		0, 18, 0, 4, 49, 50, 51, 10,
		0, 18, 0, 5, 49, 50, 51, 52, 10, 0, 0, 0,
		0, 18, 0, 6, 49, 50, 51, 52, 53, 10, 0, 0,
		0, 18, 0, 7, 49, 50, 51, 52, 53, 54, 10, 0,
		//0, 18, 0, 0, // "" // to do
		0, 18, 0, 8, 49, 50, 51, 52, 53, 54, 55, 10,
		0, 18, 0, 9, 49, 50, 51, 52, 53, 54, 55, 56, 10, 0, 0, 0,
		0, 18, 0, 10, 49, 50, 51, 52, 53, 54, 55, 56, 57, 10, 0, 0, // "123456789\n"

	})
)

func TestPack0(t *testing.T) { // test incomplete trice message

	// rc is created ReadCloser
	rc, err := receiver.NewReader("BUFFER", byteStreamPack0)
	if err != nil {
		t.Fail()
	}

	list, err := UnmarshalTriceIDList([]byte(til))
	if err != nil {
		t.Fail()
	}

	p := NewPackDecoder(list, rc, bigEndian) // p is a new decoder instance

	// ss := make([]string, 100)
	// n, _ := p.StringsRead(ss)
	// ss = ss[:n]
	// act := fmt.Sprintln(ss)

	b := make([]byte, 4096)
	n, _ := p.Read(b)
	act := fmt.Sprintln(string(b[:n]))
	exp := "[]\n"
	assert.Equal(t, exp, act)
}

func _TestPack1(t *testing.T) { // to do: repair

	// rc is created ReadCloser
	rc, err := receiver.NewReader("BUFFER", byteStreamPack1)
	if err != nil {
		t.Fail()
	}

	list, err := UnmarshalTriceIDList([]byte(til))
	if err != nil {
		t.Fail()
	}

	p := NewPackDecoder(list, rc, bigEndian) // p is a new decoder instance

	//ss := make([]string, 100)
	//n, _ := p.StringsRead(ss)
	//ss = ss[:n]
	//act := fmt.Sprintln(ss)

	b := make([]byte, 4096)
	n, _ := p.Read(b)
	act := fmt.Sprintln(string(b[:n]))
	exp := "[ATT:attention   message, SysTick is   1284\\n tst:TRICE8  %03x ->  001  07f  -80  -01\\n tst:TRICE8   %4d ->    1  127 -128   -1\\n tst:TRICE8   %4o ->    1  177 -200   -1\\n tst:TRICE16  %05x ->   00001   07fff   -8000   -0001\\n tst:TRICE16   %6d ->       1   32767  -32768      -1\\n tst:TRICE16   %7o ->       1   77777 -100000      -1\\n]\n"
	assert.Equal(t, exp, act)

	assert.Equal(t, exp, act)
}

func _TestPack2(t *testing.T) {

	// rc is created ReadCloser
	rc, err := receiver.NewReader("BUFFER", byteStreamPack2)
	if err != nil {
		t.Fail()
	}

	list, err := UnmarshalTriceIDList([]byte(til))
	if err != nil {
		t.Fail()
	}

	p := NewPackDecoder(list, rc, bigEndian) // p is a new decoder instance

	//ss := make([]string, 100)
	//n, _ := p.StringsRead(ss)
	//ss = ss[:n]
	//act := fmt.Sprintln(ss)
	//exp := "[\n 1\n 12\n 123\n 1234\n 12345\n 123456\n 1234567\n 12345678\n 123456789\n]\n"

	b := make([]byte, 4096)
	n, _ := p.Read(b)
	act := fmt.Sprintln(string(b[:n]))
	exp := "[ATT:attention   message, SysTick is   1284\\n tst:TRICE8  %03x ->  001  07f  -80  -01\\n tst:TRICE8   %4d ->    1  127 -128   -1\\n tst:TRICE8   %4o ->    1  177 -200   -1\\n tst:TRICE16  %05x ->   00001   07fff   -8000   -0001\\n tst:TRICE16   %6d ->       1   32767  -32768      -1\\n tst:TRICE16   %7o ->       1   77777 -100000      -1\\n]\n"
	assert.Equal(t, exp, act)

	assert.Equal(t, exp, act)
}
