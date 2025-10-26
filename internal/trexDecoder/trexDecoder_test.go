// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package trexDecoder

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"strings"
	"sync"
	"testing"

	"github.com/rokath/trice/internal/decoder"
	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
	"github.com/tj/assert"
)

// newF abstracts the function type for a new decoder.
type newF func(out io.Writer, sw *emitter.TriceLineComposer, lut id.TriceIDLookUp, m *sync.RWMutex, li id.TriceIDLookUpLI, in io.Reader, endian bool) decoder.Decoder

// doTableTest is the universal decoder test sequence.
func doTableTest(t *testing.T, out io.Writer, f newF, endianness bool, teTa decoder.TestTable) {
	var (
		// til is the trace id list content for test
		idl = `{
			"3713": {
				"Type": "TRICE16",
				"Strg": "MSG: 💚 START select = %d\\n"
			},
			"935": {
				"Type": "TRICE",
				"Strg": "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\\n"
			}
		}
	`
	)
	ilu := make(id.TriceIDLookUp)  // empty
	li := make(id.TriceIDLookUpLI) // empty
	luM := new(sync.RWMutex)
	assert.Nil(t, ilu.FromJSON([]byte(idl)))
	ilu.AddFmtCount(os.Stdout)
	buf := make([]byte, decoder.DefaultSize)
	var sw emitter.TriceLineComposer
	dec := f(out, &sw, ilu, luM, li, nil, endianness) // a new decoder instance
	for _, x := range teTa {
		in := io.NopCloser(bytes.NewBuffer(x.In))
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

// TestTREX takes byte sequence in tt as TCOBSv1 framed trice and converts it to the appropriate string in the tt.
// For translation is uses the idl inside doTableTest. The cycle counter starts with 0xC0 to avoid cycle error messages.
func TestTREX(t *testing.T) {
	tt := decoder.TestTable{ // little endian
		//tcobs:idLo  idHi  tsLo  tsHi  cycle count vLo   vHi   sigil delim
		{[]byte{0x81, 0x8e, 0x09, 0x23, 0xc0, 0x02, 0xb8, 0x01, 0xa4, 0x00}, `MSG: 💚 START select = 440`},
		{[]byte{0xa7, 0x83, 0x1b, 0x23, 0xc1, 0x10, 0x5c, 0x63, 0x80, 0x61, 0x50, 0x05, 0x62, 0x08, 0x41, 0x00}, `MSG:triceFifoDepthMax = 92 of max 128, triceStreamBufferDepthMax = 1360 of max 2048`},
	}
	decoder.PackageFraming = "TCOBSv1"
	var out bytes.Buffer
	doTableTest(t, &out, New, decoder.LittleEndian, tt)
	assert.Equal(t, "", out.String())
}
