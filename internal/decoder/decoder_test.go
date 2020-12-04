// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder_test

import (
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"

	"github.com/rokath/trice/internal/decoder"
)

var (
	// til is the trace id list content for tests
	til = `[
	{
		"id": 28944,
		"fmtType": "TRICE32_4",
		"fmtStrg": "tst:TRICE32_4 %%10d -\u003e     %10d     %10d     %10d    %10x\\n",
		"created": 1601240704,
		"removed": 0
	},
	{
		"id": 19049,
		"fmtType": "TRICE64_1",
		"fmtStrg": "att:64bit %#b\\n",
		"created": 1601240704,
		"removed": 0
	},
	{
		"id": 41267,
		"fmtType": "TRICE8_1",
		"fmtStrg": "tst:TRICE8_1 %d\\n",
		"created": 1601240704,
		"removed": 0
	},
	{
		"id": 45495,
		"fmtType": "TRICE64_2",
		"fmtStrg": "MSG: triceEscFifoMaxDepth = %d, index = %d\\n",
		"created": 1601240705,
		"removed": 0
	}
]`

	// byteStream are the raw read input bytes for tests
	byteStream string = string([]byte{
		236, 228, 113, 16, 0, 0, 0, 1, 127, 255, 255, 255, 128, 0, 0, 0, 255, 255, 255, 255, // TRICE32_4 %10d ->              1     2147483647     -2147483648            -1
		236, 227, 74, 105, 17, 34, 51, 68, 85, 102, 119, 136, // 64bit 0b1000100100010001100110100010001010101011001100111011110001000
		236, 228, 177, 183, 0, 0, 0, 0, 0, 0, 0, 129, 0, 0, 0, 0, 0, 0, 0, 3, // MSG: triceEscFifoMaxDepth = 129, index = 3
		236, 224, 161, 51, 255, // TRICE8_1 -1
		236, 228, 177, 183, 0, 0, 0, 0, 0, 0, 0, 98, 0, 0, 0, 0, 0, 0, 0, 2, // MSG: triceEscFifoMaxDepth = 98, index = 2
	})
)

func TestEsc(t *testing.T) {

	// rc is created ReadCloser
	rc, err := decoder.NewInputPort("BUFFER", byteStream)
	if err != nil {
		t.Fail()
	}

	list, err := decoder.UnmarshalTriceIDList([]byte(til))
	if err != nil {
		t.Fail()
	}

	p := decoder.NewEsc(list, rc) // p is a new esc decoder instance

	ss := make([]string, 100)
	n, err := p.StringsRead(ss)
	if err != nil {
		t.Fail()
	}
	ss = ss[:n]
	act := fmt.Sprintln(ss)
	exp := "[tst:TRICE32_4 %10d ->              1     2147483647     -2147483648            -1\\n att:64bit 0b1000100100010001100110100010001010101011001100111011110001000\\n MSG: triceEscFifoMaxDepth = 129, index = 3\\n tst:TRICE8_1 -1\\n MSG: triceEscFifoMaxDepth = 98, index = 2\\n]\n"
	assert.Equal(t, exp, act)
}

func TestBare(t *testing.T) {

	// rc is created ReadCloser
	rc, err := decoder.NewInputPort("BUFFER", byteStream)
	if err != nil {
		t.Fail()
	}

	list, err := decoder.UnmarshalTriceIDList([]byte(til))
	if err != nil {
		t.Fail()
	}

	p := decoder.NewBare(list, rc) // p is a new esc decoder instance

	ss := make([]string, 100)
	n, err := p.StringsRead(ss)
	if err != nil {
		t.Fail()
	}
	ss = ss[:n]
	act := fmt.Sprintln(ss)
	exp := "[tst:TRICE32_4 %10d ->              1     2147483647     -2147483648            -1\\n att:64bit 0b1000100100010001100110100010001010101011001100111011110001000\\n MSG: triceEscFifoMaxDepth = 129, index = 3\\n tst:TRICE8_1 -1\\n MSG: triceEscFifoMaxDepth = 98, index = 2\\n]\n"
	assert.Equal(t, exp, act)
}
