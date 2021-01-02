// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"

	"github.com/rokath/trice/internal/receiver"
)

// byteStreamBare are the raw read input bytes for bare format tests
var byteStreamBare string = string([]byte{
	137, 171, 205, 239, // sync
	0, 0, 255, 255,
	24, 240, 255, 255, // TRICE32_1 -1
	0, 0, 255, 255,
	0, 0, 255, 255,
	0, 0, 255, 255,
	137, 171, 205, 239, // sync
	44, 241, 255, 254, // TRICE32_2 -1 -2
	0, 0, 255, 255,
	0, 0, 255, 255,
	0, 0, 255, 255,
	0, 0, 255, 254,
	0, 0, 255, 255,
	219, 125, 255, 253, // TRICE32_3 -1 -2 -3
	0, 0, 255, 255,
	0, 0, 255, 255,
	0, 0, 255, 255,
	137, 171, 205, 239, // sync
	0, 0, 255, 254,
	0, 0, 255, 255,
	0, 0, 255, 253,
	0, 0, 255, 255,
	142, 221, 255, 252, // TRICE32_4 -1 -2 -3 -4
	0, 0, 255, 255,
	0, 0, 255, 255,
	0, 0, 255, 255,
	168, 163, 255, 255, // TRICE64_1 -1
	0, 0, 255, 255,
	0, 0, 255, 255,
	0, 0, 255, 255,
	0, 0, 255, 255,
	0, 0, 255, 255,
	0, 0, 255, 255,
	0, 0, 255, 255,
	32, 210, 255, 254, // TRICE64_2 -1 -2
	137, 171, 205, 239, // sync
})

func TestBare(t *testing.T) {

	// rc is created ReadCloser
	rc, err := receiver.NewReader("BUFFER", byteStreamBare)
	if err != nil {
		t.Fail()
	}

	list, err := UnmarshalTriceIDList([]byte(til))
	if err != nil {
		t.Fail()
	}

	p := NewBareDecoder(list, rc, littleEndian) // p is a new decoder instance

	ss := make([]string, 100)
	n, err := p.StringsRead(ss)
	if err != nil {
		t.Fail()
	}
	ss = ss[:n]
	act := fmt.Sprintln(ss)
	exp := "[tst:TRICE32_1 -1\\n tst:TRICE32_2 -1 -2\\n tst:TRICE32_3 -1 -2 -3\\n tst:TRICE32_4 -1 -2 -3 -4\\n tst:TRICE64_1 -1\\n tst:TRICE64_2 -1 -2\\n]\n"
	assert.Equal(t, exp, act)
}
