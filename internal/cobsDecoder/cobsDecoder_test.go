// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package cobsDecoder

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"strings"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

// doTableTest is the universal decoder test sequence.
func doCOBSTableTest(t *testing.T, out io.Writer, f decoder.New, endianness bool, teTa decoder.TestTable) {
	var (
		// til is the trace id list content for test
		idl = `{
			"48324": {
				"Type": "TRICE16",
				"Strg": "MSG: START select = %d, TriceDepthMax =%4u\\n"
			},
			"58755": {
				"Type": "TRICE32_1",
				"Strg": "rd:TRICE32_1 line %d (%%d)\\n"
			},
			"53709": {
				"Type": "TRICE16",
				"Strg": "MSG: STOP  select = %d, TriceDepthMax =%4u\\n"
			}
		}
	`
	)
	ilu := make(id.TriceIDLookUp) // empty
	//li := make(id.TriceIDLookUpLI) // empty
	luM := new(sync.RWMutex)
	assert.Nil(t, ilu.FromJSON([]byte(idl)))
	ilu.AddFmtCount(os.Stdout)
	buf := make([]byte, decoder.DefaultSize)
	dec := f(out, ilu, luM, nil, nil, endianness) // a new decoder instance
	for _, x := range teTa {
		in := ioutil.NopCloser(bytes.NewBuffer(x.In))
		dec.SetInput(in)
		lineStart := true
		var err error
		var n int
		var act string
		for nil == err {
			n, err = dec.Read(buf)
			if n == 0 {
				break
			}
			if decoder.ShowID != "" && lineStart {
				act += fmt.Sprintf(decoder.ShowID, decoder.LastTriceID)
			}
			act += fmt.Sprint(string(buf[:n]))
			lineStart = false
		}
		act = strings.TrimSuffix(act, "\\n")
		act = strings.TrimSuffix(act, "\n")
		assert.Equal(t, x.Exp, act)
	}
}

func TestCOBS(t *testing.T) {
	tt := decoder.TestTable{ // little endian
		{[]byte{2, 1, 1, 1, 3, 208, 7, 1, 5, 192, 1, 196, 188, 1, 1, 1, 1, 0}, `MSG: START select = 0, TriceDepthMax =   0`},
		{[]byte{2, 1, 1, 1, 3, 209, 7, 1, 5, 193, 1, 205, 209, 1, 2, 28, 1, 0}, `MSG: STOP  select = 0, TriceDepthMax =  28`},
		{[]byte{0x02, 0x03, 0x01, 0x01, 0x02, 0x13, 0x0f, 0x38, 0xcb, 0x11, 0x11, 0x11, 0x11, 0xc0, 0x01, 0x83, 0xe5, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00}, `rd:TRICE32_1 line -1 (%d)`},
	}
	var out bytes.Buffer
	doCOBSTableTest(t, &out, New, decoder.LittleEndian, tt)
	assert.Equal(t, "", out.String())
}
