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
		"id": 43171,
		"fmtType": "TRICE64_1",
		"fmtStrg": "tst:TRICE64_1 %d\\n",
		"created": 1601240704,
		"removed": 0
	},
	{
		"id": 8402,
		"fmtType": "TRICE64_2",
		"fmtStrg": "tst:TRICE64_2 %d %d\\n",
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

	// byteStreamEsc are the raw read input bytes for esc format tests
	byteStreamEsc string = string([]byte{
		236, 228, 113, 16, 0, 0, 0, 1, 127, 255, 255, 255, 128, 0, 0, 0, 255, 255, 255, 255, // TRICE32_4 %10d ->              1     2147483647     -2147483648            -1
		236, 227, 74, 105, 17, 34, 51, 68, 85, 102, 119, 136, // 64bit 0b1000100100010001100110100010001010101011001100111011110001000
		236, 228, 177, 183, 0, 0, 0, 0, 0, 0, 0, 129, 0, 0, 0, 0, 0, 0, 0, 3, // MSG: triceEscFifoMaxDepth = 129, index = 3
		236, 224, 161, 51, 255, // TRICE8_1 -1
		236, 228, 177, 183, 0, 0, 0, 0, 0, 0, 0, 98, 0, 0, 0, 0, 0, 0, 0, 2, // MSG: triceEscFifoMaxDepth = 98, index = 2
	})

	// byteStreamBare are the raw read input bytes for bare format tests
	byteStreamBare string = string([]byte{
		//0, 0, 0, 1, 0, 0, 127, 255, 0, 0, 128, 0, 33, 226, 255, 255, 0, 0, 0, 1, 0, 0, 127, 255, 0, 0, 128, 0, 233, 221, 255, 255, 0, 0, 0, 1, 0, 0, 127,
		//255, 0, 0, 128, 0, 96, 205, 255, 255, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 127, 255, 0, 0, 0, 0, 0, 0, 128, 0, 0, 0, 255, 255, 65, 171, 255, 255,
		0, 0, 0, 1,
		0, 0, 0, 0,
		0, 0, 255, 255,
		0, 0, 127, 255,
		0, 0, 0, 0,
		0, 0, 128, 0,
		0, 0, 255, 255,
		113, 16, 255, 255,

		0, 0, 119, 136,
		0, 0, 85, 102,
		0, 0, 51, 68,
		74, 105, 17, 34,

		//137, 171, 205, 239, // sync
		0, 0, 255, 255,
		0, 0, 255, 255,
		0, 0, 255, 255,
		//137, 171, 205, 239, // sync
		168, 163, 255, 255, // TRICE64_1 -1

		0, 0, 255, 255,
		0, 0, 255, 255,
		0, 0, 255, 255,
		0, 0, 255, 255,
		0, 0, 255, 254,
		0, 0, 255, 255,
		0, 0, 255, 255,
		32, 210, 255, 255, // TRICE64_2 -1 -2
		137, 171, // sync start

	})
)

func TestEsc(t *testing.T) {

	// rc is created ReadCloser
	rc, err := decoder.NewInputPort("BUFFER", byteStreamEsc)
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
	rc, err := decoder.NewInputPort("BUFFER", byteStreamBare)
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

/* -e bare
Dec  7 22:16:05.322498  JLINK:  TRICE32_1 -1
Dec  7 22:16:05.323466  JLINK:  TRICE32_2 -1 -2
Dec  7 22:16:05.324489  JLINK:  TRICE32_3 -1 -2 -3
Dec  7 22:16:05.324489  JLINK:  TRICE32_4 -1 -2 -3 -4
2020/12/07 22:16:05 input bytes: <nil> 48 [0 0 255 255 0 0 255 255 0 0 255 255 168 163 255 255 0 0 255 255 0 0 255 255 0 0 255 255 0 0 255 255 0 0 255 254 0 0 255 255 0 0 255 255 32 210 255 255]
Dec  7 22:16:05.422047  JLINK:  TRICE64_1 -1
Dec  7 22:16:05.423014  JLINK:  TRICE64_2 -1 -2
*/
/*
-e bare2
Dec  7 22:23:17.503427  JLINK:  TRICE16_1 -1
Dec  7 22:23:17.503427  JLINK:  TRICE16_2 -1 -2
Dec  7 22:23:17.504425  JLINK:  TRICE16_3 -1 -2 -3
Dec  7 22:23:17.504425  JLINK:  TRICE16_4 -1 -2 -3 -4
2020/12/07 22:23:17 input bytes: <nil> 80 [0 0 255 255 24 240 255 255 0 0 255 255 0 0 255 255 0 0 255 254 44 241 255 255 0 0 255 255 0 0 255 255 0 0 255 254 0 0 255 255 0 0 255 253 219 125 255 255 0 0 255 255 0 0 255 255 0 0 255 254 0 0 255 255 0 0 255 253 0 0 255 255 0 0 255 252 142 221 255 255]
Dec  7 22:23:17.599426  JLINK:  TRICE32_1 -1
Dec  7 22:23:17.600424  JLINK:  TRICE32_2 -1 -65537
Dec  7 22:23:17.600424  JLINK:  TRICE32_3 -1 -65537 -131073
Dec  7 22:23:17.600424  JLINK:  TRICE32_4 -1 -65537 -131073 -196609
2020/12/07 22:23:17 input bytes: <nil> 20 [0 0 255 255 0 0 255 255 0 0 255 255 168 163 255 255 0 0 255 255]
Dec  7 22:23:17.705425  JLINK:  TRICE64_1 -1
2020/12/07 22:23:17 input bytes: <nil> 28 [0 0 255 255 0 0 255 255 0 0 255 255 0 0 255 254 0 0 255 255 0 0 255 255 32 210 255 255]
Dec  7 22:23:17.708423  JLINK:  TRICE64_2 -1 -281474976710657
*/
